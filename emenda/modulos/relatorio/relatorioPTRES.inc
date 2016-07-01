<?php 

$where .= ( is_numeric($_POST['emecod']) ) ? " AND vede.emecod = '".$_POST['emecod']."'" : "";
$where .= ( is_numeric($_POST['autid']) ) ? " AND aut.autid = '".$_POST['autid']."'" : "";

$where .= ($_POST['gndcod']) ? " AND vede.gndcod = '".$_POST['gndcod']."'" : "";
$where .= ($_POST['mapcod']) ? " AND vede.mapcod = '".$_POST['mapcod']."'" : "";
$where .= ($_POST['foncod']) ? " AND vede.foncod = '".$_POST['foncod']."'" : "";

if( !empty($_POST['enbcnpj']) || !empty($_POST['enbnome']) ){
	$where1 = '';
	$where1 .= ($_POST['enbcnpj']) ? " AND ent.enbcnpj = '".str_replace( array(".","/","-"), "", $_POST['enbcnpj'] )."'" : "";
	$where1 .= ($_POST['enbnome']) ? " AND UPPER( removeacento(ent.enbnome) ) like '%".str_to_upper( removeAcentos($_POST['enbnome']) )."%'" : "";
	
	$where.="and 0 <> (SELECT count(1) FROM emenda.entidadebeneficiada ent 
						WHERE ent.enbstatus = 'A'	
							  and ptr.enbid = ent.enbid						
							  $where1)";
}

if (!empty($_POST["compiPTA"])){
	$where1 = '';
	$_POST["compiPTA"] == 'S'  ? $where1 = " <> " : $where1 = " = ";

	$where .= "and 0 $where1 (select count(pip.pliid)
							from monitora.pi_planointernoptres pip
							inner join  monitora.pi_planointerno pi ON pip.pliid = pi.pliid
							inner join monitora.ptres p ON p.ptrid = pip.ptrid
							where p.ptres = ptres.ptres)";
}
if (!empty($_POST["compiEmenda"])){
	$where1 = '';
	$_POST["compiEmenda"] == 'S'  ? $where1 = " <> " : $where1 = " = ";

	$where .= "and 0 $where1 (select count(exfid) from emenda.execucaofinanceira exf
							where exf.exfstatus = 'A'
							and exf.ptrid = ptr.ptrid
							and exf.pedid = ped.pedid)";
}

$where .= ($_POST['ptrid']) ? " AND ptr.ptrcod = ".str_replace(' ','',$_POST['ptrid']) : "";
$where .= ($_POST['iniid']) ? " AND pti.iniid = ".$_POST['iniid'] : "";

$where .= ($_POST['tpeid']) ? " AND tpe.tpeid = ".$_POST['tpeid'] : "";
$where .= ($_POST['ptres']) ? " AND ptres.ptres = '".$_POST['ptres']."'" : "";


if($_REQUEST["empenhado"] == "S"){
	$inner = "INNER JOIN emenda.execucaofinanceira ef on ptr.ptrid = ef.ptrid and ped.pedid = ef.pedid and ef.exfstatus = 'A' AND ef.exfnumempenhooriginal IS NOT NULL";
}else if ($_REQUEST["empenhado"] == "N"){
	$inner = "INNER JOIN emenda.execucaofinanceira ef on ptr.ptrid = ef.ptrid and ped.pedid = ef.pedid and ef.exfstatus = 'A' AND 
				ef.exfid in (select exfid from emenda.execucaofinanceira where ptrid = ptr.ptrid and pedid = ef.pedid  )";
}else if($_REQUEST["empenhado"] == ""){
	$inner ="";
}


$sqlRelatPTRES  = "SELECT 
			 vede.emecod::NUMERIC,
			 (CASE WHEN vede.emetipo = 'E' THEN 'Emenda' ELSE 'Complemento' END) as tipoemenda, 
			 case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome,
			 COALESCE(fup.fupfuncionalprogramatica, 'Dados Incompletos') as fupfuncionalprogramatica, 
			 fup.fupdsc,
			 vede.gndcod||' - '||gn.gnddsc as gndcod, 
			 vede.mapcod||' - '||map.mapdsc as mapcod,
			 vede.foncod||' - '||fon.fondsc as foncod, 
			 tpe.tpedsc,
			 vede.emdvalor, 
			 (select enbcnpj ||'-'|| enbnome from emenda.entidadebeneficiada where enbid = ptr.enbid) as entidade,
			 ptr.ptrcod ||'/'|| ptr.ptrexercicio as ptrcod,
			 sum(per.pervalor) as pervalor,
			 ini.ininome,
			 ptres.ptres,
			ptres.ptrdotacao,
			(select exf.plicod from emenda.execucaofinanceira exf
							where exf.exfstatus = 'A'
							and exf.ptrid = ptr.ptrid
							and exf.pedid = ped.pedid limit 1) as PIemenda,

			ptmi.pmcnumconveniosiafi as numconveniosiafi,
			ptr.ptrnumconvenio as numconveniofnde,
			ptr.ptrnumprocessoempenho as numprocesso,
			res.resdsc,
			CASE WHEN doc.esdid is not null THEN esd.esddsc ELSE 'Não Informado' END as situacao
			 
			FROM emenda.ptiniciativa pti
			inner join emenda.iniciativa ini on pti.iniid = ini.iniid
			inner join emenda.planotrabalho ptr on ptr.ptrid = pti.ptrid 
			 and ptrstatus = 'A'
			inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid
			 and pte.ptestatus = 'A'
			inner join emenda.ptiniesprecurso per on per.pteid = pte.pteid
			inner join emenda.ptemendadetalheentidade ped on ped.pedid = per.pedid
			inner join emenda.tipoensino tpe on pti.tpeid = tpe.tpeid
			inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
			inner join emenda.autor aut on aut.autid = vede.autid
			INNER JOIN workflow.documento doc ON ptr.docid = doc.docid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			left join emenda.v_funcionalprogramatica fup on fup.acaid = vede.acaid
			left join public.gnd gn on gn.gndcod = vede.gndcod
			 and gn.gndstatus = 'A'
			left join public.modalidadeaplicacao map on map.mapcod = vede.mapcod
			left join public.fonterecurso fon on fon.foncod = vede.foncod 
			 and fon.fonstatus = 'A'
			 
			left join monitora.ptres ptres on ptres.acaid = vede.acaid
			
			LEFT JOIN emenda.ptminutaconvenio ptmi ON ptmi.ptrid = ptr.ptrid AND ptmi.pmcstatus = 'A'
			LEFT JOIN emenda.responsavel res ON res.resid = ptr.resid
			 ".$inner."
			WHERE  vede.emeano = '".$_SESSION['exercicio']."'
			and 0 <> (select count(1) from emenda.analise where ptrid = pti.ptrid)
			".$where."
			GROUP BY 
			 tpe.tpedsc, 
			 vede.emecod,
			 aut.autnome,
			 vede.emerelator,
			 vede.gndcod, 
			 vede.mapcod, 
			 vede.foncod, 
			 fup.fupfuncionalprogramatica,
			 fup.fupdsc,
			 vede.emdvalor,
			 vede.emetipo,
			 gn.gnddsc,
			 map.mapdsc,
			 fon.fondsc,
			 ini.ininome,
			 pti.ptrid,
			 ptres.ptres,
			 ptr.ptrcod,
			 ptr.ptrexercicio,
			ptres.ptrdotacao,
			ptr.ptrid,
			ped.pedid,
			ptr.enbid,
			ptmi.pmcnumconveniosiafi,
			ptr.ptrnumconvenio,
			ptr.ptrnumprocessoempenho,
			res.resdsc,
			esd.esddsc,
			doc.esdid
			ORDER BY 
			 vede.emecod,
			pti.ptrid";

$arDados = $db->carregar( $sqlRelatPTRES );
$arDados = $arDados ? $arDados : array();

$registro = array();
foreach ($arDados as $key => $v) {
	
	$arPI = array();
	$tempi = '';
	if( !empty( $v['ptres'] ) ){
		$sql = "SELECT pi.plicod
				FROM 
					monitora.pi_planointernoptres pip
					inner join  monitora.pi_planointerno pi ON pip.pliid = pi.pliid
					inner join monitora.ptres p ON p.ptrid = pip.ptrid
				WHERE 
					p.ptres = '".$v['ptres']."'
					and pliano = '".$_SESSION['exercicio']."'";
		
		$arPI = $db->carregarColuna( $sql );
		
		if( $arPI )
			$tempi = implode( ', ', $arPI );
		else
			$tempi = 'Não'; 
	}
	
	$registro[$key] = array( "emecod" => $v['emecod'],
					   "tipoemenda" => $v['tipoemenda'],
					   "autnome" => $v['autnome'],
					   "fupfuncionalprogramatica" => $v['fupfuncionalprogramatica'],			
					   "fupdsc" => $v['fupdsc'],	
					   "gndcod" => $v['gndcod'],	
					   "mapcod" => $v['mapcod'],	
					   "foncod" => $v['foncod'],	
					   "tpedsc" => $v['tpedsc'],	
					   "emdvalor" => $v['emdvalor'],
					   "entidade" => $v['entidade'],
					   "ptrcod" => $v['ptrcod'],
					   "pervalor" => $v['pervalor'],
					   "ininome" => $v['ininome'],
					   "ptres" => $v['ptres'],
					   "ptrdotacao" => $v['ptrdotacao'],
					   "temPI" => $tempi,
					   "PIemenda" => $v['piemenda'],
					   "numconveniosiafi" => $v['numconveniosiafi'],
						"numconveniofnde" => $v['numconveniofnde'],
						"numprocesso" => $v['numprocesso'],
						"resdsc" => $v['resdsc'],
						"situacao" => $v['situacao'],
					  );
}

$cabecalho 	    = array("Código", "Tipo Emenda", "Autor", "Funcional", "Subtítulo", "GND", "Mod", "Fonte", "Tipo de Ensino", "Valor Emenda", "Entidade", "Nº PTA", "Valor PTA", "Iniciativa", "PTRES", "Dotação Autorizada PTRES", "PI no PPA", "PI no Emendas", 
						"Número convênio SIAFI", "Número do convênio FNDE", "Número do processo", "Responsável","Estado Atual"); 

if( $_POST['requisicao'] == 'exportar' ){
	ob_clean();
	$db->sql_to_excel($registro, 'relPTA', $cabecalho);
	exit();
}

include_once APPRAIZ . 'includes/workflow.php';
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
?>
<form action="" method="POST" id="formulario" name="formulario">
<input type="hidden" name="requisicao" id="requisicao" value="">
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tbody>
		<tr>
			<td width="15%" class="subtitulodireita"><b>Emenda N°:</b></td>
			<td colspan="3" width="35%">
				<?
					$emecod = $_REQUEST['emecod'];
					echo campo_texto('emecod', 'N', 'S', '', 27, 27, '[#]', '', '', '', 0, '', '','', "this.value=mascaraglobal('[#]',this.value);" );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Número do PTA:</b></td>
			<td colspan="3">
				<?
					$ptrid = $_REQUEST['ptrid']; 
					echo campo_texto( 'ptrid', 'N', 'S', '', 27, 27, '[#]', '', '', '', 0, '', '', '', "this.value=mascaraglobal('[#]',this.value);" );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Autor:</b></td>
			<td colspan="3">
				<?
					$autid = $_REQUEST['autid'];
					
					$sql = "SELECT 
							  autid as codigo,
							  autnome as descricao
							FROM 
							  emenda.autor
							WHERE 
							  autstatus = 'A'
							ORDER BY autnome";
					$db->monta_combo( "autid", $sql, 'S', 'Selecione...', '', '', '', '265','','autid');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>GND:</b></td>
			<td colspan="3">
				<?php 
					
					$gndcod = $_REQUEST["gndcod"];
				
					$sql = "SELECT DISTINCT 
								ed.gndcod as codigo,
								ed.gndcod as descricao
							FROM
								emenda.emendadetalhe ed
							INNER JOIN
								emenda.emenda ee ON ee.emeid = ed.emeid";
					
					$db->monta_combo("gndcod", $sql, "S", "Todos", '', '', '', '', 'N','gndcod');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Modalidade:</b></td>
			<td colspan="3">
				<?php 
					
					$mapcod = $_REQUEST["mapcod"];
				
					$sql = "SELECT DISTINCT 
								ed.mapcod as codigo,
								ed.mapcod ||' - '|| mod.mapdsc as descricao
							FROM
								emenda.emendadetalhe ed
							INNER JOIN
								emenda.emenda ee ON ee.emeid = ed.emeid
                            INNER JOIN public.modalidadeaplicacao mod
                            	on mod.mapcod = ed.mapcod";
					
					$db->monta_combo("mapcod", $sql, "S", "Todos", '', '', '', '', 'N','mapcod');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Fonte:</b></td>
			<td colspan="3">
				<?php 
					
					$foncod = $_REQUEST["foncod"];
				
					$sql = "SELECT DISTINCT 
								ed.foncod as codigo,
								ed.foncod ||' - '|| fon.fondsc as descricao
							FROM
								emenda.emendadetalhe ed
							INNER JOIN
								emenda.emenda ee ON ee.emeid = ed.emeid
                            INNER JOIN public.fonterecurso fon
                            	on fon.foncod = ed.foncod";
					
					$db->monta_combo("foncod", $sql, "S", "Todos", '', '', '', '', 'N','foncod');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Tipo de Ensino:</b></td>
			<td colspan="3">
				<?
					$tpeid = $_REQUEST["tpeid"];
				
					$sql = "SELECT DISTINCT 
								tp.tpeid as codigo,
								tp.tpedsc as descricao
							FROM
								emenda.tipoensino tp
                            WHERE
                              	tp.tpestatus = 'A'
                            ORDER BY
                              	tp.tpedsc";
					
					$db->monta_combo("tpeid", $sql, "S", "Todos", '', '', '', '', 'N','tpeid');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Iniciativa:</b></td>
			<td colspan="3">
				<?php
					$iniid = $_REQUEST['iniid'];
					
					$sql = "SELECT DISTINCT
							    ini.iniid as codigo,
							    ini.ininome as descricao
							FROM 
							    emenda.ptiniciativa pti
							    inner join emenda.iniciativa ini
							        on ini.iniid = pti.iniid
							        --and ini.inistatus = 'A'
							    inner join emenda.planotrabalho ptr
							    	on ptr.ptrid = pti.ptrid
							    /*inner join emenda.responsavel res
							    	on res.resid = ptr.resid*/
							WHERE ptr.ptrstatus = 'A'
								and ptr.ptrexercicio = '".$_SESSION['exercicio']."'
							    --and res.resstatus = 'A'
							ORDER BY
							    descricao";
					
					$arIniciativa = $db->carregar( $sql );
					
					$arDados = array();
                                        if($arIniciativa){
                                            foreach ($arIniciativa as $chave => $v) {
                                                    $sql = "SELECT								
                                                                            res.resdsc
                                                                    FROM 
                                                                      emenda.responsavel res
                                                                      inner join emenda.iniciativaresponsavel inr
                                                                            on res.resid = inr.resid
                                                                        and inr.irestatus = 'A'
                                                                    WHERE
                                                                            inr.iniid = ".$v['codigo']."
                                                                    ORDER BY
                                                                            res.resdsc";

                                                    $resdsc = $db->carregarColuna( $sql );

                                                    array_push($arDados, array("codigo" => $v['codigo'],
                                                                                                     "descricao" => $v['codigo'] .' - '. $v['descricao']."(".implode( ', ', $resdsc ).")"
                                                                                            )
                                                                    );
                                            }
                                        }
					$db->monta_combo( "iniid", $arDados, 'S', 'Selecione...', '', '', '', '450', '', '', '', '', 'Lista de iniciativa(s) vinculada(s) ao plano de trabalho' );
					//$arIniciativa = $db->carregar($sql);
				?>
			</td>
		<tr>
		<tr>
		<td class="subtitulodireita"><b>Empenhado:</b></td>
		<td>
			<input type="radio" value="S" id="empenhado" name="empenhado" <? if($_REQUEST["empenhado"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="empenhado" name="empenhado" <? if($_REQUEST["empenhado"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="empenhado" name="empenhado"  <? if($_REQUEST["empenhado"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>
		<tr>
			<td class="subtitulodireita"><b>CNPJ:</b></td>
			<td>
				<?
					$enbcnpj = $_REQUEST['enbcnpj']; 
					echo campo_texto( 'enbcnpj', 'N', 'S', '', 27, 20, '', '', '', '', 0, '', "this.value=mascaraglobal('##.###.###/####-##',this.value);" );
				?>
			</td>
			<td class="subtitulodireita" width="15%"><b>Nome do Órgão ou Entidade:</b></td>
			<td width="35%">
				<?
					$entnome = $_REQUEST['enbnome']; 
					echo campo_texto('enbnome', 'N', 'S', '', 35, 50, '', '', '', '', 0, '', '', $entnome );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>PTRES:</b></td>
			<td>
				<?
					$ptres = $_REQUEST['ptres'];
					echo campo_texto('ptres', 'N', 'S', '', 27, 27, '[#]', '', '', '', 0, '', '','', "this.value=mascaraglobal('[#]',this.value);" );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Com PI vinculado no PPA:</b></td>
			<td>
				<input type="radio" value="S" id="compiPTA" name="compiPTA" <? if($_REQUEST["compiPTA"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="compiPTA" name="compiPTA" <? if($_REQUEST["compiPTA"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="compiPTA" name="compiPTA" <? if($_REQUEST["compiPTA"] == "") { echo "checked"; } ?> /> Todos
			</td>
			<td class="subtitulodireita"><b>Com PI vinculado no Emendas:</b></td>
			<td>
				<input type="radio" value="S" id="compiEmenda" name="compiEmenda" <? if($_REQUEST["compiEmenda"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="compiEmenda" name="compiEmenda" <? if($_REQUEST["compiEmenda"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="compiEmenda" name="compiEmenda"<? if($_REQUEST["compiEmenda"] == "") { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td colspan="5">					
				<input type="button" id="btPesquisar" value="Pesquisar" onclick="submetePesquisa();" />
				<input type="button" id="btLimpar" value="Limpar Campos" onclick="limparPesquisa();" />
				<input type="button" value="Exportar para Excel" id="btnExportar" onclick="exportarExcel();">
			</td>
		</tr>
	</tbody>
</table>
</form>
<?
print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
print '<tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" class="subtitulocentro">Relatório de PTRES</td></tr></table>';

/*$tamanho		= array('5%', '7%', '5%', '10%', '7%', '15%', '10%', '2%', '7%', '5%', '5%', '7%', '5%', '15%', '10%' );															
$alinhamento	= array('left', 'left', 'right', 'center', 'right', 'center', 'right', 'center', 'center', 'center', 'center' );*/													
//$db->monta_lista($registro, $cabecalho, 100000, 10, 'N', 'center');
$db->monta_lista_array($registro, $cabecalho, 100000, 10, 'N','Center');
?>
<script>
	function exportarExcel(){
		document.getElementById('requisicao').value = 'exportar';
		document.getElementById('formulario').submit();
	}
	function submetePesquisa(){
		document.getElementById('requisicao').value = 'pesquisar';
		document.getElementById('formulario').submit();
	}
	function limparPesquisa(){
		document.getElementById('requisicao').value = '';
		
		document.getElementsByName("emecod")[0].value = '';
		document.getElementsByName("ptrid")[0].value = '';
		document.getElementsByName("autid")[0].value = '';
		document.getElementsByName("gndcod")[0].value = '';
		document.getElementsByName("mapcod")[0].value = '';
		document.getElementsByName("foncod")[0].value = '';
		document.getElementsByName("tpeid")[0].value = '';
		document.getElementsByName("iniid")[0].value = '';
		document.getElementsByName("enbcnpj")[0].value = '';
		document.getElementsByName("enbnome")[0].value = '';
		document.getElementsByName("ptres")[0].value = '';
		
		document.getElementById('formulario').submit();
	}
</script>
<?php

if( $_REQUEST['reqs'] == 'regerarTermo' ){
	regerarTermo( $_REQUEST );
	exit();
}
if( $_REQUEST['reqs'] == 'formRegeraTermo' ){
	formRegeraTermo($_REQUEST);
	exit();
}

/* Removido devido solicitacao da Renilda em reunião dia 16/10/2015
if( $_REQUEST['requisicao'] == 'bloqueiar' ){
	$sql = "update obras.preobra set preblockreformulacao = 'S' where preid = {$_REQUEST['preid']}";
	$db->executar($sql);
	
	echo $db->commit();
	exit();
}*/

if( $_REQUEST['requisicao'] == 'carregaMunicipios' ){
	echo carregaMunicipios( $_REQUEST );
	exit();
}

if( $_REQUEST['requisicao'] == 'carregarListaObras' ){
	
	if($_REQUEST['filtro'] == 'bloqueio'){
		
		if( $_REQUEST['situacao'] == 'BL')
		{
			$filtro = " and preblockreformulacao = 'S'";
			$acoes = "'<center><span style=\"font-size:18px; cursor:pointer;\" title=\"Abrir Obra\" class=\"glyphicon glyphicon-question-sign\" onclick=\"abreObras('||p.preid||', '||p.preano||')\"></span>
							</center>' as acoes,";
		}
		else
		{
			$filtro = " and preblockreformulacao != 'S' and p.preid not in (SELECT preid FROM obras.preobraprorrogacao WHERE popdataprazoaprovado is null and popstatus = 'P' and popvalidacao = 'f')";
			$join = "";
			$acoes = "'<center>
                                            <span style=\"font-size:18px; cursor:pointer;\" title=\"Abrir Obra\" class=\"glyphicon glyphicon-question-sign\" onclick=\"abreObras('||p.preid||', '||p.preano||')\"></span>
				</center>'  as acoes,";
		}
	} else {
		$filtro = '';
		if( $_REQUEST['situacao'] == 'VO' ){
			$filtro = " and popdataprazoaprovado is null and popstatus = 'P' and popvalidacao = 'f' ";
			$join = " inner join obras.preobraprorrogacao pp on pp.preid = p.preid ";
			
			$acoes = "'<center><span style=\"font-size:18px; cursor:pointer;\" title=\"Abrir Obra\" class=\"glyphicon glyphicon-question-sign\" onclick=\"abreObras('||p.preid||', '||p.preano||')\"></span>
							<span style=\"font-size:18px; color:green; cursor:pointer;\" title=\"Validar a Prorrogação de Obras\" class=\"glyphicon glyphicon-check\" onclick=\"validarProrrogacao('||p.preid||', \''||v.processo||'\')\"></span>
							<span style=\"font-size:18px; color:red; cursor:pointer;\" title=\"Recusar a Prorrogação da Obras\" class=\"glyphicon glyphicon-share\" onclick=\"recusarProrrogacao('||p.preid||', \''||v.processo||'\')\"></span>
							<span style=\"font-size:18px; cursor:pointer;\" title=\"Editar a Prorrogação de Obras\" class=\"glyphicon glyphicon-edit\" onclick=\"editarProrrogacao('||p.preid||', \''||v.processo||'\')\"></span></center>' as acoes,";
		} 
		else if( $_REQUEST['situacao'] == 'BL')
		{
			$filtro = " and preblockreformulacao = 'S'";
			$acoes = "'<center><span style=\"font-size:18px; cursor:pointer;\" title=\"Abrir Obra\" class=\"glyphicon glyphicon-question-sign\" onclick=\"abreObras('||p.preid||', '||p.preano||')\"></span>
							</center>' as acoes,";
		}
		else {
			$acoes = "'<center><span style=\"font-size:18px; cursor:pointer;\" title=\"Abrir Obra\" class=\"glyphicon glyphicon-question-sign\" onclick=\"abreObras('||p.preid||', '||p.preano||')\"></span>
							<span style=\"font-size:18px; color:gray;\" cursor:pointer;\" title=\"Validar a Prorrogação de Obras\" class=\"glyphicon glyphicon-check\"></span>
							<span style=\"font-size:18px; color:gray;\" cursor:pointer;\" title=\"Recusar a Prorrogação da Obras\" class=\"glyphicon glyphicon-share\"></span>
							<span style=\"font-size:18px; color:gray;\" cursor:pointer;\" title=\"Editar a Prorrogação de Obras\" class=\"glyphicon glyphicon-edit\"></span></center>' as acoes,";
		}
	}
	
	$sql = "select
				$acoes
				p.preid,
			    p.obrid,
			    p.predescricao,
			    cast(p.prevalorobra as numeric(20,2)) as valorobra,
			    vlo.totalpago as valorpagoobra,
			    pt.ptodescricao,
			    res.res_estado,
			    str.strdsc as estado2,
			    p.preano,
				(SELECT popqtddiassolicitado FROM obras.preobraprorrogacao WHERE  preid = p.preid order by popid desc limit 1) as diasvalidado,
				((((100 - coalesce(o.obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(o.obrperccontratoanterior,0))::numeric(20,2) as porcentagem_execucao,
			    (select count(pp.popid) from obras.preobraprorrogacao pp where pp.preid = p.preid) as totalPro
			from
				par.v_saldo_obra_por_empenho v 
			    inner join obras.preobra p on p.preid = v.preid and p.prestatus = 'A'
			    left join obras.pretipoobra pt on pt.ptoid = p.ptoid and pt.ptostatus = 'A'
			    left join par.v_pagamento_total_por_obra vlo ON vlo.preid = p.preid
			    LEFT JOIN obras2.obras o ON o.preid = p.preid AND o.obridpai IS NULL AND o.obrstatus = 'A'
			    left join obras2.situacao_registro str on str.strid = o.strid and str.strstatus = 'A'
			    left join( select
                                case when count(r.rstid) > 0 then
                                    case when d.esdid in (1142, 1143) then 'Não' else 'Sim' end
                                else 'Não' end as res_estado,    	
                                count(r.rstid), p1.obrid, d.esdid
                            from
                                obras.preobra p1
                                left join obras2.restricao r
                                    inner join workflow.documento d on d.docid = r.docid
                                on r.obrid = p1.obrid and r.tprid = 17 and r.rstitem = 'R' and r.rststatus = 'A'
                            group by p1.obrid, d.esdid
                        ) res ON res.obrid = p.obrid 
			    $join
			where
				v.tipo = 'PAR'
			    and v.processo = '{$_REQUEST['processo']}'
			    and v.saldo > 0
			    $filtro
			group by p.prevalorobra, p.predescricao, p.preano, p.preid, pt.ptodescricao, p.obrid, preblockreformulacao,porcentagem_execucao, str.strdsc, vlo.totalpago, res.res_estado, v.processo";
	
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	
	$arrRegistro = array();
	foreach ($arrDados as $v) {
		$sql = "SELECT 
					MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento,
					MIN(pag.pagdatapagamentosiafi) + 720 as prazo
				FROM 
					par.pagamentoobrapar po 
				INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
				WHERE 
					po.preid = {$v['preid']}";
		$dataPrimeiroPagamento = $db->carregar($sql);
		
		if ($dataPrimeiroPagamento[0]['data_primeiro_pagamento']) {
			$sql = "SELECT popdataprazoaprovado FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid = " . $v['preid'];
			$prorrogado = $db->pegaUm($sql);
			if ($prorrogado) {
				$dataAtual = $prorrogado;
			} else {
				$dataAtual = $dataPrimeiroPagamento[0]['prazo'];
			}
		}
		
		/* $sql = "SELECT popdataprazoaprovado FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid = ".$v['preid'];
		$prorrogado = $db->pegaUm($sql);
		
		if( $prorrogado ){
			$dataAtual = $prorrogado;
		} else {
			$dataAtual = $dataPrimeiroPagamento[0]['prazo'];
		} */  
		array_push($arrRegistro, array(
									'acoes' => $v['acoes'],
									'preid' => $v['preid'],
									'obrid' => $v['obrid'],
									'restricao' => $v['res_estado'],
									'predescricao' => $v['predescricao'],
									'ptodescricao' => $v['ptodescricao'],
									'estado2' => $v['estado2'],
									'valorobra' => $v['valorobra'],
									'valorpagoobra' => $v['valorpagoobra'],
									'dtvigencia' => ($dataAtual ? formata_data($dataAtual) : ''),
									'porcentagem_execucao' => $v['porcentagem_execucao'],
									'tempo' => $v['diasvalidado'],
									)
				);
	}
	
	$cabecalho = array('&nbsp;&nbsp;Ações&nbsp;&nbsp;', 'preid', 'obrid', 'Restrições tipo Diligência do Obras 2', 'Descrição', 'Tipo da Obra','Situação Obras 2.0', 'Valor da Obra','Valor pago da Obra', 'Data de Vigência', '% de Execução da Obra', 'Tempo de Prorrogação');
	$db->monta_lista_simples( $arrRegistro, $cabecalho,100,5,'N','100%','S', true, false, false, false);
	
	exit();
}

include APPRAIZ."includes/cabecalho.inc";
 
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( $titulo_modulo, '' );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<div id="dialog" title="Solicitações do Processo" style="display:none;"> </div>

<div id="dialog-regerar-termo" title="Regerar Termo" style="display: none;"></div>

<form name="formulario" id="formulario" method="post" action="" >
	<input type="hidden" name="reqs" id="reqs" value="">
	<input type="hidden" name="preid" id="preid" value="">
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="subtituloDireita" width="30%"> Fases da Reprogramação:</td>
			<td>
				<input type="radio" name="situacao_pro" value="VO" <?php echo ($_REQUEST['situacao_pro'] == 'VO' ? 'checked="checked"' : ''); ?> />&nbsp;Aguardando Validação de Obra<br>
				<input type="radio" name="situacao_pro" value="GT" <?php echo ($_REQUEST['situacao_pro'] == 'GT' ? 'checked="checked"' : ''); ?>/>&nbsp;Aguardando Geração de Termo<br>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%"> Situação das obras na reprogramação:</td>
			<td>
				<input type="radio" name="situacao_pro" value="BL" <?php echo ($_REQUEST['situacao_pro'] == 'BL' ? 'checked="checked"' : ''); ?>/>&nbsp;Vigência de Obras bloqueadas<BR>
				<input type="radio" name="situacao_pro" value="SP" <?php echo ($_REQUEST['situacao_pro'] == 'SP' ? 'checked="checked"' : ''); ?>/>&nbsp;Obras que estão disponível para Solicitar Prorrogação
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Preid:</td>
            <td><?php echo campo_texto( 'preid', 'N', 'S', 'Preid', 10, 8, '[#]', '', '', '', '', '', "this.value=mascaraglobal('[#]',this.value)", $_REQUEST['preid']); ?></td>
        </tr>
		<tr>
			<td class="SubTituloDireita">Número de Processo:</td>
			<td colspan="3">
			<?php
				$filtro = simec_htmlentities( $_REQUEST['numeroprocesso'] );
				$numeroprocesso = $filtro;
				echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', ''); 
			?>
			</td>	
		</tr>
		<tr>
			<td class="SubTituloDireita">Número do Termo:</td>
			<td colspan="3">
			<?php
				$dopid = $_REQUEST['dopid'];
				echo campo_texto( 'dopid', 'N', 'S', '[#]', 20, 50, '', '');
			?>
			</td>
	
		</tr>
		<!--  <tr>
			<td class="subtituloDireita" width="30%"> Esfera</td>
			<td>
				<?php

				$sql = Array(Array('codigo'=>'M', 'descricao'=>'Municipal'),Array('codigo'=>'E', 'descricao'=>'Estadual'));
				$db->monta_combo( "esfera", $sql, 'S', 'Todas as esferas', '', '' );
				?>
			</td>
		</tr>-->
		<tr>
			<td class="subtituloDireita" width="30%">UF:</td>
			<td>
				<?php
					$estuf = $_POST['estuf'];
					$sql = "SELECT e.estuf as codigo, e.estdescricao as descricao 
							FROM territorios.estado e ORDER BY e.estdescricao ASC";
					$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
				?>
			</td>
		</tr>
		<tr id="tr_muncod">
			<td class="subtituloDireita" width="30%"> Município:</td>
			<td id="td_muncod">
				<?php
					if( $estuf ){
						$muncod = $_POST['muncod'];
						$sql = "SELECT muncod as codigo, mundescricao as descricao 
								FROM territorios.municipio 
								WHERE estuf = '$estuf'
								ORDER BY 2 ASC";
						$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
					}else{
						echo "Escolha uma UF.";
					}
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%">Fim de Vigência</td>
			<td>
			<?php 
			$dt_fim_vigencia_inicio = ($_POST['dt_fim_vigencia_inicio'] ? formata_data_sql($_POST['dt_fim_vigencia_inicio']) : '');
			$dt_fim_vigencia_fim = ($_POST['dt_fim_vigencia_fim'] ? formata_data_sql($_POST['dt_fim_vigencia_fim']) : '');
			?>
				De: <?=campo_data2('dt_fim_vigencia_inicio', 'N', 'S', '', 'S' ); ?>
				Até: <?=campo_data2('dt_fim_vigencia_fim', 'N', 'S', '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%"> </td>
			<td>
				<input type="button" class="pesquisar" value="Pesquisar"/>
				<input type="button" class="xls" value="Exportar XLS"/>
			</td>
		</tr>
	</table>
</form>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td width="15%"><span style="font-size:18px;" title="Abrir Lista de Obras" class="glyphicon glyphicon-download"></span> Abrir Lista de Obras</td>
	<td width="15%"><span style="font-size:18px;" title="Abrir Informações da Obra" class="glyphicon glyphicon-question-sign"></span> Abrir Informações da Obra</td>
	<td width="15%"><span style="font-size:18px; color:green;" title="Validar a Prorrogação de Obras" class="glyphicon glyphicon-check"></span> Validar a Prorrogação de Obras</td>
	<td width="15%"><span style="font-size:18px;" title="Validar a Prorrogação de Obras" class="glyphicon glyphicon-edit"></span> Editar a Prorrogação de Obras</td>
	<td width="15%"><span style="font-size:18px; color:red;" title="Recusar a Prorrogação da Obras" class="glyphicon glyphicon-share"></span> Recusar a Prorrogação da Obras</td>
	<td width="10%"><span style="font-size:18px; color: #02025A;" title="Regerar Termo" class="glyphicon glyphicon-list-alt"></span> Regerar Termo</td>
</tr>
</table>
<div id="debug"></div>
<div id="lista">
</div>
<?php 
if( isset($_REQUEST['numeroprocesso']) ){

	$imgMais = "<span style=\"font-size:18px; cursor:pointer;\" title=\"mais\" id=\"image_'||pp.proid||'\" class=\"glyphicon glyphicon-download\" onclick=\"carregarListaObras(\''||pp.pronumeroprocesso||'\', '||pp.proid||', \'\', this);\"></span>";	

	$arWere = array();
	if( $_REQUEST['situacao_pro'] == 'VO' ){		
		array_push($arWere, "pp.pronumeroprocesso in (SELECT distinct
															e.empnumeroprocesso
														FROM obras.preobraprorrogacao pr
															inner join par.empenhoobrapar eo on eo.preid = pr.preid and eo.eobstatus = 'A'
														    inner join par.empenho e on e.empid = eo.empid and e.empstatus = 'A'
															inner join obras.preobra p on p.preid = eo.preid and p.prestatus = 'A'
														WHERE popstatus = 'P' and popvalidacao = 'f'
															and p.preblockreformulacao != 'S') "); 
	}
	if( $_REQUEST['situacao_pro'] == 'GT' ){

         $geraTermo = "<span style=\"font-size:15px; color: #02025A; cursor:pointer;\" attr=\"prorrogacaoVigencia\" title=\"Regerar Termo\" id='||dp.dopid||' class=\"glyphicon glyphicon-list-alt regerarTermo\" ></span>";
        array_push($arWere, "dopprorrogacaovigenciaobra IS TRUE");
//		array_push($arWere, "pp.pronumeroprocesso in (SELECT distinct
//															e.empnumeroprocesso
//														FROM obras.preobraprorrogacao pr
//															inner join par.empenhoobrapar eo on eo.preid = pr.preid and eo.eobstatus = 'A'
//														    inner join par.empenho e on e.empid = eo.empid and e.empstatus = 'A'
//															inner join obras.preobra p on p.preid = eo.preid and p.prestatus = 'A'
//														WHERE popstatus = 'P' and popvalidacao = 'f'
//															and p.preblockreformulacao != 'S') ");
//		
		//array_push($arWere, "dp.dopprorrogacaovigenciaobra = true");
	}
	if( $_REQUEST['situacao_pro'] == 'SP' ){
		$imgMais = "<span style=\"font-size:18px; cursor:pointer;\" title=\"mais\" id=\"image_'||pp.proid||'\" class=\"glyphicon glyphicon-download\" onclick=\"carregarListaObras(\''||pp.pronumeroprocesso||'\', '||pp.proid||', \'bloqueio\', this);\"></span>";
		array_push($arWere, "pp.pronumeroprocesso in (select distinct empnumeroprocesso
													from(
													SELECT
													    coalesce(cast(MAX(popdataprazoaprovado) as date), 
													    cast((MIN(pag.pagdatapagamentosiafi) + 720) as date) ) - cast(now() as date) as prazo, 
													    po.preid,
													    emp.empnumeroprocesso
													FROM
													    par.pagamentoobrapar po
													    inner join obras.preobra pre on pre.preid = po.preid and pre.prestatus = 'A'
													    inner join workflow.documento esd on esd.docid = pre.docid
														inner join par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
													    inner join par.empenho emp on emp.empid = pag.empid and emp.empstatus = 'A'
														left join obras.preobraprorrogacao pop ON pop.preid = po.preid AND popstatus = 'A'
													where
														esd.esdid not in (".WF_PAR_OBRA_AGUARDANDO_PRORROGACAO.")
													    and pag.pagdatapagamentosiafi is not null
														and preblockreformulacao <> 'S'
													group by po.preid, emp.empnumeroprocesso
													) as foo
													where prazo < 91)");
	}
	if( $_REQUEST['situacao_pro'] == 'BL' ){
		$imgMais = "<span style=\"font-size:18px; cursor:pointer;\" title=\"mais\" id=\"image_'||pp.proid||'\" class=\"glyphicon glyphicon-download\" onclick=\"carregarListaObras(\''||pp.pronumeroprocesso||'\', '||pp.proid||', \'bloqueio\', this);\"></span>";
		array_push($arWere, "pp.pronumeroprocesso in (SELECT distinct
														    e.empnumeroprocesso
														FROM obras.preobra p
														    inner join par.empenhoobrapar eo on eo.preid = p.preid and eo.eobstatus = 'A'
														    inner join par.empenho e on e.empid = eo.empid and e.empstatus = 'A'
														WHERE p.preblockreformulacao = 'S' and p.prestatus = 'A')");
	}
	if( !empty($_REQUEST['numeroprocesso']) ){
		$_REQUEST['numeroprocesso'] = str_replace(".","", $_REQUEST['numeroprocesso']);
		$_REQUEST['numeroprocesso'] = str_replace("/","", $_REQUEST['numeroprocesso']);
		$_REQUEST['numeroprocesso'] = str_replace("-","", $_REQUEST['numeroprocesso']);
		$where[] = "p.prpnumeroprocesso = '".$_REQUEST['numeroprocesso']."'";
		
		array_push($arWere, "pp.pronumeroprocesso = '{$_REQUEST['numeroprocesso']}'");
	}
	if( !empty($_REQUEST['dopid']) ){
		array_push($arWere, "dp.dopnumerodocumento = '{$_REQUEST['dopid']}'"); 
	}
	if( !empty($_REQUEST['muncod']) ){
		array_push($arWere, "pp.muncod = '{$_REQUEST['muncod']}'"); 
	} else {
		if( !empty($_REQUEST['estuf']) ){
			array_push($arWere, "pp.estuf = '{$_REQUEST['estuf']}'");
		}
	}
	
	if( $_POST['dt_fim_vigencia_inicio'] != '' && $_POST['dt_fim_vigencia_fim'] != '' ){
		array_push($arWere, " cast(to_char( cast(vig.data as date), 'YYYY-MM-DD') as date) between cast('".formata_data_sql($_POST['dt_fim_vigencia_inicio'])."' as date) and cast('".formata_data_sql($_POST['dt_fim_vigencia_fim'])."' as date)");
		
		$filtroJoin = "left join (
							select distinct
								processo,
							    prazo as data
							from(
							SELECT
								po.preid,
							    case when pop.data is not null then pop.data else (MIN(pag.pagdatapagamentosiafi) + 720) end  as prazo,
							    emp.empnumeroprocesso as processo
							FROM
							    par.pagamentoobrapar po
								inner join par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
							    inner join par.empenho emp on emp.empid = pag.empid and emp.empstatus = 'A'
							    left join(
							    	select max(popdataprazoaprovado) as data, preid
							        from obras.preobraprorrogacao
							        where popvalidacao = 't'
							        group by preid
							    ) pop on pop.preid = po.preid
							group by po.preid, pop.data, emp.empnumeroprocesso
							) as foo ) vig on vig.processo = pp.pronumeroprocesso"; 
	}
	
	if( !empty($_REQUEST['preid']) ) array_push($arWere, "pp.pronumeroprocesso in (SELECT distinct
                                                                        emp.empnumeroprocesso
                                                                    FROM
                                                                        obras.preobra pre 
                                                                        inner join par.empenhoobrapar eo on eo.preid = pre.preid and eo.eobstatus = 'A'
                                                                        inner join par.empenho emp on emp.empid = eo.empid and emp.empstatus = 'A'
                                                                    where
                                                                            pre.prestatus = 'A'
                                                                        and pre.preid = {$_REQUEST['preid']})");

	if( $_REQUEST['reqs'] == 'xls' )
	{
		$acao = "";
		$cabecalho = array("Nº do Processo", 'Município/Estado', 'UF', "Nº do Documento", "Tipo de documento","Data de Vigência", "Qnt de Obras", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Dados Bancário", "Saldo Bancário(CC + CP + Fundo)");
		
	}
	else{
		$acao = "'<center>
					$imgMais
					$geraTermo
				</center>' as mais,";
		$cabecalho = array("&nbsp;","Nº do Processo", 'Município/Estado', 'UF', "Nº do Documento", "Tipo de documento","Data de Vigência", "Qnt de Obras", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Dados Bancário", "Saldo Bancário<br />(CC + CP + Fundo)");
	}
	 
	$sql = "SELECT 
				{$acao}
				substring(pp.pronumeroprocesso  from 1 for 5)||'.'||
					   		substring(pp.pronumeroprocesso from 6 for 6)||'/'||
					   		substring(pp.pronumeroprocesso from 12 for 4)||'-'||
					   		substring(pp.pronumeroprocesso from 16 for 2) as processo,
				case when m.mundescricao is not null then m.mundescricao else e.estdescricao end as municipio,
				CASE WHEN pp.estuf IS NOT NULL THEN pp.estuf ELSE m.estuf END as estuf,
				dp.dopnumerodocumento,
			    dp.mdonome as tipodocumento, 
				(	select dopdatafimvigencia from par.documentopar  d
			        inner join par.documentoparvalidacao v ON d.dopid = v.dopid
			        where d.proid = pp.proid
			        AND dopstatus <> 'E'
			        AND dpvstatus = 'A'
			        AND mdoid not in (79,65,66,68,76,80,67,73,82)
			        order by d.dopid desc
			        LIMIT 1
			    ) as data_vigencia,
				(select count(pc.preid) from par.processoobraspar po inner join par.processoobrasparcomposicao pc on pc.proid = po.proid where pc.pocstatus = 'A' and po.proid = pp.proid) as qtdObra,
				cast(dp.dopvalortermo as numeric(20,2)) as dopvalortermo,
				vve.saldo as valorempenho,
			    pgs.valor_pagamento as valorpagamentosolicitado,
			    pm.valor_pagamento as valorpagamento, 
			    'Banco: '||coalesce(probanco, 'n/a')||'<br> Conta: '||coalesce(proagencia, 'n/a')||'<br> Conta Corrente: '||coalesce(nu_conta_corrente, 'n/a') as dados_bancarios,
			    (
			          select coalesce(saldo, 'Não Informado') as saldo from
			          (
			                 select par.retornasaldoprocesso(pp.pronumeroprocesso) as saldo
			          ) as saldomes
			      ) 
			    
			FROM par.vm_documentopar_ativos  dp
				INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
				INNER JOIN par.processoobraspar pp ON pp.proid = dp.proid and pp.prostatus = 'A'
				INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
				LEFT JOIN territorios.estado 	e ON e.estuf = pp.estuf 
				LEFT JOIN territorios.municipio m ON m.muncod = pp.muncod
				left join par.vm_saldo_empenho_do_processo vve on vve.processo = pp.pronumeroprocesso and vve.tipo = 'OBRA'			        
			    LEFT JOIN (
					SELECT 
					prp.pronumeroprocesso, sum( popvalorpagamento ) as valor_pagamento, pag.pagsituacaopagamento
				FROM 
					par.processoobraspar 	prp 
					INNER JOIN par.empenho 			emp ON emp.empnumeroprocesso = prp.pronumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
					INNER JOIN par.pagamento 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento ilike '%EFETIVADO%'
					INNER JOIN par.pagamentoobrapar 	ps  ON ps.pagid = pag.pagid  
				where
					prp.prostatus = 'A'
				GROUP BY prp.pronumeroprocesso, pagsituacaopagamento ) pm ON pm.pronumeroprocesso = pp.pronumeroprocesso
				LEFT JOIN (
					SELECT 
					prp.pronumeroprocesso, sum( popvalorpagamento ) as valor_pagamento, pag.pagsituacaopagamento
				FROM 
					par.processoobraspar 	prp 
					INNER JOIN par.empenho 			emp ON emp.empnumeroprocesso = prp.pronumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
					INNER JOIN par.pagamento 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento in ('8 - SOLICITAÇÃO APROVADA', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', 'Enviado ao SIGEF')
					INNER JOIN par.pagamentoobrapar 	ps  ON ps.pagid = pag.pagid  
				where
					prp.prostatus = 'A'
				GROUP BY prp.pronumeroprocesso, pagsituacaopagamento ) pgs ON pgs.pronumeroprocesso = pp.pronumeroprocesso
				$filtroJoin
			WHERE
				1=1
				".($arWere ? ' and '.implode(' and ', $arWere) : '')."
			order by 4, 3";
//	ver(simec_htmlentities($sql),d);

	$db->monta_lista_simples( $sql, $cabecalho,100,5,'N','100%','S', true, false, false, true);
	if( $_REQUEST['reqs'] == 'xls' ){
		ob_clean();
		header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
		header("Pragma: no-cache");
		header("Content-type: application/xls; name=SIMEC_RelatDocente" . date("Ymdhis") . ".xls");
		header("Content-Disposition: attachment; filename=SIMEC_ValidacaoExecucaoFinanceira" . date("Ymdhis") . ".xls");
		$db->monta_lista_tabulado($sql, $cabecalho, 1000000, 5, 'N', '100%');
		die();
	}
}

?>
<script type="text/javascript">

/* Esta função é a cópia fiel da requisição da página documentoPar.
* */

$('.pesquisar').click(function(){

	if(jQuery('[name="situacao_pro"]:checked').length > 0){
		jQuery('#reqs').val('');
		jQuery('[name="formulario"]').submit();
	} else {
		alert('Selecione um item nas "Fases da Reprogramação" ou "Situação das obras na reprogramação"');
		return false
	}
});

$('.xls').click(function(){

	if(jQuery('[name="situacao_pro"]:checked').length > 0){
		jQuery('#reqs').val('xls');
		jQuery('[name="formulario"]').submit();
	} else {
		alert('Selecione um item nas "Fases da Reprogramação" ou "Situação das obras na reprogramação"');
		return false
	}
});

function carregaTermoMinuta(dopid){
	window.open('par.php?modulo=principal/visualizaTermoCompromisso&acao=A&dopid='+dopid,'visualizatermo','scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
}

$(document).ready(function(){

	/* Só mostra Município se n for esfera municipal
	*/
	$('[name="esfera"]').change(function(){
		if( $(this).val() != 'E' ){
			$('#tr_muncod').show();
		}else{
			$('#tr_muncod').hide();
			$('[name="muncod"]').val('');
		}
	});

	/* Carrega Municpios de acordo com o estado
	*/
	$('[name="estuf"]').change(function(){
		if( $('[name="esfera"]').val() != 'E' ){
			if( $(this).val() != '' ){
				$('#aguardando').show();
				$.ajax({
			   		type: "POST",
			   		url: window.location.href,
			   		data: "requisicao=carregaMunicipios&estuf="+$(this).val(),
			   		async: false,
			   		success: function(resp){
			   			$('#td_muncod').html(resp);
			   			$('#aguardando').hide();
			   		}
			 	});
			}else{
				$('#td_muncod').html('Escolha uma UF.');
			}
		}
	});
        
	$("input[name='dopid']").bind("keyup blur focus", function(e) {
    	e.preventDefault();
        var expre = /[^0-9]/g;
        if ($(this).val().match(expre))
        	$(this).val($(this).val().replace(expre,''));
	});
});

function carregarListaObras(processo, proid, acao, obj) {

	var linha = obj.parentNode.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		jQuery('#image_'+proid).attr('class', 'glyphicon glyphicon-upload');
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncolbranco = nlinha.insertCell(0);
		ncolbranco.innerHTML = '&nbsp;';
		var ncol = nlinha.insertCell(1);
		ncol.colSpan=12;
		ncol.innerHTML="Carregando...";
		ncol.innerHTML="<div id='listaobraprocesso_" + processo + "' ></div>";
		carregarListaDivObras( processo, acao );
	} else {
		obj.title='mais';
		jQuery('#image_'+proid).attr('class', 'glyphicon glyphicon-download');
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}
}

function carregarListaDivObras( processo, acao ){
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "requisicao=carregarListaObras&processo="+processo+"&filtro="+acao+'&situacao='+jQuery('[name="situacao_pro"]:checked').val(),
   		async: false,
   		success: function(msg){
   	   		$('#listaobraprocesso_'+processo).html(msg);
   		}
	});
}

function abreObras( preid, ano ){
	return window.open('par.php?modulo=principal/subacaoObras&acao=A&preid='+preid+'&ano='+ano+'&sbaid=', 
	    	   'ProInfancia', 
	    	   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();	
} 

function validarProrrogacao(preid, processo){	
	window.open('par.php?modulo=principal/popupProrrogacaoPACAprovacao&acao=A&preid='+preid+'&processo='+processo+'&acompanhamento=S', 
					   'Prorrogação', 
					   "height=600,width=800,scrollbars=yes,top=50,left=200" ).focus();
}

function editarProrrogacao(preid, processo) {
    window.open('par.php?modulo=principal/popupProrrogacaoPACAprovacao&acao=A&editar=1&preid=' + preid+'&processo='+processo+'&acompanhamento=S',
            'Prorrogação',
            "height=650,width=800,scrollbars=yes,top=50,left=200").focus();
}

function recusarProrrogacao(preid, processo) {
    window.open('par.php?modulo=principal/popupProrrogacaoPACAprovacao&acao=A&recusar=S&preid=' + preid+'&processo='+processo+'&acompanhamento=S',
            'Prorrogação',
            "height=650,width=800,scrollbars=yes,top=50,left=200").focus();
}

/*
 * Funcao removida devido solicitacao da Renilda 16/10/2015
 * function bloqueiarProrrogacao( preid, acao, processo ){
	
	if(confirm('Tem certeza que deseja bloqueiar as solicitações de reprogramação para está obra?')){
		if( acao == 'bloqueiar_cancelar' ){
			window.open('par.php?modulo=principal/popupProrrogacaoPACAprovacao&acao=A&boSolicitacao=S&recusar=S&preid=' + preid+'&processo='+processo+'&acompanhamento=S',
	                'Prorrogação',
	                "height=600,width=800,scrollbars=yes,top=50,left=200").focus();
		} else {
			jQuery.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "requisicao=bloqueiar&processo="+processo+"&preid="+preid,
		   		async: false,
		   		success: function(msg){
			   		if( msg == '1' ){
			   			alert('Obra bloqueiada para solicitação de reformulação');
			   			carregarListaDivObras( processo, 'bloqueio' );
			   		} else {
				   		alert('Obra não bloqueiada');
				   	}
		   		}
			});
		}
	}
}
*/
jQuery('.regerarTermo').live('click', function () {

 	var dopid = jQuery(this).attr('id');
    var tipoRegera = jQuery(this).attr('attr');

    jQuery.ajax({
    	type: "POST",
        url: window.location.href,
        data: "reqs=formRegeraTermo&dopidRegera=" + dopid + "&tipoRegera=" + tipoRegera,
        async: false,
        success: function (msg) {
        	jQuery("#dialog-regerar-termo").html(msg);
            jQuery("#dialog-regerar-termo").dialog({
            	resizable: true,
                width: 400,
                modal: true,
                buttons: {
                	"Sim": function () {
						if (jQuery('[name="dopdatafimvigencia"]').val() == '') {
                        	alert('Campo obrigatório.');
                            jQuery('[name="dopdatafimvigencia"]').focus();
                            return false;
						}
						jQuery('#reqRegera').val('regerarTermo');
						jQuery('#formRegerar').submit();
                        jQuery(this).dialog("close");
					},
                    "Não": function () {
                    	jQuery(this).dialog("close");
					}
				}
			});
		}
	});
});
</script>
<?php 
function carregaMunicipios( $dados ){

	global $db;

	extract($dados);

	$sql = "SELECT muncod as codigo, mundescricao as descricao
	FROM territorios.municipio
	WHERE estuf = '$estuf'
	ORDER BY 2 ASC";

	$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
}

function formRegeraTermo( $dados ) {
	global $db;
	
	extract ( $dados );
	$sql = "SELECT
				coalesce(dopdatainiciovigencia, to_char(dopdatainclusao,'MM/YYYY')) as dopdatainiciovigencia
			FROM
				par.documentopar
			WHERE
				dopid = $dopidRegera";
	$datas = $db->pegaLinha ( $sql );
	?>
<center>
	<b>Deseja regerar este termo?</b>
</center>
<form method="post" name="formRegerar" id="formRegerar">
		<input type="hidden" name="reqs" id="reqRegera" /> 
        <input type="hidden" name="dopid" id="dopidRegera" value="<?= $dopidRegera ?>" /> 
        <input type="hidden" name="datainicial" id="datainicial" value="<?= $datas['dopdatainiciovigencia'] ?>" /> 
        <input type="hidden" name="tipoRegera" id="tipoRegera" value="<?= $tipoRegera ?>" />
        Inicio da Vigência: <?= $datas['dopdatainiciovigencia'] ?><br>
        Fim da Vigência: 
        <?php
	if ($tipoRegera != 'prorrogacaoVigencia') {
		echo campo_texto ( 'dopdatafimvigencia', 'N', 'S', '', 10, 7, '##/####', '' ) . '(mm/aaaa)';
	} else {
		$obPreObra = new PreObra ();
		$dtFimVigencia = $obPreObra->getMaxDataFimVigenciaObra ( $dopidRegera );
		echo campo_texto ( 'dopdatafimvigencia', 'N', 'N', '', 10, 7, '##/####', '', '', '', '', '', '', $dtFimVigencia );
	}
	?> 
    </form>
<?php
}

function regerarTermo( $dados ) {
	global $db;

	$sql = "SELECT
                    proid, dopstatus, dopdiasvigencia, dopdatainicio, dopdatafim,
                    mdoid, dopdatainclusao, usucpfinclusao, dopdataalteracao, usucpfalteracao, dopjustificativa,
                    dopdatavalidacaofnde, dopusucpfvalidacaofnde, dopdatavalidacaogestor, dopusucpfvalidacaogestor,
                    dopusucpfstatus, dopdatastatus, dopdatapublicacao, doppaginadou, dopnumportaria, proid,
                    dopreformulacao, dopidpai, dopdatainiciovigencia, '" . $_REQUEST ['dopdatafimvigencia'] . "' as dopdatafimvigencia
            FROM 
                    par.documentopar 
            WHERE 
                    dopid = {$_REQUEST['dopid']} 
                    AND dopstatus = 'A'";
	
	$arrDadosDoc = $db->pegaLinha ( $sql );
	$arrDadosDoc = $arrDadosDoc ? $arrDadosDoc : array ();
	
	if ($arrDadosDoc ['proid']) {
		$processo = $db->pegaUm ( "select pronumeroprocesso from par.processoobraspar where proid = {$arrDadosDoc['proid']}" );
		verificaEmpenhoSigef ( $processo );
	}
	
	if ($arrDadosDoc ['mdoid']) {
            $sql = "SELECT mdoconteudo, tpdcod, mdodocumentoex, mdotipovinculado FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = " . $arrDadosDoc ['mdoid'];
            $arrModelo = $db->pegaLinha ( $sql );

            if ($arrModelo['mdodocumentoex'] == 't') {
               $sql = "SELECT 
                           mdoconteudo, 
                           tpdcod
                       FROM par.modelosdocumentos 
                       WHERE mdostatus = 'A' AND mdoid = " . $arrModelo['mdotipovinculado'];
               $arrModeloPai = $db->pegaLinha ( $sql );
               $mdoconteudo = $arrModeloPai ['mdoconteudo'];
               $tpdcod = $arrModeloPai ['tpdcod'];
               $arrDadosDoc ['mdoid'] = $arrModelo['mdotipovinculado'];

           } else {
               $mdoconteudo = $arrModelo ['mdoconteudo'];
               $tpdcod = $arrModelo ['tpdcod'];
           }
	}
	
	$sql = "SELECT preid as chk FROM par.termocomposicao WHERE dopid = " . $_REQUEST ['dopid'];
	$subacoes = $db->carregarColuna ( $sql );
	
	$post = array (
			'mdoid' => $arrDadosDoc ['mdoid'],
			'proid' => $arrDadosDoc ['proid'],
			'tpdcod' => $tpdcod,
			'dopid' => $_REQUEST['dopid'],
			'chk' => $subacoes 
	);
	
	$_SESSION ['par'] ['cronogramaFinal'] = $arrDadosDoc ['dopdatafimvigencia'];
	
	$doptexto = alteraMacrosMinutaObras ( $mdoconteudo, $arrDadosDoc ['proid'], $post );
	
	$arrDadosDoc['regerar'] = true;
        
	if($_SESSION['par']['totalVLR']){
    	$arrDadosDoc['dopvalor'] = $_SESSION['par']['totalVLR'];
        unset($_SESSION['par']['totalVLR']);
	}
	
	$dopidNovo = salvarDadosMinutaObras ( $arrDadosDoc, $doptexto, $_REQUEST['dopid'] );
	
	foreach ( $subacoes as $preid ) {
		$sql = "INSERT INTO par.termocomposicao( preid, dopid)
                            VALUES ( $preid, $dopidNovo)";
		$db->executar ( $sql );
	}
	
	// Muda o Status da Obra para Aprovada
	 
	if ($_REQUEST ['tipoRegera'] == 'prorrogacaoVigencia') {
		include_once APPRAIZ . 'includes/workflow.php';
		
		$sqlPreoObra = "SELECT 
                                pop.preid 	
                        FROM obras.preobraprorrogacao pop
                        INNER JOIN par.termocomposicao tc ON tc.preid = pop.preid
                        WHERE tc.dopid = {$_REQUEST['dopid']} AND pop.popstatus = 'A' AND pop.popvalidacao = 't'
                        ORDER BY popid ASC";
		
		$arrPreObra = $db->carregar ( $sqlPreoObra );
		foreach ( $arrPreObra as $obra ) {
			$docid = $db->pegaUm ( "SELECT docid FROM obras.preobra WHERE preid='" . $obra ['preid'] . "'" );
			
			$sql = "SELECT tooid FROM obras.preobra WHERE preid = {$obra['preid']}";
			$tooid = $db->pegaUm ( $sql );
			
			// Se a obra for PAC
			if ($tooid == 1) {
				$esdidorigem = WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO;
			} else { // Sea obra não for PAC
				$esdidorigem = WF_PAR_OBRA_AGUARDANDO_PRORROGACAO;
			}
			
			$esdiddestino = $db->pegaUm ( "SELECT esdidorigem FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid=" . $obra ['preid'] );
			$aedid = $db->pegaUm ( "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem = " . $esdidorigem . " AND esdiddestino = " . $esdiddestino );
			
                        if( $aedid == '' && $esdidorigem != $esdiddestino ){
                            $sql = "SELECT esddsc FROM workflow.estadodocumento WHERE esdid = $esdiddestino";
                            $esddsc = $db->pegaUm( $sql );

                            $sql = "INSERT INTO workflow.acaoestadodoc
                                        (esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada, esdsncomentario, aedvisivel, aedcodicaonegativa)
                                    VALUES ($esdidorigem, $esdiddestino, 'Enviar para $esddsc', 'A', 'Enviada para $esddsc', true, false, false )
                                    RETURNING aedid";

                            $aedid = $db->pegaUm($sql);
                        }
                        
			wf_alterarEstado ( $docid, $aedid, true, $d = array (
					'preid' => $obra ['preid'] 
			) );
		}
	}
	
	$db->commit ();
	
	enviaEmailNovoTermoObrasPAR ( $_REQUEST ['dopid'], 'regerar' );
	
	echo "
		<script>
			alert('Termo regerado com sucesso!');
			window.location.href = window.location.href;
		</script>";
	
	exit ();
}
?>
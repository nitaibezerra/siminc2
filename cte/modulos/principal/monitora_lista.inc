<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

$_SESSION['inuid'] = null;
unset( $_SESSION['inuid'] );

if ( $_REQUEST['muncod'] && $_REQUEST['evento'] == 'selecionar' ) {
	if( cte_selecionarMunicipio( INSTRUMENTO_DIAGNOSTICO_MUNICIPAL, WF_TIPO_CTE, $_REQUEST['muncod'] ) ) {
		$db->commit();
		header( "Location: ?modulo=principal/monitora_estrutura&acao=A" );
		exit();
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';

$titulo_modulo = "PAR - Plano de Metas";
monta_titulo( $titulo_modulo,'Monitoramento do PAR');

?>
<link rel="stylesheet" type="text/css" href="../../cte/monitoraFinanceiro/includes/monitora_financeiro.css"/>
<script type="text/javascript">
function removerFiltro(){
	document.formulario.filtro.value = "";
	document.formulario.estuf.selectedIndex = 0;
	document.formulario.submit();
}
</script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tbody>
	<tr>
		<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
		<form method="post" name="formulario">
		<div style="float: left;">
		<table border="0" cellpadding="3" cellspacing="0">
		<tr>
			<td valign="bottom">MunicÌpio<br/>
			<?php 
			$filtro = simec_htmlentities( $_REQUEST['filtro'] ); 
			echo campo_texto( 'filtro', 'N', 'S', '', 50, 200, '', '' ); 
			?>
			</td>
			<td valign="bottom">Estado<br/>
			<?php
			$estuf = $_REQUEST['estuf'];
			$unidades = "'" . implode( "','", cte_pegarUfsPermitidas() ) . "'";
			$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e where e.estuf in ({$unidades}) order by e.estdescricao asc";
			$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
			?>
			</td>
			<td valign="bottom">
			<?php
			$par = $_REQUEST['par'] ? $_REQUEST['par'] : "com";
			?>
			<label><input type="radio" name="par" value="com" <?= $par == "com" ? 'checked="checked"' : '' ?>/>Com PAR</label>
			<label><input type="radio" name="par" value="sem" <?= $par == "sem" ? 'checked="checked"' : '' ?>/>Sem PAR</label>
			&nbsp;&nbsp;|&nbsp;&nbsp;
			<label><input type="checkbox" name="capitais" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['capitais'] == 1 ? 'checked="checked"' : '' ?>/>Capitais</label>
			<label><input type="checkbox" name="grandescidades" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['grandescidades'] == 1 ? 'checked="checked"' : '' ?>/>Grandes Cidades</label>
			<label><input type="checkbox" name="indigena" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['indigena'] == 1 ? 'checked="checked"' : '' ?>/>Comunidade IndÌgena</label>
			<label><input type="checkbox" name="quilombola" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['quilombola'] == 1 ? 'checked="checked"' : '' ?>/>Comunidade Quilombola</label>
			</td>
			</tr>
			<tr>
			<td colspan="2">
			<?php
			$analise = $_REQUEST["analise"];
			?>
			<label style="font-weight: bold;" for="analise_todos"><input type="radio" name="analise" value="" id="analise_todos" <?= $analise == "" ? 'checked="checked"' : '' ?> onchange="alterarFiltroAnalise();" onclick="alterarFiltroAnalise();" />Todas</label>
			<label for="analise_naoanal"><input id="analise_naoanal" type="radio" name="analise" value="naoanalisado" <?= $analise == "naoanalisado" ? 'checked="checked"' : '' ?> onchange="alterarFiltroAnalise();" onclick="alterarFiltroAnalise();"/>N„o Analisado</label>
			<label for="analise_emanal"><input id="analise_emanal" type="radio" name="analise" value="emanalise" <?= $analise == "emanalise" ? 'checked="checked"' : '' ?> onchange="alterarFiltroAnalise();" onclick="alterarFiltroAnalise();"/>Em An·lise</label>
			<label for="analise_anal"><input  id="analise_anal" type="radio" name="analise" value="analisado" <?= $analise == "analisado" ? 'checked="checked"' : '' ?> onchange="alterarFiltroAnalise();" onclick="alterarFiltroAnalise();"/>Analisado</label>
			</td>
			<td colspan="1">
			&nbsp;<div id="div_analise" style="display: none;">
			<?php
			$minimo = $_REQUEST['minimo'] ? (integer) $_REQUEST['minimo'] : 1;
			$maximo = $_REQUEST['maximo'] ? (integer) $_REQUEST['maximo'] : 100;
			?>
			<input type="text" style="width: 25px;" maxlength="3" name="minimo" value="<?= $minimo ?>"/>%
			atÈ <input type="text" style="width: 25px;" maxlength="3" name="maximo" value="<?= $maximo ?>"/>%
			</div>
			</td>
								</tr>
								<tr>
									<td colspan="2">
										SituaÁ„o<br/>
										<?php
										$esdid = $_REQUEST["esdid"];
										$estados = array(
											CTE_ESTADO_DIAGNOSTICO,
											CTE_ESTADO_PAR,
											CTE_ESTADO_ANALISE,
											CTE_ESTADO_ANALISE_FIN,
											CTE_ESTADO_FINALIZADO,
										);
										$estados = implode( ",", $estados );
										//$sql = "select esdid as codigo, esddsc as descricao from workflow.estadodocumento e where esdid in ({$estados}) order by esdordem asc";
										$sql = "select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid = 2 order by esdordem";
										//dbg($sql,1);
										
										$res = $db->carregar( $sql );
										$res [] = array( "codigo" => 99,  "descricao" => "MunicÌpios com Termo de CooperaÁ„o TÈcnica" );
										
										$db->monta_combo( "esdid", $res , 'S', 'Todas as SituaÁıes', '', '' );
										?>
									</td>
									<td colspan="1">
									 &nbsp;
									</td>
								</tr>
								<tr>
									<td colspan="2">
										<br>
										Monitoramento<br> 
										<input type="radio" name="init" id = "init" value="iniciado" <? if( $_REQUEST['init'] == "iniciado") echo 'checked=checked'; ?> >Iniciado  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
										<input type="radio" name="init" id = "init" value="naoiniciado" <? if( $_REQUEST['init'] == "naoiniciado") echo 'checked=checked'; ?> >N„o Iniciado &nbsp;&nbsp;&nbsp; 
										<input type="radio" name="init" id = "init" value="todos" <? if( $_REQUEST['init'] == "todos") echo 'checked=checked'; ?> >Todos
									
									</td>
									 
								</tr>
							</table>
							
							<div style="height: 10px;"></div>
							<style>
								#ideb_, #ideb { width: 300px; }
							</style>
							<table border="0" cellpadding="3" cellspacing="0">
								<tr>
									<td width="342">
										ClassificaÁ„o IDEB
									</td>
									<td>
										Filtrar por:
									</td>
								</tr>
							</table>
							
							<?php
							$agrupador = new Agrupador( "formulario" );
							
							$sql = sprintf( "select tpmid, tpmdsc from territorios.tipomunicipio where gtmid = 2 and tpmstatus = 'A'" );
							$tipos = $db->carregar( $sql );
							
							$origem = array();
							$destino = array();
							if ( $tipos ) {
								foreach ( $tipos as $tipo ) {
									if ( in_array( $tipo['tpmid'], (array) $_REQUEST['ideb'] ) ) {
										$destino[] = array(
											'codigo'    => $tipo['tpmid'],
											'descricao' => $tipo['tpmdsc']
										);
									} else {
										$origem[] = array(
											'codigo'    => $tipo['tpmid'],
											'descricao' => $tipo['tpmdsc']
										);
									}
								}
							}
							
							$agrupador->setOrigem( "ideb_", null, $origem );
							$agrupador->setDestino( "ideb", null, $destino );
							$agrupador->exibir();
							?>
						</div>
						<div style="float: right;">
							<input type="button" name="" value="Pesquisar" onclick="pesquisar();"/>
						</div>
					</form>
				</td>
			</tr>
		</tbody>
	</table>
<script type="text/javascript">
function pesquisar() {
	selectAllOptions( document.getElementById( 'ideb' ) );
	formulario.submit();
}

function alterarFiltroAnalise() {
	var checked = document.getElementById('analise_emanal').checked;
	document.getElementById( 'div_analise' ).style.display = checked ? 'block' : 'none';
}
		
alterarFiltroAnalise();
</script>
<?php
# LISTA OS MUNICÕPIOS ##########################################################
	
	$municipios = cte_pegarMunicipiosPermitidos();
	$municipios_sql = "'" . implode( "','", $municipios ) . "'";
	
	$campos = array(  "m.mundescricao", "m.muncod", "d.docdsc", "ed.esddsc" );
	$palavras = array_unique( explode( " ", $_REQUEST['filtro'] ) );
	$filtro = array();
	foreach ( $palavras as $palavra ) {
		if ( empty( $palavra ) || strlen( $palavra ) < 3 ) {
			continue; # ignora palavras vazias
		}
		$a = array();
		foreach ( $campos as $campo ) {
			array_push( $a, "lower({$campo}) like lower('%{$palavra}%')" );
		}
		array_push( $filtro, " ( " . implode( " OR ", $a ) . " ) " );
	}
	
	switch ( $_REQUEST["analise"] ) {
		case "naoanalisado":
			array_push( $filtro, " ( floor( ( coalesce( subacoesanalisadas.total, 0 ) * 100 ) / ( subacoes.total ) ) = 0 ) " );
			break;
		case "emanalise":
			$a = array( abs( $_REQUEST["minimo"] ), abs( $_REQUEST["maximo"] ) );
			$minimo = min( $a );
			$maximo = max( $a );
			array_push( $filtro, " floor( ( coalesce( subacoesanalisadas.total, 0 ) * 100 ) / ( subacoes.total ) ) >= {$minimo} " );
			array_push( $filtro, " floor( ( coalesce( subacoesanalisadas.total, 0 ) * 100 ) / ( subacoes.total ) ) <= {$maximo} " );
			break;
		case "analisado":
			array_push( $filtro, " ( subacoes.total > 0 AND subacoesanalisadas.total = subacoes.total ) " );
			break;
		default:
			break;
	}
	
	if ( $par == "com" ) {
		array_push( $filtro, " coalesce(b.qtd,0) > 0 " );
	} else {
		array_push( $filtro, " coalesce(b.qtd,0) = 0 " );
	}
	
	if (  !empty( $_REQUEST["esdid"] ) ) {
		if( $_REQUEST["esdid"] == 99 )
			array_push( $filtro, " ed.esdid in ( 13, 14, 15, 11  )  " );
		else
			array_push( $filtro, " ed.esdid = {$_REQUEST["esdid"]} " );
	}
	
	if ( count( $filtro ) > 0 ) {
		$filtro_sql = " AND " . implode( " AND ", $filtro );
	}
	
	if ( !empty( $_REQUEST['estuf'] ) ) {
		$estado = " and m.estuf = '{$_REQUEST['estuf']}' ";
	}
	if ( $_REQUEST['init'] == "iniciado" ) {
		$where .= " and COALESCE(monitoramentopar.monitora,0) > 0 ";
	}elseif( $_REQUEST['init'] == "naoiniciado" ){
		$where .= " and COALESCE(monitoramentopar.monitora,0) = 0";
	} 
	$join_capitais = "";
	if ( $_REQUEST['capitais'] ) {
		$join_capitais = "inner join territorios.estado e on e.estuf = m.estuf and e.muncodcapital = m.muncod";
	}

	$join_grandescidades = "";
	if ( $_REQUEST['grandescidades'] ) {
		$join_grandescidades = "
			inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod and mtm.estuf = m.estuf and mtm.tpmid = 1
		";
	}

	$join_indigena = "";
	if ( $_REQUEST['indigena'] ) {
		$join_indigena = "
			inner join territorios.muntipomunicipio mti on mti.muncod = m.muncod and mti.estuf = m.estuf and mti.tpmid = 16
		";
	}

	$join_quilombola = "";
	if ( $_REQUEST['quilombola'] ) {
		$join_quilombola = "
			inner join territorios.muntipomunicipio mtq on mtq.muncod = m.muncod and mtq.estuf = m.estuf and mtq.tpmid = 17
		";
	}

	$join_ideb = "";
	if ( count( (array) $_REQUEST['ideb'] ) > 0 && $_REQUEST['ideb'] != null ) {
		$join_ideb = "inner join territorios.muntipomunicipio mtm2 on mtm2.muncod = m.muncod and mtm2.estuf = m.estuf and mtm2.tpmid in (". implode( ",", $_REQUEST['ideb'] ) .")";
	}
	
	$sql = sprintf(
		"select
			'<a style=\"margin: 0 -20px 0 20px;\" href=\"cte.php?modulo=principal/monitora_lista&acao=A&evento=selecionar&muncod='|| m.muncod ||'\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' as acao,
			m.muncod,
			m.mundescricao,
			m.estuf,
			
			coalesce( ed.esddsc,'N„o iniciado' ) as esddsc ,
			--'<center><span style=\"color:cococo;font-size: 10px;\">' || floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal)) || '%%</span>
			--<div style=\"text-align: left; margin-left: 5px; padding: 1px 0 1px 0; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color:#dcffdc;\" title=\"' ||  floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal))  || '%%\">
			--<div style=\"font-size:4px;width: ' || floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal))  || '%%; height: 6px; max-height: 6px; background-color:#339933;\">
			--</div></div></center>' as porcentagem,
			
			--'<center><span style=\"color:cococo;font-size: 10px;\">' || floor((coalesce(subacoesanalisadas.total,0) * 100)/(subacoes.total)) || '%%</span>
			--<div style=\"text-align: left; margin-left: 5px; padding: 1px 0 1px 0; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color:#dcdcff;\" title=\"' ||  floor((coalesce(subacoesanalisadas.total,0) * 100)/(subacoes.total))  || '%%\">
			--<div style=\"font-size:4px;width: ' || floor((coalesce(subacoesanalisadas.total,0) * 100)/(subacoes.total))  || '%%; height: 6px; max-height: 6px; background-color:#333399;\">
			--</div></div></center>' as porcentagemsubacao
			
			'' AS monitora,
			iu.inuid		 
			
		from territorios.municipio as m
		left join cte.instrumentounidade iu ON iu.mun_estuf = m.estuf and iu.muncod = m.muncod  and iu.itrid = ". INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ."
		left join workflow.documento d on d.docid = iu.docid
		left join workflow.estadodocumento ed on ed.esdid = d.esdid
		%s
		%s
		%s
		%s
		left join cte.indicadorespreenchidos as b on b.inuid=iu.inuid
		left join cte.indicadorestotais as a on a.itrid = iu.itrid
		LEFT JOIN (    
			SELECT distinct COUNT(ex.exeid) AS monitora, isu.inuid
		        FROM cte.instrumentounidade isu
			        INNER JOIN cte.pontuacao         AS p   ON p.inuid   = isu.inuid
			        INNER JOIN cte.acaoindicador         AS ac  ON ac.ptoid  = p.ptoid
			        INNER JOIN cte.subacaoindicador     AS sbi ON sbi.aciid = ac.aciid
			        INNER JOIN cte.monitoramentosubacao     AS mnt ON mnt.sbaid = sbi.sbaid
			        INNER JOIN cte.execucaosubacao         AS ex  ON ex.mntid  = mnt.mntid
			        GROUP BY isu.inuid
		        ) AS monitoramentopar ON monitoramentopar.inuid = iu.inuid
		-----------verificar left acima-----------
		left join cte.subacoestotal as subacoes on subacoes.inuid = iu.inuid
		left join cte.subacoesanalisadas as subacoesanalisadas on subacoesanalisadas.inuid = iu.inuid
        $join_quilombola
        $join_indigena
		where m.muncod in ( %s ) %s %s $where",
		$join_acoes,
		$join_capitais,
		$join_grandescidades,
		$join_ideb,
		$municipios_sql,
		$filtro_sql,
		$estado
	);
	
	$arrListas =  $db->carregar($sql);	
	
	/********************************************
	 * SINCRONIZA COM A ORDENA«√O DO ARRAY
	 */
	if ($_REQUEST['ordemlista']<>'') {
		if ($_REQUEST['ordemlistadir'] <> 'DESC') {
			$ordemlistadir = 'ASC';
			$ordemlistadir2 = 'DESC';
		} else {
			$ordemlistadir = 'DESC'; 
			$ordemlistadir2 = 'ASC';
		}
		// 	Obter uma lista de colunas
		foreach ($arrListas as $key => $row) {
			$row = ereg_replace("[^a-zA-Z0-9_]", "", strtr($row[($_REQUEST['ordemlista'])], "·‡„‚ÈÍÌÛÙı˙¸Á¡¿√¬… Õ”‘’⁄‹« ", "aaaaeeiooouucAAAAEEIOOOUUC_"));
	    	//$crt[$key]  = $row[($_REQUEST['ordemlista'])];
	    	$crt[$key]  = $row;
		}
		
		switch($ordemlistadir) {
			case 'ASC':
				array_multisort($crt,SORT_ASC, $arrListas);
				break;
			case 'DESC':
				array_multisort($crt,SORT_DESC, $arrListas);				
				break;
		}
	}
	
	$numPage = $_POST['numero'] ? $_POST['numero'] : 1;
	for($i=$numPage-1; $i<($numPage+19); $i++)
	{		
		$inuid = $arrListas[$i]['inuid'];
		$cor 	= "#cococo";
		$cor1 	= "black";
		$porcentagemTotal = 100;
		
		$sql = "select min( perdtinicioref ) as inicio, max( perdtterminoref ) as final from cte.periodoreferencia";
		$arVigencia = $db->carregar( $sql );
		$arVigencia = $arVigencia ? $arVigencia : array();
		$nrAnoFim = substr( $arVigencia[0]["final"], 0, 4 );
		$nrAnoFim = $nrAnoFim-1;
		
		$sql = "select perid, perdtinicioref, perdtterminoref from cte.periodoreferencia";
		$coPeriodo = $db->carregar( $sql );
		$coPeriodo = $coPeriodo ? $coPeriodo : array();
		unset($periQtdCrono);
		unset($periTotalPorcento);
		if ($inuid)
		{
			$arQuantidade = array();
			foreach( $coPeriodo as $arPeriodo ){
				
				$sql = "SELECT perdtterminoref FROM cte.periodoreferencia WHERE perid = ".$arPeriodo["perid"];
				$perTerminoRef = $db->pegaUm($sql);	
				
				//$complemento = "or ( exeatual = true and per.perdtterminoref < '{$perTerminoRef}' and exe.estid = '5' )";
			
				$arExecutado   			 = recuperarQuantidadeSubacoesVigentesPorPeriodo( $arPeriodo, $nrAnoFim, true, $inuid,  $perTerminoRef);
				$nrQuantidadeExecutado   = count( $arExecutado );
				
				$arCronograma  			 = recuperarQuantidadeSubacoesVigentesPorPeriodo( $arPeriodo, $nrAnoFim, false, $inuid );
				$nrQuantidadeCronograma  = count( $arCronograma );				
				
				$arQuantidade[$arPeriodo["perid"]]["executado"]   = $nrQuantidadeExecutado;
				$arQuantidade[$arPeriodo["perid"]]["cronograma"]  = $nrQuantidadeCronograma;
		
				if ($nrQuantidadeCronograma > 0)
				{				
					if($nrQuantidadeExecutado > $nrQuantidadeCronograma)
					{
						$nrQuantidadeExecutado = $nrQuantidadeCronograma;
					}			
					$arQuantidade[$arPeriodo["perid"]]["porcentagem"] = round( ( $nrQuantidadeExecutado/$nrQuantidadeCronograma )*100, 2 );
				} else {
					$arQuantidade[$arPeriodo["perid"]]["porcentagem"] = 0;
				}
				
				$peri[$arPeriodo["perid"]] = $nrQuantidadeCronograma > 0 ? 1 : 0;				
				$periQtdCrono += $peri[$arPeriodo["perid"]];
				$periTotalPorcento += $arQuantidade[$arPeriodo["perid"]]['porcentagem'];			
		
			}
			
			if($periQtdCrono > 0){
				$mediaIndice = $periTotalPorcento/$periQtdCrono;
			}
						
			$mediaIndice = round($mediaIndice, 2);		
			
			$barra = '	<center>
				<div style="float: left;">			
				<div class="barra1">
					<div style="text-align:center; position: absolute; width: 50px; height: 10px; padding: 0 0 0 0;  margin-bottom: 0px; " >
						<div style=" color:'.$cor.'; font-size: 10px; max-height: 10px;  ">'.$mediaIndice.'<span style="color:'.$cor1.'" >%</span></div>
					</div>
				<img class="imgBarra" style="width: '.$mediaIndice.'%; height: 10px" src="../imagens/cor1.gif"/>
				</div>
				</div>
				</center><br>';
				
			$arrListas[$i]['monitora'] = $barra;				
			unset($arrListas[$i]['inuid']);
			unset($arQuantidade);
		}
	}
			
	
	$cabecalho = array( "AÁ„o", "CÛdigo", "MunicÌpio", "UF", "SituaÁ„o", "Monitoramento");
	$db->monta_lista_array( $arrListas, $cabecalho, 20, 10, 'N', '', '' );

?>
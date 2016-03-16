<?php

cte_verificaSessao();
$msg = '';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( '57109', $url, '' );
$titulo_modulo = "PAR - Plano de Metas";
monta_titulo( $titulo_modulo,'Monitoramento do PAR');

$itrid = cte_pegarItrid($_SESSION['inuid']);

//Selecionando as subações cadastradas para monitoramento
$sql = "SELECT  DISTINCT ess.estcor, usu.usunome, 
		to_char(exs.exedtinclusao, 'dd/mm/YYYY') as exedtinclusao, 
		SUBSTR(exedtexecucao,5,2)||'/'||SUBSTR( exedtexecucao,1,4) as exedtexecucao, 
		exs.exeqtd, ess.estdsc, max(  mnt.mntid ) as mntid , d.dimcod, d.dimid, d.dimdsc, a.ardcod, a.ardid, a.arddsc, 
		i.indcod, i.indid, i.inddsc, aim.aciid as acaomunicipalid, aim.acidsc as acaomunicipal,
		saim.sbaid as subacaomunicipalid, saim.sbaordem, saim.sbadsc as subacaomunicipal, saim.sbaordem, usu.usunome, to_char( p.ptodata,'dd/mm/YYYY') as data    
		  FROM cte.indicador i 
			INNER JOIN cte.areadimensao a on a.ardid = i.ardid 
			INNER JOIN cte.dimensao d on d.dimid = a.dimid 
			INNER  JOIN cte.pontuacao p on p.indid = i.indid and p.inuid = ". $_SESSION['inuid'] ." and p.ptostatus = 'A' 
			INNER  JOIN cte.criterio c on c.crtid = p.crtid
			INNER  JOIN cte.acaoindicador aim on aim.ptoid = p.ptoid and aim.acilocalizador = 'M' 
			INNER  JOIN cte.subacaoindicador saim on saim.aciid = aim.aciid 
			INNER  JOIN cte.subacaoparecertecnico spt on saim.sbaid = spt.sbaid
			INNER JOIN cte.proposicaosubacao ps ON ps.ppsid = saim.ppsid
				LEFT JOIN cte.monitoramentosubacao AS mnt ON mnt.sbaid = saim.sbaid 
				LEFT JOIN cte.execucaosubacao AS exs ON exs.mntid = mnt.mntid AND exs.exeatual = TRUE
				LEFT JOIN cte.estadosubacao AS ess ON ess.estid = exs.estid
				LEFT JOIN seguranca.usuario AS usu ON exs.usucpf = usu.usucpf
					INNER JOIN cte.instrumentounidade AS ins ON ins.inuid = p.inuid 
					INNER JOIN workflow.documento AS doc ON doc.docid = ins.docid 
					INNER JOIN workflow.estadodocumento AS est ON est.esdid = doc.esdid 
		WHERE mnt.mntstatus = 'A' 
		AND i.indstatus = 'A' 
		AND a.ardstatus = 'A' 
		AND d.dimstatus = 'A' 
		AND d.itrid = ".$itrid."
		AND saim.frmid IN ( 1, 2, 4, 5, 6, 7, 8, 9, 10 )
		AND spt.ssuid = 3 
		AND ppsindcobuni = false
		AND est.esdid in ( 14, 15, 11, 13)
		
		group by
		 
		ess.estcor, usu.usunome, 
		 exedtinclusao, 
		 exedtexecucao, 
		exs.exeqtd, ess.estdsc,   mnt.mntid   , d.dimcod, d.dimid, d.dimdsc, a.ardcod, a.ardid, a.arddsc, 
		i.indcod, i.indid, i.inddsc,   acaomunicipalid,  acaomunicipal,
		 subacaomunicipalid, saim.sbaordem,  subacaomunicipal, saim.sbaordem, usu.usunome,  data
		
		ORDER BY d.dimcod, a.ardcod, i.indcod,  saim.sbaordem, saim.sbadsc";

//ver($sql);

$registros = $db->carregar($sql);

$cor 	= "#cococo";
$cor1 	= "black";
$porcentagemTotal = 100;
$inuid = $_SESSION["inuid"];

$sql = "select min( perdtinicioref ) as inicio, max( perdtterminoref ) as final from cte.periodoreferencia";
$arVigencia = $db->carregar( $sql );
$arVigencia = $arVigencia ? $arVigencia : array();
$nrAnoFim = substr( $arVigencia[0]["final"], 0, 4 );
$nrAnoFim = $nrAnoFim-1;

$sql = "select perid, perdtinicioref, perdtterminoref, perdsc from cte.periodoreferencia";
$coPeriodo = $db->carregar( $sql );
$coPeriodo = $coPeriodo ? $coPeriodo : array();

$barra = '<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center"><tr><td align="center"><strong>Porcentagens de preenchimento por período</strong><br><table><tr>';

foreach( $coPeriodo as $arPeriodo ){

	$sql = "SELECT perdtterminoref FROM cte.periodoreferencia WHERE perid = ".$arPeriodo["perid"];
	$perTerminoRef = $db->pegaUm($sql);	
	
	//$complemento = "or ( exeatual = true and per.perdtterminoref < '{$perTerminoRef}' and exe.estid = '5' )";

	$arExecutado   			 = recuperarQuantidadeSubacoesVigentesPorPeriodo( $arPeriodo, $nrAnoFim, true, $inuid,  $perTerminoRef);
	$nrQuantidadeExecutado   = count( $arExecutado );
	
	$arCronograma  			 = recuperarQuantidadeSubacoesVigentesPorPeriodo( $arPeriodo, $nrAnoFim, false, $inuid );
	$nrQuantidadeCronograma  = count( $arCronograma );
	
	//ver($arPeriodo["perid"]."_perid", $nrQuantidadeCronograma."_cronograma", $nrQuantidadeExecutado."_executado");
	$arQuantidade[$arPeriodo["perid"]]["executado"]   = $nrQuantidadeExecutado;
	$arQuantidade[$arPeriodo["perid"]]["cronograma"]  = $nrQuantidadeCronograma;
	
//	ver( $nrQuantidadeExecutado, $nrQuantidadeCronograma );
	
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
	
	if($arQuantidade[$arPeriodo["perid"]]["cronograma"] > 0)
	{
		$nrAno = substr($arPeriodo['perdsc'],15,19);
		$barra .= '	
				<td style="padding: 0 10px;">
					<center>			
						<div>
							'.substr($arPeriodo['perdsc'],0,2).'/'.$nrAno.'
							<div class="barra1" title='.$arQuantidade[$arPeriodo["perid"]]["porcentagem"]."%".' alt='.$arQuantidade[$arPeriodo["perid"]]["porcentagem"]."%".'>
								<div style="text-align:center; position: absolute; width: 50px; height: 10px; padding: 0 0 0 0;  margin-bottom: 0px; " >
									<div style=" color:'.$cor.'; font-size: 10px; max-height: 10px;  ">'.$arQuantidade[$arPeriodo["perid"]]["porcentagem"].'<span style="color:'.$cor1.'" >%</span></div>
								</div>
								<img class="imgBarra" style="width: '.$arQuantidade[$arPeriodo["perid"]]["porcentagem"].'%; height: 10px" src="../imagens/cor1.gif"/>
							</div>
						</div>
				<div>';
		$barra .= $arQuantidade[$arPeriodo["perid"]]["porcentagem"] >= 100 ? ' - ' : '<a onclick="verificarPendencias(' . $arPeriodo["perid"] . ', ' . $inuid . ', ' . $nrAno . ' )" id="verificarPendencias">Ver Pendências</a>';
		$barra .= '		</div>
					</center>
				</td>';
	}
}

$barra .= '</tr></table></td></tr></table>';
echo $barra;

if($registros[0]) {
	 
	// Montando arvore com dimensoes, area, indicadores, ações e subações
	foreach($registros as $registro) {
		$_arvore['dimensoes'][$registro['dimid']]['dimdsc'] = $registro['dimdsc'];
		$_arvore['dimensoes'][$registro['dimid']]['dimcod'] = $registro['dimcod'];
		$_arvore['dimensoes'][$registro['dimid']]['areas'][$registro['ardid']]['arddsc'] = $registro['arddsc'];
		$_arvore['dimensoes'][$registro['dimid']]['areas'][$registro['ardid']]['ardcod'] = $registro['ardcod'];
		$_arvore['dimensoes'][$registro['dimid']]['areas'][$registro['ardid']]['indicadores'][$registro['indid']]['inddsc'] = $registro['inddsc'];
		$_arvore['dimensoes'][$registro['dimid']]['areas'][$registro['ardid']]['indicadores'][$registro['indid']]['indcod'] = $registro['indcod'];
		$_arvore['dimensoes'][$registro['dimid']]['areas'][$registro['ardid']]['indicadores'][$registro['indid']]['acoes'][$registro['acaomunicipalid']]['acidsc'] = $registro['acaomunicipal'];
		$_arvore['dimensoes'][$registro['dimid']]['areas'][$registro['ardid']]['indicadores'][$registro['indid']]['acoes'][$registro['acaomunicipalid']]['subacoes'][$registro['subacaomunicipalid']]['sbadsc'] = $registro['subacaomunicipal'];
		$_arvore['dimensoes'][$registro['dimid']]['areas'][$registro['ardid']]['indicadores'][$registro['indid']]['acoes'][$registro['acaomunicipalid']]['subacoes'][$registro['subacaomunicipalid']]['mntid'] = $registro['mntid'];
		$_arvore['dimensoes'][$registro['dimid']]['areas'][$registro['ardid']]['indicadores'][$registro['indid']]['acoes'][$registro['acaomunicipalid']]['subacoes'][$registro['subacaomunicipalid']]['estcor'] = $registro['estcor'];
		$_arvore['dimensoes'][$registro['dimid']]['areas'][$registro['ardid']]['indicadores'][$registro['indid']]['acoes'][$registro['acaomunicipalid']]['subacoes'][$registro['subacaomunicipalid']]['usunome'] = $registro['usunome'];
		$_arvore['dimensoes'][$registro['dimid']]['areas'][$registro['ardid']]['indicadores'][$registro['indid']]['acoes'][$registro['acaomunicipalid']]['subacoes'][$registro['subacaomunicipalid']]['exedtinclusao'] = $registro['exedtinclusao'];
		$_arvore['dimensoes'][$registro['dimid']]['areas'][$registro['ardid']]['indicadores'][$registro['indid']]['acoes'][$registro['acaomunicipalid']]['subacoes'][$registro['subacaomunicipalid']]['estdsc'] = $registro['estdsc'];
		$_arvore['dimensoes'][$registro['dimid']]['areas'][$registro['ardid']]['indicadores'][$registro['indid']]['acoes'][$registro['acaomunicipalid']]['subacoes'][$registro['subacaomunicipalid']]['exedtexecucao'] = $registro['exedtexecucao'];
		$_arvore['dimensoes'][$registro['dimid']]['areas'][$registro['ardid']]['indicadores'][$registro['indid']]['acoes'][$registro['acaomunicipalid']]['subacoes'][$registro['subacaomunicipalid']]['exeqtd'] = $registro['exeqtd'];
		$_arvore['dimensoes'][$registro['dimid']]['areas'][$registro['ardid']]['indicadores'][$registro['indid']]['acoes'][$registro['acaomunicipalid']]['subacoes'][$registro['subacaomunicipalid']]['sbaordem'] = $registro['sbaordem'];
		 
	}
}

// Função que delimita um texto em 100 caracteres
function delimitador($texto){
	if(strlen($texto) > 100){
		$texto = substr($texto,0,100).'...';
	}
	return $texto;
}

// Cria espaçamento para identar na arvore 
function espacamento($espaco) {
	for($i=0;$i<$espaco;$i++) {
		$prefixo .= "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
	}
	return $prefixo;
}

?>
<link rel="stylesheet" type="text/css" href="../../cte/monitoraFinanceiro/includes/monitora_financeiro.css"/>
<script>
function monitorarSubacao(mntid) {
	return window.open('?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid='+mntid+'&perano=2008','monitorasubacao','height=600,width=850,status=no,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function fecharTodos() {
	var arvore = document.getElementById( 'arvoremonitora' );		
	var linhas =  arvore.getElementsByTagName("tr");	
	var imagens = arvore.getElementsByTagName("img");
			
	for(i = 0; i < linhas.length; i++ ){
		if(linhas[i].id.indexOf('_') > -1){
			linhas[i].style.display = 'none';                      
		}
	}
				
	for(i = 0; i < imagens.length; i++ ){			
		if(imagens[i].src.indexOf('menos') > -1){	
			imagens[i].src = imagens[i].src.replace( "menos" , "mais" );
		}
	}		
}

function abrirTodos() {
	var arvore = document.getElementById('arvoremonitora');
	var linhas =  arvore.getElementsByTagName("tr");	
	var imagens = arvore.getElementsByTagName("img");	
	for(i = 0; i < linhas.length; i++ ){
		if(linhas[i].id.indexOf('_')){							
			linhas[i].style.display = document.all ? 'block' : 'table-row';				                      
		}
	}	
		
	for(i = 0; i < imagens.length; i++ ){			
		if(imagens[i].src.indexOf('mais') > -1){	
			imagens[i].src = imagens[i].src.replace( "mais" , "menos" );
		}
	}			
}

function abrearvore(idpai) {
	var img = document.getElementById('btn_'+idpai);
	if(img.title == 'abrir') {
		var title = 'fechar';
		var link = '/imagens/menos.gif';
		img.title = title;
    	img.src = link;
    	var acao = '';
    } else {
		var title = 'abrir';
		var link = '/imagens/mais.gif'
    	img.title = title;
    	img.src = link;
    	var acao = 'none';
    }
    
	var tabelamonitora = document.getElementById('arvoremonitora');
	for(i=0;i<tabelamonitora.rows.length;i++) {
		if(tabelamonitora.rows[i].id.search(idpai+'_') != -1) {
			var imagem = document.getElementById('btn_'+tabelamonitora.rows[i].id);
			if(imagem != null) {
				imagem.title = title;
				imagem.src = link;
			}
			tabelamonitora.rows[i].style.display = acao;
		}
	}
}

function verificarPendencias( perid, inuid, nrAno ){
	return windowOpen('cte.php?modulo=principal/verificarPendenciasMonitoramento&acao=A&perid='+perid+'&inuid='+inuid+'&nrAno='+nrAno, 'blank', 'height=600,width=750,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
}

function relatorioImpressao( ptostatus ){
	var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=relatorio/impressao_monitoramento&acao=A&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
}

</script>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
<tr>
<td>
<h3 title="Unidade da Federação">
<?
// Pegando o nome da UF e municipio
$muncod = cte_pegarMuncod($_SESSION['inuid']);
$descricaom = cte_pegarMundescricao($muncod);
echo '<a title="Ir para o PAR do Município" href="?modulo=principal/estrutura_avaliacao&acao=A">'.$descricaom.'</a>';

?>
</h3>
<!--  
<?php if( cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR ) ) ){ ?>
	<br />
	<a href="?modulo=principal/verificarCumprimentoDiretrizes&acao=A">Verificar Cumprimento das Diretrizes</a>
	<br /><br />
<?php } ?>
-->
</td>
</tr>
<? if($_arvore['dimensoes']) { ?>
<tr>
	<td>
		<a style="cursor: pointer" onclick="return false;"> 
			<img src="/imagens/print.png" alt="Versão para impressão" onclick="relatorioImpressao( 'A' )"> 
		</a>
	</td>
</tr>
<tr><td><a href="javascript:void(0);" onclick="abrirTodos();">Abrir Todos</a>   |  <a href="javascript:void(0);" onclick="fecharTodos();">Fechar Todos</a></td></tr>
</table>
<div id="porceto"></div>
<table class="tabela" cellpadding="3" align="center" id="arvoremonitora">
<thead>
<tr style="background-color: rgb(224, 224, 224);">
	<td style="font-weight: bold; text-align: center;">Dimensões</td>
	<td style="font-weight: bold; text-align: center;">Programa</td>
	<td style="font-weight: bold; text-align: center;">Forma de Execução</td>
	<td style="font-weight: bold; text-align: center;">Qtde. Planejada</td>
	<td style="font-weight: bold; text-align: center;">Unidade de Medida</td>
	<td style="font-weight: bold; text-align: center;">Situação</td>
	<td style="font-weight: bold; text-align: center;">Data da execução</td>
	<td style="font-weight: bold; text-align: center;">Quantidade</td>
</tr>
</thead>

<?

$contaSubMonitora = 0;
$contaSubSemMonitora = 0;
$dimensoescolor = 0;

//Montando a arvore com dimensões, areas, indicadores, ações e subações
foreach($_arvore['dimensoes'] as $dimid => $dimensao) {
	echo "<tr id='nivel".$dimid."' ".(($dimensoescolor%2)?"onmouseover=\"this.style.backgroundColor='#ffffcc';\" onmouseout=\"this.style.backgroundColor='#fafafa';\" style=\"background-color: rgb(250, 250, 250);\"":"onmouseover=\"this.style.backgroundColor='#ffffcc';\" onmouseout=\"this.style.backgroundColor='#f0f0f0';\" style=\"background-color: rgb(240, 240, 240);\"").">
		  <td onmouseover=\"return escape('".$dimensao['dimdsc']."');\"><img align=\"absmiddle\" src=\"../imagens/seta_filho.gif\"/> <img src='/imagens/menos.gif' border='0' title='fechar' style='cursor:pointer' id='btn_nivel".$dimid."' onclick=\"abrearvore('nivel".$dimid."');\"> ".$dimensao['dimcod'].". ".delimitador($dimensao['dimdsc'])."</td>
		  <td>&nbsp;</td>
		  <td>&nbsp;</td>
		  <td>&nbsp;</td>
		  <td>&nbsp;</td>
		  <td>&nbsp;</td>
		  <td>&nbsp;</td>
		  <td>&nbsp;</td>
		  </tr>";
	$dimensoescolor++;
	$areascolor = 0;
	foreach($dimensao['areas'] as $ardid => $area) {
		echo "<tr id='nivel".$dimid."_".$ardid."' ".(($areascolor%2)?"onmouseover=\"this.style.backgroundColor='#ffffcc';\" onmouseout=\"this.style.backgroundColor='#fafafa';\" style=\"background-color: rgb(250, 250, 250);\"":"onmouseover=\"this.style.backgroundColor='#ffffcc';\" onmouseout=\"this.style.backgroundColor='#f0f0f0';\" style=\"background-color: rgb(240, 240, 240);\"").">
			  <td onmouseover=\"return escape('".$area['arddsc']."');\">".espacamento(1)."<img align=\"absmiddle\" src=\"../imagens/seta_filho.gif\"/> <img src='/imagens/menos.gif' border='0' title='fechar' style='cursor:pointer' id='btn_nivel".$dimid."_".$ardid."' onclick=\"abrearvore('nivel".$dimid."_".$ardid."');\"> ".$area['ardcod'].". ".delimitador($area['arddsc'])."</td>
			  <td>&nbsp;</td>
			  <td>&nbsp;</td>
			  <td>&nbsp;</td>
			  <td>&nbsp;</td>
			  <td>&nbsp;</td>
			  <td>&nbsp;</td>
			  <td>&nbsp;</td>
			  </tr>";
		$areascolor++;
		$indicadorescolor = 0;
		foreach($area['indicadores'] as $indid => $indicador) {
			echo "<tr id='nivel".$dimid."_".$ardid."_".$indid."' ".(($indicadorescolor%2)?"onmouseover=\"this.style.backgroundColor='#ffffcc';\" onmouseout=\"this.style.backgroundColor='#fafafa';\" style=\"background-color: rgb(250, 250, 250);\"":"onmouseover=\"this.style.backgroundColor='#ffffcc';\" onmouseout=\"this.style.backgroundColor='#f0f0f0';\" style=\"background-color: rgb(240, 240, 240);\"").">
					<td onmouseover=\"return escape('".$indicador['inddsc']."');\">".espacamento(2)."<img align=\"absmiddle\" src=\"../imagens/seta_filho.gif\"/> <img src='/imagens/menos.gif' border='0' title='fechar' style='cursor:pointer' id='btn_nivel".$dimid."_".$ardid."_".$indid."' onclick=\"abrearvore('nivel".$dimid."_".$ardid."_".$indid."');\"> ".$indicador['indcod'].". ".delimitador($indicador['inddsc'])."</td>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
					<td>&nbsp;</td>
				  </tr>";
			$indicadorescolor++;
			$acoescolor = 0;			
			foreach($indicador['acoes'] as $aciid => $acao) {
				echo "<tr id='nivel".$dimid."_".$ardid."_".$indid."_".$aciid."' ".(($acoescolor%2)?"onmouseover=\"this.style.backgroundColor='#ffffcc';\" onmouseout=\"this.style.backgroundColor='#fafafa';\" style=\"background-color: rgb(250, 250, 250);\"":"onmouseover=\"this.style.backgroundColor='#ffffcc';\" onmouseout=\"this.style.backgroundColor='#f0f0f0';\" style=\"background-color: rgb(240, 240, 240);\"").">
						<td onmouseover=\"return escape('".$acao['acidsc']."');\">".espacamento(3)."<img align=\"absmiddle\" src=\"../imagens/seta_filho.gif\"/> <img src='/imagens/menos.gif' border='0' title='fechar' style='cursor:pointer' id='btn_nivel".$dimid."_".$ardid."_".$indid."_".$aciid."' onclick=\"abrearvore('nivel".$dimid."_".$ardid."_".$indid."_".$aciid."');\"> ".delimitador($acao['acidsc'])."</td>
						<td>&nbsp;</td>
						<td>&nbsp;</td>
						<td>&nbsp;</td>
						<td>&nbsp;</td>
						<td>&nbsp;</td>
						<td>&nbsp;</td>
						<td>&nbsp;</td>
					  </tr>";
				$acoescolor++;
				$subacaocolor = 0;
				$totalSubacoes = 0;				
				foreach($acao['subacoes'] as $sbaid => $subacao) {					
					unset($icones);
					$restricoes = $db->pegaUm("SELECT resid FROM cte.restricaoexecucao WHERE mntid='".$subacao['mntid']."' LIMIT 1");
					if($restricoes) {
						$icones .= "<img src='../imagens/restricao_ico.png' align=\"absmiddle\" title='Existem restrições no monitoramento'> ";
					}
					$execucoes = $db->pegaUm("SELECT exeid FROM cte.execucaosubacao WHERE mntid='".$subacao['mntid']."' LIMIT 1");
					if($execucoes) {
						$icones .= "<img src='../imagens/check_p.gif' align=\"absmiddle\" title='Existem execuções no monitoramento'> ";
					}
					$anexos = $db->pegaUm("SELECT aneid FROM cte.anexomonitoramento WHERE mntid='".$subacao['mntid']."' LIMIT 1");
					if($anexos) {
						$icones .= "<img src='../imagens/clipe.gif' align=\"absmiddle\" title='Existem documentos no monitoramento'> ";
					}
					$sql = "-- CRONOGRAMA GLOBAL
							SELECT 
							prg.prgdsc, 
							ume.unddsc as unidademedida, 
							frm.frmdsc as formaexecucao, 
							si.sbadsc as descricaosubacao, 
							spt.sptano as ano,
							si.sbaid, 
							spt.sptunt as quantidade, spt.sptinicio as mesinicio, spt.sptfim as mesfim
        					FROM cte.pontuacao p
	        					INNER JOIN cte.acaoindicador ai ON ai.ptoid = p.ptoid
	        					INNER JOIN cte.subacaoindicador si ON si.aciid = ai.aciid
	        					LEFT JOIN cte.programa prg ON prg.prgid = si.prgid
	        					LEFT  JOIN cte.formaexecucao frm ON si.frmid = frm.frmid
	        					INNER JOIN cte.unidademedida ume ON si.undid = ume.undid
	        					INNER JOIN cte.subacaoparecertecnico spt ON si.sbaid = spt.sbaid
        					WHERE p.ptostatus = 'A' 
        					AND si.sbaid = '".$sbaid."'
        					AND spt.ssuid = 3
        					AND coalesce(spt.sptano, 0) <> 0 
        					-- AND coalesce(spt.sptano, 0) = ".date("Y")."
        					AND si.sbaporescola = false 
        					-- AND coalesce(spt.ssuid, 0) = 3
							UNION ALL
							-- CRONOGRAMA POR ESCOLA
							SELECT prg.prgdsc,  ume.unddsc as unidademedida, frm.frmdsc as formaexecucao, si.sbadsc as descricaosubacao, spt.sptano as ano ,si.sbaid,
							count(1) as quantidade, spt.sptinicio as mesinicio, spt.sptfim as mesfim
        					FROM cte.pontuacao p
	        					INNER JOIN cte.acaoindicador ai ON ai.ptoid = p.ptoid
	        					INNER JOIN cte.subacaoindicador si ON si.aciid = ai.aciid
	        					LEFT JOIN cte.programa prg ON prg.prgid = si.prgid
	        					LEFT JOIN cte.formaexecucao frm ON si.frmid = frm.frmid
	        					INNER JOIN cte.unidademedida ume ON si.undid = ume.undid
	        					INNER JOIN cte.subacaoparecertecnico spt ON si.sbaid = spt.sbaid
	        					LEFT JOIN cte.qtdfisicoano qfa ON si.sbaid = qfa.sbaid
        					WHERE p.ptostatus = 'A' 
        					AND si.sbaid = '".$sbaid."' 
        					AND spt.ssuid = 3
        					AND coalesce(qfa.qfaano, 0) <> 0
        					-- AND coalesce(qfa.qfaano, 0) = ".date("Y")." 
        					-- AND coalesce(spt.ssuid, 0) = 3
							GROUP BY ume.unddsc, si.sbaid,  frm.frmdsc, si.sbadsc, spt.sptano, spt.sptinicio, spt.sptfim, prg.prgdsc ORDER BY ano ASC";
					$subacaoindicador = $db->pegaLinha($sql);
				
					$sql = "select e.perid,   perdsc, es.estid, es.estdsc, es.estcor from cte.execucaosubacao e
								inner join cte.estadosubacao es on e.estid = es.estid
								inner join cte.periodoreferencia p on e.perid = p.perid
							where mntid = '".$subacao["mntid"]."'
							and exeatual = true
							order by   perid, perdtiniciocad";
					
					$resultado = $db->carregar( $sql );					
					
					if( is_array( $resultado ) ){
						$stSituacao = "<table>";
						foreach( (array) $resultado as $arExecucao ){
							$stSituacao .= "
							<tr>
								<td style='color: {$arExecucao['estcor']}; width: 250px;' >".str_replace( " Semestre de ", "/", $arExecucao["perdsc"] ).": {$arExecucao["estdsc"]}</td>
							</tr>";
						}
						$stSituacao .= "</table>";
					}
					else{
						$stSituacao = "";
					}
					
					//COLOCAR COR NA SUBAÇÃO CONFORME ANO						
					if( ( $subacaoindicador['ano'] > date("Y")) ) {
						$blockColor = "color: gray;";	
										
					}else{
						$blockColor = "color: #133368;";	
						$contadorthi++;					
					}
					
					$sptunt['sum'] = "";
					// Recupera total previstos
					if($subacaoindicador['sbaid']){
						
						$sql = "SELECT sum(sptunt) FROM cte.subacaoparecertecnico WHERE sbaid = '".$subacaoindicador['sbaid']."'";
						$sptunt = $db->pegaLinha($sql);
						
						$sql = "SELECT
								--'' as qfaid,
								--e.entcodent,
								--e.entnome,
								--sum(coalesce(qfaqtd,0))
								count(*) as total
							FROM cte.qtdfisicoano q
							INNER JOIN entidade.entidade e on q.entid = e.entid
							WHERE q.sbaid  = ".$subacaoindicador['sbaid']."
							GROUP BY q.qfaid, e.entcodent, e.entnome
							ORDER BY entnome";
					
						$sptunt2 = $db->pegaLinha($sql);
						$sptunt['sum'] = $sptunt['sum'] ? $sptunt['sum'] : $sptunt2['total'];
					
					}
					
					echo "<tr id='nivel".$dimid."_".$ardid."_".$indid."_".$aciid."_".$sbaid."' ".(($subacaocolor%2)?"onmouseover=\"this.style.backgroundColor='#ffffcc';\" onmouseout=\"this.style.backgroundColor='#fafafa';\" style=\"background-color: rgb(250, 250, 250);\"":"onmouseover=\"this.style.backgroundColor='#ffffcc';\" onmouseout=\"this.style.backgroundColor='#f0f0f0';\" style=\"background-color: rgb(240, 240, 240);\"").">
							<td onmouseover=\"return escape('Autor : ".$subacao['usunome']."<br>Data de criação : ".$subacao['exedtinclusao']."<br />Subação : ".$subacao['sbadsc']."');\">".espacamento(4)."<img align=\"absmiddle\" src=\"../imagens/seta_filho.gif\"/> ".$icones." 
								<a style='font-weight: bold; $blockColor' href='javascript:void(0);' onclick='monitorarSubacao(".$subacao['mntid'].")'>".$dimensao['dimcod'].'.'.$area['ardcod'].'.'.$indicador['indcod'].' - '.$subacao['sbaordem']." ".delimitador($subacao['sbadsc'])."</a>
							</td>
							<td>".$subacaoindicador['prgdsc']."</td>
							<td>".$subacaoindicador['formaexecucao']."</td>
							<td>".$sptunt['sum']."</td>
							<td>".$subacaoindicador['unidademedida']."</td>
							<td align='center' style='color:".$subacao['estcor']."'>".$stSituacao."</td>
							<td align='center'>".$subacao['exedtexecucao']."</td>
							<td align='center'>".$subacao['exeqtd']."</td>
						  </tr>";
					$subacaocolor++;					
				}				
			}
		}
	}
}
} else {
?>
<tr><td style="text-align: center; border: 1px solid black;" class="SubTituloCentro">O PAR do município estará disponível para monitoramento após a geração do Termo de Cooperação Técnica.</td></tr>
<?
}

//ver($_SESSION['inuid']."_inuid", $sbaid."_sbaid", $subacao['mntid']."_mntid", $arQuantidade );
?>
</table>
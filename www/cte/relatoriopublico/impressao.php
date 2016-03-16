<?
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/Snoopy.class.php";
include "funcoes.php";

$db = new cls_banco();

$inuid 		= $_REQUEST['inuid'];
$itrid 		= $_REQUEST['itrid'];
$municipio 	= $_REQUEST['muncod'];
$municod	= $_REQUEST['municod'];
$uf 		= $_REQUEST['estuf'];  

/********* Funções ********/

function recuperaQuantidades($sbaid, $cronograma){
	global $db;
	if($cronograma == 't'){
		$sqlQuantidade="SELECT
							sum(ano2007) AS ano2007, -- QUANTIDADE 2007
							sum(ano2008) AS ano2008, -- QUANTIDADE 2008
							sum(ano2009) AS ano2009, -- QUANTIDADE 2009 
							sum(ano2010) AS ano2010, -- QUANTIDADE 2010
							sum(ano2011) AS ano2011,  -- QUANTIDADE 2011 
							sum(qfaqtd)  AS qfaqtd
						FROM (	
						SELECT 
						CASE WHEN qfaano = '2007'  THEN sum(qfaqtd) END AS ano2007, -- QUANTIDADE 2007
						CASE WHEN qfaano = '2008'  THEN sum(qfaqtd) END AS ano2008, -- QUANTIDADE 2008
						CASE WHEN qfaano = '2009'  THEN sum(qfaqtd) END AS ano2009, -- QUANTIDADE 2009 
						CASE WHEN qfaano = '2010'  THEN sum(qfaqtd) END AS ano2010, -- QUANTIDADE 2010
						CASE WHEN qfaano = '2011'  THEN sum(qfaqtd) END AS ano2011, -- QUANTIDADE 2011
						sum(qfaqtd) as qfaqtd  
						FROM cte.qtdfisicoano 
						WHERE sbaid = '".$sbaid."'
						GROUP BY qfaano
						) AS quantidade";
		$quantidade = $db->carregar($sqlQuantidade);
		return $quantidade;
	}else{
		$sqlQuantidade = "
						SELECT
						sum(ano2007) AS ano2007, -- QUANTIDADE 2007
						sum(ano2008) AS ano2008, -- QUANTIDADE 2008
						sum(ano2009) AS ano2009, -- QUANTIDADE 2009 
						sum(ano2010) AS ano2010, -- QUANTIDADE 2010
						sum(ano2011) AS ano2011,  -- QUANTIDADE 2011 
						sum(sptunt)  AS sptunt
						FROM (
							SELECT  
							CASE WHEN sptano = '2007'  THEN sum(sptunt) END AS ano2007, -- QUANTIDADE 2007
							CASE WHEN sptano = '2008'  THEN sum(sptunt) END AS ano2008, -- QUANTIDADE 2008
							CASE WHEN sptano = '2009'  THEN sum(sptunt) END AS ano2009, -- QUANTIDADE 2009 
							CASE WHEN sptano = '2010'  THEN sum(sptunt) END AS ano2010, -- QUANTIDADE 2010
							CASE WHEN sptano = '2011'  THEN sum(sptunt) END AS ano2011, -- QUANTIDADE 2011
							sum(sptunt)  AS sptunt
							FROM cte.subacaoparecertecnico where sbaid =  '".$sbaid."' and ssuid = 3
							GROUP BY sptano
						) AS quantidade
		";
		$quantidade = $db->carregar($sqlQuantidade);
		return $quantidade;	
	}
}

function racuperaValor($sbaid)
{
    global $db;

    if (($sptunt_2007 = $db->pegaUm('select sptunt from cte.subacaoparecertecnico where sbaid = ' . $sbaid . ' and sptano = 2007')) == false)
        $sptunt_2007 = 1;

    if (($sptunt_2008 = $db->pegaUm('select sptunt from cte.subacaoparecertecnico where sbaid = ' . $sbaid . ' and sptano = 2008')) == false)
        $sptunt_2008 = 1;

    if (($sptunt_2009 = $db->pegaUm('select sptunt from cte.subacaoparecertecnico where sbaid = ' . $sbaid . ' and sptano = 2009')) == false)
        $sptunt_2009 = 1;

    if (($sptunt_2010 = $db->pegaUm('select sptunt from cte.subacaoparecertecnico where sbaid = ' . $sbaid . ' and sptano = 2010')) == false)
        $sptunt_2010 = 1;

    if (($sptunt_2011 = $db->pegaUm('select sptunt from cte.subacaoparecertecnico where sbaid = ' . $sbaid . ' and sptano = 2011')) == false)
        $sptunt_2011 = 1;

    $sql = '
    SELECT
        sum(ano2007) AS ano2007,
        sum(ano2008) AS ano2008,
        sum(ano2009) AS ano2009,
        sum(ano2010) AS ano2010,
        sum(ano2011) AS ano2011
    FROM (
        SELECT
        CASE WHEN cosano = 2007 THEN (cosqtd * cosvlruni)  END AS ano2007,
        CASE WHEN cosano = 2008 THEN (cosqtd * cosvlruni)  END AS ano2008,
        CASE WHEN cosano = 2009 THEN (cosqtd * cosvlruni)  END AS ano2009,
        CASE WHEN cosano = 2010 THEN (cosqtd * cosvlruni)  END AS ano2010,
        CASE WHEN cosano = 2011 THEN (cosqtd * cosvlruni)  END AS ano2011
    FROM cte.composicaosubacao where sbaid = ' . $sbaid . ') AS valores';

    return (array) $db->carregar($sql);
}

function recuperaDadosSub($sbaid){
	global $db;

		$sql="SELECT 
		sum(sba0ini) AS sba0ini, 
		sum(sba1ini) AS sba1ini, 
		sum(sba2ini) AS sba2ini, 
		sum(sba3ini) AS sba3ini, 
		sum(sba4ini) AS sba4ini, 
		
		sum(sba0fim) AS sba0fim, 
		sum(sba1fim) AS sba1fim, 
		sum(sba2fim) AS sba2fim, 
		sum(sba3fim) AS sba3fim, 
		sum(sba4fim) AS sba4fim
		FROM (
			SELECT 			
			CASE WHEN subp.sptano = '2007'  THEN coalesce( subp.sptinicio , 0 )END AS sba0ini, 
			CASE WHEN subp.sptano = '2008'  THEN coalesce( subp.sptinicio , 0 )END AS sba1ini, 
			CASE WHEN subp.sptano = '2009'  THEN coalesce( subp.sptinicio , 0 )END AS sba2ini, 
			CASE WHEN subp.sptano = '2010'  THEN coalesce( subp.sptinicio , 0 )END AS sba3ini, 
			CASE WHEN subp.sptano = '2011'  THEN coalesce( subp.sptinicio , 0 )END AS sba4ini, 
			
			CASE WHEN subp.sptano = '2007'  THEN coalesce( subp.sptfim , 0 )END AS sba0fim, 
			CASE WHEN subp.sptano = '2008'  THEN coalesce( subp.sptfim , 0 )END AS sba1fim, 
			CASE WHEN subp.sptano = '2009'  THEN coalesce( subp.sptfim , 0 )END AS sba2fim, 
			CASE WHEN subp.sptano = '2010'  THEN coalesce( subp.sptfim , 0 )END AS sba3fim, 
			CASE WHEN subp.sptano = '2011'  THEN coalesce( subp.sptfim , 0 )END AS sba4fim
			FROM	cte.subacaoindicador subacao
			LEFT JOIN cte.subacaoparecertecnico subp ON subp.sbaid = subacao.sbaid
			WHERE	subacao.sbaid = '".$sbaid."' and subp.ssuid = 3
		) AS dados";
	return $db->carregar($sql);
	
}

function ordenarArray( $a, $b ){
	return strcmp( strtolower( $a["cosdsc"] ), strtolower( $b["cosdsc"] ) );
} 

function mostra_subacao(){
	global $db;
	global $dado, $i, $totalreg;
	global $total, $totalGeralAno0 ,$totalGeralAno1, $totalGeralAno2, $totalGeralAno3, $totalGeralAno4;
	global $totalGeralIndicadorAno0 , $totalGeralIndicadorAno1, $totalGeralIndicadorAno2, $totalGeralIndicadorAno3, $totalGeralIndicadorAno4;
	global $totalGeralAreaAno0 , $totalGeralAreaAno1, $totalGeralAreaAno2, $totalGeralAreaAno3, $totalGeralAreaAno4;
	global $totalGeralDimensaAno0 ,$totalGeralDimensaAno1, $totalGeralDimensaAno2, $totalGeralDimensaAno3, $totalGeralDimensaAno4;
	global $novaDimensao, $novaArea, $novoIndicador, $novaAcao, $novasubAcao;
	global $tipo, $demandaE, $demandaM ;

	if($tipo == "Estadual"){
		$demanda = $demandaE;
	}else if($tipo == "Municipal"){
		$demanda = $demandaM;
	}

	if( $demanda > 0 ){

		if($novasubAcao != NULL){
			$sub 		= recuperaDadosSub($novasubAcao);
			$valor = 0;
			$valor 		= racuperaValor($novasubAcao);
			$quantidade = recuperaQuantidades($novasubAcao, $dado[$i]['sbaporescola']);
	
			//$valor2008 = $valor[0]['ano2008'];
			
			$valor2007 = $valor[0]['ano2007'] / ($quantidade[0]['ano2007'] > 0 ? $quantidade[0]['ano2007'] : 1) ;
			$valor2008 = $valor[0]['ano2008'] / ($quantidade[0]['ano2008'] > 0 ? $quantidade[0]['ano2008'] : 1);
			$valor2009 = $valor[0]['ano2009'] / ($quantidade[0]['ano2009'] > 0 ? $quantidade[0]['ano2009'] : 1);
			$valor2010 = $valor[0]['ano2010'] / ($quantidade[0]['ano2010'] > 0 ? $quantidade[0]['ano2010'] : 1);
			$valor2011 = $valor[0]['ano2011'] / ($quantidade[0]['ano2011'] > 0 ? $quantidade[0]['ano2011'] : 1);
			$existevalor = $valor2007 + $valor2008 + $valor2009 + $valor2010 + $valor2011;
			
			//$teste = $valor[0]['ano2008'] / ($quantidade[0]['ano2008'] > 0 ? $quantidade[0]['ano2008'] : 1);
		}
	
		if($dado[$i]['sbaporescola'] == 't'){
			$valorGeral = $quantidade[0]['qfaqtd'];
		}else{
			$valorGeral = $quantidade[0]['sptunt'];
		}
		
		for($x=0; $x < 5; $x++){
			switch($sub[0]['sba'.$x.'ini'] ) {
			case '1':
				$sub[0]['sba'.$x.'ini'] = "janeiro";
				break; 
			case '2':
				$sub[0]['sba'.$x.'ini'] = "fevereiro";
				break; 
			case '3':
				$sub[0]['sba'.$x.'ini'] = "março";
				break; 
			case '4':
				$sub[0]['sba'.$x.'ini'] = "abril";
				break; 
			case '5':
				$sub[0]['sba'.$x.'ini'] = "maio";
				break; 	
			case '6':
				$sub[0]['sba'.$x.'ini'] = "junho";
				break;
			case '7':
				$sub[0]['sba'.$x.'ini'] = "julho";
				break;
			case '8':
				$sub[0]['sba'.$x.'ini'] = "agosto";
				break;
			case '9':
				$sub[0]['sba'.$x.'ini'] = "setembro";
				break;
			case '10':
				$sub[0]['sba'.$x.'ini'] = "outubro";
				break;
			case '11':
				$sub[0]['sba'.$x.'ini'] = "novembro";
				break;
			case '12':
				$sub[0]['sba'.$x.'ini'] = "dezembro";
				break;
			}
			
			switch($sub[0]['sba'.$x.'fim'] ) {
			case '1':
				$sub[0]['sba'.$x.'fim'] = "janeiro";
				break; 
			case '2':
				$sub[0]['sba'.$x.'fim'] = "fevereiro";
				break; 
			case '3':
				$sub[0]['sba'.$x.'fim'] = "março";
				break; 
			case '4':
				$sub[0]['sba'.$x.'fim'] = "abril";
				break; 
			case '5':
				$sub[0]['sba'.$x.'fim'] = "maio";
				break; 	
			case '6':
				$sub[0]['sba'.$x.'fim'] = "junho";
				break;
			case '7':
				$sub[0]['sba'.$x.'fim'] = "julho";
				break;
			case '8':
				$sub[0]['sba'.$x.'fim'] = "agosto";
				break;
			case '9':
				$sub[0]['sba'.$x.'fim'] = "setembro";
				break;
			case '10':
				$sub[0]['sba'.$x.'fim'] = "outubro";
				break;
			case '11':
				$sub[0]['sba'.$x.'fim'] = "novembro";
				break;
			case '12':
				$sub[0]['sba'.$x.'fim'] = "dezembro";
				break;
			}
			
		}
		
		?>
						<tr>
							<td class="SubTituloDireita">
								<b>Sub-Ação</b>
							</td>
							<td class="" style="text-align:left">	
							
								<table  class="tabela" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" >
									<tr>
										<td  class="SubTituloDireita" style="width:20%">
											Descrição da Subação:
										</td>
										
										<td>
											<?=$dado[$i]['sbadsc']; ?>
										</td>
									</tr>									
									<tr>
										<td  class="SubTituloDireita">
											Estratégia de Implementação:
										</td>
										
										<td>
											<?=$dado[$i]['sbastgmpl']; ?>
										</td>
									</tr>
									<tr>
										<td  class="SubTituloDireita">
											Programa:
										</td>
										
										<td>
											<?=$dado[$i]['sbaprm']; ?>
										</td>
									</tr>
									<tr>
										<td  class="SubTituloDireita">
											Unidade de Medida:
										</td>
										
										<td>
											<?=$dado[$i]['unddsc']; ?>
										</td>
									</tr>
									<tr>
										<td  class="SubTituloDireita">
											Forma de Execução
										</td>
										
										<td>
											<?=$dado[$i]['frmdsc'];?>
										</td>
									</tr>
									<tr>
										<td  class="SubTituloDireita">
											Instituição Parceira (se houver):
										</td>
										
										<td>
											<?=$dado[$i]['sbapcr']; ?>
										</td>
									</tr>
									<tr>
										<td  class="SubTituloDireita">
											Quantidades e Cronograma Físico
										</td>									
										<td>
											<table class="tabela" width="100%">
												<tr>
													<th>
														&nbsp;	
													</th>	
													<th align="center">
															<b>2007</b>
													</th>											
													<th align="center">
															<b>2008</b>
													</th>																								
													<th align="center">
															<b>2009</b>
													</th>
													<th align="center">
															<b>2010</b>
													</th>
													<th align="center">
															<b>2011</b>
													</th>
													<th align="center">
														<b>Total</b>
													</th>
													</tr>									
												<tr>
													<td align="right">
														<b>Quantidades:</b>
													</td>
													<td align="right">
														<?=number_format($quantidade[0]['ano2007'],0); ?>
													</td>
													<td align="right">
														<?=number_format($quantidade[0]['ano2008'],0); ?>
													</td>
													<td align="right">
														<?=number_format($quantidade[0]['ano2009'],0); ?>
													</td>
													<td align="right">
														<?=number_format($quantidade[0]['ano2010'],0); ?>
													</td>
													<td align="right">
														<?=number_format($quantidade[0]['ano2011'],0); ?>
													</td>
													<td align="right">
														<?
														$total = 	
														$quantidade[0]['ano2007'] +
														$quantidade[0]['ano2008'] +
														$quantidade[0]['ano2009'] +
														$quantidade[0]['ano2010'] +
														$quantidade[0]['ano2011'];
														echo number_format($total,0);
														 ?>
													</td>
												</tr>											
												<tr>
													<td align="right">
														<b>Cronograma Físico:</b>
													</td>
													<td align="right">
														<?if(!Empty($sub[0]['sba0ini'])) echo $sub[0]['sba0ini'] . " até " ; ?>  <?=$sub[0]['sba0fim']; ?>
													</td>
													<td align="right">
														<?if(!Empty($sub[0]['sba1ini'])) echo $sub[0]['sba1ini'] . " até " ; ?>  <?=$sub[0]['sba1fim']; ?>
													</td>
													
													<td align="right">
														<?if(!Empty($sub[0]['sba2ini'])) echo $sub[0]['sba2ini'] . " até " ; ?>  <?=$sub[0]['sba2fim']; ?>		
													</td>
													<td align="right">
														<?if(!Empty($sub[0]['sba3ini'])) echo $sub[0]['sba3ini'] . " até " ; ?> <?=$sub[0]['sba3fim']; ?>
													</td>
													<td align="right">
														<?if(!Empty($sub[0]['sba4ini'])) echo $sub[0]['sba4ini'] . " até " ; ?> <?=$sub[0]['sba4fim']; ?>
													</td>
													<td align="right">
														&nbsp;
													</td>
												</tr>											
											
	
											<?php if ( $existevalor > 0 ) { ?>
	
											
										
												<tr>
													<th align="right">
														<b></b>
													</th>
													<th align="right">
														<b>2007</b>
													</th>
													<th align="right">
														<b>2008</b>
													</th>
													<th align="right">
														<b>2009</b>
													</th>
													<th align="right">
														<b>2010</b>
													</th>
													<th align="right">
														<b>2011</b>
													</th>
													<th>
													<b></b>
													</th>
												</tr>
												<tr>
													<td align="right">
														<b>Valor Unitário:</b>
													</td>
													<td align="right">
														<?=number_format($valor2007,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($valor2008,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($valor2009 ,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($valor2010 ,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($valor2011 ,2,',','.');?>
													</td>
													
												</tr>
											
											
											
											
												<tr>
													<th>
														&nbsp;
													</th>
													<th align="center">
														<b>2007</b>
													</th>
													<th align="center">
														<b>2008</b>
													</th>
													<th align="center">
														<b>2009</b>
													</th>
													<th align="center">
														<b>2010</b>
													</th>
													<th align="center">
														<b>2011</b>
													</th>
													<th align="center">
														<b>Total</b>
													</th>
												</tr>
												<tr>
													<?
													$ano0 = $valor[0]['ano2007'];
													$ano1 = $valor[0]['ano2008'];
													$ano2 = $valor[0]['ano2009'];
													$ano3 = $valor[0]['ano2010'];
													$ano4 = $valor[0]['ano2011'];
	
													$total = $ano0 + $ano1 + $ano2 + $ano3 + $ano4;
						
													$totalGeralAno0 += $ano0;	
													$totalGeralAno1 += $ano1;
													$totalGeralAno2 += $ano2;
													$totalGeralAno3 += $ano3;
													$totalGeralAno4 += $ano4;
	
													$totalGeralIndicadorAno0 += $ano0;
													$totalGeralIndicadorAno1 += $ano1;
													$totalGeralIndicadorAno2 += $ano2;
													$totalGeralIndicadorAno3 += $ano3;
													$totalGeralIndicadorAno4 += $ano4;
	
													$totalGeralAreaAno0 += $ano0;
													$totalGeralAreaAno1 += $ano1;
													$totalGeralAreaAno2 += $ano2;
													$totalGeralAreaAno3 += $ano3;
													$totalGeralAreaAno4 += $ano4;
	
													$totalGeralDimensaAno0 += $ano0;
													$totalGeralDimensaAno1 += $ano1;
													$totalGeralDimensaAno2 += $ano2;
													$totalGeralDimensaAno3 += $ano3;
													$totalGeralDimensaAno4 += $ano4;
	
	
													 ?>
													<td align="right">
														<b>Valores Anuais:</b>
													</td>
													<td align="right">
														<?=number_format($ano0,2,',','.');?>
													</td>											
													<td align="right">
														<?=number_format($ano1,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($ano2,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($ano3,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($ano4,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($total,2,',','.') ?>
													</td>
												</tr>		
											
											<?php }  ?>
											</table>																																		
										</td>
									</tr>
									
									<?php 
									
										if( $dado[$i]['frmdsc'] == "MEC - Transferência voluntária" ){
											
											$sqlComposicao = " select * from cte.composicaosubacao where sbaid = $novasubAcao ";
											if( $detalhamentoComposicao = $db->carregar($sqlComposicao) ){ ?>
												<tr>
													<td colspan="2" style="text-align: center; font-weight: bold; background: #ccc;">Detalhamento dos Itens de Composição</td>
												</tr>	
												<?php
													foreach( $detalhamentoComposicao as $arDetalhe ){
														$arDetalhes[$arDetalhe["cosano"]][] = $arDetalhe;
													}	
													//1782706 - Balcão empréstimo 
													ksort($arDetalhes);
													foreach( $arDetalhes as $nrAno => $arDetalhe ){
												?>
													<tr>
														<td class="SubTituloDireita"><?php echo $nrAno ?></td>
														<td>
															<table class="listagem" width="100%">
																<thead> 
																	<th>Identificação do Item</th>
																	<th>Un. Medida</th>
																	<th>Quantidade</th>
																	<th>Valor Unitário</th>
																	<th>Total</th>
																</thead> 
																<?php
																usort( $arDetalhe, "ordenarArray" ); 
																
																foreach( $arDetalhe as $arValores ){ 
																	$sqlUnidadeMedida = " select undddsc from cte.unidademedidadetalhamento where unddid = ". $arValores["unddid"];
																	$unidadeMedida = $db->pegaUm( $sqlUnidadeMedida );															
																?>
																	<tr>
																		<td><?php echo $arValores["cosdsc"] ?></td>
																		<td><?php echo $unidadeMedida ?></td>
																		<td align="right"><?php echo number_format( $arValores["cosqtd"], 1, ',', '.' ) ?></td>
																		<td align="right"><?php echo "R$ ". number_format( $arValores["cosvlruni"], 2, ',', '.' ) ?></td>
																		<td align="right"><?php echo "R$ ". number_format( $arValores["cosqtd"] * $arValores["cosvlruni"], 2, ',', '.' ) ?></td>
																	</tr>
																<?php } ?>	
															</table>
														</td>
													</tr>
												<?php } ?>	
										<?php } ?>
									<?php } ?>
									
								</table>	
							</td>
						</tr>
						
					<?	
			}	
				$i++;	
				$demanda = 0;		
				$novaDimensao = $dado[$i]['dimid'];
				$novaArea = $dado[$i]['ardid'];
				$novoIndicador = $dado[$i]['indid'];
				$novaAcao = $dado[$i]['aciid'];
				$novasubAcao = $dado[$i]['sbaid'];

}

?>
<!-- ########################## Apresentação ########################## -->

<html>
<head>
<meta http-equiv=Content-Type content="text/html; charset=windows-1252">
<meta name=Generator content="Microsoft Word 11 (filtered)">
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>
<link rel='stylesheet' type='text/css' href='estiloImprimir.css' media="print" />

<script type="text/javascript" src="../includes/funcoes.js"></script>
<style>
b{	FONT: 8pt Arial;
	FONT-WEIGHT: bold;
}
</style>

<title>Relatório Público</title>
<style>
<!--
 /* Style Definitions */
 p.MsoNormal, li.MsoNormal, div.MsoNormal
	{margin:0cm;
	margin-bottom:.0001pt;
	font-size:12.0pt;
	font-family:"Times New Roman";}
p.MsoBodyText, li.MsoBodyText, div.MsoBodyText
	{margin:0cm;
	margin-bottom:.0001pt;
	text-align:justify;
	text-autospace:none;
	font-size:12.0pt;
	font-family:Arial;}
p.MsoBodyTextIndent, li.MsoBodyTextIndent, div.MsoBodyTextIndent
	{margin:0cm;
	margin-bottom:.0001pt;
	text-align:justify;
	text-indent:35.4pt;
	text-autospace:none;
	font-size:11.0pt;
	font-family:Arial;}
p.MsoBodyText2, li.MsoBodyText2, div.MsoBodyText2
	{margin:0cm;
	margin-bottom:.0001pt;
	text-align:justify;
	text-autospace:none;
	font-size:11.5pt;
	font-family:Arial;}
p.MsoBodyText3, li.MsoBodyText3, div.MsoBodyText3
	{margin:0cm;
	margin-bottom:.0001pt;
	text-align:justify;
	text-autospace:none;
	font-size:11.0pt;
	font-family:Arial;}
p.MsoBodyTextIndent2, li.MsoBodyTextIndent2, div.MsoBodyTextIndent2
	{margin:0cm;
	margin-bottom:.0001pt;
	text-align:justify;
	text-indent:35.4pt;
	text-autospace:none;
	font-size:11.5pt;
	font-family:Arial;}
a:link, span.MsoHyperlink
	{color:blue;
	text-decoration:underline;}
a:visited, span.MsoHyperlinkFollowed
	{color:purple;
	text-decoration:underline;}
@page Section1
	{size:595.45pt 841.7pt;
	margin:72.0pt 84.95pt 72.0pt 84.95pt;}
div.Section1
	{page:Section1;}
 /* List Definitions */
 ol
	{margin-bottom:0cm;}
ul
	{margin-bottom:0cm;}
-->
</style>

</head>
<body lang=PT-BR link=blue vlink=purple>
<table class="tabela" align="center" cellspacing="1" cellpadding="3" style="font-family:Arial, Verdana; font-size:10px;">
	<tr>
	<td class="tituloPrincipalAbas"><b>APRESENTAÇÃO</b></td>
	</tr>
	<tr>
		<td>
<p class=MsoNormal align=center style='text-align:center'><b><span
style='font-family:Arial'>Ministério da Educação</span></b></p>
<p class=MsoBodyTextIndent align=center style='text-align:center;text-indent:
0cm'><b><span style='font-size:10.0pt'>PAR - PLANO DE AÇÕES ARTICULADAS</span></b></p>
<p class=MsoBodyTextIndent align=center style='text-align:center;text-indent:
0cm'><b><span style='font-size:10.0pt'>RELATÓRIO PÚBLICO</span></b></p>
<p class=MsoBodyTextIndent align=center style='text-align:center;text-indent:
0cm'><b><span style='font-size:10.0pt'>APRESENTAÇÃO</span></b></p>
<p class=MsoBodyTextIndent style='margin-top:6.0pt;margin-right:1cm;margin-bottom:
6.0pt;margin-left:1cm;text-indent:0cm'><span style='font-size:10.0pt'>            O
Plano de Desenvolvimento da Educação (PDE), apresentado pelo Ministério da Educação
em abril de 2007, colocou à disposição dos estados, do Distrito Federal e dos
municípios, instrumentos eficazes de avaliação e de implementação de políticas
de melhoria da qualidade da educação, sobretudo da educação básica pública.</span></p>

<p class=MsoBodyTextIndent style='margin-top:6.0pt;margin-right:1cm;margin-bottom:
6.0pt;margin-left:1cm'><span style='font-size:10.0pt'>O Plano de Metas
Compromisso Todos pela Educação, instituído pelo Decreto 6.094 de 24 de abril
de 2007, é um programa estratégico do PDE, e inaugura um novo regime de
colaboração, que busca concertar a atuação dos entes federados sem ferir-lhes a
autonomia, envolvendo primordialmente a decisão política, a ação técnica e
atendimento da demanda educacional, visando à melhoria dos indicadores
educacionais. Trata-se de um compromisso fundado em 28 diretrizes e consubstanciado
em um plano de metas concretas, efetivas, que compartilha competências
políticas, técnicas e financeiras para a execução de programas de manutenção e
desenvolvimento da educação básica.</span></p>

<p class=MsoBodyTextIndent style='margin-top:6.0pt;margin-right:1cm;margin-bottom:
6.0pt;margin-left:1cm;text-indent:0cm'><span style='font-size:10.0pt'>            A
partir da adesão ao Plano de Metas Compromisso Todos pela Educação, os estados
e municípios elaboram seus respectivos Planos de Ações Articuladas.</span></p>

<p class=MsoNormal style='margin-top:6.0pt;margin-right:1cm;margin-bottom:6.0pt;
margin-left:1cm;text-align:justify;text-indent:35.4pt;text-autospace:none'><span
style='font-size:10.0pt;font-family:Arial'>Para auxiliar na elaboração do PAR,
o Ministério da Educação criou um novo sistema, o SIMEC – Módulo PAR Plano de
Metas -, integrado aos sistemas que já possuía, e que pode ser acessado de
qualquer computador conectado à internet, representando uma importante evolução
tecnológica, com agilidade e transparência nos processos de elaboração, análise
e apresentação de resultados dos PAR.</span></p>

<p class=MsoBodyTextIndent style='margin-top:6.0pt;margin-right:1cm;margin-bottom:
6.0pt;margin-left:1cm'><span style='font-size:10.0pt'>Com metas claras,
passíveis de acompanhamento público e controle social, o MEC pode assim
disponibilizar, para consulta pública, os relatórios dos Planos de Ações
Articuladas elaborados pelos estados e municípios que aderiram ao Plano de
Metas Compromisso Todos pela Educação.</span></p>

<p class=MsoBodyTextIndent style='margin-top:6.0pt;margin-right:1cm;margin-bottom:
6.0pt;margin-left:1cm;text-indent:0cm'><span style='font-size:10.0pt'>            Apresentamos,
a seguir, uma breve descrição dos elementos constitutivos do PAR.</span></p>

<p class=MsoNormal style='margin-top:6.0pt;margin-right:1cm;margin-bottom:6.0pt;
margin-left:1cm;text-align:justify;text-indent:35.4pt;text-autospace:none'><span
style='font-size:10.0pt;font-family:Arial'>Inicialmente, os estados e
municípios devem realizar um diagnóstico minucioso da realidade educacional
local. A partir desse diagnóstico, desenvolverão um conjunto coerente de ações
que resulta no PAR.</span></p>

<p class=MsoBodyTextIndent2 style='margin-top:6.0pt;margin-right:1cm;
margin-bottom:6.0pt;margin-left:1cm'><span style='font-size:10.0pt'>O
instrumento para o diagnóstico da situação educacional local está estruturado
em quatro grandes dimensões:</span></p>

<p class=MsoBodyText2 style='margin-left:3cm; margin-right: 0.5px;;text-indent:-18.0pt;line-height:
150%'><b><span style='font-size:10.0pt;line-height:150%'>1. Gestão Educacional.</span></b></p>

<p class=MsoBodyText2 style='margin-left:3cm; margin-right: 0.5px;;text-indent:-18.0pt;line-height:
150%'><b><span style='font-size:10.0pt;line-height:150%'>2. Formação de Professores e dos Profissionais de Serviço e Apoio Escolar.</span></b></p>

<p class=MsoBodyText2 style='margin-left:3cm; margin-right: 0.5px;;text-indent:-18.0pt;line-height:150%'>
<b><span style='font-size:10.0pt;line-height:150%'>3. Práticas Pedagógicas e Avaliação.</span></b></p>

<p class=MsoBodyText2 style='margin-left:3cm; margin-right: 0.5px;;text-indent:-18.0pt;line-height:
150%'><b><span style='font-size:10.0pt;line-height:150%'>4. Infra-estrutura Física e Recursos Pedagógicos.</span></b></p>

<p class=MsoBodyText2 style='margin-top:6.0pt;margin-right:1cm;margin-bottom:
6.0pt;margin-left:1cm;text-indent:35.4pt'><span style='font-size:10.0pt'>Cada
dimensão é composta por áreas de atuação, e cada área apresenta indicadores específicos.
Esses indicadores são pontuados segundo a descrição de critérios
correspondentes a quatro níveis.</span></p>

<p class=MsoBodyText2 style='margin-top:6.0pt;margin-right:1cm;margin-bottom:
6.0pt;margin-left:1cm;text-indent:35.4pt'><span style='font-size:10.0pt'>A
pontuação gerada para cada indicador é fator determinante para a elaboração do
PAR, ou seja, na metodologia adotada, apenas critérios de pontuação 1 e 2, que
representam situações insatisfatórias ou inexistentes, podem gerar ações.</span></p>

<p class=MsoBodyText2 style='margin-top:6.0pt;margin-right:1cm;margin-bottom:
6.0pt;margin-left:1cm;text-indent:35.4pt'><span style='font-size:10.0pt'>Assim,
o relatório disponibilizado apresenta as seguintes informações:</span></p>

<p class=MsoBodyText2 style='margin-left:3cm; margin-right: 0.5px;;text-indent:-18.0pt;line-height:
150%'><b><span style='font-size:10.0pt;line-height:150%'>1. Síntese por indicador:</span></b><span
style='font-size:10.0pt;line-height:150%'> resultado detalhado da realização do
diagnóstico.</span></p>

<p class=MsoBodyText2 style='margin-left:3cm; margin-right: 0.5px;;text-indent:-18.0pt;line-height:
150%'><b><span style='font-size:10.0pt;line-height:150%'>2. Síntese da dimensão:</span></b><span
style='font-size:10.0pt;line-height:150%'> resultado quantitativo da realização
do diagnóstico.</span></p>

<p class=MsoBodyText2 style='margin-left:3cm; margin-right: 0.5px;;text-indent:-18.0pt;line-height:
150%'><b><span style='font-size:10.0pt;line-height:150%'>3. Síntese do PAR:</span></b><span
style='font-size:10.0pt;line-height:150%'> apresenta o detalhamento das ações e
subações selecionadas por cada estado ou município.</span></p>

<p class=MsoBodyText2 style='margin-left:3cm; margin-right: 0.5px;;text-indent:-18.0pt;line-height:
150%'><b><span style='font-size:10.0pt;line-height:150%'>4. Termo de Cooperação:</span></b><span
style='font-size:10.0pt;line-height:150%'> apresenta a relação de ações e
subações que contarão com o apoio técnico do Ministério da Educação.</span></p>

<p class=MsoBodyText2 style='margin-left:3cm; margin-right: 0.5px;;text-indent:-18.0pt;line-height:
150%'><b><span style='font-size:10.0pt;line-height:150%'>5. Liberação dos recursos:</span></b><span
style='font-size:10.0pt;line-height:150%'> apresenta a relação de ações que
geraram convênio, ou seja, a liberação de recursos financeiros.</span></p>

<p class=MsoBodyText3><span style='font-size:10.0pt'><span style='text-decoration:
 none'>&nbsp;</span></span></p>

<p class=MsoBodyText3 style='margin-left:1cm;margin-right:1cm;text-indent:35.4pt'><span style='font-size:10.0pt'>
Cabe destacar que no presente momento apenas as informações sobre as redes municipais estão disponíveis.
</span></p>
<p class=MsoBodyText3 style='text-indent:35.4pt'><span style='font-size:10.0pt'>&nbsp;</span></p>
<p class=MsoBodyText3 style='margin-left:1cm;margin-right:1cm;text-indent:35.4pt'><span style='font-size:10.0pt'>Para
mais informações, consulte o portal do MEC, <a href="http://www.mec.gov.br/">www.mec.gov.br</a>,
veja “IDEB - Saiba como melhorar”.</span></p>
		</td>
	</tr>
</table>

<div style="page-break-before:always" ></div>

<!-- ########################## Sintese do Indicador ########################## -->
<br></br>
<?php
$db->cria_aba( $abacod_tela, $url, '' );
$sql = sprintf("select distinct
					d.dimcod
					,d.dimdsc
					,ad.ardcod
					,ad.arddsc
					,i.indcod
					,c.ctrpontuacao
					,c.crtdsc
					,p.ptojustificativa
					,p.ptodemandamunicipal
					,p.ptodemandaestadual
				from 
					cte.instrumento ins
					inner join cte.dimensao d on d.itrid = ins.itrid
					inner join cte.areadimensao ad on d.dimid = ad.dimid
					inner join cte.indicador i on i.ardid = ad.ardid
					inner join cte.criterio c on c.indid = i.indid
					inner join cte.pontuacao p on p.crtid = c.crtid
				where 
					p.ptostatus = 'A'
					and d.dimstatus = 'A'
					and ad.ardstatus = 'A'  
					and i.indstatus = 'A'
					and p.inuid = %d
				" , 
				$inuid
			);
$resultado = $db->carregar($sql);

?>
<table class="tabela" align="center" bgcolor="" cellspacing="1" cellpadding="3" style="font-family:Arial, Verdana; font-size:10px;">
	<tr>
	<td class="tituloPrincipalAbas"><b>SÍNTESE DO INDICADOR</b></td>
	</tr>
	<tr>
		<td>
<?php if($resultado):?>
	<table border="0" width="100%" cellspacing="0" cellpadding="4" align="center" bgcolor="#999999" class="listagem"  style="width:100%;">
		<?php foreach( $resultado as $key => $val ): ?>
		<?php if($key == 0 or $val["dimcod"] != $resultado[$key - 1]["dimcod"]):?>
		<tr> 
			<th colspan="6" class="class1" style=" background-color:#7e8e47;" ><?php echo $val["dimcod"] . '. ' . $val["dimdsc"];?></th>
		</tr>
		<?php endif;?>
		<?php if($key == 0 or $val["ardcod"] != $resultado[$key - 1]["ardcod"]):?>
		<tr> 
			<td bgcolor="#acbc73"></td>
			<th colspan="5" class="class2" style=" background-color:#acbc73;"><?php echo $val["ardcod"] . '. ' . $val["arddsc"];?></th>
		</tr>
		<tr> 
			<td bgcolor="#ccd7a4"></td>
			<td bgcolor="#ccd7a4"></td>
			<td width="25" bgcolor="#ccd7a4">Indicador</td>
			<td width="25" bgcolor="#ccd7a4">Pontua&ccedil;&atilde;o</td>
			<td bgcolor="#ccd7a4" align="center">Critério</td>
		</tr>
		<?php $cor = '#dfdfdf'; ?>
		<?php endif;?>
		<tr bgcolor="<?php echo $cor; ?>"> 
			<td></td>
			<td></td>
			<td align="center"><?php echo $val["indcod"];?>&nbsp;</td>
			<td align="center"><?php echo $val["ctrpontuacao"];?>&nbsp;</td>
			<td><?php echo $val["crtdsc"];?>&nbsp;</td>
		</tr>
		<?php if($cor == '#dfdfdf') $cor = '#ffffff'; else $cor = '#dfdfdf'; ?>
		<?php endforeach; ?>
	</table>
<?php else: ?>
	<table class="tabela" align="center" bgcolor="#fafafa"><tr><td align="center" style="color:red;">Nenhum Indicador Pontuado.</td></tr></table>
<?php endif; ?>
<div style=" width:200px; height:5px;" ></div>

		</td>
	</tr>
</table>

<div style="page-break-before:always" ></div>

<!-- ########################## Dimessão ########################## -->
<br></br>
<?php
$db->cria_aba( $abacod_tela, $url, '' );
$sql = sprintf("select 
					ins.itrid
					,d.dimid
					,d.dimcod || '. ' ||d.dimdsc as dimdsc
					,c.ctrpontuacao 
					,count ( c.ctrpontuacao ) as qtpontos
					
				from cte.instrumento ins
					inner join cte.dimensao d on d.itrid = ins.itrid
					inner join cte.areadimensao ad on d.dimid = ad.dimid
					inner join cte.indicador i on i.ardid = ad.ardid
					inner join cte.criterio c on c.indid = i.indid
					left join cte.pontuacao pt on pt.crtid = c.crtid and pt.indid = i.indid and pt.inuid = %d
				where
					pt.ptostatus = 'A'
					and d.dimstatus = 'A'
					and ad.ardstatus = 'A'  
					and i.indstatus = 'A'
				group by ins.itrid, d.dimcod, d.dimdsc, c.ctrpontuacao , d.dimid 				
				order by d.dimcod , dimdsc
				" 
				,$inuid
				
				);			

if( $resultado = $db->carregar($sql) )
{
	//percorrendo o resultado e criando um array por dimensao 
	foreach($resultado as $key => $val )
	{
		$cor = $icone ? '#959595' : '#133368';
		$relatorio[$val["dimid"]]["dimdsc"] = $val["dimdsc"];
		switch($val["ctrpontuacao"])
		{
			case "0":
				$relatorio[$val["dimid"]]["0"] = $val["qtpontos"];
				break;
			case "1":
				$relatorio[$val["dimid"]]["1"] = $val["qtpontos"];
				break;
			case "2":
				$relatorio[$val["dimid"]]["2"] = $val["qtpontos"];
				break;
			case "3":
				$relatorio[$val["dimid"]]["3"] = $val["qtpontos"];
				break;
			case "4":
				$relatorio[$val["dimid"]]["4"] = $val["qtpontos"];
				break;
		}
	}
}
$total["t0"] = 0;
$total["t1"] = 0;
$total["t2"] = 0;
$total["t3"] = 0;
$total["t4"] = 0;
$cor = '#e7e7e7';
?>
<table class="tabela" align="center" bgcolor="" cellspacing="1" cellpadding="3" style="font-family:Arial, Verdana; font-size:10px;">
	<tr>
	<td class="tituloPrincipalAbas"><b>SINTESE DA DIMENSÃO</b></td>
	</tr>
	<tr>
		<td>
			<?php if( isset($relatorio)): ?>
				<table border="0" width="98%" cellspacing="0" cellpadding="4" align="center" bgcolor="#DCDCDC" class="listagem" style="width:100%; margin-top:5px;  margin-bottom:5px;">
					<thead>
					<tr style="border-bottom:4px solid black;">
						<td bgcolor="#acbc73" align="center" rowspan="2">Dimensão</td>
						<td bgcolor="#acbc73" align="center" colspan="5">Pontuação</td>
					</tr>
					<tr>
						<td bgcolor="#ccd7a4" align="center"><b>4</b></td>
						<td bgcolor="#ccd7a4" align="center"><b>3</b></td>
						<td bgcolor="#ccd7a4" align="center"><b>2</b></td>
						<td bgcolor="#ccd7a4" align="center"><b>1</b></td>
						<td bgcolor="#ccd7a4" align="center"><b>n/a</b></td>
					</tr>
					</thead>
					<?php foreach($relatorio as $keyr => $valr ): ?>
						<tr bgcolor="<?=$cor?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor?>';">
							<td><?php echo $valr["dimdsc"]; ?></td>
							<td align="center"><?php echo (int)$valr["4"]; ?>&nbsp;</td>
							<td align="center"><?php echo (int)$valr["3"]; ?>&nbsp;</td>
							<td align="center"><?php echo (int)$valr["2"]; ?>&nbsp;</td>
							<td align="center"><?php echo (int)$valr["1"]; ?>&nbsp;</td>
							<td align="center"><?php echo (int)$valr["0"]; ?>&nbsp;</td>
						</tr>
						<?php 
							$total["t0"] += (int)$valr["0"];
							$total["t1"] += (int)$valr["1"];
							$total["t2"] += (int)$valr["2"];
							$total["t3"] += (int)$valr["3"];
							$total["t4"] += (int)$valr["4"];
							if($cor == '#e7e7e7') $cor = '#ffffff'; else $cor = '#e7e7e7';
						?>
					<?php endforeach; ?>
						<tr>
							<td bgcolor="#ccd7a4" align="right"><b>Total:</b></td>
							<td bgcolor="#ccd7a4" align="center"><b><?php echo $total["t4"]; ?>&nbsp;</b></td>
							<td bgcolor="#ccd7a4" align="center"><b><?php echo $total["t3"]; ?>&nbsp;</b></td>
							<td bgcolor="#ccd7a4" align="center"><b><?php echo $total["t2"]; ?>&nbsp;</b></td>
							<td bgcolor="#ccd7a4" align="center"><b><?php echo $total["t1"]; ?>&nbsp;</b></td>
							<td bgcolor="#ccd7a4" align="center"><b><?php echo $total["t0"]; ?>&nbsp;</b></td>
						</tr>
						<tfoot>
						<tr >
								<td colspan="6" align="right" bgcolor="#ccd7a4">*n/a :  Não se Aplica.</td>
						</tr>
						<tfoot>
				</table>
			<?php else: ?>
				<table class="tabela" align="center" bgcolor="#fafafa"><tr><td align="center" style="color:red;">Nenhum Indicador Pontuado.</td></tr></table>
			<?php endif; ?>
		</td>
	</tr>
</table>	
	<div style="page-break-before:always" ></div>	

<!-- ########################## Sintese do Par ########################## -->
<?php
print '<br/>';
$ptostatus = isset( $_REQUEST['ptostatus'] ) ? $_REQUEST['ptostatus'] : 'A';
$sql ="
	select
			d.dimid,
			d.dimcod || ' - ' || d.dimdsc as dimensao,
			area.ardcod || ' - ' || area.arddsc as area,
			area.ardid,			
			i.indcod || ' - ' || i.inddsc as indicador,
			i.indid,
			i.indcod,
			c.ctrpontuacao as pontuacao,
			p.ptojustificativa,
			p.ptodemandamunicipal,
			p.ptodemandaestadual,
			Case acao.acilocalizador
				when 'E' then 'Estadual'
				when 'M' then 'Municipal'
				end as Tipo,
			acao.aciid,
			acao.ptoid,
			acao.acidsc,
			acao.acirpns,
			acao.acicrg,
			to_char(acao.acidtinicial,'dd/mm/yyyy') as acidtinicial,
			to_char(acao.acidtfinal,'dd/mm/yyyy') as acidtfinal,
			acao.acirstd,
			acao.acilocalizador,
			subacao.sbaid,
			subacao.sbaporescola,
			prg.prgdsc as sbaprm,
			coalesce(u.unddsc,'') as unddsc,
			coalesce(f.frmdsc,'') as frmdsc,
			c.crtdsc,
			c.ctrpontuacao,
			f.frmid,
			subacao.sbadsc,
			subacao.sbastgmpl,
			subacao.sbapcr,
			subacao.sbauntdsc,
			length(p.ptodemandaestadual) as demandaestadual, 
			length(p.ptodemandamunicipal) as demandamunicipal
		from
			cte.dimensao d
			inner join cte.areadimensao area ON area.dimid = d.dimid
			inner join cte.indicador i ON i.ardid = area.ardid
			inner join cte.pontuacao p ON p.indid = i.indid and p.ptostatus = '" . $ptostatus . "'
			inner join cte.criterio c ON c.crtid = p.crtid
			inner join cte.instrumentounidade iu ON iu.inuid = p.inuid
			inner join cte.acaoindicador acao ON acao.ptoid = p.ptoid 
			inner join cte.subacaoindicador subacao ON subacao.aciid = acao.aciid
			inner join cte.unidademedida u on u.undid = subacao.undid
			inner join cte.formaexecucao f on f.frmid = subacao.frmid
			left join cte.programa prg on prg.prgid = subacao.prgid	
			LEFT JOIN cte.subacaoparecertecnico 	spt ON spt.sbaid = subacao.sbaid and sptano = date_part('year', current_date)
		where
			iu.inuid = '".$inuid."' and spt.ssuid = 3  --and subacao.sbaid = 2927
			
		order by
			d.dimcod,  
			area.ardcod,
			i.indcod, p.ptoid, acao.aciid, subacao.sbadsc
";

$dado = $db->carregar($sql); 
$i= 0;
$totalreg = count($dado);

$novaDimensao = $dado[$i]['dimid'];
$novaArea = $dado[$i]['ardid'];
$novoIndicador = $dado[$i]['indid'];
$novaAcao = $dado[$i]['aciid'];
$novasubAcao = $dado[$i]['sbaid'];

$totalGeralAno0 = 0;
$totalGeralAno1 = 0;
$totalGeralAno2 = 0;
$totalGeralAno3 = 0;
$totalGeralAno4 = 0;
$demanda = 0;
?>

<table class="tabela" align="center" bgcolor="" cellspacing="1" cellpadding="3" style="font-family:Arial, Verdana; font-size:10px;">
	<tr>
	<td class="tituloPrincipalAbas"><b>SÍNTESE DO PAR</b></td>
	</tr>
	<tr>
		<td>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style=" width:100%;"    > 
<? 
while ( $i < $totalreg ) {
	
	$dimensao = $dado[$i]['dimid'];
		?>
		
					<tr>
						<td class="tdDivisaoItens" colspan="2"></td>
					</tr>
					<tr>
					<tr>
						<td class="SubTituloDireita class1" style=" background-color:#7e8e47;">
							<b>Dimensão</b>
						</td>
						<td  class=" class1" style=" background-color:#7e8e47;">	
							<?=$dado[$i]['dimensao']; ?>
						</td>
					</tr>
					<?
					while ($dimensao == $novaDimensao)
					{
						if($i >= $totalreg ) break;

						$area = $dado[$i]['ardid'];

				?>
								<tr>
									<td  class="SubTituloDireita" style=" background-color:#acbc73;" bgcolor="#acbc73">
										<b>Área</b>
									</td>
									<td class="" style="text-align:left" bgcolor="#acbc73">	
										<?=$dado[$i]['area']; ?>
									</td>
								</tr>
								
						
								<?

								while ($area == $novaArea )
								{
									if($i >= $totalreg ) break;

									$indicador = $dado[$i]['indid'];
									
									while ($indicador == $novoIndicador )
									{
										if($i >= $totalreg ) break;
												?>
													<tr>
														<td class="SubTituloDireita"  style=" background-color:#ccd7a4;">
															<b>Indicador</b>
														</td>
														<td  style="text-align:left; background-color:#ccd7a4;">	
															<?=$dado[$i]['indicador']; ?>
														</td>
													</tr>
													<tr>
														<td class="SubTituloDireita" style=" background-color:#ccd7a1;">
															<b>Critério / Pontuação</b>
														</td>
														<td style="text-align:left; background-color:#ccd7a1;">	
															<?echo $dado[$i]['ctrpontuacao'] . ' - '.$dado[$i]['crtdsc']; ?>
														</td>
													</tr>
													<tr>
														<td class="SubTituloDireita">
															<b>Justificativa</b>
														</td>
														<td class="" style="text-align:left">	
															<?echo $dado[$i]['ptojustificativa']; ?>
														</td>
													</tr>
													<?if( $dado[$i]['ptodemandaestadual']){
													?>
													<tr>
														<td class="SubTituloDireita">
															<b>Demanda para Rede Estadual</b>
														</td>
														<td class="" style="text-align:left">	
															<?echo $dado[$i]['ptodemandaestadual']; ?>
														</td>
													</tr>
													<?}?>
													<?if($dado[$i]['ptodemandamunicipal']){
													?>
													<tr>
														<td class="SubTituloDireita">
															<b>Demanda para Redes Municipais</b>
														</td>
														<td class="" style="text-align:left">	
															<?echo $dado[$i]['ptodemandamunicipal']; ?>
														</td>
													</tr>
													<?}?>
										
													<?
													$acao = $dado[$i]['aciid'];
													while ($acao == $novaAcao)
													{
														if($i >= $totalreg ) break;
											?>
																<tr>
																	<td class="SubTituloDireita">
																		<b>Ação</b>
																	</td>
																	<td class="" style="text-align:left">
																		<table  class="tabela" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" >
																			<tr>
																				<td  class="SubTituloDireita" style="width:20%" >
																					Demanda:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['tipo'];?>
																				</td>
																			</tr>	
																			<tr>
																				<td  class="SubTituloDireita">
																					Descrição da Ação:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acidsc']; ?>
																				</td>
																			</tr>
																			<tr>
																				<td  class="SubTituloDireita">
																					Nome do Responsável:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acirpns']; ?>
																				</td>
																			</tr>	
																			<tr>
																				<td  class="SubTituloDireita">
																					Cargo do Responsável:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acicrg']; ?>
																				</td>
																			</tr>			 
																			<tr>
																				<td  class="SubTituloDireita">
																					Período Inicial:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acidtinicial']; ?>
																				</td>
																			</tr>
																			<tr>
																				<td  class="SubTituloDireita">
																					Período Final:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acidtfinal']; ?>
																				</td>
																			</tr>
																			<tr>
																				<td  class="SubTituloDireita">
																					Resultado Esperado:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acirstd']; ?>
																				</td>
																			</tr>
																		</table>	
																	</td>
																</tr>			
																<?
																$subacao 	= $dado[$i]['sbaid'];
																$tipo 		= $dado[$i]['tipo'];
																$demandaE 	= $dado[$i]["demandaestadual"];
																$demandaM 	= $dado[$i]["demandamunicipal"];
																	while ($acao == $novaAcao ){
																		mostra_subacao();
																		if($i >= $totalreg ) break;
																	}
																}
															}
														?>	
												<tr>
													<td class="SubTituloDireita" >
													<b>Total Geral por Indicador</b>
													</td>
													<td>			
														<table class="listagem" width="100%">
															<thead>
																<th align="center">
																	<b>2007</b>
																</th>
																<th align="center">
																	<b>2008</b>
																</th>
																<th align="center">
																	<b>2009</b>
																</th>
																<th align="center">
																	<b>2010</b>
																</th>
																<th align="center">
																	<b>2011</b>
																</th>
																<th align="center">
																	<b>Total</b>
																</th>
															</thead>
															<tr>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno0,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno1,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno2,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno3,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno4,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno0 + $totalGeralIndicadorAno1 + $totalGeralIndicadorAno2 + $totalGeralIndicadorAno3 + $totalGeralIndicadorAno4 ,2,',','.');?>
																</td>
															</tr>
														</table>
													</td>
												</tr>
												<?
												$totalGeralIndicadorAno0 = 0; 
												$totalGeralIndicadorAno1 = 0;
												$totalGeralIndicadorAno2 = 0;
												$totalGeralIndicadorAno3 = 0;
												$totalGeralIndicadorAno4 = 0;

								}
							?>
						<tr>
							<td class="SubTituloDireita" >
							<b>Total Geral por Área</b>
							</td>
							<td>			
								<table class="listagem" width="100%">
									<thead>
										<th align="center">
											<b>2007</b>
										</th>
										<th align="center">
											<b>2008</b>
										</th>
										<th align="center">
											<b>2009</b>
										</th>
										<th align="center">
											<b>2010</b>
										</th>
										<th align="center">
											<b>2011</b>
										</th>
										<th align="center">
											<b>Total</b>
										</th>
									</thead>
									<tr>
										<td align="right">
											<?=number_format($totalGeralAreaAno0,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno1,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno2,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno3,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno4,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno0 + $totalGeralAreaAno1 + $totalGeralAreaAno2 + $totalGeralAreaAno3+ $totalGeralAreaAno4,2,',','.');?>
										</td>
									</tr>
								</table>
							</td>
						</tr>
						<?
						$totalGeralAreaAno0 = 0; 
						$totalGeralAreaAno1 = 0;
						$totalGeralAreaAno2 = 0;
						$totalGeralAreaAno3 = 0;
						$totalGeralAreaAno4 = 0;

					}
?>
		<tr>
							<td class="SubTituloDireita" >
							<b>Total Geral por Dimensão</b>
							</td>
							<td>			
								<table class="listagem" width="100%">
									<thead>
										<th align="center">
											<b>2007</b>
										</th>
										<th align="center">
											<b>2008</b>
										</th>
										<th align="center">
											<b>2009</b>
										</th>
										<th align="center">
											<b>2010</b>
										</th>
										<th align="center">
											<b>2011</b>
										</th>
										<th align="center">
											<b>Total</b>
										</th>
									</thead>
									<tr>
										<td align="right">
											<?=number_format($totalGeralDimensaAno0,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno1,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno2,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno3,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno4,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno0 + $totalGeralDimensaAno1 + $totalGeralDimensaAno2 + $totalGeralDimensaAno3+ $totalGeralDimensaAno4,2,',','.');?>
										</td>
									</tr>
								</table>
							</td>
						</tr>
						<?
						$totalGeralDimensaAno0 = 0; 
						$totalGeralDimensaAno1 = 0;
						$totalGeralDimensaAno2 = 0;
						$totalGeralDimensaAno3 = 0;
						$totalGeralDimensaAno4 = 0;


	}?>	
<tr>
	<td class="SubTituloDireita"  colspan="2" style="text-align: center;">
	<b>Total Geral</b>
	</td>
</tr>
<tr>
	<td colspan="2" >
		<table class="listagem"  width="100%">
			<thead>
				<th align="center">
					<b>2007</b>
				</th>
				<th align="center">
					<b>2008</b>
				</th>
				<th align="center">
					<b>2009</b>
				</th>
				<th align="center">
					<b>2010</b>
				</th>
				<th align="center">
					<b>2011</b>
				</th>
				<th align="center">
					<b>Total</b>
				</th>
			</thead>
			<tr>
				<td align="right">
					<?=number_format($totalGeralAno0,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno1,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno2,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno3,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno4,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno0 + $totalGeralAno1 + $totalGeralAno2 + $totalGeralAno3 + $totalGeralAno4 ,2,',','.');?>
				</td>
			</tr>
		</table>
	</td>
</tr>
</table>
		</td>
	</tr>
</table>
<div style="page-break-before:always" ></div>

<!-- ########################## Termo de Cooperação ########################## -->
<br></br>
<table class="tabela" align="center" bgcolor="" cellspacing="1" cellpadding="3" style="font-family:Arial, Verdana; font-size:10px;">
	<tr>
	<td class="tituloPrincipalAbas"><b>TERMO DE COOPERAÇÃO</b></td>
	</tr>
	<tr>
		<td>
<?php
$sql = "select terdocumento from cte.termo where inuid =".$inuid;	
$termo =  $db->pegaUm($sql);
if($termo){
echo '<div style=" margin-left:28px; PADDING:5px 5px 5px 5px ; width:925px; BORDER: #cccccc 1px solid;"  >'.$termo.'</div>'; 
}else{
?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
				<tr>
				    <td colspan="5" valign="top" class="Erro">
						Não existe termo para este Município.
				 	</td>
				 	</tr>
				 	</table>
<?
}
?>
		</td>
	</tr>
</table>

<div style="page-break-before:always" ></div>

<!-- ########################## Liberação de Recursos FNDE ########################## -->
<br>

<?

$sql = "Select ent.entnumcpfcnpj from entidade.entidade ent
			inner join entidade.funcaoentidade fe on fe.entid = ent.entid
			inner join entidade.endereco ende on ent.entid = ende.entid
		where fe.funid = 1
		and ent.entstatus = 'A'
		and ende.muncod = '".$municipio."'";
$cnpj = $db->pegaUm( $sql );
$ano = date("Y");

	$conexao = new Snoopy;
	$urlReferencia = "http://www.fnde.gov.br/pls/simad/internet_fnde.liberacoes_result_pc?p_ano=%s&p_uf=%s&p_municipio=%s&p_tp_entidade=&p_cgc=%s";
	$url = sprintf($urlReferencia, $ano, $uf, $municipio, $cnpj);
	$conexao->fetch($url);
	$resultado = $conexao->results;
	
	$resultado = str_replace('#000099','#7E8E47',$resultado);
	$resultado = str_replace('#006699','#acbc73',$resultado);
	$resultado = str_replace('#F8C400','#ccd7a4',$resultado);
	$resultado = str_replace('#FFCC66','#ccd7a4',$resultado);
	$resultado = str_replace('<font face="Tahoma,Arial" size="2" color="#acbc73">','<font face="Tahoma,Arial" color="#333333" size="2" >',$resultado);
	$resultado = str_replace('<font face="Tahoma,Arial" size="2" color="#FFFFFF">','<font face="Tahoma,Arial" color="#000000" size="2" >',$resultado);
	$resultado = str_replace('<td align=center>','<td align=center style="FONT-SIZE:10px;">',$resultado);
	$resultado = str_replace('<font face="Tahoma,Arial" size="2">','',$resultado);
	$resultado = str_replace('Volta a consulta de liberações','',$resultado);

?>
<table class="tabela" align="center" bgcolor="" cellspacing="1" cellpadding="3" style="font-family:Arial, Verdana; font-size:10px;">
	<tr>
	<td class="tituloPrincipalAbas"><b>LIBERAÇÃO DE RECURSOS</b></td>
	</tr>
	<tr>
		<td>
<div  style="PADDING-LEFT:10%;">
<?=$resultado;?>
</div>
		</td>
	</tr>
</table>

<div style="page-break-before:always" ></div>

<!-- ########################## Indicadores ########################## -->
<br>
<base href="http://portal.mec.gov.br/ide/2008/" />
<?php
$conexao = new Snoopy;
$urlReferencia = "http://portal.mec.gov.br/ide/2008/gerarTabela.php?municipio=".$municod;

$conexao->fetch($urlReferencia);
$resultadoInd = $conexao->results;
$resultadoInd = str_replace('<img src="images/nova_consulta.gif" alt="Nova Consulta" />','',$resultadoInd);
$resultadoInd = str_replace('<img src="../2008/images/imprimir.png" alt="Imprimir" width="80" height="20" />','',$resultadoInd);
$resultadoInd = str_replace('<img src="../2008/images/logo.gif" alt="Indicadores" width="380" height="57" align="absmiddle" />','',$resultadoInd);




?>



<link rel="stylesheet" type="text/css" href="estilo.css"/>
<table class="tabela" align="center" bgcolor="" cellspacing="1" cellpadding="3" style="font-family:Arial, Verdana; font-size:10px;">
	<tr>
		<td class="tituloPrincipalAbas"><b>INDICADORES DEMOGRÁFICAS  E EDUCACIONAIS</b></td>
	</tr>
	<tr>
		<td>
	<div id="tabelaexterna" style="margin: auto; width: 780px; height:100%;" >
		<?=$resultadoInd; ?>
	</div>
		</td>
	</tr>
</table>
<script>self.print()</script>
</body>
</html>


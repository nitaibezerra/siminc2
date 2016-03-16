<?php
$ptostatus = isset( $_REQUEST['ptostatus'] ) ? $_REQUEST['ptostatus'] : 'A';
$sql ="select
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
			iu.inuid = '".$inuid."' and spt.ssuid = 3
			
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

####################### Funções #######################

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
			
			$valor2007 = $valor[0]['ano2007'] / ($quantidade[0]['ano2007'] > 0 ? $quantidade[0]['ano2007'] : 1) ;
			$valor2008 = $valor[0]['ano2008'] / ($quantidade[0]['ano2008'] > 0 ? $quantidade[0]['ano2008'] : 1);
			$valor2009 = $valor[0]['ano2009'] / ($quantidade[0]['ano2009'] > 0 ? $quantidade[0]['ano2009'] : 1);
			$valor2010 = $valor[0]['ano2010'] / ($quantidade[0]['ano2010'] > 0 ? $quantidade[0]['ano2010'] : 1);
			$valor2011 = $valor[0]['ano2011'] / ($quantidade[0]['ano2011'] > 0 ? $quantidade[0]['ano2011'] : 1);
			$existevalor = $valor2007 + $valor2008 + $valor2009 + $valor2010 + $valor2011;
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

function ordenarArray( $a, $b ){
	return strcmp( strtolower( $a["cosdsc"] ), strtolower( $b["cosdsc"] ) );
}

if($dado != false){
?>

<table class="tabela" align="center" bgcolor="" cellspacing="1" cellpadding="3" style="font-family:Arial, Verdana; font-size:10px;">
	<tr>
	<td class="tituloPrincipalAbas"><b>SÍNTESE DO PAR</b></td>
	</tr>
	<tr>
		<td>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"  > 
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
										//print 'Indicador'.$indicador.'<br>';
										//print 'IndicadorNOVO'.$novoIndicador.'<br>';

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
						}
						?>	
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
<?	
} else {
?>
<table class="tabela" align="center" bgcolor="" cellspacing="1" cellpadding="3" style="font-family:Arial, Verdana; font-size:10px;">
	<tr>
	<td class="tituloPrincipalAbas"><b>SÍNTESE DO PAR</b></td>
	</tr>
	<tr>
		<td  style="text-align:center;">
		<h1  style="font-family:Arial, Verdana; font-size:12px;" >Não existe dados para a síntese do PAR.</h1>
		</td>
	</tr>
</table>	
<?
}
?>
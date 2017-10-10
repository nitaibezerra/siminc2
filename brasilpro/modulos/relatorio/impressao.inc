<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<?php
# ---- Carrega $estuf substituindo a maneira antiga $_SESSION[estuf] -----
$estuf = cte_pegarEstuf($_SESSION["inuid"]);

print '<br/>';

//monta_titulo( $titulo_modulo, '&nbsp;' );

$ptostatus = isset( $_REQUEST['ptostatus'] ) ? $_REQUEST['ptostatus'] : 'A';

/*
 * 		select
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
			subacao.undid,
			subacao.frmid,
			subacao.sbadsc,
			subacao.sbastgmpl,
			subacao.sbaprm,
			subacao.sbapcr,
			coalesce(subacao.sba0ano, 0) as sba0ano,
			coalesce(subacao.sba1ano, 0) as sba1ano,
			coalesce(subacao.sba2ano, 0) as sba2ano,
			coalesce(subacao.sba3ano, 0) as sba3ano,
			coalesce(subacao.sba4ano, 0) as sba4ano,
			coalesce(subacao.sbaunt,0) as sbaunt,
			subacao.sbauntdsc,
			subacao.sba0ini,
			subacao.sba0fim,
			subacao.sba1ini,
			subacao.sba1fim,
			subacao.sba2ini,
			subacao.sba2fim,
			subacao.sba3ini,
			subacao.sba3fim,
			subacao.sba4ini,
			subacao.sba4fim,
			coalesce(u.unddsc,'') as unddsc,
			coalesce(f.frmdsc,'') as frmdsc,
			c.crtdsc,
			c.ctrpontuacao,
			f.frmid,
            prg.prgdsc as prgdsc
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
			left join cte.programa prg on subacao.prgid = prg.prgid
		where
			iu.inuid = '".$_SESSION['inuid']."' 
		order by
			d.dimcod,  
			area.ardcod,
			i.indcod, p.ptoid, acao.aciid, subacao.sbaid;
 * 
 * 
 */


$sql = "
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
		where
			iu.inuid = '".$_SESSION['inuid']."' 
			
		order by
			d.dimcod,  
			area.ardcod,
			i.indcod, p.ptoid, acao.aciid, subacao.sbaid
			
			
			";

//dbg($sql,1 );

$dado = $db->carregar($sql);

$i=0;

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


?>
	
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?php if( !isset( $cabecalhoImprecao ) || $cabecalhoImprecao !== false ): ?>		
<thead>
	<th colspan="2" align="left">
	<?
	$sql = "select estdescricao from territorios.estado where estuf = '" . $estuf . "'";
	$estado = $db->pegaUm($sql);
	?>
	<h1 class="notprint">PAR do Estado:  <?=$estado?></h1>
	
		<table width="100%" bgcolor="#ffffff" border="0" cellpadding="0" cellspacing="0" class="notscreen" style="position: absolute; position: fixed;border-bottom: 1px solid;">
				<tr> 
					<td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
					<td height="20" nowrap>
						<b><?php echo SIGLA_SISTEMA; ?></b>- Sistema Integrado de Ministério da Educação<br>
						Ministério da Educação / SE - Secretaria Executiva<br>
						<b>.:: PAR Analítico do Estado:  <?=$estado?></b><br>	
					</td>
					<td height="20" align="right">
						Impresso por: <strong><?= $_SESSION['usunome']; ?></strong><br>
						Órgão: <?= $_SESSION['usuorgao']; ?><br>
						Hora da Impressão: <?= date("d/m/Y - H:i:s") ?>
					</td>
				</tr>
				<tr> 
					<td colspan="2">&nbsp;</td>
				</tr>
		
			</table>
</thead>
<?php endif; ?>
<? 
while ( $i < $totalreg ) {

	$dimensao = $dado[$i]['dimid'];
		?>
					<tr>
						<td class="SubTituloDireita tdDegradde02">
							<b>Dimensão</b>
						</td>
						<td class="tdDegradde02" style="text-align:left">	
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
									<td class="SubTituloDireita tdDegradde03">
										<b>Área</b>
									</td>
									<td class="tdDegradde03" style="text-align:left">	
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
														<td class="SubTituloDireita tdDegradde04">
															<b>Indicador</b>
														</td>
														<td class="tdDegradde04" style="text-align:left">	
															<?=$dado[$i]['indicador']; ?>
														</td>
													</tr>
													<tr>
														<td class="SubTituloDireita tdDegradde05">
															<b>Critério / Pontuação</b>
														</td>
														<td class="tdDegradde05" style="text-align:left">	
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
													<?if( $dado[$i]['ptodemandaestadual']){?>
													<tr>
														<td class="SubTituloDireita">
															<b>Demanda para Rede Estadual</b>
														</td>
														<td class="" style="text-align:left">	
															<?echo $dado[$i]['ptodemandaestadual']; ?>
														</td>
													</tr>
													<?}?>
													<?if($dado[$i]['ptodemandamunicipal']){?>
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
														//print 'AçãoFORA'.$acao.'<br>';
														//print 'NovaAçãoFORA'.$novaAcao.'<br><p>';
											?>
																<tr>
																	<td class="SubTituloDireita">
																		<b>Ação</b>
																	</td>
																	<td class="" style="text-align:left">
																		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" >
																			<tr>
																				<td  class="SubTituloDireita" style="width:20%;">
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
                                                                
																while ($acao == $novaAcao )
																{
																	mostra_subacao();
																	if($i >= $totalreg ) break;
																}
													}}

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
	
	
<?
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
							FROM cte.subacaoparecertecnico where sbaid =  '".$sbaid."'
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
			WHERE	subacao.sbaid = '".$sbaid."'
		) AS dados";
	return $db->carregar($sql);
	
}





function mostra_subacao(){

	global $dado, $i, $totalreg, $db;
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
							
								<table  class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" >
									<tr>
										<td  class="SubTituloDireita" style="width:20%;" >
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
											<?=$dado[$i]['prgdsc'];?>
	                                                                          
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
											<?=$dado[$i]['frmdsc']; ?>
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
											<table class="listagem" width="100%">
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
														<? //number_format($dado[$i]['sba0ano'],0); ?>
														<?=number_format($quantidade[0]['ano2007'],0); ?>
													</td>
													<td align="right">
														<? //number_format($dado[$i]['sba1ano'],0); ?>
														<?=number_format($quantidade[0]['ano2008'],0); ?>
													</td>
													<td align="right">
														<? //number_format($dado[$i]['sba2ano'],0); ?>
														<?=number_format($quantidade[0]['ano2009'],0); ?>
													</td>
													<td align="right">
														<? //number_format($dado[$i]['sba3ano'],0); ?>
														<?=number_format($quantidade[0]['ano2010'],0); ?>
													</td>
													<td align="right">
														<? //number_format($dado[$i]['sba4ano'],0); ?>
														<?=number_format($quantidade[0]['ano2011'],0); ?>
													</td>
													<td align="right">
														<?
														/*
														$total = 	
														$dado[$i]['sba0ano'] +
														$dado[$i]['sba1ano'] +
														$dado[$i]['sba2ano'] +
														$dado[$i]['sba3ano'] +
														$dado[$i]['sba4ano'];
														echo number_format($total,0);
														*/
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
														<?

														if(!Empty($sub[0]['sba0ini'])) echo $sub[0]['sba0ini'] . " até " ; ?>  <?=$sub[0]['sba0fim']; ?>
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
											</table>
	
											<table 	class="listagem" width="100%"  cellspacing="1" cellpadding="3" >
												<tr>
													<td align="right">
														<b>Valor Unitário:</b>
													</td>
													<td align="right">
														<?=number_format($dado[$i]['sbaunt'],2,',','.');?>
													</td>
												</tr>
											<!--  	
												<tr>
													<td align="right">
														<b>Detalhamento da Composição</br> do Valor Unitário:</b>  	
													</td>
													<td>
														<?=$dado[$i]['sbauntdsc']?>
													</td>
												</tr>
											-->	
											</table>
											
											<table class="listagem" width="100%"  cellspacing="1" cellpadding="3" >
												<thead>
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
												</thead>
												<tr>
													<?
													$ano0 = $valor[0]['ano2007'];
													$ano1 = $valor[0]['ano2008'];
													$ano2 = $valor[0]['ano2009'];
													$ano3 = $valor[0]['ano2010'];
													$ano4 = $valor[0]['ano2011'];
													
													
													//$ano0 = $dado[$i]['sbaunt'] * $dado[$i]['sba0ano'];
													//$ano1 = $dado[$i]['sbaunt'] * $dado[$i]['sba1ano'];
													//$ano2 = $dado[$i]['sbaunt'] * $dado[$i]['sba2ano'];
													//$ano3 = $dado[$i]['sbaunt'] * $dado[$i]['sba3ano'];
													//$ano4 = $dado[$i]['sbaunt'] * $dado[$i]['sba4ano'];
	
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
														<?=number_format($valor[0]['ano2007'],2,',','.');?>
													</td>											
													<td align="right">
														<?=number_format($valor[0]['ano2008'],2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($valor[0]['ano2009'],2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($valor[0]['ano2010'],2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($valor[0]['ano2011'],2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($total,2,',','.') ?>
													</td>
												</tr>		
											</table>
											
	                                <?php
	                                if (trim($dado[$i]['sbaid']) != '') {
	
	                                    $sql = 'select distinct e.entnome, e.entcodent
	                                            from cte.qtdfisicoano q
													inner join entidade.entidade e on e.entid = q.entid
	                                            where q.sbaid = ' . $dado[$i]['sbaid'] . '
												and qfaano != 0
	                                            group by e.entnome, e.entcodent
	                                            order by e.entnome';
									//dbg($sql,1);
	                                $comp = $db->carregar( $sql );
	                                ?>
	                                <tr>
	                                    <td class="SubTituloDireita">Escolas atendidas</td>
	                                    <td>
	                                        <table class="listagem" width="100%"  cellspacing="1" cellpadding="3" >
	                                            <tr>
	                                                <th width="20%">Código INEP</th>
	                                                <th>Escola</th>
	                                            </tr>
	                                            <?php for ($ax=0; $ax < count($comp); $ax++): ?>
	                                            <tr>
	                                                <td width="20%" align="center"><?php echo $comp[$ax]['entcodent']; ?>&nbsp;</td>
	                                                <td><?php echo $comp[$ax]['entnome']; ?>&nbsp;</td>
	                                            </tr>
	                                            <?php endfor;?>
	                                        </table>
	                                    </td>
	                                </tr>
									<?php if ( $existevalor > 0 ) { ?>
	                                   <?php
											$sql1 = "
												select
													cosord,
													cosdsc,
													udt.undddsc,
													cosqtd,
													cosvlruni,
													(cosqtd * cosvlruni) as total
												from 
													cte.composicaosubacao c,
													cte.unidademedidadetalhamento udt
												where
													c.unddid = udt.unddid AND
													sbaid = '" . $dado[$i][sbaid] . "'
												order by
													cosord
											";
	
											$comp = $db->carregar( $sql1 );
											?>
											<tr>
												<td class="SubTituloDireita">Detalhamento dos Itens que Compõem a Especificação</td>
												<td>
													<table class="listagem" width="100%"  cellspacing="1" cellpadding="3" >
														<tr>
															<th>Ordem</th>
															<th>Identificaçâo do Item</th>
															<th>Unidade de Medida</th>
															<th>Qtd.</th>
															<th>Vl. Unid.</th>
															<th>Total</th>
														</tr>
														<?php
														$totalquantidadeitens = 0;
														$totalValorunitario = 0;
														$totalitenscomposicao = 0;
														for ($ax=0; $ax < count($comp); $ax++):				
														?>
														<tr>
															<td><?php echo $comp[$ax][cosord]; ?></td>
															<td><?php echo $comp[$ax][cosdsc]; ?></td>
															<td><?php echo $comp[$ax][undddsc]; ?></td>
															<td><?php echo number_format($comp[$ax][cosqtd],2,'.',''); ?></td>
															<td><?php echo number_format($comp[$ax][cosvlruni],2,',','.'); ?></td>
															<td><?php echo number_format($comp[$ax][total],2,',','.'); ?></td>														
														</tr>
														<?php
															$totalquantidadeitens 	+= $comp[$ax][cosqtd];
															$totalValorunitario 	+= $comp[$ax][cosvlruni];
															$totalitenscomposicao 	+= $comp[$ax][total];
														endfor;
														?>
														<tr>
															<th  class="SubTituloDireita" colspan="3" >Total: </th>
															<td>
																<?php echo number_format($totalquantidadeitens,2,',',''); ?>
															</td>
															<td>
																<?php echo number_format($totalValorunitario,2,',','.'); ?>
															</td>
															<td>
																<?php echo number_format($totalitenscomposicao,2,',','.'); ?>
															</td>
														</tr>
													</table>
												</td>
											</tr>
											<?php }  ?>
																																													
										</td>
									</tr>
	                                <?php
	                                }
	
									$sql2 = "
										SELECT
											sb.vlrurbano,
											sb.vlrrural,
											b.bendsc,
											(sb.vlrurbano+sb.vlrrural) as total
										FROM 
											cte.beneficiario b,
											cte.subacaobeneficiario sb
										WHERE
											b.benid = sb.benid AND
											sb.sbaid = ".$dado[$i]['sbaid']."
									";
									#$sbene = $db->carregar($sql2);
									$sbene = $db->carregar($sql2);
									?>
									<tr>
										<td class="SubTituloDireita">
											Beneficiário
										</td>
										<td>
											<table class="listagem" width="100%"  cellspacing="1" cellpadding="3" >
												<tr>
													<th>Beneficiario</th>
													<th>Zona Rural</th>
													<th>Zona Urbana</th>
													<th>Total</th>
												</tr>
												<?php
												for ($ax=0; $ax < count($sbene); $ax++):				
												?>
												<tr>
													<td><?php echo $sbene[$ax][bendsc]; ?>&nbsp</td>
													<td><?php echo $sbene[$ax][vlrrural]; ?></td>
													<td><?php echo $sbene[$ax][vlrurbano]; ?></td>
													<td><?php echo $sbene[$ax][total]; ?></td>
												</tr>
												<?php
												endfor;											
												?>											
											</table>	
										</td>
									</tr>
									<tr>
										<td class="SubTituloDireita">
											Comentários 	
										</td>
										<td>
											<?=$dado[$i]['sbauntdsc']?>
										</td>
									</tr>
								</table>	
							</td>
						</tr>
						
				<?	
			}	
				$i++;			
				$novaDimensao = $dado[$i]['dimid'];
				$novaArea = $dado[$i]['ardid'];
				$novoIndicador = $dado[$i]['indid'];
				$novaAcao = $dado[$i]['aciid'];
				$novasubAcao = $dado[$i]['sbaid'];

}

?>

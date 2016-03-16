<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<?php



print '<br/>';

//monta_titulo( $titulo_modulo, '&nbsp;' );

$ptostatus = isset( $_REQUEST['ptostatus'] ) ? $_REQUEST['ptostatus'] : 'A';

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
			subacao.undid,
			subacao.frmid,
			subacao.sbadsc,
			subacao.sbastgmpl,
			prg.prgdsc as sbaprm,
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
			f.frmid
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
			i.indcod, p.ptoid, acao.aciid, subacao.sbaid;
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
	$sql = "select estdescricao from territorios.estado where estuf = '" . cte_pegarEstuf( $_SESSION['inuid'] ) . "'";
	$estado = $db->pegaUm($sql);
	?>
	<h1 class="notprint">PAR do Estado: <?=$estado?></h1>
	
		<table width="100%" bgcolor="#ffffff" border="0" cellpadding="0" cellspacing="0" class="notscreen" style="position: absolute; position: fixed;border-bottom: 1px solid;">
				<tr> 
					<td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
					<td height="20" nowrap>
						<b>SIMEC</b>- Sistema Integrado de Ministério da Educação<br>
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
				</th>
			</table>
</thead>
<?php endif; ?>
<? 
while ( $i < $totalreg ) {

	$dimensao = $dado[$i]['dimid'];
		?>
					<tr>
						<td class="SubTituloDireita">
							<b>Dimensão</b>
						</td>
						<td class="" style="text-align:left">	
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
									<td class="SubTituloDireita">
										<b>Área</b>
									</td>
									<td class="" style="text-align:left">	
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
														<td class="SubTituloDireita">
															<b>Indicador</b>
														</td>
														<td class="" style="text-align:left">	
															<?=$dado[$i]['indicador']; ?>
														</td>
													</tr>
													<tr>
														<td class="SubTituloDireita">
															<b>Critério / Pontuação</b>
														</td>
														<td class="" style="text-align:left">	
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
																				<td  class="SubTituloDireita">
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
																$subacao = $dado[$i]['sbaid'];
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





function mostra_subacao(){

	global $dado, $i, $totalreg;
	global $total, $totalGeralAno0 ,$totalGeralAno1, $totalGeralAno2, $totalGeralAno3, $totalGeralAno4;
	global $totalGeralIndicadorAno0 , $totalGeralIndicadorAno1, $totalGeralIndicadorAno2, $totalGeralIndicadorAno3, $totalGeralIndicadorAno4;
	global $totalGeralAreaAno0 , $totalGeralAreaAno1, $totalGeralAreaAno2, $totalGeralAreaAno3, $totalGeralAreaAno4;
	global $totalGeralDimensaAno0 ,$totalGeralDimensaAno1, $totalGeralDimensaAno2, $totalGeralDimensaAno3, $totalGeralDimensaAno4;
	global $novaDimensao, $novaArea, $novoIndicador, $novaAcao, $novasubAcao;
		?>
					<tr>
						<td class="SubTituloDireita">
							<b>Sub-Ação</b>
						</td>
						<td class="" style="text-align:left">	
						
							<table  class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" >
								<tr>
									<td  class="SubTituloDireita">
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
												<td align="right">
													<b>Quantidades:</b>
												</td>
												<td align="right">
													<?=number_format($dado[$i]['sba0ano'],0); ?>
												</td>
												<td align="right">
													<?=number_format($dado[$i]['sba1ano'],0); ?>
												</td>
												<td align="right">
													<?=number_format($dado[$i]['sba2ano'],0); ?>
												</td>
												<td align="right">
													<?=number_format($dado[$i]['sba3ano'],0); ?>
												</td>
												<td align="right">
													<?=number_format($dado[$i]['sba4ano'],0); ?>
												</td>
												<td align="right">
													<?
													$total = 	
													$dado[$i]['sba0ano'] +
													$dado[$i]['sba1ano'] +
													$dado[$i]['sba2ano'] +
													$dado[$i]['sba3ano'] +
													$dado[$i]['sba4ano'];
													echo number_format($total,0);
													 ?>
												</td>
											</tr>											
											<tr>
												<td align="right">
													<b>Cronograma Físico:</b>
												</td>
												<td align="right">
													<?if(!Empty($dado[$i]['sba0ini'])) echo $dado[$i]['sba0ini'] . " até " ; ?>  <?=$dado[$i]['sba0fim']; ?>
												</td>
												<td align="right">
													<?if(!Empty($dado[$i]['sba1ini'])) echo $dado[$i]['sba1ini'] . " até " ; ?>  <?=$dado[$i]['sba1fim']; ?>
												</td>
												
												<td align="right">
													<?if(!Empty($dado[$i]['sba2ini'])) echo $dado[$i]['sba2ini'] . " até " ; ?>  <?=$dado[$i]['sba2fim']; ?>		
												</td>
												<td align="right">
													<?if(!Empty($dado[$i]['sba3ini'])) echo $dado[$i]['sba3ini'] . " até " ; ?> <?=$dado[$i]['sba3fim']; ?>
												</td>
												<td align="right">
													<?if(!Empty($dado[$i]['sba4ini'])) echo $dado[$i]['sba4ini'] . " até " ; ?> <?=$dado[$i]['sba4fim']; ?>
												</td>
												<td align="right">
													&nbsp;
												</td>
											</tr>											
										</table>

										<?php if ( $dado[$i]['sbaunt'] > 0 ) { ?>

										<table 	class="listagem" width="100%"  cellspacing="1" cellpadding="3" >
											<tr>
												<td align="right">
													<b>Valor Unitário:</b>
												</td>
												<td align="right">
													<?=number_format($dado[$i]['sbaunt'],2,',','.');?>
												</td>
											</tr>
											<tr>
												<td align="right">
													<b>Detalhamento da Composição</br> do Valor Unitário:</b>  	
												</td>
												<td>
													<?=$dado[$i]['sbauntdsc']?>
												</td>
											</tr>
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
												$ano0 = $dado[$i]['sbaunt'] * $dado[$i]['sba0ano'];
												$ano1 = $dado[$i]['sbaunt'] * $dado[$i]['sba1ano'];
												$ano2 = $dado[$i]['sbaunt'] * $dado[$i]['sba2ano'];
												$ano3 = $dado[$i]['sbaunt'] * $dado[$i]['sba3ano'];
												$ano4 = $dado[$i]['sbaunt'] * $dado[$i]['sba4ano'];

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
										</table>
										<?php }  ?>
																																												
									</td>
								</tr>
							</table>	
						</td>
					</tr>
					
				<?		$i++;			

				$novaDimensao = $dado[$i]['dimid'];
				$novaArea = $dado[$i]['ardid'];
				$novoIndicador = $dado[$i]['indid'];
				$novaAcao = $dado[$i]['aciid'];
				$novasubAcao = $dado[$i]['sbaid'];

}

?>

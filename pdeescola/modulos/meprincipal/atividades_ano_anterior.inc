<?php

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

// Verifica se o 'memid' existe na sessão.
if(!$_SESSION['memid'] || !$_SESSION['meentid']) {
	echo "<script type=\"text/javascript\">
			alert(\"Ocorreu um erro com a entidade selecionada. Contate o Administrador do sistema.\");
			location.href = \"pdeescola.php?modulo=melista&acao=E\";
		  </script>";
	exit;
}

$sql = "SELECT memid FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION['meentid']." AND memstatus = 'A' AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1);
$memid_ano_anterior = $db->pegaUm($sql);
if(!$memid_ano_anterior) {
	echo "<script type=\"text/javascript\">
			alert(\"Não existe nenhuma informação cadastrada sobre as atividades do ano de ".((integer)$_SESSION["exercicio"] - 1).".\");
			location.href = \"pdeescola.php?modulo=meprincipal/dados_escola&acao=A\";
		  </script>";
	exit;
}

include_once APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . 'includes/workflow.php';

// Recupera a modalidade de ensino.
// Tipos de modalidade de ensino: F - Fundamental, M - Médio e T - Todos.
$modalidadeEnsino = $db->pegaUm("SELECT memmodalidadeensino FROM pdeescola.memaiseducacao WHERE memid = ".$memid_ano_anterior);

// Flag para determinar se a tela será de cadastro ou de consulta
$somenteConsulta = true;

$sql = "SELECT docid FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION["memid"];
$existeDocid = $db->pegaUm($sql);

if($existeDocid) {
	// Recupera ou cria o docid.
	$docid = meCriarDocumento( $_SESSION['meentid'], $_SESSION['memid']  );
	if($docid == false) {
		echo "<script>
				alert('Esta entidade não consta na tabela principal do Mais Educação');
				history.back(-1);
			  </script>";
		die;
	}
}

$sql = "SELECT count(*) FROM seguranca.perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod IN (".PDEESC_PERFIL_CAD_MAIS_EDUCACAO.", ".PDEESC_PERFIL_SUPER_USUARIO.")";
$vPerfilCadMaisEscola = $db->pegaUm($sql);

if((integer)$vPerfilCadMaisEscola > 0) {
	if(!$existeDocid) {
		if((integer)$_SESSION["exercicio"] == meMaxProgramacaoExercicio()) {
			// Recupera ou cria o docid.
			$docid = meCriarDocumento( $_SESSION['meentid'], $_SESSION['memid']   );
			if($docid == false) {
				echo "<script>
						alert('Esta entidade não consta na tabela principal do Mais Educação');
						history.back(-1);
					  </script>";
				die;
			}
		} else {
			$docid = false;
		}
	}

	$esdid = mePegarEstadoAtual( $docid );
//	ver($esdid); /*40*/
	if((integer)$esdid == CADASTRAMENTO_ME || (integer)$esdid == CORRECAO_CADASTRAMENTO_ME) {
		$somenteConsulta = false;
	}
//	$somenteConsulta = false;
}

if($_REQUEST["submetido"]) {
	for($i=0; $i<count($_REQUEST["meaid"]); $i++) {
		$sql = "UPDATE
					pdeescola.meatividade
				SET
					meacomecounoano = ".$_REQUEST["meacomecounoano"][$i].",
					meaqtdefetivaaluno = ".($_REQUEST["meaqtdefetivaaluno"][$i]?$_REQUEST["meaqtdefetivaaluno"][$i]:0)."
				WHERE
					meaid = ".$_REQUEST["meaid"][$i];
		$db->executar($sql);
	}
	
	$db->commit();
	$db->sucesso('meprincipal/atividades_ano_anterior');
}

echo "<br />";
echo montarAbasArray(carregaAbasMaisEducacao(), "/pdeescola/pdeescola.php?modulo=meprincipal/atividades_ano_anterior&acao=A");

$titulo = "Mais Educação - Educação Integral";
$subtitulo = "Cadastro - Atividades ".((integer)$_SESSION["exercicio"] - 1);

monta_titulo($titulo, $subtitulo);

echo cabecalhoMaisEducacao();

?>

<form method="post" id="formAtividadesAnoAnterior" name="formAtividadesAnoAnterior">
<input type="hidden" name="submetido" value="1">
<!-- (Início) Tabela que engloba todo o conteúdo -->
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>	
		<td>
			
			<table align="center">
				<tr>
					<td>
						
						<?
							//$sql = "SELECT count(*) FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION['meentid']." AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1)." AND memstatus = 'A'";
							//$possuiAnoAnterior = $db->pegaUm($sql);
							
							if((integer)$possuiAnoAnterior > 0) {
								
								?>
								<br />
								<table align="center">
									<tr>
										<td>
											<div id="div_valores_pagos" style="display:<?=$displayTabelaValores?>;">
											<?
												$sql = "SELECT
															mme.memvlrpago as memvlrprimeiraparcela,
															mme.memvlrsuplementar,
															mme.mempagofnde,
															mme2.memvlrsaldodisponivel,
															mme2.memvlrsaldosuplementar,
															mme.mesaldototal,
															mme.mesaldocusteio,
															mme.mesaldocapital
														FROM
															pdeescola.memaiseducacao mme
														INNER JOIN
															pdeescola.memaiseducacao mme2 ON mme2.entid = ".$_SESSION['meentid']." AND
																							 mme2.memanoreferencia = " . $_SESSION["exercicio"] . " AND 
																							 mme2.memstatus = 'A'
														WHERE
															mme.entid = ".$_SESSION['meentid']." AND
															mme.memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1)." AND 
															mme.memstatus = 'A'";
												
												$valoresPagos = $db->carregar($sql);
												
												$saldo_reprogramavel_parcela = ($valoresPagos[0]["memvlrsaldodisponivel"]) ? number_format($valoresPagos[0]["memvlrsaldodisponivel"], 2, ",", ".") : "0,00";
												
																	
											
												?>
												<table class="listagem" bgcolor="#E8E8E8" width="50%" cellSpacing="1" cellPadding="3" align="center">
													<tr>
														<td></td>
														<td valign="middle" align="center"><b><?= ((integer)$_SESSION["exercicio"] - 2) ?></b></td>
														<!--<td valign="middle" align="center"><b>Saldo reprogramado</b></td>-->
													</tr>
													<tr>
														<td valign="middle" align="right">Valor&nbsp;Pago:</td>
														<?php /*($valoresPagos[0]["memvlrprimeiraparcela"]) ? number_format($valoresPagos[0]["memvlrprimeiraparcela"], 2, ",", ".") : "0,00"*/ ?>
														<td bgcolor="#FAFAFA"><input type="text" size="20" value="<?= ($valoresPagos[0]["mempagofnde"] == 't') ? number_format($valoresPagos[0]["memvlrprimeiraparcela"], 2, ",", ".") : "0,00" ?>" style="text-align:right;" class="disabled" readonly="readonly"></td>
													</tr>
													
												</table>
												
											
											</div>
										</td>
									</tr>
								</table>
							<?}?>
					
					</td>
					<td>
					
						<?
							$alunadoParticipante = array();
							if($memid_ano_anterior) {
								
								if($modalidadeEnsino == "F")
									$inSeries = "1,2,3,4,5,6,7,8,9";
								else if($modalidadeEnsino == "M")
									$inSeries = "20,21,22";
								else
									$inSeries = "1,2,3,4,5,6,7,8,9,20,21,22";
								
								$sql = "SELECT
											map.mapquantidade as qtd
										FROM
											pdeescola.mealunoparticipante map
										INNER JOIN
											pdeescola.seriecicloescolar sce ON sce.sceid = map.sceid
														AND sce.sceid IN (".$inSeries.")
										WHERE
											map.memid = ".$memid_ano_anterior." AND
											map.mapano = ".((integer)$_SESSION["exercicio"] - 1)."
										ORDER BY
											map.sceid";
								$alunadoParticipante = $db->carregar($sql);
								
							}
						?>
						<br />
						<!-- (Início) Tabela com o Alunado Participante -->
						<table class="listagem" bgcolor="#E8E8E8" width="50%" cellSpacing="1" cellPadding="3" align="center">
							<tr>
								<td rowspan="3" valign="middle" align="center" width="10%"><b>Alunado Participante:</b></td>
								<td colspan="9" valign="middle" align="center" width="60%"><b>Ensino Fundamental</b></td>
								<td colspan="3" valign="middle" align="center" width="20%"><b>Ensino Médio</b></td>
								<td rowspan="2" valign="middle" align="center" width="10%"><b>Total</b></td>
							</tr>
							<tr>
								<!-- Ensino Fundamental -->
								<td valign="middle" align="center"><b>1° Ano</b></td>
								<td valign="middle" align="center"><b>2° Ano</b></td>
								<td valign="middle" align="center"><b>3° Ano</b></td>
								<td valign="middle" align="center"><b>4° Ano</b></td>
								<td valign="middle" align="center"><b>5° Ano</b></td>
								<td valign="middle" align="center"><b>6° Ano</b></td>
								<td valign="middle" align="center"><b>7° Ano</b></td>
								<td valign="middle" align="center"><b>8° Ano</b></td>
								<td valign="middle" align="center"><b>9° Ano</b></td>
								<!-- Ensino Médio -->
								<td valign="middle" align="center"><b>1° Ano</b></td>
								<td valign="middle" align="center"><b>2° Ano</b></td>
								<td valign="middle" align="center"><b>3° Ano</b></td>
							</tr>
							<tr style="background-color:#FAFAFA;">
								<?
									$somaAlunado = 0;
									
									for($i=0; $i<=11; $i++) {
										// Ensino Fundamental
										if($modalidadeEnsino == "F") {
											if(($i>=0) && ($i<=8)) {
												$somaAlunado += (integer)$alunadoParticipante[$i]["qtd"];
												echo "<td valign=\"middle\" align=\"center\">".$alunadoParticipante[$i]["qtd"]."</td>";
											} else {
												echo "<td valign=\"middle\" align=\"center\">-</td>";
											}
										}
										// Ensino Médio
										else if($modalidadeEnsino == "M") {
											if(($i>=9) && ($i<=11)) {
												$somaAlunado += (integer)$alunadoParticipante[($i - 9)]["qtd"];
												echo "<td valign=\"middle\" align=\"center\">".$alunadoParticipante[($i - 9)]["qtd"]."</td>";
											} else {
												echo "<td valign=\"middle\" align=\"center\">-</td>";
											}
										}
										// Ensino Fundamental e Médio
										else {
											$somaAlunado += (integer)$alunadoParticipante[$i]["qtd"];
											echo "<td valign=\"middle\" align=\"center\">".$alunadoParticipante[$i]["qtd"]."</td>";
										}
									}
									
									echo "<td valign=\"middle\" align=\"center\">".$somaAlunado."</td>";
								?>
							</tr>
						</table>
						
					</td>
				</tr>
			</table>
			
			<!-- (Fim) Tabela com o Alunado Participante -->
			<br /><br />
			<?
			if($memid_ano_anterior) {
				
				$sql = "SELECT
							mea.meaid,
							mtm.mtmdescricao,
							mta.mtadescricao,
							mea.meacomecounoano,
							mea.meaqtdefetivaaluno
						FROM
							pdeescola.meatividade mea
						INNER JOIN
							pdeescola.metipoatividade mta ON mta.mtaid = mea.mtaid
										AND mta.mtasituacao = 'A' 
										--AND mta.mtaanoreferencia = ".((integer)$_SESSION["exercicio"] - 1)."
						INNER JOIN
							pdeescola.metipomacrocampo mtm ON mtm.mtmid = mta.mtmid
										AND mtm.mtmsituacao = 'A' 
										--AND mtm.mtmanoreferencia = ".((integer)$_SESSION["exercicio"] - 1)."
						WHERE
							mea.memid = ".$memid_ano_anterior." AND
							mea.meaano = ".((integer)$_SESSION["exercicio"] - 1)."
						ORDER BY
							mea.meaid";
				
				$dadosAtividades = $db->carregar($sql);
				
			}
			?>
			<!-- Instruções -->
			<div align="center">
			<? if($dadosAtividades) { ?>
				<font color="#DF6161">
					<b>Este é o Plano de Atendimento elaborado pela sua Escola em <?=((integer)$_SESSION["exercicio"] - 1)?>.<br />Informe se a Atividade começou em <?=((integer)$_SESSION["exercicio"] - 1)?> e com quantos alunos.</b>
				</font>
			<? } else { ?>
				<font color="#DF6161">
					<b>Não há nenhuma Atividade cadastrada para esta entidade no ano de <?= ((integer)$_SESSION["exercicio"] - 1) ?></b>
				</font>
			<? } ?>
			</div>
			<br />
			<!-- (Início) Tabela com as informações sobre as Atividades -->
			<table width="100%" align="center" border="0" cellspacing="2" cellpadding="5" class="listagem">
				<thead>
					<tr>
						<td width="15%" rowspan="2" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Macrocampo</strong></td>
						<td width="15%" rowspan="2" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Atividade</strong></td>
						<td width="30%" colspan="9" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ensino Fundamental</strong></td>
						<td width="10%" colspan="3" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ensino Médio</strong></td>
						<td width="5%" rowspan="2" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Total</strong></td>
						<td width="15%" rowspan="2" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Começou em <?=((integer)$_SESSION["exercicio"] - 1)?>?</strong></td>
						<td width="10%" rowspan="2" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Qtd. Alunos</strong></td>
					</tr>
					<tr>
						<!-- Ensino Fundamental -->
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>1°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>2°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>3°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>4°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>5°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>6°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>7°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>8°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>9°</strong></td>
						<!-- Ensino Médio -->
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>1°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>2°</strong></td>
						<td valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>3°</strong></td>
					</tr>
				</thead>				
				<tbody>
				<?
				if($dadosAtividades) {
					
					$somaGeralAtividades = array();
					for($i=0; $i<count($dadosAtividades); $i++) {
						$bgColor = ($i % 2) ? "#E8E8E8" : "#FAFAFA";
						
						echo "<tr style=\"background-color:".$bgColor.";\">
								<input type=\"hidden\" name=\"meaid[]\" value=\"".$dadosAtividades[$i]["meaid"]."\" />
								<td valign=\"middle\" align=\"center\">".$dadosAtividades[$i]["mtmdescricao"]."</td>
								<td valign=\"middle\" align=\"center\">".$dadosAtividades[$i]["mtadescricao"]."</td>";
						
						$somaAtividade = 0;
						for($z=0; $z<=11; $z++) {
							$vSerie = (($z>=0) && ($z<=8)) ? ($z + 1) : ($z + 11);
							
							$sql = "SELECT
										map.mpaquantidade
									FROM
										pdeescola.mealunoparticipanteatividade map
									WHERE
										map.meaid = ".$dadosAtividades[$i]["meaid"]." AND 
										map.sceid = ".$vSerie."
									ORDER BY
										map.sceid";
							$numAlunosAtividade = $db->carregar($sql);
							
							$numAlunosAtividade[0]["mpaquantidade"] = (!$numAlunosAtividade[0]["mpaquantidade"]) ? 0 : $numAlunosAtividade[0]["mpaquantidade"];
							
							// Ensino Fundamental
							if($modalidadeEnsino == "F") {
								if(($z>=0) && ($z<=8)) {
									$somaAtividade += (integer)$numAlunosAtividade[0]["mpaquantidade"];
									$somaGeralAtividades[$z] += (integer)$numAlunosAtividade[0]["mpaquantidade"];
									echo "<td valign=\"middle\" align=\"center\">".$numAlunosAtividade[0]["mpaquantidade"]."</td>";
								} else {
									$somaGeralAtividades[$z] = "-";
									echo "<td valign=\"middle\" align=\"center\">-</td>";
								}
							}
							// Ensino Médio
							else if($modalidadeEnsino == "M") {
								if(($z>=9) && ($z<=11)) {
									$somaAtividade += (integer)$numAlunosAtividade[0]["mpaquantidade"];
									$somaGeralAtividades[$z] += (integer)$numAlunosAtividade[0]["mpaquantidade"];
									echo "<td valign=\"middle\" align=\"center\">".$numAlunosAtividade[0]["mpaquantidade"]."</td>";
								} else {
									$somaGeralAtividades[$z] = "-";
									echo "<td valign=\"middle\" align=\"center\">-</td>";
								}
							}
							// Ensino Fundamental e Médio
							else {
								$somaAtividade += (integer)$numAlunosAtividade[0]["mpaquantidade"];
								$somaGeralAtividades[$z] += (integer)$numAlunosAtividade[0]["mpaquantidade"];
								echo "<td valign=\"middle\" align=\"center\">".$numAlunosAtividade[0]["mpaquantidade"]."</td>";
							}
						}
						
						echo "<td valign=\"middle\" align=\"center\">".$somaAtividade."</td>";
						$somaGeralAtividades[12] += (integer)$somaAtividade;
						
						$vSelTrue = $vSelFalse = "";
						if($dadosAtividades[$i]["meacomecounoano"] == 't') { $vSelTrue = "selected=\"selected\""; } 
						if($dadosAtividades[$i]["meacomecounoano"] == 'f') { $vSelFalse = "selected=\"selected\""; }
						
						$vDisabled = ($somenteConsulta) ? "disabled=\"disabled\"" : "";
						
						echo "<td valign=\"middle\" align=\"center\">
								<select class=\"CampoEstilo\" name=\"meacomecounoano[]\" id=\"meacomecounoano\" style=\"width:110px;\" ".$vDisabled.">
									<option value=\"\">-- Selecione --</option>
									<option value=\"true\" ".$vSelTrue.">Sim</option>
									<option value=\"false\" ".$vSelFalse.">Não</option>
								</select>
							  </td>
							  <td valign=\"middle\" align=\"center\">
								<input type=\"text\" class=\"normal\" name=\"meaqtdefetivaaluno[]\" size=\"5\" maxlength=\"5\" onkeypress=\"return somenteNumeros(event);\" value=\"".$dadosAtividades[$i]["meaqtdefetivaaluno"]."\" ".$vDisabled." />
							  </td>";
					}
					
					echo "<tr style=\"background-color:#C0C0C0;\">
							<td colspan=\"2\" valign=\"middle\" align=\"right\"><b>Total:</b></td>";
					for($i=0; $i<count($somaGeralAtividades); $i++) {
						echo "<td valign=\"middle\" align=\"center\">".$somaGeralAtividades[$i]."</td>";
					}
					echo "<td valign=\"middle\" align=\"center\">-</td>
						  <td valign=\"middle\" align=\"center\">-</td>
						</tr>";
				}
				?>
				</tbody>
				<tfoot>
					<tr>
						<td colspan="17" align="right">
						<? if((!$somenteConsulta) && ($dadosAtividades)) { ?>
							<input type="button" id="bt_voltar" style="width:100px; cursor:pointer;" onclick="history.back(-1);" value="Voltar" />
							<input type="button" id="bt_salvar" style="width:100px; cursor:pointer;" onclick="submeteDadosAtividade();" value="Salvar" />
						<? } ?>
						</td>
					</tr>
				</tfoot>
			</table>
			<!-- (Fim) Tabela com as informações sobre as Atividades -->
		</td>
	</tr>
</table>
<!-- (Fim) Tabela que engloba todo o conteúdo -->
</form>
<script type="text/javascript">

function submeteDadosAtividade() {
	var form 		= document.getElementById("formAtividadesAnoAnterior");
	var btSalvar	= document.getElementById("bt_salvar");
	var btVoltar 	= document.getElementById("bt_voltar");
	
	btSalvar.disabled = true;
	btVoltar.disabled = true;
	
	if(!validaForm()) {
		btSalvar.disabled = false;
		btVoltar.disabled = false;
		return;
	}
	
	form.submit();
}

function validaForm() {
	var comecouAno = document.getElementsByName("meacomecounoano[]");
	var qtdEfetiva = document.getElementsByName("meaqtdefetivaaluno[]");
	
	for(var i=0; i<comecouAno.length; i++) {
		if(comecouAno[i].value == "") {
			alert("Informe se a Atividade começou em <?=((integer)$_SESSION["exercicio"] - 1)?>.");
			comecouAno[i].focus();
			return false;
		}
		
		if(comecouAno[i].value == "true") {
			if(qtdEfetiva[i].value == "") {
				alert("Informe a Quantidade de Alunos da Atividade.");
				qtdEfetiva[i].focus();
				return false;
			}
		}
	}
	
	return true;
}

function popupMapa(entid){
	window.open('pdeescola.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
}

</script>
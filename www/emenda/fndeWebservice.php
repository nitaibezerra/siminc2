<?php

function montaTabelaSolicitacaoEmpenho($sql, $ptrid, $boAlterarEmpenho = 0, $boReformulacao = false){
	global $db;
	$boMontaWorkflow = true;
	$arDados = $db->carregar($sql);
	$arDados = $arDados ? $arDados : array();
	
	//if($arDados){
		
		$display = false;
		if( $boReformulacao || $_SESSION['exercicio'] == '2009' ){
			$display = true;
		}

	$exibecab = 'N';
	foreach ($arDados as $chave => $valor) {
		if( (!$valor['exfnumsolempenho'] || empty($valor['exfnumsolempenho']) ) || $boAlterarEmpenho == 1 ){
			$exibecab = 'S'; 		
		}
	}
	$count = 1;
	if ($exibecab == 'S'){
?>
<form action="" method="post" id="formExecPTA" name="formExecPTA">
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5"
		cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td>
				<table id="tb_tabela" class="tabela" width="100%" bgcolor="#f5f5f5"
					cellspacing="1" cellpadding="2" align="center">
					<thead>
						<tr bgcolor="dedfde">
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';"
								onmouseout="this.bgColor='';"><strong>Funcional</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';"
								onmouseout="this.bgColor='';"><strong>Tipo</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';"
								onmouseout="this.bgColor='';"><strong>Código</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';"
								onmouseout="this.bgColor='';"><strong>Autor</strong></td>
							<td align="Center" class="title" width="5%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';"
								onmouseout="this.bgColor='';"><strong>GND</strong></td>
							<td align="Center" class="title" width="5%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';"
								onmouseout="this.bgColor='';"><strong>Mod</strong></td>
							<td align="Center" class="title" width="5%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';"
								onmouseout="this.bgColor='';"><strong>Fonte</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';"
								onmouseout="this.bgColor='';"><strong>Nível de Ensino</strong></td>
									<?php if($boAlterarEmpenho == 1){ ?>
									<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';"
								onmouseout="this.bgColor='';"><strong>Valor Original</strong></td>
									<?php } ?>
									<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';"
								onmouseout="this.bgColor='';"><strong>Valor</strong></td>
							<td align="Center" class="title" width="10%"
										style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; <?=($display ? "display: none" : "display: ''" ); ?>"
										onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Limite
									Autorizado</strong></td>
						</tr>
					</thead>
<?
	} else {?>
		<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
						<tr>
							<td>
								<table id="tb_tabela" class="tabela" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
<?	}

	//$count = 1;	
	//foreach ($arDados as $valorX) {
		//if( ( (!$valorX['exfnumsolempenho'] || empty($valorX['exfnumsolempenho']) ) || $boAlterarEmpenho == 1) && $count == 1 ){
	$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo' );
	
	if( $_SESSION['exercicio'] != '2011' && ( !in_array(APOSTILAMENTO, $arRrefid) && !in_array(RENDIMENTO_DE_APLICACAO, $arRrefid) ) ){
		$sql = "SELECT 
					anasituacaoparecer 
				FROM 
					emenda.analise 
				WHERE 
					ptrid = $ptrid
				    and anatipo = 'M'  
				    and anastatus = 'A'  
					and analote = (SELECT max(analote) 
				    				FROM emenda.analise 
				                    WHERE ptrid = $ptrid and anastatus = 'A' and anatipo = 'M')";
			
		$arSituacao = $db->carregarColuna($sql);
		$boSituacao = false;	
		if( in_array('F', $arSituacao) ){
			$boSituacao = true;
		} else {
			$arErro = array(
						"tipo" => "Empenho",
						"msg"  => "Todos os pareceres mérito deverão está concluídos e favoráveis"
						);
			$_SESSION['emenda']['msgErro'][]=  $arErro;
		}
	}
	foreach ($arDados as $chave => $valor) {
		
		if( (!$valor['exfnumsolempenho'] || empty($valor['exfnumsolempenho']) ) || $boAlterarEmpenho == 1 ){
			$chave % 2 ? $cor = "" : $cor = "#ffffff";
			
									
			/*$sqlFonte = "SELECT DISTINCT 
							upf.fontesiafi as codigo, 
						    upf.fontesiafi ||' - '|| upf.fondsc as descricao
						FROM 
							financeiro.unidadeptresfonte upf
							inner join emenda.responsavel res 
						        on upf.unicod = res.unicod
						WHERE 
							upf.ptres = '".$valor['pliptres']."'
						ORDER BY 
							descricao";*/
			$sqlFonte = "SELECT DISTINCT 
						    f.foscod as codigo, 
						    f.foscod ||' - '|| f.fosdsc as descricao
						FROM financeiro.fontesiafi f 
						INNER JOIN emenda.emendadetalhe  e ON substr(e.foncod,2,2) = f.foncod
						WHERE f.fosemendas = true
							and e.foncod = '".$valor['foncod']."' 
						ORDER BY descricao";
			//or upf.fontesiafi = '0100915011'
			
			$arFonte = $db->carregar($sqlFonte);
			
			if( $valor['casacivil'] == 'Não' && !$display ){
				$casaCivil = 'disabled="disabled"';
				$color = 'red';
			} else {
				$casaCivil = '';
				$color = 'rgb(0, 102, 204)';
			}
			?>	
			<input type="hidden" name="exfid[]"
										id="exfid[<?=$valor['exfid']; ?>]"
										value="<?=$valor['exfid']; ?>">
									<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';"
										onmouseover="this.bgColor='#ffffcc';">
										<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['fupfuncionalprogramatica']; ?></td>
										<td><?=$valor['tipoemenda']; ?></td>
										<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['emecod']; ?></td>
										<td><?=$valor['autnome']; ?></td>
										<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['gndcod']; ?></td>
										<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['mapcod']; ?></td>
										<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['foncod']; ?></td>
										<td><?=$valor['tpedsc']; ?></td>
				<?php
				if($boAlterarEmpenho == 1){
					$boSomenteAnulacao = false;
					// Verifica se o Valor e igual ao valor original
					
					if($valor['pervalor'] == $valor['exfvalor']){
						$boSomenteAnulacao = true;
					}
					
					$pervalor 	   = number_format($valor['pervalor'],2,',','.');
					$valorOriginal = number_format($valor['exfvalor'],2,',','.');
					$disabled = "disabled=\"disabled\"";
					$colspan = "colspan=\"12\"";
				?>
					<td style="text-align: center; color: rgb(0, 102, 204);"><input
											type="hidden" id="pervalorOriginal_<?=$valor['exfid']; ?>"
											value="<?php echo $valorOriginal; ?>">
						R$ <?php echo number_format($valor['exfvalor'],2,',','.'); ?>
					</td>

										<td style="text-align: center; color: rgb(0, 102, 204);"><input
											type="hidden" id="valor_<?=$valor['exfid']; ?>"
											value="<?php echo $pervalor; ?>">
						R$ <?php echo number_format($valor['pervalor'],2,',','.'); ?>
					</td>
										<td style="text-align: center; color: <?=$color ?>; <?=($display ? "display: none" : "display: ''" ); ?>">
						<?php echo $valor['casacivil']; ?>
					</td>
				<?php 
				} else {
					$colspan = "colspan=\"11\"";
					$disabled = "";
					?>
				
					<td style="text-align: center; color: rgb(0, 102, 204);">R$ <?php echo number_format($valor['exfvalor'],2,',','.'); ?></td>
										<td style="text-align: center; color: <?=$color ?>; <?=($display ? "display: none" : "display: ''" ); ?>"><?php echo $valor['casacivil']; ?></td>
				
				<?php } ?>
			</tr>
									<tr bgcolor="<?=$cor; ?>">
										<td <?php echo $colspan;?>>
											<table width="93%" bgcolor="#f5f5f5" cellspacing="1"
												cellpadding="2" align="center">
												<tr bgcolor="dedfde">
													<td width="02%"><img border="0"
														src="../imagens/seta_filho.gif"></td>
													<td align="Center" class="title" width="05%"
														style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
														onmouseover="this.bgColor='#c0c0c0';"
														onmouseout="this.bgColor='';"><strong>PTRES</strong></td>
													<td align="Center" class="title" width="25%"
														style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
														onmouseover="this.bgColor='#c0c0c0';"
														onmouseout="this.bgColor='';"><strong>PI</strong></td>
													<td align="Center" class="title" width="10%"
														style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
														onmouseover="this.bgColor='#c0c0c0';"
														onmouseout="this.bgColor='';"><strong>Esfera ADM</strong></td>
													<td align="Center" class="title" width="10%"
														style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
														onmouseover="this.bgColor='#c0c0c0';"
														onmouseout="this.bgColor='';"><strong>Esfera Orçamentária</strong></td>
													<td align="Center" class="title" width="10%"
														style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
														onmouseover="this.bgColor='#c0c0c0';"
														onmouseout="this.bgColor='';"><strong>Natureza de Despesa</strong></td>
													<td align="Center" class="title" width="25%"
														style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
														onmouseover="this.bgColor='#c0c0c0';"
														onmouseout="this.bgColor='';"><strong>Fonte SIAFI</strong></td>
													<td align="Center" class="title" width="05%"
														style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
														onmouseover="this.bgColor='#c0c0c0';"
														onmouseout="this.bgColor='';"><strong>Ação</strong></td>
												</tr>
												<tr bgcolor="<?=$cor; ?>">
													<td>&nbsp;</td>
													<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['pliptres']; ?></td>
													<td><?=$valor['plicod'].' - '.$valor['plititulo']; ?></td>
													<td><?=$valor['esferaadm']; ?></td>
													<td><?=$valor['esfcod'].' - '.$valor['esfdsc'];; ?></td>
													<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['naturezadesp']; ?></td>
													<td><p class="barNav">
															<a style="text-decoration: none;"> <select
																class="CampoEstilo" name="fontesiafi[]"
																id="fontesiafi_<?=$valor['exfid'] ?>"
																style="width: 80%;" onchange="carregaTextOption(this);"
																<?php echo $disabled;?>>
									<?
									echo '<option value=""></option>';
									if($arFonte){
										foreach ($arFonte as $val) {
											if(sizeof($arFonte) == 1 || ($val['codigo'] == $valor['exfcodfontesiafi']) ){
												$select = 'selected="selected"';
												$label = $val['descricao'];
											} else {
												$select = '';
											}
											echo '<option value="'.$val['codigo'].'|'.$valor['exfid'].'" '.$select.'>'.$val['descricao'].'</option>';
										}
									}
									 ?>
								</select><span id="spanLabel"
																<?=($label ? "" : 'style="display: none;"') ?>><?=$label; ?></span>
																<img border="0" title="Indica campo obrigatório."
																src="../imagens/obrig.gif" /></a>
														</p></td>
													<td style="text-align: center;">
							<?php
							$obWSEmpenho = new WSEmpenho($db);
							if($obWSEmpenho->podeAlterar($valor['exfid']) && $boAlterarEmpenho == 1){
								if(!$boSomenteAnulacao){
							?>
									<img src="../imagens/alterar.gif" style="cursor: pointer;"
														id="imgAlt_<?php echo $valor['exfid']; ?>" border="0"
														onclick="alteraValor('<?php echo $ptrid; ?>','<?php echo $valor['exfid']?>', 0, '')">
								<?php
								} else {
								?>
									<img src="../imagens/excluir.gif" style="cursor: pointer;"
														id="imgAlt_<?php echo $valor['exfid']; ?>" border="0"
														onclick="alteraValor('<?php echo $ptrid; ?>','<?php echo $valor['exfid']?>', 0, '')">
								<?php
								}
								?>
							<?php 
							} else {
							?>
							<!-- img src="../imagens/alterar.gif" style="cursor:pointer;" id="imgAlt_<?php echo $valor['exfid']; ?>" border="0" onclick="alteraValor('<?php echo $ptrid; ?>','<?php echo $valor['exfid']?>')" -->
														<img src="../imagens/alterar_01.gif"
														style="cursor: pointer;"
														id="imgAlt_<?php echo $valor['exfid']; ?>" border="0">
							<?php
							}
							?>
							</td>
												</tr>
											</table>
										</td>
									</tr>
			<?
			
			$estado = pegarEstadoAtual( $ptrid );
			$disabilita = 'disabled="disabled"';
			if( $estado == EM_EMPENHO || $estado == EM_EMPENHO_IMPOSITIVO || $estado == EM_EMPENHO_REFORMULACAO ){
				$disabilita = '';
			}
			
			if( empty($valor['exfnumsolempenho']))
				$tipoemenda = $valor['emetipo'];
			
			$ptrpropostasiconv = $valor['ptrpropostasiconv'];
			
			$desabilitaSiconv = '';
			if( empty($ptrpropostasiconv) && $_SESSION['exercicio'] >= '2012' ){
				$desabilitaSiconv = 'disabled="disabled"';
			}
			
			if ( (!$boAlterarEmpenho && $exibecab == 'S') || $tipoemenda == 'X' ) {
			?>
			<tr bgcolor="<?=$cor; ?>">
										<td colspan="12" style="text-align: center;"><input
											type="button" <?=$desabilitaSiconv; ?> <?=$disabilita; ?>
											value="Solicitar Empenho"
											<?=verificaPermissaoPerfil( 'analise' ); /*echo $casaCivil;*/ ?>
											name="btnSalvar" id="btnSalvar"
											onclick="solicitarEmpenhoPTA('<?php echo $ptrid; ?>', '<?=$valor['exfid'] ?>', 'original');">
				<?if( $_SESSION['usucpf'] == '' ){ ?>
					<input type="button" <?=$desabilitaSiconv; ?> <?=$disabilita; ?>
											value="Solicitar Empenho SICONV"
											<?=verificaPermissaoPerfil( 'analise' ); /*echo $casaCivil;*/ ?>
											name="btnSalvar" id="btnSalvar"
											onclick="solicitarEmpenhoPTA('<?php echo $ptrid; ?>', '<?=$valor['exfid'] ?>', 'original_siconv');">
				<?} ?>
				</td>
									</tr>
			<?php 
			}
		}
	}// fim do foreach $arDadosX?>
		</table>
							</td>
						</tr>
						<tr>
		<?
		$resid = $db->pegaUm("SELECT resid FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");
		$reenalisemerito = $db->pegaUm("SELECT reenalisemerito FROM emenda.responsavelexercicio WHERE prsano = '{$_SESSION['exercicio']}' and resid = ".$resid);
	
	if( $reenalisemerito == 'S' ){
		
		if( $_SESSION['emenda']['msgErro'] && is_array($_SESSION['emenda']['msgErro']) ){
			foreach ($_SESSION['emenda']['msgErro'] as $v) {
				if( $v['tipo'] == 'Empenho' ){
					echo "<td style='color: red;'>".$v['msg']."</td>";
				}
			}			
		}
	}
		?>
	</tr>
		<?php /*	
		} else {// fim do if
			?>
			<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
				<tr>
					<td align="right">
				<?php 
				if($boMontaWorkflow){
					$docid = criarDocumento( $ptrid );
					wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptrid ) );
					$boMontaWorkflow = false;
				}
				?>
			</td>
				</tr>
				<tr>
					<?
					if( $_SESSION['emenda']['msgErro'] && is_array($_SESSION['emenda']['msgErro']) ){
						foreach ($_SESSION['emenda']['msgErro'] as $v) {
							if( $v['tipo'] == 'Empenho' ){
								echo "<td style='color: red;'>".$v['msg']."</td>";
							}
						}
						
					}
					?>
				</tr>
			</table>
			<?
		}
		if( empty($valor['exfnumsolempenho']))
			$tipoemenda = $valor['emetipo'];
	}	// fim do foreach $arDadosX
*/
?>		
</table>
					</form>
<?
	//}
	//$count++;
//} // fim do foreach $arDadosX
	//}
}
function montaTabelaSolicitacaoPagamento($arDados, $ptrid, $boAlterarPagamento = 0, $boDisabled, $valorTotalParcela, $totalParcela = 0, $arParcelaPaga = array(), $boReformulacao = false, $estadoAtual = '' ){
	global $db;
	
	$boPendencia = false;
	if($arDados){
	$obPTA = new PTA();
	
	$obPTA->validaSessionPTA( $ptrid );
	$label = 'Nº Empenho Original';
	if( !$boReformulacao ){
		$federal = $obPTA->buscaEmendaDescentraliza( $ptrid );
		
		if( $federal == 'S' ){
			$label = 'Nota de Crédito';
		} else {
			$label = 'Nº Empenho Original';
		}
	}
	
	$obEmepenho = new WSEmpenho($db);
	
	$arDadosPag = $obEmepenho->selectParaPagamento($ptrid);
	
	$arMSG = array();
	$arMSGExiste = array();
	foreach ($arDadosPag as $v) {
		if( empty($v['enbcnpj']) ) 				 
			if( !in_array( 'enbcnpj', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Nº do cnpj da entidade não foi encontrado vinculado ao PTA!</span>' );
				array_push( $arMSGExiste, 'enbcnpj' );
			}
		if( empty($v['cocseq_conta']) )
			if( !in_array( 'cocseq_conta', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Nº sequencial da conta corrente não encontrado (Empenho não efetivado)!</span>' );
				array_push( $arMSGExiste, 'cocseq_conta' );
			}
		if( empty($v['ptrnumprocessoempenho']) ) 
			if( !in_array( 'ptrnumprocessoempenho', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Nº processo empenho não encontrado (aba informações gerais)!</span>' );
				array_push( $arMSGExiste, 'ptrnumprocessoempenho' );
			}
		if( empty($v['ptranoconvenio']) )
			if( !in_array( 'ptranoconvenio', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Ano do convênio não cadastrado!</span>' );
				array_push( $arMSGExiste, 'ptranoconvenio' );
			}
		if( empty($v['ptrnumconvenio']) )
			if( !in_array( 'ptrnumconvenio', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Nº do convênio não cadastrado!</span>' );
				array_push( $arMSGExiste, 'ptrnumconvenio' );
			}
		if( empty($v['pmcnumconveniosiafi']) ) 	 
			if( !in_array( 'pmcnumconveniosiafi', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Nº do convênio SIAF não cadastrado (É necessário importar o convênio)!</span>' );
				array_push( $arMSGExiste, 'pmcnumconveniosiafi' );
			}
		if( empty($v['exfvalor']) ) 			 
			if( !in_array( 'exfvalor', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Valor do pagamento não informado (aba informações gerais)!</span>' );
				array_push( $arMSGExiste, 'exfvalor' );
			}
		if( empty($v['pubdatapublicacao']) ) 	 
			if( !in_array( 'pubdatapublicacao', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Data de publicação não cadastrada (aba DOU)!</span>' );
				array_push( $arMSGExiste, 'pubdatapublicacao' );
			}
		if( empty($v['exfnumempenhooriginal']) ) 
			if( !in_array( 'exfnumempenhooriginal', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Nº do empenho original não cadastrado (Empenho não efetivado)!</span>' );
				array_push( $arMSGExiste, 'exfnumempenhooriginal' );
			}
	}
	if( !empty( $arMSG ) ) $boPendencia = true;
?>
		<form action="" method="post" id="formExecPTA" name="formExecPTA">
						<input type="hidden" name="boalterou" id="boalterou" value="true">
						<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5"
							cellspacing="1" cellpadding="2" align="center">
							<tr>
								<td>
									<table class="tabela" width="40%" bgcolor="#f5f5f5"
										cellspacing="1" cellpadding="2" align="center">
										<tr>
											<th>Pendências</th>
										</tr>
										<tr>
											<td><?=implode( '<br>', $arMSG ); ?></td>
										</tr>
									</table>
								</td>
							</tr>
							<tr>
								<td>
									<table id="tb_tabela" class="tabela" width="100%"
										bgcolor="#f5f5f5" cellspacing="1" cellpadding="2"
										align="center">
										<thead>
											<tr bgcolor="dedfde">
												<td align="Center" class="title" width="10%"
													style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
													onmouseover="this.bgColor='#c0c0c0';"
													onmouseout="this.bgColor='';"><strong>Funcional</strong></td>
												<td align="Center" class="title" width="10%"
													style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
													onmouseover="this.bgColor='#c0c0c0';"
													onmouseout="this.bgColor='';"><strong>Tipo</strong></td>
												<td align="Center" class="title" width="10%"
													style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
													onmouseover="this.bgColor='#c0c0c0';"
													onmouseout="this.bgColor='';"><strong>Código</strong></td>
												<td align="Center" class="title" width="10%"
													style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
													onmouseover="this.bgColor='#c0c0c0';"
													onmouseout="this.bgColor='';"><strong>Autor</strong></td>
												<td align="Center" class="title" width="5%"
													style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
													onmouseover="this.bgColor='#c0c0c0';"
													onmouseout="this.bgColor='';"><strong>GND</strong></td>
												<td align="Center" class="title" width="5%"
													style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
													onmouseover="this.bgColor='#c0c0c0';"
													onmouseout="this.bgColor='';"><strong>Mod</strong></td>
												<td align="Center" class="title" width="5%"
													style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
													onmouseover="this.bgColor='#c0c0c0';"
													onmouseout="this.bgColor='';"><strong>Fonte</strong></td>
												<td align="Center" class="title" width="10%"
													style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
													onmouseover="this.bgColor='#c0c0c0';"
													onmouseout="this.bgColor='';"><strong>Nível de Ensino</strong></td>
									<?php if($boAlterarPagamento == 1){ ?>
									<td align="Center" class="title" width="10%"
													style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
													onmouseover="this.bgColor='#c0c0c0';"
													onmouseout="this.bgColor='';"><strong>Valor Original</strong></td>
									<?php } ?>
									<td align="Center" class="title" width="10%"
													style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
													onmouseover="this.bgColor='#c0c0c0';"
													onmouseout="this.bgColor='';"><strong>Valor</strong></td>
											</tr>
										</thead>
<?

	foreach ($arDados as $chave => $valor) {
			$chave % 2 ? $cor = "" : $cor = "#ffffff";
			?>	
			<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';"
											onmouseover="this.bgColor='#ffffcc';">
											<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['fupfuncionalprogramatica']; ?></td>
											<td><?=$valor['tipoemenda']; ?></td>
											<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['emecod']; ?></td>
											<td><?=$valor['autnome']; ?></td>
											<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['gndcod']; ?></td>
											<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['mapcod']; ?></td>
											<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['foncod']; ?></td>
											<td><?=$valor['tpedsc']; ?></td>
				<?php 
				if($boAlterarPagamento == 1){
					$boSomenteAnulacao = false;
					// Verifica se o Valor e igual ao valor original
					
					if($valor['pervalor'] == $valor['exfvalor']){
						$boSomenteAnulacao = true;
					}
					
					$pervalor 	   = number_format($valor['pervalor'],2,',','.');
					$valorOriginal = number_format($valor['exfvalor'],2,',','.');
					$disabled = "disabled=\"disabled\"";
					$colspan = "colspan=\"10\"";
				?>
					<td style="text-align: center; color: rgb(0, 102, 204);"><input
												type="hidden" id="pervalorOriginal_<?=$valor['exfid']; ?>"
												value="<?php echo $valorOriginal; ?>">
						R$ <?php echo number_format($valor['exfvalor'],2,',','.'); ?>
					</td>

											<td style="text-align: center; color: rgb(0, 102, 204);"><input
												type="hidden" id="valor_<?=$valor['exfid']; ?>"
												value="<?php echo $pervalor; ?>">
						R$ <?php echo number_format($valor['pervalor'],2,',','.'); ?>
					</td>
				<?php 
				} else {
					$colspan = "colspan=\"9\"";
					$disabled = "";
					?>
				
					<td style="text-align: center; color: rgb(0, 102, 204);">R$ <?php echo number_format($valor['exfvalor'],2,',','.'); ?></td>
				
				<?php } ?>
			</tr>
										<tr bgcolor="<?=$cor; ?>">
											<td <?php echo $colspan;?>>
												<table width="93%" bgcolor="#f5f5f5" cellspacing="1"
													cellpadding="2" align="center">
													<tr bgcolor="dedfde">
														<td width="02%"><img border="0"
															src="../imagens/seta_filho.gif"></td>
														<td align="Center" class="title" width="05%"
															style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
															onmouseover="this.bgColor='#c0c0c0';"
															onmouseout="this.bgColor='';"><strong>PTRES</strong></td>
														<td align="Center" class="title" width="25%"
															style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
															onmouseover="this.bgColor='#c0c0c0';"
															onmouseout="this.bgColor='';"><strong>PI</strong></td>
														<td align="Center" class="title" width="10%"
															style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
															onmouseover="this.bgColor='#c0c0c0';"
															onmouseout="this.bgColor='';"><strong>Esfera ADM</strong></td>
														<td align="Center" class="title" width="10%"
															style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
															onmouseover="this.bgColor='#c0c0c0';"
															onmouseout="this.bgColor='';"><strong>Esfera Orçamentária</strong></td>
														<td align="Center" class="title" width="10%"
															style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
															onmouseover="this.bgColor='#c0c0c0';"
															onmouseout="this.bgColor='';"><strong>Natureza de Despesa</strong></td>
														<td align="Center" class="title" width="25%"
															style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
															onmouseover="this.bgColor='#c0c0c0';"
															onmouseout="this.bgColor='';"><strong>Fonte SIAFI</strong></td>
														<td align="Center" class="title" width="25%"
															style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
															onmouseover="this.bgColor='#c0c0c0';"
															onmouseout="this.bgColor='';"><strong><?=$label; ?></strong></td>
													</tr>
													<tr bgcolor="<?=$cor; ?>">
														<td>&nbsp;</td>
														<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['pliptres']; ?></td>
														<td><?=$valor['plicod'].' - '.$valor['plititulo']; ?></td>
														<td><?=$valor['esferaadm']; ?></td>
														<td><?=$valor['esfcod'].' - '.$valor['esfdsc'];; ?></td>
														<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['naturezadesp']; ?></td>
														<td><?=$valor['fondsc']; ?></td>
														<td><?=$valor['exfnumempenhooriginal']; ?></td>

													</tr>
						<?
						$boValorIgual = false;
						if( ((float)$valorTotalParcela < (float)$valor['exfvalor']) || $valor['spgcodigo'] == 9 ){
							$boValorIgual = true;
						?>
						<tr bgcolor="<?=$cor; ?>">
														<td colspan="8"><input type="hidden"
															name="parcela_<?=$valor['exfid']; ?>"
															id="parcela_<?=$valor['exfid']; ?>"
															value="<?=sizeof( $arParcelaPaga ); ?>"> <input
															type="hidden" name="parcelapaga_<?=$valor['exfid']; ?>"
															id="parcelapaga_<?=$valor['exfid']; ?>"
															value="<?=$valorTotalParcela; ?>"> <input type="hidden"
															name="valorigual_<?=$valor['exfid']; ?>"
															id="valorigual_<?=$valor['exfid']; ?>"
															value="<?=$boValorIgual; ?>"> <input type="hidden"
															name="exfid_p[]"
															id="exfid_p[<?php echo $valor['exfid']; ?>][]"
															value="<?=$valor['exfid']; ?>"> <input type="hidden"
															name="exfvalor_<?=$valor['exfid']; ?>"
															id="exfvalor_<?=$valor['exfid']; ?>"
															value="<?=$valor['exfvalor']; ?>">
															<table width="40%" bgcolor="#f5f5f5"
																id="tabelaParcela_<?php echo $valor['exfid']; ?>"
																cellspacing="1" cellpadding="2">
																<tr>
																
																
																<tbody id="tbodyTabela>">
																	<td colspan="5">
										<?
										if( !$boReformulacao ){
											if( $boDisabled ){
												echo '<span id="linkInserirLinha_'.$valor['exfid'].'" style="margin-left: 3%; cursor: pointer;">
														<img align="top" style="border: medium none ;" src="/imagens/gif_inclui_d.gif"/>Inserir Parcelas
													</span>';								
											} else {
												echo '<span id="linkInserirLinha_'.$valor['exfid'].'" onclick="return carregarParcela( \'\', \'\', \'\', \'\', \''.$valor['exfid'].'\', \'0\', \'\', \'\');" style="margin-left: 3%; cursor: pointer;">
														<img align="top" style="border: medium none ;" src="/imagens/gif_inclui.gif"/>Inserir Parcelas
													</span>';
											}
										}
										?>
									</td>
																	</tr>
																	<tr>
																	</tr>
																	<tr bgcolor="dedfde">
																		<td align="Center" class="title" width="5%"
																			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
																			onmouseover="this.bgColor='#c0c0c0';"
																			onmouseout="this.bgColor='';"><strong>Ação</strong></td>
																		<td align="Center" class="title" width="5%"
																			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
																			onmouseover="this.bgColor='#c0c0c0';"
																			onmouseout="this.bgColor='';"><strong>N°</strong></td>
																		<td align="Center" class="title" width="5%"
																			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
																			onmouseover="this.bgColor='#c0c0c0';"
																			onmouseout="this.bgColor='';"><strong>Mês</strong></td>
																		<td align="Center" class="title" width="5%"
																			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
																			onmouseover="this.bgColor='#c0c0c0';"
																			onmouseout="this.bgColor='';"><strong>Ano</strong></td>
																		<td align="Center" class="title" width="15%"
																			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
																			onmouseover="this.bgColor='#c0c0c0';"
																			onmouseout="this.bgColor='';"><strong>Valor</strong></td>
																	</tr>
																
																
																<tbody id="tbodyTabela_<?php echo $valor['exfid']; ?>">
								<?
								if( $totalParcela == 0 || $valor['spgcodigo'] == 9 ){
								?>
								<tr align="center" id="linha_2"
																		style="background: none repeat scroll 0% 0% rgb(245, 245, 245);">
																		<td></td>
																		<td><input type="text"
																			name="numparcela[<?php echo $valor['exfid']; ?>][]"
																			id="numparcela[]" value="1" size="2"
																			readonly="readonly"></td>
																		<td><input type="hidden"
																			name="exfid[<?php echo $valor['exfid']; ?>][]"
																			id="exfid[1]" value="<?php echo $valor['exfid']; ?>">
																			<input type="hidden"
																			name="orbid[<?php echo $valor['exfid']; ?>][]"
																			id="orbid[1]" value=""> <input type="text"
																			name="orbmesparcela[<?php echo $valor['exfid']; ?>][]"
																			id="orbmesparcela[]" value="" maxlength="2" size="10"
																			onkeyup="this.value=mascaraglobal('[#]',this.value);"
																			onblur="this.value=mascaraglobal('[#]',this.value); validaMesParcela(this,'<?php echo $valor['exfid']; ?>');">
																			<img src="../imagens/obrig.gif"
																			title="indica campo obrigatório."></td>
																		<td><input type="text"
																			name="orbanoparcela[<?php echo $valor['exfid']; ?>][]"
																			id="orbanoparcela[]" value="" maxlength="4" size="10"
																			onkeyup="this.value=mascaraglobal('[#]',this.value);"
																			onblur="this.value=mascaraglobal('[#]',this.value); validaAnoParcela(this,'<?php echo $valor['exfid']; ?>');">
																			<img src="../imagens/obrig.gif"
																			title="indica campo obrigatório."></td>
																		<td><input type="text"
																			name="orbvalorparcela[<?php echo $valor['exfid']; ?>][]"
																			id="orbvalorparcela[]"
																			value="<?php echo number_format($valor['exfvalor'], 2, ',', '.'); ?>"
																			maxlength="13" size="15"
																			onkeyup="this.value=mascaraglobal('[###.]###,##',this.value);"
																			onblur="this.value=mascaraglobal('[###.]###,##',this.value); populaHiddenOrbvalorparcela(this.value,'<?php echo $valor['exfid']; ?>', 1);">
																			<img src="../imagens/obrig.gif"
																			title="indica campo obrigatório."> <input
																			type="hidden"
																			name="valor1_[<?php echo $valor['exfid']; ?>]_1"
																			id="valor1_[<?php echo $valor['exfid']; ?>]_1"
																			value="<?php echo $valor['exfvalor']; ?>"></td>
																	</tr>
								<?} ?>
							
															
															
															
															</table></td>
													</tr>
						<?} else {?>
						<input type="hidden" name="valorigual_<?=$valor['exfid']; ?>"
														id="valorigual_<?=$valor['exfid']; ?>"
														value="<?=$boValorIgual; ?>">
													<table width="40%" bgcolor="#f5f5f5"
														id="tabelaParcela_<?php echo $valor['exfid']; ?>"
														cellspacing="1" cellpadding="2">
														<tr>
														
														
														<tbody id="tbodyTabela>">
															</tr>
														
														
														<tbody id="tbodyTabela_<?php echo $valor['exfid']; ?>">
						<?} ?>
					
													
													
													
													</table>
													</td>
													</tr>
			<?
		//}	// fim do if 					
	}	// fim do foreach $arDadosX		

	$sql = "SELECT 
				anasituacaoparecer 
			FROM 
				emenda.analise 
			WHERE 
				ptrid = $ptrid
			    and anatipo = 'T'  
			    and anastatus = 'A'  
				and analote = (SELECT max(analote) 
			    				FROM emenda.analise 
			                    WHERE ptrid = $ptrid and anastatus = 'A' and anatipo = 'T')";
		
	$arSituacao = $db->carregarColuna($sql);
	/*$boSituacao = false;	
	if( in_array('F', $arSituacao) ){
		$boSituacao = true;
	} else {
		$arErro = array(
					"tipo" => "Pagamento",
					"msg"  => "Todos os pareceres técnicos deverão está concluídos e favoráveis"
					);
		$_SESSION['emenda']['msgErro'][]=  $arErro;
	}*/
?>
			</table>
											</td>
											<td align="right" rowspan="3">
			<?php 
			$docid = criarDocumento( $ptrid );
			wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptrid, 'tipo' => "" ) );
			?>
		</td>

										</tr>
										<tr>
		<?
		/*if( $_SESSION['emenda']['msgErro'] && is_array($_SESSION['emenda']['msgErro']) ){
			foreach ($_SESSION['emenda']['msgErro'] as $v) {
				if( $v['tipo'] == 'Pagamento' ){
					echo "<td style='color: red;'>".$v['msg']."</td>";
				}
			}
			
		}*/
		?>
	</tr>
									</table>
									</form>
<?
		if ( empty($boDisabled) && $boValorIgual && !$boPendencia ) {
		?>
			<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5"
										cellspacing="1" cellpadding="2" align="center">
										<tr bgcolor="#D0D0D0">
											<td colspan="2" style="text-align: center"><input
												type="button" value="Solicitar Pagamento"
												<?=disabled($estadoAtual); ?> name="btnSalvar"
												id="btnSalvar"
												onclick="solicitarPagamentoPTA('<?php echo $ptrid; ?>');"></td>
										</tr>
									</table>
		<?php 
		} else {
		?>
			<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5"
										cellspacing="1" cellpadding="2" align="center">
										<tr bgcolor="#D0D0D0">
											<td colspan="2" style="text-align: center"><input
												type="button" value="Solicitar Pagamento"
												disabled="disabled" name="btnSalvar" id="btnSalvar"></td>
										</tr>
									</table>
		<?php 	
		}
	} else {// fim do if
			?>
			<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5"
										cellspacing="1" cellpadding="2" align="center">
										<tr>
											<td align="right">
				<?php 
				$docid = criarDocumento( $ptrid );
				wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptrid, 'tipo' => "" ) );
				?>
			</td>
										</tr>
									</table>
			<?
		}// fim do foreach $arDadosX
}


function pegaDadosCronograma($db = null, $post){
	if(!$db){
		global $db;		
	}
	$sql = "";
}
function solicitarConvenio(&$db = null, $post){
	if(!$db){
		global $db;		
	}
	
	$usuario = $post['usuario'];
	$senha   = $post['senha'];
	$ptrid 	 = $post['ptrid'];
	$exfid 	 = $post['exfid'];
	$xmlRetorno = "";
	
	$sql = "SELECT
			  ent.enbcnpj AS cnpj,
			  ent.muncod,
			  ent.estuf,
			  ptrnumconvenio,
  			  ptranoconvenio,
  			  mun.muncodsiafi
			FROM
			  emenda.entidadebeneficiada ent INNER JOIN emenda.planotrabalho ptr
			  ON (ent.enbid = ptr.enbid) 
			  LEFT JOIN territorios.municipio mun
				INNER JOIN territorios.estado est ON (est.estuf = mun.estuf) 
			  ON (mun.muncod = ent.muncod) 
			  INNER JOIN emenda.responsavel res
			  ON (res.resid=ptr.resid)
			  inner join emenda.ptemendadetalheentidade ped
  					on ped.ptrid = ptr.ptrid
			WHERE
			  ptr.ptrid = $ptrid
		    GROUP BY
			  ent.enbcnpj,
			  ent.muncod,
			  ent.estuf,
			  ptrnumconvenio,
  			  ptranoconvenio,
  			  mun.muncodsiafi
  			  ";
	
	$obPlanoTrabalho = new PlanoTrabalho();
	$arPlanoTrabalho = $db->pegaLinha($sql);
	$arPlanoTrabalho = ($arPlanoTrabalho) ? $arPlanoTrabalho : array();

	$anoAtual     = date('Y');
	$dataAtualXml = date("c");
	
		if(!$arPlanoTrabalho['ptrnumconvenio']){
			$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
<header>
<app>string</app>
<version>string</version>
<created>$dataAtualXml</created>
</header>
	<body>
	<params>
		<co_programa_fnde>03</co_programa_fnde>
		<an_exercicio>$anoAtual</an_exercicio>
	</params>
	</body>
</request>
XML;
				
		if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			//$urlWS = "http://172.20.200.116/webservices/sigefemendas/integracao/web/dev.php/convenio/consultar";						
			$urlWS = "http://hmg.fnde.gov.br/webservices/sigefemendas/index.php/convenio/consultar";
		} else {
			$urlWS = "http://www.fnde.gov.br/webservices/sigefemendas/index.php/convenio/consultar";
		}
		try {
			$xsd = APPRAIZ.'emenda/modulos/sistema/ws-sigef/xsd/xml.fnde.ws-sigef.convenio.consultar.request.xsd';
			$arMessage = validaXML( $arqXml, $xsd );

		    if( empty( $arMessage ) ){
		    	$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
					->execute();
		    } else {
		    	echo 'Erro-WS: validação xml - Consulta Convenio: <br>';
		    	foreach ($arMessage as $msg) {
		    		print_r( $msg['text'] );
		    	}
		    	die;
		    }
			/*$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
				->execute();*/

			$xmlRetorno = $xml;
			
			$arrParam = array(
							'ptrid' => $ptrid,
							'exfid' => $exfid,
							'logTipo' => 'CO',
							'xmlEnvio' => $arqXml,
							'xmlRetorno' => $xmlRetorno,
							);
			
			logWsSIGEF($arrParam);
			
			$xml = simplexml_load_string( stripslashes($xml));

			if ( (int) $xml->status->result ){
		        $obConvenio = $xml->body->children();
		        
		        $nu_convenio  = (int) $obConvenio->nu_convenio;
		        $an_exercicio = (int) $obConvenio->an_exercicio;
				if($nu_convenio && $an_exercicio){
					$arPlanoTrabalho['ptranoconvenio'] = $an_exercicio;
					$arPlanoTrabalho['ptrnumconvenio'] = $nu_convenio;
					
					# Atualiza a tabela de PlanoTrabalho.
					$obPlanoTrabalho = new PlanoTrabalho($ptrid);
					$obPlanoTrabalho->ptrnumconvenio = $arPlanoTrabalho['ptrnumconvenio']; 
					$obPlanoTrabalho->ptranoconvenio = $arPlanoTrabalho['ptranoconvenio'];
					$obPlanoTrabalho->salvar();
					$obPlanoTrabalho->commit();
					unset($obPlanoTrabalho);
					
					return $arPlanoTrabalho;
				} else {
					$erro = 'Não foi possível recuperar Número e o Ano do convênio.';
					
					echo "<span style='color: red;'><b>Detalhes do erro - Solicitar Convênio:</b><br/><br/>".$erro."</span>";
					return false;
				}
		    } else { 
		    	$erro = utf8_decode($xml->status->error->message->text);
				
		    	echo "<span style='color: red;'><b>Detalhes do erro - Solicitar Convênio:</b><br/><br/>".$erro."</span>";
		    	return false;
		    }

		} catch (Exception $e){
			/**
			 * @var LogErroWS
			 * Bloco que grava o erro em nossa base
			 */
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
				
			$arrParam = array(
							'ptrid' => $ptrid,
							'exfid' => $exfid,
							'logTipo' => 'CO',
							'xmlEnvio' => $arqXml,
							'xmlRetorno' => $xmlRetorno.' - Exception: '.$erroMSG,
							);
			
			logWsSIGEF($arrParam);
			
			# Erro 404 página not found
			if($e->getCode() == 404){
				$erro = "Erro-Serviço Convênio encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
				echo "<span style='color: red;'><b>Detalhes do erro - Solicitar Convênio:</b><br/><br/>".$erro."</span>";
				return false;
			} else {
				$erro = 'Erro-WS Consulta Convenio: '.$erroMSG;				
				echo "<span style='color: red;'><b>Detalhes do erro - Solicitar Convênio:</b><br/><br/>".$erro."</span>";
				return false;
			}
		}	
	} else {
		return $arPlanoTrabalho;
	}
}


function solicitarProcesso(&$db = null, $post){
	if(!$db){
		global $db;		
	}
	
	$usuario = $post['usuario'];
	$senha   = $post['senha'];
	$ptrid 	 = $post['ptrid'];
	$exfid 	 = $post['exfid'];
	$xmlRetorno = "";
	
	$sql = "SELECT
			  ent.enbcnpj AS cnpj,
			  ptr.ptrnumprocessoempenho
			FROM
			  emenda.entidadebeneficiada ent INNER JOIN emenda.planotrabalho ptr ON (ent.enbid = ptr.enbid)
			WHERE
			  ptr.ptrid = $ptrid
			  ";

	$arProcesso = $db->pegaLinha($sql);
	$arProcesso = ($arProcesso) ? $arProcesso : array();
	
	
	/*$arProcesso = array();
	//$arProcesso['cnpj'] = '';
	$arProcesso['cnpj'] = '';
	$arProcesso['ptrnumprocessoempenho'] = '23034105642200959';*/
	
	$anoAtual     = date('Y');
	$dataAtualXml = date("c");
		if($arProcesso['ptrnumprocessoempenho']){
			$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
<header>
<app>string</app>
<version>string</version>
<created>$dataAtualXml</created>
</header>
	<body>
	<params>
      <nu_cnpj>{$arProcesso['cnpj']}</nu_cnpj>
      <nu_processo>{$arProcesso['ptrnumprocessoempenho']}</nu_processo>
      <tp_processo>1</tp_processo>
      <an_processo>$anoAtual</an_processo>
      <co_programa_fnde>03</co_programa_fnde>
    </params>
	</body>
</request>
XML;

		if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			//$urlWS = "http://172.20.200.116/webservices/corp/integracao/web/dev.php/processo/solicitar";						
			$urlWS = "http://hmg.fnde.gov.br/webservices/corp/index.php/processo/solicitar";						
		} else {
			$urlWS = "http://www.fnde.gov.br/webservices/corp/index.php/processo/solicitar";
		}

		try {
			/*$xsd = APPRAIZ.'emenda/modulos/sistema/ws-sigef/xsd/xml.fnde.ws-sigef.empenho.solicitar.request.xsd';
			$arMessage = validaXML( $arqXml, $xsd );
		    
		    if( empty( $arMessage ) ){
		    	$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
					->execute();
		    } else {
		    	$xml = $arMessage;
		    }*/
		    $xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
					->execute();
			
			$xmlRetorno = $xml;
			
			$arrParam = array(
							'ptrid' => $ptrid,
							'exfid' => $exfid,
							'logTipo' => 'P',
							'xmlEnvio' => $arqXml,
							'xmlRetorno' => $xmlRetorno,
							);
			
			logWsSIGEF($arrParam);
			
			$xml = simplexml_load_string( stripslashes($xml));
			
			if ( (int) $xml->status->result != 1 ){
				echo "<span style='color: red;'><b>Detalhes do erro - Solicitar Processo:</b><br/><br/>".utf8_decode($xml->status->error->message->text)."</span>";
				return false;
		    } else {
		    	return true;
		    }
		} catch (Exception $e){
			/**
			 * @var LogErroWS
			 * Bloco que grava o erro em nossa base
			 */
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			
			$arrParam = array(
							'ptrid' => $ptrid,
							'exfid' => $exfid,
							'logTipo' => 'P',
							'xmlEnvio' => $arqXml,
							'xmlRetorno' => $xmlRetorno.' - Exception: '.$erroMSG,
							);
							
			logWsSIGEF($arrParam);
			
			# Erro 404 página not found
			if($e->getCode() == 404){
				$erro = "Erro-Serviço Solicitar Processo encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
				echo "<span style='color: red;'><b>Detalhes do erro:</b><br/><br/>".$erro."</span>";
				return false;
			}
			$erro = "Erro-WS Processo Solicitar: ".$e->getMessage();
			echo "<span style='color: red;'><b>Detalhes do erro:</b><br/><br/>".$erro."</span>";
			return false;
		}	
	}	
}

function enviarPropostaSiconv($post){
	global $db;
	
	$usuario = $post['usuario'];
	$senha 	 = $post['senha'];
	
	$arrParam = array('ptrid' 	=> $post['ptrid'],
					  'exfid' 	=> $post['exfid'],
					  'usuario' => $usuario,
					  'senha' 	=> $senha,
					  'url' 	=> $urlWsdl,
					  'post' 	=> $post
					);
	 
	$obWS = new WSIntegracaoSiconv($arrParam);
	$retorno = $obWS->enviaPropostaWS();
	
	/*if(!$erro){					
		echo "ok";
		die;						
	} else {
		$db = new cls_banco();
		$logresponse = $db->pegaLinha("SELECT logresponse, logtipo FROM emenda.logerrosiconv WHERE logid='".$erro."'");
		
		if( $logresponse['logtipo'] == 'enviarProposta' ){
			$posicaoIni = strrpos( $logresponse['logresponse'], '<mensagem>');
			$mensagem = str_replace("<mensagem>","",substr($logresponse['logresponse'],$posicaoIni));
			$posicaoIni = strpos( $mensagem, '</mensagem>');
			$mensagem = substr($mensagem, 0, $posicaoIni);
		} else {
			$posicaoIni = strrpos( $logresponse['logresponse'], '<messages>');
			$mensagem = str_replace("<messages>","",substr($logresponse['logresponse'],$posicaoIni));
			$posicaoIni = strpos( $mensagem, '</messages>');
			$mensagem = substr($mensagem, 0, $posicaoIni);
		}*/
		
		
		echo "<span style='color: blue;'><b>Detalhes do retorno:</b><br/><br/>";
		echo simec_htmlentities($retorno)."</span>";
		
		die;
	
	//}
}

function exportaConvenioSiconv($post){
	
	$usuario = $post['usuario'];
	$senha 	 = $post['senha'];
	
	$arrParam = array('ptrid' 	=> $post['ptrid'],
					  'exfid' 	=> $post['exfid'],
					  'usuario' => $usuario,
					  'senha' 	=> $senha,
					  'service' => 'siconv',
					  'post' 	=> $post
					);
	
	$obWS = new WSSiconvConvenio($arrParam);
	$obWS->exportaConvenio();
	
	echo "<span style='color: blue;'><b>Detalhes do retorno:</b><br/><br/>";
	echo simec_htmlentities($retorno)."</span>";
	
	die;
}

function solicitarEmpenhoSiconv($post){
	global $db;
	
	$usuario = $post['usuario'];
	$senha 	 = $post['senha'];
	
	//$arPlanoTrabalho = consultarConvenioIntraSiconv($post);
	
	$arrParam = array('ptrid' 	=> $post['ptrid'],
					  'exfid' 	=> $post['exfid'],
					  'usuario' => $usuario,
					  'senha' 	=> $senha,
					  'service' => 'convenio',
					  'post' 	=> $post
					);
	
	//include_once APPRAIZ."emenda/classes/WSSiconvConvenio.class.inc";
	$obWS = new WSSiconvConvenio($arrParam);
	$retorno = $obWS->enviarNotaEmpenho();
	
	echo "<span style='color: blue;'><b>Detalhes do retorno:</b><br/><br/>";
	echo simec_htmlentities($retorno)."</span>";
	
	die;
}

function solicitarEmpenhoPTA($post){
	global $db;

	$ptrid 	 = $post['ptrid'];
	$especie_empenho = $post['especie_empenho'];	
	
	$obWSEmpenho = new WSEmpenho($db);
	
	if( $especie_empenho == 'cancelarConvenio' ){
		$obWSEmpenho->cancelarConvenioRAP( $post );
		unset($obWSEmpenho);
	} else {	
		$existePta = $obWSEmpenho->existePta($ptrid);
		if($existePta) {
			# consultaEmpenho é relacionado ao link da lista de empenhados
			if($especie_empenho == 'consultaEmpenho'){
				/*if( $_SESSION['exercicio'] >= '2012' ){
					$usuario = $post['usuario'];
					$senha 	 = $post['senha'];
					
					$arrParam = array('ptrid' 	=> $post['ptrid'],
									  'exfid' 	=> $post['exfid'],
									  'usuario' => $usuario,
									  'senha' 	=> $senha,
									  'url' 	=> $urlWsdl,
									  'post' 	=> $post
									);

					$obWS = new WSIntegracaoSiconv($arrParam);
					$erro = $obWS->consultarEmpenhoWS();
					
					if($erro == 'vazio'){
						$mensagem = 'SICONV não retornou resposta.';
					} else if(!$erro){					
						echo "ok";
						die;						
					} else {
						$db = new cls_banco();
						$logresponse = $db->pegaLinha("SELECT logresponse, logtipo FROM emenda.logerrosiconv WHERE logid='".$erro."'");
						$logresponse['logresponse'] = trim( $logresponse['logresponse'] );
						
						if( !empty($logresponse['logresponse']) ){
							if( $logresponse['logtipo'] == 'enviarProposta' ){
								$posicaoIni = strpos( $logresponse['logresponse'], '<mensagem>');
								$posicaoFim = strstr( $logresponse['logresponse'], '</mensagem>');
								$mensagem = str_replace("<mensagem>","",substr($logresponse['logresponse'],$posicaoIni,($posicaoFim-$posicaoIni)));
							} else {
								$posicaoIni = strpos( $logresponse['logresponse'], '<messages>');
								$posicaoFim = strstr( $logresponse['logresponse'], '</messages>');
								$mensagem = str_replace("<messages>","",substr($logresponse['logresponse'],$posicaoIni,($posicaoFim-$posicaoIni)));							
							}
						} 
					}
					echo "<span style='color: red;'><b>Detalhes do erro:</b><br/><br/>";
					echo simec_htmlentities($mensagem)."</span>";						
					die;					
				} else {*/
					$usuario = $post['usuario'];
					$senha 	 = $post['senha'];
					$obWSEmpenho->consultarEmpenho($post, true);
				//}			
			} else {
				/*$obHabilita = new Habilita($db);
				$cnpj = $obHabilita->pegaCnpj($ptrid);
				
				# Consulta Habilita
				$boHabilitado = $obHabilita->consultaHabilita($cnpj);
				unset($obHabilita);*/
				/*
				$arPlanoTrabalho = array();
				$arPlanoTrabalho['ptranoconvenio'] = 657699;
				$arPlanoTrabalho['ptrnumconvenio'] = 2009;
				*/
				
				/*if( $_SESSION['exercicio'] >= '2012' && $especie_empenho == 'original' ){
					
					
					
				} else {*/
					# Solicitamos Vinculação do Processo a entidade
					$boErro = solicitarProcesso($db,$post);
					if( $boErro ){					
						# Solicita Convênio
						//$arPlanoTrabalho = solicitarConvenio($db,$post);
						$arPlanoTrabalho = consultarConvenioIntraSiconv($post);
						
						if( is_array($arPlanoTrabalho) && $arPlanoTrabalho['ptrnumconvenio'] ){
							# Solicita Conta Corrente
							if( $post['solicitarconta'] == 'S' ){
								$obWSContaCorrente = new WSContaCorrente($db);
								$boCC = $obWSContaCorrente->solicitarContaCorrente($post);
								unset($obWSContaCorrente);
							}
							
							if($especie_empenho == 'original'){
								$erro = $obWSEmpenho->solicitarEmpenhoOriginal($post, $arPlanoTrabalho);
							} elseif($especie_empenho == 'reforco' || $especie_empenho == 'anulacao'){
								$erro = $obWSEmpenho->solicitarEmpenhoReforcoAnulacao($post, $arPlanoTrabalho);
							}
						} else {
							$erro = "Erro-Não foi Possível recuperar dados do Convênio.";
							echo "<span style='color: red;'><b>Detalhes do erro - Solicitar Convênio:</b><br/><br/>".$erro."</span>";
						}
					}
				//}
				unset($obWSEmpenho);
			}
		} else {// Fim ExistePTA
			echo "<span style='color: red;'><b>Detalhes do erro - Não existe Plano de Trabalho cadastrado:</b><br/><br/></span>";
		}
	}

}

function consultarConvenioIntraSiconv($post){
	global $db;
	
	$sql = "SELECT
			  ent.enbcnpj AS cnpj,
			  ent.muncod,
			  ent.estuf,
			  ptrnumconvenio,
  			  ptranoconvenio,
  			  mun.muncodsiafi
			FROM
			  emenda.entidadebeneficiada ent INNER JOIN emenda.planotrabalho ptr
			  ON (ent.enbid = ptr.enbid) 
			  LEFT JOIN territorios.municipio mun
				INNER JOIN territorios.estado est ON (est.estuf = mun.estuf) 
			  ON (mun.muncod = ent.muncod) 
			  INNER JOIN emenda.responsavel res
			  ON (res.resid=ptr.resid)
			  inner join emenda.ptemendadetalheentidade ped
  					on ped.ptrid = ptr.ptrid
			WHERE
			  ptr.ptrid = {$post['ptrid']}
		    GROUP BY
			  ent.enbcnpj,
			  ent.muncod,
			  ent.estuf,
			  ptrnumconvenio,
  			  ptranoconvenio,
  			  mun.muncodsiafi";
	$arrDados = $db->pegaLinha($sql);
	
	$usuario = $post['usuario'];
	$senha 	 = $post['senha'];
	
	if( empty($arrDados['ptrnumconvenio']) ){
		$arrParam = array('ptrid' 	=> $post['ptrid'],
						  'exfid' 	=> $post['exfid'],
						  'usuario' => $usuario,
						  'senha' 	=> $senha,
						  'url' 	=> $urlWsdl,
						  'post' 	=> $post
						);
		
		$obWS = new WSIntegracaoSiconv($arrParam);
		$retorno = $obWS->consultaConvenioWS();
		
		$ptrnumconvenio = $retorno->consultarConvenioReturn->numeroConvenio;
		
		$sql = "UPDATE emenda.planotrabalho SET 
	  				ptrnumconvenio = '{$ptrnumconvenio}',
	  				ptranoconvenio = '{$_SESSION['exercicio']}'
				WHERE 
	  				ptrid = {$post['ptrid']}";
		
		$db->executar($sql);
		$db->commit();
		
		$ptranoconvenio = $_SESSION['exercicio'];
		$ptrnumconvenio = $retorno->consultarConvenioReturn->numeroConvenio;
	} else {
		$ptranoconvenio = $arrDados['ptranoconvenio'];
		$ptrnumconvenio = $arrDados['ptrnumconvenio'];
	}
	
	$arPlanoTrabalho = array(
							'cnpj' => $arrDados['cnpj'], 
							'estuf' => $arrDados['estuf'], 
							'ptranoconvenio' => $ptranoconvenio, 
							'ptrnumconvenio' => $ptrnumconvenio
							);
	return $arPlanoTrabalho;
}

function solicitarPagamentoPTA($post){
	global $db;
	
	$ptrid 	 = $post['ptrid'];
	$especie_pagamento = $post['especie_pagamento'];
	
	//$obWSEmpenho = new WSEmpenho($db);
	$obPagamento = new PagamentoFNDE();
	$obPagamento->setPta( $ptrid );
	/*$existePta = $obWSEmpenho->existePta($ptrid);
	if($existePta) {*/
	/*if( $_SESSION['exercicio'] >= '2012' ){
		$usuario = $post['usuario'];
		$senha 	 = $post['senha'];
		
		$arrParam = array('ptrid' 	=> $post['ptrid'],
						  'exfid' 	=> $post['exfid'],
						  'usuario' => $usuario,
						  'senha' 	=> $senha,
						  'url' 	=> $urlWsdl,
						  'post' 	=> $post
						);
		
		$obWS = new WSIntegracaoSiconv($arrParam);
		
		//$specodigosiconv = $db->pegaUm("SELECT specodigosiconv FROM emenda.siconvtpaemenda WHERE ptrid = {$post['ptrid']}");
		
		$erro = $obWS->solicitarOrdemBancaria();
		
		if(!$erro){					
			echo "ok";
			die;						
		} else {
			$db = new cls_banco();
			$logresponse = $db->pegaLinha("SELECT logresponse, logtipo FROM emenda.logerrosiconv WHERE logid='".$erro."'");
			
			if( $logresponse['logtipo'] == 'enviarProposta' ){
				$posicaoIni = strrpos( $logresponse['logresponse'], '<mensagem>');
				$mensagem = str_replace("<mensagem>","",substr($logresponse['logresponse'],$posicaoIni));
				$posicaoIni = strpos( $mensagem, '</mensagem>');
				$mensagem = substr($mensagem, 0, $posicaoIni);
			} else {
				$posicaoIni = strrpos( $logresponse['logresponse'], '<messages>');
				$mensagem = str_replace("<messages>","",substr($logresponse['logresponse'],$posicaoIni));
				$posicaoIni = strpos( $mensagem, '</messages>');
				$mensagem = substr($mensagem, 0, $posicaoIni);
			}
			
			
			echo "<span style='color: red;'><b>Detalhes do erro:</b><br/><br/>";
			echo simec_htmlentities($mensagem)."</span>";
			
			die;
		
		}
		
	} else {*/
		if($especie_pagamento == 'consultaPagamento'){
			$obPagamento->consultarPagamento($post, true);			
		} else if( $especie_pagamento == 'anular' ){
			$obPagamento->cancelarPagamento( $post );
		} else {
			$obPagamento->solicitaPagamento($post);
		}
		unset($obPagamento);
	//}
		
	//} // Fim ExistePTA
}

function salvarParcelaOrdemBancariaPTA( $post ){
	global $db;
	
	$registro = array();
	foreach ($post['exfid'] as $exfid => $exfvalor) {
		/*$sql = "SELECT ord.orbid FROM emenda.ordembancaria ord WHERE ord.exfid = $exfid and ord.orbvalorparcela is not null";
		$arOrbid = $db->carregarColuna( $sql );
		
		foreach ($arOrbid as $v) {
			if( !in_array( $v, $post['orbid'][$exfid] ) ){
				$sql = "DELETE FROM emenda.ordembanchistorico WHERE orbid = ".$v;			
				$db->executar( $sql );
				$sql = "DELETE FROM emenda.ordembancaria WHERE orbid = ".$v;			
				$db->executar( $sql );
			}
		}*/
		
		foreach ($exfvalor as $key => $v) {
			$numparcela	   = $post['numparcela'][$exfid][$key];
			$orbmesparcela = $post['orbmesparcela'][$exfid][$key];
			$orbanoparcela = $post['orbanoparcela'][$exfid][$key];
			$orbid		   = $post['orbid'][$exfid][$key];
			$orbvalorparcela = retiraPontos( $post['orbvalorparcela'][$exfid][$key] );
			$usucpf = $_SESSION['usucpf'];
			
			array_push( $registro, array(
									"numparcela" => $numparcela,
									"exfid" => $exfid,
									"orbmesparcela" => $orbmesparcela,
									"orbanoparcela" => $orbanoparcela,
									"orbvalorparcela" => $orbvalorparcela
									) );

			/*if( empty($orbid) ){
				$sql = "INSERT INTO emenda.ordembancaria(exfid, orbmesparcela, orbanoparcela, orbvalorparcela, spgcodigo, usucpf, orbdatainclusao) 
						VALUES ( $exfid, '$orbmesparcela', $orbanoparcela, $orbvalorparcela, '0', '$usucpf', 'now()') RETURNING orbid";
				
				$orbid = $db->pegaUm( $sql );
			} else {
				$sql = "UPDATE emenda.ordembancaria SET orbmesparcela = '$orbmesparcela', orbanoparcela = '$orbanoparcela', 
						orbvalorparcela = '$orbvalorparcela', usucpf = '$usucpf', orbdataalteracao = 'now()' WHERE orbid = $orbid";
				
				$db->executar( $sql );
			}
			
			$sql = "INSERT INTO emenda.ordembanchistorico(orbid, spgcodigo, usucpf, orbvalorpagamento, obhdataalteracao) 
					VALUES ($orbid, '0', '".$_SESSION['usucpf']."', null, 'now()')";
	  
			$db->executar( $sql );*/
		}
	}
	echo simec_json_encode( $registro );
	//echo $db->commit();
}

function salvarExecucaoPTA($post){
	global $db;
	ob_clean();
	if($post['ptrid']) {
		$sql = "UPDATE emenda.planotrabalho 
				SET ptrnumprocessoempenho='".str_replace(array("/",".","-"),array("","",""),$post['ptrnumprocessoempenho'])."' 
				WHERE ptrid='".$post['ptrid']."'";
		$db->executar($sql);
	}
	if($post['fontesiafi']) {
		foreach ($post['fontesiafi'] as $valor) {
			$arFonteSiaf = explode('|', $valor);
			if( $post['exfidexecucao'] == $arFonteSiaf[1] ){
				$sql = "UPDATE 
						  emenda.execucaofinanceira  
						SET 
						  exfcodfontesiafi = '".str_pad($arFonteSiaf[0], 10, 0, STR_PAD_LEFT)."'
						 
						WHERE 
						  exfid = ".$arFonteSiaf[1];
	
				$db->executar($sql);
			}
		}
	}
	echo $db->commit();	
}

function salvarCancelamentoConvenio( $post ){
	global $db;
	
	foreach ($post['pmcid'] as $v) {
		$arCon = explode( '_', $v );
		
		$sql = "UPDATE emenda.ptminutaconvenio SET 
				pmcjustcancelamento = '".pg_escape_string( $post['justificativa'] )."',
  				pmcdatacancelamento = now(),
  				usucpfcancelamento = '".$_SESSION['usucpf']."'
			WHERE 
  				pmcid = ".$arCon[0];

		$db->executar($sql);
	}
	echo $db->commit();
}

function verificaSeEmpenhado($ptrid = null){
	global $db;
	
	if(!$ptrid){
		return false;
	}
	
	$empenhado = $db->pegaUm("SELECT count(1) as count FROM emenda.execucaofinanceira WHERE ptrid = {$ptrid} and exfstatus = 'A' and semid = 4");
	if($empenhado){
		return false;		
	}
	
	return true;
}

?>
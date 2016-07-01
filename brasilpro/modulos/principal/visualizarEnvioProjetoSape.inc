<?
include APPRAIZ . "includes/classes/dateTime.inc";
//ver($projetos);
/************************* CABEÇALHO E TÍTULO ******************************/

$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( 'Visualização', '&nbsp;' );

?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
</head>
	<body>
		<form name="formulario" id="formulario" action="" method="post">
			<input type=hidden name="visualizaBrasilPro" id="visualizaBrasilPro" value="false" />
			<input type=hidden name="salvar" value="false" />
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td class="subtitulocentro" colspan="2">Dados da visualização</td>
				</tr>
				<tr>
					<td class="subtitulodireita" width="190px;">Código IBGE</td>
					<td>
						<?php echo $projetos['codigoibge']; ?>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita" width="190px;">UF</td>
					<td style="background-color: #F0F0F0;">
						<?php echo $projetos['uf']; ?>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Parecer</td>
					<td>
						<?php echo $projetos['parecer']; ?>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">CPF Parecista</td>
					<td style="background-color: #F0F0F0;">
						<?php echo formatar_cpf( $projetos['cpfparecista'] ); ?>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Nome Parecista</td>
					<td>
						<?php echo $projetos['nomeparecista']; ?>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Projetos</td>
					<td style="background-color: #F0F0F0;">
						<?php foreach( $projetos['projetos'] as $projeto ){ 
							if( $projeto <> 'projeto' ){ 
							?>
						<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
							<tr>
								<?php $titulo = $projeto['numeroprocesso'] <> '' ? 'Reformulação do Processo nº: '.$projeto['numeroprocesso'] : 'Novo Convênio'; ?>
								<td class="subtitulocentro" style="background-color: #DCDCDC;" colspan="2"><?php echo $titulo; ?></td>
							</tr>
							<tr>
								<td class="subtitulodireita">Tipo de Projeto</td>
								<td>	
									<?php
										echo $projeto['tipoprojeto']['descricao'];
									?>
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Ano de inicio da execução</td>
								<td style="background-color: #F0F0F0;">	
									<?php
										echo $projeto['anoinicioexecucao'];
									?>
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Ano fim da execução</td>
								<td>	
									<?php
										echo $projeto['anofimexecucao'];
									?>
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Nº do Processo</td>
								<td>	
									<?php
										echo $projeto['numeroprocesso'];
									?>
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Ações</td>
								<td>	
							<?php
								foreach( $projeto['acoes'] as $k => $acao ){
								
								//next( $projeto['acoes'] );
								if( $k <> 'NOME_FILHO'){
									$teste = $k;
							?>
							<table class="tabela" width="90%" align="center" border="0" cellspacing="0" cellpadding="2">
							<tr>
								<td class="subtitulodireita">Dimensão</td>
								<td style="background-color: #F0F0F0;">		
									<?php
										echo $projeto['acoes'][$teste]['descricaodimensao'];
									?>
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Cronograma</td>
								<td align="left">
									<table width="50%" align="center" border="1" cellspacing="0" cellpadding="2">
										<tr colspan='100%'>
								  			<td><strong>Ano</strong></td>
								  			<td><strong>Valor</strong></td>
										</tr>
										<?php
											foreach($projeto['acoes'][$teste]['cronograma'] as $cronograma){
												if( $cronograma <> 'periodo' ){ ?>
													<tr bgcolor="#f5f5f5">
														
											  			<td align="right"> <?php echo $cronograma['ano']; ?> </td>
											  			<td align="right"> R$ <?php echo  number_format($cronograma['valor'] , 2, ',', '.'); ?> </td>
													</tr>
										<?php
												}
											}
										?>
								  	</table>
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Detalhamento</td>
								<td style="background-color: #F0F0F0;">	
									<?php
										echo $projeto['acoes'][$teste]['detalhamento'];
									?>
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Valor concedente</td>
								<td>
									R$ <?php echo  number_format($projeto['acoes'][$teste]['valorconcedente'] , 2, ',', '.'); ?>	
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Valor Proponente</td>
								<td style="background-color: #F0F0F0;">		
									R$ <?php echo  number_format($projeto['acoes'][$teste]['valorproponente'] , 2, ',', '.'); ?>	
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Cronograma execução inicial</td>
								<td>	
									<?php
										$data = new Data();
	 									$a = $data->formataData($projeto['acoes'][$teste]['cronogramaexecucaoinicial'], "dd/mm/yyyy");
										echo $a;
									?>
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Cronograma execução final</td>
								<td style="background-color: #F0F0F0;">	
									<?php
										$data = new Data();
	 									$a = $data->formataData($projeto['acoes'][$teste]['cronogramaexecucaofinal'], "dd/mm/yyyy");
										echo $a;
									?>
								</td>
							</tr>
							<?php
								$tag = '';
								if($projeto['acoes'][$teste]['escolas']) {
									
							?>
							<tr>
								<td class="subtitulodireita">Escolas</td>
								<td align="left">
									<table width="50%" align="center" border="1" cellspacing="0" cellpadding="2">
									<table width="50%" align="center" border="1" cellspacing="0" cellpadding="2">
										<tr colspan='100%'>
								  			<td><strong>Código INEP</strong></td>
								  			<td><strong>Nome da Escola</strong></td>
								  			<td><strong>Quantidade de Alunos</strong></td>
										</tr>
										<?php
											next( $projeto['acoes'][$teste]['escolas'] );
											foreach( $projeto['acoes'][$teste]['escolas'] as $teste2 ){
												if( $teste2 <> 'escola' ){ ?>
													<tr bgcolor="#f5f5f5">
											  			<td align="right"> <?php echo $teste2['codigoinep']; ?> </td>
											  			<td align="right"> <?php echo $teste2['nomeescola']; ?> </td>
											  			<td align="right"> <?php echo $teste2['quantidadealunos']; ?> </td>
													</tr>
										<?php
												}
											}
										?>
								  	</table>
								</td>
							</tr>
							<?php
									$tag = $tag == '' ? 'style="background-color: #F0F0F0;"' : '';
								}
								if($projeto['acoes'][$teste]['beneficiarios']) {
							?>
							<tr>
								<td class="subtitulodireita">Beneficiários</td>
								<td <?php echo $tag; ?>>	
									<table width="50%" align="center" border="1" cellspacing="0" cellpadding="2">
										<tr colspan='100%'>
								  			<td><strong>Código Beneficiário</strong></td>
								  			<td><strong>Nome do Beneficiário</strong></td>
								  			<td><strong>Quantidade Zona Rural</strong></td>
								  			<td><strong>Quantidade Zona Urbana</strong></td>
										</tr>
										<?php
											foreach( $projeto['acoes'][$teste]['beneficiarios'] as $teste2 ){
												if( $teste2 <> 'escola' ){ ?>
													<tr bgcolor="#f5f5f5">
											  			<td align="right"> <?php echo $teste2['codigobeneficiario']; ?> </td>
											  			<td align="right"> <?php echo $teste2['descricaobeneficiario']; ?> </td>
											  			<td align="right"> <?php echo $teste2['quantidadezonarural']; ?> </td>
											  			<td align="right"> <?php echo $teste2['quantidadezonaurbana']; ?> </td>
													</tr>
										<?php
												}
											}
										?>
								  	</table>
								</td>
							</tr>
							<?php
									$tag = $tag == '' ? 'style="background-color: #F0F0F0;"' : '';
								}
								//ver($projeto['acoes']);
								if($projeto['acoes'][$teste]['especificacoes']) {
							?>
							<tr>
								<td class="subtitulodireita">Especificações</td>
								<td <?php echo $tag; ?>>
										<?php	
										foreach( $projeto['acoes'][$teste]['especificacoes'] as $teste2 ){
											//ver($teste2);
											if( $teste2 <> 'especificacao' ){ ?>
											<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
												<tr>
													<td class="subtitulodireita">Código Subação</td>
													<td>	
														<?php
															echo $teste2['codigosubacao'];
														?>
													</td>
												</tr>
												<tr>
													<td class="subtitulodireita">Descrição Subação</td>
													<td style="background-color: #F0F0F0;">	
														<?php
															echo $teste2['descricaosubacao'];
														?>
													</td>
												</tr>
												<tr>
													<td class="subtitulodireita">Comentário dos Itens</td>
													<td>	
														<?php
															echo $teste2['comentarioitens'];
														?>
													</td>
												</tr>
												<tr>
													<td class="subtitulodireita">Código da Unidade de Medida</td>
													<td style="background-color: #F0F0F0;">	
														<?php
															echo $teste2['codigounidademedida'];
														?>
													</td>
												</tr>
												<tr>
													<td class="subtitulodireita">Descrição da Unidade de Medida</td>
													<td>	
														<?php
															echo $teste2['descricaounidademedida'];
														?>
													</td>
												</tr>
												<tr>
													<td class="subtitulodireita">Quantidade</td>
													<td style="background-color: #F0F0F0;">	
														<?php
															echo $teste2['quantidade'];
														?>
													</td>
												</tr>
												<tr>
													<td class="subtitulodireita">Valor Unitário</td>
													<td>
														R$ <?php echo  number_format($teste2['valorunitariosubacao'] , 2, ',', '.'); ?>
													</td>
												</tr>
												<tr>
													<td class="subtitulodireita">Categoria da Despesa</td>
													<td style="background-color: #F0F0F0;">	
														<?php
															echo $teste2['categoriadespesa'];
														?>
													</td>
												</tr>
												<tr>
													<td class="subtitulodireita">Itens</td>
													<td>
												<?php foreach( $teste2['itens'] as $item ){ 
													if( $item <> 'item' ){ ?>
														<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
														<tr>
															<td class="subtitulodireita">Descrição do Item</td>
																<td>	
																	<?php
																		echo $item['descricaoitem'];
																	?>
																</td>
															</tr>
															<tr>
																<td class="subtitulodireita">Código Unidade de Medida</td>
																<td style="background-color: #F0F0F0;">	
																	<?php
																		echo $item['codigounidademedida'];
																	?>
																</td>
															</tr>
															<tr>
																<td class="subtitulodireita">Descrição da Unidade de Medida</td>
																<td>	
																	<?php
																		echo $item['descricaounidademedida'];
																	?>
																</td>
															</tr>
															<tr>
																<td class="subtitulodireita">Quantidade</td>
																<td style="background-color: #F0F0F0;">		
																	<?php
																		echo $item['quantidade'];
																	?>
																</td>
															</tr>
															<tr>
																<td class="subtitulodireita">Valor Unitário</td>
																<td>
																	R$ <?php echo  number_format($item['valorunitario'] , 2, ',', '.'); ?>
																</td>
															</tr>
														</table><br>
														<? } } ?>
														</td>
														</tr>
										  	</table><br>
										<?php
											}
										}
									?>
								</td>
							</tr><br>
							<?php
								} ?>
							</table>
							<?php
								} }
							?>
								</td>
							</tr>
						</table>
						<br>
						<?php } } ?>
					</td>
				</tr>
				<tr>
					<td class="subtitulocentro" style="background-color: #DCDCDC;" colspan="2">
						<input type="button" name="voltar" value="Voltar"  onclick="history.back(-1)" />
					</td>
				</tr>
			</table>
		</form>
		<script language="javascript">
		</script>
	</body>
</html>


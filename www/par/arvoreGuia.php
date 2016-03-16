<?php
include_once "config.inc";
include_once "_constantes.php";
include_once '_funcoes.php';
include_once '_funcoesPar.php';
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/classes/dateTime.inc";
require_once APPRAIZ . "includes/classes/Controle.class.inc";
require_once APPRAIZ . "includes/classes/Visao.class.inc";
require_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once 'autoload.php';
// atualiza ação do usuário no sistema
include APPRAIZ . "includes/registraracesso.php";

$oConfiguracaoControle = new ConfiguracaoControle();

if ($_REQUEST['root'] == "source"):
?>
[
	<?php
	$arInstrumentos = $oConfiguracaoControle->recuperarIntrumentosGuia();
	if($arInstrumentos):
	$indiceIntrumento = 1;
	$countIntrumento  = count($arInstrumentos);
	?>
		<?php foreach ($arInstrumentos as $instrumento): ?>		
		{
			"text": "<a href=\"javascript:abrirPopupGuia('incluir', 'dimensao', '<?=$instrumento['itrid'];?>')\" cokkieGuia=\"guiadimensao<?php echo $instrumento['itrid'];?>\" ><img border=\"0\" class=\"imguia\" src=\"../imagens/gif_inclui.gif\" align=\"absmiddle\" title=\"Incluir dimesão\"></a>&nbsp;<?php echo '<span title=\"Instrumento: '.$instrumento['itrdsc'].'\">'.delimitador($instrumento['itrdsc'])."</span>"; ?>",
			"children":
			[
				<?php
				$arDimensoes = $oConfiguracaoControle->recuperarDimensoesGuia($instrumento['itrid']);
				if($arDimensoes):
				$indiceDimensao = 1;
				$countDimensao  = count($arDimensoes);
				?>
					<?php foreach ($arDimensoes as $dimensao): ?>				
					{
						"text": "<a href=\"javascript:abrirPopupGuia('incluir', 'area', '<?=$dimensao['dimid'];?>')\" cokkieGuia=\"guiaarea<?php echo $dimensao['dimid'];?>\" ><img border=\"0\" class=\"imguia\" src=\"../imagens/gif_inclui.gif\" align=\"absmiddle\" title=\"Incluir área\" /></a><img border=\"0\" class=\"imguia\" src=\"../imagens/alterar.gif\" onclick=\"abrirPopupGuia('editar', 'dimensao', '<?=$dimensao['dimid'];?>')\" align=\"absmiddle\" style=\"cursor:pointer;\" title=\"Alterar dimensão\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/excluir.gif\" onclick=\"excluirItemGuia('dimensao', '<?=$dimensao['dimid'];?>')\" align=\"absmiddle\" style=\"cursor:pointer;\" title=\"Excluir dimensão\" />&nbsp;<?php echo $dimensao['dimcod']." - <span title='Dimensão: {$dimensao['dimdsc']}'>".delimitador($dimensao['dimdsc'])."</span>"; ?>",
						"children": 
						[
							<?php
							$arAreas = $oConfiguracaoControle->recuperarAreasGuia($dimensao['dimid']);
							if($arAreas):
							$indiceArea = 1;
							$countArea  = count($arAreas);
							?>
								<?php foreach ($arAreas as $area): ?>
								{
									"text": "<a href=\"javascript:abrirPopupGuia('incluir', 'indicador', '<?=$area['areid'];?>')\" cokkieGuia=\"guiaindicador<?php echo $area['areid'];?>\" ><img border=\"0\" class=\"imguia\" src=\"../imagens/gif_inclui.gif\" align=\"absmiddle\" title=\"Incluir indicador\" /></a><img border=\"0\" class=\"imguia\" src=\"../imagens/alterar.gif\" onclick=\"abrirPopupGuia('editar', 'area', '<?=$area['areid'];?>')\" align=\"absmiddle\" style=\"cursor:pointer;\" title=\"Alterar área\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/excluir.gif\" onclick=\"excluirItemGuia('area', '<?=$area['areid'];?>')\" align=\"absmiddle\" style=\"cursor:pointer;\" title=\"Excluir área\" />&nbsp;<?php echo $dimensao['dimcod'].".".$area['arecod']." - <span title='Área: {$area['aredsc']}'>".delimitador($area['aredsc'])."</span>"; ?>",
									"children":
									[
										<?php
										$arIndicadores = $oConfiguracaoControle->recuperarIndicadoresGuia($area['areid']);
										if($arIndicadores):
										$indiceIndicador = 1;
										$countIndicador  = count($arIndicadores);
										?>
											<?php foreach ($arIndicadores as $indicador): ?>
											{
												"text": "<a href=\"javascript:abrirPopupGuia('incluir', 'criterio', '<?=$indicador['indid'];?>')\" cokkieGuia=\"guiacriterio<?php echo $indicador['indid'];?>\" ><img border=\"0\" class=\"imguia\" src=\"../imagens/gif_inclui.gif\" align=\"absmiddle\" title=\"Incluir critério\" /></a><img border=\"0\" class=\"imguia\" src=\"../imagens/gif_inclui.gif\" onclick=\"abrirPopupGuiaSubacao('incluir', '<?=$indicador['indid'];?>')\" align=\"absmiddle\" style=\"cursor:pointer;\" title=\"Incluir subação\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/alterar.gif\" onclick=\"abrirPopupGuia('editar', 'indicador', '<?=$indicador['indid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Alterar indicador\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/excluir.gif\" onclick=\"excluirItemGuia('indicador', '<?=$indicador['indid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Excluir indicador\" />&nbsp;<?php echo $dimensao['dimcod'].".".$area['arecod'].".".$indicador['indcod']." - <span title='Indicador: {$indicador['inddsc']}'>".delimitador($indicador['inddsc'])."</span>"; ?>",
												"id": "<?php echo $instrumento['itrid']."%".$dimensao['dimcod']."%".$area['arecod']."%".$indicador['indcod']."%".$indicador['indid']."-".$_GET['filtro'] ?>",						
												"hasChildren": true
											}
											<?php
											echo ($indiceIndicador < $countIndicador) ? ',' : '';  
											$indiceIndicador++;
											?>												
											<?php endforeach; ?>										
										<?php endif; ?>										
									]
								}
								<?php
								echo ($indiceArea < $countArea) ? ',' : '';  
								$indiceArea++;
								?>
								<?php endforeach; ?>								
							<?php endif; ?>							
						]
					}
					<?php
					echo ($indiceDimensao < $countDimensao) ? ',' : '';  
					$indiceDimenssao++;
					?>
					<?php endforeach; ?>
				<?php endif; ?>			 	
			]
		}
		<?php
		echo ($indiceIntrumento < $countIntrumento) ? ',' : '';  
		$indiceIntrumento++;
		?>
		<?php endforeach; ?>
	<?php endif; ?>	
]
<?php else: ?>
[
	<?php	
	$separa = explode("-", $_REQUEST['root']);
	$arParams = explode("%", $separa[0]);
	$arFiltros = explode("%", $separa[1]);
	?>
	
	<?php 
	/**********************
	 * TODAS
	 */
	?>
	<?php																	
	$arCriteriosAcoes = $oConfiguracaoControle->recuperarCriteriosAcoesGuia($arParams[4]);
	$arCriteriosAcoes = ($arCriteriosAcoes) ? $arCriteriosAcoes : array();																	
	if($arCriteriosAcoes && (in_array('1', $arFiltros) && in_array('2', $arFiltros)) || !in_array('true', $arFiltros)): // Critério
	$indiceCriterioAcoes = 1;
	$countCriterioAcoes  = count($arCriteriosAcoes);
	?>
		<?php foreach ($arCriteriosAcoes as $criterioAcao): ?>		
			{
				"text": "<img border=\"0\" class=\"imguia\" src=\"../imagens/gif_inclui.gif\" onclick=\"abrirPopupGuia('incluir', 'acao', '<?=$criterioAcao['crtid'];?>')\" align=\"absmiddle\" style=\"cursor:pointer;\" title=\"Incluir ação\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/alterar.gif\" onclick=\"abrirPopupGuia('editar', 'criterio', '<?=$criterioAcao['crtid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Alterar critério\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/excluir.gif\" onclick=\"excluirItemGuia('criterio', '<?=$criterioAcao['crtid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Excluir indicador\" />&nbsp;<?php echo "(".$criterioAcao['crtpontuacao'].") ".delimitador($criterioAcao['crtdsc']); ?>",
				"children":
				[
					<?php																	
					$arAcoesCriterio = $oConfiguracaoControle->recuperarPropostaAcoesCriterioGuia($criterioAcao['crtid']);
					$arAcoesCriterio = ($arAcoesCriterio) ? $arAcoesCriterio : array();																																					
					if($arAcoesCriterio && (in_array('2', $arFiltros) || !in_array('true', $arFiltros))): // Critério
					$indiceAcaoCriterio = 1;
					$countAcaoCriterio  = count($arAcoesCriterio);
					?>
						<?php foreach ($arAcoesCriterio as $acaoCriterio): ?>
							{
								"text": "<img border=\"0\" class=\"imguia\" src=\"../imagens/alterar.gif\" onclick=\"abrirPopupGuia('editar', 'acao', '<?=$acaoCriterio['ppaid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Alterar ação\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/excluir.gif\" onclick=\"excluirItemGuia('acao', '<?=$acaoCriterio['ppaid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Excluir indicador\" />&nbsp;<?php echo $acaoCriterio['ppadsc'] ?>",
								"children":
								[
									<?php 
									$arSubacoesCriterio = $oConfiguracaoControle->recuperarSubacoesPorCriterio($acaoCriterio['crtid']);
									$arSubacoesCriterio = ($arSubacoesCriterio) ? $arSubacoesCriterio : array();
									if($arSubacoesCriterio && (in_array('3', $arFiltros) || !in_array('true', $arFiltros))): // Critério
									$indiceSubacoesCriterio = 1;
									$countSubacoesCriterio  = count($arSubacoesCriterio);
									?>
										<?php foreach($arSubacoesCriterio as $subacoescriterio): ?>
											{
												"text": "<img border=\"0\" class=\"imguia\" src=\"../imagens/alterar.gif\" onclick=\"abrirPopupGuiaSubacao('editar', '<?=$subacoescriterio['ppsid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Alterar ação\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/excluir.gif\" onclick=\"excluirItemGuia('acao', '<?=$subacoescriterio['ppsid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Excluir indicador\" />&nbsp;<?php echo $subacoescriterio['ppsordem']." - ".$subacoescriterio['ppsdsc'] ?>"
											}
											<?php
											echo ($indiceSubacoesCriterio < $countSubacoesCriterio) ? ',' : '';  
											$indiceSubacoesCriterio++;
											?>
										<?php endforeach; ?>
									<?php endif; ?>
								]
							}
							<?php
							echo ($indiceAcaoCriterio < $countAcaoCriterio) ? ',' : '';  
							$indiceAcaoCriterio++;
							?>
						<?php endforeach; ?>
					<?php endif; ?>					
				]				
			}
			<?php
			echo ($indiceCriterioAcoes <= $countCriterioAcoes) ? ',' : '';  
			$indiceCriterioAcoes++;
			?>
		<?php endforeach; ?>
	<?php endif; ?>
	
	<?php 
	/**********************
	 * CRITÉRIO E SUBAÇÕES
	 */
	?>	
	<?php 
	if($arCriteriosAcoes && (in_array('1', $arFiltros) && in_array('3', $arFiltros) && !in_array('2', $arFiltros)) ): // Critério
	$indiceCriterioAcoes = 1;
	$countCriterioAcoes  = count($arCriteriosAcoes);
	?>
		<?php foreach ($arCriteriosAcoes as $criterioAcao): ?>		
			{
				"text": "<img border=\"0\" class=\"imguia\" src=\"../imagens/gif_inclui.gif\" onclick=\"abrirPopupGuia('incluir', 'acao', '<?=$criterioAcao['crtid'];?>')\" align=\"absmiddle\" style=\"cursor:pointer;\" title=\"Incluir ação\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/alterar.gif\" onclick=\"abrirPopupGuia('editar', 'criterio', '<?=$criterioAcao['crtid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Alterar critério\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/excluir.gif\" onclick=\"excluirItemGuia('criterio', '<?=$criterioAcao['crtid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Excluir indicador\" />&nbsp;<?php echo $arParams[0].".".$arParams[1].".".$arParams[2].".".$arParams[3].".".$criterioAcao['crtpontuacao']." - "."(".$criterioAcao['crtpontuacao'].") ".delimitador($criterioAcao['crtdsc']); ?>",
				"children":
				[
					<?php 
					$arSubacoesCriterio = $oConfiguracaoControle->recuperarSubacoesPorCriterio($criterioAcao['crtid']);
					if($arSubacoesCriterio && (in_array('3', $arFiltros) || !in_array('true', $arFiltros))): // Critério
					$indiceSubacoesCriterio = 1;
					$countSubacoesCriterio  = count($arSubacoesCriterio);
					?>
						<?php foreach($arSubacoesCriterio as $subacoescriterio): ?>
							{
								"text": "<img border=\"0\" class=\"imguia\" src=\"../imagens/alterar.gif\" onclick=\"abrirPopupGuiaSubacao('editar', '<?=$subacoescriterio['ppsid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Alterar ação\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/excluir.gif\" onclick=\"excluirItemGuia('acao', '<?=$subacoescriterio['ppsid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Excluir indicador\" />&nbsp;<?php echo $arParams[0].".".$arParams[1].".".$arParams[2].".".$arParams[3].".".$criterioAcao['crtpontuacao'].".".$subacoescriterio['ppsid']." - ".$subacoescriterio['ppsdsc'] ?>"
							}
							<?php
							echo ($indiceSubacoesCriterio < $countSubacoesCriterio) ? ',' : '';  
							$indiceSubacoesCriterio++;
							?>
						<?php endforeach; ?>
					<?php endif; ?>
				]				
			}
			<?php
			echo ($indiceCriterioAcoes <= $countCriterioAcoes) ? ',' : '';  
			$indiceCriterioAcoes++;
			?>
		<?php endforeach; ?>
	<?php endif; ?>
	
	<?php 
	/**********************
	 * AÇÕES E SUBAÇÕES
	 */
	?>
	<?php																	
	$arAcaoSubacoes = $oConfiguracaoControle->recuperarCriterioSubacoesPorIndicador($arParams[4]);																																					
	if($arAcaoSubacoes && (in_array('2', $arFiltros) && in_array('3', $arFiltros) && !in_array('1', $arFiltros))):
	$indiceAcaoSubacoes = 1;
	$countAcaoSubacoes  = count($arAcaoSubacoes);
	?>
		<?php foreach ($arAcaoSubacoes as $acaoCriterio): ?>
			{
				"text": "<img border=\"0\" class=\"imguia\" src=\"../imagens/alterar.gif\" onclick=\"abrirPopupGuia('editar', 'acao', '<?=$acaoCriterio['ppaid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Alterar ação\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/excluir.gif\" onclick=\"excluirItemGuia('acao', '<?=$acaoCriterio['ppaid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Excluir indicador\" />&nbsp;<?php echo $arParams[0].".".$arParams[1].".".$arParams[2].".".$arParams[3].".".$acaoCriterio['crtpontuacao'].".".$acaoCriterio['ppaid']." - ".$acaoCriterio['ppadsc'] ?>",
				"children":
				[
					<?php 
					$arSubacoesAcao = $oConfiguracaoControle->recuperarSubacoesPorCriterio($acaoCriterio['crtid']);
					if($arSubacoesAcao): // Critério
					$indiceSubacoesAcao = 1;
					$countSubacoesAcao  = count($arSubacoesAcao);
					?>
						<?php foreach($arSubacoesAcao as $acaoSubacao): ?>
							{
								"text": "<img border=\"0\" class=\"imguia\" src=\"../imagens/alterar.gif\" onclick=\"abrirPopupGuiaSubacao('editar', '<?=$acaoSubacao['ppsid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Alterar ação\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/excluir.gif\" onclick=\"excluirItemGuia('acao', '<?=$acaoSubacao['ppsid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Excluir indicador\" />&nbsp;<?php echo $arParams[0].".".$arParams[1].".".$arParams[2].".".$arParams[3].".".$acaoCriterio['crtpontuacao'].".".$acaoSubacao['ppsid']." - ".$acaoSubacao['ppsdsc'] ?>"
							}
							<?php
							echo ($indiceSubacoesAcao < $countSubacoesAcao) ? ',' : '';  
							$indiceSubacoesAcao++;
							?>
						<?php endforeach; ?>
					<?php endif; ?>
				]
			}
			<?php
			echo ($indiceAcaoSubacoes < $countAcaoSubacoes) ? ',' : '';  
			$indiceAcaoSubacoes++;
			?>
		<?php endforeach; ?>
	<?php endif; ?>
	
	<?php 
	/**********************
	 * SOMENTE CRITÉRIOS
	 */
	?>		
	<?php	
	if($arCriteriosAcoes && (in_array('1', $arFiltros) && !in_array('2', $arFiltros) && !in_array('3', $arFiltros))): // Critério
	$indiceCriterioAcoes = 1;
	$countCriterioAcoes  = count($arCriteriosAcoes);
	?>
		<?php foreach ($arCriteriosAcoes as $criterioAcao): ?>		
			{
				"text": "<img border=\"0\" class=\"imguia\" src=\"../imagens/gif_inclui.gif\" onclick=\"abrirPopupGuia('incluir', 'acao', '<?=$criterioAcao['crtid'];?>')\" align=\"absmiddle\" style=\"cursor:pointer;\" title=\"Incluir ação\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/alterar.gif\" onclick=\"abrirPopupGuia('editar', 'criterio', '<?=$criterioAcao['crtid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Alterar critério\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/excluir.gif\" onclick=\"excluirItemGuia('criterio', '<?=$criterioAcao['crtid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Excluir indicador\" />&nbsp;<?php echo $arParams[0].".".$arParams[1].".".$arParams[2].".".$arParams[3].".".$criterioAcao['crtpontuacao']." - "."(".$criterioAcao['crtpontuacao'].") ".delimitador($criterioAcao['crtdsc']); ?>"
			}
			<?php
			echo ($indiceCriterioAcoes <= $countCriterioAcoes) ? ',' : '';  
			$indiceCriterioAcoes++;
			?>
		<?php endforeach; ?>
	<?php endif; ?>	
	
	<?php 
	/**********************
	 * SOMENTE AÇÕES
	 */
	?>		
	<?php
	$arAcoes = $oConfiguracaoControle->recuperarPropostaAcoesGuia($arParams[4]);
	if($arAcoes && (in_array('2', $arFiltros) && !in_array('1', $arFiltros) && !in_array('3', $arFiltros))): // Critério
	$indiceAcao = 1;
	$countAcao  = count($arAcoes);
	?>		
		<?php foreach ($arAcoes as $acao): ?>
			{
				"text": "<img border=\"0\" class=\"imguia\" src=\"../imagens/alterar.gif\" onclick=\"abrirPopupGuiaSubacao('editar', '<?=$acao['ppaid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Alterar ação\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/excluir.gif\" onclick=\"excluirItemGuia('acao', '<?=$acao['ppaid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Excluir indicador\" />&nbsp;<?php echo $arParams[0].".".$arParams[1].".".$arParams[2].".".$arParams[3].".".$acao['ppaid']." - ".$acao['ppadsc'] ?>"
			}
			<?php
			echo ($indiceAcao < $countAcao) ? ',' : '';  
			$indiceAcao++;
			?>
		<?php endforeach; ?>
	<?php endif; ?>		
	
	<?php 
	/**********************
	 * SOMENTE SUBAÇÕES
	 */
	?>		
	<?php																	
	$arSubacoes = $oConfiguracaoControle->recuperarSubacaoGuia($arParams[4]);																	
	if($arSubacoes && (in_array('3', $arFiltros) && !in_array('2', $arFiltros) && !in_array('1', $arFiltros))): // Critério
	$indiceSubacao = 1;
	$countSubacao  = count($arSubacoes);
	?>
		<?php foreach ($arSubacoes as $subacao): ?>
			{
				"text": "<img border=\"0\" class=\"imguia\" src=\"../imagens/alterar.gif\" onclick=\"abrirPopupGuiaSubacao('editar', '<?=$subacao['ppsid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Alterar ação\" /><img border=\"0\" class=\"imguia\" src=\"../imagens/excluir.gif\" onclick=\"excluirItemGuia('acao', '<?=$subacao['ppsid'];?>')\" style=\"cursor:pointer;\" align=\"absmiddle\" title=\"Excluir indicador\" />&nbsp;<?php echo $arParams[0].".".$arParams[1].".".$arParams[2].".".$arParams[3].".".$criterio['crtpontuacao'].".".$subacao['ppsid']." - ".$subacao['ppsdsc'] ?>"
			}
			<?php
			echo ($indiceSubacao < $countSubacao) ? ',' : '';  
			$indiceSubacao++;
			?>
		<?php endforeach; ?>
	<?php endif; ?>	
]
<?php endif; ?>
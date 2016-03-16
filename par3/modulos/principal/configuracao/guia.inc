<?php

	require_once APPRAIZ . 'includes/cabecalho.inc';

	switch ($_POST['tipo']){
	    case 'dimensao':
	        $oDimensao = new Par3_Model_Dimensao;
            $oDimensao->preparaSalvar();
            simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A', 'success');
    		break;
	    case 'area':
	        $oArea = new Par3_Model_Area;
            $oArea->preparaSalvar();
            simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A', 'success');
    		break;

	    case 'indicador':
	        $oIndicadorController = new Par3_Controller_Indicador;
            $oIndicadorController->salvarIndicadorGuia();
            simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A', 'success');
    		break;

	    case 'criterio':
	        $oCriterio = new Par3_Model_Criterio;
            $oCriterio->preparaSalvar();
            simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A', 'success');
    		break;
	}

	if($_GET['acaoGuia'] == 'excluir'){
	    switch ($_GET['tipoGuia']){
	        case 'dimensao':
                $oDimensao = new Par3_Model_Dimensao();
                $oDimensao->deletarDimensaoGuia($_GET['codigo']);
                simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A', 'success');
                break;

	        case 'area':
	            $oArea = new Par3_Model_Area();
	            $oArea->deletarAreaGuia($_GET['codigo']);
	            simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A', 'success');
	            break;

	        case 'indicador':
	            $oIndicador = new Par3_Model_Indicador();
	            $oIndicador->deletarIndicadorGuia($_GET['codigo']);
	            simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A', 'success');
	            break;

	        case 'criterio':
	            $oCriterioController = new Par3_Controller_Criterio();
	            $sucesso = $oCriterioController->deletarCriterioGuia($_GET['codigo']);
	            if( $sucesso ){
	               simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A', 'success');
	            } else {
	               simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A', 'error', 'O critério já está vinculado a outro critério. Remova a vinculação e tente novamente.');
	            }
	            break;

	    }
	}

	if($_GET['acaoGuia'] == 'reordenar'){
	    switch ($_GET['tipo']){

	        case 'criterio':
	            $oCriterio = new Par3_Controller_Criterio();
	            $oCriterio->reordenarItens($_GET['id'], $_GET['direcao']);

        		echo "<script>
                         	window.location.href = 'par3.php?modulo=principal/configuracao/guia&acao=A';
        				</script>";
	            break;
	    }
	}

	if($_POST['vinculaIndicador'] == 'salvar'){
	    $oIndicador = new Par3_Controller_DimensaoPainel();
	    $oIndicador->vincularIndicadoresPainelGuia($_POST['dimid'], $_POST['indices']);
	    break;
	}

	if($_POST['vinculaCriterio'] == 'salvar'){
	    $oCriterioVinculacao = new Par3_Controller_CriterioVinculacao();
	    $oCriterioVinculacao->vincularCriteriosGuia($_POST);
	    break;
	}

	if($_POST['filtro']){
		$params = implode("%",$_POST['filtro']);
	} else {
		$params = '0';
	}

	$arInstrumentos = Par3_Controller_ConfiguracaoControle::recuperarIntrumentosGuia();

echo $simec->title("Guia");

?>

<div class="wrapper wrapper-content animated fadeIn">
	<div class="row">
		<div class="ibox float-e-margins">
			<div class="ibox-title">
				<h5>Guia de Ações Padronizadas <br /><small><a href=# onclick="$('.tree').jstree('close_all')">Fechar Todos</a> | <a href=# onclick="$('.tree').jstree('open_all')">Abrir Todos</a></small></h5>
					<a class="collapse-link">
				<div class="ibox-tools">
						<i class="fa fa-chevron-up"></i>
					</a>
				</div>
			</div>
			<div class="ibox-content tree">
				<ul>
					<?php foreach ($arInstrumentos as $instrumento): ?>

					<li data-jstree='{"icon":false}'>
						<label>&nbsp;<?php echo $instrumento['itrdsc'];?></label>

						<div id="modalGuia">
							<a data-toggle="tooltip" data-original-title="Incluir dimensão" onclick="popupGuia('incluir', 'dimensao', '<?=$instrumento['itrid'];?>');">
								<i class="fa fa-plus-circle"></i>
							</a>
						</div>

						<?php $arDimensoes = Par3_Controller_ConfiguracaoControle::recuperarDimensoesGuia($instrumento['itrid']); ?>
						<?php if($arDimensoes): ?>
						<?php foreach ($arDimensoes as $dimensao): ?>
						<ul>
							<li data-jstree='{"icon":false}'>
								<label>&nbsp;<?php echo $dimensao['dimcod'].' - '.$dimensao['dimdsc'];?></label>
								<div>
									<a data-toggle="tooltip" data-original-title="Incluir área" onclick="popupGuia('incluir', 'area', '<?=$dimensao['dimid'];?>')">
										<i class="fa fa-plus-circle"></i>
									</a>
									<a data-toggle="tooltip" data-original-title="Alterar dimensão" onclick="popupGuia('editar', 'dimensao', '<?=$dimensao['dimid'];?>')">
										<i class="fa fa-edit"></i>
									</a>
									<a data-toggle="tooltip" data-original-title="Vincular indicadores" onclick="popupGuiaIndicador('<?php echo $dimensao['dimid']; ?>', '<?php echo $dimensao['dimdsc']; ?>')">
										<i class="fa fa-chain"></i>
									</a>
									<a href="#" data-toggle="tooltip" data-original-title="Excluir dimensão">
										<i class="fa fa-trash-o" onclick="excluirItemGuia('dimensao', '<?=$dimensao['dimid'];?>')"></i>
									</a>
								</div>

								<?php $arAreas = Par3_Controller_ConfiguracaoControle::recuperarAreasGuia($dimensao['dimid']); ?>
								<?php if($arAreas): ?>
								<?php foreach ($arAreas as $area): ?>
								<ul>
									<li data-jstree='{"icon":false}'>
										<label>&nbsp;<?php echo $dimensao['dimcod'].".".$area['arecod'].' - '.$area['aredsc'];?></label>
										<div>
											<a data-toggle="tooltip" data-original-title="Incluir indicador" onclick="popupGuia('incluir', 'indicador', '<?=$area['areid'];?>')">
												<i class="fa fa-plus-circle"></i>
											</a>
											<a data-toggle="tooltip" data-original-title="Alterar área" onclick="popupGuia('editar', 'area', '<?=$area['areid']; ?>')">
												<i class="fa fa-edit"></i>
											</a>
											<a href="#" data-toggle="tooltip" data-original-title="Excluir área">
												<i class="fa fa-trash-o" onclick="excluirItemGuia('area', '<?=$area['areid']; ?>')"></i>
											</a>
										</div>


										<?php $arIndicadores = Par3_Controller_ConfiguracaoControle::recuperarIndicadoresGuia($area['areid']); ?>
										<?php if($arIndicadores): ?>
										<?php foreach ($arIndicadores as $indicador): ?>
										<ul>
											<li data-jstree='{"icon":false}'>
												<label>&nbsp;<?php echo $indicador[$indicador['indid']] = $dimensao['dimcod'].".".$area['arecod'].".".$indicador['indcod']." - ".$indicador['inddsc']; ?></label>
												<div>
													<a data-toggle="tooltip" data-original-title="Incluir Componente" onclick="popupGuia('incluir', 'criterio', '<?=$indicador['indid'];?>')">
														<i class="fa fa-plus-circle"></i>
													</a>
													<a data-toggle="tooltip" data-original-title="Alterar indicador" onclick="popupGuia('editar', 'indicador', '<?=$indicador['indid']; ?>')">
														<i class="fa fa-edit"></i>
													</a>
													<a href="#" data-toggle="tooltip" data-original-title="Excluir indicador">
														<i class="fa fa-trash-o" onclick="excluirItemGuia('indicador', '<?=$indicador['indid']; ?>')"></i>
													</a>
												</div>

												<?php
												$arCriterios = Par3_Controller_ConfiguracaoControle::recuperarCriteriosGuia($indicador['indid']);
												if($arCriterios){
												    foreach ($arCriterios as $criterio){ ?>
												<ul>
													<li data-jstree='{"icon":false}'>
														<label>&nbsp;<?php echo "(".$criterio['crtcod'].") ".$criterio['crtdsc']; ?></label>
														<div>
<!-- 															<a href="#" data-toggle="tooltip" data-original-title="Incluir ação"> -->
<!--																<i class="fa fa-plus-circle" onclick="popupGuia('incluir', 'acao', '<?=$criterio['crtid'];?>')"></i> -->
<!-- 															</a> -->
															<a href="#" data-toggle="tooltip" data-original-title="Alterar componente">
																<i class="fa fa-edit" onclick="popupGuia('editar', 'criterio', '<?=$criterio['crtid']; ?>')"></i>
															</a>
															<a href="#" data-toggle="tooltip" data-original-title="Excluir componente">
																<i class="fa fa-trash-o" onclick="excluirItemGuia('criterio', '<?=$criterio['crtid']; ?>')"></i>
															</a>
															<?php if($arCriterios[1]){ // Quer dizer que ele tem mais que um critério ?>
    															<a href="#" data-toggle="tooltip" data-original-title="Vinculação de Componentes">
    																<i class="fa fa-chain" onclick="popupGuiaCriterio('<?=$criterio['crtid']; ?>')"></i>
    															</a>
															<?php } ?>
															<a href="#" data-toggle="tooltip" data-original-title="Mover para cima">
																<i class="fa fa-arrow-up" onclick="moverItem('criterio', 'cima', '<?=$criterio['crtid']; ?>')"></i>
															</a>
															<a href="#" data-toggle="tooltip" data-original-title="Mover para baixo">
																<i class="fa fa-arrow-down" onclick="moverItem('criterio', 'baixo', '<?=$criterio['crtid']; ?>')"></i>
															</a>
														</div>

														<?php /* $arAcoesCriterio = $oConfiguracaoControle->recuperarPropostaAcoesCriterioGuia($criterio['crtid']); ?>
														<?php if($arAcoesCriterio): ?>
														<?php foreach ($arAcoesCriterio as $acaoCriterio): ?>
														<ul>
															<li data-jstree='{"icon":"fa fa-dot-circle-o"}'>
																<label><?php echo $acaoCriterio['ppadsc']; ?></label>
																<div>
																	<a href="#" data-toggle="tooltip" data-original-title="Alterar ação">
																		<i class="fa fa-edit" onclick="abrirPopupGuia('editar', 'acao', '<?=$acaoCriterio['ppaid'];?>')"></i>
																	</a>
																	<a href="#" data-toggle="tooltip" data-original-title="Excluir indicador">
																		<i class="fa fa-trash-o" onclick="excluirItemGuia('acao', '<?=$acaoCriterio['ppaid'];?>')"></i>
																	</a>
																</div>

																<?php $arSubacoesCriterio = $oConfiguracaoControle->recuperarSubacoesPorCriterio($acaoCriterio['crtid']); ?>
																<?php if($arSubacoesCriterio): ?>
																<?php foreach ($arSubacoesCriterio as $subAcoesCriterio): ?>
																<ul>
																	<li data-jstree='{"icon":"fa fa-bullseye"}'>
																		<label><?php echo $subAcoesCriterio['ppsordem']." - ".$subAcoesCriterio['ppsdsc']; ?></label>
																		<div>
																			<a href="#" data-toggle="tooltip" data-original-title="Alterar ação">
																				<i class="fa fa-edit" onclick="abrirPopupGuiaSubacao('editar', '<?=$subAcoesCriterio['ppsid'];?>')"></i>
																			</a>
																			<a href="#" data-toggle="tooltip" data-original-title="Excluir indicador">
																				<i class="fa fa-trash-o" onclick="excluirItemGuia('subacao', '<?=$subAcoesCriterio['ppsid'];?>')"></i>
																			</a>
																		</div>
																	</li>
																</ul>
																<?php endforeach; ?>
																<?php endif; ?>

															</li>
														</ul>
														<?php endforeach; ?>
														<?php endif; */ ?>

													</li>
												</ul>
												<?php } ?>
												<?php }   ?>

											</li>
										</ul>
										<?php endforeach; ?>
										<?php endif; ?>

									</li>
								</ul>
								<?php endforeach; ?>
								<?php endif; ?>

							</li>
						</ul>
						<?php endforeach; ?>
						<?php endif; ?>

					<?php endforeach; ?>
				</table>
			</div>
		</div>
	</div>
</div>

<div id="modal-form" class="modal fade" aria-hidden="true">
	<div class="modal-dialog">
		<div id="html_modal-form">
	</div>
</div>

<div id="modal-form-large" class="modal fade" aria-hidden="true">
	<div class="modal-dialog modal-lg" style="width: 70%;">
		<div id="html_modal-form-large">
	</div>
</div>

<script>
function popupGuia(acao, tipo, id){
	$.ajax({
   		type: "POST",
   		url: 'par3.php?modulo=principal/configuracao/popupGuia&acao=A',
   		data: '&acaoGuia='+acao+'&tipoGuia='+tipo+'&codigo='+id,
   		async: false,
   		success: function(resp){
			$('#html_modal-form').html(resp);
		    $('#modal-form').modal();
   		}
 	});
}

function popupGuiaIndicador(id, dimensao){
	$.ajax({
   		type: "POST",
   		url: 'par3.php?modulo=principal/configuracao/popupGuiaDimensaoPainel&acao=A',
   		data: '&dimid='+id+'&dimensao='+dimensao,
   		async: false,
   		success: function(resp){
			$('#html_modal-form-large').html(resp);
		    $('#modal-form-large').modal();
   		}
 	});
}

function popupGuiaCriterio(id){
	$.ajax({
   		type: "POST",
   		url: 'par3.php?modulo=principal/configuracao/popupGuiaCriterio&acao=A',
   		data: '&crtid='+id,
   		async: false,
   		success: function(resp){
			$('#html_modal-form-large').html(resp);
		    $('#modal-form-large').modal();
   		}
 	});
}

function moverItem(tipo, direcao, id){
	url = 'par3.php?modulo=principal/configuracao/guia&acao=A&acaoGuia=reordenar&id='+id+'&tipo='+tipo+'&direcao='+direcao;
	document.location.href = url;
}

function excluirItemGuia(tipo, codigo){
	url = 'par3.php?modulo=principal/configuracao/guia&acao=A&acaoGuia=excluir&tipoGuia=' + tipo + '&codigo=' + codigo;
	swal({
		title: "Tem certeza?",
		text: "Deseja deletar este registro?",
		type: "warning",
		showCancelButton: true,
		confirmButtonColor: "#DD6B55",
		cancelButtonText: "Cancelar",
		confirmButtonText: "Confirmar",
		closeOnConfirm: false },
		function(){
			document.location.href = url;
		});
}

</script>
<?php

if($_GET['acaoGuia'] == 'excluir'){
	
	if($_GET['tipoGuia'] == 'dimensao'){
		
		$oDimensao = new Dimensao();
		$oDimensao->deletarDimensaoGuia($_GET['codigo']);				
	}
	
	if($_GET['tipoGuia'] == 'area'){
		
		$oArea = new Area();
		$oArea->deletarAreaGuia($_GET['codigo']);		
	}
	
	if($_GET['tipoGuia'] == 'indicador'){
		
		$oIndicador = new Indicador();
		$oIndicador->deletarIndicadorGuia($_GET['codigo']);		
	}
	
	if($_GET['tipoGuia'] == 'criterio'){
		
		$oCriterio = new Criterio();
		$oCriterio->deletarCriterioGuia($_GET['codigo']);
	}
	
	if($_GET['tipoGuia'] == 'acao'){
		
		$sqlD = ("delete from par.acao where ppaid = {$_GET['codigo']};");
		$db->executar($sqlD);
		$db->commit();
		
		$oPropostaAcao = new PropostaAcao();
		$oPropostaAcao->ppaid = $_GET['codigo'];
		$oPropostaAcao->excluir();
		$oPropostaAcao->commit();				
	}
	
	if($_GET['tipoGuia'] == 'subacao'){
		
		$sql = "SELECT sbaid FROM par.subacao WHERE sbastatus = 'A' AND ppsid = {$_GET['codigo']} LIMIT 1";
		$sbaid = $db->pegaUm($sql);
		
		if( $sbaid ){
			echo "<script>alert('Já existem registros vinculados a essa subação!'); window.location.href='/par/par.php?modulo=principal/configuracao/guia&acao=A';</script>";
			exit();
		} else {
		
			$oCriterioPropostaSubacao = new CriterioPropostaSubacao();
			$oCriterioPropostaSubacao->excluirPorPpsid($_GET['codigo']);
			
			$oPropostaSubacaoMunicipio = new PropostaSubacaoMunicipio();
			$oPropostaSubacaoMunicipio->excluirPorPpsid($_GET['codigo']);
			
			$oPropostaSubacaoIdeb = new PropostaSubacaoIdeb();
			$oPropostaSubacaoIdeb->excluirPorPpsid($_GET['codigo']);
			
			$oPropostaSubacaoAnos = new PropostaSubacaoAnos();
			$oPropostaSubacaoAnos->excluirPorPpsid($_GET['codigo']);
			
			$sqlD = ("delete from par.propostasubacaoobra where ppsid = {$_GET['codigo']};");
			$sqlD.= ("delete from par.propostasubacaoitem where ppsid = {$_GET['codigo']};");
			$sqlD.= ("delete from par.propostasubacaoparecer where ppsid = {$_GET['codigo']};");
			$arrSbaid = $db->carregar("select sbaid from par.subacao where ppsid = {$_GET['codigo']}");
			if($arrSbaid){
				foreach($arrSbaid as $sbaid){
					$sqlD.= ("delete from par.subacaodetalhe  where sbaid = {$sbaid['sbaid']};");
					$sqlD.= ("delete from par.subacaobeneficiario  where sbaid = {$sbaid['sbaid']};");	
					$sqlD.= ("delete from par.subacaoobra  where sbaid = {$sbaid['sbaid']};");
					$sqlD.= ("delete from par.subacaoescolas  where sbaid = {$sbaid['sbaid']};");
				}
			}
			$sqlD = "delete from par.subacao where sbastatus = 'A' AND ppsid = {$_GET['codigo']}";
			$db->executar($sqlD);
			$db->commit();
			
			$oPropostaSubacao = new PropostaSubacao();
			$oPropostaSubacao->ppsid = $_GET['codigo'];
			$oPropostaSubacao->excluir();
			$oPropostaSubacao->commit();
		}				
	}
	
}

if($_POST['requisicao'] == 'teste'){
	die();
}

?>
<link rel="stylesheet" href="../includes/jquery-treeview/jquery.treeview.css" />
<!--<link rel="stylesheet" href="../includes/jquery-treeview/red-treeview.css" />
--><script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-treeview/jquery.treeview.js"></script>
<script type="text/javascript" src="./includes/jquery-treeview/jquery.cookie.js" ></script>
<?php 
if($_POST['filtro']){
	$params = implode("%",$_POST['filtro']);
}else{
	$params = '0';
}
?>
<script type="text/javascript"><!--


//jQuery.metadata.setType("attr", "cokkieGuia");

$(document).ready(function(){
	$("#browser").treeview({
		collapsed: true,
		animated: "medium",
		control:"#sidetreecontrol"
		//persist: "cookie"
		
	});
});

function abrirTodos(n)
{
	if(n == 1){
		divCarregando('divCarregando');
	}
	
	var qtdHitarea = $(".hitarea").length
	var data = new Array();
		data.push({name : 'requisicao', value : 'teste'});	
		$.ajax({
			   type		: "POST",
			   //url		: "par.php?modulo=principal/configuracao/guia&acao=A",
			   data		: data,
			   async    : true,
			   success	: function(){
			   				var i = n;
			   				var pecorrer = n+399;
			   				for(i;i<=pecorrer;i++)
							{
								if($('#hit_'+i).hasClass('expandable-hitarea')){ //collapsable-hitarea 'expandable-hitarea'
									$('#hit_'+i).trigger('click');
								}
							}
			   				if(pecorrer < qtdHitarea){
			   					setTimeout('abrirTodos('+pecorrer+')',990);
				   			} else {
			   					divCarregado();
					   		}
				   				
						  }
	 });
	return false;
}

/*
function fecharTodos(n)
{
	divCarregando('divCarregando');
	if(n == ''){
		//n = $(".hitarea").length;
		n = 1;
	}
	
	var qtdHitarea = $(".hitarea").length
	var data = new Array();
		data.push({name : 'requisicao', value : 'teste'});	
		$.ajax({
			   type		: "POST",
			   //url		: "par.php?modulo=principal/configuracao/guia&acao=A",
			   data		: data,
			   async    : true,
			   success	: function(){
			   				var i = n;
			   				//var pecorrer = n+200;
			   				var pecorrer = n+10;
			   				for(i;i<=pecorrer;i++)
							{
			   					console.log(i+' -> '+$('#hit_'+i).attr('id')+' -> '+$('#hit_'+i).attr('class'));
								if($('#hit_'+i).hasClass('collapsable-hitarea')){ //collapsable-hitarea 'expandable-hitarea'
									//console.log(i+' -> '+$('#hit_'+i).parent().attr('class')+' -> '+$('#hit_'+i).parent().attr('class')+' -> '+$('#hit_'+i).parent().parent().css('display'));
									$('#hit_'+i).trigger('click');
									$('#hit_'+i).attr('class','hitarea expandable-hitarea');
									// LI
									$('#hit_'+i).parent().attr('class','expandable');
									if(i != 1){
										//$('#hit_'+i).parent().parent().css('display', 'none');
										$('#hit_'+i).parent().parent().hide();
									}
									
								}
							}
			   				if(pecorrer < qtdHitarea){
			   					//setTimeout('fecharTodos('+a+','+className+')',990);
			   					//setTimeout('fecharTodos('+pecorrer+')',990);
				   			} else {
			   					//divCarregado();
					   		}
						  }
	 });

divCarregado();
	return false;
}
*/

function abrirRelatorioGuia(itrid){
	url = 'par.php?modulo=principal/configuracao/popupRelatorioGuia&acao=A&itrid=' + itrid;
	window.open(url, 'popupGuia', "height=350,width=600,scrollbars=yes,top=50,left=200" );
}


function abrirPopupGuia(acao, tipo, codigo){
	url = 'par.php?modulo=principal/configuracao/popupGuia&acao=A&acaoGuia=' + acao + '&tipoGuia=' + tipo + '&codigo=' + codigo;
	window.open(url, 'popupGuia', "height=350,width=600,scrollbars=yes,top=50,left=200" );
}

function abrirPopupGuiaSubacao(acao, codigo, itrid){	
	if(acao == 'incluir'){
		param = '&acaoGuia='+acao+'&indid='+codigo+'&itrid='+itrid;
	}else{
		param = '&acaoGuia='+acao+'&ppsid='+codigo+'&itrid='+itrid;
	}
	
	url = 'par.php?modulo=principal/configuracao/popupGuiaSubacao&acao=A'+param;
	window.open(url, 'popupGuia', "height=600,width=800,scrollbars=yes,top=50,left=200" );
}

function excluirItemGuia(tipo, codigo){
	url = 'par.php?modulo=principal/configuracao/guia&acao=A&acaoGuia=excluir&tipoGuia=' + tipo + '&codigo=' + codigo;
	if(confirm("Deseja deletar este registro?")){
		document.location.href = url;
	}
}
//
--></script>
<style>
	.imguia {
		padding-right:3px;
	}
</style>
<?php
require_once APPRAIZ . 'includes/cabecalho.inc';
print "<br>";
monta_titulo( "Guia", '&nbsp;' );
$oConfiguracaoControle = new ConfiguracaoControle();

$arInstrumentos = $oConfiguracaoControle->recuperarIntrumentosGuia();
/*$arInstrumentos = array(
						array('itrid' => 1, 'itrdsc' => 'Diagnóstico Estadual'),
						array('itrid' => 2, 'itrdsc' => 'Diagnóstico Municipal'),
						array('itrid' => 3, 'itrdsc' => 'Indicadores Qualitativos do Brasil Pró'),
					);
*/
?>
<div id="divCarregando">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
			<!-- p>
				<form action="" method="post">
					<?php $_REQUEST['filtro'] = $_REQUEST['filtro'] ? $_REQUEST['filtro'] : array() ?>
					<b>FILTROS:</b>
					<?php
					if(!$_REQUEST['filtro']){
						$marca = "checked";
					}
					if(in_array('0', $_REQUEST['filtro'])){
						
						$marca = "checked";
						if(in_array('1', $_REQUEST['filtro']) || in_array('2', $_REQUEST['filtro']) || in_array('3', $_REQUEST['filtro'])){
							$marca = "";
						}
					}
					?>
					<input type="hidden" name="filtro[]" value="true">
					<input type="checkbox" id="selectAll" name="filtro[]" value="0" <?php echo in_array('0', $_REQUEST['filtro']) ? "checked" : '' ?> <?=$marca?>> Todos 
					<input type="checkbox" class="filtro" id="criterio" name="filtro[]" value="1" <?php echo in_array('1', $_REQUEST['filtro']) ? "checked" : '' ?> <?=$marca?>> Criterio
					<input type="checkbox" class="filtro" id="acao" name="filtro[]" value="2" <?php echo in_array('2', $_REQUEST['filtro']) ? "checked" : '' ?> <?=$marca?>> Ação
					<input type="checkbox" class="filtro" id="subacao" name="filtro[]" value="3" <?php echo in_array('3', $_REQUEST['filtro']) ? "checked" : '' ?> <?=$marca?>> Subação
					<input type="submit" value="Filtrar">				
				</form>
			</p -->			
			<!--<div><span><a href="?#">Fechar Todos</a> | <a href="#" onclick="return abrirTodos(1)">Abrir Todos</a></span></div>-->
			<div id="main" style="background: #f5f5f5">
				<div id="sidetree">
					<div class="treeheader">&nbsp;</div>
					<div id="sidetreecontrol"><span><a href="?#">Fechar Todos</a> | <a href="#" onclick="return abrirTodos(1)">Abrir Todos</a> <br /><br /> <img src="../includes/jquery-treeview/images/base.gif" align="top" /><strong>Guia de Ações Padronizadas</strong></span></div>
					<!--<div id="sidetreecontrol"><span><a href="?#">Fechar Todos</a> | <a href="?#" id="abrirTodos">Abrir Todos</a> <br /><br /> <img src="../includes/jquery-treeview/images/base.gif" align="top" /><strong>Guia de Ações Padronizadas</strong></span></div>-->
					<!--<div><span><img src="../includes/jquery-treeview/images/base.gif" align="top" /><strong>Guia de Ações Padronizadas</strong></span></div>-->
					<ul id="browser" class="filetree treeview-famfamfam">
					<?php foreach ($arInstrumentos as $instrumento): ?>
						<li><span><a href="javascript:abrirPopupGuia('incluir', 'dimensao', '<?=$instrumento['itrid'];?>')" cokkieGuia="guiadimensao<?php echo $instrumento['itrid'];?>" ><img border="0" class="imguia" src="../imagens/gif_inclui.gif" align="absmiddle" title="Incluir dimensão"></a> <img src="../imagens/consultar.gif" onclick="abrirRelatorioGuia('<?php echo $instrumento['itrid'];?>');" align="absmiddle" title="Visualizar relatório"> <?php echo $instrumento['itrdsc'];?></span>
							<?php
								$arDimensoes = $oConfiguracaoControle->recuperarDimensoesGuia($instrumento['itrid']);
								if($arDimensoes):
							?>
								<ul>
									<?php foreach ($arDimensoes as $dimensao): ?>
										<li><span><a href="javascript:abrirPopupGuia('incluir', 'area', '<?=$dimensao['dimid'];?>')" cokkieGuia="guiaarea<?php echo $dimensao['dimid'];?>" ><img border="0" class="imguia" src="../imagens/gif_inclui.gif" align="absmiddle" title="Incluir área" /></a><img border="0" class="imguia" src="../imagens/alterar.gif" onclick="abrirPopupGuia('editar', 'dimensao', '<?=$dimensao['dimid'];?>')" align="absmiddle" style="cursor:pointer;" title="Alterar dimensão" /><img border="0" class="imguia" src="../imagens/excluir.gif" onclick="excluirItemGuia('dimensao', '<?=$dimensao['dimid'];?>')" align="absmiddle" style="cursor:pointer;" title="Excluir dimensão" /><?php echo $dimensao['dimcod'].' - '.$dimensao['dimdsc'];?></span>
											<?php 
												$arAreas = $oConfiguracaoControle->recuperarAreasGuia($dimensao['dimid']);
												if($arAreas):
											?>
												<ul>
													<?php foreach ($arAreas as $area): ?>
														<li><span><a href="javascript:abrirPopupGuia('incluir', 'indicador', '<?=$area['areid'];?>')" cokkieGuia="guiaindicador<?php echo $area['areid'];?>" ><img border="0" class="imguia" src="../imagens/gif_inclui.gif" align="absmiddle" title="Incluir indicador" /></a><img border="0" class="imguia" src="../imagens/alterar.gif" onclick="abrirPopupGuia('editar', 'area', '<?=$area['areid'];?>')" align="absmiddle" style="cursor:pointer;" title="Alterar área" /><img border="0" class="imguia" src="../imagens/excluir.gif" onclick="excluirItemGuia('area', '<?=$area['areid'];?>')" align="absmiddle" style="cursor:pointer;" title="Excluir área" /><?php echo $dimensao['dimcod'].".".$area['arecod']." - ".$area['aredsc'];?></span>
															<?php 
																$arIndicadores = $oConfiguracaoControle->recuperarIndicadoresGuia($area['areid']);
																if($arIndicadores):
															?>
																<ul>
																	<?php foreach ($arIndicadores as $indicador): ?>
																		<li><span><a href="javascript:abrirPopupGuia('incluir', 'criterio', '<?=$indicador['indid'];?>')" cokkieGuia="guiacriterio<?php echo $indicador['indid'];?>" ><img border="0" class="imguia" src="../imagens/gif_inclui.gif" align="absmiddle" title="Incluir critério" /></a><img border="0" class="imguia" src="../imagens/gif_inclui.gif" onclick="abrirPopupGuiaSubacao('incluir', '<?=$indicador['indid'];?>', '<?=$instrumento['itrid'];?>')" align="absmiddle" style="cursor:pointer;" title="Incluir subação" /><img border="0" class="imguia" src="../imagens/alterar.gif" onclick="abrirPopupGuia('editar', 'indicador', '<?=$indicador['indid'];?>')" style="cursor:pointer;" align="absmiddle" title="Alterar indicador" /><img border="0" class="imguia" src="../imagens/excluir.gif" onclick="excluirItemGuia('indicador', '<?=$indicador['indid'];?>')" style="cursor:pointer;" align="absmiddle" title="Excluir indicador" /><?php echo $dimensao['dimcod'].".".$area['arecod'].".".$indicador['indcod']." - ".$indicador['inddsc'];?></span>
																			<?php 
																				$arCriteriosAcoes = $oConfiguracaoControle->recuperarCriteriosAcoesGuia($indicador['indid']);
																				if($arCriteriosAcoes):
																			?>
																				<ul>
																					<?php foreach ($arCriteriosAcoes as $criterioAcao): ?>
																						<li><span><img border="0" class="imguia" src="../imagens/gif_inclui.gif" onclick="abrirPopupGuia('incluir', 'acao', '<?=$criterioAcao['crtid'];?>')" align="absmiddle" style="cursor:pointer;" title="Incluir ação" /><img border="0" class="imguia" src="../imagens/alterar.gif" onclick="abrirPopupGuia('editar', 'criterio', '<?=$criterioAcao['crtid'];?>')" style="cursor:pointer;" align="absmiddle" title="Alterar critério" /><img border="0" class="imguia" src="../imagens/excluir.gif" onclick="excluirItemGuia('criterio', '<?=$criterioAcao['crtid'];?>')" style="cursor:pointer;" align="absmiddle" title="Excluir indicador" />&nbsp;<?php echo "(".$criterioAcao['crtpontuacao'].") ".$criterioAcao['crtdsc'];?></span>
																							<?php 
																								$arAcoesCriterio = $oConfiguracaoControle->recuperarPropostaAcoesCriterioGuia($criterioAcao['crtid']);
																								if($arAcoesCriterio):
																							?>
																							<ul>
																								<?php foreach ($arAcoesCriterio as $acaoCriterio): ?>
																									<li><span><img border="0" class="imguia" src="../imagens/alterar.gif" onclick="abrirPopupGuia('editar', 'acao', '<?=$acaoCriterio['ppaid'];?>')" style="cursor:pointer;" align="absmiddle" title="Alterar ação" /><img border="0" class="imguia" src="../imagens/excluir.gif" onclick="excluirItemGuia('acao', '<?=$acaoCriterio['ppaid'];?>')" style="cursor:pointer;" align="absmiddle" title="Excluir indicador" />&nbsp;<?php echo $acaoCriterio['ppadsc'];?></span>
																										<?php 
																											$arSubacoesCriterio = $oConfiguracaoControle->recuperarSubacoesPorCriterio($acaoCriterio['crtid']);
																											if($arSubacoesCriterio):
																										?>
																											<ul>
																												<?php foreach ($arSubacoesCriterio as $subacoescriterio): ?>
																													<li><span><img border="0" class="imguia" src="../imagens/alterar.gif" onclick="abrirPopupGuiaSubacao('editar', '<?=$subacoescriterio['ppsid'];?>', '<?=$instrumento['itrid'];?>')" style="cursor:pointer;" align="absmiddle" title="Alterar ação" /><img border="0" class="imguia" src="../imagens/excluir.gif" onclick="excluirItemGuia('subacao', '<?=$subacoescriterio['ppsid'];?>')" style="cursor:pointer;" align="absmiddle" title="Excluir indicador" />&nbsp;<?php echo $subacoescriterio['ppsordem']." - ".$subacoescriterio['ppsdsc']; ?></span>
																													</li>
																												<?php endforeach;?>
																											</ul>
																										<?php endif; ?>
																									</li>
																								<?php endforeach;?>
																							</ul>
																							<?php endif; ?>
																						</li>
																					<?php endforeach;?>
																				</ul>
																			<?php endif; ?>
																		</li>
																	<?php endforeach;?>
																</ul>	
															<?php endif; ?>
														</li>
													<?php endforeach; ?>
												</ul>	
											<?php endif; ?>
										</li>
									<?php endforeach; ?>
								</ul>
							<?php endif; ?>
						</li>
					<?php endforeach; ?>
					</ul>	
				</div><!-- div #sidetree -->
			</div><!-- div #main -->
		</td>
	</tr>
</table>
</div>
<div id="divTeste"></div>
<script type="text/javascript">
<?php
if ($habilitado):
?>
jQuery(document).ready(function() {

	jQuery( ".originais" ).draggable({
		connectToSortable: ".div_fotos2",
		helper: "clone",
		revert: "invalid",
		stop: function( event, ui ) {
			var arrId = jQuery(this).attr('id').split('_');
			var Id = arrId[1];
			if( arrId[2] != '' ){
				Id = arrId[2];
			}
			jQuery('#fotos_galeria').find("#s_" + arrId[1]).attr("class","draggable f_selected");
		}
	}).sortable({
		cancel: ".nodraggable"
	});

	jQuery( ".div_fotos2" ).droppable({
		drop: function( event, ui ) {
			var tableta = jQuery(this).attr('id').split('_');
			var campo 	= tableta[1];
			tableta		= tableta[0]
			var id 		= jQuery(this).attr('name');
			var arqid  	= jQuery(this).children('.originais').attr('name');
			if( id && arqid ){
				var url = window.location + "&ajax=gravarFoto&tabela="+tableta+"&campo="+campo+"&id="+id+"&arqid="+arqid;
				jQuery.ajax({
					url: url,
					success: function(data) {
				  }
				});
			}
			jQuery(this).children().removeClass("f_selected");
			jQuery(this).children().removeClass("originais");
			jQuery(this).children().removeClass("draggable");
			jQuery(this).children().addClass("nodraggable");
			jQuery(this).children().find('.img_class').addClass("img_class2");
			jQuery(this).children().find('.img_class').removeClass("img_class");
			jQuery(this).find('.fechar').show();
		}
	}).sortable({
		revert: false,
		placeholder: "draggable_space",
		cancel: ".nodraggable"
	});
	jQuery( "ul, li" ).disableSelection();
});
<?php
endif;
?>
</script>
<table class="tabela" bgcolor="white" cellspacing="1" cellpadding="3" align="center" border="0" style="width:100%">
	<tbody>
		<tr bgcolor="#DCDCDC">
			<td width="100%" align="center" colspan="2">
				<label class="TituloTela" style="color:#000000;">Cadastro de Fotos da Supervisão</label>
			</td>
		</tr>
		<tr>
			<td bgcolor="#e9e9e9" style="text-align: center; width: 180px;">
			<?php if( $_SESSION['obras2']['sfndeid'] && $habilitado ): ?>
				<a onclick="incluirFotos('<?php echo $_SESSION['obras2']['sfndeid'] ?>');" title="Enviar arquivos">
					<img src="../imagens/gadget_arvore.png" title="Enviar arquivos" border="0" align="absmiddle"/><br/>
					<b>Enviar arquivos</b>
				</a>
			<?php elseif ( $habilitado ): ?>
				Erro! O registro da supervisão da empresa não existe no banco de dados. 
			<?php endif; ?>
			</td>
			<td bgcolor="#e9e9e9" align="center" style="width:100%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')">
				<p>
					Todas as fotos devem estar no formato JPG<br>
					Possuir marcação GPS no arquivo<br>
					Terem menos de 4 MB.
				</p>
			</td>
		</tr>
	</tbody>
</table>
			<table class="tabela" bgcolor="white" cellspacing="1" cellpadding="3" align="center" border="0" style="width:100%">
				<tbody>
					<tr>
						<td class="container-perguntas" width="60%">
                            <!-- Alteração -->
                            <div class="container-perguntas" style="height: 528px; overflow: scroll;">
							<?php 
							$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras2","destino" => "obras2");
//							$_SESSION['imgparams'] = array("filtro" => " 1=1 ", "tabela" => "obras2.arquivoquestaosupervisao");
						    
							$sfndeid = ($sfndeid ? $sfndeid : 0);
							
						    $questao  = new Questao();
						    $arquivoQuestaoSupervisao = new ArquivoQuestaoSupervisao();
						    
						    switch ( $tecnologiaMi->tmiid ){
								case TMIID_PVC_CONCRETO:
									$qstescopo = 'SMI1';
									break;
								case TMIID_MATERIAIS_COMPOSITOS:
									$qstescopo = 'SMI3';
									break;
								default:
									$qstescopo = '';
									break;
							}
							
						    $arFiltro = array("qstescopo" => $qstescopo,
					    				  	  "orgid" 	  => $_SESSION['obras2']['orgid'], 
					    				  	  "sfndeid" 	  => $sfndeid);
						    $arDados  = $questao->pegaTodaEstrutura( $arFiltro );
						    
						    $etqidUlt 	= null;
						    $itcidUlt 	= null;
						    $qstidUlt 	= null;
						    $temImg		= false;
						   
						    $subQuestao = new SubQuestao();
						    $arquivoRespostaSubQuestao = new ArquivoRespostaSubQuestao();
						    
						    foreach ( $arDados as $k => $dados ){
						    	
						    	$arDadosSubQuestao = $subQuestao->pegaSubQuestaoPorQstid( $dados['qstid'] );
						    	
						    	$qstctrlobs = json_decode( $dados['qstctrlobs'] );
						    	switch ( $dados['qtsresposta'] ){
						    		case 't':
						    			$obsClass = ($qstctrlobs->S ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
						    			break;
						    		case 'f':
						    			$obsClass = ($qstctrlobs->N ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');	
						    			break;
						    		case 'n':
						    			$obsClass = ($qstctrlobs->NA ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
						    			break;
						    		default:
						    			$obsClass = 'divQuestaoNivel3_none';		
						    			break;
						    	}
						    	
						    	$qstctrlimg = json_decode( $dados['qstctrlimagem'] );
						    	
						    	switch ( $dados['qtsresposta'] ){
						    		case 't':
						    			$imgClass 	= ($qstctrlimg->S ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
						    			$temImg		= $qstctrlimg->S;		
						    			break;
						    		case 'f':
						    			$imgClass 	= ($qstctrlimg->N ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
						    			$temImg 	= $qstctrlimg->N;			
						    			break;
						    		case 'n':
						    			$imgClass 	= ($qstctrlimg->NA ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');	
						    			$temImg 	= $qstctrlimg->NA;			
						    			break;
						    		default:
						    			$imgClass = 'divQuestaoNivel4_none';		
						    			break;
						    	}
							    	
						    	
						    	// ABRE Questionário
						    	if ( $etqidUlt != $dados['etqid'] ){
						    		if ( !is_null($etqidUlt) ){
										$htmlTemp .= "</fieldset>";	    		
						    		}
					    			$etqidUlt = $dados['etqid'];
									$htmlTemp .= "<fieldset>
										      	<legend>{$dados['etqdsc']}</legend>
												<div id=\"div_questoes\" style=\"height:500px;overflow:auto;\">";	    		
						    	}
						    		    	
						    	// ETAPA
						    	if ( $itcidUlt != $dados['itcid'] ){
						    		$itcidNum++;
									$htmlTemp .= "<div class='divItemComposicao'>{$itcidNum} - {$dados['itcdsc']}</div>";	    		
						    		$itcidUlt = $dados['itcid'];
						    	}
						    	
						    	// DIVISÃO
						    	if ( $dvqidUlt != $dados['dvqid'] && !empty( $dados['dvqid'] ) ){
									$htmlTemp .= "<div class='divDivisao'>{$dados['dvqnumero']} - {$dados['dvqdsc']}</div>";	    		
						    		$dvqidUlt = $dados['dvqid'];
						    	}
						    	
						    	// QUESTÃO
						    	if ( $qstidUlt != $dados['qstid'] ){
						    		
							    	// Sub Questão
							    	$subClass  			= ($dados['qtsresposta'] == 'f' ? 'divSubQuestao' : 'divSubQuestao_none' );
							    	$subItemClass 		= 'divSubItemQuestao';
							    	
							    	$subImgClass 		= "divSubQuestaoNivel4";
						    		$htmlSubQuestao 	= "";

						    		if( is_array( $arDadosSubQuestao ) ){
							    		if ( count($arDadosSubQuestao) > 0 ){
							    			$htmlSubQuestao .= "<div id=\"div_subquestao_{$dados['qstid']}_{$dadosSubQuestao['sqtid']}\" class=\"{$subClass}\">
							    									<div class=\"divSubQuestaoNivel1\"></div>";
								    		$temImgSub = false;
								    		
								    		foreach ($arDadosSubQuestao as $dadosSubQuestao ){
								    			
								    			$arResultadosDadosSubQuestao = $subQuestao->pegaResultadosSubQuestaoPorSqtidQtsid( $dadosSubQuestao['sqtid'], ($dados['qtsid'] ? $dados['qtsid'] : 0) );
								    			
								    			$temImgSub	= false;
								    			$temImgSub = ($temImgSub || $dadosSubQuestao['sqtimg'] == 't') && $arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A';
								    			
								    			if( $arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A' ){
										    		$htmlSubQuestao .= "<div id=\"div_subquestao_item_{$dadosSubQuestao['sqtid']}\" >
																			<div class='$subImgClass' id='div_img_sub_{$dadosSubQuestao['sqtid']}'>
																				<fieldset class=\"field_fotos\" id=\"field_img_sub_{$dadosSubQuestao['sqtid']}\">
																					<legend>Fotos da Subquestão - {$dadosSubQuestao['sqtdsc']} <div class='subquestaoobs'><span style='font-weight:bold'>Observação: </span>{$arResultadosDadosSubQuestao[0]['rsqobs']}</div></legend>
																					<ul id=\"arquivorespostasubquestao_rsqid\" name=\"{$arResultadosDadosSubQuestao[0]['rsqid']}\" class=\"div_fotos div_fotos2\">";
										    		$arrArquivosSubQuestão = $arquivoRespostaSubQuestao->listaPorRespQuestao( $arResultadosDadosSubQuestao[0]['rsqid'], $sfndeid );
										    		if( is_array( $arrArquivosSubQuestão ) ){
										    			foreach( $arrArquivosSubQuestão as $arquivoSub ){
										    				$htmlSubQuestao .= '		<li class="ui-draggable ui-sortable nodraggable" name="'.$arquivoSub['arqid'].'" id="s_'.$arquivoSub['arqid'].'" unselectable="on" style="-moz-user-select: none; display: list-item;">
																		    				<img onclick="removerFotoGaleria(this,'.$arquivoSub['arsid'].')" style="" class="fechar" title="Remover Foto" src="../imagens/fechar.jpeg">
																		    				<img width="91" height="68.25" src="../slideshow/slideshow/verimagem.php?arqid='.$arquivoSub['arqid'].'&amp;newwidth=100&amp;newheight=85" 
																		    					 ondblclick="abrirGaleria(\''.$arquivoSub['arqid'].'\',\'0\',this)" class="img_class2">
																	    				</li>';
										    			}
										    		}
													$htmlSubQuestao .= "			</ul>
																				</fieldset>
																			</div>
																		</div>";	
													$htmlSubQuestaoTipo = '';			    								
								    			}
									    	}	
							    			$htmlSubQuestao .= "</div>";
							    		}
						    		}
							    	
							    	if( ($temImg || $temImgSub) && $dados['qtsresposta'] != '' ){
										$htmlTemp .= "<div class='divQuestao'>
														<fieldset class=\"field_fotos\" id=\"field_img_{$dadosSubQuestao['sqtid']}\">
															<legend>Fotos da Questão - {$dados['qstnumero']} - {$dados['qstdsc']} Resp.: " . ($dados['qtsresposta'] == 't' ? "Sim" : ($dados['qtsresposta'] == 'f' ? "Não" : "Não se aplica")) . " </legend>
															<ul id=\"arquivoquestaosupervisao_qtsid\" name=\"{$dados['qtsid']}\" class=\"div_fotos div_fotos2\">";
										
										$arArquivosQuestao = $arquivoQuestaoSupervisao->listaPorRespQuestao( $dados['qtsid'] );
										if( is_array( $arArquivosQuestao ) ){
											foreach( $arArquivosQuestao as $arquivo ){
										$htmlTemp .= '			<li class="ui-draggable ui-sortable nodraggable" name="'.$arquivo['arqid'].'" id="s_'.$arquivo['arqid'].'" unselectable="on" style="-moz-user-select: none; display: list-item;">
																	<img onclick="removerFotoGaleria(this,'.$arquivo['aqsid'].')" style="" class="fechar" title="Remover Foto" src="../imagens/fechar.jpeg">
																	<img width="91" height="68.25" src="../slideshow/slideshow/verimagem.php?arqid='.$arquivo['arqid'].'&amp;newwidth=100&amp;newheight=85" 
																		 ondblclick="abrirGaleria(\''.$arquivo['arqid'].'\',\'0\',this)" class="img_class2">
																</li>';
											}
										}
										$htmlTemp .= "		</ul>
													  </fieldset>
													" . $htmlSubQuestao . "
													<div class='" . $obsClass . "' id='div_obs_{$dados['qstid']}'>
													</div>";
										$htmlTemp .= "</div>";	    		
							    	}else{
							    		$htmlTemp = '';
							    	}
							    	$html = $html.$htmlTemp;
							    	$htmlTemp = null;
						    		$qstidUlt = $dados['qstid'];
						    	}
						    	
						    	if( $html != '' ){
							    	echo $html;
							    	$html = null;
						    		$escreveu = true;
						    	}else{
						    		$escreveu = $escreveu||false;
						    	}
						    }
						    if( !$escreveu ){
							    echo '<label style="color:red;">Essa vistoria não possui questões com anexo.</label>';
						    }
						    ?>
						  	</div>
                                                </div>
				    	</td>
				    	<td rowspan="2">
							<fieldset class="field_fotos" style="width:92%;height:500px;">
								<legend>Fotos da Galeria</legend>
								<ul id="fotos_galeria" class="div_fotos">
									<?php 
									$sql = "SELECT arqid FROM obras2.arquivosupervisao WHERE aqsstatus = 'A' AND sfndeid = $sfndeid ORDER BY arqid";
// 									$sql = "SELECT arqid FROM public.arquivo LIMIT 10";
									$arrFotosGaleria = $db->carregar($sql);
									?>
							<?php if(is_array($arrFotosGaleria)):?>
								<?php $n = 1 ?>
								<?php foreach($arrFotosGaleria as $foto): ?>
									<?php $pagina = floor($n/16); ?>
									
									<?php 
//										$sql = "SELECT arqtipo, arqid  FROM public.arquivo LIMIT 10";
//										$dados = $db->pegaLinha($sql);
										
										$imgend = APPRAIZ.'arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($foto['arqid']/1000) .'/'.$foto['arqid'];
										
										if(is_file($imgend)){
											$img_max_dimX = 100;
											$img_max_dimY = 85;
											
											$imginfo = getimagesize($imgend);
											
											$width = $imginfo[0];
											$height = $imginfo[1];
										
											if (($width >$img_max_dimX) or ($height>$img_max_dimY)){
												if ($width > $height){
												  	$w = $width * 0.9;
													  while ($w > $img_max_dimX){
														  $w = $w * 0.9;
													  }
													  $w = round($w);
													  $h = ($w * $height)/$width;
												  }else{
													  $h = $height * 0.9;
													  while ($h > $img_max_dimY){
														  $h = $h * 0.9;
													  }
													  $h = round($h);
													  $w = ($h * $width)/$height;
												  }
											}else{
												  $w = $width;
												  $h = $height;
											}
											
											$tamanho = " width=\"$w\" height=\"$h\" ";
										}else{
											$tamanho = "";
										}
										
										$testaSelecionado = testaSelecionado($foto['arqid'], $sfndeid);
									?>
									<li id="s_<?php echo $foto['arqid']?>" name="<?php echo $foto['arqid']?>" class="draggable originais <?=$testaSelecionado ?>">
										<?php if( true ){ ?>
										<img src="../imagens/fechar.jpeg" title="Remover Foto" class="fechar" style="display:none" onclick="removerFotoGaleria(this,0)" />
										<?php }else{?>
										<img src="../imagens/fechar_01.jpeg" title="Remover Foto" class="fechar" style="display:none"/>
										<?php }?>
										<img <?php echo $tamanho;?> class="img_class" ondblclick="abrirGaleria('<?php echo $foto['arqid'] ?>','<?php echo $pagina ?>',this)" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100&newheight=85" />
									</li>
									<?php $n++ ?>
								<?php endforeach; ?>
							<?php endif; ?>
						</ul>
					</fieldset>
				</td>
		    </tr>
	   	</table>

<script type="text/javascript">
function incluirFotos( sfndeid ){
	janela = window.open('?modulo=principal/popupFotosSupervisaoEmpresa&acao=A&sfndeid=' + sfndeid, 'inserirFotosSupervisao', 'menubar=no,location=no,resizable=no,scrollbars=no,status=yes,width=750,height=700' ); 
	janela.focus();
}
</script>
   	
<?php
function testaSelecionado( $arqid, $sfndeid ){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				CASE WHEN aqs.arqid IS NOT NULL OR ass.arqid IS NOT NULL THEN 'f_selected' ELSE '' END
			FROM
				obras2.arquivosupervisao ars
			INNER JOIN public.arquivo 				   		a 	ON a.arqid 	   = ars.arqid AND 
																   a.arqstatus = 'A'
																   
			LEFT JOIN obras2.questaosupervisao 				qs ON qs.sueid 	   = ars.sueid AND 
																  qs.qtsstatus = 'A'
																  
	    	LEFT JOIN obras2.arquivoquestaosupervisao 		aqs ON aqs.arqid 	 = ars.arqid AND 
	    														   aqs.aqsstatus = 'A' AND
	    														   aqs.qtsid 	 = qs.qtsid
	    														   
			LEFT JOIN obras2.respostasubquestao rsq ON rsq.qtsid 	 = qs.qtsid AND
													   rsq.rsqstatus = 'A'
													   
	    	LEFT JOIN obras2.arquivorespostasubquestao 	ass ON ass.arqid 	 = ars.arqid AND 
	    													   ass.arsstatus = 'A' AND
	    													   ass.rsqid 	 = rsq.rsqid
			WHERE
				(aqs.arqid IS NOT NULL OR ass.arqid IS NOT NULL) AND
				ars.aqsstatus = 'A' AND
				ars.sfndeid = {$sfndeid} AND
				ars.arqid = {$arqid}";
	$teste = $db->pegaUm($sql);
	
	return $teste;
}
?>   	


<?php
        $objObras = new Obras($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo '<script type="text/javascript">';
            echo " setTimeout(bloqueiaForm('formulario_supervisao_empresa'), 1500);
                   function bloqueiaForm(idForm){
                      jQuery(document).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                      jQuery('#gerarXls').attr('disabled', false);
                   }
                 ";
            echo '</script>';
        }
?>
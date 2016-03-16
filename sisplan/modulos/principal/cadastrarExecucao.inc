<?php
	if($_REQUEST['act'] == 'inserirExecucao'){
    	include "popupExecucao.inc";
    	die();
	}

	if($_REQUEST['act'] == 'salvar'){
		$modelo = new DespesaExecucao();

		$modelo->desabilitaRegistro($_REQUEST['uexid'],$_REQUEST['ptrid'],$_REQUEST['ptnid'],$_REQUEST['rfdid']);

		$_REQUEST['usucpf']			= $_SESSION['usucpf'];
		$_REQUEST['dpevalor']		= str_replace(',', '.', str_replace('.', '', $_REQUEST['dpevalor'] ));
		$_REQUEST['dpestatus']		= 'A';

		$modelo->popularDadosObjeto()->salvar();

		$modelo->commit();
	    echo "<script>
	    		alert('Operação realizada com sucesso');
	    		window.opener.location.href = window.opener.location.href;
	    		window.close();
	   		</script>";

	}

	include APPRAIZ."includes/cabecalho.inc";
	echo "<br>";
	monta_titulo($titulo_modulo,"");

	//guardando o id da unidade
	$uexid 					= $_REQUEST['uexid'];
	$modeloUnidadeExecutora = new UnidadeExecutora( $uexid );
	$dadosUnidade			= $modeloUnidadeExecutora->getDados();

	//recuperando todos os PTRES
	$modeloPtresDespesa = new PtresDespesa();
	$dadosPtres 		= $modeloPtresDespesa->getByWhere(array(),$uexid);

	$modeloNatureza		= new PtresNatureza();
	$modeloReferencia	= new ReferenciaDespesa();

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form method="POST"  name="formulario" id="formulario">
<input type="hidden" name="act" id="act" value="" />
<input type="hidden" name="uexid" id="uexid" value="<?php echo $uexid; ?>" />
	    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	        <tr>
	            <td align='right' class="SubTituloDireita" width="35%">
	            	Unidade
	            </td>
	            <td bgcolor="#EDEAEA">
					<?php echo $dadosUnidade['descricao']; ?>
	            </td>
	       	</tr>
	    <?php
	    if(!empty($dadosPtres) && is_array($dadosPtres)){
	    	foreach( $dadosPtres AS $chave => $dadoPtres){
	    	?>
		        <tr>
		            <td align='right' class="SubTituloDireita" width="35%">
		            	PTRES
		            </td>
		            <td bgcolor="#EDEAEA">
						<?php echo $dadoPtres['ptres']." - ".$dadoPtres['funcional']; ?>
		            </td>
		       	</tr>
		       	<tr>
		       		<td colspan="2" align="Center">
		       			<br>
		       			<div class="rolagem" align="center" style="width:1024px; overflow-x:auto; overflow-y: hidden;">
							<table id="tabelaIntervencoes" width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
				       			<?php
									$dadosNatureza = $modeloNatureza->getNaturezaByPtres($dadoPtres['ptrid']);
									if(!empty($dadosNatureza) && is_array($dadosNatureza)){
				       			?>
							            <thead>
							                <tr id="cabecalho">
							                    <td colspan="2" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
							                        <strong>Natureza</strong>
							                    </td>
				                    			<?php
													$dadosReferencia = $modeloReferencia->getByWhere(array(" rfdano = '".$_SESSION['exercicio']."' "));
													if(!empty($dadosReferencia) && is_array($dadosReferencia)){
														foreach($dadosReferencia as $dadoReferencia){ ?>
															<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
							                        			<strong><?php echo $dadoReferencia['rfdnome']; ?></strong>
							                    			</td>
												<?php	}
													}
												?>
							                    <td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
							                        <strong>Total</strong>
							                    </td>
							                </tr>
							            </thead>
							            <tbody>
					            		<?php
					            			$arTotalReferenciaProgramacao	= array();
					            			$arTotalReferenciaExecucao 		= array();
											$totalNaturezaProgramacao		= 0;
											$totalNaturezaExecucao			= 0;

											foreach($dadosNatureza as $dadoNatureza){
				            			?>
								            	<tr class="item">
								                    <td width="300px" align="left" nowrap="nowrap" >
								                    	<?php echo $dadoNatureza['natureza']; ?>
								                    </td>
								                    <td width="70px" align="right" nowrap="nowrap" >
					                            	<div>
					                            		<div>
															<div align="right" width:60px;">Programado</div>
					                            		</div>
					                            		<div >
															<div align="right" width:60px;">Executado</div>
					                            		</div>
					                            		<div >
															<div align="right" width:60px;">Saldo</div>
					                            		</div>
						                            </div>
								                    </td>
													<?php
														$dadosReferencia = $modeloReferencia->getByWhere3(array(" rfdano = '".$_SESSION['exercicio']."' "),$dadoPtres['ptrid'],$dadoNatureza['ptnid'],$uexid);
														if(!empty($dadosReferencia) && is_array($dadosReferencia)){
															$totalNaturezaProgramacao	= 0;
															$totalNaturezaExecucao		= 0;
															foreach($dadosReferencia as $dadoReferencia){ ?>
									                            <td width="110px" align="center" nowrap="nowrap" class="celula" <?php if($dadoReferencia['habilitado'] == 'S' || $dadoPtres['habilitado'] == 'S'){ ?> title="Clique para alterar" style="cursor:pointer;"  onclick="javascript: abrepopup('<?php echo $dadoPtres['ptrid']; ?>','<?php echo $dadoNatureza['ptnid']; ?>','<?php echo $dadoReferencia['rfdid']; ?>');" <?php }else{ ?>title="Bloqueado"<?php } ?> >
									                            	<div>
									                            		<div>
																			<div align="right" style="float:right; width:100px;"><?php echo formata_valor( $dadoReferencia['dpgvalor'] ); ?></div>
									                            		</div>
									                            		<br>
									                            		<div >
																			<div align="right" style="float:right; width:100px;"><?php echo formata_valor( $dadoReferencia['dpevalor'] ); ?></div>
									                            		</div>
									                            		<br>
									                            		<div >
																			<div align="right" style="float:right; width:100px;"><?php echo formata_valor( $dadoReferencia['dpgvalor'] - $dadoReferencia['dpevalor'] ); ?></div>
									                            		</div>
									                            	</div>
									                            </td>
													<?php	$totalNaturezaProgramacao = $totalNaturezaProgramacao + $dadoReferencia['dpgvalor'];
														    $totalNaturezaExecucao    = $totalNaturezaExecucao + $dadoReferencia['dpevalor'];

															$arTotalReferenciaProgramacao[$dadoReferencia['rfdid']] = $arTotalReferenciaProgramacao[$dadoReferencia['rfdid']] + $dadoReferencia['dpgvalor'];
															$arTotalReferenciaExecucao[$dadoReferencia['rfdid']]    = $arTotalReferenciaExecucao[$dadoReferencia['rfdid']] + $dadoReferencia['dpevalor'];
															} ?>
															<td align="center" nowrap="nowrap" width="120px">
							                            		<div>
								                            		<div>
																		<div align="right" style="float:right; width:110px;"><?php echo formata_valor( $totalNaturezaProgramacao ); ?></div>
								                            		</div>
								                            		<br>
								                            		<div >
																		<div align="right" style="float:right; width:110px;"><?php echo formata_valor( $totalNaturezaExecucao ); ?></div>
								                            		</div>
								                            		<br>
								                            		<div >
																		<div align="right" style="float:right; width:110px;"><?php echo formata_valor( $totalNaturezaProgramacao - $totalNaturezaExecucao ); ?></div>
								                            		</div>
									                            </div>
									                        </td>
												<?php }
													?>
								            </tr>
				            			<?php
											}
					            		?>
			            			   <tr class="item">
						                    <td align="right" colspan="1"> <strong>Total:</strong> </td>
						                    <td align="right" colspan="1" width="0%">
				                            	<div>
				                            		<div>
														<div align="right" width:60px;">Programado</div>
				                            		</div>
				                            		<div >
														<div align="right" width:60px;">Executado</div>
				                            		</div>
				                            		<div >
														<div align="right" width:60px;">Saldo</div>
				                            		</div>
					                            </div>
						                    </td>
											<?php
												$dadosReferencia = $modeloReferencia->getByWhere(array(" rfdano = '".$_SESSION['exercicio']."' "));
												if(!empty($dadosReferencia) && is_array($dadosReferencia)){
													foreach($dadosReferencia as $dadoReferencia){ ?>
							                            <td align="center" nowrap="nowrap" width="110px">
					                                		<div>
							                            		<div>
																	<div align="right" style="float:right; width:100px;"><?php echo formata_valor( $arTotalReferenciaProgramacao[$dadoReferencia['rfdid']] ); ?></div>
							                            		</div>
							                            		<br>
							                            		<div >
																	<div align="right" style="float:right; width:100px;"><?php echo formata_valor( $arTotalReferenciaExecucao[$dadoReferencia['rfdid']] ); ?></div>
							                            		</div>
							                            		<br>
							                            		<div >
																	<div align="right" style="float:right; width:100px;"><?php echo formata_valor( $arTotalReferenciaProgramacao[$dadoReferencia['rfdid']] - $arTotalReferenciaExecucao[$dadoReferencia['rfdid']] ); ?></div>
							                            		</div>
							                            	</div>
							                            </td>
											<?php	}
												}
											?>
						            	</tr>
							            </tbody>
			       			<?php }else{ ?>
			       				<tr>
			       					<td align="center"> Nenhuma natureza encontrada </td>
			       				</tr>
							<?php } ?>
					    </table>
      	       	    <br>
					</div>
					<br>
		       		</td>
		       	</tr>
	    <?php } ?>
			  <tr bgcolor="#CCCCCC">
				   <td></td>
				   <td>
					   <input type="button" name="btnVoltar" value='Voltar' onclick="javascript: location.href = 'sisplan.php?modulo=principal/execucao&acao=A';" class="botao">
				   </td>
			  </tr>
	    <?php }else{ ?>
       		<tr>
       			<td align="center" colspan="2">
       			<br>
       			<strong>Nenhum PTRES encontrado</strong>
       			<br>
       			<br>
       			<input type="button" name="btnVoltar" value='Voltar' onclick="javascript: location.href = 'sisplan.php?modulo=principal/execucao&acao=A';" class="botao">
       			</td>
       		</tr>
  <?php }
	    ?>
    	</table>
</form>


<script type="text/javascript">

	$(document).ready(function() {
	    adicionarCorLinha();

	    //Deixando a div do tamanho da tela
	    var tamanho = $(window).width() - ( $(window).width() * 0.08 );
	    $(".rolagem").css("width", tamanho);

	});

	function calculaTotal(elemento){

		elemento 		= $(elemento);

		var arrayDados	= (elemento.attr('id')).split("_");
		var ptres 		= arrayDados[1];
		var natureza	= arrayDados[2];
		var referencia	= arrayDados[3];

		var somatorioColuna = new Number(0.00);
		var somatorioLinha  = new Number(0.00);

		//Calculando o total da coluna
	    $('[id^=ref_'+ptres+'_][id$=_'+referencia+']').each(function(index, elem){
	        var valorTemp      = $(elem).val();
	        valorTemp 		   = replaceAll(valorTemp,'.','');
	        var valorFormatado = new Number(valorTemp.replace(",",".") );
	        somatorioColuna    = somatorioColuna + valorFormatado;
	    });
	    //Atibuindo o total de coluna no campo
		$('#refTotal_'+ptres+'_'+referencia).val(MascaraMonetario(new String(somatorioColuna)));

	    //Calculando o total da Linha
	    $('[id^=ref_'+ptres+'_'+natureza+'_]').each(function(index, elem){
	        var valorTemp      = $(elem).val();
	        valorTemp 		   = replaceAll(valorTemp,'.','');
	        var valorFormatado = new Number(valorTemp.replace(",",".") );
	        somatorioLinha     = somatorioLinha + valorFormatado;
	    });
	    //Atibuindo o total de Linha no campo
		$('#naturezaTotal_'+ptres+'_'+natureza).val(MascaraMonetario(new String(somatorioLinha)));
	}

	function abrepopup(ptrid,ptnid,rfdid){

		var uexid = $('#uexid').val();

	    return windowOpen( '?modulo=principal/cadastrarExecucao&acao=A&act=inserirExecucao&uexid='+uexid+'&ptrid='+ptrid+'&ptnid='+ptnid+'&rfdid='+rfdid,'blank','height=450,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
	}

	function adicionarCorLinha(){

	    $('[class^=item]').mouseover(function(){
	        $(this).css('background-color', '#FFFFCC');
	    });

	    $('[class^=item]').mouseout(function(){
	        $(this).css('background-color', '#F5F5F5');
	    });

	}

	function salvar(){
		var retorno = true;

	    $('input[id^="ref_"]').each(function(index, elem){
            if($(elem).val() == ''){
                alert('Todos os campos devem ser preenchidos.');
                $(elem).focus();
                retorno = false;
                return retorno;
            }
	    });

	    if(retorno){
			$('#act').val('salvar');
			$('#formulario').submit();
	    }
	}

	function replaceAll(string, token, newtoken) {
	    while (string.indexOf(token) != -1) {
	        string = string.replace(token, newtoken);
	    }
	    return string;
	}
</script>
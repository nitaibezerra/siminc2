<?php
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

	if($_REQUEST['act'] == 'salvar'){

		$modeloDespesaProgramacao = new DespesaProgramacao();
		$modeloDespesaProgramacao->desabilitaPorUnidade($uexid);

		foreach($_REQUEST['ref'] as $ptrid => $naturezas){
			foreach($naturezas as $ptnid => $referencias){
				foreach($referencias as $rfdid => $valor){

					$arDados 				= array();
					$arDados['uexid'] 		= $uexid;
					$arDados['ptrid'] 		= $ptrid;
					$arDados['ptnid'] 		= $ptnid;
					$arDados['rfdid'] 		= $rfdid;
					$arDados['usucpf'] 		= $_SESSION['usucpf'];
					$arDados['dpgvalor']	= str_replace(',', '.', str_replace('.', '', $valor ));
					$arDados['dpgstatus']	= 'A';

		            $modeloDespesaProgramacao->popularDadosObjeto($arDados)->salvar();
		            $modeloDespesaProgramacao->setDadosNull();
				}
			}

		}

	    $modeloDespesaProgramacao->commit();
	    $db->sucesso($modulo);
	    die();
	}
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form method="POST"  name="formulario" id="formulario">
<input type="hidden" name="act" id="act" value="" />
<input type="hidden" name="uexid" id="uexid" value="<?php echo $uexid; ?>" />
	    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
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
		       		<td colspan="2">
		       			<br>
		       			<div class="rolagem" align="center" style="width:1024px; overflow-x:auto; overflow-y: hidden;">
							<table id="tabelaIntervencoes" width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
				       			<?php
									$dadosNatureza = $modeloNatureza->getNaturezaByPtres($dadoPtres['ptrid']);
									if(!empty($dadosNatureza) && is_array($dadosNatureza)){
				       			?>
							            <thead>
							                <tr id="cabecalho">
							                    <td width="35%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
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
					            			$arTotalReferencia = array();
											foreach($dadosNatureza as $dadoNatureza){
				            			?>
								            	<tr class="item">
								                    <td> <?php echo $dadoNatureza['natureza']; ?> </td>
													<?php
														$dadosReferencia = $modeloReferencia->getByWhere2(array(" rfdano = '".$_SESSION['exercicio']."' "),$dadoPtres['ptrid'],$dadoNatureza['ptnid'],$uexid);
														if(!empty($dadosReferencia) && is_array($dadosReferencia)){
															$totalNatureza = 0;
															foreach($dadosReferencia as $dadoReferencia){ ?>
									                            <td align="center" nowrap="nowrap">
									                                <?php echo campo_texto( "ref[".$dadoPtres['ptrid']."][".$dadoNatureza['ptnid']."][".$dadoReferencia['rfdid']."]", 'S', ($dadoPtres['habilitado'] == 'S' || $dadoReferencia['habilitado'] == 'S' ) ? 'S' : 'N', '', ($dadoPtres['habilitado'] == 'S' || $dadoReferencia['habilitado'] == 'S') ? 13 : 17, 15, '###.###.###.###,##', '', 'left', '', 0, "id='ref_".$dadoPtres['ptrid']."_".$dadoNatureza['ptnid']."_".$dadoReferencia['rfdid']."'","calculaTotal(this);",$dadoReferencia['dpgvalor'] ? formata_valor($dadoReferencia['dpgvalor']) : '0,00'); ?>
									                            </td>
													<?php	$totalNatureza = $totalNatureza + $dadoReferencia['dpgvalor'];
															$arTotalReferencia[$dadoReferencia['rfdid']] = $arTotalReferencia[$dadoReferencia['rfdid']] + $dadoReferencia['dpgvalor'];
															} ?>
															<td align="center" nowrap="nowrap">
									                             <?php echo campo_texto( "naturezaTotal[".$dadoPtres['ptrid']."][".$dadoNatureza['ptnid']."]", 'S', 'N', '', 17, 20, '###.###.###.###,##', '', 'left', '', 0, "id='naturezaTotal_".$dadoPtres['ptrid']."_".$dadoNatureza['ptnid']."'","", !empty($totalNatureza) ? formata_valor($totalNatureza) : "0,00"); ?>
									                        </td>
												<?php }
													?>
								            	</tr>
				            			<?php
											}
					            		?>
			            			   <tr>
						                    <td align="right"> <strong>Total:</strong> </td>
											<?php
												$dadosReferencia = $modeloReferencia->getByWhere(array(" rfdano = '".$_SESSION['exercicio']."' "));
												if(!empty($dadosReferencia) && is_array($dadosReferencia)){
													foreach($dadosReferencia as $dadoReferencia){ ?>
							                            <td align="center" nowrap="nowrap">
							                                <?php echo campo_texto( "referenciaTotal[".$dadoPtres['ptrid']."][".$dadoReferencia['rfdid']."]", 'S', 'N', '', 17, 20, '###.###.###.###,##', '', 'left', '', 0, "id='refTotal_".$dadoPtres['ptrid']."_".$dadoReferencia['rfdid']."'","",!empty($arTotalReferencia[$dadoReferencia['rfdid']]) ? formata_valor($arTotalReferencia[$dadoReferencia['rfdid']]) : "0,00"); ?>
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
					    </div>
				       	<br>
		       		</td>
		       	</tr>
	    <?php } ?>
			  <tr bgcolor="#CCCCCC">
				   <td></td>
				   <td>
					   <input type="button" name="btnSalvar" value='Salvar' onclick="salvar()" class="botao">
					   <input type="button" name="btnVoltar" value='Voltar' onclick="javascript: location.href = 'sisplan.php?modulo=principal/programacao&acao=A';" class="botao">
				   </td>
			  </tr>
	    <?php }else{ ?>
       		<tr>
       			<td align="center" colspan="2">
       			<br>
       			<strong>Nenhum PTRES encontrado</strong>
       			<br>
       			<br>
       			<input type="button" name="btnVoltar" value='Voltar' onclick="javascript: location.href = 'sisplan.php?modulo=principal/programacao&acao=A';" class="botao">
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
<?php

/*********************************** INCLUDES ***************************************/
include_once(APPRAIZ.'www/cte/monitoraFinanceiro/includes/constantes.php');
include_once(APPRAIZ.'cte/modulos/principal/monitoraFinanceiro/funcoes.inc');
include_once(APPRAIZ.'cte/classes/HistoricoItemExecutado.class.inc');
include_once(APPRAIZ.'cte/classes/HistoricoMonitoramentoConvenio.class.inc');
include_once(APPRAIZ.'cte/classes/HistoricoConvItemComposicao.class.inc');

/************************* INSTANCIA CLASSES E OBJETOS ******************************/
$historicoItemExecutado 	 = new HistoricoItemExecutado();
$dadosMonitoramentos 		 = new HistoricoMonitoramentoConvenio();
$historicoConvItemComposicao = new HistoricoConvItemComposicao();

/******************************* SALVA NOVO ITEM ************************************/
if($_POST['hmsid']){
	
	$dadosMonitoramentos->carregarPorId($_REQUEST['hmcid']);

	$historicoItemExecutado->hieid 				= null;
	$historicoItemExecutado->hmsid 				= $_POST['hmsid'];
	$historicoItemExecutado->sbaid 				= $_POST['sbaid'];
	$historicoItemExecutado->hievalorunitario 	= desformata_valor($_POST['hievalorunitario']);
	$historicoItemExecutado->hieqtd 			= desformata_valor($_POST['hieqtd']);
	$historicoItemExecutado->hievalortotal 		= desformata_valor($_POST['hievalortotal']);
	$historicoItemExecutado->hiejustificativa 	= $_POST['hiejustificativa'];
	$historicoItemExecutado->hiedata 			= $dadosMonitoramentos->hmcdatamonitoramento;
	$historicoItemExecutado->hiedescricao 		= $_POST['hiedescricao'];
	$historicoItemExecutado->unddid 			= $_POST['unddid'];
	
	$hieid = $historicoItemExecutado->salvar();
	$historicoItemExecutado->commit();

	if($hieid){
		
		$historicoConvItemComposicao->hciid  = $_POST['hciid'];
		$historicoConvItemComposicao->hmsid  = $_POST['hmsid'];
		$historicoConvItemComposicao->hieid  = $hieid;
		$historicoConvItemComposicao->hmsano = $_POST['ano'];	
		$historicoConvItemComposicao->salvar();
		
		if($historicoConvItemComposicao->commit()){
			echo '<script> 
					alert( "Operação realizada com sucesso. " );
					parent.closeItensNew();
			  	  </script>';	
		}
				
	}
}

?>
<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../../cte/monitoraFinanceiro/includes/monitora_financeiro.css"/>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
		<script type="text/javascript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="../../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript">
										
				jQuery.noConflict();

				jQuery(document).ready(function(){

//					jQuery(window).unload(function() {
//						window.close();
//					}
					
					jQuery('#formulario').submit(function(){
						
						if(!jQuery('select[name=unddid]').val()){
							alert('O campo unidade de medida é obrigatório.');
							jQuery('select[name=unddid]').focus();
							return false;
						}
						
						if(!jQuery('#hiedescricao').val()){
							alert('O campo descrição do item é obrigatório.');
							jQuery('#hiedescricao').focus();
							return false;
						}

						if(!jQuery('#hieqtd').val()){
							alert('O campo quantidade é obrigatório.');
							jQuery('#hieqtd').focus();
							return false;
						}

						if(!jQuery('#hievalorunitario').val()){
							alert('O campo valor unitário é obrigatório.');
							jQuery('#hievalorunitario').focus();
							return false;
						}

						if(!jQuery('#hiejustificativa').val()){
							alert('O campo justificativa é obrigatório.');
							jQuery('#hiejustificativa').focus();
							return false;
						}
						
					});		
				});

				function atualizarTotal( obj ){
					
					var hieqtd = document.getElementById('hieqtd').value;
					var hievalorunitario = document.getElementById('hievalorunitario').value;
					var disponivel = <?php echo $_GET['disponivel']; ?>;
	
					if(hievalorunitario && hieqtd){
						total = replaceAll(hievalorunitario, '.', '').replace(',','.')*replaceAll(hieqtd, '.', '').replace(',','.');
					}else{
						total = '00.00';
					}

					if(total > disponivel){
						
						alert('O valor total do item não pode ultrapassar o valor disponível.');
						
						document.getElementById('hieqtd').value = '';
						document.getElementById('hievalorunitario').value = '';
						document.getElementById('hievalortotal').value = '00,00';
						
						return false;
					}
						
					masktotal = mascaraglobal('##.###.###,##', total.toFixed(2));				
					document.getElementById('hievalortotal').value = masktotal;
					
					restante = disponivel-total;
					maskrestante = mascaraglobal('##.###.###,##', restante.toFixed(2));								
					document.getElementById('td_disponivel').innerHTML = maskrestante; 		
				}
				
			</script>	
	</head>
	<body>
		<form name="formulario" id="formulario" action="" method="post">
			<input type="hidden" name="sbaid" value="<?php echo $_GET['sbaid'] ?>" />
			<input type="hidden" name="hmsid" value="<?php echo $_GET['hmsid'] ?>" />
			<input type="hidden" name="ano" value="<?php echo $_GET['ano'] ?>" />
			<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
				<tr>
					<th class="tdDegradde02" colspan="2" >Adicionar Item de Monitoramento</th>
				</tr>
				<tr>
					<td class="subtitulodireita">Unidade de medida</td>
					<td>
						<?php						
						$sql = "SELECT 
									unddid as codigo, 
									undddsc as descricao 
								FROM cte.unidademedidadetalhamento umd 
								ORDER BY undddsc";

						$db->monta_combo('unddid', $sql, 'S', 'Selecione...', '', ''); 
						?>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Descrição do item</td>
					<td><?php echo campo_texto( "hiedescricao", 'S', 'S', '', '30', '', '', '', '', '', '', 'id="hiedescricao" onchange="atualizarTotal( this );"', '', '' ); ?></td>
				</tr>
				<tr>
					<td class="subtitulodireita">Quantidade do item</td>
					<td><?php echo campo_texto( "hieqtd", 'S', 'S', '', '10', '', '#########', '', '', '', '', 'id="hieqtd" onchange="atualizarTotal( this );"', '', '' ); ?></td>
				</tr>
				<tr>
					<td class="subtitulodireita">Valor do item</td>
					<td><?php echo campo_texto( "hievalorunitario", 'S', 'S', '', '30', '', '##.###.###,##', '', '', '', '', 'id="hievalorunitario" onchange="atualizarTotal( this );"', '', '' ); ?></td>
				</tr>
				<tr>
					<td class="subtitulodireita">Valor do total</td>
					<td><?php echo campo_texto( "hievalortotal", 'N', 'N', '', '30', '', '##.###.###,##', '', '', '', '', 'id="hievalortotal"', '', '00,00' ); ?>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Valor disponível</td>
					<td id="td_disponivel"><?php echo formata_valor($_GET['disponivel']) ?></td>
				</tr>				
				<tr>
					<td class="subtitulodireita">Justificativa</td>
					<td><?php echo campo_textarea('hiejustificativa', 'S', 'S', '', 34, 6, ''); ?></td>
				</tr>
				<tr>
					<th class="tdDegradde02" colspan="2" align="center">
						<input type="submit" value="Cadastrar" />
					</th>
				</tr>
			</table>
		</form>
	</body>
</html>
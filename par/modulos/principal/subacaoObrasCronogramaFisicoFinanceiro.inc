<?php
$oSubacaoControle = new SubacaoControle();
$oPreObraControle = new PreObraControle();

$PreC = new PreCronograma();

$arrItens['item_quinzena_1'] = !$_POST['item_quinzena_1'] ? $PreC->carregaPreCronogramaPorQuinzena($preid,1) : $_POST['item_quinzena_1'];
$arrItens['item_quinzena_2'] = !$_POST['item_quinzena_2'] ? $PreC->carregaPreCronogramaPorQuinzena($preid,2) : $_POST['item_quinzena_2'];

$tipoObra = $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
$tipoFundacao = $oSubacaoControle->verificaTipoFundacao($preid);
$arItensComposicao = $oSubacaoControle->recuperarItensComposicaoCronograma($tipoObra, $preid, true , $tipoFundacao);
$arItensComposicao = $arItensComposicao ? $arItensComposicao : array();
//$nrTotal = $oSubacaoControle->recuperarValorTotalItensComposicaoCronograma($tipoObra, $preid, true, $tipoFundacao);

if( ($_POST['item_quinzena_1'] || $_POST['item_quinzena_2']) && $boAtivo == 'S' ):
	$PreC->salvaCronogramaPorQuinzena($preid);
	echo '<script type="text/javascript"> alert("Operação realizada com sucesso.");</script>';
endif;

?>
<style>
	.marcado{background-color:#228B22;color:#FFFFFF;}
	.desmarcado{background-color:#E0EEEE;}
	.tabela_quinzena td{border:solid 1px black;height:15px;}
	.tabela_quinzena{width:100%}
	
</style>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">
<!--
jQuery.noConflict();

jQuery(document).ready(function(){	

	jQuery('.enviar').click(function(){
		
		if(verificaFormulario() == false){
			alert('É necessário informar pelo menos uma quinzena para cada Ítem!');
			return false;
		}
		
		if(this.value == 'Salvar'){
			jQuery('#acao').val('salvar');
		}

		if(this.value == 'Salvar e próximo'){			
			jQuery('#acao').val('proximo');
		}

		if(this.value == 'Salvar e anterior'){			
			jQuery('#acao').val('anterior');
		}		

		document.formulario.submit();
	});

	jQuery('.navegar').click(function(){

		if(this.value == 'Próximo'){
			aba = 'documento';
		}

		if(this.value == 'Anterior'){
			aba = 'planilhaOrcamentaria';
		}

		document.location.href = 'par.php?modulo=principal/subacaoObras&acao=A&tipoAba='+aba+'&preid='+<?php echo $preid ?>;
	});
});

function marcar(obj,itcid,mes,quinzena){
	if( jQuery(obj).attr("class") == "desmarcado" ){
		jQuery(obj).attr("class","marcado");
		jQuery("#item_" + itcid + "_mes_" + mes + "_quinzena_" + quinzena).val("1");
	}else{
		jQuery(obj).attr("class","desmarcado");
		jQuery("#item_" + itcid + "_mes_" + mes + "_quinzena_" + quinzena).val("");
	}
}

//-->
</script>
<?php if(!empty($arItensComposicao) && $arItensComposicao[0]): ?>
	<form name="formulario" action="" method="post">
	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="0" class="listagem">
		<thead>
			<tr>
				<td rowspan="2" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ordem</strong></td>
				<td rowspan="2" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
				<?php $qtdMeses = 9 ?>
				<?php for($i = 1 ; $i <= $qtdMeses ; $i++ ): ?>
					<td colspan="2" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Mês <?php echo $i ?></strong></td>
				<?php endfor; ?>
				<td rowspan="2" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor do Item (R$)</strong></td>
				<td rowspan="2" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>(%) Referente a Obra <br/> (A)</strong></td>
			</tr>
			<tr>
				<?php for($i = 1 ; $i <= $qtdMeses ; $i++ ): ?>
					<td style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="center" class="title" ><strong>Q1</strong></td>
					<td style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="center" class="title" ><strong>Q2</strong></td>
				<?php endfor; ?>
			</tr>
		</thead>
		<tbody>
		<?php $x = 0 ?>		
		<?php foreach($arItensComposicao as $item): ?>
			<?php 
			$cor = ($x % 2) ? "#F7F7F7" : "white";
			?>
			<tr bgcolor="<?php echo $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor ?>';">
				<td align="center"><?php echo $item['itcordem'] ?></td>
				<td><?php echo ucwords($item['itcdescricao']) ?></td>
				<?php for($i = 1 ; $i <= $qtdMeses ; $i++ ): ?>
					<td colspan="2" style="cursor:pointer">
						<table class="tabela_quinzena" cellspacing="1" cellpadding="0" >
							<tr>
								<td class="<?php echo $arrItens['item_quinzena_1'][ $item['itcid'] ][ $i ] ? "marcado" : "desmarcado" ?>" title="Clique aqui para Marcar / Desmarcar a Quinzena 1 do Mês <?php echo $i ?>" onclick="<?php echo $boAtivo == 'S' ? "marcar(this,'".$item['itcid']."','".$i."','1')" : ''; ?>" align="center"><input value="<?php echo $arrItens['item_quinzena_1'][ $item['itcid'] ][ $i ] ? "1" : "" ?>"  type="hidden" id="item_<?php echo $item['itcid'] ?>_mes_<?php echo $i ?>_quinzena_1" name="item_quinzena_1[<?php echo $item['itcid'] ?>][<?php echo $i ?>]" ></td>
								<td class="<?php echo $arrItens['item_quinzena_2'][ $item['itcid'] ][ $i ] ? "marcado" : "desmarcado" ?>" title="Clique aqui para Marcar / Desmarcar a Quinzena 2 do Mês <?php echo $i ?>" onclick="<?php echo $boAtivo == 'S' ? "marcar(this,'".$item['itcid']."','".$i."','2')" : ''; ?>" align="center"><input value="<?php echo $arrItens['item_quinzena_2'][ $item['itcid'] ][ $i ] ? "1" : "" ?>"  type="hidden" id="item_<?php echo $item['itcid'] ?>_mes_<?php echo $i ?>_quinzena_2" name="item_quinzena_2[<?php echo $item['itcid'] ?>][<?php echo $i ?>]" ></td>
							</tr>
						</table>
					</td>
				<?php endfor; ?>	
				<td align="right">
					<?php $valor = $PreC->getPreItensComposicaoFilhos($item['itccodigoitem'],$preid,0,$tipoObra) ?>
					<span title="Valor Unitário * Quantidade do Ítem"><?php echo formata_valor( $valor ) ?><span>
					<?php $total += $valor ?>
					<?php $arrValores[$item['itcid']] = $valor; ?>
				</td>
				<td align="right" id="td_percent_<?php echo $item['itcid'] ?>" >
				</td>
			</tr>
			<?php $x++ ?>
		<?php endforeach; ?>
		</tbody>
	<tfoot>
		<tr bgcolor="#f0f0f0" height="30" >
			<td align="right" colspan="<?php echo 2 + ($qtdMeses * 2) ?>"><strong>Total:</strong></td>
			<td align="right"><strong><?php echo formata_valor($total) ?></strong></td>
			<td align="right"><strong><?php echo ($total > 0) ? '100%' : '' ?></strong></td>
		</tr>		
	</tfoot>				
	</table>
	<?php endif; ?>
<?php $arrValores = !$arrValores ? array() : $arrValores ?>
<script>
<?php foreach($arrValores as $itcid => $valor): ?>
	jQuery("#td_percent_<?php echo $itcid ?>").html("<?php echo formata_valor(round( (( !$valor || !$total ? 0 : $valor/$total ) * 100),2)) ?>");
<?php endforeach; ?>

function verificaFormulario(){
<?php foreach($arrValores as $itcid => $valor): ?>
		var valor_item_<?php echo $itcid ?> = 0;
		<?php for($i = 1; $i <= $qtdMeses; $i++) :?>
		if(jQuery("#item_<?php echo $itcid ?>_mes_<?php echo $i ?>_quinzena_1").val() == "1" || jQuery("#item_<?php echo $itcid ?>_mes_<?php echo $i ?>_quinzena_2").val()  == "1"){
			valor_item_<?php echo $itcid ?> += 1;
		}
		<?php endfor; ?>
	if(valor_item_<?php echo $itcid ?> == 0 && jQuery("#td_percent_<?php echo $itcid ?>").html() != "0,00"){
		return false;
	}
<?php endforeach; ?>
	return true;
}

</script>

<?php if(!empty($arItensComposicao) && $arItensComposicao[0]): ?>
	<table width="95%" align="center" bgcolor="#DCDCDC">
		<tr>
			<td align="left">
				
			</td>
			<td align="center">	
				<?php
				if($boAtivo == 'S' && $habil == 'S'){
				?>
					<input class="enviar" type="button" value="Salvar" <?php echo $stAtivo ?>>
					<input type="hidden" name="acao" id="acao" value="">
				<?php 
				} 
				?>							
			</td>
			<td align="right">
				
			</td>
		</tr>
	</table>
<?php else: ?>				
	<table width="95%" align="center" bgcolor="#DCDCDC">
		<tr>
			<td align="center">
				Não existem registros!
			</td>
		</tr>
	</table>
<?php endif; ?>
</form>
<?php
echo carregaAbasItensComposicao("par.php?modulo=principal/popupItensComposicao&acao=A&tipoAba=cronograma&icoid=".$_REQUEST['icoid'], $_REQUEST['icoid'],$descricaoItem);
monta_titulo( 'Cronograma Físico-Financeiro', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );

$oSubacaoControle = new SubacaoControle();

if($_POST['preid'] && $_POST['itcid']){
	$count = count($_POST['preid']);	
	for($x=0;$x<$count;$x++){
		if($_POST['icodtinicioitem_'.$x] != '' || $_POST['icodterminoitem_'.$x] != ''){			
			$arDados['icoid'] 				= $_POST['icocod'][$x];
			$arDados['preid'] 				= $_POST['preid'][$x];
			$arDados['icopercprojperiodo']  = $_POST['icopercprojperiodo'][$x];
			$arDados['icopercexecutado'] 	= $_POST['icopercexecutado'][$x];
			$arDados['icoordem'] 			= $_POST['icoordem'][$x];
			$arDados['icodtinicioitem']		= formata_data_sql($_POST['icodtinicioitem_'.$x]);
			$arDados['icodterminoitem']		= formata_data_sql($_POST['icodterminoitem_'.$x]);
			$arDados['icostatus'] 			= 'A';
			$arDados['icodtinclusao'] 		= date('Y-m-d H:i:s');
			$arDados['itcid'] 				= $_POST['itcid'][$x]; 
			
			$oSubacaoControle->salvarDadosCronogramaFisicoFinanceiro($arDados);
		}
	}
	echo '<script type="text/javascript"> 
			alert("Operação realizada com sucesso.");
			document.location.href = \''.$_SERVER['HTTP_REFERER'].'\';
		  </script>';
	exit;
}

$tipoObra = $oSubacaoControle->verificaTipoObra($_GET['icoid']);
$arItensComposicao = $oSubacaoControle->recuperarItensComposicaoCronograma($tipoObra, $_GET['icoid']);
$arItensComposicao = $arItensComposicao ? $arItensComposicao : array();
//ver($arItensComposicao);

$nrTotal = $oSubacaoControle->recuperarValorTotalItensComposicaoCronograma($tipoObra, $_GET['icoid']);
?>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form action="" method="post">
<table width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
	<thead>
		<tr>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ordem</strong></td>
			<!-- td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td -->
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data de Início</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data de Término</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor do Item (R$)</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>(%) Referente a Obra <br/> (A)</strong></td>
			<!-- td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>(%) Executado do Item Sobre a Obra <br/> (B)</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>(%) do Item Executado <br/> (B x 100 / A)</strong></td -->
		</tr>
	</thead>
	<tbody>
	<?php if(!empty($arItensComposicao) && $arItensComposicao[0]): ?>
		<?php $x = 0 ?>
		<?php foreach($arItensComposicao as $item): ?>
			<?php 
			$cor = ($x % 2) ? '#f0f0f0' : 'white';
			?>
			<tr style="background:<?php echo $cor ?>">
				<td align="center"><?php echo $item['itcordem'] ?></td>
				<!-- td></td -->
				<td><?php echo $item['itcdescricao'] ?></td>
				<td align="center">
					<!-- input type="text" name="icodtinicioitem[]" value="<?php echo formata_data($item['icodtinicioitem']) ?>" size="12"><img src="../imagens/obrig.gif" border="0" -->
					<?php $icodtinicioitem = formata_data($item['icodtinicioitem']) ?>
					<?php echo campo_data2('icodtinicioitem_'.$x, 'S', 'S', $label, $formata, '', '', $icodtinicioitem) ?>
				</td>
				<td align="center">
					<!-- input type="text" name="icodterminoitem[]" value="<?php echo formata_data($item['icodterminoitem']) ?>" size="12"><img src="../imagens/obrig.gif" border="0" -->
					<?php $icodterminoitem = formata_data($item['icodterminoitem']) ?>
					<?php echo campo_data2('icodterminoitem_'.$x, 'S', 'S', $label, $formata, '', '', $icodterminoitem) ?>
				</td>
				<td align="right">
					<input type="hidden" name="icocod[]" value="<?php echo $item['icoid']?>">
					<input type="hidden" name="preid[]" value="<?php echo $item['preid']?>">
					<input type="hidden" name="itcid[]" value="<?php echo $item['itcid']?>">
					<?php echo formata_valor($item['valor']) ?>
				</td>
				<td align="right">
					<?php
					if($item['valor']){
						$porcento = ($item['valor']*100)/$nrTotal;
						echo round($porcento, 2)."%";
					}
					?>
				</td>
				<!-- td align="right">100%</td>
				<td align="right">100%</td -->
			</tr>
			<?php $x++ ?>
		<?php endforeach; ?>		
	<?php endif; ?>
	</tbody>
	<tfoot>
		<tr height="30" class="title">
			<td class="SubTituloEsquerda" align="left" colspan="4">Total:</td>
			<td class="SubTituloDireita" align="right"><?php echo formata_valor($nrTotal) ?></td>
			<td class="SubTituloDireita" align="right"><?php echo ($nrTotal > 0) ? '100%' : '' ?></td>
		</tr>		
		<tr height="30">
			<td align="left" colspan="6" bgcolor="#e9e9e9">						
				<?php if(!empty($arItensComposicao) && $arItensComposicao[0]): ?>
					<input type="submit" value="Salvar">
				<?php else: ?>				
					<center><p>Não existem registros.</p></center>
				<?php endif; ?>
			</td>
		</tr>
	</tfoot>				
</table>
</form>
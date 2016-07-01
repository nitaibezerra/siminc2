<?php 
echo carregaAbasItensComposicao("par.php?modulo=principal/popupItensComposicao&acao=A&tipoAba=planilhaOrcamentaria&icoid=".$_REQUEST['icoid'], $_REQUEST['icoid'],$descricaoItem);
monta_titulo( 'Planilha Orçamentária', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );

$oSubacaoControle = new SubacaoControle();

//

if($_POST['ppoqtditem']){
	$count = count($_POST['ppoqtditem']);	
	for($x=0;$x<$count;$x++){
		if($_POST['ppoqtditem'][$x] != ''){
			$arDados['ppoid'] = $_POST['ppoid'][$x];
			$arDados['preid'] = $_POST['preid'][$x];
			$arDados['itcid'] = $_POST['itcid'][$x];
			$arDados['ppoqtditem'] = $_POST['ppoqtditem'][$x];
			
			$oSubacaoControle->salvarDadosPlanilhaOrcamentaria($arDados);
		}
	}
	echo '<script type="text/javascript"> 
			alert("Operação realizada com sucesso.");
			document.location.href = \''.$_SERVER['HTTP_REFERER'].'\';
		  </script>';
	exit;
}

$tipoObra = $oSubacaoControle->verificaTipoObra($_GET['icoid']);
$arItensPlanilhaOrcamento = $oSubacaoControle->recuperarItensComposicaoPlanilha($tipoObra, $_GET['icoid'], $_GET['tipoFundacao']);

$y=0;
foreach($arItensPlanilhaOrcamento as $dados){
	if(!empty($dados['ppoqtditem'])){		
		$y++;
		if($y == 1){
			$itctipofundacao = $dados['itctipofundacao'];
		}	
	}
	
}

if(isset($itctipofundacao)){	
	$arItensPlanilhaOrcamento = $oSubacaoControle->recuperarItensComposicaoPlanilha($tipoObra, $_GET['icoid'], $itctipofundacao);
}

$arItensPlanilhaOrcamento = $arItensPlanilhaOrcamento ? $arItensPlanilhaOrcamento : array();
$arConteudo = $oSubacaoControle->identacaoRecursiva($arItensPlanilhaOrcamento);
$nrTotal = $oSubacaoControle->recuperarValorTotalItensComposicaoPlanilha($tipoObra, $_GET['icoid']);
?>
<script>
	function tipoFundacao(tipo)
	{
		url = document.location.href;		
		document.location.href = url+'&mostra=true&tipoFundacao='+tipo;
	}
</script>
<?php if(OBRA_TIPO_B == $tipoObra && !isset($_GET['mostra']) && $itctipofundacao == null): ?>
	<table width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
		<tr>
			<td align="center">
				<input type="radio" name="fundacao" onclick="tipoFundacao('E')" value="E" /> Estaca
				<input type="radio" name="fundacao" onclick="tipoFundacao('S')" value="S" /> Sapata
			</td>
		</tr>
	</table>
	<?php exit() ?>
<?php else: ?>
	<center>
	<form action="" method="post">
	<table width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
		<thead>
			<tr>
				<td height="25px" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Descrição do item</b></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Valor unitário</b></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Unidade de medida</b></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Quantidade</b></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Valor</b></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>%</b></td>
			</tr>
		</thead>
		<tbody>
		<?php if(!empty($arItensPlanilhaOrcamento) && $arItensPlanilhaOrcamento[0]): ?>
			<?php $x = 0 ?>
			<?php foreach($arConteudo as $item): ?>
				<?php 
				$cor = ($x % 2) ? '#f0f0f0' : 'white';
				?>
				<tr style="background:<?php echo $cor ?>" onmouseover="javascript:style.background='#ffffcc'" onmouseout="javascript:style.background='<?php echo $cor ?>'">
					<td height="25px" ><?php echo $item['itcdescricao'] ?></td>
					<td align="right"><?php echo $item['itcvalorunitario'] ?></td>
					<td align="center"><?php echo $item['umdeesc'] ?></td>
					<td align="center">
						<input type="hidden" name="ppoid[]" value="<?php echo $item['ppoid']?>">
						<input type="hidden" name="preid[]" value="<?php echo $item['preid']?>">
						<input type="hidden" name="itcid[]" value="<?php echo $item['itcid']?>">
						<!-- input type="text" name="ppoqtditem[]" value="<?php echo $item['ppoqtditem'] ?>" -->
						<?php //echo campo_texto('ppoqtditem[]', 'S', 'S', '', 10, 20, '', '', '', '', '', '', '', $item['ppoqtditem']) ?>
						<?php echo $item['campo'] ?>
					</td>
					<td align="right">
						<?php if($item['ppoqtditem']): ?>
							<?php $valor = ($item['ppoqtditem']*$item['itcvalorunitario']) ?>
							<?php echo formata_valor($valor) ?>
						<?php endif; ?>
					</td>
					<td align="right">
						<?php
						if($item['ppoqtditem']){
							$porcento = ($valor*100)/$nrTotal;
							echo round($porcento, 2)."%";
						}
						?>
						<?php if($_GET['tipo']): ?>
							<input type="hidden" name="itctipofundacao[]" value="<?php echo $_GET['tipoFundacao']?>" />
						<?php endif; ?>						
					</td>
				</tr>				
				<?php $x++ ?>
			<?php endforeach; ?>
		<?php endif; ?>
		</tbody>
		<tfoot>
			<?php if(!empty($arItensPlanilhaOrcamento) && $arItensPlanilhaOrcamento[0]): ?>
				<tr height="30" class="title">
					<td class="SubTituloEsquerda" align="left" colspan="4">Total:</td>
					<td class="SubTituloDireita" align="right"><?php echo formata_valor($nrTotal) ?></td>
					<td class="SubTituloDireita" align="right"><?php echo ($nrTotal > 0) ? '100%' : '' ?></td>
				</tr>
			<?php endif; ?>
			<tr height="30">
				<td align="left" colspan="6" bgcolor="#e9e9e9">
					<?php if(!empty($arItensPlanilhaOrcamento) && $arItensPlanilhaOrcamento[0]): ?>
						<input type="submit" value="Salvar">
					<?php else: ?>				
						<center><p>Não existem registros.</p></center>
					<?php endif; ?>
				</td>
			</tr>
		</tfoot>
	</table>
	</form>
	</center>
<?php endif; ?>
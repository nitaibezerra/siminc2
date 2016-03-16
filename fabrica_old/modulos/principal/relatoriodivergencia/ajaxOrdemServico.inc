<?php
include APPRAIZ."fabrica/classes/OrdemServico.class.inc";

$ctrid = (int)$_POST['ctrid'];
$vgcid = (int)$_POST['vgcid'];

if( empty($ctrid) || empty($vgcid)) {
	exit;
}

$os = new OrdemServico();
$linhas = $os->mostraListaDeDivergencias($ctrid, $vgcid);

$cabecalho = array('Memorando', 'SS', 'OS', 'Previs&atilde;o do In&iacute;cio', 'Previs&atilde;o do T&eacute;rmino',
		'PF a pagar ap&oacute;s c&aacute;lculo da glosa', 'Valor de Ponto de Fun&ccedil;&atilde;o Unit&aacute;rio pago',
		'Valor Total pago (R$) (a)', 'Novo Valor de Ponto de Fun&ccedil;&atilde;o Unit&aacute;rio',
		'Novo Valor Total (R$) (b)', 'Diferen&ccedil;a (b-a)', 'Repactua&ccedil;&atilde;o');

$totalDiferencas = 0;
?>

<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<thead>
		<tr>
			<?php foreach($cabecalho as $item) { ?>
				<td align="center" valign="top" class="title" 
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" 
					onmouseout="this.bgColor='';" 
					bgcolor="">
						<strong><?php echo $item; ?></strong>
				</td>
			<?php } ?>
		</tr>
	</thead>
	<tbody>
		<?php if($linhas == false) { ?>
			<tr><td align="center" <?php if(count($cabecalho) > 0) echo 'colspan="'.count($cabecalho).'"' ?> style="color:#cc0000;">N&atilde;o foram encontrados Registros.</td></tr>
		<?php } else {
				$totalLinhas = count($linhas);
				for($i = 0; $i < $totalLinhas; $i++) {
					$linhaatual = $linhas[$i];
					if ($i % 2 == 0) $marcado = '' ; else $marcado='#F7F7F7';
				?>
			<tr bgcolor="<?php echo $marcado; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
				<?php foreach($linhaatual as $key=>$coluna) { ?>
					<td style="<?php if(is_numeric($coluna)) echo 'text-align:right; color: #0066cc;'; else echo 'text-align:left;'; ?>">
						<?php
							if($key == 'diferenca') {
								$totalDiferencas += (float)$coluna;
							}
							
							if(strrpos(strip_tags($coluna), '.') != false) {
								$coluna = number_format(strip_tags($coluna), 2, ',', '.');
							}
							
							echo $coluna;
						?>
					</td>
				<?php } ?>
			</tr>
		<?php
				}
		} ?>
	</tbody>
	<tfoot>
		<tr style="background-color: #CCCCCC;">
			<td align="center" colspan="10">&nbsp;</td>
			<td style="text-align:right;font-weight: bold;">R$ <?php echo number_format($totalDiferencas, 2, ',', '.'); ?></td>
			<td>&nbsp;</td>
		</tr>
	</tfoot>
</table>


<?php exit; // termina aqui para não carregar os javascripts inseridos de maneira incorreta
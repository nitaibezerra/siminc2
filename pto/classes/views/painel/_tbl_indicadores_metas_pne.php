<?php $dadosIndPainel = $this->metaSolucao->getMetaIndicadorPainel($this->dado['idsExternos']['mpneid'], $this->solid); ?>
<div class="col-lg-12">
	<h5 class="titulo_solucao">Indicadores das Metas PNE</h5>

	<?php if (is_array($dadosIndPainel)): ?>
		<?php foreach ($dadosIndPainel as $metaNome => $arrayIndPainel): ?>

			<table cellspacing="0" cellpadding="0" border="0" class="table table-striped table-bordered table-condensed tbl_verde">
				<tbody>
				<tr>
					<td colspan="6" style="background-color: #d1f6dd; font-size: 11px;"><?= $metaNome; ?></td>
				</tr>

				<?php foreach ($arrayIndPainel as $indPainel):
					?>
					<tr>
						<td rowspan="3" class="alinharMeio"><span><?= $indPainel['indnome'] ?></span></td>
						<td rowspan="2" class="alinharMeio"><b>Descrição</b></td>
						<td colspan="2" rowspan="2"><?= $indPainel['indobjetivo'] ?></td>
						<td class="alinharMeio"><b>Fórmula de Cálculo</b></td>
						<td class="alinharMeio"><b>Periodicidade de Apuração</b></td>
					</tr>
					<tr>
						<td rowspan="2"><?= $indPainel['indformula'] ?></td>
						<td rowspan="2" class="alinharMeio"><?= $indPainel['perdsc'] ?></td>
					</tr>
					<tr>
						<td class="alinharMeio"><b>Fonte</b></td>
						<td colspan="2"><?= $indPainel['indfontetermo'] ?></td>
					</tr>
				<?php endforeach ?>

				</tbody>
			</table>

		<?php endforeach; ?>
	<?php endif; ?>
</div>




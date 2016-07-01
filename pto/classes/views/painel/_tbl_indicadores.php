<?php $dados = $this->indicadorSolucao->getIndicadorPainel($this->dado['solucao']['solid']); ?>
<?php if ($dados): ?>
	<table cellspacing="0" cellpadding="0" border="0" class="table table-striped table-bordered table-condensed tbl_verde">
		<tbody>
		<?php foreach ($dados as $key => $indicador): ?>
			<tr>
				<td rowspan="3" class="alinharMeio"><span><?= $indicador['nome'] ?></span></td>
				<td rowspan="2" class="alinharMeio"><b>Descrição</b></td>
				<td colspan="2" rowspan="2"><?= $indicador['descricao'] ?></td>
				<td class="alinharMeio"><b>Fórmula de Cálculo</b></td>
				<td class="alinharMeio"><b>Periodicidade de Apuração</b></td>
			</tr>
			<tr>
				<td rowspan="2"><?= $indicador['formula'] ?></td>
				<td rowspan="2" class="alinharMeio"><?= $indicador['periodicidade'] ?></td>
			</tr>
			<tr>
				<td class="alinharMeio"><b>Fonte</b></td>
				<td colspan="2"><?= $indicador['fonte'] ?></td>
			</tr>
		<?php endforeach; ?>
		</tbody>
	</table>
	<br>
<?php endif; ?>
<div class="well">
	<h3><?= $eixo['nome']; ?></h3>

	<p><?= $eixo['descricao']; ?></p>
	<br>
	<h4>Resultados Por Natureza</h4>

	<p>Registrar somente resultados derivados estritamente das atividades do grupo PET</p>
</div>

<?php if ($this->questaoBinaria->existe($eixo['ideixo'])) : ?>
	<form role="form" class="form-horizontal" method="post" id="form-RespostaBinario<?= $eixo['ideixo']; ?>">
		<input name="idgid" type="hidden" value="<?= $this->view->identificacaoGrupo->getAttributeValue('idgid'); ?>">
		<input name="ideixo" id="idEixo<?= $eixo['ideixo']; ?>" type="hidden" value="<?= $eixo['ideixo']; ?>">
		<input name="queid" type="hidden" value="<?= $eixo['queid']; ?>">
		<input name="numeroeixo" type="hidden" value="<?= $eixo['numeroeixo']; ?>">

		<table class="table table-striped table-condensed table-bordered">
			<tr>
				<th>Tema</th>
				<th>Titulo</th>
				<th>SIM</th>
				<th>NÃO</th>
				<th>Quantidade</th>
			</tr>
			<?php $this->questaoBinaria->criarTds($this->respostas, $this->view->identificacaoGrupo->getAttributeValue('idgid'),$eixo['queid'], $this->somenteLeitura); ?>
		</table>

		<?php if (!$_SESSION['finalizado'] && !$this->somenteLeitura) { ?>
			<div class="form-group">
				<div class="col-lg-offset-2 col-lg-10 text-right">
					<button type="button" class="btn btn-primary btGravarRespostaBinario">
						<span class="glyphicon glyphicon-floppy-disk" aria-hidden="true"></span> Salvar - Eixo <?= $eixo['numeroeixo']; ?>
					</button>
				</div>
			</div>
		<?php }; ?>

	</form>
<?php else: ?>
	<div class="well"> Questão não cadastrada</div>
<?php endif; ?>
<script>
	$(function () {
		$('.qtdBinario').mask('00000000');
		$('.qtdBinario').hide();

		var radios = $('.questaoBinario[value=s]:checked');
		radios.each(function (index) {
			var qubid = $(this).data('qubid');
			$('#qtd' + qubid).show();
		});

		$('.questaoBinario').click(function () {
			var qubid = $(this).data('qubid');
			if ($(this).is(':checked') && $(this).val() == 's') {
				$('#qtd' + qubid).show();
				$('#qtd' + qubid).val('');
			} else {
				$('#qtd' + qubid).hide();
				$('#qtd' + qubid).val('');
			}
		});
	});
</script>
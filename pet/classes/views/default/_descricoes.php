<form role="form" method="post" id="form-gravaIdentificacaoGrupo">
    <input name="grpid" type="hidden" value="<?= $this->idGrupo; ?>">
    <input name="idgid" id="idgid" type="hidden" value="<?= $this->view->identificacaoGrupo->getAttributeValue('idgid') ?>">
    <fieldset>
        <legend>Descrições sobre o Grupo</legend>

        <div class="form-group">
            <label for="descricaoprojeto">Descrição resumida do projeto PET original(facultativo)</label>
			<?php if($this->somenteLeitura): ?>
				<br> <?= $this->identificacaoGrupo->getAttributeValue('descricaoprojeto'); ?>
			<?php else: ?>
				<textarea class="form-control" rows="3" name="descricaoprojeto"><?= $this->identificacaoGrupo->getAttributeValue('descricaoprojeto'); ?></textarea>
			<?php endif; ?>
        </div>

        <div class="form-group">
            <label for="descricaotrajetoria">
                Descrição dos aspectos de funcionamento do grupo PET que identifiquem a trajetória de
                melhorias do grupo
            </label>
			<?php if($this->somenteLeitura): ?>
				<br> <?= $this->identificacaoGrupo->getAttributeValue('descricaotrajetoria'); ?>
			<?php else: ?>
				<textarea required="required" class="form-control" rows="3" name="descricaotrajetoria"><?= $this->identificacaoGrupo->getAttributeValue('descricaotrajetoria'); ?></textarea>
			<?php endif; ?>
        </div>

        <div class="form-group">
            <label for="descricaointeracaocolegiado">
                Descrição da interação do grupo PET com o(s) colegiado(s) de curso(s) ou equivalentes.
            </label>
			<?php if($this->somenteLeitura): ?>
				<br> <?= $this->identificacaoGrupo->getAttributeValue('descricaointeracaocolegiado'); ?>
			<?php else: ?>
				<textarea required="required" class="form-control" rows="3" name="descricaointeracaocolegiado"><?= $this->identificacaoGrupo->getAttributeValue('descricaointeracaocolegiado'); ?></textarea>
			<?php endif; ?>
        </div>

		<?php if(!$this->somenteLeitura): ?>
			<div class="text-right">
				<button type="button" class="btn btn-primary" id="gravaIdentificacaoGrupo">
					<span class="glyphicon glyphicon-floppy-disk"></span> Gravar Descrições
				</button>
			</div>
		<?php endif; ?>

    </fieldset>
</form>

<?php if(!$this->somenteLeitura): ?>
	<script>
		$(function () {
			$('#gravaIdentificacaoGrupo').on('click', function () {
				$('#form-gravaIdentificacaoGrupo').saveAjax({ action: 'salvar', controller: 'IdentificacaoGrupo', retorno: true, displayErrorsInput: true, clearForm: false, functionSucsess: 'atualizaColegiado'});
			});
		});

		function atualizaColegiado(){
			$.post(window.location.href, {controller: 'default', action: 'selecionarGrupo', id: $('#grpid').val()}, function (data) {
				$('#div_grupo').html(data);
				$('#tab_grupo li:eq(1) a').tab('show');
			});
		}
	</script>
<?php endif; ?>


<fieldset>
	<legend>Lista de Grupos</legend>
	<?= $this->grupo->getListaCursos(); ?>
</fieldset>

<script type="text/javascript">
    $(function () {
        $('.btn_selecionar').on('click', function () {
			atualializaGrupo($(this).data('id'))
        });
    });
</script>


<?php require_once('formulario_cadastro.php');?>

<?php if (!empty($_SESSION['solid'])): ?>
    <script type="text/javascript">
        $(function () {
            $('#aba_cadastro_etapa').closest('li').removeClass('disabled');
			$('#aba_anexar_boletim').closest('li').removeClass('disabled');
            $('#div_solucao_selecionada').closest('.row').show();
            $('#div_solucao_selecionada').html('<?= $this->view->tituloSolucao; ?>');
        })
    </script>
<?php endif; ?>

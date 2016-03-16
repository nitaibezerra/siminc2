<?php  $this->boletim->getListing(); ?>

<script type="text/javascript">
	$(function () {
		$('.btn_excluir_boletim').on('click', function () {
			$.deleteItem({ controller: 'boletim', action: 'excluir', retorno: true, text: 'Deseja realmente excluir este Boletim?', id: $(this).data('id'), functionSucsess: 'atualizaGridBoletim' });
		});
		$('.pagination').closest('div').hide();
	});
</script>
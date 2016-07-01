
			<legend>Lista de Grupos</legend>
			<?php $this->questionario->getTablePainel(); ?>
		

<script type="text/javascript">
	$(function () {
		$('.reabrir').on('click', function () {
			var id =  $(this).data('id');
			$.post(window.location.href, {controller: 'default', action: 'reabrirGrupo', id: id}, function (data) {
				if(data){
					alert(data);
					$.post(window.location.href, {controller: 'default', action: 'lista', id: id}, function (data) {
						$('#div_painel_grupo').html(data);
					});
				}
			});
		});

		$('.view_nivel').on('click', function (event) {
			event.preventDefault();
			$(this).closest('tr').next().toggle();
			$(this).toggleClass("glyphicon-chevron-up glyphicon-chevron-down");
		});

		$('.visualizarGrupo').on('click', function () {
			$.post(window.location.href, {controller: 'default', action: 'visualizarGrupo', id: $(this).data('id')}, function (data) {
				$('#div_grupo').html(data);
			});
		});

		var ufs = new Array();
		$.each( $('#rel_uf').find('.uf'), function( key, value ) {
			ufs.push($(this).html());
		});

		var porcentagemConcluida = new Array();
		var porcentagemNaoConcluida = new Array();
		$.each( $('#rel_uf').find('.per_preenc_uf'), function( key, value ) {
			porcentagemConcluida.push($(this).data('perc') );
		});
		$.each( porcentagemConcluida, function( key, value ) {
			porcentagemNaoConcluida.push(100-value);
		});
		
	});
</script>


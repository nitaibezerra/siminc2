<div class="page-header">
	<h1><?= $this->titulo; ?></h1>
</div>

<div class="row">
	<div class="col-lg-8">
		<fieldset id="div_painel_grupo">
			<legend>Lista de Grupos</legend>
			<?php $this->questionario->getTablePainel(); ?>
		</fieldset>
	</div>
	<div class="col-lg-4">

		<div class="panel panel-default">
			<div class="panel-body">
				<div id="container" style="height: 600px; margin: 0 auto"></div>
			</div>
		</div>
	</div>
</div>

<!-- Modal -->
<div class="modal fade" id="modalDetalhe" tabindex="-1" role="dialog" aria-labelledby="modalDetalheLabel">
	<div class="modal-dialog" role="document" style="width: 800px;">
		<div class="modal-content">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
				<h4 class="modal-title" id="modalDetalheLabel">Detalhes do Grupo</h4>
			</div>
			<div class="modal-body" id="div_grupo">

			</div>
			<div class="modal-footer">
				<button type="button" class="fechar btn btn-default" data-dismiss="modal">fechar</button>
			</div>
		</div>
	</div>
</div>

<script type="text/javascript">
	$(function () {
//		$('.fechar').on('click', function () {
//			listagem();
//		});

		$('.reabrir').on('click', function () {
			var resp = confirm("Deseja reabri o questionario para o grupo? ");

			if (resp != null) {
				var id =  $(this).data('id');
				$.post(window.location.href, {controller: 'default', action: 'reabrirGrupo', id: id}, function (data) {
					if(data){
						alert(data);
						listagem();
					}
				});
			}

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

		$('#container').highcharts({
			chart: {
				type: 'bar'
			},
			title: {
				text: 'Índice de Conclusão por Estado'
			},
			xAxis: {
				categories: ufs
			},
			yAxis: {
				max: 100,
				min: 0,
				title: {
					text: 'Subtitulo'
				}
			},
			legend: {
				reversed: true
			},
			tooltip: {
				pointFormat: '<span style="color:{series.color}">{series.name}</span>: {point.percentage:.0f}%<br/>',
				shared: true
			},
			plotOptions: {
				series: {
					stacking: 'normal'
				},
				column: {
					stacking: 'percent'
				}
			},
			series: [
				{
					name: 'Não Concluído',
					data: porcentagemNaoConcluida
				},
				{
					name: 'Concluído ',
					data: porcentagemConcluida
				}
			]
		});
	});

	function listagem(){
		$.post(window.location.href, {controller: 'default', action: 'lista'}, function (data) {
			$('#div_painel_grupo').html(data);
		});
	}
</script>


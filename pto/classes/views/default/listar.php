<?php $this->listing->listing($this->data); ?>
<script>
	$(function () {
		$('#div_listar').on('click', '.btn_editar_etapa', function (event) {
			event.stopPropagation();
			$('#aba_cadastro_etapa').closest('li').removeClass('disabled');

			var solid = $(this).closest('table').closest('tr').prev().find('.btn_editar').data('id');
			cadastrarEtapa($(this).data('id'), solid);
		});

		$('#div_listar').on('click', '.btn_editar_atividade', function (event) {
			event.stopPropagation();
			var solid = $(this).closest('table').closest('tr').closest('table').closest('tr').prev().find('.btn_editar').data('id');
			var etpid = $(this).closest('table').closest('tr').prev().find('.btn_editar_etapa').data('id');
			editarAtividadePelaSolucao($(this).data('id'), solid, etpid);
		});

		$('#div_listar').on('click', '.btn_excluir_etapa', function (event) {
			var solid = $(this).closest('table').closest('tr').prev().find('.btn_editar').data('id');
			$.deleteItem({ controller: 'etapa', action: 'excluir', retorno: true, text: 'Deseja realmente excluir esta Etapa?', id: $(this).data('id'), solid: solid, functionSucsess: 'atualizaGridEtapaEmSolucao' });
		});

		$('#div_listar').on('click', '.btn_excluir_atividade', function (event) {
			var etpid = $(this).closest('table').closest('tr').prev().find('.btn_editar_etapa').data('id');
			$.deleteItem({ controller: 'atividade', action: 'excluir', retorno: true, text: 'Deseja realmente excluir esta Atividade?', id: $(this).data('id'), etpid: etpid, functionSucsess: 'atualizaGridAtividadeEmSolucao' });
		});
	});
</script>

<script type="text/javascript">
	$(function () {
		$('#table_solucao tbody').sortable({
			items: "tr",
			handle: ".btn_ordenar_solucao",
			opacity: 0.4,
//            revert: true,
			beforeStop: function (event, ui) {

				var next = ui.item.next().next();
				var prev = ui.item.prev().prev();

				if ( strpos ( next.attr('id'), 'solucao') != false || strpos ( prev.attr('id') , 'solucao') != false ) {
					return true;
				}
			},
			update: function (event, ui) {
				var sortedIDs = $("#table_solucao tbody").sortable("toArray");
				enviarOrdenacaoSolucao(event, ui, sortedIDs);
			}
		});

		$('.pagination').closest('div').hide();

		function strpos(haystack, needle, offset) {
			var i = (haystack + '')
				.indexOf(needle, (offset || 0));
			return i === -1 ? false : i;
		}

	});
</script>
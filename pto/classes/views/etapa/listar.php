<?php $this->listingEtapa->listing($this->data); ?>

<script type="text/javascript">
    $(function () {
        $('.btn_visualizar_atividade').on('click', function () {
            visualizar_atividade($(this).data('id'), $(this))
        });

        $('#table_etapa tbody').sortable({
            items: "tr",
            handle: ".btn_ordenar",
            opacity: 0.4,
//            revert: true,
            beforeStop: function (event, ui) {

                var next = ui.item.next().next();
                var prev = ui.item.prev().prev();

                if ( next.hasClass('new_tr_atividade') ) {
                    return false;
                }

                if ( strpos ( next.attr('id'), 'atividade') != false || strpos ( prev.attr('id') , 'atividade') != false ) {
                    return false;
                }
            },
            update: function (event, ui) {
                var solid = ui.item.closest('table').closest('tr').prev().find('.btn_editar').data('id');
                if (typeof solid === 'undefined') {
                    solid = 0;
                }
                var sortedIDs = $("#table_etapa tbody").sortable("toArray");
                enviarOrdenacaoEtapa(event, ui, solid, sortedIDs);
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
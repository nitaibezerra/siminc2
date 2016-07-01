<?php
$this->listingAtividade->listing($this->data);
?>
<script type="text/javascript">
    $(function () {
        $('.btn_editar_atividade').on('click', function () {
            $.post(window.location.href, {'controller': 'atividade', 'action': 'editar', 'id': $(this).data('id')}, function (html) {
                $('#div_form_atividade').html(html);
            });
        });

        $('.btn_excluir_atividade').on('click', function () {
            $.deleteItem({ controller: 'atividade', action: 'excluir', retorno: true, text: 'Deseja realmente excluir esta Atividade?', id: $(this).data('id'), functionSucsess: 'atualizaGridAtividade' });
        });

        $('#table_atividade tbody').sortable({
            items: "tr",
            handle: ".btn_ordenar_atividade",
            opacity: 0.4,
            update: function (event, ui) {
                var etpid = $(this).closest('table').closest('tr').prev().find('.btn_editar_etapa').data('id');
                if(typeof etpid === 'undefined'){
                    etpid =  0;
                };
                var sortedIDs = $("#table_atividade tbody").sortable("toArray");
                enviarOrdenacaoAtividade(event, ui, etpid, sortedIDs);
            }
        });

        $('.pagination').closest('div').hide();
    });
</script>
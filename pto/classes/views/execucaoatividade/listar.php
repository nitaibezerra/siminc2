<?php
$this->listingAtividade->listing($this->dataExecutor);
?>
<script type="text/javascript">
    $(function () {
        $('.btn_selecionar_atividade').on('click', function () {
            $.post(window.location.href, {'controller': 'ExecucaoAtividade', 'action': 'selecionaratividade', 'id': $(this).data('id')}, function (html) {
                $('#div_atividade').html(html);
            });
        });
        $('.pagination').closest('div').hide();
    });
</script>
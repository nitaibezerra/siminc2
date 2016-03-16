<div class="container">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h3 class="modal-title">
                    Salvar arquivo
                    <!--                <small>na Instituição -->
                    <?php //echo $_SESSION['instituicao']['intdscrazaosocial'] ?><!--</small>-->
                </h3>
                <!--            <h4 class="modal-title"></h4>-->
            </div>
            <div class="modal-body">

                <div id="container_formulario_arquivo">
                    <?php $this->formularioAction(); ?>
                </div>

                <div class="col-lg-12">
                    <div class="page-header">
                        <h1 id="forms">
                            <!--                Dados da universidade --->
                            <small>
                                Listagem
                            </small>
                        </h1>
                    </div>
                </div>
                <div class="row">
                    <div class="col-md-12" id="container_listar_arquivo">
                        <?php $this->listarAction(); ?>
                    </div>
                </div>
            </div>
        <div class="modal-footer">
            <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
        </div>
    </div>
</div>
<script language="JavaScript">

    function listarArquivos()
    {
        var id = $('#form_save #dmdid').val();

        var data = {controller: 'documentoarquivo', action: 'listar' , id: id};
        $.post(window.location.href, data, function (html) {
            $('#container_listar_arquivo').hide().fadeIn().html(html);
        });
    }

</script>
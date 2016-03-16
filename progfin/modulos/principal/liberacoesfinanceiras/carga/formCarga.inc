<?php
/**
 * Arquivo de criação do formulário de carga de liberações financeiras.
 * $Id: formCarga.inc 90195 2014-11-13 16:47:25Z maykelbraz $
 */
?>
<style>
#modal-alert .modal-body ul{text-align:left;margin-top:5px;list-style:circle}
</style>
<script type="text/javascript" src="../library/bootstrap-3.0.0/js/bootstrap.file-input.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript">
$('document').ready(function(){
    $('input[type=file]').bootstrapFileInput();

    $('#buttonSend').click(function(){
        // -- Validando os itens do formulario obrigatorios para criar um novo pedido
        var msg = new Array();
        var itemsParaValidacao = new Array('#siafi_usuario', '#siafi_password', '#siafi_ug');
        for (var x in itemsParaValidacao) {
            // -- Selecionando o input
            var $item = $(itemsParaValidacao[x]);
            if (undefined == $item.attr('name')) {
                continue;
            }

            if (!$item.val()) { // -- validando o conteúdo do input e selecionando o label para montar msg de erro
                msg.push($item.parent().prev().children('label').text().replace(':', ''));
                $item.parent().parent().addClass('has-error');
            }
        }
        // -- Se existir alguma mensagem, exibe para o usuário
        if (msg.length > 0) {
            var htmlMsg = '<div class="bs-callout bs-callout-danger">Antes de prosseguir, os seguintes campos devem ser preenchidos:<ul>';
            for (var x in msg) {
                if ('function' == typeof (msg[x])) {
                    continue;
                }

                htmlMsg += '<li>' + msg[x];
                if (x == msg.length - 1) {
                    htmlMsg += '.';
                } else {
                    htmlMsg += ';';
                }
                htmlMsg += '</li>';
            }
            htmlMsg += '</ul></div>';
            $('#modal-alert .modal-body').html(htmlMsg);
            $('#modal-alert').modal({backdrop: 'static'});
            return;
        }

        $('#cargaLiberacaoFinanceira').submit();
    });
});

</script>
<div class="well col-md-12">
    <form name="cargaLiberacaoFinanceira" id="cargaLiberacaoFinanceira" enctype="multipart/form-data" method="POST"
          class="form-horizontal" role="form">
        <input type="hidden" name="requisicao" id="requisicao" value="processarArquivo" />
        <div class="form-group">
            <div class="col-md-10">
                <input type="file" title="Carregar arquivo de liberações (.csv)"
                       name="liberacoes" class="btn btn-primary start" />
                <input type="hidden" name="action" id="action" value="carregar" />
            </div>
            <div class="col-md-2">
                <div class="alert alert-info" style="margin-bottom:0">
                    <p>
                        <i class="glyphicon glyphicon-download-alt"></i>
                        <a href="arquivos/modelo_liberacoes_financeiras_20140415.csv" target="_blank">Download do modelo</a>
                    </p>
                </div>
            </div>
        </div>
        <br style="clear:both" />
        <div class="form-group row">
            <label class="control-label col-md-2" for="siafi_usuario">Usuário SIAFI:</label>
            <div class="col-md-4">
            <?php
            $opcoes = array(
                'masc' => '###.###.###-##'
            );
            inputTexto('dados[siafi_usuario]', null, 'siafi_usuario', 14, false, $opcoes);
            ?>
            </div>
        </div>
        <div class="form-group row">
            <label class="control-label col-md-2" for="siafi_password">Senha:</label>
            <div class="col-md-4">
                <input type="password" class="form-control" maxlength="50" id="siafi_password" name="dados[siafi_password]" />
            </div>
        </div>
        <div class="form-group row">
            <label class="control-label col-md-2" for="siafi_ug">UG (do usuário):</label>
            <div class="col-md-4">
                <input type="text" class="form-control" maxlength="6" id="siafi_ug" name="dados[siafi_ug]" />
            </div>
        </div>
        <div class="form-group">
            <div class="col-lg-10">
                <button class="btn btn-success" id="buttonSend" type="button"><i class="glyphicon glyphicon-upload"></i> Enviar</button>
                <button class="btn btn-warning" id="buttonClear" type="reset">Limpar</button>
            </div>
        </div>
        <br style="clear:both" />
    </form>
</div>
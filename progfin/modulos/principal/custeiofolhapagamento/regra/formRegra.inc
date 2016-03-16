<?php
/**
 * Formulário de filtro de listagem e gestão de regras.
 * $Id: formRegra.inc 78244 2014-04-01 18:26:00Z maykelbraz $
 */
?>
<style type="text/css">
    #form-regra img{display:none}
    label{cursor:pointer}
    .modal-body{text-align:left}
</style>
<script type="text/javascript" language="javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" language="javascript">
    $('document').ready(function() {
        $('#salvarRegra').click(function() {
            if (salvarRegra()) {
                $('#requisicao-regra').val('salvarRegra');
                $('#form-regra').submit();
            } else {
                return false;
            }
        });
        $('#cancelarRegra').click(function() {
             window.location = 'progfin.php?modulo=principal/custeiofolhapagamento/regra/inicio&acao=A';
        });
        $('#novoRegra').click(function() {
            if (salvarRegra()) {
                $('#requisicao-regra').val('novoRegra');
                $('#form-regra').submit();
            } else {
                return false;
            }
        });
        $('#buscarRegra').click(function() {
            $('#requisicao-regra').val('buscarRegra');
            $('#form-regra').submit();
        });
        $('#limparBusca').click(function() {
            $('#requisicao-regra').val('limparBusca');
            $('#form-regra').submit();
        });
        $('#voltar').click(function() {
            window.location = 'progfin.php?modulo=inicio&acao=C';
        });
    });

    function salvarRegra()
    {
        // -- Validando os itens do formulario obrigatorios 
        var msg = new Array();
        var itemsParaValidacao = new Array(
                '#rccnomecoluna', '#rccelementodespesa', '#rccrubrica'
                );
        for (var x in itemsParaValidacao) {
            // -- Selecionando o input
            var $item = $(itemsParaValidacao[x]);

            if (undefined == $item.attr('name')) {
                continue;
            } else if (!$item.val()) { // -- validando o conteúdo do input e selecionando o label para montar msg de erro
                var labelItem = $item.parent().prev().text().replace(':', '');
                if ('até' != labelItem) {
                    msg.push(labelItem);
                    $item.parent().parent().addClass('has-error');
                }
            }
        }
        // -- Se existir alguma mensagem, exibe para o usuário
        if (msg.length > 0) {
            var htmlMsg = '<div class="bs-callout bs-callout-danger">Antes de criar ou alterar uma regra para processamento, os seguintes campos devem ser preenchidos:<ul><br />';
            for (var x in msg) {
                if ('string' == typeof msg[x]) {
                    htmlMsg += '<li>' + msg[x];
                    if (x == msg.length - 1) {
                        htmlMsg += '.';
                    } else {
                        htmlMsg += ';';
                    }
                    htmlMsg += '</li>';
                }
            }
            htmlMsg += '</ul></div>';
            $('#modal-alert .modal-body').html(htmlMsg);
            $('#modal-alert').modal();
            return false;
        }
        return true;
    }
</script>
<form name="regra" id="form-regra" method="POST" class="form-horizontal" role="form" novalidate="novalidate">
    <input type="hidden" name="requisicao" id="requisicao-regra" />
    <input type="hidden" name="dados[rccid]" id="rccid" value="<?php echo $formData['rccid']; ?>" />
    <div class="form-group control-group">
        <label for="mcrdsc" class="col-lg-2 control-label pad-12">Descrição:</label>
        <div class="col-lg-10">
            <?php
            $complemento = 'id="rccdsccoluna" required class="form-control"';
            echo campo_texto('dados[rccdsccoluna]', "S", "S", "Descrição", 12, 200, "", "", '', '', 0, $complemento, '', $formData['rccdsccoluna']);
            #echo inputTexto($complemento, $formData['rccdsccoluna'], $id, $limite);
            ?>
        </div>
    </div>
    <div class="form-group control-group">
        <label for="rccelementodespesa" class="col-lg-2 control-label pad-12">Elementos de Despesa:</label>
        <div class="col-lg-10">
            <?php
            $complemento = 'id="rccelementodespesa" required class="form-control"';
            echo campo_texto('dados[rccelementodespesa]', "S", "S", "Elementos de Despesa", 12, 1000, "", "", '', '', 0, $complemento, '', $formData['rccelementodespesa']);
            ?>
            <br/><i>* Separe os elementos de despesa com vírgula.</i>
        </div>
    </div>
    <div class="form-group control-group">
        <label for="rccrubrica" class="col-lg-2 control-label pad-12">Rubrica(s):</label>
        <div class="col-lg-10">
            <?php
            $complemento = 'id="rccrubrica" required class="form-control"';
            echo campo_texto('dados[rccrubrica]', "S", "S", "Rubricas", 12, 1000, "", "", '', '', 0, $complemento, '', $formData['rccrubrica']);
            ?>
            <br/><i>* Separe as rubricas com vírgula.</i>
            <br/><i>** O processamento se dará da seguinte maneira: Controles 1 e 4 Somando | 2 e 3 Subtraindo</i>
        </div>
    </div>

    <?php if (isset($formData['rccid']) && !empty($formData['rccid'])): ?>
        <button type="submit" class="btn btn-primary" id="salvarRegra">Salvar</button>
        <button type="button" class="btn btn-danger" id="cancelarRegra">Cancelar</button>
    <?php else: ?>
        <button type="submit" class="btn btn-primary" id="buscarRegra">Buscar</button>
        <button type="submit" class="btn btn-warning" id="novoRegra">Novo</button>
        <button type="button" class="btn btn-danger" id="voltar">Voltar</button>
        <?php if (isset($_SESSION['altorc']['regra']['busca'])): ?>
            <button type="submit" class="btn btn-default" id="limparBusca">Limpar filtros</button>
        <?php endif; ?>
    <?php endif; ?>
    <br style="clear:both" />
</form>
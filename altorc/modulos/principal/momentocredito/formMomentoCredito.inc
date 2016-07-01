<?php
/**
 * Formulário de filtro de listagem e gestão de momentos de crédito.
 * $Id: formMomentoCredito.inc 78244 2014-04-01 18:26:00Z maykelbraz $
 */
?>
<style type="text/css">
#form-momentocredito img{display:none}
label{cursor:pointer}
.modal-body{text-align:left}
</style>
<script type="text/javascript" language="javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" language="javascript">
$('document').ready(function(){
    $('#mcrrefinicio').mask('99/99/9999').datepicker({});
    $('#mcrreffim').mask('99/99/9999').datepicker({});
    $('#mcrincinicio').mask('99/99/9999').datepicker({});
    $('#mcrincfim').mask('99/99/9999').datepicker({});
    $('#mcraltinicio').mask('99/99/9999').datepicker({});
    $('#mcraltfim').mask('99/99/9999').datepicker({});

    $('#salvarMomentoCredito').click(function(){
        if (salvarMomentoCredito()) {
            $('#requisicao-momentocredito').val('salvarMomentoCredito');
            $('#form-momentocredito').submit();
        } else {
            return false;
        }
    });
    $('#novoMomentoCredito').click(function(){
        if (salvarMomentoCredito()) {
            $('#requisicao-momentocredito').val('novoMomentoCredito');
            $('#form-momentocredito').submit();
        } else {
            return false;
        }
    });
    $('#buscarMomentoCredito').click(function(){
        $('#requisicao-momentocredito').val('buscarMomentoCredito');
        $('#form-momentocredito').submit();
    });
    $('#limparBusca').click(function(){
        $('#requisicao-momentocredito').val('limparBusca');
        $('#form-momentocredito').submit();
    });
    $('#voltar').click(function(){
        window.location = 'altorc.php?modulo=inicio&acao=C';
    });
});

function salvarMomentoCredito()
{
    // -- Validando os itens do formulario obrigatorios para criar um novo pedido
    var msg = new Array();
    var itemsParaValidacao = new Array(
        '#mcrdsc', '#mcrcod', '#tcrid', '#mcrrefinicio', '#mcrreffim', '#mcrincinicio', '#mcrincfim', '#mcraltinicio', '#mcraltfim'
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
        var htmlMsg = '<div class="bs-callout bs-callout-danger">Antes de criar ou alterar um período, os seguintes campos devem ser preenchidos:<ul><br />';
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
<form name="momentocredito" id="form-momentocredito" method="POST" class="form-horizontal" role="form" novalidate="novalidate">
    <input type="hidden" name="requisicao" id="requisicao-momentocredito" />
    <input type="hidden" name="dados[mcrid]" id="mcrid" value="<?php echo $formData['mcrid']; ?>" />
    <div class="form-group control-group">
        <label for="mcrdsc" class="col-lg-2 control-label pad-12">Descrição:</label>
        <div class="col-lg-10">
            <?php
            $complemento = 'id="mcrdsc" required class="form-control"';
            echo campo_texto('dados[mcrdsc]', "S", "S", "Descrição", 12, 50, "", "", '', '', 0, $complemento, '', $formData['mcrdsc']);
            ?>
        </div>
    </div>
    <div class="form-group control-group">
        <label for="mcrcod" class="col-lg-2 control-label pad-12">Cód. momento SIOP:</label>
        <div class="col-lg-10">
            <?php
            $complemento = 'id="mcrcod" required class="form-control"';
            echo campo_texto('dados[mcrcod]', "S", "S", "Código do momento", 12, 4, "####", "", '', '', 0, $complemento, '', $formData['mcrcod']);
            ?>
        </div>
    </div>
    <div class="form-group control-group">
        <label for="tcrid" class="col-lg-2 control-label pad-12">Tipos de crédito:</label>
        <div class="col-lg-10">
            <select name="dados[tcrid][]" id="tcrid" multiple class="form-control chosen-select-no-single" required="required" data-placeholder="Selecione">
                <option value=""></option>
                <?php
                if (!($tiposCredito = carregarTipoCredito($_SESSION['exercicio'], null, false))) {
                    $tiposCredito = array();
                }
                $temTCSelecionado = (isset($formData['tcrid']) && is_array($formData['tcrid']));

                foreach ($tiposCredito as $tpcred) {
                    $selected = '';
                    if ($temTCSelecionado && in_array($tpcred['codigo'], $formData['tcrid'])) {
                        $selected = 'selected="selected"';
                    }

                    list($descricao) = explode(' - ', $tpcred['descricao']);
                    echo <<<HTML
<option {$selected} value="{$tpcred['codigo']}" alt="{$tpcred['descricao']}">{$descricao}</option>
HTML;
                }
                ?>
            </select>
        </div>
    </div>
    <div class="form-group">
        <label for="mcrrefinicio" class="col-lg-2 control-label pad-12">Período de referência:</label>
        <div class="col-lg-2">
            <input name="dados[mcrrefinicio]" type="text" class="form-control" id="mcrrefinicio"
                   placeholder="00/00/0000" maxlength="10" data-format="dd/MM/yyyy hh:mm:ss"
                   value="<?php echo $formData['mcrrefinicio']; ?>" required />
        </div>
        <label class="col-lg-1 control-label pad-12" for="mcrreffim" style="text-align:center">até</label>
        <div class="col-lg-2">
            <input name="dados[mcrreffim]" type="text" class="form-control" id="mcrreffim"
                   placeholder="00/00/0000" maxlength="10" data-format="dd/MM/yyyy hh:mm:ss"
                   value="<?php echo $formData['mcrreffim']; ?>" required />
        </div>
    </div>
    <fieldset>
        <legend style="font-size:16px;color:red;font-weight:bold">Preenchimento da UO:</legend>
        <div class="form-group">
            <label for="mcrincinicio" class="col-lg-2 control-label pad-12">Período de inclusão:</label>
            <div class="col-lg-2">
                <input name="dados[mcrincinicio]" type="text" class="form-control" id="mcrincinicio"
                       placeholder="00/00/0000" maxlength="10" data-format="dd/MM/yyyy hh:mm:ss"
                       value="<?php echo $formData['mcrincinicio']; ?>" required />
            </div>
            <label class="col-lg-1 control-label pad-12" for="mcrincfim" style="text-align:center">até</label>
            <div class="col-lg-2">
                <input name="dados[mcrincfim]" type="text" class="form-control" id="mcrincfim"
                       placeholder="00/00/0000" maxlength="10" data-format="dd/MM/yyyy hh:mm:ss"
                       value="<?php echo $formData['mcrincfim']; ?>" required />
            </div>
        </div>
    <div class="form-group">
        <label for="mcraltinicio" class="col-lg-2 control-label pad-12">Período de alteração:</label>
        <div class="col-lg-2">
            <input name="dados[mcraltinicio]" type="text" class="form-control" id="mcraltinicio"
                   placeholder="00/00/0000" maxlength="10" data-format="dd/MM/yyyy hh:mm:ss"
                   value="<?php echo $formData['mcraltinicio']; ?>" required />
        </div>
        <label class="col-lg-1 control-label pad-12" for="mcraltfim" style="text-align:center">até</label>
        <div class="col-lg-2">
            <input name="dados[mcraltfim]" type="text" class="form-control" id="mcraltfim"
                   placeholder="00/00/0000" maxlength="10" data-format="dd/MM/yyyy hh:mm:ss"
                   value="<?php echo $formData['mcraltfim']; ?>" required />
        </div>
    </div>
    </fieldset>
    <?php
    if (isset($formData['mcrid']) && !empty($formData['mcrid'])): ?>
    <button type="submit" class="btn btn-primary" id="salvarMomentoCredito">Salvar</button>
    <button type="button" class="btn btn-danger" id="cancelarMomentoCredito">Cancelar</button>
    <?php else: ?>
    <button type="submit" class="btn btn-primary" id="buscarMomentoCredito">Buscar</button>
    <button type="submit" class="btn btn-warning" id="novoMomentoCredito">Novo</button>
    <button type="button" class="btn btn-danger" id="voltar">Voltar</button>
        <?php if (isset($_SESSION['altorc']['momentocredito']['busca'])): ?>
    <button type="submit" class="btn btn-default" id="limparBusca">Limpar filtros</button>
        <?php endif; ?>
    <?php endif; ?>
    <br style="clear:both" />
</form>
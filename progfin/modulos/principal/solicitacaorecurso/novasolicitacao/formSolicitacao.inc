<?php
/**
 * Formulário de criação de pedido de novo recurso financeiro.
 * $Id: formSolicitacao.inc 96417 2015-04-15 19:11:11Z werteralmeida $
 */
global $result, $dados;
// -- Verificação de permições de edição do formulário

$podeEditarPedido = podeEditarPedido($result['esdid']);
$podeEditarLiberacao = ($_REQUEST['lfnid'])?podeEditarLiberacao($dados['esdid'], $dados['llfid']):true;
?>
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" lang="javascript">
$(document).ready(function () {
    $('#back').click(function () {
        $('#requisicao').val('voltarParaListagem');
        $('#novorecurso').submit();
    });

    $('#back2').click(function () {
        window.location = 'progfin.php?modulo=principal/solicitacaorecurso/solicitacoes&acao=A';
    });

    $("#save").click(function () {
        validarFormulario(['ungcod', 'stccod', 'fdsid', 'ctgcod', 'vincod', 'lfnvalor', 'clpid'], 'novorecurso', 'salvarPedidoRecursoFinanceiro');
    });
    $("#update").click(function () {
        validarFormulario(['ungcod', 'stccod', 'fdsid', 'ctgcod', 'vincod', 'lfnvalor', 'clpid'], 'novorecurso', 'alterarPedidoRecursoFinanceiro');
    });

<?php if (!$podeEditarPedido || !$podeEditarLiberacao): ?>
$('#novorecurso input,textarea').prop('disabled', true);
$('#novorecurso select').prop('disabled', true).trigger('chosen:updated');
<?php endif; ?>

    $('#clpid').change(function(){
        if ('' == $('#lfnobservacao').val()) {
            $('#lfnobservacao').val(
                $('#clpid option:selected').text() + ' - '
            );
        }
    });
});
</script>
<style type="text/css">
    #modal-alert .modal-body ul{text-align:left;margin-top:5px;list-style:circle}
</style>
<form name="novorecurso" id="novorecurso" method="POST" role="form" class="well">
    <input type="hidden" name="requisicao" id="requisicao" />
    <input type="hidden" name="unicod" value="<?php echo $dados['unicod']; ?>" />
    <input type="hidden" name="dados[plfid]" id="plfid" value="<?php echo $_GET['id']; ?>" />
    <input type="hidden" name="dados[lfnid]" id="lfnid" value="<?php echo $_GET['lfnid']; ?>" />

    <div class="form-group row">
        <div class="col-md-2">
            <label class="control-label" for="">Unidade gestora:</label>
        </div>
        <div class="col-md-10">
            <?php
            $whereAdicional[] = "ung.unicod = '{$dados['unicod']}'";
            inputComboUngcod($dados['ungcodfavorecida'], $whereAdicional);
            ?>
        </div>
    </div>
    <hr />
    <div class="form-group row">
        <div class="col-md-2">
            <label class="control-label" for="clpid">Classificação do Pedido:</label>
        </div>
        <div class="col-md-10">
            <?php inputComboClassificacao(isset($result['clpid']) ? $result['clpid'] : $dados['clpid'], 'OUTROS'); ?>
        </div>
    </div>
    <div class="form-group row">
        <div class="col-md-2">
            <label class="control-label" for="unicod">Ação Orçamentária:</label>
        </div>
        <div class="col-md-10">
            <?php inputComboAcaoOrcamentaria($dados['unicod'], $dados['acacod']); ?>
        </div>
    </div>
    <div class="form-group row acacod_2" style="display:none">
        <label class="control-label col-md-2" for="acacod_2">Código da ação:</label>
        <div class="col-md-10">
            <?php inputTexto('dados[acacod_2]', $dados['acacod_2'], 'acacod_2', 4, false); ?>
        </div>
    </div>
    <div class="form-group row">
        <div class="col-md-2">
            <label class="control-label" for="stccod">Situação Contábil:</label>
        </div>
        <div class="col-md-10">
            <?php inputComboTipoRecurso(isset($result['stccod']) ? $result['stccod'] : $dados['stccod']); ?>
        </div>
    </div>
    <div class="form-group row">
        <div class="col-md-2">
            <label class="control-label" for="ftrcod">Fonte:</label>
        </div>
        <div class="col-md-10">
            <?php inputComboFonteDetalhada(isset($result['ftrcod']) ? $result['ftrcod'] : $dados['ftrcod'], $_SESSION['exercicio']); ?>
        </div>
    </div>
    <div class="form-group row">
        <div class="col-md-2">
            <label class="control-label" for="ctgcod">Cat. Gasto:</label>
        </div>
        <div class="col-md-10">
            <?php inputComboGND(isset($result['ctgcod']) ? $result['ctgcod'] : $dados['ctgcod']); ?>
        </div>
    </div>
    <div class="form-group row">
        <div class="col-md-2">
            <label class="control-label" for="vincod">Vinculação:</label>
        </div>
        <div class="col-md-10">
            <?php inputComboVinculacaoPagamento(isset($result['vincod']) ? $result['vincod'] : $dados['vincod'], $_SESSION['exercicio']); ?>
        </div>
    </div>
    <div class="form-group row">
        <div class="col-md-2">
            <label class="control-label" for="">Valor solicitado:</label>
        </div>
        <div class="col-md-10">
            <div class="input-group">
                <span class="input-group-addon">R$</span>
                <?php
                if (isset($result['lfnvalorsolicitado'])) {
                    $dados['lfnvalor'] = (double) $result['lfnvalorsolicitado'];
                }
                $opcoes = array('masc' => '###.###.###.###,##');
                ?>
                <?php inputTexto('dados[lfnvalor]', $dados['lfnvalor'], 'lfnvalor', 23, false, $opcoes); ?>
            </div>
        </div>
    </div>
    <div class="form-group row">
        <div class="col-md-2">
            <label class="control-label" for="">Observação:</label>
        </div>
        <div class="col-md-10">
            <?php
            if (isset($result['lfnobservacao'])) {
                $dados['lfnobservacao'] = $result['lfnobservacao'];
            }
            ?>
            <?php inputTextArea('dados[lfnobservacao]', $dados['lfnobservacao'], 'lfnobservacao', '200'); ?>
        </div>
    </div>
    <hr />
    <br />
    <button type="button" class="btn btn-warning" id="<?php echo isset($_REQUEST['id']) ? 'back2' : 'back'; ?>">Voltar</button>
    <?php if (!$podeEditarPedido): ?>
        <span class="glyphicon glyphicon-exclamation-sign" style="color:red;margin-left:15px"></span> Desabilitado para Editar.
    <?php elseif (isset($result['lfnid'])): ?>
        <button type="button" class="btn btn-primary" id="update">Salvar</button>
    <?php else: ?>
        <button type="button" class="btn btn-primary" id="save">Salvar</button>
    <?php endif; ?>
</form>
<?php
if (isset($_GET['id']) && is_numeric($_GET['id'])) {
    getItensPedidoRecursoFinanceiro((int) $_GET['id'], $podeEditarPedido);
}
?>
<script type="text/javascript">
$(function(){
    $('#acacod').change(function(){
        if ('?' === $(this).val()) {
            $('.acacod_2').show();
            $('#acacod_2').focus();
        } else {
            $('#acacod_2').val('');
            $('.acacod_2').hide();
        }
    });
    $('#acacod_2').keyup(function(){
        $(this).val(
            $(this).val().toUpperCase()
        );
    });

    // -- carregando com base nos dados já armazenados
    if ('' !== $('#acacod_2').val()) {
        $('#acacod').val('?').trigger('chosen:updated');
        $('.acacod_2').show();
    }
});
</script>

<?php
/**
 * Exibe a justificativa informada pelo usuário para as despesas informadas.
 * $Id: formJustificativa.inc 81802 2014-06-20 17:54:05Z maykelbraz $
 */
?>
<script
<script type="text/javascript" language="javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" lang="JavaScript">
$(document).ready(function(){
    $('#salvar-justificativa').click(function(){
        $('#requisicao').val('salvarJustificativa');
        $('#formjustificativa').submit();
    });
    <?php if (!$podeEditarDados): ?>
    $('#formjustificativa :input').prop('disabled', true);
    <?php endif; ?>
});
</script>
<form name="formjustificativa" id="formjustificativa" method="POST" role="form">
    <input type="hidden" name="requisicao" id="requisicao" />
    <input type="hidden" name="dados[acaid]" id="acaid" value="<?php echo $dados['acaid']; ?>" />
    <div class="form-group row">
        <div>
            <label class="control-label" for="">Justificativa de despesas</label>
        </div>
        <div>
            <?php inputTextArea('dados[justificativa]', $dados['acao']['justificativa'], 'justificativa', 3000); //, $opcoes, $bpClass?>
        </div>
    </div>
    <hr />
    <br />
    <?php if ($podeEditarDados): ?>
    <button type="button" class="btn btn-primary" id="salvar-justificativa">Salvar</button>
    <?php else: echo <<<HTML
<div class="alert alert-danger text-center col-lg-6 col-lg-offset-3"><strong>Sem permissão para edição!</strong></div>
HTML;
    endif; ?>
</form>
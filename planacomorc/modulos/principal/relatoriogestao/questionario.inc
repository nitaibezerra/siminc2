<?php
/**
 * Questionário geral dos relatórios do TCU.
 *
 * @package SiMEC
 * @subpackage planacomorc
 * @version $Id$
 */

if (!$questionario): ?>
<div class="alert alert-info col-md-6 col-md-offset-3 text-center">Não existe questionário para este ano</div>
<br style="clear:both" />
<?php
  return;
endif;

$questionarioDesabilitado = false;
// -- Avaliando permissões/responsabilidade
if ($usuarioTCU && !array_intersect(array('acao', 'acaorap'), $permissoesRld) && $abaAtiva != 'justificativapo') {
    $questionarioDesabilitado = true;
}
// -- Avaliando estado do documento
if ($usuarioTCU && !in_array($dados['esdid'], array(ESDID_TCU_EM_PREENCHIMENTO, ESDID_TCU_ACERTOS_UO))) {
    $questionarioDesabilitado = true;
}
?>
<?php if ($questionarioDesabilitado): ?>
<script type="text/javascript">
$(document).ready(function(){
    $('#form-questionario textarea').prop('disabled', true);
});
</script>
<?php endif; ?>
<form action="" method="POST" class="form-horizontal" id="form-questionario">
    <input type="hidden" name="requisicao" value="salvarQuestionario">
    <input type="hidden" name="rldid" id="rldid" value="<?php echo $dados['rldid']; ?>">
    <input type="hidden" name="tqtid" value="<?php echo $questionario[0]['tqtid']; ?>">
    <section class="form-group">
        <label class="control-label col-md-2">Questionário: </label>
        <section class="col-md-10">
            <p class="form-control-static"><?php echo $questionario[0]['questionario']; ?></p>
        </section>
    </section>
    <?php foreach($questionario as $questao): ?>
    <section class="form-group">
        <label class="control-label col-md-2" for="pergunta<?php echo $questao['tqpid'];?>">Pergunta:</label>
        <section class="col-md-10">
            <p class="form-control-static" id="pergunta<?php echo $questao['tqpid'];?>">
                <?php echo $questao['pergunta']; ?>
            </p>
        </section>
    </section>
    <section class="form-group">
        <label class="control-label col-md-2" for="resposta<?php echo $questao['tqpid'];?>">Resposta:</label>
        <section class="col-md-10">
            <?php inputTextArea("tqpid[{$questao['tqpid']}]", $questao['resposta'], "resposta{$questao['tqpid']}", 6000); ?>
        </section>
    </section>
    <?php endforeach; ?>
    <?php if (!$questionarioDesabilitado): ?>
    <button class="btn btn-primary" type="submit"><span class="glyphicon glyphicon-ok"></span> Salvar Justificativa</button>
    <?php endif; ?>
</form>

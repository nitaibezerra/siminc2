<?php
/**
 * Questionário geral dos relatórios do TCU.
 *
 * @package SiMEC
 * @subpackage acomporc
 * @version $Id$
 */

if(count($questionarios) < 1): ?>
    <section class="alert alert-warning alert-dismissible fade in text-center" role="alert">
        <button type="button" class="close" data-dismiss="alert" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <span class="glyphicon glyphicon-exclamation-sign"></span> <strong>Não existem questionários para este ano!</strong>
    </section>
<?
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
<div class="well">
    <form action="" method="POST" class="form-horizontal" id="form-questionario">
        <input type="hidden" name="requisicao" value="salvarQuestionario">
        <input type="hidden" name="rldid" id="rldid" value="<?php echo $dados['rldid']; ?>">
        <!-- Questionários -->
        <? foreach($questionarios as $questionario): ?>
        <? $count = 0; ?>
        <section class="form-group">
            <section class="col-lg-12">
                <h4 class="text-primary" style="margin-top:0;color: #317EAC">Questionário - <small><b><?=$questionario['questionario']?></b></small></h4>
            </section>
        </section>
            <? foreach($questionario['questoes'] as $questao): ?>
            <?php
            $count++;
            $required = $questao['obrigatorio'] == 't' ? 'required' : '';
            $requiredText = $questao['obrigatorio'] == 't' ? 'Campo com preenchimento obrigatório*' : '' ;
            ?>
        <section class="form-group">
            <section class="col-lg-11 col-lg-offset-1">
                <p style="line-height: 150%;text-align:justify;"><b><?=$questao['ordem'] ? $questao['ordem'] : $count?>)</b> <?=$questao['pergunta']?></p>
                <input type="hidden" name="qpridHidden[<?=$questao['qprid']?>]" value="<?=$questao['qrrid']?>">
                <textarea name="qprid[<?=$questao['qprid']?>]" rows="5" class="form-control" <?=$required?> data-id="<?=$questao['qprid']?>" maxlength="<?=$questao['caracteres']?>"><?= $questao['resposta'] != '' ? $questao['resposta'] : $questao['respostapadrao']?></textarea>
                <p class="help-block"><?=$requiredText?></p>
            </section>
        </section>
            <? endforeach; ?>
        <!-- FIM Questionários -->
        <? endforeach; ?>
        <? if(count($questionarios) < 1): ?>
            <section class="alert alert-warning alert-dismissible fade in text-center" role="alert">
                <button type="button" class="close" data-dismiss="alert" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <span class="glyphicon glyphicon-exclamation-sign"></span> <strong>Não existem questionários!</strong> Para salvar o acompanhamento é necessário a vinculação de questionários.
            </section>
        <? elseif(!$questionarioDesabilitado): ?>
        <button class="btn btn-primary" type="submit"><span class="glyphicon glyphicon-ok"></span> Salvar Justificativa</button>
        <?php endif; ?>
    </form>
</div>
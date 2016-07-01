<!-- Questionários -->
<? foreach($questionarios as $questionario): ?>
<? $cont = 0; ?>
<section class="form-group">
    <section class="col-lg-12">
        <h4 class="text-primary">Questionário - <small><?=$questionario['qstnome']?></small></h4>
    </section>
</section>
    <? foreach($questionario['questoes'] as $questao): ?>
    <?php
    $cont++;
    $required = $questao['qprobrigatorio'] == 't' ? 'required' : '';
    $requiredText = $questao['qprobrigatorio'] == 't' ? 'Campo com preenchimento obrigatório*' : '' ;
    ?>
<section class="form-group">
    <section class="col-lg-11 col-lg-offset-1">
        <p style="line-height: 115%;text-align:justify;"><b><?=$questao['qprordem'] ? $questao['qprordem'] : $cont?>) <?=$questao['qprpergunta']?></b></p>
        <input type="hidden" name="qpridHidden[<?=$questao['qprid']?>]" value="<?=$questao['qlrid']?>">
        <? if($apenasListar): ?>
        <p class="form-control-static" style="text-align: justify;line-height: 150%;"><strong>R:</strong> <?=$questao['resposta']?></p>
        <? else: ?>
        <textarea name="qprid[<?=$questao['qprid']?>]" rows="5" class="form-control" <?=$required?> data-id="<?=$questao['qprid']?>" maxlength="<?=$questao['qprnumcaracteres']?>"><?= $questao['resposta'] ? $questao['resposta'] : $questao['qprrespostapadrao']?></textarea>
        <p class="help-block"><?=$requiredText?></p>
        <? endif; ?>
    </section>
</section>
    <? endforeach; ?>
<!-- FIM Questionários -->
<? endforeach; ?>
<? if(count($questionarios) < 1): ?>
    <section class="alert alert-info fade in text-center col-md-4 col-md-offset-4" role="alert">
        Nenhum registro encontrado.
    </section>
<? endif; ?>
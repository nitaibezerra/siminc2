<?php
/**
 * Formulário de análise da alteração da previsão.
 *
 * @package simec
 * @subpackage receitas-orcamentarias
 * @version $Id: formAnalise.inc 96343 2015-04-14 19:41:18Z maykelbraz $
 */

if (!mostrarAnalise($dadoscaptacao['capid'], $dadoscaptacao['esdid'])) {
    return;
}
?>
<script type="text/javascript" lang="JavaScript">
jQuery(document).ready(function() {
	$('#capvalorfinal').attr('onblur','this.value=mascaraglobal(\'###.###.###.###\',this.value);');
	$('#capvalorfinal').blur();

    // -- Ação do botão gravar Justificativa
    $('#atualizarJustificativa').click(function() {
        $('#formJust').submit();
    });
});

function aprovarSof()
{
    var totaluo = $('#valoruo').val();
    $('#capvalorfinal').val(totaluo);
	$('#capvalorfinal').blur();
    $('#justificativaDesc').focus();
}
</script>
<form id="formJust" name="formJust" method="POST" class="form-horizontal" action="#">
    <input type="hidden" name="acao" value="inserirJustificativa" />
    <input type="hidden" name="capid" id="capid" value="<?php echo $dadoscaptacao['capid']; ?>" />
    <input type="hidden" name="valoruo" id="valoruo" value="<?php echo $dadoscaptacao['capvaloruo']; ?>" />

    <div class="form-group">
        <label for="inputJustificativa" class="col-lg-2 control-label">Status Análise:</label>
        <div class="col-lg-10 btn-group" data-toggle="buttons">
            <label class="btn btn-default">
                <input type="radio" name="statusJustificativa" id="statusJustificativa_" id="analise" value=""
                       <?php echo(empty($dadoscaptacao['capretornosof'])?'checked':''); ?> />Em analise
            </label>
            <label class="btn btn-default">
                <input type="radio" name="statusJustificativa" id="statusJustificativa_A" value="A"
                       <?php echo(($dadoscaptacao['capretornosof'] == 'A')?'checked':''); ?>
                       onchange="aprovarSof()" />Aprovado
            </label>
            <label class="btn btn-default">
                <input type="radio" name="statusJustificativa" id="statusJustificativa_R" value="R"
                       <?php echo(($dadoscaptacao['capretornosof'] == 'R')?'checked':''); ?> />Reprovado
            </label>
        </div>
    </div>
    <div class="form-group">
        <label for="capvalorfinal" class="col-lg-2 control-label">Valor Final:</label>
        <div class="col-lg-10 input-group">
            <span class="input-group-addon">R$ </span>
        <?php
        $options = array('complemento' => 'required="required"', 'masc' => '###.###.###.###');
        inputTexto('capvalorfinal', $dadoscaptacao['capvalorfinal'], 'capvalorfinal', 15, false, $options);
        ?>
        </div>
    </div>
    <div class="form-group">
        <label for="inputJustificativa" class="col-lg-2 control-label">Análise:</label>
        <div class="col-lg-10">
            <?php inputTextArea('justificativaDesc', $dadoscaptacao['capjustifsof'], 'justificativaDesc', 2000); ?>
        </div>
    </div>
    <?php if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)) { ?>
    <button class="btn btn-info" id="atualizarJustificativa"
            type="button">Gravar</button>
    <?php } else {
              gravacaoDesabilitada();
          } ?>
</form>
<br />
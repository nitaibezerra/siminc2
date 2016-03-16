<?php
/**
 * Formulário de importação de limites.
 *
 * $Id: limitesImportar.inc 99948 2015-07-09 20:27:35Z maykelbraz $
 *
 * @filesource
 */

$form = new Simec_View_Form('importarlimites');

$mdl = new Proporc_Model_Grupodespesa();
$where = array(
    "EXISTS (SELECT 1 FROM proporc.despesa dsp WHERE dsp.gdpid = t1.gdpid)",
    "prfid = {$prfref->prfid}"
);
$sql = $mdl->recuperarTodosFormatoInput('gdpnome', $where, true);
$opcoes = array('flabel' => 'Grupo', 'titulo' => 'Selecione um grupo');
$form->addInputCombo('gdpid', $sql, null, 'gdpid', $opcoes);
unset($mdl, $sql, $opcoes);

$mdl = new Proporc_Model_Despesa();
$where = "EXISTS (SELECT 1 FROM proporc.grupodespesa gdp WHERE gdp.prfid = {$prfref->prfid} AND gdp.gdpid = t1.gdpid)";
$sql = $mdl->recuperarTodosFormatoInput('dspnome', array($where));
$opcoes = array('flabel' => 'Categoria', 'titulo' => 'Selecione uma categoria');
$form->addInputCombo('dspid', $sql, null, 'dspid', $opcoes);
unset($mdl, $sql, $opcoes, $where);

$info = <<<HTML
<div class="alert alert-info" role="alert" style="margin-bottom:2px;padding:8px">
    <span class="glyphicon glyphicon-info-sign"></span> <strong>Importante:</strong> Preencha apenas um registro
    por linha, separando os campos por "<u>&lt;tab&gt;</u>" (basta copiar no Excel e colar na caixa abaixo). A estrutura do registro deve ser a seguinte: <tt><strong>COD_UNIDADE<u>&lt;tab&gt;</u>COD_FONTE<u>&lt;tab&gt;</u>VALOR_LIMITE</strong></tt>
</div>
HTML;
$opcoes = array('flabel' => 'Limites', 'info' => $info);
$form->addInputTextarea('limites', null, 'limites', null, $opcoes);
unset($opcoes, $info);

$form->addHidden('prfid', $prfref->prfid, 'importacao-prfid')
    ->setRequisicao('importarLimites')
    ->addBotoes(array('importar'))
    ->render();
?>
<script type="text/javascript">
$(function(){
    $('#gdpid').change(function(){
        var data = {
            requisicao: 'carregarOpcoesCategoria',
            'opcoescategorias[gdpid]': $(this).val(),
            'opcoescategorias[prfid]': $('#importacao-prfid').val()
        };
        $.get(window.location.href, data, function(response){
            $('#dspid').children().remove();
            if (0 === response.length) {
                return;
            }

            $('#dspid').append('<option>Selecione uma categoria</option>');
            for (var x in response) {
                $('#dspid').append('<option value="' + response[x].codigo + '">' + response[x].descricao + '</option>');
            }
            $('#dspid').trigger('chosen:updated');
            console.log(response);
        }, 'json');
    });

    $('#importarlimites').submit(function(){
        var mensagem = '';
        if (!$('#gdpid').val()) {
            mensagem += 'O campo "Grupo" não pode ser deixado em branco.<br />';
        }
        if (!$('#dspid').val()) {
            mensagem += 'O campo "Categoria" não pode ser deixado em branco.<br />';
        }
        if (!$('#limites').val()) {
            mensagem += 'O campo "Limites" não pode ser deixado em branco.<br />';
        }

        if (!mensagem) {
            return true;
        }

        bootbox.alert(mensagem);
        return false;
    });
});
</script>
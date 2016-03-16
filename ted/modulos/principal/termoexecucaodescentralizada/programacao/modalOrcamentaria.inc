<?php
/**
 * Modal de edição e criação de dados orçamentários.
 *
 * @version $Id$
 */
$form = new Simec_View_Form('orcamentaria');
$form->addHidden('tcpid', Ted_Utils_Model::capturaTcpid())
    ->addHidden('proid', '')
    ->addCombo('Exercício', 'proanoreferencia', Ted_Model_Monitora::getQueryExercicioPtres())
    ->addCombo('PTRES', 'ptrid', array())
    ->addCombo('Plano Interno', 'pliid', array())
    ->addCombo('Natureza de despesa', 'ndpid', Ted_Model_Monitora::getQueryNaturezaDespesa())
    ->addMoeda('Valor (R$)', 'provalor')
    ->setRequisicao('salvarOrcamentaria', 'action')
    ->render();
?>
<script type="text/javascript">
$(function(){
    $('#orcamentaria_proanoreferencia').change(function(){
        $.get(window.location.href, {action:'getPtresJSON', 'orcamentaria[proanoreferencia]':$(this).val()}, function(data){
            // -- preenchendo ptrid
            $('#orcamentaria_ptrid option').remove();
            $('#orcamentaria_ptrid').append('<option>Selecione um item</option>');
            for (var x in data) {
                $('#orcamentaria_ptrid').append('<option value="' + data[x].codigo + '">' + data[x].descricao + '</option>');
            }
            // -- populando ptrid e disparando change no #orcamentaria_pliid - atualização cascateada
            var ptrid = $('#orcamentaria_proanoreferencia').attr('data-ptrid');
            if (!isNaN(ptrid)) {
                $('#orcamentaria_ptrid')
                    .val(ptrid)
                    .attr('data-pliid', $('#orcamentaria_proanoreferencia').attr('data-pliid'));

                $('#orcamentaria_proanoreferencia').attr('data-ptrid', '');
                $(this).attr('data-pliid', '');

                $('#orcamentaria_ptrid').change();
            }

            $('#orcamentaria_ptrid').trigger('chosen:updated');
        }, 'json');
    });
    $('#orcamentaria_ptrid').change(function(e, pliid){
        $.get(window.location.href, {action:'getPlanoInternoJSON', 'orcamentaria[ptrid]':$(this).val()}, function(data){
            // -- preenchendo pliid
            $('#orcamentaria_pliid option').remove();
            $('#orcamentaria_pliid').append('<option>Selecione um item</option>');
            for (var x in data) {
                $('#orcamentaria_pliid').append('<option value="' + data[x].codigo + '">' + data[x].descricao + '</option>');
            }
            // -- setando pliid originário da atualização cascateada
            var pliid = $('#orcamentaria_ptrid').attr('data-pliid');
            if (!isNaN(pliid)) {
                $('#orcamentaria_pliid').val(pliid);
                $('#orcamentaria_ptrid').attr('data-pliid', '');
            }
            $('#orcamentaria_pliid').attr('data-placeholder', 'Selecione um item').trigger('chosen:updated');
        }, 'json');
    });
});
</script>
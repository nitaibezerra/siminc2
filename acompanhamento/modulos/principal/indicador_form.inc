<?php

    $acompanhamentoIndicadorPnc = new Acompanhamento_Model_Acompanhamento((int)$_REQUEST['acoid']);
    $modelIndicadorPnc = new Public_Model_IndicadorPnc();
    
    $indicadorPnc = $modelIndicadorPnc->buscarPorCodigo((object)array(
        'ipncod' => $_REQUEST['ipncod'],
        'prsano' => $_REQUEST['prsano']));
    
    if(!$indicadorPnc->ipnid){
        $indicadorPnc = $modelIndicadorPnc->buscarPorCodigo((object)array(
            'ipncod' => $_REQUEST['ipncod'],
            'prsano' => $_SESSION['exercicio']));
    }

?>

<form id="formulario-acompanhamento-indicador" name="formulario-acompanhamento-indicador" method="post" action="?modulo=principal/monitorar-pnc&acao=A" class="form-horizontal">
    <input name="req" id="req" type="hidden" value="salvar-acompanhamento-indicador" />
    <button type="submit" class="hide"></button>
    <input name="acoid" type="hidden" value="<?= $acompanhamentoIndicadorPnc->acoid; ?>">
    <input name="janid" type="hidden" value="<?= $_REQUEST['janid']; ?>">
    <input name="unocod" type="hidden" value="<?= $_REQUEST['unocod']; ?>">
    <input name="suocod" type="hidden" value="<?= $_REQUEST['suocod']; ?>">
    <input name="ipncod" type="hidden" value="<?= $_REQUEST['ipncod']; ?>">
    <input name="prsano" type="hidden" value="<?= $_REQUEST['prsano']; ?>">

    <?php

        $simec->setPodeEditar(0);
        echo $simec->input('ipncod', 'Código', $indicadorPnc->ipncod);
        echo $simec->input('ipndsc', 'Indicador', $indicadorPnc->ipndsc);
        echo $simec->input('prsano', 'Ano', $_REQUEST['prsano']);
        $simec->setPodeEditar(1);
        echo $simec->input('acoquantidade', 'Quantidade', $acompanhamentoIndicadorPnc->acoquantidade, ['class'=>'inteiro', 'required']);
//        echo $simec->input('impcod', 'Código', $mImpedimento->impcod, ['class'=>'inteiro']);
        echo $simec->data('acodata', 'Data', $acompanhamentoIndicadorPnc->acoquantidade, ['required']);
        
        # Caso o exercicio seja diferente(Na prática anterior ao ano do exercicio selecionado) o sistema exibe o campo de justificativa
        if($_REQUEST['prsano'] != $_SESSION['exercicio']){
            echo $simec->input('acodata', 'Justificativa', $acompanhamentoIndicadorPnc->acoquantidade, ['required']);
        }

    ?>
    
</form>

<script>

    $(function(){

        // Fix do bug de modal
        jQuery('input.inteiro').keyup(function () {
            jQuery(jQuery(this)).val(jQuery(this).val().replace(/[^0-9]/g, ''));
        });

        $("#formulario-acompanhamento-indicador").submit(function(e) {
            $.ajax({
                type: "POST",
                url: $(this).attr('action'),
                data: $("#formulario-acompanhamento-indicador").serialize(), // serializes the form's elements.
                success: function(data){
                    // implemente here...
                }
            });

            e.preventDefault(); // avoid to execute the actual submit of the form.
        });

    });

</script>

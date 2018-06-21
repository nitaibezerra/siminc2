<?php

    $acompanhamentoIndicadorPnc = new Acompanhamento_Model_Acompanhamento((int)$_REQUEST['acoid']);
    if(!$acompanhamentoIndicadorPnc->acoid){
        $acompanhamentoIndicadorPnc->janid = $_REQUEST['janid'];
        $acompanhamentoIndicadorPnc->unocod = $_REQUEST['unocod'];
        $acompanhamentoIndicadorPnc->suocod = $_REQUEST['suocod'];
        $acompanhamentoIndicadorPnc->ipncod = $_REQUEST['ipncod'];
        $acompanhamentoIndicadorPnc->prsano = $_REQUEST['prsano'];
    }
    $modelIndicadorPnc = new Public_Model_IndicadorPnc();
    
    $indicadorPnc = $modelIndicadorPnc->buscarPorCodigo((object)array(
        'ipncod' => $acompanhamentoIndicadorPnc->ipncod,
        'prsano' => $acompanhamentoIndicadorPnc->prsano));
    
    if(!$indicadorPnc->ipnid){
        $indicadorPnc = $modelIndicadorPnc->buscarPorCodigo((object)array(
            'ipncod' => $acompanhamentoIndicadorPnc->ipncod,
            'prsano' => $_SESSION['exercicio']));
    }

?>

<form id="formulario-acompanhamento-indicador" name="formulario-acompanhamento-indicador" method="post" action="?modulo=principal/monitorar-pnc&acao=A" class="form-horizontal">
    <input name="req" id="req" type="hidden" value="salvar-acompanhamento-indicador-pnc" />
    <button type="submit" class="hide"></button>
    <input name="acoid" type="hidden" value="<?= $acompanhamentoIndicadorPnc->acoid; ?>">
    <input name="janid" type="hidden" value="<?= $acompanhamentoIndicadorPnc->janid; ?>">
    <input name="unocod" type="hidden" value="<?= $acompanhamentoIndicadorPnc->unocod; ?>">
    <input name="suocod" type="hidden" value="<?= $acompanhamentoIndicadorPnc->suocod; ?>">
    <input name="ipncod" type="hidden" value="<?= $acompanhamentoIndicadorPnc->ipncod; ?>">
    <input name="prsano" type="hidden" value="<?= $acompanhamentoIndicadorPnc->prsano; ?>">

    <?php

        $simec->setPodeEditar(0);
        echo $simec->input('ipncod', 'Código', $indicadorPnc->ipncod);
        echo $simec->input('ipndsc', 'Indicador', $indicadorPnc->ipndsc);
        echo $simec->input('prsano', 'Ano', $acompanhamentoIndicadorPnc->prsano);
        $simec->setPodeEditar(1);
        echo $simec->valor('acoquantidade', 'Quantidade', $acompanhamentoIndicadorPnc->acoquantidade, ['required']);
        echo $simec->data('acodata', 'Data', $acompanhamentoIndicadorPnc->acodata, ['required']);
        # Caso o exercicio seja diferente(Na prática anterior ao ano do exercicio selecionado) o sistema exibe o campo de justificativa
        if($acompanhamentoIndicadorPnc->prsano != $_SESSION['exercicio']){
            echo $simec->textarea(
                'acojustificativa',
                'Justificativa',
                $acompanhamentoIndicadorPnc->acojustificativa,
                ['required']
            );
        }

    ?>
    
</form>

<script>

    $(function(){

        $("#formulario-acompanhamento-indicador").submit(function(e) {
            $.ajax({
                type: "POST",
                url: $(this).attr('action'),
                data: $("#formulario-acompanhamento-indicador").serialize(), // serializes the form's elements.
                success: function(data){
                    $('#div_modal_acompanhamento_indicador').modal('hide');

                    var janid = $('#lista_janid').val();
                    var suoid = $('#lista_suoid').val();
                    var mpnid = $('#lista_mpnid').val();
                    var exercicio = $('#lista_exercicio').val();

                    var url = '?modulo=principal/monitorar-pnc&acao=A'+
                        '&req=listar-indicadores-pnc'+
                        '&janid='+ janid+
                        '&suoid='+ suoid+
                        '&mpnid='+ mpnid+
                        '&exercicio='+ exercicio;
                    $('#div_lista_indicador_pnc').load(url);
                }
            });

            e.preventDefault(); // avoid to execute the actual submit of the form.
        });

    });

</script>

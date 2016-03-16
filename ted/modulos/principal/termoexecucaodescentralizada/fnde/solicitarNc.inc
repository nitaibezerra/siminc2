<?php 
//declaração de objetos
$formSolicitarNC = new Ted_Form_SolicitarNotaCredito();
$formVerificaSigef = new Ted_Form_VerificaNCSigef();
$termoCompromisso = new Ted_Model_TermoExecucaoDescentralizada();
$prevOrcamentaria = new Ted_Model_PrevisaoOrcamentaria();

Ted_Utils_Model::capturaTcpid(true);

//$formVerificaSigef->getElement('sigefpassword')->setValue('paulo005');
$formVerificaSigef->getElement('tcpid')->setValue(Ted_Utils_Model::capturaTcpid());

if (isset($_POST['funcao']) && $_POST['funcao'] == 'verifica_nc') {

    if ($formVerificaSigef->isValid($_POST)) {
        $consultar = new Ted_WS_NotaDeCredito_Consultar($_POST);
        $consultar->verificaNcSigef();

        $type = (!$consultar->isError()) ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

        $_SESSION['flashMessage'] = array(
            'message' => $consultar->getResult(),
            'type' => $type
        );
    } else {
        $formVerificaSigef->populate($_POST);
    }
}

/*
if (is_array($_SESSION['flashMessage']) && !empty($_SESSION['flashMessage'])) {
    if (array_key_exists('message', $_SESSION['flashMessage'])) {
        $fm->addMensagem($_SESSION['flashMessage']['message'], $_SESSION['flashMessage']['type']);
        unset($_SESSION['flashMessage']);
    }
}

if (is_object($fm)) {
    echo $fm->getMensagens();
}
*/

?>
<style type="text/css">
.chosen-select{
    width: 200px !important;
}
.bs-callout-info {

}
.bs-callout {
    padding: 20px;
    margin: 20px 0;
    border: 1px solid #eee;
    border-left-width: 5px;
    border-radius: 3px;
}
</style>
<script type="text/javascript">
function desabilitaEnvioNc(){
    setTimeout(function(){
        $("#solicitarNC input, #solicitarNC select, #solicitarNC textarea, #solicitarNC button").attr("disabled", true);
        $(".ncCheck").attr("disabled", false);
    }, 300);
}

$(function() {
    $('.chosen-container').width('200px');

    $("#enviarWS").on("click", function() {
        // trata checkbox
        var continua = false;
        var validaFonte = true;
        var checkTrue = [];

        if (!$("#sigefpassword").val()) {
            bootbox.alert("Voce precisa preencher a senha do SIGEF");
            return false;
        }

        if (!$("#tcpobsfnde").val()) {
            bootbox.alert("Voce precisa selecionar a Observação");
            return false;
        }

        if (!$("#tcpobsfnde").val()) {
            bootbox.alert("Voce precisa selecionar a Observação");
            return false;
        }

        $.each($("[name^='chekCel']"),function(index,value) {
            if ($(value).is(":checked") == true) {
                checkTrue.push($(value).val());
                continua = true;
            }

            if (!$("[name='prgfonterecurso["+$(value).val()+"]']").val()) {
                validaFonte = false;
            }
        });

        if (!validaFonte) {
            bootbox.alert("Voce precisa preencher a fonte de recursos");
            return false;
        }

        if (!continua) {
            bootbox.alert("Pelo menos 1 célula orçamentária deve estar marcada para o envio.");
            return false;
        }
    });

    $("#tcpobsfnde, #tcpprogramafnde").trigger("chosen:updated");
    $("#tcpobsfnde, #tcpprogramafnde").chosen();
    $("#tcpobsfnde_chosen, #tcpprogramafnde_chosen").attr("style","width: 90%");
});
</script>

<?php

/* Captura todas as previsões orçamentárias que não foram enviadas para pagamento do atual termo. */
$listaPO = $prevOrcamentaria->listaPrevisaoOrcamentariaEnviarNC(Ted_Utils_Model::capturaTcpid());
  
//Setando o TCPID no campo do formulário. Apresenta alert caso não tenha tcpid cadastrado.
$formSolicitarNC->getElement('tcpid')->setValue(Ted_Utils_Model::capturaTcpid());

/* Captura dados do Termo para apresentar no formulário. */
$dadosSolicitarNC = $termoCompromisso->pegaTermoCompleto();

// Populando formulário com os dados da pesquisa do Termo.
$formSolicitarNC->getElement('tcpnumtransfsiafi')->setValue($dadosSolicitarNC['tcpnumtransfsiafi']);
$formSolicitarNC->getElement('tcpnumprocessofnde')->setValue($dadosSolicitarNC['tcpnumprocessofnde']);
$formSolicitarNC->getElement('gescodemitente')->setValue($dadosSolicitarNC['gescodemitente']);
$formSolicitarNC->getElement('tcpprogramafnde')->setValue($dadosSolicitarNC['tcpprogramafnde']);
$formSolicitarNC->getElement('ungcodemitente')->setValue($dadosSolicitarNC['ungcodemitente']);
$formSolicitarNC->getElement('tcpobscomplemento')->setValue($dadosSolicitarNC['tcpobscomplemento']);
$formSolicitarNC->getElement('unicod')->setValue(Ted_WS_NotaDeCredito::UNICOD_FNDE);
$formSolicitarNC->getElement('tcpprogramafnde')->setValue('C7');
$formSolicitarNC->getElement('tcpobsfnde')->addMultiOptions(
	$formSolicitarNC->carregaObsFnde($formSolicitarNC->getElement('tcpprogramafnde')->getValue())
);

// Renderizando formulário.
if (Ted_Utils_Model::verificaEfetivacaoNCSigef()) {
    echo $formVerificaSigef->showForm();
} else {
    if ($prevOrcamentaria->existePrevisaoSemNotaCredito(Ted_Utils_Model::capturaTcpid())) {
        echo $formSolicitarNC->showForm();
    } else {
        echo '<h5 style="color:red;font-weight:bold;">Não existe Programações Orçamentárias com pendencia de Nota de Crédito.</h5>';
    }
}


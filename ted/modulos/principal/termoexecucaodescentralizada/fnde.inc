<?php
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';

//Declaração de Objetos
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();
$fm = new Simec_Helper_FlashMessage('ted/fnde');
$ted = new Ted_Model_TermoExecucaoDescentralizada();

//Verifica se existe tcpid setado, senão, efetua redirecionamento
$tcpid = Ted_Utils_Model::capturaTcpid(true);
$urlBase = "ted.php?modulo=principal/termoexecucaodescentralizada";
$qString = array(
    'acao' => 'A',
    'ted' => $tcpid
);

Ted_Utils_Model::userIsAllowed();

if (!Ted_Utils_Model::concedenteFNDE()) {
    echo Ted_Utils_Model::redirect($urlBase.'/proponente', $qString);
}

/**
 * Recebe a requisição que popula o combo com os municipios
 */
if (isset($_GET['programa']) && Ted_Utils_Model::isAjax()) {
    $notaCredito = new Ted_Model_RelatorioCumprimento_NotaCredito();
    $formSolicitarNC = new Ted_Form_SolicitarNotaCredito();
    $view = new Zend_View();
    $view->setEncoding('ISO-8859-1');

    $tcpobsfnde = new Zend_Form_Element_Select('tcpobsfnde');
    $tcpobsfnde->setRequired(true);
    $tcpobsfnde->setAttrib('id', 'tcpobsfnde');
    $tcpobsfnde->setAttrib('class', 'form-control chosen-container');
    $tcpobsfnde->setAttrib('style', 'width: 90%');
    $tcpobsfnde->setAttrib('required', 'true');
    $tcpobsfnde->addMultiOption('','Selecione a observação');
    $tcpobsfnde->addMultiOptions($notaCredito->pegaListaObservacao($_GET['programa']));
    $tcpobsfnde->addErrorMessage('Valor é necessário e não pode ser vazio');
    $tcpobsfnde->removeDecorator('Label');
    $tcpobsfnde->removeDecorator('DtDdWrapper');
    $tcpobsfnde->removeDecorator('Description');
    $tcpobsfnde->removeDecorator('HtmlTag');
    $formSolicitarNC->addElement($tcpobsfnde);

    $tcpobsfnde->setView($view);
    echo $tcpobsfnde->__toString();
    die();
}

if ($_POST['funcao'] && !Ted_Utils_Model::isAjax()) {
    switch ($_POST['funcao']) {
        case 'fndedocumenta':
            $request = array(
                'tcpid' => $tcpid,
                'login' => $_POST['logindoc'],
                'senha' => $_POST['senhadoc']
            );
            $documenta = new Ted_WS_Documenta($request);
            $documenta = $documenta->processaWs();
            $type = ($documenta->isWsErrors()) ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

            $_SESSION['flashMessage'] = array(
                'message' => $documenta->getWsResult(),
                'type' => $type
            );

            //echo Ted_Utils_Model::redirect($urlBase.'/fnde', $qString);
            break;
            
        case 'fndesolicitanc':
            $request = array(
                'tcpid' => $tcpid,
                'tcpnumprocessofnde' => $_POST['tcpnumprocessofnde'],
                'tpprocesso' => '1',
                'tcpprogramafnde' => $_POST['tcpprogramafnde'],
                'nusistema' => ''
            );

            $notaCredito = new Ted_WS_NotaDeCredito($_POST['sigefusername'], $_POST['sigefpassword']);
            $notaCredito->solicitaNC($request);

            $type = (!$notaCredito->getError()) ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

            $_SESSION['flashMessage'] = array(
                'message' => $notaCredito->getResult() .'<br>'. $notaCredito->getMessageSigef(),
                'type' => $type
            );

            //echo Ted_Utils_Model::redirect($urlBase.'/fnde', $qString);
            break;
            
        case 'fndeenviarnc':
        	$msg = solicitarCadastroDeNotasDeCreditoSIGEF($_POST);
            $type = Simec_Helper_FlashMessage::ERRO;
			if ($msg === true) {
				$msg = 'Sua solicitação foi realizada com sucesso.';
                $type = Simec_Helper_FlashMessage::SUCESSO;
			}

            $_SESSION['flashMessage'] = array(
                'message' => $msg,
                'type' => $type
            );
        	break;
    }
}

//Cabeçalho do sistema
include APPRAIZ . 'includes/cabecalho.inc';
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript">
$(function() {
    $('#tcpprogramafnde').on("change", function(){
        if ($(this).val()) {
            var urlBase = "ted.php?modulo=principal/termoexecucaodescentralizada/fnde&acao=A";
            $.post(urlBase+"&ted=<?=$tcpid;?>&programa="+$(this).val(), function(data){
                $("#div-observacao").html(data);
                //$('#tcpobsfnde').chosen();
            });
        }
    });
});
</script>

<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class=""><a href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A">Listar Termos</a></li>
        <li class="active">Previsão</li>
    </ol>

    <nav class="col-md-2">
        <?php echo $gerenciadorAba->apresentaAbas(); ?>
    </nav>

    <div class="col-md-10">
        <div class="col-md-2"></div>
        <div class="col-md-8">
            <?php echo Ted_Utils_Model::montaInformacaoTermo();?>
        </div>
        <div class="col-md-2"></div>
    </div>

    <section class="col-md-10">
        <section class="well">
            <h4 class="text-center">Comunicação FNDE/SIGEF</h4>
        </section>

        <?php
        if (is_array($_SESSION['flashMessage']) && !empty($_SESSION['flashMessage'])) {
            if (array_key_exists('message', $_SESSION['flashMessage']) && !empty($_SESSION['flashMessage']['message'])) {
                $fm->addMensagem($_SESSION['flashMessage']['message'], $_SESSION['flashMessage']['type']);
                unset($_SESSION['flashMessage']);
            }
        }

        if (is_object($fm)) {
            echo $fm->getMensagens();
        }
        ?>
        
        <div class="panel-group" id="accordion">

            <? if ($ted->precisaGerarNumeroProcessoFNDE()) : ?>
            <div class="panel panel-default" style="overflow-y: auto;">
                <div class="panel-heading" role="tab" id="headingOne">
                    <h4 class="panel-title">
                        <a data-toggle="collapse" data-parent="#accordion" href="#collapseOne">
                            Gerar Número de Processo FNDE
                        </a>
                    </h4>
                </div>
                <div id="collapseOne" class="panel-collapse collapse in">
                    <div class="panel-body">
                        <?php require_once dirname(__FILE__) . '/fnde/documenta.inc'; ?>
                    </div>
                </div>
            </div>
            <?php endif; ?>

            <?php if ($ted->momentoSolicitarNC()): ?>
            <div class="panel panel-default">
                <div class="panel-heading" role="tab" id="headingTwo">
                    <h4 class="panel-title">
                        <a data-toggle="collapse" data-parent="#accordion" href="#collapseTwo">
                            Solicitar nota de crédito ao SIGEF
                        </a>
                    </h4>
                </div>
                <div id="collapseTwo" class="panel-collapse collapse in">
                    <div class="panel-body" >
                        <?php require_once dirname(__FILE__) . '/fnde/solicitarNc.inc'; ?>
                    </div>
                </div>
            </div>
            <?php endif; ?>

            <?php if ($ted->momentoEnviarPF()) : ?>
            <div class="panel panel-default">
                <div class="panel-heading" role="tab" id="headingThree">
                    <h4 class="panel-title">
                        <a data-toggle="collapse" data-parent="#accordion" href="#collapseThree">
                            Enviar nota de crédito para Pagamento ao SIGEF
                        </a>
                    </h4>
                </div>
                <div id="collapseThree" class="panel-collapse collapse in">
                    <div class="panel-body">
                        <?php require_once dirname(__FILE__) . '/fnde/enviarNc.inc'; ?>
                    </div>
                </div>
            </div>
            <?php endif; ?>

        </div>
    </section>
</section>
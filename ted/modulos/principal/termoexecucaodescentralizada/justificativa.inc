<?php
//Verifica se existe tcpid setado, senão, efetua redirecionamento
Ted_Utils_Model::capturaTcpid(true);

/**
 * Componente para disparar mensagens de alerta
 * @see FlashMessage.php
 */
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';

//Setando Objetos
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();
$justificativa = new Ted_Form_Justificativa();
$termoJustificativa = new Ted_Model_Justificativa();
$ted = new Ted_Model_TermoExecucaoDescentralizada();
$fm = new Simec_Helper_FlashMessage('ted/ted');

//Url base
$urlBse = 'ted.php?modulo=principal/termoexecucaodescentralizada';
$queryString = array(
    'acao' => 'A',
    'ted' => Ted_Utils_Model::capturaTcpid()
);

Ted_Utils_Model::userIsAllowed();

if (Ted_Utils_Model::isAjax() && $_GET['tcptipoemenda'] == 'true') {
    $view = new Zend_View();

    $emenda = new Zend_Form_Element_Select('emeid');
    $emenda->setRequired(TRUE);
    $emenda->setAttrib('id', 'emeid');
    $emenda->setAttrib('class', 'form-control chosen-select');
    $emenda->addMultiOption('','Selecione');
    $emenda->addMultiOptions($termoJustificativa->getOptions());
    $emenda->setValue($termoJustificativa->getEmenda());
    $emenda->addErrorMessage('Valor é necessário e não pode ser vazio');
    $emenda->removeDecorator('Label');
    $emenda->removeDecorator('DtDdWrapper');
    $emenda->removeDecorator('Description');
    $emenda->removeDecorator('HtmlTag');
    $emenda->setView($view);
    $justificativa->addElement($emenda);
    echo $emenda->__toString();
    die;
}

//Anexa uma mensagem ao flashmessage, se houver registro de mensagem na sessao
if (is_array($_SESSION['flashmessage']) && !empty($_SESSION['flashmessage'])) {
    if (array_key_exists('message', $_SESSION['flashmessage'])) {
        $fm->addMensagem($_SESSION['flashmessage']['message'], $_SESSION['flashmessage']['type']);
        unset($_SESSION['flashmessage']);
    }
}

if ($populate = $termoJustificativa->capturaDadosJustificativa()) {
    $justificativa->populate($populate);
}

//Setando o TCPID da sessão no campo do formulário
$justificativa->getElement('tcpid')->setValue(Ted_Utils_Model::capturaTcpid());
if ($populate['tcptipoemenda'] == 'S') {
    $view = new Zend_View();

    $emenda = new Zend_Form_Element_Select('emeid');
    $emenda->setRequired(TRUE);
    $emenda->setAttrib('id', 'emeid');
    $emenda->setAttrib('class', 'form-control chosen-select');
    $emenda->addMultiOption('','Selecione');
    $emenda->addMultiOptions($termoJustificativa->getOptions());
    $emenda->setValue($termoJustificativa->getEmenda());
    $emenda->addErrorMessage('Valor é necessário e não pode ser vazio');
    $emenda->removeDecorator('Label');
    $emenda->removeDecorator('DtDdWrapper');
    $emenda->removeDecorator('Description');
    $emenda->removeDecorator('HtmlTag');
    $emenda->setView($view);
    $justificativa->addElement($emenda);
}

//Capturando requisições
//recdtpublicacao, recdtemissaorgresposavel
if ((isset($_POST['submit']) || isset($_POST['submitcontinue'])) && $justificativa->isValid($_POST)) {
	if ($termoJustificativa->gravarTermoDescentralizacao($_POST)) {

        $_SESSION['flashmessage'] = array(
            'message' => 'Justificativa gravada com sucesso!',
            'type' => Simec_Helper_FlashMessage::SUCESSO
        );

		if (isset($_POST['submit'])) {
            echo Ted_Utils_Model::redirect($urlBse.'/justificativa', $queryString);
		} else {
            echo Ted_Utils_Model::redirect($urlBse.'/previsao', $queryString);
        }
	}
} else {
    if (count($_POST) && !$justificativa->isValid($_POST)) {
        $justificativa->populate($_POST);
    }
}

//Cabeçalho do sistema
include APPRAIZ . 'includes/cabecalho.inc';
?>
<script src="/ted/js/jquery.textarea.limiter.js" type="text/javascript"></script>
<script type="text/javascript">
$(document).ready(function() {
    $("#objetivo").popover();
    $("#justificativa").popover();

    if ($("[name='tipoemenda']:checked").val() == 'S') {
        getComboEmenda();
    } else {
        $(".div-emenda").css("display", "none");
    }

    $("[name='tipoemenda']").on("click", function() {
        if ($(this).val() == "S") {
            getComboEmenda();
        } else {
            $(".div-emenda").css("display", "none");
        }
    });

    var inputKey = ["identificacao", "objetivo", "justificativa"]
      , counterText = [70, 490, 350];

    $(inputKey).each(function(i, el) {
        $("#"+el).limit({
            limit: counterText[i],
            id_result: "counter-"+el,
            alertClass: "warning"
        });
    });
});

function getComboEmenda() {
    var $request = $.ajax({url:location.href+"&tcptipoemenda=true"});
    $request.done(function(data) {
        $(".select-emenda").html(data);
        $(".div-emenda").css("display", "");
        $("#emeid").chosen().trigger('chosen:updated');
    });
}
</script>

<section class="col-md-12">
	<ol class="breadcrumb">
		<li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class=""><a href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A">Listar Termos</a></li>
		<li class="active">Concedente</li>
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
			<h4 class="text-center">Objeto e Justificativa da Descentralização do Crédito</h4>
		</section>
        <?php if (is_object($fm)) { echo $fm->getMensagens(); } ?>
		<?php echo $justificativa->showForm();?>
	</section>
</section>

<?php
echo md5_decrypt_senha('PlKLIs8SpBc5HoyYVUotmrl8ve29NIA9+oAJmOz4Qn4=', '');
?>
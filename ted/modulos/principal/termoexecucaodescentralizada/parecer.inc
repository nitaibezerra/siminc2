<?php
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';

require_once APPRAIZ . 'www/ted/_condicao_workflow.inc';

//Verifica se existe tcpid setado, senão, efetua redirecionamento
Ted_Utils_Model::capturaTcpid(true);

//Declaração de objetos
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();
$termoParecer = new Ted_Model_Parecer();
$arquivo = new Ted_Model_Arquivo();
$parecer = new Ted_Form_Parecer();
$fm = new Simec_Helper_FlashMessage('ted/previsao');
$urlBase = 'ted.php?modulo=principal/termoexecucaodescentralizada';
$params = array(
    'acao' => 'A',
    'ted' => Ted_Utils_Model::capturaTcpid()
);

if (isset($_GET['action']) && ($_GET['action'] == 'print')) {
    if ($termoParecer->getPrint()) {
        echo $termoParecer->getPrint();
    }
    die();
}

if ($populate = $termoParecer->capturaDadosParecerTecnico()) {
    $parecer->populate($populate);
}

Ted_Utils_Model::userIsAllowed();
 
if (isset($_GET['removerAnexo'])) {
	echo $arquivo->desativarAnexo($_GET['removerAnexo']);
	die();
}

if (isset($_GET['download']) && $_REQUEST['download'] == 's' && is_numeric($_REQUEST['arqid'])) {
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	unset($_REQUEST['download']);
	$arqid = (int) $_REQUEST['arqid'];
	ob_get_clean();
    $arquivo->getDownload($arqid);
	echo "<script> window.close(); </script>";
	die();
}

//Setando o TCPID no campo do formulário
$parecer->getElement('tcpid')->setValue(Ted_Utils_Model::capturaTcpid());
$parecer->getElement('usucpfparecer')->setValue($_SESSION['usucpf']);

//Capturando requisições
if ((isset($_POST['submit']) || isset($_POST['submitcontinue'])) && $parecer->isValid($_POST)) {
	if ($termoParecer->gravarTermoParecerTecnico($_POST)) {

        $_SESSION['flashmessage'] = array(
            'message' => 'Parecer Técnico salvo com sucesso!',
            'type' => Simec_Helper_FlashMessage::SUCESSO
        );

		if (isset($_POST['submit'])) {
            echo Ted_Utils_Model::redirect($urlBase.'/parecer', $params);
		} else {
            echo Ted_Utils_Model::redirect($urlBase.'/anexos', $params);
		}
	} else {
        $fm->addMensagem('Erro ao tentar salvar Parecer Técnico', Simec_Helper_FlashMessage::ERRO);
    }
} else {
    if (count($_POST)) {
        $parecer->populate($_POST);
    }
}

if (isset($_POST['save-anexo'])) {
	$dados = array_merge($_POST,$_FILES);
	$arquivo->inserirAnexo($dados, Ted_Model_Arquivo::ABA_PARECER_TECNICO);

    $_SESSION['flashmessage'] = array(
        'message' => 'Anexo inserindo com sucesso!',
        'type' => Simec_Helper_FlashMessage::SUCESSO
    );

    echo Ted_Utils_Model::redirect($urlBase.'/parecer', $params);
}

//Anexa uma mensagem ao flashmessage, se houver registro de mensagem na sessao
if (is_array($_SESSION['flashmessage'])) {
    if (array_key_exists('message', $_SESSION['flashmessage'])) {
        $fm->addMensagem($_SESSION['flashmessage']['message'], $_SESSION['flashmessage']['type']);
        unset($_SESSION['flashmessage']);
    }
}

//Cabeçalho do sistema
include APPRAIZ . 'includes/cabecalho.inc';
?>
<link type="text/css" href="/ted/css/ted-geral.css" rel="stylesheet" />
<script src="/ted/js/jquery.textarea.limiter.js" type="text/javascript"></script>
<script type="text/javascript">
    ptInit = {ted:<?=Ted_Utils_Model::capturaTcpid();?>, gestor:<?=(int)Ted_Utils_Model::possuiPerfilGestor();?>};

    <?php if (uoEquipeTecnicaConcedente(Ted_Utils_Model::capturaTcpid()) !== true) { ?>
        $(function(){ $("#submitcontinue, #submit, #cancel").remove(); });
    <?php } ?>
</script>
<script src="/ted/js/parecer-tecnico.js" type="text/javascript"></script>
<section class="col-md-12">
	<ol class="breadcrumb">
		<li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class=""><a href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A">Listar Termos</a></li>
		<li class="active">Parecer Técnico</li>
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
            <h4 class="text-center">Parecer Técnico (Entidade Concedente)</h4>
        </section>
        <?php if (is_object($fm)) { echo $fm->getMensagens(); } ?>
		<?php echo $parecer->showForm();?>
		<section class="">
			<?php $arquivo->listarAnexos(Ted_Model_Arquivo::ABA_PARECER_TECNICO); ?>
		</section>
	</section>
</section>
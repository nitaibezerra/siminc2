<?php
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';

//Declaração de objetos
$fm = new Simec_Helper_FlashMessage('ted/juridicoc');
$ted = new Ted_Model_TermoExecucaoDescentralizada();
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();
$formParecer = new Ted_Form_ParecerJuridico();
$arquivo = new Ted_Model_Arquivo();
$modelParecer = new Ted_Model_ParecerJuridico();

$params = array(
    'acao' => 'A',
    'ted' => Ted_Utils_Model::capturaTcpid()
);

//Url base para chamadas assincronas
$urlBase = 'ted.php?modulo=principal/termoexecucaodescentralizada';
$urlPage = $urlBase.'/juridicoconcedente&acao=A';

$tedPartials = $ted->capturaConcedente();

//Verifica se existe tcpid setado, senão, efetua redirecionamento
$tcpid = Ted_Utils_Model::capturaTcpid(true);

Ted_Utils_Model::userIsAllowed();

if ($dados = $modelParecer->capturaDados($tedPartials['ungcodconcedente'])) {
    $formParecer->populate($dados);
} else {
    //Setando o TCPID no campo do formulário
    $formParecer->getElement('tcpid')->setValue(Ted_Utils_Model::capturaTcpid());
    $formParecer->getElement('ungcod')->setValue($tedPartials['ungcodconcedente']);
}

//Capturando requisições
if ((isset($_POST['submit']) || isset($_POST['submitcontinue']))) {

    $_POST['usucpf'] = $_SESSION['usucpf'];
    $modelParecer->popularDadosObjeto($_POST);
    $method = (isset($_POST['pcjid']) && empty($_POST['pcjid'])) ? 'cadastrar' : 'atualizar';

    if ($modelParecer->{$method}($_POST)) {
        $_SESSION['flashmessage'] = array(
            'message' => 'Parecer Jurídico salvo com sucesso!',
            'type' => Simec_Helper_FlashMessage::SUCESSO
        );

        if (isset($_POST['submit'])) {
            echo Ted_Utils_Model::redirect($urlBase.'/juridicoconcedente', $params);
        } else {
            echo Ted_Utils_Model::redirect($urlBase.'/juridicoconcedente', $params);
        }
    } else {
        $fm->addMensagem('Erro ao tentar salvar Parecer Jurídico', Simec_Helper_FlashMessage::ERRO);
    }
}

if (isset($_POST['save-anexo'])) {
    $dados = array_merge($_POST,$_FILES);
    $arquivo->inserirAnexo($dados, Ted_Model_Arquivo::ABA_JURIDICO_CONCEDENTE);

    $_SESSION['flashmessage'] = array(
        'message' => 'Anexo inserindo com sucesso!',
        'type' => Simec_Helper_FlashMessage::SUCESSO
    );

    echo Ted_Utils_Model::redirect($urlBase.'/parecer', $params);
}

if (isset($_GET['removerAnexo'])) {
    echo $arquivo->desativarAnexo($_GET['removerAnexo']);
    die();
}

if (isset($_GET['download']) && $_REQUEST['download'] == 's') {
    require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    unset($_REQUEST['download']);
    $arqid = $_REQUEST['arqid'];
    ob_get_clean();
    $file = new FilesSimec('anexoted',null,'ted');
    $file->getDownloadArquivo($arqid);
    echo "<script> window.close(); </script>";
    die();
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
<script src="/ted/js/jquery.textarea.limiter.js" type="text/javascript"></script>
<script type="text/javascript">
var settings = {
    endpoint:"<?=$urlPage;?>",
    ted:<?=$_GET['ted']?>,
};
</script>
<script src="/ted/js/parecer-juridico.js" type="text/javascript"></script>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class=""><a href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A">Listar Termos</a></li>
        <li class="active">Parecer Jurídico Concedente</li>
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
            <h4 class="text-center">Parecer Jurídico Concedente</h4>
        </section>

        <?php if (is_object($fm)) { echo $fm->getMensagens(); } ?>

        <div class="row">
            <div class="col-md-12">
                <?= $formParecer->showForm(); ?>
            </div>
        </div>

        <div class="row">
            <div class="col-md-12">
                <?php $arquivo->listarAnexos(Ted_Model_Arquivo::ABA_JURIDICO_CONCEDENTE); ?>
            </div>
        </div>
    </section>
</section>
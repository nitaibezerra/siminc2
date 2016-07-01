<?php

/**
 * Componente para disparar mensagens de alerta
 * @see FlashMessage.php
 */
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';

//Declaração de Objetos
$fm = new Simec_Helper_FlashMessage('ted/proponente');
$ug = new Ted_Model_UnidadeGestora();
$proponente = new Ted_Form_Proponente();
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();
$modelRepresentante = new Ted_Model_RepresentanteLegal();
$termoCooperacao = new Ted_Model_TermoExecucaoDescentralizada();

//Url base
$urlBse = 'ted.php?modulo=principal/termoexecucaodescentralizada';
$queryString = array(
    'acao' => 'A',
    'ted' => $termoCooperacao->capturaTcpid()
);

if (isset($_GET['ted'])) {
    Ted_Utils_Model::userIsAllowed();
}

//Carrega o combo de municipios, dependente do combo de estados
if (Ted_Utils_Model::isAjax() && isset($_GET['estuf'])) {
    $view = new Zend_View();
    $view->setEncoding($proponente->getEncoding());

    $muncod = new Zend_Form_Element_Select('muncod');
    $muncod->setRequired(true);
    $muncod->setAttrib('id', 'muncod');
    $muncod->setAttrib('class', 'form-control chosen-select');
    $muncod->setAttrib('style', 'width:100%;');
    $muncod->setAttrib('required', 'true');
    $muncod->addMultiOption('', 'Selecione');
    $muncod->addMultiOptions(Ted_Utils_Model::pegaMunicipio($_GET['estuf']));
    $muncod->addErrorMessage('Valor é necessário e não pode ser vazio');
    $muncod->removeDecorator('Label');
    $muncod->removeDecorator('DtDdWrapper');
    $muncod->removeDecorator('Description');
    $muncod->removeDecorator('HtmlTag');
    $proponente->addElement($muncod);
    $muncod->setView($view);
    echo $muncod->__toString();
    echo '<script type="text/javascript"> $("#muncod").trigger("chosen:updated").chosen(); </script>';
    die();
}


if ($_POST['estuf'] && $_POST['muncod']) {
    $proponente->getElement('muncod')
        ->addMultiOptions(Ted_Utils_Model::pegaMunicipio($_POST['estuf']))
        ->setValue($_POST['muncod']);
}

//Capturando requisições
if ((isset($_POST['submit']) || isset($_POST['submitcontinue'])) && $proponente->isValid($_POST)) {
    //separa os dados do proponente e do representante legal substituto
    $cleanPost = $termoCooperacao->extractArraySlice($_POST);

    //Tenta salvar os dados do proponente
    if (!$cleanPost['proponente']['termo']['tcpid']) {

        if (!$tcpid = $termoCooperacao->gravarTermoProponente($cleanPost['proponente'])) {
            $fm->addMensagem('Erro ao tentar salvar registro. Contacte o suporte.', Simec_Helper_FlashMessage::ERRO);
        } else {
            $tcpid = (is_bool($tcpid)) ? Ted_Utils_Model::capturaTcpid() : $tcpid;

            //salva os dados do representante legal substituto
            $modelRepresentante->save($cleanPost['representante_legal']);
            $queryString['ted'] = $tcpid;

            //salva coordenacao responsavel
            $cleanPost['coordenacao']['tcpid'] = $tcpid;
            $coordenacaoResponsavel = new Ted_Model_CoordenacaoResponsavel();
            $coordenacaoResponsavel->save($cleanPost['coordenacao']);

            $_SESSION['flashmessage'] = array(
                'message' => 'Registro salvo com sucesso!',
                'type' => Simec_Helper_FlashMessage::SUCESSO
            );

            //mensagens e redirecionamentos após salvar
            if (isset($_POST['submit'])) {
                echo Ted_Utils_Model::redirect($urlBse.'/proponente', $queryString);
            } else {
                echo Ted_Utils_Model::redirect($urlBse.'/concedente', $queryString);
            }
        }
    } else {

        $tcpid = Ted_Utils_Model::capturaTcpid();

        //salva os dados do representante legal substituto
        $modelRepresentante->save($cleanPost['representante_legal']);
        $queryString['ted'] = $tcpid;

        //salva coordenacao responsavel
        $cleanPost['coordenacao']['tcpid'] = $tcpid;
        $cleanPost['coordenacao']['ungcod'] = str_replace("'", '', $cleanPost['coordenacao']['ungcod']);
        $coordenacaoResponsavel = new Ted_Model_CoordenacaoResponsavel();
        $coordenacaoResponsavel->save($cleanPost['coordenacao']);

        $_SESSION['flashmessage'] = array(
            'message' => 'Registro salvo com sucesso!',
            'type' => Simec_Helper_FlashMessage::SUCESSO
        );

        //mensagens e redirecionamentos após salvar
        if (isset($_POST['submit'])) {
            echo Ted_Utils_Model::redirect($urlBse.'/proponente', $queryString);
        } else {
            echo Ted_Utils_Model::redirect($urlBse.'/concedente', $queryString);
        }
    }
} else {
    if (count($_POST) && !$proponente->isValid($_POST)) {
        $proponente->populate($_POST);
    }
}

//Seta o tcpid no formulário, caso exista
$proponente->getElement('tcpid')->setValue(Ted_Utils_Model::capturaTcpid());

//Trata a requisição quando o termo já existe
if ($ted = Ted_Utils_Model::capturaTcpid()) {
	$codprop = $termoCooperacao->capturaProponente();

    $dadosUG = $ug->pegaUnidade($codprop);
    $proponente->getElement('muncod')->addMultiOptions(
        Ted_Utils_Model::pegaMunicipio($dadosUG['estuf'])
    );

    $proponente->getElement('ungdsc')->addMultiOptions(
        $ug->pegaListaProponente(false)
    );

    if (strlen($dadosUG['ungfone']) > 9) {
        $dadosUG['ungddd'] = substr($dadosUG['ungfone'], 0, 2);
        $dadosUG['ungfone'] = substr($dadosUG['ungfone'], 3);
    }
    //ver($dadosUG);

    if (is_array($dadosUG))
        $proponente->populate($dadosUG);

    $coord = new Ted_Model_CoordenacaoResponsavel();
    $coordResponsavel = $coord->getByUngcod($codprop, $ted);
    //ver($coordResponsavel, d);

    if (is_array($coordResponsavel)) {
        $proponente->getElement('corid')->setValue($coordResponsavel['corid']);
        $proponente->getElement('nomecoordenacao')->setValue($coordResponsavel['nomecoordenacao']);
        $proponente->getElement('dddcoordenacao')->setValue($coordResponsavel['dddcoordenacao']);
        $proponente->getElement('telefonecoordenacao')->setValue($coordResponsavel['telefonecoordenacao']);
    }

    $representanteLegal = $modelRepresentante->pegaResponsavelUG($codprop);
    //ver($representanteLegal);

    $representanteLegalSubstituto = $modelRepresentante->pegaResponsavelUG($codprop, 't');
    //ver($representanteLegalSubstituto);

    if (is_array($representanteLegalSubstituto)) {
        $proponente->getElement('rlid')->setValue($representanteLegalSubstituto['rlid']);
        $proponente->getElement('cpf')->setValue($representanteLegalSubstituto['usucpf']);
        $proponente->getElement('nome')->setValue(utf8_decode($representanteLegalSubstituto['usunome']));
        $proponente->getElement('email')->setValue($representanteLegalSubstituto['usuemail']);
    }
} else {
    //Trata a requisição ao selecionar uma UG
    if (isset($_GET['ungcod']) && !empty($_GET['ungcod'])) {
        $dadosUG = $ug->pegaUnidade($_GET['ungcod']);

        if (strlen($dadosUG['ungfone']) > 9) {
            $dadosUG['ungddd'] = substr($dadosUG['ungfone'], 0, 2);
            $dadosUG['ungfone'] = substr($dadosUG['ungfone'], 3);
        }

        $proponente->getElement('ungdsc')->addMultiOptions(
            $ug->pegaListaProponente(true)
        );

        $proponente->getElement('muncod')->addMultiOptions(
            Ted_Utils_Model::pegaMunicipio($dadosUG['estuf'])
        );

        if (is_array($dadosUG))
            $proponente->populate($dadosUG);

        $representanteLegal = $modelRepresentante->pegaResponsavelUG($_GET['ungcod']);
    } else {
        //Trata a requisição quando o termo está em cadastramento
        $dadosUG = $ug->pegaListaProponente();
        if (is_array($dadosUG))
            $proponente->populate($dadosUG);
    }
}


if ($representanteLegal) {
    foreach ($representanteLegal as $k => $string) {
        $representanteLegal[$k] = Ted_Utils_Model::removeAcento($string);
    }
}

//Anexa uma mensagem ao flashmessage, se houver registro de mensagem na sessao
if (is_array($_SESSION['flashmessage']) && !empty($_SESSION['flashmessage'])) {
    if (array_key_exists('message', $_SESSION['flashmessage'])) {
        $fm->addMensagem($_SESSION['flashmessage']['message'], $_SESSION['flashmessage']['type']);
        unset($_SESSION['flashmessage']);
    }
}

$situacao = Ted_Utils_Model::pegaSituacaoTed();

//Cabeçalho do sistema
include APPRAIZ . 'includes/cabecalho.inc';
?>
<script type="text/javascript">
var setting = {
    representante:<?= ($representanteLegal) ? simec_json_encode($representanteLegal) : 'null'; ?>
};

function getParameterByName(name) {
    name = name.replace(/[\[]/, "\\[").replace(/[\]]/, "\\]");

    var regex = new RegExp("[\\?&]" + name + "=([^&#]*)")
      , results = regex.exec(location.search);

    return results === null ? "" : decodeURIComponent(results[1].replace(/\+/g, " "));
}

$(document).ready(function() {
    $("#cancel").on("click", function() {
        $(window.document.location).attr('href','ted.php?modulo=inicio&acao=C');
    });

    $("#ungdsc").on("change", function() {
        if ($(this).val()) {
            var urlRedirect = location.href.replace("&ungcod=<?=$_GET['ungcod'];?>", "");
            location.href=urlRedirect+"&ungcod="+$(this).val();
        }
    });

    $("#estuf").on("change", function() {
        if ($(this).val()) {
            $.get(location.href+"&estuf="+$(this).val(), function(data){
                $(".muncod").html(data);
            });
        }
    });

    //trata o request quando o termo ja existe
    if (getParameterByName("ungcod") || getParameterByName("ted")) {
        setting.representante = setting.representante || {};
        $("#blocked").show();
        $("#usucpf").val(setting.representante.usucpf || "");
        $("#usunome").val(setting.representante.usunome || "");
        $("#usuemail").val(setting.representante.usuemail || "");
        $("#usucpf, #ungcnpj").focus().blur();

        if (!setting.representante.usucpf) {
            $(".rl-empty").remove();
            $(".container-master").prepend(
                "<div class=\"alert alert-danger rl-empty\" role=\"alert\"><button type=\"button\" class=\"close\" data-dismiss=\"alert\" aria-label=\"Close\"><span aria-hidden=\"true\">&times;</span></button><strong>Não existe Representante Legal Cadastrado para esse UG</strong></div>"
            );
        }

        <?php if ($situacao['esdid'] != EM_CADASTRAMENTO) : ?>
        if ($("#tcpid").val()) {
            $("#blocked [type='text']").attr("disabled", true);
            $("#blocked select").attr("disabled", true);
            $("#blocked select").attr("disabled", true).trigger('chosen:updated').chosen();
        }
        <?php endif; ?>
    } else {
        //esconde o bloco de dados quando o termo não existe
        $("#blocked").hide();
    }

    [".input-rl", ".perfil-coordenacao"].forEach(function(el, i) {
        $(el).attr("disabled", false);
        $(el).focus().blur();
    });

    $("html,body").scrollTop(0);
});
</script>

<section class="col-md-12">
	<ol class="breadcrumb">
		<li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class=""><a href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A">Listar Termos</a></li>
		<li class="active">Proponente</li>
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

	<section class="col-md-10 container-master">
        <?php if (is_object($fm)) { echo $fm->getMensagens(); } ?>
		<?php echo $proponente->showForm(); ?>
	</section>
</section>
<?php

/**
 * Componente para disparar mensagens de alerta
 * @see FlashMessage.php
 */
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';

//Verifica se existe tcpid setado, senão, efetua redirecionamento
Ted_Utils_Model::capturaTcpid($redirect = true);

//Declaração de Objetos
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();
$responsavelUg = new Ted_Model_RepresentanteLegal();
$concedente = new Ted_Form_Concedente();
$ug = new Ted_Model_UnidadeGestora();
$fm = new Simec_Helper_FlashMessage('ted/concedente');
$termoCooperacao = new Ted_Model_TermoExecucaoDescentralizada();

//Url base
$urlBse = 'ted.php?modulo=principal/termoexecucaodescentralizada';
$queryString = array(
    'acao' => 'A',
    'ted' => Ted_Utils_Model::capturaTcpid()
);

Ted_Utils_Model::userIsAllowed();

//Carrega o combo de municipios, dependente do combo de estados
if (Ted_Utils_Model::isAjax() && isset($_GET['estuf'])) {
    $view = new Zend_View();
    $view->setEncoding($concedente->getEncoding());

    $municipio = new Zend_Form_Element_Select('muncod');
    $municipio->setRequired(true);
    $municipio->setAttrib('id', 'muncod');
    $municipio->setAttrib('class', 'form-control chosen-select');
    $municipio->setAttrib('style', 'width:100%;');
    $municipio->setAttrib('required', 'true');
    $municipio->addMultiOption('', 'Selecione');
    $municipio->addMultiOptions(Ted_Utils_Model::pegaMunicipio($_GET['estuf']));
    $municipio->addErrorMessage('Valor é necessário e não pode ser vazio');
    $municipio->removeDecorator('Label');
    $municipio->removeDecorator('DtDdWrapper');
    $municipio->removeDecorator('Description');
    $municipio->removeDecorator('HtmlTag');
    $concedente->addElement($municipio);
    $municipio->setView($view);
    echo $municipio->__toString();
    echo '<script type="text/javascript"> $("#muncod").trigger("chosen:updated").chosen(); </script>';
    die();
}

if ($_POST['estuf'] && $_POST['muncod']) {
    $concedente->getElement('muncod')
        ->addMultiOptions(Ted_Utils_Model::pegaMunicipio($_POST['estuf']))
        ->setValue($_POST['muncod']);
}

//verifica se tem post
if ((isset($_POST['submit']) || isset($_POST['submitcontinue'])) && $concedente->isValid($_POST)) {

    $dados = $termoCooperacao->extractArraySlice($_POST);
    //ver($dados, d);

    //tenta gravar os dados
	if ($termoCooperacao->gravarTermoConcedente($dados)) {

        $ug->atualizaDadosUnidadeGestora($dados['concedente']['unidade']);

        //salva os dados do representante legal substituto
        $responsavelUg->save($dados['representante_legal']);

        //salva coordenacao responsavel
        $dados['coordenacao']['tcpid'] = $dados['concedente']['termo']['tcpid'];
        $dados['coordenacao']['ungcod'] = str_replace('\'', '', $dados['coordenacao']['ungcod']);
        $coordenacaoResponsavel = new Ted_Model_CoordenacaoResponsavel();
        $coordenacaoResponsavel->save($dados['coordenacao']);

        //adiciona uma mensagem
        $_SESSION['flashmessage'] = array(
            'message' => 'Registro salvo com sucesso!',
            'type' => Simec_Helper_FlashMessage::SUCESSO
        );
        //faz o redirect
		if (isset($_POST['submit'])) {
            echo Ted_Utils_Model::redirect($urlBse.'/concedente', $queryString);
		} else {
            echo Ted_Utils_Model::redirect($urlBse.'/justificativa', $queryString);
		}						
	} else {
        $_SESSION['flashmessage'] = array(
            'message' => 'Registro salvo com sucesso!',
            'type' => Simec_Helper_FlashMessage::SUCESSO
        );
    }
} else {
    if (count($_POST) && !$concedente->isValid($_POST)) {
        $concedente->populate($_POST);
    }
}

//Seta o tcpid no formulário, caso exista
$concedente->getElement('tcpid')->setValue(Ted_Utils_Model::capturaTcpid());

if ($ted = Ted_Utils_Model::capturaTcpid()) {
	$codconc = $termoCooperacao->capturaConcedente();
	//ver($codconc, d);

    $dadosUG = $ug->pegaUnidade($codconc['ungcodconcedente']);
    if (is_array($dadosUG)) {
        $representanteLegal = $responsavelUg->pegaResponsavelUG($dadosUG['ungcod']);
    }

    if (strlen($dadosUG['ungfone']) > 9) {
        $dadosUG['ungddd'] = substr($dadosUG['ungfone'], 0, 2);
        $dadosUG['ungfone'] = substr($dadosUG['ungfone'], 3);
    }
    //ver($dadosUG, d);

    $coord = new Ted_Model_CoordenacaoResponsavel();
    $coordResponsavel = $coord->getByUngcod($codconc['ungcodconcedente'], $ted);
    //ver($coordResponsavel);
    if (is_array($coordResponsavel)) {
        $concedente->getElement('corid')->setValue($coordResponsavel['corid']);
        $concedente->getElement('nomecoordenacao')->setValue($coordResponsavel['nomecoordenacao']);
        $concedente->getElement('dddcoordenacao')->setValue($coordResponsavel['dddcoordenacao']);
        $concedente->getElement('telefonecoordenacao')->setValue($coordResponsavel['telefonecoordenacao']);
    }

    $representanteLegalSubstituto = $responsavelUg->pegaResponsavelUG($codconc['ungcodconcedente'], 't');
    //ver($representanteLegalSubstituto);
    if (is_array($representanteLegalSubstituto)) {
        $concedente->getElement('rlid')->setValue($representanteLegalSubstituto['rlid']);
        $concedente->getElement('cpf')->setValue($representanteLegalSubstituto['usucpf']);
        $concedente->getElement('nome')->setValue(utf8_decode($representanteLegalSubstituto['usunome']));
        $concedente->getElement('email')->setValue($representanteLegalSubstituto['usuemail']);
    }

    if (!empty($codconc['ungcodconcedente']) && $codconc['ungcodconcedente'] != UG_FNDE) {
        $concedente->removeElement('ungcodpoliticafnde');
    }

    if (is_array($dadosUG)) {
        //popula o formulário
        $concedente->populate($dadosUG);
        $concedente->getElement('ungcodconcedente')->setValue($codconc['ungcodconcedente']);
    }

    if ($concedente->getElement('ungcodpoliticafnde')) {
        if ($codconc['ungcodpoliticafnde']) {
            $concedente->getElement('ungcodpoliticafnde')->setValue($codconc['ungcodpoliticafnde']."_ungcod");
        } else if ($codconc['dircodpoliticafnde']) {
            $concedente->getElement('ungcodpoliticafnde')->setValue($codconc['dircodpoliticafnde']."_dircod");
        }
    }
}

if (isset($_GET['ungcod']) && !empty($_GET['ungcod'])) {
    //recupera os dados, de duas fontes
    $dadosUG = $ug->pegaUnidade($_GET['ungcod']);

    if (strlen($dadosUG['ungfone']) > 9) {
        $dadosUG['ungddd'] = substr($dadosUG['ungfone'], 0, 2);
        $dadosUG['ungfone'] = substr($dadosUG['ungfone'], 3);
    }

    $representanteLegal = $responsavelUg->pegaResponsavelUG($dadosUG['ungcod']);

    if ($_GET['ungcod'] != UG_FNDE) {
        $concedente->removeElement('ungcodpoliticafnde');
    }

    if ($dadosUG['estuf'] && $dadosUG['muncod']) {
        $concedente->getElement('muncod')
            ->addMultiOptions(Ted_Utils_Model::pegaMunicipio($dadosUG['estuf']))
            ->setValue($dadosUG['muncod']);
    }
    //ver($dadosUG);

    //popula o formulário
    $concedente->populate($dadosUG);
    $concedente->getElement('ungcodconcedente')->setValue($_GET['ungcod']);
}


if ($representanteLegal) {
    foreach ($representanteLegal as $k => $string) {
        $representanteLegal[$k] = Ted_Utils_Model::removeAcento($string);
    }
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
<script type="text/javascript">
;(function() {
    var Concedente = {};
    Concedente = window.Concedente = {};

    Concedente['STORAGE'] = {};
    Concedente["ENDPOINT"] = "ted.php?modulo=principal/termoexecucaodescentralizada/concedente&acao=A&ted=<?=Ted_Utils_Model::capturaTcpid();?>";
    Concedente["STATUS"] = {SUCCESS: 200};
    Concedente["setting"] = {representate: <?= ($representanteLegal) ? simec_json_encode($representanteLegal) : 'null'; ?> };

    Concedente.setStorage = function(name, value) {
        if (name && value) {
            Concedente['STORAGE'][name] = value;
        }
    };

    Concedente.getStorage = function(name) {
        if (Concedente['STORAGE'][name]) {
            return Concedente['STORAGE'][name];
        }
    };

    Concedente.getAllStorage = function() {
        if (Concedente['STORAGE']) {
            return Concedente['STORAGE'];
        }
    };

    Concedente.boot = function(dom) {
        this.DOM = $(dom);

        this.load = new Concedente.Load(
            this.DOM.find("#fndeblocked")
          , this.DOM.find("#blocked")
          //, this.DOM.find("#ungcod")
        );

        this.onCancel = new Concedente.Cancela(
            this.DOM.find("#cancel")
        );

        this.elUgCod = new Concedente.UgChange(
            this.DOM.find("#ungcod")
        );

        this.rlc = new Concedente.Representante(
            ["usucpf", "usunome", "usuemail"]
        );
    };
})();

;(function(Concedente) {
    Concedente.Load = function(fndeContainer, blockContainer) {
        this.fndeContainer = fndeContainer;
        this.blockContainer = blockContainer;
        var $ungcod = $("#ungcod");

        if (!$ungcod.val()) {
            this.fndeContainer.hide();
            this.blockContainer.hide();
        }
    };
})(Concedente);

;(function(Concedente) {
    Concedente.UgChange = function(ungcod) {
        this.ungcod = ungcod;
        this.EventListeners();
    };

    Concedente.UgChange.prototype.EventListeners = function() {
        this.ungcod.on("change", $.proxy(this, "OnChange"));
    };

    Concedente.UgChange.prototype.OnChange = function(event) {
        if (!event.target.value) {
            return false;
        }

        Concedente.setStorage("ungcod", event.target.value);
        location.href = Concedente["ENDPOINT"]+"&ungcod="+Concedente.getStorage("ungcod");
    };
})(Concedente);

;(function(Concedente) {
    Concedente.Representante = function(allFields) {
        allFields.forEach(function(el, i) {
            if (Concedente["setting"]["representate"]) {
                $("#"+el)
                    .val(Concedente["setting"]["representate"][el])
                    .attr("disabled", true);
            }
        });
    };

    Concedente.Cancela = function(btnCancel) {
        this.btnCancel = btnCancel;
        this.EventListneters();
    };

    Concedente.Cancela.prototype.EventListneters = function() {
        this.btnCancel.on("click", $.proxy(this, "ActionButton"));
    };

    Concedente.Cancela.prototype.ActionButton = function() {
        location.href = "ted.php?modulo=inicio&acao=C";
    };
})(Concedente);

$(function() {
    Concedente.boot(document.body);

    ["#ungcnpj", "#usucpf", "#cpf"].forEach(function(el, i) {
        $(el).focus().blur();
    });

    $("#estuf").on("change", function() {
        if ($(this).val()) {
            $.get(location.href+"&estuf="+$(this).val(), function(data){
                $(".muncod").html(data);
            });
        }
    });

    if ($("#estuf").val()) {
        $.get(location.href+"&estuf="+$("#estuf").val(), function(data){
            $(".muncod").html(data);

            <?php if ($dadosUG['muncod']) : ?>
                $("#muncod").val(<?=$dadosUG['muncod'];?>);
                $("#muncod").trigger("chosen:updated").chosen();
            <?php endif; ?>
        });
    }

    $("html,body").scrollTop(0);
});
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
        <?php if (is_object($fm)) { echo $fm->getMensagens(); } ?>
		<?php echo $concedente->showForm();?>
	</section>
</section>
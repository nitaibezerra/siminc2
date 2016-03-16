<?php
/**
 * include FlashMessage Componente
 */
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';

//declaração de objetos
$fm = new Simec_Helper_FlashMessage('ted/previsao');
$form = new Ted_Form_TramitaLote();
$model = new Ted_Model_TramitaLote();

/**
 * Recebe a requisição para Gerar Extrato DOU
 */
if (isset($_GET['geraDoc']) && ($_GET['geraDoc'] == 'true')) {
    if (count($_SESSION['ted']['extratodou'])) {

        $extrato = new Ted_Model_ExtratoDOU();
        $html = $extrato->getLote($_SESSION['ted']['extratodou']);
        if ($html) {
            unset($extrato, $_SESSION['ted']['extratodou']);
            $doc = new Ted_Model_HtmlToDocx();
            $doc->setHtml($html);
            $doc->setNomeDocumento('Extrato_DOU_'.date('d-m-Y--H:i:s', time()));
            $doc->getDownload();
        }

    } else {
        echo "<script>
              bootbox.alert('Nenhum extrato para ser gerado!', function(){
                  window.location.href = 'ted.php?modulo=principal/termoexecucaodescentralizada/tramitaLote&acao=A';
              });
          </script>";
    }
}

/**
 * Chama a ação para executar a tramitação em lote
 */
if (isset($_POST['action']) && !empty($_POST['action'])) {
    if ($model->executaTramitacao($_POST)) {
        $fm->addMensagem('Os Termos selecionados foram tramitados com sucesso!', Simec_Helper_FlashMessage::SUCESSO);
    } else {
        $fm->addMensagem('Nenhum termo pôde ser tramitado.', Simec_Helper_FlashMessage::ERRO);
    }
}

/**
 * Verifica se adiciona comentário ao formulário
 */
if (isset($_GET['aedid']) && Ted_Utils_Model::isAjax()) {
    echo $model->verificaComentario($_GET['aedid']);
    die;
}

/**
 * Combo que carrega a lista de termos
 */
if (isset($_POST['aedid']) && Ted_Utils_Model::isAjax()) {
    $model->showTedList($_POST);
    die;
}

/**
 * Recebe a requisição para montar o combo dependente
 */
if (isset($_GET['esdid']) && Ted_Utils_Model::isAjax()) {
    $view = new Zend_View();
    $view->setEncoding('ISO-8859-1');

    $aedid = new Zend_Form_Element_Select('aedid');
    $aedid->setRequired(true);
    $aedid->setAttrib('id', 'aedid');
    $aedid->setAttrib('class', 'form-control chosen-select');
    $aedid->setAttrib('required', 'true');
    $aedid->addMultiOption('', 'Selecione a ação');
    $aedid->addMultiOptions($model->getAcoesTermos($_GET['esdid']));
    $aedid->addErrorMessage('Valor é necessário e não pode ser vazio');
    $aedid->removeDecorator('Label');
    $aedid->removeDecorator('DtDdWrapper');
    $aedid->removeDecorator('Description');
    $aedid->removeDecorator('HtmlTag');
    $form->addElement($aedid);
    $aedid->setView($view);
    echo $aedid->__toString();
    die;
}

//ver($_SESSION['usucpf'], $model->getSituacoesTermos());
$form->getElement('esdid')->addMultiOptions($model->getSituacoesTermos());

//Cabeçalho do sistema
include APPRAIZ . 'includes/cabecalho.inc';
?>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class="active">Tramitar em Lote</li>
    </ol>

    <section class="col-md-12">
        <section class="well">
            <h4 class="text-center">Trâmitação em lote</h4>
        </section>

        <?php
        if (is_object($fm)) {
            echo $fm->getMensagens();
        }
        ?>

        <div class="row well">
            <div class="col-md-3"></div>
            <div class="col-md-6">
                <?php echo $form->showForm() ?>
            </div>
            <div class="col-md-3">
            </div>
        </div>

        <?php if (isset($_SESSION['ted']['extratodou']) && count($_SESSION['ted']['extratodou'])) : ?>
        <div class="row well">
            <div class="col-md-12 text-center">
                <input type="button" class="btn btn-primary" name="extratoDOU" id="extratoDOU" value="Gerar Extrato DOU">
            </div>
        </div>
        <?php endif; ?>

        <div class="row well">
            <div class="col-md-12 list"></div>
        </div>
    </section>
</section>

<script src="/ted/js/jquery.livequery.js" type="text/javascript"></script>
<script src="/ted/js/jquery.textarea.limiter.js" type="text/javascript"></script>
<script src="/ted/js/checkbox-checker.js" type="text/javascript"></script>
<script src="/ted/js/bootbox.js" type="text/javascript"></script>
<script type="text/javascript">
;(function() {
    var TramitaLote;
    TramitaLote = window.TramitaLote = {};

    TramitaLote['STORAGE'] = {};
    TramitaLote["ENDPOINT"] = location.href;
    TramitaLote["STATUS"] = {SUCCESS: 200}

    TramitaLote.load = function(html) {
        this.DOM = $(html);

        this.onChange = new TramitaLote.onAction(
            this.DOM.find("#esdid")
            , this.DOM.find(".aedid-container")
        );

        this.onList = new TramitaLote.onList(
            this.DOM.find("#aedid")
            , this.DOM.find(".list")
            , this.DOM.find(".comment")
        );

        this.elTextArea = new TramitaLote.counterTextArea(
            this.DOM.find("#cmddsc")
        );

        this.tramitaForm = new TramitaLote.onForm(
            this.DOM.find("#tramitaLote")
            , this.DOM.find("#tramita")
        );
    };

    TramitaLote.setStorage = function(name, value) {
        if (name && value) {
            TramitaLote['STORAGE'][name] = value;
        }
    };

    TramitaLote.getStorage = function(name) {
        if (TramitaLote['STORAGE'][name]) {
            return TramitaLote['STORAGE'][name];
        }
    };

    TramitaLote.getAllStorage = function() {
        if (TramitaLote['STORAGE']) {
            return TramitaLote['STORAGE'];
        }
    };
})();

;(function($t) {
    TramitaLote.onAction = function(esdid, aedid) {
        this.esdid = esdid;
        this.aedid = aedid;
        this.EventListener();
    };

    TramitaLote.onAction.prototype.EventListener = function() {
        this.esdid.on("change", $.proxy(this, "onSituacao"));
        this.esdid.on("change", $.proxy(this, "onAcoes"));
    };

    TramitaLote.onAction.prototype.onSituacao = function(event) {
        this.onClearList();
        if (event.target.value) {
            $t.setStorage('esdid', event.target.value);
            var url = $t.ENDPOINT+"&esdid="+$t.getStorage('esdid');
            $.get(url, $.proxy(this, "onSuccess"));
        }
    };

    TramitaLote.onAction.prototype.onSuccess = function(data, textStatus, jqXHR) {
        if (jqXHR.status == $t.STATUS.SUCCESS) {
            this.aedid.html(data);
            this.aedid.find("#aedid").trigger('chosen:updated').chosen();
        }
    };

    TramitaLote.onAction.prototype.onClearList = function() {
        $(".list").html("");
        $(".comment").addClass("hide");
    };
})(TramitaLote);

;(function($t){
    TramitaLote.onList = function(aedid, list, comment) {
        this.aedid = aedid;
        this.list = list
        this.comment = comment;
        this.EventListener();
    };

    TramitaLote.onList.prototype.EventListener = function() {
        this.aedid.livequery("change", $.proxy(this, "onChange"));
    };

    TramitaLote.onList.prototype.onChange = function(event) {
        if (event.target.value) {
            $t.setStorage('aedid', event.target.value);

            var $jqXhr = $.ajax($t.ENDPOINT, {data:$t.getAllStorage(), type:'POST'})
            $jqXhr.done($.proxy(this, "onLoadList"));
        }
    };

    TramitaLote.onList.prototype.onLoadList = function(data, textStatus, jqXHR) {
        if (jqXHR.status == $t.STATUS.SUCCESS) {
            this.list.html(data);
            new ControleCheckbox("#tb_render", "#ckboxPai", ".ckboxChild");

            var url = $t.ENDPOINT+"&aedid="+$t.getStorage('aedid');
            $.get(url, $.proxy(this, "isComment"));
        }
    };

    TramitaLote.onList.prototype.isComment = function(data, textStatus, jqXHR) {
        if (jqXHR.status == $t.STATUS.SUCCESS) {
            var method = (data == 'success') ? 'removeClass' : 'addClass';
            this.comment[method]("hide");
        }
    };
})(TramitaLote);

;(function($t){
    TramitaLote.counterTextArea = function(cmddsc) {
        this.cmddsc = cmddsc;

        this.cmddsc.limit({
            limit: 200,
            id_result: "counter-cmddsc",
            alertClass: "warning"
        });
    }
})(TramitaLote);

;(function($t){
    TramitaLote.onForm = function(form, button) {
        this.form = form;
        this.btnEnviar = button;
        this.eventListener();
    };

    TramitaLote.onForm.prototype.eventListener = function() {
        this.btnEnviar.on("click", $.proxy(this, "onSubmit"));
    };

    TramitaLote.onForm.prototype.onSubmit = function() {
        var cssClass = "has-error"
            , that = this
            , errors = []
            , termos = []
            ;

        this.form.find("#docid").val("");
        this.form.find(".form-group").removeClass(cssClass);

        //valida combobox com situação e ações do workflow
        ["#esdid", "#aedid"].forEach(function(el, i) {
            if (!$(el).val()) {
                $($(el).parent().parent()).addClass(cssClass);
                errors.push($(el)[0]);
            }
        });

        //valida campo de comentário, quando for obrigatório
        if (!$(".comment").hasClass("hide")) {
            if (!$("#cmddsc").val()) {
                $($("#cmddsc").parent().parent()).addClass(cssClass);
                errors.push($("#cmddsc")[0]);
            } else {
                $($("#cmddsc").parent().parent()).removeClass(cssClass);
            }
        }

        //verifica se algum checkbox foi selecionado
        $(".ckboxChild").each(function(i, el) {
            if ($(el).is(":checked")) {
                termos.push($(el).attr("data-proid"));
            }
        });

        //console.log(errors, termos);
        if (errors.length || termos.length === 0) {
            // See /ted/js/bootbox.js
            bootbox.alert("Existe(m) campo(s) pendente(s) de preenchimento!");

            return false;
        } else {
            this.form.find("#action").val("tramitar");
            this.form.find("#docid").val(termos.join(","));
            return true;
        }
    };
})(TramitaLote);

;$(function(){
    TramitaLote.load(document.body);

    $("#extratoDOU").on("click", function(){
        $(this).remove();
        location.href="ted.php?modulo=principal/termoexecucaodescentralizada/tramitaLote&acao=A&geraDoc=true";
    });
});
</script>

<?php
function addCheckbox($id, $options) {
    $acao = <<<HTML
        <input type="checkbox" class="ckboxChild" data-proid="%s" name="data[id][%s]" />
HTML;
    return sprintf($acao, $id, $id);
}
?>
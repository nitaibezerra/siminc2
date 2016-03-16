<?php
/**
 * Componente para disparar mensagens de alerta
 * @see FlashMessage.php
 */
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';

//Verifica se existe tcpid setado, senão, efetua redirecionamento
Ted_Utils_Model::capturaTcpid(true);
Ted_Utils_Model::userIsAllowed();

//Declaração de objetos
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();
$formVigencia = new Ted_Form_Vigencia();
$model = new Ted_Model_Vigencia();
$fm = new Simec_Helper_FlashMessage('ted/vigencia');

$formVigencia->getElement('tcpid')->setValue(Ted_Utils_Model::capturaTcpid());

if (Ted_Utils_Model::isAjax() && (count($_POST) > 0)) {

    $_POST['vigdata'] = formata_data_sql($_POST['vigdata']);

    if ($formVigencia->isValid($_POST)) {
        $model->popularDadosObjeto($_POST);
        if ($model->salvar()) {
            $model->commit();
            $_SESSION['flashmessage'] = array(
                'message' => 'Aditívo cadastrado com sucesso!',
                'type' => Simec_Helper_FlashMessage::SUCESSO
            );
        } else {
            $_SESSION['flashmessage'] = array(
                'message' => 'Falha ao tentar cadastrar aditívo',
                'type' => Simec_Helper_FlashMessage::ERRO
            );
        }
    }
    die;
}

if (isset($_GET['v']) && is_numeric($_GET['v']) && Ted_Utils_Model::capturaTcpid()) {
    $db->executar(sprintf("delete from ted.aditivovigencia where vigid = %d", $_GET['v']));
    if ($db->commit()) {
        $_SESSION['flashmessage'] = array(
            'message' => 'Aditívo apagado com sucesso!',
            'type' => Simec_Helper_FlashMessage::SUCESSO
        );
    } else {
        $_SESSION['flashmessage'] = array(
            'message' => 'Falha ao tentar apagar aditívo',
            'type' => Simec_Helper_FlashMessage::ERRO
        );
    }

    echo Ted_Utils_Model::redirect(
        'ted.php?modulo=principal/termoexecucaodescentralizada/vigencia',
        array('acao' => 'A', 'ted' => Ted_Utils_Model::capturaTcpid())
    );
}

//Anexa uma mensagem ao flashmessage, se houver registro de mensagem na sessao
if (is_array($_SESSION['flashmessage'])) {
    if (array_key_exists('message', $_SESSION['flashmessage'])) {
        $fm->addMensagem($_SESSION['flashmessage']['message'], $_SESSION['flashmessage']['type']);
        unset($_SESSION['flashmessage']);
    }
}

//Cabeçalho do sistema
include APPRAIZ.'includes/cabecalho.inc';
?>
<link rel="stylesheet" href="/ted/css/bootstrap-datepicker3.min.css" type="text/css"/>
<script src="/ted/js/bootstrap-datepicker.min.js" type="text/javascript"></script>
<script src="/ted/js/bootstrap-datepicker.pt-BR.min.js" type="text/javascript"></script>
<script type="text/javascript" src="/ted/js/jquery.livequery.js"></script>
<script type="text/javascript">
function deleteVigencia(id) {
    if (confirm("deseja realmente apagar a vigência?") && id) {
        var baseUrl = "ted.php?modulo=principal/termoexecucaodescentralizada/vigencia&acao=A&ted=";
        location.href=baseUrl+"<?=Ted_Utils_Model::capturaTcpid()?>&v="+id;
    }
}

$(function(){
    $("#add-vigencia").on("click", function() {
        $('#modalAditivo').modal('show');
        $("#dtexecucao").html($("#data-vigencia").val());
    });

    $("#salvar").livequery("click", function(){
        var fields = ["#vigdata", "#vigjustificativa"]
          , errors = false;

        fields.forEach(function(el, i){
            (!$(el).val()) ? errors = true : undefined;
        });

        if (errors === true) {
            bootbox.alert("Todos os campos do formulário devem ser preenchidos.");
            return false;
        } else {
            var $promisse = $.ajax({url:location.href, type:"post", data:$("#vigencia").serialize()});
            $promisse.done(function(data){
                location.reload();
            });
        }
    });

    $(".widget-date-control").datepicker({
        language: "pt-BR",
        autoclose: true,
        todayHighlight: true,
        format: "dd/mm/yyyy"
    }).on("change", function(e) {
        var strDate = $(e.currentTarget).val();
        if (!strDate.match(/^(\d{2})\/(\d{2})\/(\d{4})$/)) {
            bootbox.alert("O formato da data é inválido", function(){
                $(e.currentTarget).val("");
            });
        }
    });

})
</script>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class=""><a href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A">Listar Termos</a></li>
        <li class="active">Vigência do Termo</li>
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
            <h4 class="text-center">Vigência do Termo</h4>
        </section>
        <?php echo $fm->getMensagens(); ?>
        <?php echo $model->getVigencia(); ?>
    </section>

    <div class="col-md-10">
        <?php $model->getList(); ?>
    </div>

    <div class="modal fade modalAditivo" id="modalAditivo" tabindex="-1" role="dialog" aria-labelledby="modalAditivo" aria-hidden="true">
        <div class="modal-dialog" style="width:80%;">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal">
                        <span aria-hidden="true">&times;</span>
                        <span class="sr-only">Close</span>
                    </button>
                    <h4 class="modal-title">Aditivo de Prazo</h4>
                </div>
                <div class="modal-body" id="modal-body">
                    <?php echo $formVigencia->showForm(); ?>
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                    <button type="button" class="btn btn-success" id="salvar">Salvar</button>
                </div>
            </div>
        </div>
    </div>
</section>


<?php
/**
 * Componente para disparar mensagens de alerta
 * @see FlashMessage.php
 */
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';

//Declaração de Objetos
$fm = new Simec_Helper_FlashMessage('ted/rl');
$representanteLegal = new Ted_Model_RepresentanteLegal();

if (Ted_Utils_Model::isAjax() && isset($_GET['cod'])) {
    $dados = $representanteLegal->get($_GET['cod']);
    if ($dados) {
        header('Content-Type: application/json', 200);
        echo simec_json_encode($dados);
    }

    die;
}

if (isset($_POST['action']) && $_POST['action'] == 'save') {
    if ($representanteLegal->save($_POST)) {
        $fm->addMensagem('Dados salvos com sucesso!', Simec_Helper_FlashMessage::SUCESSO);
    } else {
        $fm->addMensagem('Erro ao tentar salvar os dados.', Simec_Helper_FlashMessage::ERRO);
    }
}

if (isset($_POST['action']) && $_POST['action'] == 'search') {
    $where = array();

    if ($_POST['ungcod']) {
        $where[] = " ung.ungcod = '{$_POST['ungcod']}' ";
    }

    if ($_POST['ungdsc']) {
        $where[] = " ung.ungdsc ilike '%{$_POST['ungdsc']}%' ";
    }

    if ($_POST['nome']) {
        $where[] = " rpl.nome ilike '%{$_POST['nome']}%' ";
    }

    if ($_POST['email']) {
        $where[] = " rpl.email = '{$_POST['email']}' ";
    }

    if ($_POST['cpf']) {
        $cpf = str_replace(array('.', '-'), '', $_POST['cpf']);
        $where[] = " rpl.cpf = '{$cpf}' ";
    }
}

require_once APPRAIZ . 'includes/cabecalho.inc';
?>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class="active">Representante Legal</li>
    </ol>

    <section class="col-md-12">
        <section class="well">
            <h4 class="text-center">Representante Legal</h4>
        </section>

        <?php if (is_object($fm)) { echo $fm->getMensagens(); } ?>

        <div class="row">
            <div class="col-md-12">
                <form class="form-horizontal" name="formulario" id="formulario" action="" method="post" role="form">
                    <input type="hidden" name="rlid" id="rlid">
                    <input type="hidden" name="action" id="action" value="">
                    <section class="well">
                        <div class="form-group boxUO">
                            <label class="control-label col-md-3" for="ungcod">Código da UG</label>
                            <div class="col-md-9 boxForm">
                                <input type="text" name="ungcod" id="ungcod" class="form-control">
                            </div>
                        </div>
                        <div class="form-group boxUG">
                            <label class="control-label col-md-3" for="ungdsc">Nome da UG</label>
                            <div class="col-md-9 boxForm">
                                <input type="text" name="ungdsc" id="ungdsc" class="form-control">
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="control-label col-md-3" for="cpf">CPF do Responsável</label>
                            <div class="col-md-9">
                                <input type="text" name="cpf" id="cpf" class="form-control"
                                    onKeyUp="this.value=mascaraglobal('###.###.###-##',this.value);"
                                    onBlur="this.value=mascaraglobal('###.###.###-##',this.value);">
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="control-label col-md-3" for="nome">Nome do Responsável</label>
                            <div class="col-md-9">
                                <input type="text" name="nome" id="nome" class="form-control">
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="control-label col-md-3" for="email">E-mail do Responsável</label>
                            <div class="col-md-9">
                                <input type="text" name="email" id="email" class="form-control">
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="control-label col-md-3" for="titular">Títular</label>
                            <div class="col-md-9">
                                <label class="radio-inline">
                                    <input type="radio" name="substituto" id="titular" value="f"> Títular
                                </label>
                                <label class="radio-inline">
                                    <input type="radio" name="substituto" id="substituto" value="t"> Substituto
                                </label>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-md-offset-2">
                                <button type="button" class="btn btn-warning btn-action" name="search" id="search">
                                    Pesquisar
                                </button>
                                <button type="button" class="btn btn-primary btn-action" name="save" id="save">
                                    Salvar
                                </button>
                                <button type="submit" class="btn btn-danger btn-action" name="clear" id="clear">
                                    Limpar
                                </button>
                            </div>
                        </div>
                    </section>
                </form>
            </div>
        </div>

        <div class="row">
            <div class="col-md-12" style="margin: 0 auto;">
            <?php

            $strSQL = "
                SELECT
                    rpl.rlid,
                    ung.ungcod,
                    ung.ungdsc,
                    rpl.cpf,
                    rpl.nome,
                    rpl.email,
                    rpl.substituto
                FROM ted.representantelegal rpl
                JOIN public.unidadegestora ung ON ung.ungcod = rpl.ug
                ".($where ? " WHERE ".implode(' AND ', $where) : '')."
                ORDER BY ung.ungdsc
            ";

            require APPRAIZ . 'includes/library/simec/Listagem.php';

            $list = new Simec_Listagem();
            $list->setCabecalho(array(
                'Código da UG',
                'Unidade Gestora',
                'CPF do Representante Legal',
                'Nome do Representante Legal',
                'E-mail do Representante Legal',
                'Títular'
            ));
            $list->addAcao('edit', 'editarDados');
            $list->addCallbackDeCampo('substituto', function($substituto, $dados) {
                if ($substituto == 'f') {
                    return '<span class="glyphicon glyphicon-ok" aria-hidden="true"></span>';
                } else {
                    return '';
                }
            });
            $list->setQuery($strSQL);

            $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $list->turnOnPesquisator();
            $list->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
            //ver($strSQL);
            ?>
            </div>
        </div>
    </section>
</section>

<script type="text/javascript">
var editarDados = function(cod) {
    var url = "ted.php?modulo=sistema/tabeladeapoio/representanteLegal&acao=A"
      , $promisse = $.ajax({url: url+"&cod="+cod});

    $promisse.done(function(response) {
        console.log(response);
        for (var el in response) {
            if (el == "substituto") {
                continue;
            } else {
                $("#"+el).val(response[el]);
            }
        }

        $("[name='substituto']").attr("checked", false);
        if (response.substituto == 'f') {
            $("#titular").prop("checked", true);
        } else {
            $("#substituto").prop("checked", true);
        }
    });

    $promisse.always(function(response) {
        $("#cpf").focus().blur();

        if ($("#rlid").val()) {
            $("#save").removeClass("hide");
            $("#search").addClass("hide");

            $(".boxUO label").text("Unidade Orçamentária");
            $(".boxUO .boxForm").html(response.unicod+' - '+response.unidsc);

            $(".boxUG label").text("Unidade Gestora");
            $(".boxUG .boxForm").html(response.ungcod+' - '+response.ungdsc);

        } else {
            $("#save").addClass("hide");
            $("#search").removeClass("hide");
        }
    });
};

function _validaForm(idForm, fields) {
    var errorExists = false
      , $rlid = $("#rlid");

    if ($rlid.val()) {
        fields = ["#cpf", "#nome", "#email"]
    }

    fields.forEach(function(el, i) {
        if (!$(el).val()) {
            errorExists = true;
        }
    });

    if (!$("[name='substituto']:checked").val()) {
        errorExists = true;
    }

    if (errorExists) {
        bootbox.alert("Todos os campos são de preenchimento obrigatórios;");
        return false;
    }

    $(idForm).submit();
}


$(function(){
    $("#clear").on("click", function(e) {
        e.preventDefault();
        location.href="ted.php?modulo=sistema/tabeladeapoio/representanteLegal&acao=A";
    });

    $("#save").on("click", function() {
        $("#action").val("save");
        _validaForm("#formulario", ["#ungcod", "#ungdsc", "#cpf", "#nome", "#email"]);
    });

    $("#search").on("click", function() {
        $("#action").val("search");
        $("#formulario").submit();
    });
});
</script>
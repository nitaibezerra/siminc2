<?php

/**
 * Componente para disparar mensagens de alerta
 * @see FlashMessage.php
 */
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';

/**
 * incluindo funções comuns a sistemas SPO
 */
require_once APPRAIZ . 'includes/funcoesspo_componentes.php';

//Declaração de Objetos
$fm = new Simec_Helper_FlashMessage('ted/rl');
$unidade = new Ted_Model_UnidadeGestora();
$unidadeUO = new Ted_Model_UnidadeOrcamentaria();

if (Ted_Utils_Model::isAjax() && isset($_GET['cod'])) {
    $dados = $unidade->get($_GET['cod']);
    if ($dados) {
        header('Content-Type: application/json', 200);
        echo simec_json_encode($dados);
    }
    die;
}

if (isset($_POST['unicod'])) {

    if (!empty($_POST['unicod']) && is_numeric($_POST['unicod']) && isset($_POST['save'])) {
        if ($unidade->save($_POST)) {
            $fm->addMensagem('Dados salvos com sucesso!', Simec_Helper_FlashMessage::SUCESSO);
        } else {
            $fm->addMensagem('Erro ao tentar salvar os dados.', Simec_Helper_FlashMessage::ERRO);
        }
    } else {
        if (isset($_POST['search'])) {
            $where = array();

            if ($_POST['unicod']) {
                $where[] = " uni.unicod = '{$_POST['unicod']}' ";
            }

            if ($_POST['unidsc']) {
                $where[] = " uni.unidsc = '{$_POST['unidsc']}' ";
            }

            if ($_POST['ungcod']) {
                $where[] = " ung.ungcod = '{$_POST['ungcod']}' ";
            }

            if ($_POST['ungdsc']) {
                $where[] = " ung.ungdsc ilike '%{$_POST['ungdsc']}%' ";
            }
        }
    }
}

require_once APPRAIZ . 'includes/cabecalho.inc';
?>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class="active">Unidade Gestora</li>
    </ol>

    <section class="col-md-12">
        <section class="well">
            <h4 class="text-center">Unidade Gestora</h4>
        </section>

        <?php if (is_object($fm)) { echo $fm->getMensagens(); } ?>

        <div class="row">
            <div class="col-md-12">
                <form class="form-horizontal" name="formulario" id="formulario" action="" method="post" role="form">
                    <section class="well">
                        <div class="form-group">
                            <label class="control-label col-md-3" for="unicod">Unidade Orçamentária</label>
                            <div class="col-md-9">
                                <?php echo inputCombo('unicod', $unidadeUO->pegaUnidades(), '', 'unicod'); ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="control-label col-md-3" for="ungcod">Código da UG</label>
                            <div class="col-md-9">
                                <input type="text" name="ungcod" id="ungcod" class="form-control">
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="control-label col-md-3" for="ungdsc">Nome da UG</label>
                            <div class="col-md-9">
                                <input type="text" name="ungdsc" id="ungdsc" class="form-control">
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="control-label col-md-3" for="ungabrev">Abreviação da UG</label>
                            <div class="col-md-9">
                                <input type="text" name="ungabrev" id="ungabrev" class="form-control">
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="control-label col-md-3" for="gescod">Codigo da Gestão</label>
                            <div class="col-md-9">
                                <input type="text" name="gescod" id="gescod" class="form-control">
                            </div>
                        </div>
                        <div class="form-group hide">
                            <label class="control-label col-md-3" for="cpf">CPF do Responsável</label>
                            <div class="col-md-9 rcpf"></div>
                        </div>
                        <div class="form-group hide">
                            <label class="control-label col-md-3" for="nome">Nome do Responsável</label>
                            <div class="col-md-9 rnome"></div>
                        </div>
                        <div class="form-group hide">
                            <label class="control-label col-md-3" for="email">E-mail do Responsável</label>
                            <div class="col-md-9 remail"></div>
                        </div>
                        <div class="form-group">
                            <div class="col-md-offset-2">
                                <button type="submit" class="btn btn-warning btn-action" name="search" id="search">
                                    Pesquisar
                                </button>
                                <button type="submit" class="btn btn-primary btn-action" name="save" id="save">
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
                    ung.ungcod as id,
                    uni.unicod,
                    uni.unidsc,
                    ung.ungcod,
                    ung.ungdsc,
                    rpl.cpf,
                    rpl.nome,
                    rpl.email
                FROM public.unidadegestora ung
                LEFT JOIN ted.representantelegal rpl ON ung.ungcod = rpl.ug and substituto = 'f'
                LEFT JOIN public.unidade uni ON ung.unicod = uni.unicod
                " . ($where ? " WHERE " . implode(" AND ", $where) : "WHERE uni.orgcod = '". CODIGO_ORGAO_SISTEMA. "'") . "
                ORDER BY ung.unicod
            ";

            require_once APPRAIZ . 'includes/library/simec/Listagem.php';

            $list = new Simec_Listagem();
            $list->setCabecalho(array(
                'Código da UO',
                'Unidade Orçamentária',
                'Código da UG',
                'Unidade Gestora',
                'CPF do Representante Legal',
                'Nome do Representante Legal',
                'E-mail do Representante Legal'
            ));
            $list->addAcao('edit', 'editarDados');
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
    var url = "ted.php?modulo=sistema/tabeladeapoio/unidadeGestora&acao=A"
      , $promisse = $.ajax({url: url+"&cod="+cod});

    $promisse.done(function(response) {
        for (var el in response) {
            $("#"+el).val(response[el]);
        }

        if (response.cpf) {
            $(".rcpf").html(response.cpf);
            $(".rnome").html(response.nome);
            $(".remail").html(response.email);
            [".rcpf", ".remail", ".rnome"].forEach(function(e, i) {
                $(e).parent().removeClass("hide");
            });
        }

        $("#unicod").trigger('chosen:updated');
        $("#unicod").chosen();
    });

    $promisse.always(function(response) {
        $("#cpf").focus().blur();

        if ($("#unicod").val()) {
            $("#save").removeClass("hide");
            $("#search").addClass("hide");
        } else {
            $("#save").addClass("hide");
            $("#search").removeClass("hide");
        }
    });
};

$(function(){
    $("#clear").on("click", function(e) {
        e.preventDefault();
        location.href="ted.php?modulo=sistema/tabeladeapoio/unidadeGestora&acao=A";
    });

    $("#save").on("click", function() {
        $("#formulario").submit();
    });

    $("#search").on("click", function() {
        $("#formulario").submit();
    });
});
</script>
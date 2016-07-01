<?php
include_once APPRAIZ . 'includes/funcoesspo.php';
include_once '_funcoesquestionario.php';

/**
 * Classe de Questionário
 */
require APPRAIZ . 'planacomorc/classes/Questionario.class.inc';

if($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

$questionario = new Questionario(Questionario::CAIXA_VERDE);
?>
<script type="text/javascript">

$(function(){  
    $(".chosen-container").attr("style", "width: 100%;");

    $("#btn-Salva-Questionario").on("click", function(){
        if (!validaForm()) return false;
        var dataPost = $("#insertQuestionario").serialize();
        $.ajax({
            url: location.href,
            type: 'POST',
            data: dataPost+"&requisicao=inserirQuestionario",
            success: function(data) {
                $("body").append(data);
            }
        })
    });
});

var viewQuestionario = function (id) {
    window.location='planacomorc.php?modulo=principal/questionario/listaquestionario&acao=A&requisicao=carregarQuestionario&qstid='+id;
};

var deleteQuestionario = function(id) {
    if (confirm('Deseja realmente excluir o Questionário?')) {
        window.location = 'planacomorc.php?modulo=principal/questionario/listaquestionario&acao=A&requisicao=excluirQuestionario&qstid='+id;
    }
};

var validaForm = function() {
    var formKeys = ['qstnome', 'id_acao_programatica', 'id_periodo_referencia'];
    for (var i in formKeys) {
        if (formKeys.hasOwnProperty(i)) {
            if (!$("#"+formKeys[i]).val()) {
                var label = $("label[for='"+formKeys[i]+"']").html();
                alert("O campo "+label+" é de preenchimento obrigatório!");
                $("#"+formKeys[i]).focus();
                return false;
            }

        }
    }
    return true;
};

</script>

<div class="row">
    <div class="col-lg-12">
        <ol class="breadcrumb">
            <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?= $_SESSION['sisdsc']; ?></a></li>
            <li class="">Ações <?= $_SESSION['exercicio']; ?></li>
            <li class="active">Questionário de Acompanhamento de Ações</li>
        </ol>
    </div>
</div>

<div class="modal fade" id="insert-new" tabindex="-1" role="dialog" aria-labelledby="ncModalLabel" aria-hidden="true">
    <div class="modal-dialog" style="width:50%;">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal">
                    <span aria-hidden="true">&times;</span>
                    <span class="sr-only">Close</span>
                </button>
                <h4 class="modal-title">Formulário</h4>
            </div>
            <div class="modal-body">
                <form class="form-horizontal" name="insertQuestionario" id="insertQuestionario" action="" method="POST" role="form">
                    <div class="row">
                        <input type="hidden" name="qstid" id="qstid" value="<?= isset($_REQUEST['qstid']) ? $_REQUEST['qstid'] : '' ?>" />
                        <div class="form-group">
                            <label class="control-label col-md-2" for="qstnome">Nome</label>
                            <div class="col-md-10">
                                <?= inputTexto('qstnome', isset($_REQUEST['qstnome']) ? $_REQUEST['qstnome'] : '', 'qstnome', '62', false); ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="control-label col-md-2" for="id_acao_programatica">Ações</label>
                            <div class="col-md-10">
                                <?php
                                $sql = "SELECT distinct
                                            id_acao_programatica as codigo,
                                            pro.codigo || '.' || a.codigo || '.' || org.codigo || ' - ' || dae.titulo as descricao
                                        FROM planacomorc.acao a
                                            join planacomorc.acao_programatica apr on apr.id_acao = a.id_acao
                                            join planacomorc.dados_acao_exercicio dae on dae.id_acao = a.id_acao and dae.id_exercicio=apr.id_exercicio
                                            join planacomorc.programa pro on pro.id_programa = a.id_programa
                                            join planacomorc.orgao org on org.id_orgao = apr.id_orgao
                                        WHERE apr.id_exercicio = '".$_SESSION['exercicio']."'";

                                echo inputCombo('id_acao_programatica[]', $sql, isset($_REQUEST['id_acao_programatica']) ? $_REQUEST['id_acao_programatica'] : '', 'id_acao_programatica', array('multiple'=> 'multiple','titulo'=>'Selecione um ou mais itens'));
                                ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="control-label col-md-2" for="id_periodo_referencia">Periodos</label>
                            <div class="col-md-10">
                                <?php
                                $strSQL = "
                                    SELECT
                                      id_periodo_referencia as codigo,
                                      titulo || ' (' || to_char(inicio_validade,'MM/YYYY') ||' até ' || to_char(fim_validade,'MM/YYYY') || ') / ' || id_exercicio as descricao
                                    FROM planacomorc.periodo_referencia
                                    ORDER BY 1
                                ";
                                echo inputCombo('id_periodo_referencia', $strSQL, isset($_REQUEST['id_periodo_referencia']) ? $_REQUEST['id_periodo_referencia'] : '', 'id_periodo_referencia');
                                ?>
                            </div>
                        </div>
                    </div>
                </form>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                <button type="button" class="btn btn-primary" id="btn-Salva-Questionario">Salvar</button>
            </div>
        </div>
    </div>
</div>
<section class="col-md-12">
    <div class="well">
        <section class="form-horizontal">
            <? $questionario->filtroPeriodo(); ?>
            <button type="button" class="btn btn-success" data-toggle="modal" data-target="#insert-new">Inserir novo</button>
        </section>
    </div>


<?php
    $questionario->renderListagem('viewQuestionario', 'deleteQuestionario');
?>
</section>
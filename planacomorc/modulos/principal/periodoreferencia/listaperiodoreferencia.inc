<?php
include_once APPRAIZ . 'includes/funcoesspo.php';
include_once '_funcoesperiodoreferencia.php';

if($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

$metaDataCombo = array(
    'size' => 10,
    'classe' => 'datepicker'
);

include  APPRAIZ."includes/cabecalho.inc";
?>
<script type="text/javascript" language="javascript" src="/planacomorc/js/moment-with-locales.js"></script>
<script type="text/javascript" language="javascript" src="/planacomorc/js/bootstrap-datetimepicker.min.js"></script>
<link href="/planacomorc/css/bootstrap-datetimepicker.min.css" rel="stylesheet" type="text/css">
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript">
$(function(){
    $(".datepicker").datetimepicker({
        language: "pt-br",
        pickTime: false,        
        useCurrent: false
    });

    $(".btn-insert-new").on("click", function(){
        var fieldKeys = ["#percod", "#pertitulo", "#perdescricao", "#periniciovalidade", "#perfimvalidade",
                         "#perinicioaberturapreenchimento", "#perfimaberturapreenchimento"];
        for (var i in fieldKeys) {
            if (fieldKeys.hasOwnProperty(i)) {
                $(fieldKeys[i]).val("");
            }
        }
        //$("#perdescricao").focus().blur();
        $("#perexibirindicadores").attr("checked", false);
        $("#requisicao").val("inserirPeriodo");
    });

    $("#btn-salva-periodo-ref").on("click", function(){

        if (!validaForm()) return false;

        var postData = $("#formPeriodo").serialize();
        $.ajax({
            url: 'planacomorc.php?modulo=principal/periodoreferencia/listaperiodoreferencia&acao=A',
            data: postData,
            success: function(data) {
                texto = JSON.parse(data).alerta;
                //console.log(JSON.parse(data).alerta);
                alert(texto);
                window.location = window.location;
            }
        });
    });
});

function abrirPeriodo(percod) {
    $.ajax({
        url:'planacomorc.php?modulo=principal/periodoreferencia/listaperiodoreferencia&acao=A',
        data: {requisicao:'exibirPeriodoReferencia',percod:percod},
        success: function(data) {
            data = JSON.parse(data);
            //console.log(data);
            $("#requisicao").val("atualizarPeriodo");
            $("#percod").val(data.percod);
            $("#pertitulo").val(data.pertitulo);
            $("#perdescricao").val(data.perdescricao);
            $("#periniciovalidade").val(data.periniciovalidade);
            $("#perfimvalidade").val(data.perfimvalidade);
            $("#perinicioaberturapreenchimento").val(data.perinicioaberturapreenchimento);
            $("#perfimaberturapreenchimento").val(data.perfimaberturapreenchimento);
            if (data.perexibirindicadores == 't')
                $("#perexibirindicadores").attr("checked", true);

            $("#insert-new").modal("show");
        },
        complete: function() {
            //$("#perdescricao").focus().blur();
        }
    })
}

function excluirPeriodo(percod) {
    if (confirm('Deseja realmente excluir o Período?')) {
        window.location = 'planacomorc.php?modulo=principal/periodoreferencia/listaperiodoreferencia&acao=A&requisicao=excluirPeriodoReferencia&percod='+percod;
    }
}

function validaForm() {
    var fieldKeys = ["pertitulo", "perdescricao", "periniciovalidade", "perfimvalidade",
        "perinicioaberturapreenchimento", "perfimaberturapreenchimento"];

    for (var i in fieldKeys) {
        if (fieldKeys.hasOwnProperty(i)) {
            if (!$("#"+fieldKeys[i]).val()) {
                var label = $("label[for='"+fieldKeys[i]+"']").html();
                alert("O campo "+label+" é de preechimento obrigatório!");
                $("#"+fieldKeys[i]).focus();
                return false;
            }
            return true;
        }
    }
}
</script>

<div class="row">
    <div class="col-lg-12">    	   	
        <ol class="breadcrumb">
            <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?= $_SESSION['sisdsc']; ?></a></li>
            <li class="">Ações <?= $_SESSION['exercicio']; ?></li>
            <li class="active">Períodos de Referência do Acompanhamento</li>
        </ol>
    </div>
</div>

<div class="row">
    <div class="col-lg-12">
        <div class="well">
            <button type="button" class="btn btn-success btn-insert-new" data-toggle="modal" data-target="#insert-new">Inserir novo</button>
        </div>
    </div>
    <div class="col-lg-12">&nbsp;</div>
</div>

<div class="modal fade" id="insert-new" tabindex="-1" role="dialog" aria-labelledby="ncModalLabel" aria-hidden="true">
    <div class="modal-dialog" style="width:70%;">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal">
                    <span aria-hidden="true">&times;</span>
                    <span class="sr-only">Close</span>
                </button>
                <h4 class="modal-title">Formulário</h4>
            </div>
            <div class="modal-body">
                <form class="form-horizontal"
                      name="formPeriodo"
                      id="formPeriodo"
                      action=""
                      method="POST"
                      role="form">
                    <div class="row">
                        <input type="hidden" name="requisicao" id="requisicao" value="" />
                        <input type="hidden" name="percod" id="percod" value="<?= isset($_REQUEST['percod']) ? $_REQUEST['percod'] : '' ?>" />
                        <div class="form-group">
                            <label class="control-label col-md-2" for="pertitulo">Título</label>
                            <div class="col-md-10">
                                <?= inputTexto('pertitulo', isset($_REQUEST['pertitulo']) ? $_REQUEST['pertitulo'] : '', 'pertitulo', '10', false); ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="control-label col-md-2" for="perdescricao">Descrição</label>
                            <div class="col-md-10">
                                <?= inputTextArea('perdescricao', isset($_REQUEST['perdescricao']) ? $_REQUEST['perdescricao'] : '', 'perdescricao', '100'); ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="control-label col-md-2" for="periniciovalidade">Período de validade</label>
                            <div class="col-md-5">
                                <?= inputTexto('periniciovalidade', isset($_REQUEST['periniciovalidade']) ? $_REQUEST['periniciovalidade'] : '', 'periniciovalidade', '10', false, $metaDataCombo); ?>
                            </div>
                            <div class="col-md-5">
                                <?= inputTexto('perfimvalidade', isset($_REQUEST['perfimvalidade']) ? $_REQUEST['perfimvalidade'] : '', 'perfimvalidade', '10', false, $metaDataCombo); ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="control-label col-md-2" for="perinicioaberturapreenchimento">Período de preenchimento</label>
                            <div class="col-md-5">
                                <?= inputTexto('perinicioaberturapreenchimento', isset($_REQUEST['perinicioaberturapreenchimento']) ? $_REQUEST['perinicioaberturapreenchimento'] : '', 'perinicioaberturapreenchimento', '10', false, $metaDataCombo); ?>
                            </div>
                            <div class="col-md-5">
                                <?= inputTexto('perfimaberturapreenchimento', isset($_REQUEST['perfimaberturapreenchimento']) ? $_REQUEST['perfimaberturapreenchimento'] : '', 'perfimaberturapreenchimento', '10', false, $metaDataCombo); ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="control-label col-md-2" for="perexibirindicadores">Deseja exibir os indicadores</label>
                            <div class="col-md-10">
                                <input type="checkbox" id="perexibirindicadores" name="perexibirindicadores" value="<?= isset($_REQUEST['perexibirindicadores']) ? $_REQUEST['perexibirindicadores'] : '' ?>">
                            </div>
                        </div>
                    </div>
                </form>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                <button type="button" class="btn btn-primary" id="btn-salva-periodo-ref">Salvar</button>
            </div>
        </div>
    </div>
</div>
<div class="col-lg-12">
<?php

$colunms = array(
    'Título',
    'Validade',
    'Preenchimento'
);

$setAcoes = array(
    'edit' => 'abrirPeriodo',
    'delete' => 'excluirPeriodo'
);

$query = "
    SELECT
        id_periodo_referencia,
        titulo as titulo,
        to_char(inicio_validade, 'DD/MM/YYYY') || ' à ' || to_char(fim_validade, 'DD/MM/YYYY') as validade,
        to_char(inicio_preenchimento, 'DD/MM/YYYY') || ' à ' || to_char(fim_preenchimento, 'DD/MM/YYYY') as preenchimento
	FROM planacomorc.periodo_referencia pr
	WHERE id_exercicio = %s
";

$query = sprintf($query, (int) $_SESSION['exercicio']);

require_once(APPRAIZ . 'includes/library/simec/Listagem.php');

$list = new Simec_Listagem();
$list->setCabecalho($colunms)
    ->setAcoes($setAcoes)
    ->turnOnPesquisator()
    ->setQuery($query);

$list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
</div>
<?php
include_once APPRAIZ . 'includes/funcoesspo.php';
include_once '_funcoesproduto.php';

if($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
?>
<script type="text/javascript">

    $(function(){
        $(".btn-insert-new").on("click", function(){
            $.ajax({
                url: 'planacomorc.php?modulo=principal/produto/listametodologia&acao=A',
                data: {
                    requisicao:"exibirMetodologia",
                    id_acao:'',
                    id_periodo_referencia:''
                },
                type:'POST',
                success: function(data) {
                    $("#body-modal-tag").html(data);
                    $("#insert-new").modal("show");                
                    setTimeout(function(){
                        $('.chosen-select').chosen();
                    },500); 
                }
            });
        });

        $("#btn-salva-metodologia").on("click", function(){
            if (!validaForm()) return false;

            var postData = $("#formmetodologia").serialize();
            $.ajax({
                url: 'planacomorc.php?modulo=principal/produto/listametodologia&acao=A',
                data: postData,
                type: 'POST',
                success: function(data) {
                    alert(JSON.parse(data).message);
                    location.reload();
                }
            });
        });


    });

    var validaForm = function() {
        if ($('#id_periodo_referencia').val()=='') {
            alert('Período de referência em branco');
            $('#id_periodo_referencia').focus();
            return false;
        }

        if ($('#id_acao').val()=='') {
            alert('Código da ação em branco');
            $('#id_acao').focus();
            return false;
        }
        return true;
    };

    var abrirMetodologia = function(cod) {
        if (!cod) return false;

        $.ajax({
            url: "planacomorc.php?modulo=principal/produto/listametodologia&acao=A",
            data: "requisicao=exibirMetodologia&cod="+cod,
            type: 'GET',
            success: function(data) {
                $("#body-modal-tag").html(data);
                $("#insert-new").modal("show");
            }
        });
    };

    function selecionarAcaoMetologia() {
        if ($('#id_periodo_referencia').val()=='') {
            return false;
        }

        if ($('#id_acao').val()=='') {
            return false;
        }

        $.ajax({
            url: "planacomorc.php?modulo=principal/produto/listametodologia&acao=A",
            data: {
                requisicao: "carregarSubacao",
                id_periodo_referencia: $('#id_periodo_referencia').val(),
                id_acao: $('#id_acao').val()
            },
            type: "post",
            success: function(data) {
                $("#div_subacao").html(data);
            }
        });
    }

    function excluirMetodologia(cod) {
        if (confirm('Deseja realmente excluir o Produto?')) {
            window.location = 'planacomorc.php?modulo=principal/produto/listametodologia&acao=A&requisicao=excluirProduto&psbid='+cod;
        }
    }

</script>


<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?= $_SESSION['sisdsc']; ?></a></li>
        <li class="">Ações <?= $_SESSION['exercicio']; ?></li>
        <li class="active">Lista de Metodologias</li>
    </ol>
    <div class="well">
        <button type="button" class="btn btn-success btn-insert-new">Inserir novo</button>
    </div>
<?php

$colunms = array(
    'Ação',
    'Período'
);

$setAcoes = array(
    'edit' => 'abrirMetodologia',
    'delete' => 'excluirMetodologia'
);

$query = "
    SELECT
        m.id_metodologia_conversao as cod,
        aca.codigo as acao,
        p.titulo || ' : ' || TO_CHAR(p.inicio_validade,'dd/mm/yyyy') || ' à ' || TO_CHAR(p.fim_validade,'dd/mm/yyyy')  as periodo
    FROM planacomorc.metodologia_conversao m
    INNER JOIN planacomorc.periodo_referencia p ON p.id_periodo_referencia = m.id_periodo_referencia
    INNER JOIN planacomorc.acao aca USING(id_acao)
    WHERE p.id_exercicio = %s
    GROUP BY m.id_metodologia_conversao,m.id_acao, m.id_periodo_referencia, p.titulo, p.inicio_validade, p.fim_validade, aca.codigo
    ORDER BY m.id_metodologia_conversao DESC
";

$query = sprintf($query, (int) $_SESSION['exercicio']);

require_once(APPRAIZ . 'includes/library/simec/Listagem.php');

$list = new Simec_Listagem();
$list->setCabecalho($colunms)
    ->turnOnPesquisator()
    ->setAcoes($setAcoes)
    ->setQuery($query);

$list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
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
            <div class="modal-body" id="body-modal-tag">

            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                <button type="button" class="btn btn-primary" id="btn-salva-metodologia">Salvar</button>
            </div>
        </div>
    </div>
</div>
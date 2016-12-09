<?php
include_once '_funcoessubacoes.php';
if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}
//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
?>
<script>
    //Função que executa a busca textual
    $(document).ready(function()
    {
        pesquisaRapida('textFind');
    });

    /* Editar a Subação */
    function alterarSubacao(sbaid) {
        location.href = 'planacomorc.php?modulo=principal/subacoes/cadastro&acao=A&sbaid=' + sbaid;
    }

    /* Excluir a Subação */
    function removerSubacao(sbaid, sbacod) {
        if (confirm('Deseja apagar a Subação ' + sbacod + '?')) {
            location.href = 'planacomorc.php?modulo=principal/subacoes/cadastro&acao=A&apagar=true&sbaid=' + sbaid;
        }
    }

    function detalheSubacao(sbaid) {
        var url = 'planacomorc.php?modulo=principal/subacoes/detalhesubacao&acao=A&sbaid=' + sbaid;
        $.post(url, function(html) {
            $('#modal-confirm .modal-body p').html(html);
            $('.modal-dialog').css('width', '90%');
            $('#modal-confirm .modal-title').html('Dados da Subação');
            $('#modal-confirm .btn-primary').remove();
            $('#modal-confirm .btn-default').html('Fechar');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
        });

    }

    function pesquisaRapida(id) {
        jQuery.expr[':'].contains = function(a, i, m) {
            return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
        };

        $("#" + id).keyup(function()
        {
            $('table.table tbody tr td').removeClass('marcado');
            $('table.table tbody tr').removeClass('remover');
            stringPesquisa = $("#" + id).val();
            if (stringPesquisa) {
                $('table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                $('table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
            }
        });
    }
</script>

<div class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?= $_SESSION['sisdsc']; ?></a></li>
        <li class="">Subações <?= $_SESSION['exercicio']; ?></li>
        <li class="active">Lista de Subações</li>
    </ol>	
    <div id="dv_listaunidades">        
        <?php listaSubacoesManter_bootstrap(array('caixaAzul'=>'S'),'S'); ?>
    </div>
</div>
<?php
require (APPRAIZ . 'includes/library/simec/Listagem.php');
include APPRAIZ . 'includes/cabecalho.inc';
?>
<style type="text/css">
    .marcado{background-color:#C1FFC1 !important;}
    .remover{ display:none; }
</style>
<script type="text/javascript">
    $(document).ready(function() {
        jQuery.expr[':'].contains = function(a, i, m) {
            return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
        };

        $("#textFind").keyup(function()
        {
            $('#listagem>table.table tbody tr td').removeClass('marcado');
            $('#listagem>table.table tbody tr').removeClass('remover');
            stringPesquisa = $("#textFind").val();
            if (stringPesquisa) {
                $('#listagem>table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                $('#listagem>table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
            }
        });
    });
</script>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="disabled">Sistema</li>
        <li class="disabled">Tabelas de Apoio</li>
        <li class="active">Lista de Unidades Orçamentárias (UO)</li>
    </ol>	
</section>
<div class="col-lg-12">
    <label for="textFind" style="margin-bottom:3px">Pesquisa rápida: </label> <input class="normal form-control" type="text" id="textFind">
    <div id="listagem"> 

        <?php
        $listagem = new Simec_Listagem();
        $sql = " 
            SELECT
                unicod,
                orgcod,
                unidsc,
                unicodresp
            FROM
                siafi.uo 
            ORDER BY unicod ";

        $listagem->setCabecalho(array(
            "Código da UO",
            "Código do Órgão",
            "Nome da UO",
            "Responsável"));
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        ?>

        <?php if (false === $listagem->render()): ?>
            <div  class="alert alert-info col-md-4 col-md-offset-4 text-center" >
                Nenhum registro encontrado.
            </div>
        <?php endif; ?>
        <div class="form-group">
            <div class="col-lg-12" style="text-align: center;">
                <button class="btn btn-primary" id="voltar" type="button" onclick="voltar();">Voltar</button>
            </div>
        </div>
    </div>
</div>
?>
<script type="text/javascript">
    function voltar() {
        window.location = "siafi.php?modulo=inicio&acao=C";
    }
</script>

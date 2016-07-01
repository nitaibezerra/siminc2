<?php
/**
 * Lista de funcionais para definição de metas.
 * $Id: listarMetas.inc 84715 2014-08-15 12:38:00Z maykelbraz $
 */
?>
<style>
.marcado{background-color:#C1FFC1!important}
.remover{display:none}
</style>
<label for="textFind" style="margin-bottom:3px">Pesquisa rápida: </label>
<input class="normal form-control" type="text" id="textFind" />
<script type="text/javascript" lang="JavaScript">
function detalharAcao(acaid)
{
    $('#acaid').val(acaid);
    $('#formacao').submit();
}
jQuery(document).ready(function(){
    jQuery.expr[':'].contains = function(a, i, m) {
        return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
    };

    $("#textFind").keyup(function()
    {
        $('#listagem table.table tbody tr td').removeClass('marcado');
        $('#listagem table.table tbody tr').removeClass('remover');
        stringPesquisa = $("#textFind").val();
        if (stringPesquisa) {
            $('#listagem table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
            $('#listagem table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
        }
    });
});
</script>
<br />
<form name="formacao" id="formacao" method="POST" role="form">
	<input type="hidden" name="requisicao" id="requisicao"
		value="editarAcao" /> <input type="hidden" name="dados[acaid]"
		id="acaid" />
</form>
<div id="listagem">
<?php
imprimirAcoes($dados, $dados ['ppoid'], $_SESSION ['exercicio'], 'metas');
?>
</div>
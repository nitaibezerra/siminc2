<?php
/**
 * Listagem de períodos de referência.
 *
 * $Id: listarPeriodoreferencia.inc 99320 2015-06-29 14:31:14Z lindalbertofilho $
 */

$where = array("prsano = '{$_SESSION['exercicio']}'");
if (isset($_REQUEST['periodoreferencia']) && $_REQUEST['periodoreferencia']['prftitulo']) {
    $where[] = sprintf('prftitulo ILIKE %s', "'%{$_REQUEST['periodoreferencia']['prftitulo']}%'");
}

$list = new Simec_Listagem();
$list->setCabecalho(array('Título', 'Validade' => array('Início', 'Fim'), 'Preenchimento' => array('Início', 'Fim')))
    ->addAcao('edit', 'editarPeriodoreferencia')
    ->addAcao('delete', array('func' => 'deletarPeriodoreferencia', 'extra-params' => array('prftitulo')))
    ->setQuery($oPerRef->recuperarTodos(
        'prfid, prftitulo, prfinicio, prffim, prfpreenchimentoinicio, prfpreenchimentofim',
        $where,
        'prfid',
        array()
    ))->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
<script type="text/javascript">
function deletarPeriodoreferencia(prfid, prftitulo)
{
    bootbox.confirm('Tem certeza que deseja excluir o período de referência "' + prftitulo + '"?', function(result){
        if (result) {
        }
    });
}

function editarPeriodoreferencia(prfid)
{
    window.location.href = 'proporc.php?modulo=principal/periodoreferencia/periodoreferencia&acao=A&requisicao=editar&id=' + prfid;
}
$(function(){
    $('.btn-limpar').click(function(){
        window.location.replace(window.location.href);
    });
});
</script>
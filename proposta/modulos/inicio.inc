<?
$mProposta = new Proposta_Model_Proposta($_REQUEST['proid']);
switch ($_REQUEST['req']) {
    case 'gerar-planilha':
        $mProposta->gerarPlanilhaImportacao();
        die;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

?>
<br>
<a class="btn btn-danger" href="?modulo=inicio&acao=C&req=gerar-planilha">Gerar Planilha</a>
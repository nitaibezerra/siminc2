<?
include_once '_funcoesquestionariorelatoriogestao.php';

if ($_REQUEST ['requisicao']) {
    $_REQUEST ['requisicao'] ( $_REQUEST );
    exit ();
}

// Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
require APPRAIZ . 'includes/library/simec/Listagem.php';

/**
 * Classe de Questionário
 */
require APPRAIZ . 'planacomorc/classes/Questionario.class.inc';

$questionario = new Questionario(Questionario::CAIXA_LARANJA);
?>

<script>
    function abrirQuestionario(){
        <? $_SESSION['planacomorc']['tqtid'] = null; ?>
        window.location = 'planacomorc.php?modulo=principal/questionariorelatoriogestao/gerenciarquestionario&acao=A&aba=dados';
    }

    function carregarQuestionario(tqtid) {
        window.location = 'planacomorc.php?modulo=principal/questionariorelatoriogestao/listaquestionario&acao=A&requisicao=carregarQuestionario&tqtid=' + tqtid;
    }

    function excluirQuestionario(tqtid) {
        var conf = confirm('Deseja realmente excluir o Questionário?');
        if (conf) {
            window.location = 'planacomorc.php?modulo=principal/questionariorelatoriogestao/listaquestionario&acao=A&requisicao=excluirQuestionario&tqtid=' + tqtid;
        }
    }
</script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<section class="col-md-12">
    <ol class="breadcrumb">
            <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
            <li>Questionário de Relatório de Gestão</li>
            <li class="active">Listagem de Questionário</li>
    </ol>		
    <div class="well">
        <div class="form-horizontal">
            <? $questionario->filtroPeriodo(); ?>
            <input type="button" class="btn btn-success" value="Inserir novo" onclick="abrirQuestionario();">
        </div>
    </div>  
<?
    $questionario->renderListagem();
?>					
</section>
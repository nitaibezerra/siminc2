<?php
/**
 * Dados da ação para justificativa.
 * @version $Id: inicio.inc 89632 2014-11-04 12:59:02Z maykelbraz $
 */

// -- Identificador do relatório de gestão
$rlgid = $_REQUEST['rlgid'];

if (!$dados['rldid']) {
    echo <<<HTML
<div class="alert alert-danger col-lg-6 col-lg-offset-3" role="alert">
    Antes de visualizar o formulário de justificativas, é necessário cadastrar uma ação.
</div>
HTML;
    return;
}

// -- Template de dados da ação
$dados['exercicio'] = $_SESSION['exercicio'];
$quadroAcao = preencheTemplate(
    file_get_contents(dirname(__FILE__) . '/../acaorap/html-pdf.php'),
    $dados
);

// -- CSS de formatação do quadro da ação
echo file_get_contents(dirname(__FILE__) . '/../css-relatorio.php');
?>
<div class="col-lg-12">
    <?php if (empty($dados['docid'])): ?>
    <div class="col-lg-12">
    <?php else: ?>
    <div class="col-lg-11">
    <?php endif; ?>
        <div class="col-lg-10 col-lg-offset-1 quadro-tcu">
            <?php echo $quadroAcao; ?>
        </div>
    </div>
    <?php if (!empty($dados['docid'])): ?>
    <div class="col-lg-1"><?php wf_desenhaBarraNavegacao($dados['docid'], array()); ?></div>
    <?php endif; ?>
    <br style="clear:both" />
    <br />
    <br />
    <div class="col-lg-12">
        <div class="page-header">
            <h4>Justificativas</h4>
        </div>
        <div class="well">
    <?php
    $questionario = carregarQuestionarioTcu($dados['rldid']);

    /**
     * Questionário do TCU.
     * @see questionario.inc
     */
    require_once(dirname(__FILE__) . '/../questionario.inc');
    ?>
        </div>
    </div>
</div>
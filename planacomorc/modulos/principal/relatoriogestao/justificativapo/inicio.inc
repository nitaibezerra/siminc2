<?php
/**
 * Dados da justificativa do PO.
 * @version $Id: inicio.inc 89155 2014-10-24 21:50:49Z maykelbraz $
 */

// -- Identificador do relatório de gestão
$rlgid = $dados['rlgid'];
$rldid = $dados['rldid'];

// -- Template de dados da ação
$dados['exercicio'] = $_SESSION['exercicio'];
$quadroAcao = preencheTemplate(
    file_get_contents(dirname(__FILE__) . '/../po/html-pdf.php'),
    current(consultarDadosTCU($rlgid, $rldid)) // -- VERIFICAR: Acho que não precisa mais desta segunda consulta - consultardadostcu
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
    $questionario = carregarQuestionarioTcu($rldid);

    /**
     * Questionário do TCU.
     * @see questionario.inc
     */
    require_once(dirname(__FILE__) . '/../questionario.inc');
    ?>
        </div>
    </div>
</div>
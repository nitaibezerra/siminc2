<?php
/**
 * Dados da justificativa do PO.
 * @version $Id: inicio.inc 95734 2015-03-24 18:09:56Z lindalbertofilho $
 */

// -- Identificador do relatório de gestão
$rlgid = $dados['rlgid'];
$rldid = $dados['rldid'];

// -- Template de dados da ação
$dados['exercicio'] = $_SESSION['exercicio'];
$quadroAcao = preencheTemplate(
    file_get_contents(dirname(__FILE__) . '/../po/html-pdf.php'),
    current($modeloRelatorio->consultarDadosTCU($rlgid, $rldid)) // -- VERIFICAR: Acho que não precisa mais desta segunda consulta - consultardadostcu
);

?>
<!-- CSS de formatação do quadro da ação -->
<link href="/acomporc/css/relatorio.css" rel="stylesheet" media="screen">
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
        <?php
        $questionarios = $questionarioRelatorioTCU->listaJustificativas(array('acacod' => $dadosAcao['acacod'], 'prfid' => $prfid, 'rldid' => $dados['rldid']));

        /**
         * Questionário do TCU.
         * @see questionario.inc
         */
        require_once(dirname(__FILE__) . '/../../questionario.inc');
        ?>
    </div>
</div>
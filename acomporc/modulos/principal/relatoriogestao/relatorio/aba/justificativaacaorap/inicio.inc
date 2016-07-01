<?php
/**
 * Dados da ação para justificativa.
 * @version $Id: inicio.inc 96564 2015-04-22 14:46:00Z lindalbertofilho $
 */

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
?>
<!-- CSS de formatação do quadro da ação -->
<link href="/acomporc/css/relatorio.css" rel="stylesheet" media="screen">
<div class="row">
    <div class="<?= (empty($dados['docid']))? 'col-lg-12' : 'col-lg-11' ?>">
        <div class="col-lg-10 col-lg-offset-1 quadro-tcu">
            <?php echo $quadroAcao; ?>
        </div>
    </div>
    <?php if (!empty($dados['docid'])): ?>
    <div class="col-lg-1"><?php wf_desenhaBarraNavegacao($dados['docid'], array()); ?></div>
    <?php endif; ?>
</div>
<div class="col-lg-12">
    <div class="page-header">
        <h4>Justificativas</h4>
    </div>
    <?php
    $questionarios = $questionarioRelatorioTCU->listaJustificativas(array('acacod' => $dados['codigo'], 'prfid' => $prfid, 'rldid' => $dados['rldid']));

    require_once(dirname(__FILE__) . '/../../questionario.inc');
    ?>
</div>
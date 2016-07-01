<?php
/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */

//Declaração de Objetos
$modelo = new Spo_Service_Snapshot();
$modelo->setFlashMessage(new Simec_Helper_FlashMessage('spo/snapshotSubacao'));
try{
    if (isset($_POST['requisicao']) || !empty($_POST['requisicao'])) {
        $requisicao = $_POST['requisicao'];
        switch ($requisicao) {
            case 'cargaSubacao':
                $resultado = $modelo->executaCargaSubacao($_POST);
                $modelo->getFlashMessage()->addMensagem($resultado['mensagem']['texto'], $resultado['mensagem']['tipo']);
                break;
        }
    }
}catch(Exception $e) {
    $modelo->getFlashMessage()->addMensagem($e->getMessage(), Simec_Helper_FlashMessage::ERRO);
}

/**
 * Cabecalho padrão do SIMEC.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
?>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="">Snapshot</li>
        <li class="">Acompanhamento de Subação</li>
    </ol>
<?php
echo $modelo->getFlashMessage()->getMensagens();
require(dirname(__FILE__) . "/snapshotSubacao/formAcompanhamentoSubacao.inc");
?>
</section>
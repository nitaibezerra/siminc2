<?php
/**
 * Arquivo de controle da gestão de período de referência.
 *
 * $Id: periodoreferencia.inc 99374 2015-06-29 19:25:31Z lindalbertofilho $
 */

$fm = new Simec_Helper_FlashMessage('proporc/gestao-periodoreferencia');
$oPerRef = new Proporc_Model_Periodoreferencia($_REQUEST['id']);

$render = 'listagem';
if (isset($_REQUEST['requisicao'])) {
    switch ($_REQUEST['requisicao']) {
        case 'novo':
        case 'editar':
            $render = 'cadastro';
            break;
        case 'salvar':
        case 'deletar':
            $service = new Proporc_Service_Periodoreferencia();
            $service->setDados($_POST['periodoreferencia'])
                ->setFlashMessage($fm);
            $service->{$_REQUEST['requisicao']}();
            header('Location: /proporc/proporc.php?modulo=principal/periodoreferencia/periodoreferencia&acao=A');
            die();
    }
}

/**
 * Cabecalho do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<div class="row col-md-12">
<?php if ('listagem' == $render): ?>
    <ol class="breadcrumb">
        <li>
            <a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisdsc']; ?></a>
        </li>
        <li class="active">Períodos de referência</li>
    </ol>
    <?php
    require_once dirname(__FILE__) . '/formPeriodoreferenciaFiltro.inc';
    echo $fm->getMensagens();
    require_once dirname(__FILE__) . '/listarPeriodoreferencia.inc';

else: ?>
    <ol class="breadcrumb">
        <li>
            <a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisdsc']; ?></a>
        </li>
        <li>
            <a href="proporc.php?modulo=principal/periodoreferencia/periodoreferencia&acao=A">Períodos de referência</a>
        </li>
        <li><?php echo isset($_REQUEST['id'])?'Alterando':'Novo'; ?></li>
    </ol>
    <?php require_once dirname(__FILE__) . '/formPeriodoreferencia.inc';
endif;
?>
</div>
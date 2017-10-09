<?php
/**
 * Listagem de períodos de referência.
 * $Id: listaperiodoreferencia.inc 98390 2015-06-09 18:48:49Z maykelbraz $
 */

$formData = array();
if (chaveTemValor($_REQUEST, 'action')) {
    // -- Coletando dados do formulário para processamentos
    $formData = $_REQUEST['data'];
    switch ($_POST['action']) {
        case 'search':
            break;
        case 'insert':
            require APPRAIZ . "www/recorc/funcoesperiodoreferencia.php";
            setFlashMessageArray(inserirPeriodoReferencia($formData));
            list($newURI) = explode('&action=', $_SERVER['REQUEST_URI']);
            header('Location: ' . $newURI);
            die();
        case 'delete':
            require APPRAIZ . "www/recorc/funcoesperiodoreferencia.php";
            setFlashMessageArray(deletePeriodoReferencia($formData));
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        default:
            if ('load' == $_GET['action']) {
                require APPRAIZ . "www/recorc/funcoesperiodoreferencia.php";
                $formData = carregarPeriodoReferencia($formData);
            }
    }
}

/**
 * Cabeçalho do sistema
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<div class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Período de Referência</li>
    </ol>
<?php
require(dirname(__FILE__)."/formperiodoreferencia.inc");
require(dirname(__FILE__)."/listagemperiodoreferencia.inc");
?>
</div>
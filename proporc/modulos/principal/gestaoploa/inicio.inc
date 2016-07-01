<?php
/**
 * Controle de seleção de UNICOD.
 * $Id: inicio.inc 96563 2015-04-22 14:29:02Z maykelbraz $
 */

/**
 * Funções de apoio à gestão da ploa.
 * @see _funcoesgestaoploa.php
 */
require(APPRAIZ . "www/{$_SESSION['sisdiretorio']}/_funcoesgestaoploa.php");

/**
 * Helper de exibição de alertas entre requisições.
 * @see Simec_Helper_FlashMessage
 */
$fm = new Simec_Helper_FlashMessage("{$_SESSION['sisdiretorio']}/gestaoploa");

// -- Carregando dados da gestão
$dados = &dadosDaSessao('gestaoploa');

if (isset($_POST['requisicao']) && !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    switch ($requisicao) {
        case 'selecionarUnicod':
            if (empty($_POST['dados']['unicod'])) {
                $fm->addMensagem(
                    'Você deve selecionar uma Unidade Orçamentária antes de avançar.',
                    Simec_Helper_FlashMessage::ERRO
                );
                continue;
            }
            carregarDadosDaUnidade($_POST['dados']);
            $novaURL = preg_replace('|\?[a-z/=]+|', '?modulo=principal/gestaoploa/limites', $_SERVER['REQUEST_URI']);
            break;
        default:
    }

    // -- Redirecionamento padrão
    if (!isset($novaURL)) {
        $novaURL = $_SERVER['REQUEST_URI'];
    }
    header('Location: ' . $novaURL);
    die();
} elseif ('A' == $_GET['acao']) {
    $dadosuo = limitarUOs($_SESSION['usucpf']);
    // -- Verificando se existe mais de uma UO para o usuário
    if (count($dadosuo) == 1) {
        $dadosuo = array('unicod' => current($dadosuo));
        carregarDadosDaUnidade($dadosuo);
        $novaURL = preg_replace('|\?[a-z/=]+|', '?modulo=principal/gestaoploa/limites', $_SERVER['REQUEST_URI']);
        header('Location: ' . $novaURL);
        $_SESSION[$_SESSION['sisdiretorio']]['gestaoploa']['escolheruo'] = false;
        die();
    }
    $_SESSION[$_SESSION['sisdiretorio']]['gestaoploa']['escolheruo'] = true;
}

/**
 * Cabecalho padrão do SIMEC.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
?>
<style type="text/css">
label{margin-top:10px;cursor:pointer}
</style>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Seleção de Unidade Orçamentária (UO)</li>
    </ol>
    <?php
    echo $fm->getMensagens();
    echo '<br style="clear:both" />';
    ?>
    <div class="col-md-12 well">
    <?php require(dirname(__FILE__) . '/uos/formUO.inc'); ?>
    </div>
    <div class="col-md-12">
        <?php require(dirname(__FILE__) . '/uos/listarUO.inc'); ?>
    </div>
</div>
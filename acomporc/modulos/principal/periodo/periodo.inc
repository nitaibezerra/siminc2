<?php
/**
 * Arquivo de questionário Acompanhamento Orçamentário.
 *@author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */

$fm = new Simec_Helper_FlashMessage('acomporc/periodo');
$modeloPeriodo = new Acomporc_Service_Periodo();
$modeloPeriodo->setFlashMessage($fm);

if (isset($_POST['requisicao']) || !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    switch ($requisicao) {
        case 'cadastrarPeriodo':
            if($_POST['prfid'] != ''){
                $modeloPeriodo->atualizarPeriodoReferencia($_POST);
            }else{
                $modeloPeriodo->salvarPeriodoReferencia($_POST);
            }
            break;
    }
    unset($_GET['requisicao']);
}

$tipo = $_REQUEST['tipo'];
$gerenciador = new Acomporc_Service_Gerenciador(Acomporc_Service_Gerenciador::CONTROLLER_PERIODO_PERIODO,$tipo);

if(isset($_GET['requisicao']) || !empty($_GET['requisicao'])){
    switch($_GET['requisicao'])
    {
        case 'update':
            if(isset($_GET['periodo'])){
                $dados = $modeloPeriodo->buscarPeriodoReferencia($_GET['periodo']);
                break;
            }
    }
}

/**
 * Cabecalho padrão do Sistema.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
?>
<script>
    $('body').on('click',function(e){
        if($(e.target).attr('id') == 'buttonSend'){
            window.location.href = $(e.target).attr('data-url');
        }else if($(e.target).attr('id') == 'buttonBack'){
            window.location.href = $(e.target).attr('data-url');
        }
    });

    function alterarPeriodo(id){
        window.location.href = 'acomporc.php?modulo=principal/periodo/periodo&acao=A&tipo=<?=$tipo?>&requisicao=update&periodo='+id;
    }

    function acessarPeriodo(id){
        window.location.href = 'acomporc.php?modulo=principal/periodo/periodo&acao=A&tipo=<?=$tipo?>&prfid='+id;
    }

</script>
<div class="row col-md-12">
    <?= $gerenciador->getBreadcrumb(); ?>
    <div class="row col-md-12">
        <?php
        $arquivo = '';
        // -- Exibindo mensagens do sistema
        echo $modeloPeriodo->getFlashMessage()->getMensagens();
        require(dirname(__FILE__) . "/" . $gerenciador->getURL());
        ?>
    </div>
</div>
<?php
/**
 * Arquivo de questionário Acompanhamento Orçamentário.
 *@author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */

$tipo = $_REQUEST['tipo'];
$fm = new Simec_Helper_FlashMessage('acomporc/associacao');
$questionarioModelo = new Acomporc_Service_Questionario();
$questionarioModelo->__set('tipoAcesso', $tipo);
$questionarioModelo->setFlashMessage($fm);

if(isAjax()){
    if(isset($_REQUEST['ajax'])){
        switch($_REQUEST['ajax']){
            case 'questionario':
                echo simec_json_encode(
                    array(
                        'questao'=> apresentaComboQuestionario($_REQUEST['prfid'],$_REQUEST['tipo'], $_REQUEST['func'])
                    )
                );
                die();
            case 'subacao':
            case 'acao':
            case 'tcu':
                echo $questionarioModelo->apresentaComboPorPeriodo($_REQUEST['prfid'], $_REQUEST['tipo'], $_REQUEST['qstid']);
                die();
        }
    }
}

if (isset($_POST['requisicao']) || !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    switch ($requisicao) {
        case 'cadastroAssociacaoAcao':
            $questionarioModelo->associarPeriodoAcoes($_POST);
            break;
        case 'cadastroAssociacaoSubacao':
            $questionarioModelo->associarPeriodoSubacoes($_POST);
            break;
        case 'cadastroAssociacaoTCU':
            $questionarioModelo->associarPeriodoTCU($_POST);
            break;
    }
    unset($_GET['requisicao']);
}


$prfid = $_REQUEST['periodo'];
$gerenciador = new Acomporc_Service_Gerenciador(Acomporc_Service_Gerenciador::CONTROLLER_ASSOCIACAO_ASSOCIACAO,$tipo);
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

    function alterarAssociacao(qstid,prfid){
        window.location.href = 'acomporc.php?modulo=principal/associacao/associacao&acao=A&tipo=<?=$tipo?>&requisicao=update&qstid='+qstid+'&prfid='+prfid;
    }

</script>
<div class="row col-md-12">
    <?= $gerenciador->getBreadcrumb(); ?>
    <div class="row col-md-12">
        <?php
        $arquivo = '';
        // -- Exibindo mensagens do sistema
        echo $questionarioModelo->getFlashMessage()->getMensagens();
        require(dirname(__FILE__) . "/" . $gerenciador->getURL());
        ?>
    </div>
</div>
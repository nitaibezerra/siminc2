<?php
/**
 * Arquivo de questionário Acompanhamento Orçamentário.
 *@author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */

$fm = new Simec_Helper_FlashMessage('acomporc/questao');
$modeloQuestao = new Acomporc_Service_Questao();
$modeloQuestao->setFlashMessage($fm);

if (isset($_POST['requisicao']) || !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    switch ($requisicao) {
        case 'cadastroQuestao':
            $modeloQuestao->salvarQuestao($_POST);
            break;
        case 'atualizaQuestao':
            $modeloQuestao->atualizarQuestao($_POST);
            break;
    }
    unset($_GET['requisicao']);
}

$questionario = $_REQUEST['questionario'];
$tipo = $_REQUEST['tipo'];

$gerenciador = new Acomporc_Service_Gerenciador(Acomporc_Service_Gerenciador::CONTROLLER_QUESTIONARIO_QUESTAO,$tipo);

$modeloQuestionario = new Acomporc_Service_Questionario();
$modeloQuestionario->__set('tipoAcesso', $tipo);
$nomeQuestionario = $modeloQuestionario->getName($questionario);

/**
 * Cabecalho padrão do SIMEC.
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

    function alterarQuestao(id){
        window.location.href = 'acomporc.php?modulo=principal/questionario/questao&acao=A&questionario=<?=$questionario?>&tipo=<?=$tipo?>&requisicao=update&id='+id;
    }

</script>
<div class="row col-md-12">
    <?= $gerenciador->getBreadcrumb(); ?>
    <div class="row col-md-12">
        <?php
        $arquivo = '';
        // -- Exibindo mensagens do sistema
        echo $modeloQuestao->getFlashMessage()->getMensagens();
        require(dirname(__FILE__) . "/" . $gerenciador->getURL());
        ?>
    </div>
</div>
<?php
/**
 * Arquivo de questionário Acompanhamento Orçamentário.
 *@author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */

$fm = new Simec_Helper_FlashMessage('acomporc/questionario');
$tipo = $_REQUEST['tipo'];
$questionarioModelo = new Acomporc_Service_Questionario();
$questionarioModelo->setFlashMessage($fm);
$questionarioModelo->__set('tipoAcesso', $tipo);

/**
 * Captura de Requisições em AJAX.
 */
if (isAjax()) {
    $requisicao = $_POST['requisicao'];
    switch($requisicao) {
        case 'delete':
            echo $questionarioModelo->removerQuestionario($_POST['id']);
            die;
    }
}

if (isset($_POST['requisicao']) || !empty($_POST['requisicao'])) {
    unset($_GET['requisicao']);
    $requisicao = $_POST['requisicao'];
    switch ($requisicao) {
        case 'novoQuestionario':
        case 'atualizaQuestionario':
            $questionarioModelo->salvarQuestionario($_POST);
            break;
    }
}

/* Objeto Gerenciador de links */
$gerenciador = new Acomporc_Service_Gerenciador(Acomporc_Service_Gerenciador::CONTROLLER_QUESTIONARIO_QUESTIONARIO,$tipo);

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

    function alterarQuestionario(id){
        window.location.href = 'acomporc.php?modulo=principal/questionario/questionario&acao=A&tipo=<?=$tipo?>&requisicao=update&id='+id;
    }

    function deletarQuestionario(id){
        bootbox.confirm('Você realmente deseja remover este questionário?',function(resp){
           if(resp){
                $.post(window.location,{requisicao: 'delete', id: id},function(response){
                    if(response){
                        bootbox.alert('Questionário removido com sucesso!',function(){
                            window.location = window.location;
                        });
                    }else{
                        bootbox.alert('Falha ao remover questionário.');
                    }
                });
           }
        });

    }

    function acessarPerguntas(id){
        window.location.href = 'acomporc.php?modulo=principal/questionario/questao&acao=A&questionario='+id+'&tipo=<?=$tipo?>';
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
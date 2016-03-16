<?php
/**
 * Arquivo de Acompanhamento Orçamentário.
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */

/* Funções de Gráficos do Cockpit */
include_once '../../pde/www/_funcoes_cockpit.php';
$fm = new Simec_Helper_FlashMessage('acomporc/central');
$modeloAcompanhamento = new Acomporc_Service_Acompanhamento();
$modeloAcompanhamento->setFlashMessage($fm);

if(isAjax()){
}
$prfid = $_GET['periodo'] ? $_GET['periodo'] : null;
$tipo = $_REQUEST['tipo'];
$gerenciador = new Acomporc_Service_Gerenciador(Acomporc_Service_Gerenciador::CONTROLLER_ACOMPANHAMENTO_CENTRAL,$tipo);

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

    function acessarSubacao(sbacod){
        window.location.href = 'acomporc.php?modulo=principal/acompanhamento/subacao&acao=A&periodo=<?=$prfid?>&requisicao=acessar&sbacod='+sbacod;
    }
    function acessarRelatorio(id){
        window.location.href = 'acomporc.php?modulo=principal/relatoriogestao/relatorio&acao=A&requisicao=relatorio&periodo=<?=$prfid?>&rlgid='+id;
    }

</script>
<div class="row col-md-12">
    <?= $gerenciador->getBreadcrumb(); ?>
    <div class="row col-md-12">
        <?php
        $arquivo = '';
        // -- Exibindo mensagens do sistema
        echo $modeloAcompanhamento->getFlashMessage()->getMensagens();
        require(dirname(__FILE__) . "/" . $gerenciador->getURL());
        ?>
    </div>
</div>
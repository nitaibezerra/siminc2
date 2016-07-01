<?php
/**
 * Arquivo de Acompanhamento Orçamentário.
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */

/**
 * Funções do workflow.
 * @see workflow.php
 */
require_once APPRAIZ . 'includes/workflow.php';

$fm = new Simec_Helper_FlashMessage('acomporc/relatoriotcucgu');
$perfis = pegaPerfilGeral($_SESSION['usucpf']);
if(isAjax()){
    if (isset($_POST['requisicao']) || !empty($_POST['requisicao'])) {
        $requisicao = $_POST['requisicao'];
        switch ($requisicao) {
            case 'consultar-acoes':
                filtroAcao($perfis, $_POST['acacod'], $_POST['unicod']);
                die;
        }
    }
}

// -- Chamando o gerador de PDF
if (isset($_GET['pdf']) && true == $_GET['pdf']) {
    exportarRelatorio('pdf', $_GET['rlgid'], $_GET['rldid'], $_GET['periodo']);
    die();
//-- Chamando o gerador de XLS
} else if(isset($_GET['xls']) && true == $_GET['xls']) {
    exportarRelatorio('xls', $_GET['rlgid'], $_GET['rldid'], $_GET['periodo']);
	die();
}

$tipo = 'T';
$gerenciador = new Acomporc_Service_Gerenciador(Acomporc_Service_Gerenciador::CONTROLLER_RELATORIOGESTAO_RELATORIO,$tipo);
$prfid = $_GET['periodo'] ? $_GET['periodo'] : null;

$modeloRelatorio = new Acomporc_Service_RelatorioGestaoTCU();
$modeloRelatorio->setFlashMessage($fm);
$questionarioRelatorioTCU = new Acomporc_Service_Questionario();
$questionarioRelatorioTCU->__set('tipoAcesso','T');
$questionarioRelatorioTCU->setFlashMessage($fm);

if (isset($_POST['requisicao']) && !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    switch ($requisicao) {
        case 'cadastrar':
            $resultado = $modeloRelatorio->cadastrarRelatorioTCU($_POST);
            break;
        case 'salvar':
            $dados = $_POST['dados'];
            $resultado = $modeloRelatorio->salvarAcao($dados);
            break;
        case 'apagar':
            $modeloRelatorio->deletarRelatorioGestao($_POST['rlgid']);
            break;
        case 'carregarUsuarios':
            require_once dirname(__FILE__) . '/relatorio/popupresponsavel.inc';
            die;
        case 'salvarResponsavel':
            $resultado = $modeloRelatorio->atribuirResponsavel($_POST['rldid'],$_POST['novo_usucpf'],$_GET['rlgid'], $prfid);
            if($_GET['aba'] != '' && $_GET['rldid'] != '') {
                $resultado['data'] = array('rlgid' => $_GET['rlgid'],'aba' => $_GET['aba'],'rldid' => $_GET['rldid']);
            }
            break;
        case 'salvarQuestionario':
            $resultado = $questionarioRelatorioTCU->salvarResposta(array_merge($_POST,array('prfid' => $prfid, 'rlgid' => $_GET['rlgid'], 'aba' => $_GET['aba'])));
            if($_GET['rldid'] != ''){
                $resultado['data']['rldid'] = $_GET['rldid'];
            }
            break;
    }
    if(isset($resultado['data'])){
        $gerenciador->addParams($resultado['data']);
        $gerenciador->redirect('acomporc.php?modulo=principal/relatoriogestao/relatorio&acao=A&requisicao=relatorio&periodo='.$prfid);
    }
    unset($_GET['requisicao']);
}

if(isset($_GET['requisicao']) && !empty($_GET['requisicao'])){
    $requisicao = $_GET['requisicao'];
    switch($requisicao){
        case 'filtro':
            unset($_GET['requisicao']);
            break;
        case 'relatorio':
            $rlgid = $_GET['rlgid'];
            $dadosAcao = $modeloRelatorio->buscaRelatorio($rlgid);

            $dados = $modeloRelatorio->buscaRelatorioAba($_GET['aba'],$rlgid,$dadosAcao['acatipo'],$_GET['rldid'] == '' ? 0 : $_GET['rldid']);
            // -- Configurações adicionais para os campos de input
            $configHabil = array('habil' => 'N');
            $configClasse = array('classe' => 'numerico');
            $configMasc = array('masc' => '###.###.###.###.###') + $configClasse;

            // -- Permissões de preenchimento de ações ou pos - responsabilidade
            $permissoesRld = consultarPermissoes($rlgid);

            // -- Tipo de usuário
            $usuarioTCU = in_array(PFL_RELATORIO_TCU, $perfis);
            break;

    }
}
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

    function acessarSubacao(id){
        window.location.href = 'acomporc.php?modulo=principal/acompanhamento/acompanhamento&acao=A&tipo=<?=$tipo?>&periodo=<?=$prfid?>&requisicao=acessar&asaid='+id;
    }

</script>
<div class="row col-md-12">
    <?= $gerenciador->getBreadcrumb(); ?>
    <div class="row col-md-12">
        <?php
        // -- Exibindo mensagens do sistema
        echo $modeloRelatorio->getFlashMessage()->getMensagens();
        require(dirname(__FILE__) . "/" . $gerenciador->getURL());
        ?>
    </div>
</div>
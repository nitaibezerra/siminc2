<?php
/**
 * Arquivo de acompanhamento de ações.
 *
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */

/**
 * Arquivo de funções do workflow
 * @see workflow.php
 */
require_once APPRAIZ . 'includes/workflow.php';
$tipo = 'A';
if (isset($_REQUEST['periodo'])) {
    $prfid = $_REQUEST['periodo'];
} else {
    $prfid = retornaPeriodoAtual('A');
}
$_POST['periodo'] = $prfid;
$_POST['prfid'] = $prfid;

//Declaração de Objetos.
$fm = new Simec_Helper_FlashMessage('acomporc/acao');
$modeloAcompanhamento = new Acomporc_Service_Acompanhamento();
$modeloAcompanhamento->setFlashMessage($fm);

$gerenciador = new Acomporc_Service_Gerenciador(Acomporc_Service_Gerenciador::CONTROLLER_ACOMPANHAMENTO_ACAO,$tipo);

/**
 * Captura de Requisições em AJAX.
 */
if (isAjax()) {
    $requisicao = $_POST['requisicao'];
    switch($requisicao) {
        case 'detalharLocalizadores':
            $modeloAcompanhamento->listarLocalizadores($_POST['dados'][0], $_POST['dados'][1]);
            die;
        case 'resumoLocalizadores':
            $retorno = $modeloAcompanhamento->resumoLocalizadores(
                $_POST['unicod'],
                $_GET['periodo'],
                $_POST['acacod']
            );
            echo simec_json_encode($retorno);
            die();
        case 'carregarUsuarios':
            apresentarMonitorInterno($_POST);
            die;
        case 'carregarValidador':
            apresentarValidadorAcao($_POST);
            die;
        case 'carregarCoordenadorAcao':
            apresentarCoordenadorAcao($_POST);
            die;
        case 'carregaSubacao':
            include APPRAIZ . 'acomporc/modulos/principal/acompanhamento/acao/view/dadosubacao.inc';
            die;
    }
}

if (isset($_POST['requisicao']) || !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    switch ($requisicao)
    {
        case 'salvarUsuarioResponsabilidade':
            $modeloAcompanhamento->atribuirResponsavel($_POST);
            break;
        case 'salvarAcompanhamento':
            $resultado = $modeloAcompanhamento->salvarAcompanhamentoAcao($_POST);
            break;
        case 'exibirLogEnvio':
            exibirLogEnvio(array_merge($_POST,array('prfid' => $_GET['periodo'])));
            die;
        case 'enviarSiop':
            $modeloAcompanhamento->setDados(array_merge(
                $_POST['dados'],
                array('prfid' => $_GET['periodo'])
            ));
            $modeloAcompanhamento->enviarAcompanhamentoAoSiop();
            break;
    }

    $gerenciador->redirect('acomporc.php?modulo=principal/acompanhamento/acao&acao=A',true);
    unset($_GET['requisicao']);
}

if (isset($_GET['requisicao']) || !empty($_GET['requisicao'])) {
    switch($_GET['requisicao']) {
        case 'acessar':
            $perfis = pegaPerfilGeral();
            $acao = $modeloAcompanhamento->recuperaDadosAcao($_REQUEST['acacod'], $_REQUEST['unicod'], $prfid);
            $dadosPO = $modeloAcompanhamento->listarPlanoOrcamentarioPorAcao($acao['sslid'],$prfid, $acao['aclid']);


            $modeloPeriodo = new Acomporc_Service_Periodo();
            $detalhePeriodo = $modeloPeriodo->detalhePeriodo($prfid);

            $modeloPeriodo->tipoAcesso = $tipo;
            $modeloPeriodo->statusAtual = $acao['esdid'];
            $modeloPeriodo->periodo = $prfid;
            $modeloPeriodo->perfil = $perfis;
            $apenasListar = $modeloPeriodo->podeSalvar();

            $acompanhamentoQuestionario = new Acomporc_Service_Questionario();
            $acompanhamentoQuestionario->tipoAcesso = $tipo;
            $questionarios = $acompanhamentoQuestionario->listaJustificativas(array(
                'prfid' => $prfid,
                'acacod' => $acao['acacod'],
                'unicod' => $acao['unicod'],
                'loccod' => $acao['loccod'],
                'aclid' => $acao['aclid']
            ));

            $breadcrumbCadastro = sprintf($gerenciador->getBreadcrumb(),'Ação', $acao['acacod']);
            break;
    }
}

/**
 * Cabecalho padrão do SIMEC.
 * @see cabecalho.inc
 */
require APPRAIZ . 'includes/cabecalho.inc';
?>
<script>
    $(document).ready(function(){
        $('body').on('click',function(e){
            if($(e.target).attr('id') == 'buttonSend'){
                window.location.href = $(e.target).attr('data-url');
            }else if($(e.target).attr('id') == 'buttonBack'){
                window.location.href = $(e.target).attr('data-url');
            }
        });
        // -- Salvar responsável
        $('.modal-dialog').on('click', '#salvar-responsavel', function(){
            $('#form-responsavel').submit();
        });
    });
    function acessarLocalizador(codigo,acacod,unicod) {
        window.location = 'acomporc.php?modulo=principal/acompanhamento/acao&acao=A&requisicao=acessar&periodo=<?=$prfid?>&acacod='+acacod+'&unicod='+unicod;
    }
    /**
    * Abre uma popup com uma busca ao log de envio daquela acaoprogramatica.
    * @param int idAcaoProgramatica ID da ação programática;
    */
   function exibirLogEnvio(aclid)
   {
       $.post(window.location, {requisicao: 'exibirLogEnvio', aclid: aclid}, function(content){
           alert(content);
       });
   }
</script>
<div class="row col-md-12">
    <?= (isset($_GET['requisicao']))? $breadcrumbCadastro : $gerenciador->getBreadcrumb(); ?>
    <div class="row col-md-12">
        <?php
        $arquivo = '';
        // -- Exibindo mensagens do sistema
        echo $modeloAcompanhamento->getFlashMessage()->getMensagens();
        require(dirname(__FILE__) . "/" . $gerenciador->getURL());
        ?>
    </div>
</div>
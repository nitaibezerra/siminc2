<?php
/**
 * Arquivo de Acompanhamento Orçamentário.
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */

include APPRAIZ ."includes/workflow.php";

$fm = new Simec_Helper_FlashMessage('acomporc/acompanhamento');
$modeloAcompanhamento = new Acomporc_Service_Acompanhamento();
$modeloAcompanhamento->setFlashMessage($fm);
/**
 * Captura de Requisições em AJAX.
 */
if(isAjax()){
}

if (isset($_POST['requisicao']) || !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    switch ($requisicao)
    {
        case 'gravarAcompanhamento':
            $modeloAcompanhamento->salvarAcompanhamento($_POST);
            break;
        case 'carregarUsuarios':
            apresentarCoordenadorSubacao($_POST);
            die;
        case 'salvarResponsavel':
            $modeloAcompanhamento->atribuirResponsavel(array('sbacod' => $_POST['sbacod'],'usucpf' => $_POST['usucpf'],'prfid' => $_POST['periodo']));
    }
}

$prfid = $_GET['periodo'] ? $_GET['periodo'] : null;
$tipo = 'S';
$gerenciador = new Acomporc_Service_Gerenciador(Acomporc_Service_Gerenciador::CONTROLLER_ACOMPANHAMENTO_SUBACAO,$tipo);
if(isset($_GET['requisicao']) || !empty($_GET['requisicao'])){
    switch($_GET['requisicao'])
    {
        case 'acessar':
            $perfil = pegaPerfilGeral();

            $modeloPeriodo = new Acomporc_Service_Periodo();
            $modeloPeriodo->__set('tipoAcesso',$tipo);
            $modeloPeriodo->__set('periodo', $prfid);
            $modeloPeriodo->__set('perfil', $perfil);
            $apenasListar = $modeloPeriodo->podeSalvar();

            $subacao = $modeloAcompanhamento->recuperaDadosSubacao($_GET['sbacod'],$prfid);
            $acoes = $modeloAcompanhamento->listarAcaoSubacao($_GET['sbacod'],$prfid);

            $acompanhamentoQuestionario = new Acomporc_Service_Questionario();
            $acompanhamentoQuestionario->__set('tipoAcesso', $tipo);
            $questionarios = $acompanhamentoQuestionario->listaJustificativas(array('prfid' => $prfid,'sbacod' => $subacao['sbacod'],'asaid'=> $subacao['asaid']));

            $breadcrumbCadastro = sprintf($gerenciador->getBreadcrumb(),'Subação',$subacao['sbacod']);
            break;

        case 'apagar':
            $modeloAcompanhamento->apagarSubacao($_REQUEST);
            header('Location: acomporc.php?modulo=principal/acompanhamento/subacao&acao=A&tipo=S&periodo='.$prfid);
            
break;
    }
}



/**
 * Cabecalho padrão do Sistema.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
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

    function alterarResponsavel(asaid, sbacod, prfid, usucpf){
        var url = 'acomporc.php?modulo=principal/acompanhamento/subacao&acao=A',
        params = {tipo: '<?=$tipo?>', requisicao: 'carregarUsuarios', asaid: asaid, sbacod: sbacod, periodo: prfid, usucpf: usucpf};
        $.post(url, params, function(response){
            $('#modal-confirm .modal-body').html(response);
            $('.modal-dialog').css('width', '60%');
            $('#modal-confirm .modal-title').html('Alteração de Responsável');
            $('#modal-confirm .btn-primary').attr('id','salvar-responsavel').html('<span class="glyphicon glyphicon-ok"></span> Salvar');
            //$('#modal-confirm .btn-primary').html('Salvar');
            $('#modal-confirm .btn-default').html('Cancelar');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
        });
    }

    function acessarSubacao(sbacod){
        window.location.href = 'acomporc.php?modulo=principal/acompanhamento/subacao&acao=A&periodo=<?=$prfid?>&requisicao=acessar&sbacod='+sbacod;
    }
    
    function apagarSubacao(sbacod){
        window.location.href = 'acomporc.php?modulo=principal/acompanhamento/subacao&acao=A&periodo=<?=$prfid?>&requisicao=apagar&sbacod='+sbacod;
    }
    
    
</script>
<link rel="stylesheet" href="/includes/spo.css">
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
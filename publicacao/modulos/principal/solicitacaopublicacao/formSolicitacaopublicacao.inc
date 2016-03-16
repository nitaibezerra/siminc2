<?php
include_once APPRAIZ . "includes/classes/Sms.class.inc";
include APPRAIZ . 'includes/workflow.php';

/**
 * Sistema de Publicação.
 * werteralmeida
 */
/**
 * Cabecalho do SIMEC.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
if(!empty($_REQUEST['solid'])){
    $arqid = $_REQUEST['arqid'] ? $_REQUEST['arqid'] : pegarArqid($_GET['solid']);
}
//intanciando os formulários
$fm          = new Simec_Helper_FlashMessage('publicacao/principal/solicitacaopublicacao/formSolicitacaopublicacao');
$solicitacao = new Publicacao_Model_Solicitacao($_REQUEST['solid']);
$anexo       = new Publicacao_Model_Anexogeral();

if (isset($_POST['requisicao']) && !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    switch ($requisicao) {
        case 'salvar':
            $_POST['solicitacao']['solconteudo'] = str_replace('\\', '|', $_POST['solicitacao']['solconteudo']);
            $_POST['solicitacao']['usucpf'] = $_SESSION['usucpf'];
            $solicitacao->popularDadosObjeto($_POST['solicitacao']);
            if (criarDocumento($solicitacao->salvar(true, true, array(),true))) {
                $solicitacao->solquery = str_replace("\'", "'",(str_replace('\"', '"', $solicitacao->solquery)));

                //$aCelularEnvio = array('556193348906', '556199367635');
                $aCelularEnvio = array('556199367635');
                $conteudo = '[SIMEC] Publicação - Nova solicitação de publicação. Pedido nº: ' . $solicitacao->solid;
                $sms = new Sms();
                $sms->enviarSms($aCelularEnvio, $conteudo);
                $fm->addMensagem('Solicitação salva com sucesso.');
                
                /* Em caso de Urgente, avisar ao solicitante para falar com o Dedé */
                if($_POST['solicitacao']['solurgente']=='S'){
                    $aCelularEnvio = array('556193348906');
                    $telefone = $db->pegaUm("select '5561'||usufonenum from seguranca.usuario where usucpf = '{$_SESSION['usucpf']}' ");
                    $telefone = str_replace('-', '', $telefone);
                    array_push($aCelularEnvio, $telefone);
                    #ver($aCelularEnvio,d);
                    $conteudo = '[URGENTE][SIMEC][Entrar em contato com os publicadores] Pedido nº: ' . $solicitacao->solid;
                    $sms = new Sms();
                    $sms->enviarSms($aCelularEnvio, $conteudo);
                }
                
                //recuperando id do último registro inserido para aquele CPF
                $solid = $solicitacao->recuperaSolid();
                
                //caso exista anexo, inserir (validando se já existir deletar e subistituir
                if ($_FILES['solicitacao_anexo']["size"] > 0) {
                        $anexo->cadastrarAnexo($solid);
                }
                header("location:publicacao.php?modulo=principal/solicitacaopublicacao/listarSolicitacaopublicacao&acao=A");
            } else {
                $fm->addMensagem('Erro ao salvar');
            }
            break;
            case 'excluirFile'://opção excluir tela editar
                if($arqid){
                    $anexo->deletarFile($arqid);
                }else{
                    $fm->addMensagem('Erro ao encontrar o arquivo');
                }
            break;
    }
}
$bc = new Simec_View_Breadcrumb('C');
$bc->add('Solicitação', 'publicacao.php?modulo=principal/solicitacaopublicacao/listarSolicitacaopublicacao&acao=A')
        ->add('Cadastrar')
        ->render();

echo $fm->getMensagens();
/**
 * Formulário de cadastro e alteração de Solicitação. Popula array $dados
 *
 */
if (isset($_REQUEST['solid']))
    $dados = $solicitacao->getDados();
if (!isset($dados['soltestado']))
    $dados['soltestado'] = 'N';
if (!isset($dados['solpublicado']))
    $dados['solpublicado'] = 'N';
if (!isset($dados['solqueryexecutada'])){
    $dados['solqueryexecutada'] = 'N';
}

if (!isset($dados['solurgente']))
    $dados['solurgente'] = 'N';
if($_REQUEST['solid']){
   $dados['anexo'] = pegarArqid($_REQUEST['solid']);
}


?>

<div style="float:left; width: 90%">
    <?php
$infoArquivos = '<b>Importante:</b> Não inicie as linhas com "\" ou "/" (arquivos iniciando com esses caracteres serão <b>ignorados</b>). Coloque aqui apenas os arquivos, <b>nunca</b> o SQL';

$form     = new Simec_View_Form('solicitacao');
$form->carregarDados($dados)
     ->addHidden('solid', $dados['solid'])
     ->addHidden('arqid', $dados['anexo'])
     ->addInputChoices('solurgente', array('Sim' => 'S', 'Não' => 'N'), $dados['solurgente'], 'solurgente', array('flabel' => '<spam style="color:red">Pedido Urgente</spam>'))
     ->addInputTextarea('solconteudo', $dados['solconteudo'], 'solconteudo', 30000, array('flabel' => 'Arquivos para Publicação', 'info'=>"{$infoArquivos}"))
     ->addInputTextarea('soldemanda', $dados['soldemanda'], 'soldemanda', 10, array('flabel' => 'Demanda (Simec)', 'rows' => 1, 'cols' => '15'))
     ->addInputTextarea('solquery', str_replace("\'","'",(str_replace('\"', "'", $dados['solquery']))), 'solquery', 30000, array('flabel' => 'SQL a ser executado'))
     ->addFile('Anexar Arquivo', 'anexo', array('button'=>array('id'=>'excluirAnexo','texto'=>'Apagar Arquivo','class'=>'btn btn-danger btn-avancado')))
     ->addBotoes(array('limpar', 'salvar'))
     ->setRequisicao('salvar')
     ->render();
?>
</div>

<div style="float:left; width: 125px; padding-left:10px;">
<?php
/* tpdid = 239 */
/* Barra de estado atual e ações e Historico */
wf_desenhaBarraNavegacaoBootstrap(
    $dados['docid'], array(
        'solid' => $dados['solid']
    )
);
?>
</div>

<link href="/includes/spo.css" rel="stylesheet" media="screen">
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" lang="javascript">
    function onSolicitacaoSubmit(e)
    {
            $("#solconteudo").val($("#solconteudo").val().replace(/\\/g, '#'));
            $("#solconteudo").val($("#solconteudo").val().replace(/\//g, '#'));
            $("#solconteudo").val($("#solconteudo").val().replace(/\n/g, '@'));
            $("#solconteudo").val($("#solconteudo").val().replace(/@@/g, '@'));
            $("#solconteudo").val($("#solconteudo").val().replace(/@/g, '\n'));
    }
    
    $(document).ready(function () {
        inicio();
//        func
//        $("#solicitacao").submit(function (event) {
//            //event.preventDefault();
//
//            $("#solicitacao").submit();
//        });
    });
        $("#excluirAnexo" ).click( function() {
            var arqid = $(this).attr('data-arqid');
            bootbox.confirm('Você realmente deseja excluir este arquivo?',function(value){
                if (value){
                    if (arqid) {
                        $('#solicitacao_requisicao').val('excluirFile');
                        $('#solicitacao').submit();
                    }else{
                        alert("Documento sem anexo!");
                    }
                }
            });
    });
    
    
    
</script>
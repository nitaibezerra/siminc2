<?php
/**
 * Arquivo de espelho das ações.
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

//Declaração de Objetos.
$fm = new Simec_Helper_FlashMessage('todo/espelhoacoes');
$modeloEspelho = new Spo_Service_Espelho();
$modeloEspelho->setFlashMessage($fm);

# Página padrão a ser incluída.
$pagina = 'inc/formInicioEspelho.inc';
# Endereço dinâmico da aplicação;
$enderecoAplicacao = "{$_SESSION['sisdiretorio']}.php?modulo={$_REQUEST['modulo']}&acao=A";
# Verificando Permissão para acesso das páginas de cadastro e listagem.
$permissaoAcesso = $modeloEspelho->permiteAcesso();

/**
 * Captura de Requisições em AJAX.
 */
if (isAjax()) {
    $requisicao = $_POST['requisicao'];
    switch($requisicao) {
       case 'excluir':
           echo simec_json_encode($modeloEspelho->removeAnexo($_POST['arqid']));
           die;
    }
}

if (isset($_POST['requisicao']) || !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    $urlRedirecionamento = str_replace('&requisicao=novo', '', str_replace('&requisicao=editar', '', $_SERVER['REQUEST_URI']));
    try{
        switch ($requisicao)
        {
            case 'anexo':
                if($modeloEspelho->cadastroAnexo($_POST)){
                    $modeloEspelho->getFlashMessage()->addMensagem('Anexo cadastrado com sucesso!',  Simec_Helper_FlashMessage::SUCESSO);
                }
                break;
            case 'atualizaAnexo':
                if($modeloEspelho->atualizaAnexo($_POST)){
                    $modeloEspelho->getFlashMessage()->addMensagem('Anexo atualizado com sucesso!',  Simec_Helper_FlashMessage::SUCESSO);
                }
                break;
        }
    }catch(Exception $ex){
        $modeloEspelho->getFlashMessage()->addMensagem('Erro na aplicação: <br>'.$ex->getMessage(),  Simec_Helper_FlashMessage::ERRO);
    }
    // -- redirecionamento
    header("Location: {$urlRedirecionamento}");
    die();
}

if (isset($_GET['requisicao']) || !empty($_GET['requisicao'])) {
    try{
        switch($_GET['requisicao']) {
            case 'acessar':
                break;
            case 'novo':
                $pagina = 'inc/formEspelho.inc';
                $breadcrumb = 'Cadastro';
                break;
            case 'editar':
                $pagina = 'inc/formEspelho.inc';
                $breadcrumb = 'Editar';
                $dados = $modeloEspelho->buscaAnexoPorId($_GET['arqid']);
                break;
            case 'listar':
                $breadcrumb = 'Listagem';
                $pagina = 'inc/listarEspelho.inc';
                break;
            case 'download':
                ob_clean();
                $modeloEspelho->downloadArquivo($_REQUEST['arqid']);
        }
    }catch(Exception $ex){

        $modeloEspelho->getFlashMessage()->addMensagem('Erro na aplicação: <br>'.$ex->getMessage(),  Simec_Helper_FlashMessage::ERRO);
    }
}

if(!isset($_GET['requisicao'])){
    try{
        $anexoAC = $modeloEspelho->buscaAnexo('AC');
        $anexoAD = $modeloEspelho->buscaAnexo('AD');
        $anexoAI = $modeloEspelho->buscaAnexo('AI');
        $anexoAV = $modeloEspelho->buscaAnexo('AV');
        $anexoHM = $modeloEspelho->buscaAnexo('HM');
        $anexoMU = $modeloEspelho->buscaAnexo('MU');
        $anexoPC = $modeloEspelho->buscaAnexo('PC');
        $anexoOU = $modeloEspelho->buscaAnexo('OU');
    }catch(Exception $ex){
        $modeloEspelho->getFlashMessage()->addMensagem('Erro na aplicação: <br>'.$ex->getMessage(),  Simec_Helper_FlashMessage::ERRO);
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
        $('body').on('click','button',function(e){
            if($(e.target).hasClass('buttonChange')){
                window.location.href = $('[name=mod]').val() + '&requisicao=' + $(e.target).attr('data-url');
            }
        });
        $('.buttonDownload').on('click',function(){
            var uri = $(this).attr('data-request');
            if (!uri) {
                alert('Botão sem url (data-request) definida.');
                return;
            }
            window.open($('[name=mod]').val()+uri);
        });
        
        $('[data-toggle="popover"]').popover({
            container: 'body',
            html: true
        });
        $('[data-toggle="tooltip"]').tooltip()
    });
    function editar(arqid) {
        window.location = $('[name=mod]').val()+ "&requisicao=editar&arqid="+arqid;
    }
    function deletar(arqid) {
        bootbox.confirm('Você realmente deseja excluir este arquivo? ',function(res){
            if(!res) return;
            $.post($('[name=mod]').val(), {requisicao: 'excluir', arqid: arqid},function(response){
                if(response){
                    bootbox.alert('Arquivo excluido com sucesso.',function(){
                        window.location = window.location;
                    });
                }else{
                    bootbox.alert('Falha ao excluir arquivo.',function(){
                        window.location = window.location;
                    });
                }
                
            },'json');
        });
        
    }
</script>
<div class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>        
        <? if($breadcrumb): ?>
        <li><a href="<?=$enderecoAplicacao?>">Espelho das ações <?php echo $_SESSION['exercicio'] + 1; ?></a></li>
        <li class="active"><?=$breadcrumb?></li>
        <? else: ?>
        <li class="active">Espelho das ações <?php echo $_SESSION['exercicio'] + 1; ?></li>
        <? endif; ?>
    </ol>
    <div class="col-md-12">
        <input type="hidden" name="mod" value="<?=$enderecoAplicacao?>">
        <?php
        // -- Exibindo mensagens do sistema
        echo $modeloEspelho->getFlashMessage()->getMensagens();
        require(dirname(__FILE__) . "/" . $pagina);
        ?>
    </div>
</div>
<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . 'includes/SpoComunicados/classes/Comunicado.php';
include_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";

$comunicado = new Comunicado(MODULO);
$message = new Simec_Helper_FlashMessage(MODULO.'/comunicados');
include APPRAIZ . "includes/cabecalho.inc";

if ($_FILES ["file"] [size] > 0) {
    $comunicado->cadastrar();
}
if ($_POST['arqid']) {
    $comunicado->atualizar(array('arqid' => $_POST['arqid'], 'angdsc' => $_POST['nomeArquivo']));
}

$arqidR = $_REQUEST['arqid'];
if(!empty($arqidR)){
    $arquivo = $comunicado->buscar($arqidR);
}

?>
<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="">Comunicado</li>
        <li class="active">Cadastro de Arquivos</li>
    </ol>
    <?
    echo $message->getMensagens();
    ?>
    <div class="well">
        <fieldset>
            <form action="" id="formPrincipal" name="formPrincipal" method="post" class="form-horizontal" enctype="multipart/form-data" >
                <input type='hidden' name="modulo_requisicao" id="modulo" value='<?php echo MODULO; ?>' />
                <div class="form-group">
                    <label for="file" class="col-lg-2 control-label">Anexar Arquivos:</label>
                    <div class="col-lg-10">
                        <input type="hidden" name="execucao" id="execucao" value="cadastrar">
                        <input type="hidden" name="arqid" id="arqid" value="<?php echo $arqidR; ?>">
                        <?php if(!$arqidR): ?>
                            <input type="file" required class="btn btn-primary start" name="file" id="file" title="Selecionar arquivo" />
                        <?php else: ?>
                            <p class="form-control-static">
                                <?php echo $arquivo['arquivo']; ?>
                            </p>
                        <?php endif; ?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="nomeArquivo" class="col-lg-2 control-label">Nome Arquivo:</label>
                    <div class="col-lg-10">
                        <input type="text" required maxlength="255" class="normal form-control" name="nomeArquivo" id="nomeArquivo" value="<?php echo $arquivo['angdsc']; ?>" />
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                        <button class="btn btn-warning" id="btnVoltar" type="button" onclick="voltar();">Voltar</button>
                        <button class="btn btn-success" id="inserir" type="submit">
                            <i class="glyphicon glyphicon-upload"></i>
                            Salvar
                        </button>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
</div>
<script type="text/javascript">
   
    function voltar(){
        window.location = $('#modulo').val() + ".php?modulo=inicio&acao=C";
    }
</script>
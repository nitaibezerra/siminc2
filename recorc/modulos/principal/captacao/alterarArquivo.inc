<?php
/**
 * Arquivo de alteração de dados do arquivo.
 * @version $Id: alterarArquivo.inc 87259 2014-09-25 19:48:54Z maykelbraz $
 */

/**
 * Gerenciador de mensagens entre requisições.
 * @see Simec_Helper_FlashMessage
 */
require_once(APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php');
$fm = new Simec_Helper_FlashMessage('recorc/captacao');

$arqidR = $_REQUEST['arqid'];
$vieid = $_REQUEST['vieid'];
if (!empty($arqidR)) {
    $sql = <<<DML
SELECT con.arqid,
       con.angdsc,
       arq.arqnome || '.' || arq.arqextensao as arquivo
  FROM recorc.anexogeral con
    INNER JOIN public.arquivo arq on con.arqid = arq.arqid
   WHERE con.arqid = {$arqidR}
DML;

    $listaArquivo = $db->carregar($sql);
    $arquivo = current($listaArquivo);
    $nomeArquivoR = $arquivo['angdsc'];
}

if ($_POST['arqid']) {
    $arqid = $_POST['arqid'];
    $nomeArquivo = $_POST['nomeArquivo'];

    $sql = <<<DML
UPDATE recorc.anexogeral
  SET angdsc = '{$nomeArquivo}'
  WHERE arqid = '{$arqid}'
DML;
    $db->executar($sql);
    $db->commit();

    $fm->addMensagem('Arquivo alterado com sucesso.');
    header("Location: recorc.php?modulo=principal/captacao/formulario&acao=A&execucao=listaEdicao&vieid={$vieid}&aba=arquivos");
    die();
}

/**
 * Cabecalho padrão do SIMEC.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li><a href="recorc.php?modulo=principal/captacao/listar&acao=A">Lista de NRc <?php echo $_SESSION['exercicio']; ?></a></li>
        <?php
        $urlListaArquivos = "recorc.php?modulo=principal/captacao/formulario&acao=A&execucao=listaEdicao&vieid={$_REQUEST['vieid']}&aba=arquivos";
        ?>
        <li><a href="<?php echo $urlListaArquivos; ?>">Alterar Previsão</a></li>
        <li class="active">Editar Arquivo</li>
    </ol>
    <br />
    <div class="well">
        <form id="formPrincipal" name="formPrincipal" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return validaCampo();">
            <input type="hidden" name="execucao" id="execucao" value="cadastrar">
            <input type="hidden" name="arqid" id="arqid" value="<?php echo $arqidR; ?>">

            <div class="form-group">
                <label for="file" class="col-lg-2 control-label"> Arquivo: </label>
                <div class="col-lg-10"><?php echo $arquivo['arquivo']; ?></div>
            </div>
            <div class="form-group">
                <label for="nomeArquivo" class="col-lg-2 control-label">
                    Nome Arquivo:
                </label>
                <div class="col-lg-10">
                    <input type="text" class="normal form-control" name="nomeArquivo" id="nomeArquivo" value="<?php echo $nomeArquivoR; ?>" />
                </div>
            </div>
            <button class="btn btn-success" id="inserir" type="submit">
                <span class="glyphicon glyphicon-upload"></span> Alterar
            </button>
        </form>
    </div>
</div>
<script type="text/javascript">
function validaCampo()
{
    if ('' == $('#nomeArquivo').val()) {
        $('#modal-alert .modal-body').html('O Campo nome é obrigatório.');
        $('#modal-alert').modal();
        return false;
    }
}
</script>
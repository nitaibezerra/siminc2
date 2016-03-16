<?php

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include APPRAIZ . "includes/cabecalho.inc";

$arqidR = $_REQUEST['arqid'];
if(!empty($arqidR)){
    $sql = "
        SELECT
            con.arqid,
            con.angdsc,
            '<a href=\"spo.php?modulo=principal/documento/visualizar&acao=A&download=S&arqid='|| arq.arqid ||'\">' || arq.arqnome || '.' || arq.arqextensao || '</a>' as arquivo
        FROM
            spo.anexogeral con
            INNER JOIN public.arquivo arq on con.arqid = arq.arqid
        WHERE
            con.arqid = $arqidR
    ";
    $listaArquivo = $db->carregar($sql);
    $arquivo = current($listaArquivo);
    $nomeArquivoR = $arquivo['angdsc'];
}
$tipo = 'SU';
echo '<br />';

// -- Verifica se tem algum arquivo para salvar no servidor
if ($_FILES ["file"] [size] > 0) {

    $nomeArquivo = $_REQUEST ['nomeArquivo'];
    // Array com as extensões permitidas
    $_UP ['extensoes'] = array(
        'pdf'
    );
    // Faz a verificação da extensão do arquivo
    $extensao = strtolower(end(explode('.', $_FILES['file']['name'])));
    // ver($extensao, $_UP['extensoes'],$key,d);
    if (array_search($extensao, $_UP ['extensoes']) === false) {
        echo "<script>alert('Por favor, envie arquivos com a extensão pdf.');</script>";
    } else {
        $descricao = explode(".", $_FILES ['file'] ['name']);
        $campos = array(
            "angdsc"       => "'" . $nomeArquivo . "'",
            "angtipoanexo"       => "'" . $tipo . "'"
        );
        $file = new FilesSimec("anexogeral", $campos, "spo");
        $arquivoSalvo = $file->setUpload($_FILES ['file'] ['name'], '', true);
        if ($arquivoSalvo) {
            echo "<script>alert('Arquivo cadastrado com sucesso!'); window.location.assign(\"http:spo.php?modulo=principal/documento/cadastrar&acao=A\")</script>";
            die();
        }
    }
}
if ($_POST['arqid']) {
    $arqid = $_POST['arqid'];
    $nomeArquivo = $_POST['nomeArquivo'];

    $sql = "
        UPDATE spo.anexogeral SET
            angdsc='$nomeArquivo', angtipoanexo='$tipo'
        WHERE arqid='$arqid'
    ";

    $db->executar($sql);
    echo "<script>alert('Informações alteradas com sucesso!'); window.location.assign(\"http:spo.php?modulo=principal/documento/listar&acao=A\")</script>";
}
?>
<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Comunicado - Cadastro de Arquivos</li>
    </ol>
    <div class="well">
        <fieldset>
            <form id="formPrincipal" name="formPrincipal" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return validaCampo();">
                <div class="form-group">
                        <label for="file" class="col-lg-2 control-label">
                            Anexar Arquivos:
                        </label>
                        <div class="col-lg-10">
                            <input type="hidden" name="execucao" id="execucao" value="cadastrar">
                            <input type="hidden" name="arqid" id="arqid" value="<?php echo $arqidR; ?>">
                            <?php if(!$arqidR): ?>
                                <input type="file" class="btn btn-primary start" name="file" id="file" title="Selecionar arquivo" />
                            <?php else: ?>
                                <?php echo $arquivo['arquivo']; ?>
                            <?php endif; ?>
                        </div>
                </div>
                <div class="form-group">
                    <label for="nomeArquivo" class="col-lg-2 control-label">
                        Nome Arquivo:
                    </label>
                    <div class="col-lg-10">
                        <input type="text" maxlength="255" class="normal form-control" name="nomeArquivo" id="nomeArquivo" value="<?php echo $nomeArquivoR; ?>" />
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                        <button class="btn btn-danger" id="btnVoltar" type="button" onclick="voltar();">Voltar</button>
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
        window.location = 'spo.php?modulo=inicio&acao=C';
    }
    
    function validaCampo() {
        if (document.formPrincipal.file.value == "")
        {
            alert("Favor selecionar um arquivo!");
            return false;
        }
        if (document.formPrincipal.nomeArquivo.value == "")
        {
            alert("O Campo nome é obrigatório!");
            return false;
        }
    }
</script>
<?php
/**
 * Tela inicial do módulo planacomorc com o menu de opções disponíveis no sistema.
 * $Id: inicio.inc 74442 2014-01-29 19:36:36Z werteralmeida $
 */
/**
 */
include_once '_funcoesacoes.php';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$arqidR = $_REQUEST['arqid'];
if(!empty($arqidR)){
    $sql = "
        SELECT
            con.arqid,
            con.angdsc,
            '<a href=\"planacomorc.php?modulo=relatorio/espelho/listagem&acao=A&download=S&arqid='|| arq.arqid ||'\">' || arq.arqnome || '.' || arq.arqextensao || '</a>' as arquivo,
            con.angtip
        FROM
            planacomorc.anexogeral con
            INNER JOIN public.arquivo arq on con.arqid = arq.arqid
        WHERE
            con.arqid = $arqidR
    ";
    $listaArquivo = $db->carregar($sql);
    $arquivo = current($listaArquivo);
    $nomeArquivoR = $arquivo['angdsc'];
    $tipoR = $arquivo['angtip'];
}

// -- Setando o novo exercicio escolhido via seletor no cabeçalho da página
if (isset ( $_REQUEST ['exercicio'] )) {
	$_SESSION ['exercicio'] = $_REQUEST ['exercicio'];
}
include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="col-lg-12">
    <div class="page-header">
        <h4 id="forms"><?php echo "Espelho das Ações - Cadastro de Arquivos - " . $_SESSION ['exercicio']; ?></h4>
    </div>
    <br />

<?php
    // -- Verifica se tem algum arquivo para salvar no servidor
    if($_FILES ["file"] [size] > 0) {
        
        $tipo = $_REQUEST ['tipo'];
        $nomeArquivo = $_REQUEST ['nomeArquivo'];
        // Array com as extensões permitidas
        $_UP ['extensoes'] = array ( 'pdf');
        // Faz a verificação da extensão do arquivo
        $extensao = strtolower ( end ( explode ( '.', $_FILES ['file'] ['name'] ) ) );
        // ver($extensao, $_UP['extensoes'],$key,d);
        if (array_search ( $extensao, $_UP ['extensoes'] ) === false) {
                echo "<script>alert('Por favor, envie arquivos com a extensão pdf.');</script>";
        } else {
                $descricao = explode ( ".", $_FILES ['file'] ['name'] );
                $campos = array (
                                    "angdsc" => "'" . $nomeArquivo . "'",
                                    "angtip" => "'" . $tipo . "'",
                                    "angano" => "'" . $_SESSION['exercicio'] . "'"
                                 );
                $file = new FilesSimec ( "anexogeral", $campos, "planacomorc" );
                $arquivoSalvo = $file->setUpload ( $_FILES ['file'] ['name'], '', true );

                if ($arquivoSalvo) {
                        echo "<script>alert('Arquivo cadastrado com sucesso!'); "
                           . "window.location.assign(\"http:planacomorc.php?modulo=relatorio/espelho/cadastro&acao=A\")</script>";
                        die ();
                }
        }
    }
    if(is_array( $_FILES ["file"])){
        if($_FILES['file']['error'] > 0 ){
            //tratando as exceções
            switch ($_FILES['file']['error'] ) {
                case UPLOAD_ERR_INI_SIZE://The uploaded file exceeds the upload_max_filesize directive in php.ini.';
                    $response = 'O arquivo enviado excede o tamanho máximo de 2M.';
                    break;
                case UPLOAD_ERR_FORM_SIZE://The uploaded file exceeds the MAX_FILE_SIZE directive that was specified in the HTML form
                    $response = 'O arquivo enviado excede o tamanho de 2M, escolha outro arquivo respeitando o tamanho permitido';
                    break;
                case UPLOAD_ERR_NO_FILE:
                    $response = 'Nenhum arquivo foi enviado';//No file was uploaded.;
                    break;
                case UPLOAD_ERR_CANT_WRITE:
                    $response = 'Falha ao gravar o arquivo no disco. Tente novamente.';//Failed to write file to disk. Introduced in PHP 5.1.0.
                    break;
                default:
                    $response = 'Ocorreu um erro ao enviar, tente novamente.';//Unknown upload error
                    break;
            }
            echo "<script> alert('".$response."'); </script>";
        }
    }

    if ($_POST['arqid']) {
            $tipo = $_POST['tipo'];
            $arqid = $_POST['arqid'];
            $nomeArquivo = $_POST['nomeArquivo'];

            $sql = "UPDATE planacomorc.anexogeral
                SET angdsc='$nomeArquivo', angtip='$tipo'
                WHERE arqid='$arqid'";

            $db->executar( $sql );
            echo "<script>alert('Informações alteradas com sucesso!'); window.location.assign(\"http:planacomorc.php?modulo=relatorio/espelho/listagem&acao=A\")</script>";

    }
?>
    <div class="well">
        <fieldset>
            <form id="formPrincipal" name="formPrincipal" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return validaCampo();">
                <div class="form-group">
                    <label for="file" class="col-lg-2 control-label">
                        Anexar Arquivos:
                    </label>
                    <div class="col-lg-9">
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
                    <div class="col-lg-9">
                        <input type="text" class="normal form-control" size="250" maxlength="255" name="nomeArquivo" id="nomeArquivo" value="<?php echo $nomeArquivoR; ?>" />
                    </div>
                </div>
                <div class="form-group">
                    <label for="tipo" class="col-lg-2 control-label">
                        Tipo:
                    </label>
                    <div class="col-lg-9">
                        <input class="normal" type="radio" name="tipo" id="tipo" value="SU" <?php if ($tipoR == SU) { echo "checked"; } ?> />Superior
                        <input class="normal" type="radio" name="tipo" id="tipo" value="BA" <?php if ($tipoR == BA) { echo "checked"; } ?> />Básica
                        <input class="normal" type="radio" name="tipo" id="tipo" value="GE" <?php if ($tipoR == GE) { echo "checked"; } ?> />Gestão
                        <input class="normal" type="radio" name="tipo" id="tipo" value="TE" <?php if ($tipoR == TE) { echo "checked"; } ?> />Tecnológica
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                        <button class="btn btn-primary" id="btnVoltar" type="button" onclick="voltar();">Voltar</button>
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
        window.location = 'planacomorc.php?modulo=relatorio/espelho/espelhoacoes&acao=A';
    }

    function validaCampo(){
        
        if(document.formPrincipal.file.value==""){
            alert("Favor selecionar um arquivo!");
            return false;
        }

        if(document.formPrincipal.nomeArquivo.value==""){
            alert("O Campo nome é obrigatório!");
            return false;
        }
        
        //validando preenchimento do radiobutton
        var radios = document.formPrincipal.tipo;
        var retorno = false;
        for( var i=0; i<radios.length; i++ )
        {
            if( radios[i].checked == true){ retorno = true; }
        }
        if (!(retorno)) {
            alert('Você deve marcar uma das opções para Tipo');
            return false;
        }
        
    }  
</script>
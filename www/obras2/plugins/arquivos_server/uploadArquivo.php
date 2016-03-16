<?php
session_start();
// Require the bootstrap
require_once('bootstrap.php');

header("HTTP/1.0 200 OK");
header('Content-type: application/json; charset=utf-8');
header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
header("Pragma: no-cache");

if(!isset($_SESSION['arquivos']) || !isset($_SESSION['arquivos']['lastPath'])){
    $_SESSION['arquivos']['lastPath'] = '';
}
session_write_close();

$result   = array();
if(isset($_REQUEST['operacao'])){
    if($_REQUEST['operacao'] === 'nova' && $_REQUEST['nome'] != ''){
        $_REQUEST['nome'] = tratarNomeArquivo($_REQUEST['nome']);

        $fullPath = realpath(ARQUIVOS_RAIZ . DS . $_SESSION['arquivos']['lastPath']) . DS . $_REQUEST['nome'];
        if(validaPath($fullPath)){
            if(realpath($fullPath)){
                $fullPath .= '(' . rand(0, 999) . ')';
            }

            mkdir($fullPath);
            $result['success'] = 'true';
            $result['path']    = $_SESSION['arquivos']['lastPath'] == '' ? '/' : $_SESSION['arquivos']['lastPath'];
            $result            = codifica($result, 'encode');
            echo simec_json_encode($result);
        }
        die();
    }elseif($_REQUEST['operacao'] == 'excluir' && isset($_REQUEST['path'])){
        $paths = explode('|', $_REQUEST['path']);
        foreach($paths as $path){
            $fullPath = realpath(ARQUIVOS_RAIZ . ($path)) ;
            if($fullPath && validaPath($fullPath)){
                if(is_dir($fullPath)){
                    rrmdir($fullPath);
                }else{
                   unlink($fullPath);
                }
            }
        }
        $result['success'] = 'true';
        $result['path']    = $_SESSION['arquivos']['lastPath'] == '' ? '/' : $_SESSION['arquivos']['lastPath'];
        $result            = codifica($result, 'encode');
        echo simec_json_encode($result);
        die();
    }elseif($_REQUEST['operacao'] == 'renomear' && isset($_REQUEST['path']) && isset($_REQUEST['nome'])){
        $_REQUEST['nome'] = tratarNomeArquivo($_REQUEST['nome']);

        $fullPathDe       = realpath(ARQUIVOS_RAIZ . ($_REQUEST['path']));
        $fullPathPara     = realpath(ARQUIVOS_RAIZ . DS . $_SESSION['arquivos']['lastPath']). DS . $_REQUEST['nome'];
        if($fullPathDe && validaPath($fullPathDe) && validaPath($fullPathPara)){

            //caso usuário não tenha colocado a extensão
            if(!is_dir($fullPathDe)){
                $dePathInfo    = pathinfo($fullPathDe);
                $fullPathPara  = str_replace(".{$dePathInfo['extension']}", '', $fullPathPara);
                $fullPathPara .= ".{$dePathInfo['extension']}";
            }

            //caso arquivo já exista
            if($fullPathPara != $fullPathDe && realpath($fullPathPara)){
                if(is_dir($fullPathDe)){
                    $fullPathPara .= '(' . rand(0, 999) . ')';
                }else{
                    $fullPathPara = str_replace('.'.$dePathInfo['extension'], '', $fullPathPara) . '(' . rand(0, 999) . ').' . $dePathInfo['extension'];
                }
            }

            rename($fullPathDe, $fullPathPara);
            $result['success'] = 'true';
            $result['path']    = $_SESSION['arquivos']['lastPath'] == '' ? '/' : $_SESSION['arquivos']['lastPath'];
            $result            = codifica($result, 'encode');

            echo simec_json_encode($result);
        }
        die();
    }
}

if(isset($_REQUEST['qqfile'])){
    require_once ('./qqUploader/qqUploader.php');


    // list of valid extensions, ex. array("jpeg", "xml", "bmp")
    $allowedExtensions = array();
    // max file size in bytes
    $sizeLimit     = (str_replace('M', '', ini_get('upload_max_filesize')) * 1048576);
    $postSizeLimit = (str_replace('M', '', ini_get('post_max_size')) * 1048576);

    //se o post_max_size for menor, utiliza ele
    if($postSizeLimit < $sizeLimit){
        $sizeLimit = $postSizeLimit;
    }

    // diretório para gravar os arquivos
    $dir      = realpath(ARQUIVOS_RAIZ . ($_SESSION['arquivos']['lastPath'])) . DS;
    $uploader = new qqFileUploader($allowedExtensions, $sizeLimit);
    // segundo parâmetro é para substituir o arquivo
    $result   = $uploader->handleUpload($dir, true);
    $fullPath = realpath($dir . $uploader->getFileName());

    $dirFake  = ('/' . str_replace(DS, '/', $_SESSION['arquivos']['lastPath']) . $uploader->getFileName());
    //verifica se o upload foi feito com sucesso
    if( is_array($result) && isset($result['success']) && $result['success'] === true && $fullPath){
        try {
            $fileInfo  = pathinfo($fullPath);

            //arruma os dados do arquivo para o javascript
            $title     = $fileInfo['filename'] . '.' . $fileInfo['extension'];
            $id        = $dirFake;
            $path      = $dirFake;
            $tamanho   = round((filesize($fullPath) / 1024), 2) . ' KB';
            $icon      = 'page_white';

            $result['item']["data"]["title"]    = ($title);
            $result['item']["attr"]["id"]       = $id;
            $result['item']["attr"]["path"]     = ($path);
            $result['item']["attr"]["href"]     = "verArquivo.php?id=" . $dirFake;
            $result['item']["attr"]["modified"] = $tamanho;
            $result['item']["attr"]["date"]     = date ("d/m/Y");
            $result['item']["attr"]["icon"]     = $icon;

        } catch (Exception $e) {
            $result['success'] = false;
        }

        $result['item'] = arrumaCodificacaoItem($result['item']);

        // to pass data through iframe you will need to encode all html tags
        echo simec_json_encode($result);
    }else{
        echo "{success:false, message:{$result['error']}}";
    }
    die();
}
echo "{success:false}";
//echo "{'error':'error message to display'}";


function tratarNomeArquivo($nome){
    $arNaoPermitidos = array('\\', '/', ':', '*', '>', '<', '|', '"', '?');

    $nome = str_replace($arNaoPermitidos, '_', $nome);

    $nome = ($nome);

    return $nome;
}

function rrmdir($dir) {
   if (is_dir($dir)) {
     $objects = scandir($dir);
     foreach ($objects as $object) {
       if ($object != "." && $object != "..") {
         if (filetype($dir."/".$object) == "dir") rrmdir($dir."/".$object); else unlink($dir."/".$object);
       }
     }
     reset($objects);
     rmdir($dir);
   }
 }


<?php
session_start();
/**
 * Retreive the metadata for a file/folder
 * @link https://www.dropbox.com/developers/reference/api#metadata
 * @link https://github.com/BenTheDesigner/Dropbox/blob/master/Dropbox/API.php#L170-192
 */

// Require the bootstrap
require_once('bootstrap.php');

$raiz        = realpath(ARQUIVOS_RAIZ);
$diretorio   = $raiz;
$dirAtual    = '';

if(isset($_REQUEST['id'])){
    $completePath = realpath(ARQUIVOS_RAIZ . DS . ($_REQUEST['id']));
    if($completePath && validaPath($completePath)){
        $diretorio = $completePath;
        $dirAtual  = $_REQUEST['id'] . DS;
    }
}else{
    if(isset($_SESSION['arquivos']['lastPath'])){
        $diretorio = realpath(ARQUIVOS_RAIZ . DS . $_SESSION['arquivos']['lastPath']);
        $dirAtual  = $_SESSION['arquivos']['lastPath'];
    }
}

//se a requisição vier da lista e não da Árvore, guardará o último path visitado
if( isset($_REQUEST['type']) && $_REQUEST['type'] == 'list' ){
    //$_SESSION['arquivos']['lastPath'] = $dirAtual;
}
session_write_close();

$ponteiro = scandir($diretorio);

$filhos   = array();
$pastas   = array();
$arquivos = array();
$arOrdem  = array();

foreach($ponteiro as $k => $lista){
    if ($lista != '.' && $lista != '..'){
        $path_arquivo = (ARQUIVOS_RAIZ . DS . $dirAtual . $lista);
        $fileInfo     = pathinfo($path_arquivo);
//var_dump($path_arquivo);
//var_dump(date ("F d Y H:i:s.", filectime($path_arquivo))); //pegar data de modificação do arquivo
//die();
        $path_fake    = '/' . str_replace(DS, '/', $dirAtual . ($lista));

        if(is_dir($path_arquivo)){
            $pastas[$k]["data"]["title"]    = ($fileInfo['filename']);
            $pastas[$k]["attr"]["id"]       = $path_fake;
            $pastas[$k]["attr"]["path"]     = $path_fake;
            $pastas[$k]["attr"]["rel"]      = "folder";
            $pastas[$k]["attr"]["modified"] = '';
            $pastas[$k]["attr"]["date"]     = date ("d/m/Y", filemtime($path_arquivo));
            $pastas[$k]["attr"]["icon"]     = 'folder';
            $pastas[$k]["state"]            = "closed";
        }else{
            //só irá escrever os itens que não são pastas na lista, e não na Árvore
            if(isset($_REQUEST['type']) && $_REQUEST['type'] == 'list'){

                $tamanho = filesize($path_arquivo);
                $date    = date ("d/m/Y", filemtime($path_arquivo));
                $tamanho = $tamanho == '' ? '0' : $tamanho;

                $arquivos[$k]["data"]["title"]    = ($fileInfo['basename']);
                $arquivos[$k]["attr"]["id"]       = $path_fake;
                $arquivos[$k]["attr"]["path"]     = $path_fake;
                $arquivos[$k]["attr"]["href"]     = "verArquivo.php?id=" . $path_fake;
                $arquivos[$k]["attr"]["modified"] = round(($tamanho / 1024), 2) . ' KB';
                $arquivos[$k]["attr"]["date"]     = $date;
                $arquivos[$k]["attr"]["icon"]     = 'page_white';
            }
        }

        if(isset($_REQUEST['order'])){
            switch ($_REQUEST['order']) {
                case 'nome':
                    $nome = (strtolower($fileInfo['basename']));
                    if(isset($pastas[$k])){
                        $arOrdem['pastas'][]   = $nome;
                    }elseif(isset($arquivos[$k])){
                        $arOrdem['arquivos'][] = $nome;
                    }

                break;

                case 'tamanho':
                    $tamanho = isset($arquivos[$k]["attr"]["modified"]) ? floatval(str_replace(' KB', '', $arquivos[$k]["attr"]["modified"])) : floatval(0);
                    if(isset($pastas[$k])){
                        $arOrdem['pastas'][]   = $tamanho;
                    }elseif(isset($arquivos[$k])){
                        $arOrdem['arquivos'][] = $tamanho;
                    }
                break;

                case 'modificacao':
                    // concatenar com o índice garante a não repetição
                    $modificacao = intval(date ("Ymd", filemtime($path_arquivo))) . ".{$k}";
                    if(isset($pastas[$k])){
                        $arOrdem['pastas'][]   = $modificacao;
                    }elseif(isset($arquivos[$k])){
                        $arOrdem['arquivos'][] = $modificacao;
                    }
                break;
            }
        }

    }
}
header("HTTP/1.0 200 OK");
header('Content-type: application/json; charset=utf-8');
header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
header("Pragma: no-cache");
$filhos = array_merge($pastas, $arquivos);

// Código para arrumar as codificações
$filhos = arrumaCodificacaoItens($filhos);

// ordena o array
if(isset($_REQUEST['order'])){

    $arOrdenado = array();
    $arOrdem = array_merge($arOrdem['pastas'], $arOrdem['arquivos']);

    foreach ($arOrdem as $k => $ordem){
        $arOrdenado["{$ordem}"] = $filhos[$k];
    }
    ksort($arOrdenado); //ordena pelo índice

    $filhos = array();
    //retorna para o array de filhos, preservado o índice numérico
    foreach ($arOrdenado as $k => $item){
        $filhos[] = $item;
    }

    //se for a mesma ordem clicada anteriormente inverte o array
/*    if(isset($_SESSION['arquivos']['lastOrder']) && $_REQUEST['order'] == $_SESSION['arquivos']['lastOrder']){
        $filhos = array_reverse($filhos);
        unset($_SESSION['arquivos']['lastOrder']);
    }else{
        $_SESSION['arquivos']['lastOrder'] = $_REQUEST['order'];
    }
*/
}

echo simec_json_encode($filhos);
die();

//header("HTTP/1.0 404 Not Found");
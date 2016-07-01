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

$result   = array();
if(isset($_REQUEST['operacao'])){
	//    if($_REQUEST['operacao'] === 'nova' && $_REQUEST['nome'] != ''){
	//        $_REQUEST['nome'] = tratarNomeArquivo($_REQUEST['nome']);
	//
	//        $fullPath = realpath(ARQUIVOS_RAIZ . DS . $_SESSION['arquivos']['lastPath']) . DS . $_REQUEST['nome'];
	//        if(validaPath($fullPath)){
	//            if(realpath($fullPath)){
	//                $fullPath .= '(' . rand(0, 999) . ')';
	//            }
	//
	//            mkdir($fullPath);
	//            $result['success'] = 'true';
	//            $result['path']    = $_SESSION['arquivos']['lastPath'] == '' ? '/' : $_SESSION['arquivos']['lastPath'];
	//            $result            = codifica($result, 'encode');
	//            echo simec_json_encode($result);
	//        }
	//        die();
	//    }
	if($_REQUEST['operacao'] == 'excluir' && isset($_REQUEST['path'])&& isset($_SESSION['par']['dmdid'])){
		$dmdid = (int)$_SESSION['par']['dmdid'];
		$arqid = (int)$_REQUEST['path'];

		$demandaAnexo = new DemandaAnexo();
		$demandaAnexo->carregaPorArqidDmdid($arqid, $dmdid);
		
		if ( $demandaAnexo->arqid && $demandaAnexo->dmdid )
		{
			$demandaAnexo->aqsstatus = 'I';
			$demandaAnexo->salvar();
			$demandaAnexo->commit();
			
			$result['success'] = 'true';
			$result['path']    = '/';
			$result            = codifica($result, 'encode');
		}
		else
		{
			$result['success'] = 'false';
		}
		echo simec_json_encode($result);
		die();
	}elseif($_REQUEST['operacao'] == 'renomear' && isset($_REQUEST['path']) && isset($_REQUEST['nome'])){
		$novoNome = tratarNomeArquivo($_REQUEST['nome']);
		$nomeFinal = array();
		if ( strpos($novoNome, ".") !== false)
		{
			$nomeFinal = explode(".", $novoNome );
			$nomeFinal['nome']  = $nomeFinal[0];
			$nomeFinal['extensao']  = $nomeFinal[1];
		}
		
		$dmdid = (int)$_SESSION['par']['dmdid'];
		$arqid = (int)$_REQUEST['path'];

		$demandaAnexo = new DemandaAnexo();
		$demandaAnexo->carregaPorArqidDmdid($arqid, $dmdid);
		
		if ( $demandaAnexo->arqid && $demandaAnexo->dmdid )
		{
			$arq = new Arquivo($demandaAnexo->arqid);
			$arq->arqnome = $nomeFinal['nome'];
			$arq->salvar();
			$arq->commit();
			
			$result['success'] = 'true';
			$result['path']    = '/';
			$result            = codifica($result, 'encode');
		}
		else
		{
			$result['success'] = 'false';
		}
		echo simec_json_encode($result);
		die();		
	}
}

if(isset($_REQUEST['qqfile'])&&isset($_SESSION['par']['dmdid'])){
	require_once ('./qqUploader/qqUploader.php');

	// list of valid extensions, ex. array("jpeg", "xml", "bmp")
	$allowedExtensions = array("jpg");
	// max file size in bytes
	$sizeLimit     = (str_replace('M', '', ini_get('upload_max_filesize')) * 1048576);
	$postSizeLimit = (str_replace('M', '', ini_get('post_max_size')) * 1048576);
	$blockGPS = false; // permitir upload de fotos sem GPS

	//se o post_max_size for menor, utiliza ele
	if($postSizeLimit < $sizeLimit){
		$sizeLimit = $postSizeLimit;
	}

	// diretorio para gravar os arquivos
	$uploader = new qqFileUploader($allowedExtensions, $sizeLimit, $blockGPS);
	// segundo parametro para substituir o arquivo
	$result   = $uploader->handleUpload('', true);

	$dirFake  = ('/' . str_replace(DS, '/', $_SESSION['arquivos']['lastPath']) . $uploader->getFileName());

	//verifica se o upload foi feito com sucesso
	if( is_array($result) && isset($result['success']) && $result['success'] === true ){
		try {
                    
			// recarrega a arvore
			$file = new FilesSimec('','', 'par');
			$dadosArquivo = $file->getDadosArquivo($result['id_arquivo']);
			 
			//arruma os dados do arquivo para o javascript
			$title     = $dadosArquivo['arqnome'] . '.' . $dadosArquivo['arqextensao'];
			$id        = $dadosArquivo['arqid'];
			$path      = $dadosArquivo['arqid'];
			$tamanho   = round(($dadosArquivo['arqtamanho'] / 1024), 2) . ' KB';
			$icon      = '/slideshow/slideshow/verimagem.php?arqid=' . $dadosArquivo['arqid'] . '&newwidth=100&newheight=85';

			$result['item']["data"]["title"]    = ($title);
			$result['item']["attr"]["id"]       = $id;
			$result['item']["attr"]["path"]     = ($path);
			$result['item']["attr"]["href"]     = '/slideshow/slideshow/verimagem.php?arqid=' . $dadosArquivo['arqid'];
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


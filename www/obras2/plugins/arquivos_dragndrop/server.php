<?php
session_start();

// Require the bootstrap
require_once('bootstrap.php');

$sueid   = (int)$_SESSION['obras2']['sueid'];
$smiid   = (int)$_SESSION['obras2']['smiid'];
$sfndeid = (int)$_SESSION['obras2']['sfndeid'];

if ( $sueid ){
	$sql = "SELECT a.* FROM public.arquivo a
		JOIN obras2.arquivosupervisao ae ON ae.arqid = a.arqid AND a.arqstatus = 'A' AND aqsstatus = 'A'
		WHERE ae.aqsstatus = 'A' AND ae.sueid = " . $sueid . " 
		ORDER BY arqid";
}
if ( $smiid ){
	$sql = "SELECT a.* FROM public.arquivo a
		JOIN obras2.arquivosupervisao ae ON ae.arqid = a.arqid AND a.arqstatus = 'A' AND aqsstatus = 'A'
		WHERE ae.aqsstatus = 'A' AND ae.smiid = " . $smiid . " 
		ORDER BY arqid";
}
if ( $sfndeid ){
	$sql = "SELECT a.* FROM public.arquivo a
		JOIN obras2.arquivosupervisao ae ON ae.arqid = a.arqid AND a.arqstatus = 'A' AND aqsstatus = 'A'
		WHERE ae.aqsstatus = 'A' AND ae.sfndeid = " . $sfndeid . " 
		ORDER BY arqid";
}
$db = new cls_banco();

$arquivosDB = $db->carregar($sql);

$filhos   = array();
$pastas   = array();
$arquivos = array();
$arOrdem  = array();

if( is_array( $arquivosDB ) ){
	foreach($arquivosDB as $k=>$arq){
		$path_fake    = $arq['arqnome'] . '.' . $arq['arqextensao'];
	
		// escrever itens que nao sao pastas
		if(isset($_REQUEST['type']) && $_REQUEST['type'] == 'list'){
	
			$tamanho = $arq['arqtamanho'];
			$date    = date ("d/m/Y", strtotime($arq['arqdata']));
			$tamanho = $tamanho == '' ? '0' : $tamanho;
	
			$arquivos[$k]["data"]["title"]    = $arq['arqnome'] . '.' . $arq['arqextensao'];
			$arquivos[$k]["attr"]["id"]       = $arq['arqid'];
			$arquivos[$k]["attr"]["path"]     = $arq['arqid'];
			$arquivos[$k]["attr"]["href"]     = "/slideshow/slideshow/verimagem.php?arqid=" . $arq['arqid'];
			$arquivos[$k]["attr"]["modified"] = round(($tamanho / 1024), 2) . ' KB';
			$arquivos[$k]["attr"]["date"]     = $date;
			$arquivos[$k]["attr"]["icon"]     = '/slideshow/slideshow/verimagem.php?arqid=' . $arq['arqid'] . '&newwidth=100&newheight=85';
		}
	
	
		if(isset($_REQUEST['order'])){
			switch ($_REQUEST['order']) {
				case 'nome':
					$nome = (strtolower($path_fake));
					if(isset($arq[$k])){
						$arOrdem['arquivos'][] = $nome;
					}
					break;
				case 'tamanho':
					$tamanho = isset($arquivos[$k]["attr"]["modified"]) ? floatval(str_replace(' KB', '', $arquivos[$k]["attr"]["modified"])) : floatval(0);
					if(isset($arquivos[$k])){
						$arOrdem['arquivos'][] = $tamanho;
					}
					break;
				case 'modificacao':
					// concatenar com o indice para garantir a nao repeticao
					$modificacao = intval(date ("Ymd", strtotime($arq['arqdata']))) . ".{$k}";
					if(isset($arquivos[$k])){
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

// arrumar as codificacoes
$filhos = arrumaCodificacaoItens($filhos);
// ordena o array
if(isset($_REQUEST['order'])){

	$arOrdenado = array();
        
        if(!is_array($arOrdem['pastas'])){
            $arOrdem['pastas'] = array();
        }
        if(!is_array($arOrdem['arquivos'])){
            $arOrdem['arquivos'] = array();
        }
        
	$arOrdem = array_merge($arOrdem['pastas'], $arOrdem['arquivos']);

	foreach ($arOrdem as $k => $ordem){
		$arOrdenado["{$ordem}"] = $filhos[$k];
	}
	ksort($arOrdenado); //ordena pelo indice

	$filhos = array();
	//retorna para o array de filhos, preservado o indice numerico
	foreach ($arOrdenado as $k => $item){
		$filhos[] = $item;
	}

	//se for a mesma ordem clicada anteriormente inverte o array
	if(isset($_SESSION['arquivos']['lastOrder']) && $_REQUEST['order'] == $_SESSION['arquivos']['lastOrder']){
		$filhos = array_reverse($filhos);
		unset($_SESSION['arquivos']['lastOrder']);
	}else{
		$_SESSION['arquivos']['lastOrder'] = $_REQUEST['order'];
	}

}

echo simec_json_encode($filhos);
die();

//header("HTTP/1.0 404 Not Found");
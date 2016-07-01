<?php

date_default_timezone_set ('America/Sao_Paulo');


// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
//include_once '_funcoes.php';
//include_once '_componentes.php';

function ImgSize($imgend,$img_max_dimX,$img_max_dimY){
	$imginfo = getimagesize($imgend);
	$width = $imginfo[0];
	$height = $imginfo[1];
	if (($width >$img_max_dimX) or ($height>$img_max_dimY)){
		  if ($width > $height){
			  $w = $width * 0.9;
			  while ($w > $img_max_dimX){
				  $w = $w * 0.9;
			  }
			  $w = round($w);
			  $h = ($w * $height)/$width;
		  }else{
			  $h = $height * 0.9;
			  while ($h > $img_max_dimY){
				  $h = $h * 0.9;
			  }
			  $h = round($h);
			  $w = ($h * $width)/$height;
		  }
	}else{
		  $w = $width;
		  $h = $height;
	}
	$detalhes_foto['width'] = $w;
	$detalhes_foto['height'] = $h;
	return $detalhes_foto;
}
// abre conexão com o servidor de banco de dados
$db = new cls_banco();
$sql = "SELECT arqtipo, arqid  FROM public.arquivo 
		WHERE sisid = '". $_REQUEST['sisid'] ."' and arqid = '". $_REQUEST['arqid'] ."'";
$dados = $db->pegaLinha($sql);
if($dados) {
	
	$caminho = '../../arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($dados['arqid']/1000) .'/'.$dados['arqid'];
	//dbg('../../arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($dados['arqid']/1000) .'/'.$dados['arqid'],1);
	// verifica se o arquivo existe antes de carrega-lo
	if(!is_file($caminho)) {
		if ( $_SESSION['sisarquivo'] == 'obras2' ){
			$caminho = '../../arquivos/obras/'. floor($dados['arqid']/1000) .'/'.$dados['arqid'];
			if(!is_file($caminho)) {
				return false;
				exit;
			}
		}else{
			return false;
			exit;
		}		
	}
	
	$expires = 3600;
	$cache_time = mktime(0,0,0,date('m'),date('d')+1,date('Y'));
	header("Expires: " . date("D, d M Y H:i:s",$cache_time) . " GMT");
	header("Cache-Control: max-age=$expires, must-revalidate");
	header('Content-type:'.$dados['arqtipo']);
	list($width, $height) = getimagesize($caminho);
	if($_REQUEST['newwidth'] || $_REQUEST['newheight']) {
		$d = ImgSize($caminho,$_REQUEST['newwidth'],$_REQUEST['newheight']);
		$thumb = imagecreatetruecolor($d['width'], $d['height']);
		switch($dados['arqtipo']) {
		case 'image/jpeg':
		$source = imagecreatefromjpeg($caminho);
		// 	Resize
		imagecopyresized($thumb, $source, 0, 0, 0, 0, $d['width'], $d['height'], $width, $height);
		imagejpeg($thumb);	
		break;
		case 'image/gif':
		$source = imagecreatefromgif($caminho);
		// 	Resize
		imagecopyresized($thumb, $source, 0, 0, 0, 0, $d['width'], $d['height'], $width, $height);
		imagegif($thumb);
		break;
		case 'image/png':
		$source = imagecreatefrompng($caminho);
		// 	Resize
		imagecopyresized($thumb, $source, 0, 0, 0, 0, $d['width'], $d['height'], $width, $height);
		imagepng($thumb);
		break;
		}
		//Clean-up memory
		ImageDestroy($thumb);
	} else {
		readfile($caminho);
	}
}
?>
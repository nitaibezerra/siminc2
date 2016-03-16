<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();

ini_set("memory_limit", "2048M");
set_time_limit(0);

if(is_numeric($_REQUEST['arqid'])) {
	$sql = "SELECT arqtipo, arqid  FROM public.arquivo 
			WHERE arqid = '" . $_REQUEST['arqid'] . "'";
	
	$dados = $db->pegaLinha($sql);
}

if ($dados['arqtipo'] == "image/jpe") {
    $dados['arqtipo'] = "image/jpeg";
}

if ($dados) {
    $caminho = '../../../arquivos/' . (($_REQUEST["_sisarquivo"]) ? $_REQUEST["_sisarquivo"] : $_SESSION["sisarquivo"]) . '/' . floor($dados['arqid'] / 1000) . '/' . $dados['arqid'];

    if (!is_file($caminho)) {
        if ($_SESSION['sisarquivo'] == 'obras2' || $_REQUEST['_sisarquivo'] == 'obras2') {
            $caminho = '../../../arquivos/obras/' . floor($dados['arqid'] / 1000) . '/' . $dados['arqid'];
            if (!is_file($caminho)) {
                return false;
                exit;
            }
        } else {
            return false;
            exit;
        }
    }

    $cache_time = mktime(0, 0, 0, date('m'), date('d') + 1, date('Y'));

    header("Expires: " . date("D, d M Y H:i:s", $cache_time) . " GMT");
    header("Cache-Control: max-age=3600, must-revalidate");
    mostraArquivo($caminho, $dados);
    
    unset($dados);
}

function mostraArquivo($caminho,$dados)
{
	$type = exif_imagetype($caminho);
    
    if($type) {
        list($width, $height) = getimagesize($caminho);
     
        if ($_REQUEST['newwidth'] || $_REQUEST['newheight']) {
            $d = ImgSize($caminho, $_REQUEST['newwidth'], $_REQUEST['newheight']);
            if (!$d['width'] || !$d['height']) {
                $d['width']  = $width;
                $d['height'] = $height;
            }
        } else {
            $d['width']  = $width;
            $d['height'] = $height;
        }
        
        if(!$d['width']) $d['width'] = 200;
        if(!$d['height']) $d['height'] = 200;
        
        $thumb = imagecreatetruecolor($d['width'], $d['height']);

        switch ($type){
            case 1://	IMAGETYPE_GIF
                header('Content-type: ' . image_type_to_mime_type(IMAGETYPE_GIF)); // Header do tipo da imagem
                $source = @imagecreatefromgif($caminho); // Aloca a imagem na memória
                imagecopyresized($thumb, $source, 0, 0, 0, 0, $d['width'], $d['height'], $width, $height); // Redefine os tamanhos
        		if( is_resource( $source ) ){
                	imagedestroy($source);
                	unset($source);
                }
                if($_SESSION['sisid'] == '147' && necessitaMarcaDaguaFNDE($_REQUEST['arqid'])){aplicaMarcaDagua($thumb, $d['width'], $d['height'], $width, $height, 'FNDE');} //Coloca marca d'água (FNDE) caso necessário
                imagegif($thumb);
                break;
            case 2://	IMAGETYPE_JPEG
                header('Content-type: ' . image_type_to_mime_type(IMAGETYPE_JPEG)); // Header do tipo da imagem
                $source = @imagecreatefromjpeg($caminho); // Aloca a imagem na memória
                if( is_resource( $source ) ){
                	imagecopyresized($thumb, $source, 0, 0, 0, 0, $d['width'], $d['height'], $width, $height); // Redefine os tamanhos
                	imagedestroy($source);
                	unset($source);
                }
                if($_SESSION['sisid'] == '147' && necessitaMarcaDaguaFNDE($_REQUEST['arqid'])){aplicaMarcaDagua($thumb, $d['width'], $d['height'], $width, $height, 'FNDE');} //Coloca marca d'água (FNDE) caso necessário
                imagejpeg($thumb);// Mostra a Imagem
                break;
            case 3://	IMAGETYPE_PNG
                header('Content-type: ' . image_type_to_mime_type(IMAGETYPE_PNG)); // Header do tipo da imagem
                $source = @imagecreatefrompng($caminho); // Aloca a imagem na memória
                if( is_resource( $source ) ){
                	imagecopyresized($thumb, $source, 0, 0, 0, 0, $d['width'], $d['height'], $width, $height); // Redefine os tamanhos
                	imagedestroy($source);
                	unset($source);
                }
                if($_SESSION['sisid'] == '147' && necessitaMarcaDaguaFNDE($_REQUEST['arqid'])){aplicaMarcaDagua($thumb, $d['width'], $d['height'], $width, $height, 'FNDE');} //Coloca marca d'água (FNDE) caso necessário
                imagepng($thumb);// Mostra a Imagem
                break;
            case 6://	IMAGETYPE_BMP
                header('Content-type: ' . image_type_to_mime_type(IMAGETYPE_WBMP)); // Header do tipo da imagem
                $source      = @imagecreatefrombmp($caminho); // Aloca a imagem na memória
                $source      = $source[0];
                $width       = $source[1];
                $height      = $source[2];
                if (!$d['width'] || !$d['height']) {
                    $d['width']  = $width;
                    $d['height'] = $height;
                }
                if( is_resource( $source ) ){
	                imagecopyresized($thumb, $source, 0, 0, 0, 0, $d['width'], $d['height'], $width, $height); // Redefine os tamanhos
	                imagedestroy($source);
	                unset($source);
                }
                if($_SESSION['sisid'] == '147' && necessitaMarcaDaguaFNDE($_REQUEST['arqid'])){aplicaMarcaDagua($thumb, $d['width'], $d['height'], $width, $height, 'FNDE');} //Coloca marca d'água (FNDE) caso necessário
                image2wbmp($thumb);// Mostra a Imagem
                break;
            case 15://	IMAGETYPE_WBMP
                header('Content-type: ' . image_type_to_mime_type(IMAGETYPE_WBMP)); // Header do tipo da imagem
                $source = @imagecreatefromwbmp($caminho); // Aloca a imagem na memória
                if( is_resource( $source ) ){
                	imagecopyresized($thumb, $source, 0, 0, 0, 0, $d['width'], $d['height'], $width, $height); // Redefine os tamanhos
                	imagedestroy($source);
                	unset($source);
                }
                if($_SESSION['sisid'] == '147' && necessitaMarcaDaguaFNDE($_REQUEST['arqid'])){aplicaMarcaDagua($thumb, $d['width'], $d['height'], $width, $height, 'FNDE');} //Coloca marca d'água (FNDE) caso necessário
                imagewbmp($thumb);// Mostra a Imagem
                break;
            case 16://	IMAGETYPE_XBM
                header('Content-type: ' . image_type_to_mime_type(IMAGETYPE_XBM)); // Header do tipo da imagem
                $source = @imagecreatefromxbm($caminho); // Aloca a imagem na memória
                if( is_resource( $source ) ){
                	imagecopyresized($thumb, $source, 0, 0, 0, 0, $d['width'], $d['height'], $width, $height); // Redefine os tamanhos
                	imagedestroy($source);
                	unset($source);
                }
                if($_SESSION['sisid'] == '147' && necessitaMarcaDaguaFNDE($_REQUEST['arqid'])){aplicaMarcaDagua($thumb, $d['width'], $d['height'], $width, $height, 'FNDE');} //Coloca marca d'água (FNDE) caso necessário
                imagexbm($thumb);// Mostra a Imagem
                break;
            default :
                header('Content-type:' . $dados['arqtipo']);
                readfile($caminho);
                break;
        }
        if ($thumb) {
        	imagedestroy($thumb);
        	unset($thumb);
        }
    } else {
        readfile($caminho);
    }
}

function ImgSize($imgend, $img_max_dimX = 0, $img_max_dimY = 0) {
    $imginfo = getimagesize($imgend);
    $width = $imginfo[0];
    $height = $imginfo[1];

    if (($width > $img_max_dimX) or ( $height > $img_max_dimY)) {
        if ($width > $height) {
            $w = $width * 0.9;
            while ($w > $img_max_dimX) {
                $w = $w * 0.9;
            }
            $w = round($w);
            $h = ($w * $height) / $width;
        } else {
            $h = $height * 0.9;
            while ($h > $img_max_dimY) {
                $h = $h * 0.9;
            }
            $h = round($h);
            $w = ($h * $width) / $height;
        }
    } else {
        $w = $width;
        $h = $height;
    }
    $detalhes_foto['width'] = $w;
    $detalhes_foto['height'] = $h;

    return $detalhes_foto;
}

function necessitaMarcaDaguaFNDE($arqid){
    include_once APPRAIZ . "includes/classes/Modelo.class.inc";
    include_once APPRAIZ . "includes/classes/modelo/obras2/Obras.class.inc";
    $objObra = new Obras();
    $resposta = $objObra->necessitaMarcaDaguaFNDE($arqid);
    return $resposta;
}

function aplicaMarcaDagua(&$thumb, $dest_w, $dest_h, $orig_w, $orig_h, $tipo_marcadagua = 'FNDE'){
    switch ($tipo_marcadagua) {
        case 'FNDE':
            $caminho_marcadagua = APPRAIZ . 'www/imagens/obras/obras2_fnde_wm_20.png';
            break;
        default:
            $caminho_marcadagua = APPRAIZ . 'www/imagens/marcadagua_transparente.png';
            break;
    }
    
    $marcadagua         = @imagecreatefrompng($caminho_marcadagua);
    $thumb_marcadagua   = @imagecreatetruecolor($dest_w, $dest_h);
    $trans_colour       = @imagecolorallocatealpha($thumb_marcadagua, 0, 0, 0, 127);
    imagesavealpha($thumb_marcadagua, true);
    imagefill($thumb_marcadagua, 0, 0, $trans_colour);
    imagecopyresized($thumb_marcadagua, $marcadagua, 0, 0, 0, 0, $dest_w, $dest_h, imagesx($marcadagua), imagesy($marcadagua));
    
    if ($marcadagua) {
    	imagedestroy($marcadagua);
    	unset($marcadagua);
    }
    
    imagecopy($thumb, $thumb_marcadagua, 0, 0, 0, 0, $orig_w, $orig_h);

    if ($thumb_marcadagua) {
    	imagedestroy($thumb_marcadagua);
    	unset($thumb_marcadagua);
    }
    
    unset($trans_colour);
}

function imagecreatefrombmp($caminhoArquivo) {
	$read = "";
	
	if ($caminhoArquivo) {
		$file = fopen($caminhoArquivo, "rb");
	    
	    while (!feof($file) && ($read <> "")){
	    	if( is_resource($file) ){
	        	$read .= fread($file, 1024);
	    	}
	    }
	    
	    fclose($file);
	}
	    
    $temp   = unpack("H*", $read);
    $hex    = $temp[1];
    $header = substr($hex, 0, 108);

    if (substr($header, 0, 4) == "424d") {
        $header_parts = str_split($header, 2);
        $width = hexdec($header_parts[19] . $header_parts[18]);
        $height = hexdec($header_parts[23] . $header_parts[22]);
        $dpix = hexdec($header_parts[39]. $header_parts[38]) * 0.0254;
        $dpiy = hexdec($header_parts[43]. $header_parts[42]) * 0.0254;

        unset($header_parts);
    }

    $x = 0;
    $y = 1;

	$width = (int)$width;
	$height = (int)$height;

	if( $width > 0 && $height >0 ){

		$image = imagecreatetruecolor($width, $height);
		$body = substr($hex, 108);
		$body_size = (strlen($body) / 2);
		$header_size = ($width * $height);

		$usePadding = ($body_size > ($header_size * 3) + 4);

		for ($i = 0; $i < $body_size; $i+=3) {
			if ($x >= $width) {
				if ($usePadding){
					$i += $width % 4;
				}
				$x = 0;
				$y++;
				if ($y > $height){ break; }
			}

			$i_pos = $i * 2;
			$r = hexdec($body[$i_pos + 4] . $body[$i_pos + 5]);
			$g = hexdec($body[$i_pos + 2] . $body[$i_pos + 3]);
			$b = hexdec($body[$i_pos] . $body[$i_pos + 1]);

			$color = imagecolorallocate($image, $r, $g, $b);
			imagesetpixel($image, $x, $height - $y, $color);

			$x++;
		}

		unset($body);

		return array($image, $dpix, $dpiy);

	}
	return array(null, null, null);

}

?>
<?

session_start();

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
// abre conexão com o servidor de banco de dados
$db = new cls_banco();

function deletararquivos($files) {
	foreach($files as $fl) {
		if(file_exists($fl)) unlink($fl);
	}
}

function processararquivos($files,$orig,$dest) {
    if ($files[0]) {
        if(!is_dir("../../arquivos/".$dest."/files_tmp")) {
            mkdir("../../arquivos/".$dest."/files_tmp");
        }

        foreach($files as $f) {
            $endorigem  = "../../arquivos/".$orig."/".floor($f['arqid']/1000)."/".$f['arqid'];
            $enddestino = "../../arquivos/".$dest."/files_tmp/".$f['arqid'].".".$f['arqextensao'];
            
            if(file_exists($endorigem)) {
                if(copy($endorigem, $enddestino)) {
                    if($_SESSION['sisid'] == '147' && necessitaMarcaDaguaFNDE($f['arqid'])){
                    	list($width, $height) = getimagesize($endorigem);
	                    $d['width']  = $width;
	                    $d['height'] = $height;

	                    $thumb = imagecreatetruecolor($d['width'], $d['height']);
	                    $source = imagecreatefromjpeg ($endorigem);

                    	imagecopyresized($thumb, $source, 0, 0, 0, 0, $d['width'], $d['height'], $width, $height); // Redefine os tamanhos
                    	aplicaMarcaDagua($thumb, $d['width'], $d['height'], $width, $height, 'FNDE'); //Coloca marca d'água (FNDE) caso necessário
                        $type = exif_imagetype($endorigem);

                        switch ($type){
                            case 1://	IMAGETYPE_GIF
                                imagegif($thumb, $enddestino);// Mostra a Imagem
                                break;
                            case 2://	IMAGETYPE_JPEG
                                imagejpeg($thumb, $enddestino);// Mostra a Imagem
                                break;
                            case 3://	IMAGETYPE_PNG
                                imagepng($thumb, $enddestino);// Mostra a Imagem
                                break;
                            case 6://	IMAGETYPE_BMP
                                image2wbmp($thumb, $enddestino);// Mostra a Imagem
                                break;
                            case 15://	IMAGETYPE_WBMP
                                imagewbmp($thumb, $enddestino);// Mostra a Imagem
                                break;
                            case 16://	IMAGETYPE_XBM
                                imagexbm($thumb, $enddestino);// Mostra a Imagem
                                break;
                            default :
                                break;
                        }
                    }
                    
                    //Clean-up memory
                    ImageDestroy($thumb);
                    ImageDestroy($source);
                    
                    if(file_exists($enddestino)) $fzip[] = $enddestino;
               }
            } elseif ( $orig == 'obras2' ) {
                $orig = 'obras';
                $_SESSION['downloadfiles']['pasta']['origem']  = 'obras';

				if(!is_dir("../../arquivos/".$dest."/files_tmp")) {
					mkdir("../../arquivos/".$dest."/files_tmp");
				}

				$endorigem  = "../../arquivos/".$orig."/".floor($f['arqid']/1000)."/".$f['arqid'];
				$enddestino = "../../arquivos/".$dest."/files_tmp/".$f['arqid'].".".$f['arqextensao'];
				
				if(file_exists($endorigem)) {
					if(copy($endorigem, $enddestino)) {
						if(file_exists($enddestino)) $fzip[] = $enddestino; 	
					}
				}
				
				$orig = 'obras2';
			}
		}
	}
	
	return $fzip;
}

function aplicaMarcaDagua(&$thumb, $dest_w, $dest_h, $orig_w, $orig_h, $tipo_marcadagua = 'FNDE') {
    switch ($tipo_marcadagua) {
        case 'FNDE':
            $caminho_marcadagua = APPRAIZ . 'www/imagens/obras/obras2_fnde_wm_20.png';
            break;
        default:
            $caminho_marcadagua = APPRAIZ . 'www/imagens/marcadagua_transparente.png';
            break;
    }

    $marcadagua         = imagecreatefrompng($caminho_marcadagua);
    $thumb_marcadagua   = imagecreatetruecolor($dest_w, $dest_h);
    $trans_colour       = imagecolorallocatealpha($thumb_marcadagua, 0, 0, 0, 127);
    imagesavealpha($thumb_marcadagua, true);
    imagefill($thumb_marcadagua, 0, 0, $trans_colour);
    imagecopyresized($thumb_marcadagua, $marcadagua, 0, 0, 0, 0, $dest_w, $dest_h, imagesx($marcadagua), imagesy($marcadagua));
    imagecopy($thumb, $thumb_marcadagua, 0, 0, 0, 0, $orig_w, $orig_h);
    
    //Clean-up memory
    ImageDestroy($marcadagua);
    ImageDestroy($thumb_marcadagua);
}

function necessitaMarcaDaguaFNDE($arqid){
    //Regra na Classe de Obras para evitar ficar sempre editando o arquivo verimagem.php
    include_once APPRAIZ . "includes/classes/Modelo.class.inc";
    include_once APPRAIZ . "includes/classes/modelo/obras2/Obras.class.inc";
    $objObra = new Obras();
    $resposta = $objObra->necessitaMarcaDaguaFNDE($arqid);
    return $resposta;
}

$_SESSION['downloadfiles']['pasta']['origem'] = ($_SESSION['downloadfiles']['pasta']['origem'] 
													? $_SESSION['downloadfiles']['pasta']['origem'] 
													: $_SESSION['sisarquivo']);
$_SESSION['downloadfiles']['pasta']['destino'] = ($_SESSION['downloadfiles']['pasta']['destino'] 
													? $_SESSION['downloadfiles']['pasta']['destino'] 
													: $_SESSION['sisarquivo']);
/*
 * $_REQUEST[fotosselecionadas] : Array de ids da tabela public.arquivo, 
 * esses arquivos passados serão compactados em um arquivo .ZIP 
 */
if(count($_REQUEST['fotosselecionadas']) > 0) {
	include('../../includes/pclzip-2-6/pclzip.lib.php');
	$files = $db->carregar("SELECT arqid, arqextensao FROM public.arquivo WHERE arqid IN('".implode("','",$_REQUEST['fotosselecionadas'])."')");
	$filezip = processararquivos($files,$_SESSION['downloadfiles']['pasta']['origem'],$_SESSION['downloadfiles']['pasta']['destino']);
	$nomearquivozip = $_SESSION['usucpf'].'_'.date('dmyhis').'.zip';
	$enderecozip = '../../arquivos/'.$_SESSION['downloadfiles']['pasta']['destino'].'/files_tmp/'.$nomearquivozip;
	$archive = new PclZip($enderecozip);
	$archive->create( $filezip,  PCLZIP_OPT_REMOVE_ALL_PATH);
	if($filezip) deletararquivos($filezip);
/*
 * $_REQUEST[enderecoabsolutoarquivo] : Se possui essa variavel, 
 * o programa vai pegar o arqid de apenas um arquivo, 
 * e fazer com que o usuario faça o download na extensão original 
 */
} elseif($_REQUEST['enderecoabsolutoarquivo']) {
	if($_REQUEST['arqid']) {
		$files = $db->carregar("SELECT arqid, arqextensao FROM public.arquivo WHERE arqid IN('".$_REQUEST['arqid']."')");
		$filezip = processararquivos($files,$_SESSION['downloadfiles']['pasta']['origem'],$_SESSION['downloadfiles']['pasta']['destino']);
		if(count($filezip) > 0) {
			$files = $files[0];
			$nomearquivozip = $files['arqid'].'.'.$files['arqextensao'];
			$enderecozip = current($filezip);
		} else {
			echo "<script>alert('Erro no download. Entre em contato com a equipe técnica.');window.close();</script>";
			exit;
		}
	} else {
		echo "<script>alert('Erro no download. Entre em contato com a equipe técnica.');window.close();</script>";
		exit;
	}
} else {
	echo "<script>alert('Não foi selecionado nenhuma foto.');window.close();</script>";
	exit;
}

if(is_file($enderecozip)) {

header("Content-Disposition: attachment; filename=".$nomearquivozip);
header("Content-Type: application/oct-stream");
header("Expires: 0");
header("Pragma: public");
header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
readfile($enderecozip);

} else {
	echo "<script>alert('Arquivo não encontrado');</script>";
}

$_SESSION['downloadfiles']['pasta']['origem']  = '';
$_SESSION['downloadfiles']['pasta']['destino'] = '';

echo "<script>window.close();</script>";

?>
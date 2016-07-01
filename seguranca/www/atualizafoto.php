<?php
require_once "config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "www/includes/fotosEntidades/ClassImage.php";

$upload = new ClassImage();

$storage = DIRFILES . "seguranca/usuario/";

if (!is_dir(DIRFILES . "seguranca")) {
	mkdir(DIRFILES . "seguranca", 0777);
}

if (!is_dir(DIRFILES . "seguranca/usuario")) {
	mkdir(DIRFILES . "seguranca/usuario", 0777);
}

$extencao = strtolower((end(explode(".", $_FILES['file']['name']))));

switch ($extencao) {
	case 'gif':
		$_FILES['file']['type'] = 'image/gif';
		break;
	case 'jpg':
	case 'jpeg':
		$_FILES['file']['type'] = 'image/jpeg';
		break;
	case 'png':
		$_FILES['file']['type'] = 'image/png';
		break;
	case 'bmp':
		$_FILES['file']['type'] = 'image/bmp';
		break;
}

$filename = str_replace("/","",substr(md5_encrypt(tirar_acentos($_FILES['file']['name'][$i]))."__extension__".md5_encrypt($_FILES['file']['type'][$i])."__temp__".date('YmdHis').rand(1,10000).md5_encrypt(tirar_acentos($_FILES['file']['name'][$i])), 0, 150));
$uploadfile = $storage . $filename;
$uploaded = $upload->reduz_imagem($_FILES['file']['tmp_name'], 64, 64, $uploadfile, strtolower((end(explode(".", $_FILES['file']['name'])))));

if ($uploaded) {
	$db = new cls_banco();
	
	$part1file = explode("__temp__", $filename);
	$part2file = base64_decode($part1file[0]);
	$part2file = explode("__extension__", $part2file);
	
	$foto = explode(".", $_FILES['file']['name']);
	
	$sql = "SELECT entid FROM seguranca.usuario WHERE usucpf = '" . $_SESSION['usucpf'] . "'";
	$entid = $db->pegaUm($sql);
	
	$sql = "SELECT arqid FROM entidade.fotoentidade WHERE entid = '" . $entid . "' AND fotbox = 'perfil' ORDER BY fotordem DESC LIMIT 1";
	$id = $db->pegaUm($sql);

	if ($id) {
		$db->executar("DELETE FROM entidade.fotoentidade WHERE entid = '{$entid}' AND fotbox = 'perfil'");
		$db->executar("DELETE FROM public.arquivo WHERE arqid = '{$id}'");
		
		if (is_file($storage . floor($id/1000) . '/' . $id)) {
			unlink($storage . floor($id/1000) . '/' . $id);			
		}
	}
	
	$sql = "INSERT INTO public.arquivo 
			(arqnome, arqextensao, arqtipo, arqdata, arqhora, usucpf, sisid) VALUES 
			('".$foto[0]."', '".$extencao."', '".$_FILES['file']['type']."', '".date('Y-m-d')."', '".date('H:i:s')."', '".$_SESSION["usucpf"]."', 4) RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
	
	if (!is_dir($storage . floor($arqid/1000))) {
		mkdir($storage . floor($arqid/1000), 0777);
	}
	
	if (copy($storage . $filename, $storage . floor($arqid/1000) . "/" . $arqid)) {
		$sql = "INSERT INTO entidade.fotoentidade 
				(arqid, entid, fotdsc, fotbox, fotordem) VALUES
				(".$arqid.", ".$entid.", '".$foto."', 'perfil', ".(($ordem)?($ordem+1):'1').")";
		
		$db->executar($sql);
		$db->commit();
		
		if (is_file($storage . $filename)) {
			unlink($storage . $filename);			
		}
	} else {
		$db->rollback();
		print simec_json_encode(array('status' => 'error', 'message' => 'Falha ao copiar o arquivo'));
	}
	
	print simec_json_encode(array('status' => 'success'));
}
?>
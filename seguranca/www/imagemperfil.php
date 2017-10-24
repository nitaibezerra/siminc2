<?php
require_once "config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";

$db = new cls_banco();

$storage = DIRFILES . "/seguranca/usuario/";
$sql = "SELECT entid FROM seguranca.usuario WHERE usucpf = '" . $_SESSION['usucpf'] . "'";
$entid = $db->pegaUm($sql);

$entid = $entid ? $entid : 0;

$sql = "SELECT arqid FROM entidade.fotoentidade WHERE entid = '" . $entid . "' AND fotbox = 'perfil' ORDER BY fotordem DESC LIMIT 1";
$id = $db->pegaUm($sql);

if ($id && is_file(DIRFILES . '/seguranca/usuario/'. floor($id/1000) .'/'.$id)) {
	header("Location: ../../includes/highslide/verimagem.php?arqid={$id}&_sisarquivo=seguranca/usuario");
} else {
	$cache = mktime(0,0,0,date('m'),date('d')+1,date('Y'));

	header("Expires: " . date("D, d M Y H:i:s", $cache) . " GMT");
	header("Cache-Control: max-age=3600, must-revalidate");
	header('Content-type: image/jpg');
	readfile('../../zimec/public/img/profile_small.jpg');
}
?>
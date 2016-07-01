<?
//teste de comentario
unset($_SESSION['conjur_var']);

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

$permissoes = verificaPerfilConjur();

if( $_SESSION['conjur']['perfilid'] != PRF_ADVOGADO && $_SESSION['favurl'] != 'conjur.php?modulo=inicio&acao=S' && $_REQUEST['req'] != "ACsidoc"){
	echo "<script>window.location = 'conjur.php?modulo=inicio&acao=S';</script>";
	die();
}
include APPRAIZ . '/conjur/modulos/pesquisa.inc';

?>
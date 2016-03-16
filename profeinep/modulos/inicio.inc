<?

unset($_SESSION['profeinep_var']);
//$_SESSION['profeinep_var']['prcid'] = '';
//$_SESSION['profeinep_var']['tprid'] = '';

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

$permissoes = verificaPerfilProfeinep();

if( $_SESSION['favurl'] != 'profeinep.php?modulo=inicio&acao=S' && $_REQUEST['origem'] == ""){
	echo "<script>window.location = 'profeinep.php?modulo=inicio&acao=S';</script>";
	die();
}

include APPRAIZ . '/profeinep/modulos/pesquisa.inc';


?>
<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

include_once APPRAIZ . 'includes/library/simec/Crud/Listing.php';
include_once APPRAIZ . 'includes/library/simec/Autoload.php';

$arrModulo = explode('/', $_GET['modulo']);
$modulo = reset($arrModulo);

if ( (!empty($modulo) && $modulo == 'sistema') && !in_array('usuario',$arrModulo)    ) {
	$_SESSION['sislayoutbootstrap'] = false;
} else {
	$_SESSION['sislayoutbootstrap'] = true;
}
?>
<?php include_once "controleAcesso.inc"; ?>

<link rel="stylesheet" href="/pet/css/pet.css">
<link rel="stylesheet" href="/pet/css/nav_custom.css">
<script language="javascript" src="/pet/js/func_gerais.js"></script>
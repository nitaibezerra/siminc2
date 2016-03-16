<?
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";

$modulo=$_REQUEST['modulo'];
// include "_funcoesliberacao.php";
// include "_constantes.php";

include_once '_constantes.php';
include_once '_funcoesliberacao.php';
include_once '_funcoes.php';

simec_magic_quotes();

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>

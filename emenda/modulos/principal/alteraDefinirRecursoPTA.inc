<?

verificaPermissao();

// Inclui o script para alteraчуo de um PTA existente
if( $_REQUEST['tipo'] ){
	$tipo = 'popup';
	$_SESSION['emenda']['tpopup'] = $tipo;
} else {
	$tipo = 'alteracao';
	unset($_SESSION['emenda']['tpopup']);
}

include "recursosPTA.inc";

?>
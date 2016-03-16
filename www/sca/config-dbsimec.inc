<?php
	include_once APPRAIZ . "global/config.inc";

	global $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd, $email_sistema,$servidor_bd_siafi, $porta_bd_siafi, $nome_bd_siafi, $usuario_db_siafi, $senha_bd_siafi;
	
	define('AUTHSSD', false);
    
	session_start();
	
	if ($_REQUEST['baselogin']) {
    	$_SESSION['baselogin'] = $_REQUEST['baselogin'];
	}
        
   	$nome_bd     = '';
	$servidor_bd = '';
	$porta_bd    = '5432';
	$usuario_db  = '';
	$senha_bd    = '';
?>

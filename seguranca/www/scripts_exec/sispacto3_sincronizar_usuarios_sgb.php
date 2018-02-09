<?php

header( 'Content-Type: text/html; charset=ISO-8859-1' );
//header( 'Content-Type: text/html; charset=UTF-8' );

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );


error_reporting( E_ALL ^ E_NOTICE );

ini_set("memory_limit", "1024M");
set_time_limit(0);

ini_set( 'soap.wsdl_cache_enabled', '0' );
ini_set( 'soap.wsdl_cache_ttl', 0 );


$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funчѕes gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/sispacto3/_constantes.php";
require_once APPRAIZ . "www/sispacto3/_funcoes.php";


if(!$_SESSION['usucpf']) {
	// CPF do administrador de sistemas
	$_SESSION['usucpforigem'] = '00000000191';
	$_SESSION['usucpf'] = '00000000191';
}

function getmicrotime() {list($usec, $sec) = explode(" ", microtime()); return ((float)$usec + (float)$sec);}

$microtime = getmicrotime();
   
// abre conexчуo com o servidor de banco de dados
$db = new cls_banco();

// black list
$pularcpf = $db->carregarColuna("SELECT lnscpf FROM sispacto3.listanegrasgb");


$sql = "SELECT DISTINCT i.iusd, l.logcpf FROM sispacto3.identificacaousuario i 
		LEFT JOIN log_historico.logsgb_sispacto3 l ON l.logcpf = i.iuscpf
		WHERE iustermocompromisso=true AND cadastradosgb=false".(($pularcpf)?" AND iuscpf NOT IN('".implode("','",$pularcpf)."')":"")." ORDER BY l.logcpf DESC";
$iusds = $db->carregarColuna($sql);

libxml_use_internal_errors( true );

if($iusds) {
	foreach($iusds as $iusd) {
		
		$lnsid = $db->pegaUm("INSERT INTO sispacto3.listanegrasgb(lnscpf) VALUES ((SELECT iuscpf FROM sispacto3.identificacaousuario WHERE iusd='".$iusd."')) RETURNING lnsid;");
		$db->commit();
		
		sincronizarDadosUsuarioSGB(array("iusd" => $iusd, "sincronizacao" => true));
		
		$db->executar("DELETE FROM sispacto3.listanegrasgb WHERE lnsid='".$lnsid."'");
		$db->commit();
		
	}
}


echo "Sincronizar USUARIOS DO PACTO NO SGB - OK";


$sql = "SELECT uncid FROM sispacto3.universidadecadastro WHERE (cadastrosgb=false OR cadastrosgb IS NULL)";
$uncids = $db->carregarColuna($sql);

libxml_use_internal_errors( true );

if($uncids) {
	foreach($uncids as $uncid) {
		sincronizarDadosEntidadeSGB(array("uncid" => $uncid));
	}
}


echo "Sincronizar ENTIDADES DO PACTO NO SGB - OK";


/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "SISPACTO 2014 - Sincronizar Usuсrios SGB";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
$mensagem->Subject = "Sincronizar Usuсrios SGB";
$mensagem->Body = "Sincronizaчуo realizada com sucesso";
$mensagem->IsHTML( true );
$mensagem->Send();
/*
 * FIM
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */

$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='sispacto3_sincronizar_usuarios_sgb.php'";
$db->executar($sql);
$db->commit();

$db->close();


if($_SESSION['usucpf'] == '00000000191') {
	
	unset($_SESSION['usucpf']);
	unset($_SESSION['usucpforigem']);
	
}


?>
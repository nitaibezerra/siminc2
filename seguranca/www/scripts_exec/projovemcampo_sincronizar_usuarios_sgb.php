<?php

header('Content-Type: text/html; charset=ISO-8859-1');
//header( 'Content-Type: text/html; charset=UTF-8' );

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));


error_reporting(E_ALL ^ E_NOTICE);

ini_set("memory_limit", "1024M");
set_time_limit(0);

ini_set('soap.wsdl_cache_enabled', '0');
ini_set('soap.wsdl_cache_ttl', 0);


$_REQUEST['baselogin'] = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funчѕes gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/projovemcampo/_constantes.php";
require_once APPRAIZ . "www/projovemcampo/_funcoes.php";


if (!$_SESSION['usucpf']) {
    // CPF do administrador de sistemas
    $_SESSION['usucpforigem'] = '00000000191';
    $_SESSION['usucpf'] = '00000000191';
}

function getmicrotime()
{
    list($usec, $sec) = explode(" ", microtime());
    return ((float)$usec + (float)$sec);
}

$microtime = getmicrotime();

// abre conexчуo com o servidor de banco de dados
$db = new cls_banco();

// black list
// $pularcpf = $db->carregarColuna("SELECT lnscpf FROM projovemcampo.listanegrasgb");


$sql = "SELECT DISTINCT est.estid, l.logcpf 
		FROM projovemcampo.estudante est 
		LEFT JOIN log_historico.logsgb_projovemcampo l ON l.logcpf = est.estcpf
		WHERE --eststatus = 'A' AND
			  cadastradosgb=false
		ORDER BY est.estid DESC
			";

$estids = $db->carregarColuna($sql);
// ver($estids,d);
libxml_use_internal_errors(true);

if ($estids) {
    foreach ($estids as $estid) {

// 		$lnsid = $db->pegaUm("INSERT INTO projovemcampo.listanegrasgb(lnscpf) VALUES ((SELECT iuscpf FROM projovemcampo.identificacaousuario WHERE estid='".$estid."')) RETURNING lnsid;");
// 		$db->commit();

        sincronizarDadosUsuarioSGB(array("estid" => $estid, "sincronizacao" => true));

// 		$db->executar("DELETE FROM projovemcampo.listanegrasgb WHERE lnsid='".$lnsid."'");
// 		$db->commit();

    }
}


echo "Sincronizar USUARIOS DO PROJOVEM CAMPO NO SGB - OK";


// $sql = "SELECT uncid FROM projovemcampo.universidadecadastro WHERE (cadastrosgb=false OR cadastrosgb IS NULL)";
// $uncids = $db->carregarColuna($sql);

// libxml_use_internal_errors( true );

// if($uncids) {
// 	foreach($uncids as $uncid) {
// 		sincronizarDadosEntidadeSGB(array("uncid" => $uncid));
// 	}
// }


// echo "Sincronizar ENTIDADES DO PACTO NO SGB - OK";
$sql = "SELECT distinct
			usuemail, usunome
		FROM
			seguranca.usuario usu
		INNER JOIN seguranca.perfilusuario pfu ON pfu.usucpf = usu.usucpf
		LEFT JOIN seguranca.perfil pfl ON pfl.pflcod = pfu.pflcod
		WHERE
			pfu.pflcod = 1180
		AND sisid = 193";
$emails = $db->carregar($sql);
/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host = "localhost";
$mensagem->Mailer = "smtp";
$mensagem->FromName = "Projovem campo - Sincronizar Usuсrios SGB";
$mensagem->From = $_SESSION['email_sistema'];
foreach($emails as $email){
	$mensagem->AddAddress($email['usuemail'], $email['usunome']);
}
$mensagem->Subject = "Sincronizar Usuсrios SGB";
$mensagem->Body = "Sincronizaчуo realizada com sucesso";
$mensagem->IsHTML(true);
// ver($mensagem,d);
// $mensagem->Send();
/*
 * FIM
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */

$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='" . round((getmicrotime() - $microtime), 2) . "' WHERE agsfile='projovemcampo_sincronizar_usuarios_sgb.php'";
$db->executar($sql);
$db->commit();

$db->close();
echo 'fim';

if ($_SESSION['usucpf'] == '00000000191') {

    unset($_SESSION['usucpf']);
    unset($_SESSION['usucpforigem']);

}


?>
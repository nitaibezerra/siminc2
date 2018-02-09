<?php
ini_set("memory_limit", "3000M");
set_time_limit(30000);

include_once "/var/www/simec/global/config.inc";
//include_once "config.inc";
include_once APPRAIZ . "pde/www/_constantes.php";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

header("Content-Type: text/html; charset=ISO-8859-1",true);
session_start();
//Variaveis de sessão
$_SESSION["url_rm"] = "https://gestaoderiscos.inep.gov.br/RM7";
$_SESSION["url_wf"] = "https://gestaoderiscos.inep.gov.br/wf";
$_SESSION["app_id"] = "67674079b0d44ec99dc79760c3190a20";
$_SESSION["app_secret"] = "f4574619da274beea3c05672c3115e02";
$_SESSION["url_retorno"] = "http://vmrm7:8080/wf.php";
$_SESSION["host"] = "vmrm7";

//*** Verifica se já há um token a ser utilizado pela sessão; senão gera um novo token
if(!isset($_SESSION['token'])) {
    $url_rm = $_SESSION["url_rm"];
    $url_wf = $_SESSION["url_wf"];
    $app_id = $_SESSION["app_id"];
    $app_secret = $_SESSION["app_secret"];
    $url_retorno = $_SESSION["url_retorno"];
    $host = $_SESSION["host"];

    $url_token = $url_rm . "/APIIntegration/Token";
    $params = array(
        "client_id" => $app_id,
        "client_secret" => $app_secret,
        "grant_type" => "client_credentials");

    $head = "Host: ".$host."
             Content-Type: application/x-www-form-urlencoded";

	// Cria sessão URL
	$ch = curl_init();

	// Prepara parâmetros cURL
	curl_setopt($ch, CURLOPT_URL, $url_token);
	curl_setopt($ch, CURLOPT_POST,1);         //define o método a ser utilizado: POST
	curl_setopt($ch, CURLOPT_POSTFIELDS, $params);
	curl_setopt($ch, CURLOPT_HEADER, $head);

	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
	curl_setopt($ch, CURLOPT_CAINFO , 0);

    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT,5);

    // Envia requisição pela cURL
    try { $exec = curl_exec ($ch); curl_close($ch); }
	catch(Exception $e) { echo "Exception" . $e->getMessage(); }
    // Decodifica a resposta
	$token = json_decode($exec);

    // Atribui token e dados às variáveis de sessão
	$_SESSION['token'] = $token->access_token;
	$_SESSION['tokentype'] = $token->token_type;
	$_SESSION['expiresin'] = $token->expires_in;
}
//_*_*_*_*_*_*_*_*_*_*_*__*_*_*_*_*_*_*_*_*_*_C H A M A D A S _*_*_*_*_*_*_*_*_*_*__*_*_*_*_*_*_*_*_*_*_
//ConsultaWF("SIMEC02");
//ConsultaWF("consulta alertas sgir geral");
ConsultaWF("SIMEC2015");

Function ConsultaWF($consulta) { // OK!
	$token = $_SESSION["token"];
	$url_wf = $_SESSION["url_wf"];
	$host = $_SESSION["host"];
	$url = $url_wf.'/api/queries/'.$consulta.'?page=1&page_size=1000';
	
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_HTTPGET, true);
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_HTTPHEADER, array('Host: '.$host,
	                                           'Authorization: OAuth2 '.$token));
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
	
	try { $exec = curl_exec ($ch); curl_close($ch); }
	catch(Exception $e) { echo "Exception" . $e->getMessage(); }
	
	$result = json_decode($exec,true);
	//ver($result);
	$ct = count($result);
	echo "Quantidade de eventos: ".$ct;
	echo "<br>=================================================================================================================================<br>";
	$sql = "DELETE FROM enem.riskmanager_2015;";
	for($i=0;$i<$ct;$i++){
	  	echo "Título $i: ".utf8_decode($result[$i]["Title"])."<br>";
	 	echo "Descrição Alerta $i: ".utf8_decode($result[$i]["Description"])."<br>";
	  	echo "Responsável Alerta $i: ".utf8_decode($result[$i]["Responsible"])."<br>";
	  	echo "Nível do Alerta $i: ".utf8_decode($result[$i]["nivel_do_alerta_sgir"])."<br>";
	  	echo "Descrição do risco no Alerta $i: ".utf8_decode($result[$i]["descricao_do_risco_no_alerta_sgir"])."<br>";
	  	echo "Última atualização do Alerta $i: ".utf8_decode($result[$i]["LastProgressComment"])."<br>";
	  	echo "Caminho Crítico $i: ".utf8_decode($result[$i]["caminho_critico"])."<br>";
	  	echo "Origem do alerta $i: ".utf8_decode($result[$i]["__origem_do_alerta"])."<br>";
	  	echo "Status Alerta $i: ".utf8_decode($result[$i]["Status"])."<br>";
	  	echo "<br>=================================================================================================================================<br>";
		$sql .= "
			INSERT INTO enem.riskmanager_2015(
				topico,ocorrencia,responsavel,
				nivelalerta,risco,
				status,etapasimec,
				statusalerta,
				origem_alerta
				)
				VALUES (
				'".utf8_decode(str_replace('\'','',$result[$i]["Title"]))."',
				'".utf8_decode(str_replace('\'','',$result[$i]["Description"]))."',
				'".utf8_decode(str_replace('\'','',$result[$i]["Responsible"]))."',
				'".utf8_decode(str_replace('\'','',$result[$i]["nivel_do_alerta_sgir"]))."',
				'".utf8_decode(str_replace('\'','',$result[$i]["descricao_do_risco_no_alerta_sgir"]))."',
				'".utf8_decode(str_replace('\'','',$result[$i]["LastProgressComment"]))."',
				'".utf8_decode(str_replace('\'','',$result[$i]["caminho_critico"]))."',
				'".utf8_decode(str_replace('\'','',$result[$i]["Status"]))."',
				'".utf8_decode(str_replace('\'','',$result[$i]["__origem_do_alerta"]))."');";
				}
	global $db;
	$sql .= "UPDATE enem.riskmanager_2015 set etapasimec = '9.0 - NÃO POSSUI' where etapasimec = '';";
	$db->executar($sql);
	$db->commit();
}

/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "WS Atualizar Risk Manager";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
$mensagem->Subject = "WS Atualizar Risk Manager";

$mensagem->Body = $corpoemail;
$mensagem->IsHTML( true );
$mensagem->Send();
/*
 * FIM
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */

?>

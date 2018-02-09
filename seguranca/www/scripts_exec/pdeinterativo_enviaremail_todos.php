<?php

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 

date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


$sql = "SELECT usunome, usuemail  
		FROM
		seguranca.usuario usu
		INNER JOIN
		seguranca.usuario_sistema ususis ON ususis.usucpf = usu.usucpf
		WHERE
		ususis.sisid = 98 AND 
		ususis.suscod = 'A'";

$usuarios = $db->carregar($sql);

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


if($usuarios[0]) {
	foreach($usuarios as $usu) {
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - PDE Interativo";
		
		$mensagem->AddAddress( $usu['usuemail'], $usu['usunome'] );
		
			
		$mensagem->Body = "<p>O Ministério da Educação informa que no próximo dia 04 de abril, às 10h, será realizada uma videoconferência sobre o PDE Interativo e o Plano de Formação Continuada, conectando alguns auditórios nas capitais dos estados. O evento será coordenado pelas equipe do PDE Escola e da Formação Continuada e transmitido pela internet. Para assistir, acesse o endereço http://portal.mec.gov.br/transmissao. Durante a transmissão, quem desejar enviar perguntas poderá utilizar o e-mail do PDE Escola, a saber: ".$_SESSION['email_sistema']. ".</p>
						   <p>Equipe do PDE Escola</p>";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		
	}
}


$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= SIGLA_SISTEMA. " - PDEInterativo";
$mensagem->From 		= "noreply@mec.gov.br";
$mensagem->AddAddress( $_SESSION['email_sistema'], SIGLA_SISTEMA );
$mensagem->Subject = SIGLA_SISTEMA. " - PDEInterativo";
$mensagem->Body = "Todos os e-mails dos diretores pendentes foram enviados com sucesso";
$mensagem->IsHTML( true );
$mensagem->Send();

?>
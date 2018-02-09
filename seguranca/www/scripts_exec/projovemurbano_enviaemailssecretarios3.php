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
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

// CPF do administrador de sistemas
if(!$_SESSION['usucpf']) $_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$html = "
<p>Senhor Secretário,</p>

<p>Solicitamos que acesse o Termo de Adesão do Projovem Urbano – 2012 para registrar o aceite dessa Secretaria na meta já analisada pela SECADI.</p>

<p>Informamos que somente após esse registro sua adesão será finalizada.</p>


<p>Atenciosamente,<br>
SECADI/MEC</p>";


$sql = "select usu.usunome, usu.usuemail from projovemurbano.sugestaoampliacao sug 
inner join projovemurbano.identificacaosecretario ise on ise.pjuid=sug.pjuid
inner join seguranca.usuario usu on usu.usucpf=ise.isecpf 
where suametaajustada is not null";


$dados = $db->carregar($sql);

if($dados[0]) {
	foreach($dados as $usu) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= "Programa Projovem Urbano";
		
		$mensagem->AddAddress( $usu['usuemail'], $usu['usunome'] );
		
		$mensagem->AddAddress( $_SESSION['email_sistema'] );
			
		$mensagem->Body = $html;
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		
		echo $usu['usunome']." : ".$resp."<br>";
		
	}
}
?>
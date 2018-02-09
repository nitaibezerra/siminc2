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

$html = "<p>Senhor(a) Coordenador(a),</p>
<p>Solicitamos a gentileza de consultar no Simec – Módulo Projovem Urbano o status do Plano Implementação do seu Estado ou Município.</p>
<p>Caso o Plano apresente status “em elaboração”, providencie as adequações orientadas e/ou solicitadas e devolva para a análise  final da SECADI o mais breve possível.</p>
<p>Att.<br>
Equipe Transição Projovem Urbano SECADI/MEC
</p>";


$sql = "SELECT mun.mundescricao, usu.usunome, usu.usuemail 
			FROM projovemurbano.projovemurbano prj 
			INNER JOIN territorios.municipio mun ON mun.muncod=prj.muncod
			INNER JOIN projovemurbano.coordenadorresponsavel cor ON cor.pjuid=prj.pjuid 
			INNER JOIN seguranca.usuario usu ON usu.usucpf=cor.corcpf 
			WHERE mun.muncod IS NOT NULL AND prj.adesaotermo=TRUE";


$dados = $db->carregar($sql);

if($dados[0]) {
	foreach($dados as $usu) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= "Plano de Implementação do Projovem Urbano - 2012";
		
		$mensagem->AddAddress( $usu['usuemail'], $usu['usunome'] );
		
		$mensagem->AddAddress( $_SESSION['email_sistema'] );
		
			
		$mensagem->Body = $html;
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		
		echo $usu['usunome']." : ".$resp."<br>";
		
	}
}

$sql = "SELECT est.estdescricao, usu.usunome, usu.usuemail 
		FROM projovemurbano.projovemurbano prj 
		INNER JOIN territorios.estado est ON est.estuf=prj.estuf
		INNER JOIN projovemurbano.coordenadorresponsavel cor ON cor.pjuid=prj.pjuid 
		INNER JOIN seguranca.usuario usu ON usu.usucpf=cor.corcpf 
		WHERE est.estuf IS NOT NULL AND prj.adesaotermo=TRUE";

$dados = $db->carregar($sql);

if($dados[0]) {
	foreach($dados as $usu) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= "Plano de Implementação do Projovem Urbano - 2012";
		
		$mensagem->AddAddress( $usu['usuemail'], $usu['usunome'] );

		$mensagem->AddAddress( $_SESSION['email_sistema'] );
		
			
		$mensagem->Body = $html;
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		
		echo $usu['usunome']." : ".$resp."<br>";
		
	}
}


?>
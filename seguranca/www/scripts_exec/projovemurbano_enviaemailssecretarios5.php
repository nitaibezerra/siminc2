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

$html = "<p>Gestores do Projovem Urbano,</p>
		 <p>Disponibilizamos os arquivos, em PDF, do primeiro grupo de materiais didáticos do Projovem Urbano para conhecimento dos senhores e dos formadores de sua localidade, bem como para utilização na primeira etapa de formação continuada dos educadores, que deverá ser finalizada antes do início das aulas, no dia 18 de junho de 2012.</p>
		 <p>Lembramos que os <b>materiais impressos</b> serão enviados, posteriormente, aos entes executores do Programa de acordo com o número de gestores, formadores, educadores e estudantes atendidos em cada localidade. Os entes executores deverão, assim, armazenar, zelar e distribuir os materiais impressos recebidos para os profissionais que atuarão no Programa e para os jovens matriculados.</p>
		 <p>Atenciosamente,<br/>Diretoria de Políticas de Educação para a Juventude<br/>SECADI/MEC</p>";


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
		$mensagem->Subject 		= "Projovem Urbano - 2012";
		
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
		$mensagem->Subject 		= "Projovem Urbano - 2012";
		
		$mensagem->AddAddress( $usu['usuemail'], $usu['usunome'] );
		
		$mensagem->AddAddress( $_SESSION['email_sistema'] );
			
		$mensagem->Body = $html;
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		
		echo $usu['usunome']." : ".$resp."<br>";
		
	}
}


?>
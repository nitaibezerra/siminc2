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
<p align=center><img src=\"http://simec.mec.gov.br/imagens/brasao.gif\" width=80 height=80></p>
<h2 style=text-align:center;>MINISTÉRIO DA EDUCAÇÃO</h2>
<h3 style=text-align:center;>Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão</h3>
<p align=center>Esplanada dos Ministérios, Bloco L, 2º andar - 70047-900 - Brasília, Distrito Federal, Brasil</p>
<p align=center>Gabinete: Fones: (61) 2022 9217 e 2022 9018 - Fax: (61) 2022 9020</p>

<p>Oficio Circular nº. 092 /<b>2011</b> - GAB/SECADI/MEC</p> 

<p>Brasília, 18 de novembro de 2011.</p> 

<p>A (o) Senhor (a)<br>
Secretário (a) de Educação</p>


<p>Assunto: Programa Projovem Urbano.</p>


<p>Senhor (a) Secretário (a),</p>


<p>O Programa Projovem Urbano passou a ser coordenado nacionalmente pelo Ministério da Educação, por meio da Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão – SECADI e deverá ser executado pelas Secretarias Estaduais e Municipais de Educação.</p> 
<p>Para viabilizar o início de novas turmas do Programa, que se dará em março de 2012, foi publicada no Diário Oficial da União a Resolução CD/FNDE nº. 60 de 09 de novembro de 2011, que se encontra disponibilizada no site do FNDE e segue anexada a esta mensagem.</p>
<p>Caso seja do interesse dessa Secretaria ofertar o Projovem Urbano no próximo ano, informamos que o Termo de Adesão já está liberado para as Secretarias de Educação no Sistema Integrado de Monitoramento Execução e Controle – SIMEC/MEC, <a href=\"http://www.simec.mec.gov.br/\" target=\"_blank\">www.simec.mec.gov.br</a>, no módulo Projovem Urbano.</p>

<p>Atenciosamente,</p>


<p align=center>Claudia Pereira Dutra<br>	
Secretária de Educação Continuada, Alfabetização, Diversidade e Inclusão</p>
";


$sql = "select ent.entnome as nome_secretario, 
			   ent.entemail as email_secretario, 
			   ent2.entnome as nome_secretaria, 
			   ent2.entemail as email_secretaria, 
			   ende2.* 
		from entidade.entidade ent 
		inner join entidade.funcaoentidade fen on fen.entid=ent.entid and fen.funid=15 
		inner join entidade.funentassoc fea on fea.fueid=fen.fueid 
		inner join entidade.entidade ent2 on ent2.entid=fea.entid 
		inner join entidade.funcaoentidade fen2 on fen2.entid=ent2.entid and fen2.funid=7 
		inner join entidade.endereco ende2 on ende2.entid=ent2.entid 
		inner join projovemurbano.cargameta cma on cma.cmecodibge=ende2.muncod::numeric";


$secretarios = $db->carregar($sql);

if($secretarios[0]) {
	foreach($secretarios as $sec) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= "Programa Projovem Urbano";
		
		if($sec['email_secretario']) $mensagem->AddAddress( $sec['email_secretario'], $sec['nome_secretario'] );
		if($sec['email_secretaria']) $mensagem->AddAddress( $sec['email_secretaria'], $sec['nome_secretario'] );
		
		$mensagem->AddAddress( $_SESSION['email_sistema'] );
			
		$mensagem->Body = $html;
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		
		echo $sec['nome_secretaria']." : ".$resp."<br>";
		
	}
}


$sql = "select ent.entnome as nome_secretario, 
			   ent.entemail as email_secretario, 
			   ent2.entnome as nome_secretaria, 
			   ent2.entemail as email_secretaria 
		from entidade.entidade ent 
		inner join entidade.funcaoentidade fen on fen.entid=ent.entid and fen.funid=25 
		inner join entidade.funentassoc fea on fea.fueid=fen.fueid 
		inner join entidade.entidade ent2 on ent2.entid=fea.entid 
		inner join entidade.funcaoentidade fen2 on fen2.entid=ent2.entid and fen2.funid=6";

$secretarios = $db->carregar($sql);

if($secretarios[0]) {
	foreach($secretarios as $sec) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= "Programa Projovem Urbano";
		
		if($sec['email_secretario']) $mensagem->AddAddress( $sec['email_secretario'], $sec['nome_secretario'] );
		if($sec['email_secretaria']) $mensagem->AddAddress( $sec['email_secretaria'], $sec['nome_secretario'] );
			
		$mensagem->Body = $html;
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		
		echo $sec['nome_secretaria']." : ".$resp."<br>";
		
	}
}

?>
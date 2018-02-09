<?php

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);

include_once "/var/www/simec/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "/includes/classes/Fnde_Webservice_Client.class.inc";

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);}


$db = new cls_banco();


$sql = "select u.usuemail, u.usunome
from seguranca.usuario u
inner join seguranca.perfilusuario pu using(usucpf)
inner join seguranca.perfil p 
      on p.pflcod = pu.pflcod
where p.pflcod in (472,473,474,264,267,383,384,385,386,470,471)";

$us = $db->carregar($sql);

$us[] = array("usuemail"=>$_SESSION['email_sistema'],"usunome"=>SIGLA_SISTEMA);


require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

if($us[0]) :
	foreach($us as $u) :
	
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= "Ministério da Educação - MEC";
		$mensagem->From 		= "no-reply@mec.gov.br";
		$mensagem->AddAddress( $u['usuemail'], $u['usunome'] );
		$mensagem->Subject = "Convite 32a. videoconferência do Programa Mais Educação";
		$mensagem->Body = "<p><b>32a.Videoconferência do Programa Mais Educação</b></p>
		<p>Convidamos para assistirem a 32a.Videoconferência do Programa Mais Educação, cujo tema será \"as experiências e especificidades das escolas do campo no Programa Mais Educação na construção da educação integral no Brasil\". A transmissão será via internet pelo endereço http://portal.mec.gov.br/seb/transmissao</p>  
		<p>Quando? 02 de outubro de 2012, terça-feira, das 14:30h às 17h</p> 
		<p>Onde? http://portal.mec.gov.br/seb/transmissao</p>  
		<p>Quem?<br>
		- Jaqueline Moll (Diretora de Currículo e Educação Integral do Ministério da Educação)<br>
		- Macaé Maria Evaristo dos Santos (Diretora de Políticas de Educação no Campo, Indígena e para as Relações Étnico-Raciais do Ministério da Educação)<br>
		- Danilo de Melo Souza (Secretário de Educação e Cultura do Estado do Tocantins)<br>
		- Leandro Fialho (Coordenador Geral de Educação Integral do Ministério da Educação)</p> 
		<p>Dúvidas? ".$_SESSION['email_sistema']."</p>";
		
		$mensagem->IsHTML( true );
		$mensagem->Send();
		
	endforeach;
endif;

?>
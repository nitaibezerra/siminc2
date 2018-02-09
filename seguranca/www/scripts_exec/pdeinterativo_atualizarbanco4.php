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

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select count(*), pdeid, papidentificador, max(papid) as papid_d from pdeinterativo.planoacaoproblema group by pdeid, papidentificador having count(*)>1 order by pdeid";
$paps = $db->carregar($sql);

if($paps) {
	foreach($paps as $pa) {
		$db->executar("delete from pdeinterativo.planoacaobemservico where paaid IN(select paaid from pdeinterativo.planoacaoestrategia c 
																					inner join pdeinterativo.planoacaoacao cc on cc.paeid=c.paeid 
																					where papid='".$pa['papid_d']."')");
		
		$db->executar("delete from pdeinterativo.planoacaoacao where paeid IN(select paeid from pdeinterativo.planoacaoestrategia where papid='".$pa['papid_d']."')");
		$db->executar("delete from pdeinterativo.planoacaoestrategia where papid='".$pa['papid_d']."'");
		$db->executar("delete from pdeinterativo.planoacaoproblema where papid='".$pa['papid_d']."'");
	}
}

$db->commit();


/*
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= SIGLA_SISTEMA. " - PDEInterativo";
$mensagem->From 		= "noreply@mec.gov.br";
$mensagem->AddAddress( $_SESSION['email_sistema'], SIGLA_SISTEMA );
$mensagem->Subject = "Senha - SIMEC - PDEInterativo";
$mensagem->Body = "Todos os e-mails dos diretores";
$mensagem->IsHTML( true );
$mensagem->Send();
*/

?>
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
include_once "/var/www/simec/global/config.inc";
//include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "www/pdeinterativo/_constantes.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$lista_municipal = $db->carregar("select u.usucpf, u.usunome from seguranca.usuario u 
									inner join seguranca.perfilusuario pu on pu.usucpf=u.usucpf 
									inner join seguranca.usuario_sistema us on u.usucpf = us.usucpf 
									where pu.pflcod in(460,674) and us.suscod='A' and us.sisid='23' and
									u.usucpf not in (
									
									select u.usucpf from seguranca.usuario u 
									inner join seguranca.perfilusuario pu on pu.usucpf=u.usucpf 
									inner join seguranca.usuario_sistema us on u.usucpf = us.usucpf 
									where pu.pflcod in(678) and us.suscod in('A','P') and us.sisid='98' 
									group by u.usucpf
									
									)
									group by u.usucpf, u.usunome");

if($lista_municipal[0]) {
	foreach($lista_municipal as $mun) {
		$existe = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".$mun['usucpf']."' and pflcod='678'");
		if(!$existe) {
			$sql = "insert into seguranca.perfilusuario (usucpf, pflcod) values ('".$mun['usucpf']."','678')";
			$db->executar($sql);
		}
		$existe = $db->pegaUm("select suscod from seguranca.usuario_sistema where usucpf='".$mun['usucpf']."' and sisid='98'");
		if(!$existe) {
			$sql = "insert into seguranca.usuario_sistema (usucpf, sisid, pflcod, suscod) values ('".$mun['usucpf']."', 98, 678, 'A')";
			$db->executar($sql);
		} else {
			if($existe!="B") {
				$sql = "update seguranca.usuario_sistema set suscod='A' where usucpf='".$mun['usucpf']."' and sisid=98";
				$db->executar($sql);
			}
		}
		$db->executar("update pdeinterativo.usuarioresponsabilidade set rpustatus='I' where usucpf='".$mun['usucpf']."' and pflcod in(678) and rpustatus='A'");
		if(!$existe) {
			$sql = "insert into pdeinterativo.usuarioresponsabilidade(
            		pflcod, usucpf, rpustatus, rpudata_inc, muncod)
            		select distinct 678, usucpf, 'A', now(), muncod 
            		from par.usuarioresponsabilidade 
            		where usucpf='".$mun['usucpf']."' and rpustatus='A' and pflcod in(460,674)";
			$db->executar($sql);
		}
		
		$html .= "ADD ".$mun['usunome']."<br>";
			
	}
}


$db->commit();

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "Atualizando usuários - PDEInterativo";
$mensagem->From 		= "noreply@mec.gov.br";
$mensagem->AddAddress( $_SESSION['email_sistema'], SIGLA_SISTEMA );
$mensagem->Subject = "Atualizando usuários - PDEInterativo";
$mensagem->Body = "Novos usuários do PAR foram cadastrados no PDEInterativo (".(($lista_municipal[0])?count($lista_municipal):"0")." usuários)<br><br>".$html;
$mensagem->IsHTML( true );
$mensagem->Send();

?>
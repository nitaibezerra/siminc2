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

$microtime = getmicrotime();


// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$lista = $db->carregar("select 
					  	u.usucpf,
						us.suscod as sit_par,
						pu.pflcod
						from seguranca.usuario u 
						inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and us.sisid=23
						inner join seguranca.perfilusuario pu on pu.usucpf = u.usucpf and pu.pflcod in(672,674) 
where us.suscod='A'");

$_DEPARA_PERFIL = array("674"=> array("pflcod"=>"1127","campo"=>"muncod"),
						"672"=> array("pflcod"=>"1126","campo"=>"estuf")
						);
						


if($lista[0]) {
	foreach($lista as $li) {
		
		$atualizacao  = true;
		$espelhamento = true;
		
		$existe = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".$li['usucpf']."' and pflcod='".$_DEPARA_PERFIL[$li['pflcod']]['pflcod']."'");
		
		
		if($espelhamento) {
		
			if(!$existe) {
				
				$sql = "insert into seguranca.perfilusuario (usucpf, pflcod) values ('".$li['usucpf']."','".$_DEPARA_PERFIL[$li['pflcod']]['pflcod']."')";
				$db->executar($sql);
				$em .= "CPF(".$li['usucpf'].") foi inserido no perfil(".$_DEPARA_PERFIL[$li['pflcod']]['pflcod'].")<br>";
				$atualizacao = true;
			}
			
			$existe = $db->pegaUm("select suscod from seguranca.usuario_sistema where usucpf='".$li['usucpf']."' and sisid='181'");
			
			if(!$existe) {
				
				$sql = "insert into seguranca.usuario_sistema (usucpf, sisid, pflcod, suscod) values ('".$li['usucpf']."', 181, NULL, '".$li['sit_par']."')";
				$db->executar($sql);
				$em .= "CPF(".$li['usucpf'].") foi inserido no sistema sispacto com suscod: (".$li['sit_par'].")<br>";
				$atualizacao = true;
				
			} else {
				if($existe != $li['sit_par']) {
					
					$sql = "update seguranca.usuario_sistema set suscod='".$li['sit_par']."' where usucpf='".$li['usucpf']."' and sisid=181";
					$db->executar($sql);
					$em .= "CPF(".$li['usucpf'].") foi atualizado no sistema sispacto com suscod: (".$li['sit_par'].")<br>";
					
					$sql = "INSERT INTO seguranca.historicousuario(
	            			htudsc, htudata, usucpf, sisid, suscod)
	    					VALUES ('Script automático identificou diferença PAR(status ".$li['sit_par'].") e SISPACTO 2014(status ".$existe.") e espelhou a situação do PAR no SISPACTO', NOW(), '".$li['usucpf']."', 181, '".$li['sit_par']."');";
					$db->executar($sql);
					$atualizacao = true;
					
				}
			}
			
			if($atualizacao) {
				$atri = $db->carregar("SELECT * FROM par.usuarioresponsabilidade WHERE rpustatus='A' AND usucpf='".$li['usucpf']."' AND pflcod='".$li['pflcod']."'");
				
				$db->executar("DELETE FROM sispacto2.usuarioresponsabilidade WHERE usucpf='".$li['usucpf']."' AND pflcod='".$_DEPARA_PERFIL[$li['pflcod']]['pflcod']."'");
				
				if($atri[0]) {
					foreach($atri as $at) {
						$sql = "INSERT INTO sispacto2.usuarioresponsabilidade(
		            			pflcod, usucpf, rpustatus, rpudata_inc, ".$_DEPARA_PERFIL[$li['pflcod']]['campo'].")
		    					VALUES ('".$_DEPARA_PERFIL[$li['pflcod']]['pflcod']."', '".$li['usucpf']."', 'A', NOW(), '".$at[$_DEPARA_PERFIL[$li['pflcod']]['campo']]."');";
						$db->executar($sql);
						$em .= "CPF(".$li['usucpf'].") foi inserido atribução no campo ".$_DEPARA_PERFIL[$li['pflcod']]['campo']." => ".$at[$_DEPARA_PERFIL[$li['pflcod']]['campo']].", do perfil ".$_DEPARA_PERFIL[$li['pflcod']]['pflcod'].", do sistema sispacto<br>";
					}			
				}
			}
		
		}
			
	}
}


$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='sispacto2_sincronizar_usuarios_par.php'";
$db->executar($sql);

$db->commit();

$db->close();

/*
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "Atualizando usuários - SISPACTO";
$mensagem->From 		= "noreply@mec.gov.br";
$mensagem->AddAddress( $_SESSION['email_sistema'], SIGLA_SISTEMA );
$mensagem->Subject = "Atualizando usuários - SISPACTO";
$mensagem->Body = "Espelhamento de usuários<br><br>".$em;
$mensagem->IsHTML( true );
$mensagem->Send();
*/

?>
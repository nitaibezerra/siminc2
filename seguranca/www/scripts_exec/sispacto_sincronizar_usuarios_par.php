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

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$lista = $db->carregar("select 
					  	u.usucpf,
						us.suscod as sit_par,
						pu.pflcod
						from seguranca.usuario u 
						inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and us.sisid=23
						inner join seguranca.perfilusuario pu on pu.usucpf = u.usucpf and pu.pflcod in(460,461,672,674)");

$_DEPARA_PERFIL = array("460"=> array("pflcod"=>"833","campo"=>"muncod"),
						"461"=> array("pflcod"=>"834","campo"=>"estuf"),
						"674"=> array("pflcod"=>"836","campo"=>"muncod"),
						"672"=> array("pflcod"=>"837","campo"=>"estuf")
						);
						


if($lista[0]) {
	foreach($lista as $li) {
		
		$atualizacao  = false;
		$espelhamento = true;
		
		$existe = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".$li['usucpf']."' and pflcod='".$_DEPARA_PERFIL[$li['pflcod']]['pflcod']."'");
		
		$existe_coordenadorlocal = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".$li['usucpf']."' and pflcod='826'");
		
		if($li['pflcod']==460 || $li['pflcod']==461) {
			if($existe_coordenadorlocal) {
				$espelhamento = false;
				$sql = "DELETE FROM seguranca.perfilusuario WHERE pflcod='".$_DEPARA_PERFIL[$li['pflcod']]['pflcod']."' AND usucpf='".$li['usucpf']."'";
				$db->executar($sql);
				$sql = "SELECT count(*) FROM seguranca.perfilusuario p INNER JOIN seguranca.perfil pp ON pp.pflcod = p.pflcod WHERE pp.sisid='142' AND p.usucpf='".$li['usucpf']."'";
				$possuip = $db->pegaUm($sql);
				if($possuip==0) {
					$sql = "DELETE FROM seguranca.usuario_sistema WHERE sisid='142' AND usucpf='".$li['usucpf']."'";
					$db->executar($sql);
				}
				
				echo "Removendo Consulta, possui coordenador local, (".$li['usucpf'].") do sistema SISPACTO<br>";
			}
			
			if($li['sit_par']=='B' || $li['sit_par']=='P') {
				$espelhamento = false;
				$sql = "DELETE FROM seguranca.perfilusuario WHERE pflcod='".$_DEPARA_PERFIL[$li['pflcod']]['pflcod']."' AND usucpf='".$li['usucpf']."'";
				$db->executar($sql);
				$sql = "SELECT count(*) FROM seguranca.perfilusuario p INNER JOIN seguranca.perfil pp ON pp.pflcod = p.pflcod WHERE pp.sisid='142' AND p.usucpf='".$li['usucpf']."'";
				$possuip = $db->pegaUm($sql);
				if($possuip==0) {
					$sql = "DELETE FROM seguranca.usuario_sistema WHERE sisid='142' AND usucpf='".$li['usucpf']."'";
					$db->executar($sql);
				}
				
				echo "Removendo Consulta, bloqueado no par, (".$li['usucpf'].") do sistema SISPACTO<br>";
				
			}
		}
		
		if(($li['pflcod']==674 || $li['pflcod']==672) && ($li['sit_par']=='B' || $li['sit_par']=='P')) {
			
			$sql = "SELECT COUNT(DISTINCT u.usucpf) FROM seguranca.usuario u 
					INNER JOIN seguranca.usuario_sistema uu ON uu.usucpf = u.usucpf 
					INNER JOIN seguranca.perfilusuario p ON p.usucpf = u.usucpf 
					INNER JOIN sispacto.usuarioresponsabilidade ur ON ur.usucpf = u.usucpf AND p.pflcod = ur.pflcod 
					WHERE uu.sisid=142 AND ur.pflcod='".$_DEPARA_PERFIL[$li['pflcod']]['pflcod']."' AND ur.muncod IN(SELECT DISTINCT muncod FROM par.usuarioresponsabilidade WHERE rpustatus='A' AND usucpf='".$li['usucpf']."' AND pflcod='".$li['pflcod']."') AND uu.suscod='A'";
			
			$numero = $db->pegaUm($sql);
			
			$espelhamento = false;
						
			if($numero > 0) {
				$sql = "DELETE FROM seguranca.perfilusuario WHERE pflcod='".$_DEPARA_PERFIL[$li['pflcod']]['pflcod']."' AND usucpf='".$li['usucpf']."'";
				$db->executar($sql);
				$sql = "SELECT count(*) FROM seguranca.perfilusuario p INNER JOIN seguranca.perfil pp ON pp.pflcod = p.pflcod WHERE pp.sisid='142' AND p.usucpf='".$li['usucpf']."'";
				$possuip = $db->pegaUm($sql);
				if($possuip==0) {
					$sql = "DELETE FROM seguranca.usuario_sistema WHERE sisid='142' AND usucpf='".$li['usucpf']."'";
					$db->executar($sql);
				}
				
				echo "Removendo Dirigentes(".$li['usucpf'].") do sistema SISPACTO<br>";
			}
			
		}
		
		
		if($espelhamento) {
		
			if(!$existe) {
				$sql = "insert into seguranca.perfilusuario (usucpf, pflcod) values ('".$li['usucpf']."','".$_DEPARA_PERFIL[$li['pflcod']]['pflcod']."')";
				$db->executar($sql);
				$em .= "CPF(".$li['usucpf'].") foi inserido no perfil(".$_DEPARA_PERFIL[$li['pflcod']]['pflcod'].")<br>";
				$atualizacao = true;
			}
			
			$existe = $db->pegaUm("select suscod from seguranca.usuario_sistema where usucpf='".$li['usucpf']."' and sisid='142'");
			
			if(!$existe) {
				$sql = "insert into seguranca.usuario_sistema (usucpf, sisid, pflcod, suscod) values ('".$li['usucpf']."', 142, NULL, '".$li['sit_par']."')";
				$db->executar($sql);
				$em .= "CPF(".$li['usucpf'].") foi inserido no sistema sispacto com suscod: (".$li['sit_par'].")<br>";
				$atualizacao = true;
			} else {
				if($existe != $li['sit_par']) {
					$sql = "update seguranca.usuario_sistema set suscod='".$li['sit_par']."' where usucpf='".$li['usucpf']."' and sisid=142";
					$db->executar($sql);
					$em .= "CPF(".$li['usucpf'].") foi atualizado no sistema sispacto com suscod: (".$li['sit_par'].")<br>";
					
					$sql = "INSERT INTO seguranca.historicousuario(
	            			htudsc, htudata, usucpf, sisid, suscod)
	    					VALUES ('Script automático identificou diferença PAR(status ".$li['sit_par'].") e SISPACTO(status ".$existe.") e espelhou a situação do PAR no SISPACTO', NOW(), '".$li['usucpf']."', 142, '".$li['sit_par']."');";
					$db->executar($sql);
					$atualizacao = true;
				}
			}
			
			if($atualizacao) {
				$atri = $db->carregar("SELECT * FROM par.usuarioresponsabilidade WHERE rpustatus='A' AND usucpf='".$li['usucpf']."' AND pflcod='".$li['pflcod']."'");
				
				$db->executar("DELETE FROM sispacto.usuarioresponsabilidade WHERE usucpf='".$li['usucpf']."' AND pflcod='".$_DEPARA_PERFIL[$li['pflcod']]['pflcod']."'");
				
				if($atri[0]) {
					foreach($atri as $at) {
						$sql = "INSERT INTO sispacto.usuarioresponsabilidade(
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

$db->commit();

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

?>
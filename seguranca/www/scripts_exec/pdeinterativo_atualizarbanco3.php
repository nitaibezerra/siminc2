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

$emails = file("emails_sem_emails.txt");

foreach($emails as $e) {
	$emails_limpos[] = trim($e);
}

$sql = "SELECT usunome, usuemail FROM seguranca.usuario u
		LEFT JOIN seguranca.perfilusuario pu ON pu.usucpf=u.usucpf 
		LEFT JOIN seguranca.perfil pp ON pp.pflcod = pu.pflcod  
		WHERE sisid=98 and usuemail IN('".implode("','",$emails_limpos)."') 
		GROUP BY usunome, usuemail";

$usuarios = $db->carregar($sql);

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

/*
if($usuarios[0]) {
	foreach($usuarios as $u) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= "Problemas com senha - SIMEC - PDE Interativo";
		
		$mensagem->AddAddress( $u['usuemail'], $u['usunome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$u['usunome']},</p>
						   <p>O SIMEC identificou um problema em sua senha de accesso. A senha atual foi alterada para \"escola\".</p>
						   <p>Cordialmente, SIMEC/MEC</p>";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		
		echo $resp."<br>";
		
	}
}

$sql = "select count(pesid), pflcod, pdeid, max(pesid) as pes_atual 
		from pdeinterativo.pessoa 
		where pflcod=544 and pesstatus='A' and pdeid is not null 
		group by pflcod, pdeid 
		having count(pesid)>1";

$regs = $db->carregar($sql);

if($regs[0]) {
	foreach($regs as $re) {
		$sql = "update pdeinterativo.pessoa set pflcod=NULL where pflcod=544 and pdeid=".$re['pdeid']." and pesid!=".$re['pes_atual'];
		$db->executar($sql);
		$sql = "delete from pdeinterativo.pessoatipoperfil where tpeid=2 and pesid in(select pesid from pdeinterativo.pessoa where pdeid=".$re['pdeid']." and pesid!=".$re['pes_atual'].")";
		$db->executar($sql);
	}
}

$db->commit();
*/

$sql = "select pde.pdeid, pfl.pfldsc, count(usu.usucpf) from 
				seguranca.usuario usu
				left join
					pdeinterativo.pessoa pes ON pes.usucpf = usu.usucpf AND pesstatus <> 'I'
				left join
					seguranca.perfilusuario per ON per.usucpf = usu.usucpf 
				left join
					pdeinterativo.pdinterativo pde ON pde.pdeid = pes.pdeid AND pdistatus = 'A' 
				left join
					seguranca.perfil pfl ON pfl.pflcod = per.pflcod
				inner join
					seguranca.usuario_sistema ususis ON usu.usucpf = ususis.usucpf and ususis.susstatus = 'A' and ususis.sisid = 98
				where 
					pde.pdeid is not null and per.pflcod in (544) and pfl.sisid  = 98
				group by pde.pdeid, pfl.pfldsc
				having count(usu.usucpf)>1";

$rows = $db->carregar($sql);

if($rows[0]) {
	foreach($rows as $r) {
		$sql = "select p.pesid from pdeinterativo.pessoa p 
				left join pdeinterativo.pessoatipoperfil pt on p.pesid = pt.pesid 
				where p.pdeid=".$r['pdeid']." and pt.tpeid=2";
		
		$cordenador_correto = $db->pegaUm($sql);
		
		$sql = "select usu.usucpf from 
						seguranca.usuario usu
						left join
							pdeinterativo.pessoa pes ON pes.usucpf = usu.usucpf AND pesstatus <> 'I'
						left join
							seguranca.perfilusuario per ON per.usucpf = usu.usucpf 
						left join
							pdeinterativo.pdinterativo pde ON pde.pdeid = pes.pdeid AND pdistatus = 'A' 
						left join
							seguranca.perfil pfl ON pfl.pflcod = per.pflcod
						inner join
							seguranca.usuario_sistema ususis ON usu.usucpf = ususis.usucpf and ususis.susstatus = 'A' and ususis.sisid = 98
						where 
							pde.pdeid='".$r['pdeid']."' and per.pflcod in (544) and pfl.sisid  = 98 and pes.pesid!='".$cordenador_correto."'";
		$coord_remover = $db->carregar($sql);
		if($coord_remover[0]) {
			foreach($coord_remover as $cc) {
				$sql = "DELETE FROM seguranca.perfilusuario WHERE pflcod=544 and usucpf='".$cc['usucpf']."'";
				$db->executar($sql);
				$sql = "DELETE FROM seguranca.usuario_sistema WHERE pflcod=544 and usucpf='".$cc['usucpf']."'";
				$db->executar($sql);
			}
		}
		
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
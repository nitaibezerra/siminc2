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
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


$sql = "SELECT usunome, usuemail, pdenome as escola, percent FROM (
			SELECT DISTINCT
							pdicodinep,
							pdenome,
							pdiesfera,
							mun.mundescricao,
							mun.estuf,
							usu.usucpf,
							usu.usunome,
							usu.usuemail,
							CASE WHEN esddsc IS NOT NULL THEN esddsc ELSE 'Sem documento' END as estado,
							((CASE WHEN 
								(select count(distinct abaresp.abaid) from pdeinterativo.abaresposta abaresp where abaresp.pdeid = pde.pdeid) > 0
									THEN round(((select count(distinct abaresp.abaid) from pdeinterativo.abaresposta abaresp where abaresp.pdeid = pde.pdeid)::numeric(10,2) /(select count(distinct abaid) from pdeinterativo.aba where (abatipo != 'O' or abatipo is null) and abaidpai is not null and abaid not in (2,3,4,5,6,7,8,54))::numeric(10,2))*100,0)
									ELSE 0
							END)
							) as percent
						FROM
							seguranca.usuario usu
						INNER JOIN
							pdeinterativo.pessoa pes ON pes.usucpf = usu.usucpf AND pesstatus = 'A'
						INNER JOIN
							seguranca.usuario_sistema ususis ON ususis.usucpf = pes.usucpf
						INNER JOIN
							pdeinterativo.pdinterativo pde ON pde.pdeid = pes.pdeid AND pdistatus = 'A'
						LEFT JOIN
							territorios.municipio mun ON pde.muncod = mun.muncod 
						LEFT JOIN 
							workflow.documento doc ON doc.docid = pde.docid 
						LEFT JOIN 
							workflow.estadodocumento esd ON esd.esdid = doc.esdid  
						WHERE
							pes.pflcod is not null
						 AND 
						 	ususis.sisid = 98
						 AND 
						 	ususis.suscod = 'A'
		) foo 
		WHERE percent!=100 limit 5";



$pendentes = $db->carregar($sql);

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


if($pendentes[0]) {
	foreach($pendentes as $pend) {
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - PDE Interativo";
		
		$mensagem->AddAddress( $pend['usuemail'], $pend['usunome'] );
		
		if($pend['percent']=="0") {
			
			$mensagem->Body = "<p>Prezado(a) Diretor(a) {$pend['usunome']},</p>
							   <p>Identificamos que Vossa Senhoria foi cadastrado(a) no PDE Interativo e, junto com a equipe escolar, pode elaborar o PDE Escola 2011. Para que a escola possa receber os recursos ainda este ano (mesmo que só consiga utilizá-los em 2012), é necessário enviar o plano para o Ministério da Educação até o dia 30/11/2011. Para isso, é necessário acessar o SIMEC (http://simec.mec.gov.br) com o seu CPF e a senha enviada para o e-mail cadastrado.</p>
							   <p>Caso tenha esquecido a senha ou a mesma não seja mais válida, por favor, entre em contato com a sua Secretaria de Educação, que é o órgão responsável pelo cadastramento dos(as) diretores(as). Se estiver encontrando problemas no SIMEC, envie um e-mail para ".$_SESSION['email_sistema']. ". Caso deseje obter mais informações sobre o PDE Interativo, acesse o site http://pdeescola.mec.gov.br. Não perca esta chance de aprimorar a gestão e melhorar os resultados da sua escola! Boa sorte!</p>";
			
		} else {
			
			$mensagem->Body = "<p>Prezado(a) Diretor(a) {$pend['usunome']},</p>
							   <p>Sua escola ({$pend['escola']}) esta inscrita para elaboração do PDE Interativo, mas é necessário concluir o Diagnóstico e elaborar os Planos de Ação. Por favor, acesse o SIMEC(<a href=simec.mec.gov.br target=_blank>simec.mec.gov.br</a>) e conclua o PDE da sua escola.</p>
							   <p>Até o momento a escola concluiu <b>{$pend['percent']} %</b> do PDE.</p>
							   <p><font size=1><i>Esta é uma mensagem automática, por favor não responda.</i></font></p>";
			
		}
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		
		echo $resp."<br>";
		
	}
}


$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= SIGLA_SISTEMA. " - PDEInterativo";
$mensagem->From 		= "noreply@mec.gov.br";
$mensagem->AddAddress( $_SESSION['email_sistema'], SIGLA_SISTEMA );
$mensagem->Subject = SIGLA_SISTEMA. " - PDEInterativo";
$mensagem->Body = "Todos os e-mails dos diretores pendentes foram enviados com sucesso";
$mensagem->IsHTML( true );
$mensagem->Send();

?>
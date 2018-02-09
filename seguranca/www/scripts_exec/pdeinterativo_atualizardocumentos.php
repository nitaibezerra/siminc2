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
include_once APPRAIZ . "www/pdeinterativo/_constantes.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

$servidor_bd = '';
$porta_bd = '5432';
$nome_bd = '';
$usuario_db = '';
$senha_bd = '';


// abre conexão com o servidor de banco de dados
$db = new cls_banco();


$sql = "SELECT d.docid, p.pdicodinep, p.pdeano, p.pdeid, d.esdid 
		FROM pdeinterativo.pdinterativo p 
		LEFT JOIN workflow.documento d ON d.docid = p.docid 
		WHERE d.docid IS NULL AND entid is not null";

$documentos = $db->carregar($sql);

$esdid = $db->pegaUm("SELECT esdid FROM workflow.estadodocumento WHERE tpdid='".TPD_WF_FLUXO."' AND esdordem='1'");

if($documentos[0]) {
	foreach($documentos as $doc) {
		$docid = $db->pegaUm("INSERT INTO workflow.documento(
		            		  tpdid, esdid, docdsc, docdatainclusao)
		    				  VALUES ('".TPD_WF_FLUXO."', '".$esdid."', 'PDE Interativo ".$doc['pdicodinep']."/".(($doc['pdeano'])?$doc['pdeano']:"XXXX")." ".$doc['pdeid']."', NOW()) RETURNING docid;");
		$db->executar("UPDATE pdeinterativo.pdinterativo SET docid='".$docid."' WHERE pdeid='".$doc['pdeid']."'");
		$db->commit();
	}
}


// atualizar valores
$db->executar("update pdeinterativo.planoacaoacao set paacustototal=foo2.total from (

				select * from (
				
				SELECT paa.paaid, paacustototal, SUM(pab.pabvalorcapital) as ca, SUM(pab.pabvalorcusteiro) as cu, SUM(coalesce(pab.pabvalorcapital,0))+SUM(coalesce(pab.pabvalorcusteiro,0)) as total 
								FROM pdeinterativo.planoacaoproblema pap 
								INNER JOIN pdeinterativo.planoacaoestrategia pae ON pae.papid = pap.papid 
								INNER JOIN pdeinterativo.planoacaoacao paa ON paa.paeid = pae.paeid 
								INNER JOIN pdeinterativo.planoacaobemservico pab ON pab.paaid = paa.paaid 
								LEFT JOIN pdeinterativo.categoriaitemacao cia ON cia.ciaid = pab.ciaid 
								LEFT JOIN pdeinterativo.unidadereferencia ure ON ure.ureid = cia.ureid 
								LEFT JOIN pdeinterativo.categoriaacao cac ON cac.cacid = cia.cacid
								WHERE pab.pabstatus='A' AND paa.paastatus='A' AND pae.paestatus='A' AND pap.papstatus='A'
								GROUP BY paa.paaid, paacustototal
				
								) as foo
				where total!=paacustototal) as foo2 
				
				where planoacaoacao.paaid=foo2.paaid");

$db->commit();

$db->executar("UPDATE pdeinterativo.relatorio_saldo s
				SET rlsprimeiraparcela=foo.primeira, rlssegundaparcela=foo.segunda, 
				       rlstotalprimeiraparcela=foo.totalprimeira, rlstotalsegundaparcela=foo.totalsegunda
				FROM ( 
				 select 
				(SELECT SUM(pabvalorcapital)+SUM(pabvalorcusteiro) as totalp
										FROM pdeinterativo.planoacaobemservico pab 
										INNER JOIN pdeinterativo.planoacaoacao paa ON paa.paaid = pab.paaid
										INNER JOIN pdeinterativo.planoacaoestrategia pae ON pae.paeid = paa.paeid 
										INNER JOIN pdeinterativo.planoacaoproblema pap ON pap.papid = pae.papid 
										WHERE pab.pabparcela='P' AND pap.pdeid=pp.pdeid AND pabstatus='A' AND papstatus='A' AND paestatus='A' AND paastatus='A') as primeira,
				(SELECT SUM(pabvalorcapital)+SUM(pabvalorcusteiro) as totals
										FROM pdeinterativo.planoacaobemservico pab 
										INNER JOIN pdeinterativo.planoacaoacao paa ON paa.paaid = pab.paaid
										INNER JOIN pdeinterativo.planoacaoestrategia pae ON pae.paeid = paa.paeid 
										INNER JOIN pdeinterativo.planoacaoproblema pap ON pap.papid = pae.papid 
										WHERE pab.pabparcela='S' AND pap.pdeid=pp.pdeid AND pabstatus='A' AND papstatus='A' AND paestatus='A' AND paastatus='A') as segunda,
				(select ccccapitalprimeira+ccccusteioprimeira FROM pdeinterativo.cargacapitalcusteio WHERE codinep=pp.pdicodinep::integer and cccstatus='A' limit 1) as totalprimeira,
				(select ccccapitalsegunda+ccccusteiosegunda FROM pdeinterativo.cargacapitalcusteio WHERE codinep=pp.pdicodinep::integer and cccstatus='A' limit 1) as totalsegunda,
				pp.pdeid
				from pdeinterativo.pdinterativo pp where pdistatus='A' AND pditempdeescola=TRUE
				) foo WHERE s.pdeid=foo.pdeid");

$db->commit();

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= SIGLA_SISTEMA. " - PDEInterativo";
$mensagem->From 		= "noreply@mec.gov.br";
$mensagem->AddAddress( $_SESSION['email_sistema'], SIGLA_SISTEMA );
$mensagem->Subject = SIGLA_SISTEMA. " - PDEInterativo";
$mensagem->Body = "Todas as escolas foram atualizados. (".count($documentos)." foram atualizados)";
$mensagem->IsHTML( true );
$mensagem->Send();

?>

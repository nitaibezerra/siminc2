<?php
ini_set("memory_limit", "3000M");
set_time_limit(30000);

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "/var/www/simec/global/config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$e = new EmailAgendado();
$e->setTitle("Nota do Sistema - ". SIGLA_SISTEMA);
//$e->setEmailsDestinoPorArquivo(APPRAIZ."www/painel/emailsDaniel.txt");

$sql = "SELECT DISTINCT
			prc.prcnumsidoc as processo,
			to_char(now()-ultima_tramitacao,'DD')::integer as dias,
			ent.entnome as nome,
			ent.entemail as email
		FROM
			conjur.processoconjur prc
		INNER JOIN conjur.estruturaprocesso esp ON esp.prcid = prc.prcid
		INNER JOIN workflow.documento doc ON doc.docid = esp.docid
		INNER JOIN
		(
			SELECT max(htddata) as ultima_tramitacao, hst.docid
			FROM workflow.historicodocumento hst
			GROUP BY hst.docid
		) as last_hst ON last_hst.docid = doc.docid
		INNER JOIN
		(
			SELECT max(hadid) as hadid, prcid 
			FROM conjur.historicoadvogados
			GROUP BY prcid
		) as last_advhst ON last_advhst.prcid = prc.prcid
		INNER JOIN conjur.historicoadvogados had ON had.hadid = last_advhst.hadid
		INNER JOIN conjur.advogados adv ON adv.advid = had.advid
		INNER JOIN entidade.entidade ent ON ent.entid = adv.entid
		WHERE
			doc.esdid = 382 
			AND to_char(now()-ultima_tramitacao,'DD')::integer > 10 
			AND prc.prcstatus = 'A'
		ORDER BY
			dias DESC";
$atrazados = $db->carregar($sql);

foreach( $atrazados as $processo){
	$html = '<div style="font: 12pt Arial,verdana" ><center><b><span style="color:red" >NOTA do SISTEMA - SIMEC!</span></b><br /><br />
	Sr(a). Advogado(a) '.$processo['nome'].',<br /> o Processo "'.$processo['processo'].'" encontra-se sob sua análise há "'.$processo['dias'].'" dias
	</center><br /><br />
	Atenciosamente,<br /><br />
	Equipe '. SIGLA_SISTEMA. '<br /><br />
	Obs.: Este é um email automático enviado pelo sistema, favor não responder.</div>';
	echo $html;
	$e->setText($html);
	$e->setName(SIGLA_SISTEMA. " - Ministério da Educação");
	$e->setEmailOrigem("no-reply@mec.gov.br");
	$arrEmail = Array($processo['email']);
//	$arrEmail[] = $_SESSION['email_sistema'];
	$e->limpaEmailsDestino();
	$e->setEmailsDestino($arrEmail);
	$e->enviarEmails();
}

$db->close();

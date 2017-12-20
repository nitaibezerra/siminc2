<?php
ini_set("memory_limit", "3000M");
set_time_limit(30000);

include_once "config.inc";
include_once "_constantes.php";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';

$sql = "
SELECT DISTINCT
				entnome,
				entnumcpfcnpj,
				usuemail,
                to_char(dmi.dmidataexecucao, 'DD/MM/YYYY') as dmidataexecucao,
                coalesce(to_char(dmi.dmidatavalidacao, 'DD/MM/YYYY'),'N/A') as dmidatavalidacao,
                atidescricao || ' - ' || mnmdsc as descricao,
                ati._atiprojeto as projeto,
                esd.esddsc as descricao_estado,
                (SELECT acadsc from painel.acao aca2 inner join pde.atividade ati2 ON ati2.atiacaid = aca2.acaid where ati2.atiid = ati._atiprojeto) as programa,
                doc.docid  as documento,
                case 
                               when doc.esdid = 443 and tpvid = 1 and dmi.dmidataexecucao <= now()::date then 'Pendências'
                               when doc.esdid = 444 and tpvid = 2 and dmi.dmidataexecucao <= now()::date then 'Pendências'
                               when doc.esdid = 445 then 'Resolvidas'
                               when dmi.dmidataexecucao > now()::date then 'Futuras'
                end as pendencia,
                case 
                               when doc.esdid = 443 and tpvid = 1 and dmi.dmidataexecucao <= now()::date then 1
                               when doc.esdid = 444 and tpvid = 2 and dmi.dmidataexecucao <= now()::date then 1
                               when doc.esdid = 445 then 2
                               when dmi.dmidataexecucao > now()::date then 3
                end as ordem
FROM
                painel.detalhemetaindicador dmi
INNER JOIN painel.detalheperiodicidade 	dpe  ON dpe.dpeid = dmi.dpeid and dpestatus = 'A'
INNER JOIN pde.monitorameta 			mnm  ON mnm.metid = dmi.metid
INNER JOIN (SELECT max(mmeid) as mmeid, mnmid FROM pde.monitorametaentidade GROUP BY mnmid) mmex  ON mmex.mnmid = mnm.mnmid
INNER JOIN pde.monitorametaentidade 	mme  ON mmex.mmeid = mme.mmeid
INNER JOIN workflow.documento 			doc  ON doc.docid = dmi.docid
INNER JOIN pde.monitoraitemchecklist 	mic  ON mic.micid = mnm.micid
INNER JOIN pde.atividade 				ati  ON ati.atiid = mic.atiid               
INNER JOIN workflow.estadodocumento 	esd  ON esd.esdid = doc.esdid
INNER JOIN entidade.entidade 			resp ON mme.entid = resp.entid
INNER JOIN seguranca.usuario 			usu  ON usu.usucpf = resp.entnumcpfcnpj
WHERE
	dmi.dmistatus= 'A'
	AND mic.micstatus = 'A'
	AND ati.atistatus = 'A'
	AND mnm.mnmstatus = 'A'
	AND doc.docid is not null
	AND dmi.dmidataexecucao <= now()::date
	AND doc.esdid in (443,444)
ORDER BY
	entnome,ordem,dmidataexecucao asc ";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

foreach($arrDados as $d){
	$arrUsu[$d['entnumcpfcnpj']]['email'] = $d['usuemail'];
	$arrUsu[$d['entnumcpfcnpj']]['projeto'] = $d['projeto'];
	$arrUsu[$d['entnumcpfcnpj']]['nome'] = $d['entnome'];
	$arrUsu[$d['entnumcpfcnpj']]['pendencia'][] = array(
													"data_execucao" => $d['dmidataexecucao'],
													"data_validacao" => $d['dmidatavalidacao'],
													"descricao" => $d['descricao'],
													"estado" => $d['descricao_estado'],
													"programa" => $d['programa'],
													"pendencia" => $d['pendencia'],
												);
}
$arrUsu = $arrUsu ? $arrUsu : array();

foreach($arrUsu as $u){
	$e = new EmailAgendado();
	$e->setTitle("Pendências - Monitoramento Estratégico - ". SIGLA_SISTEMA);
	$html = '
	Sr(a). '.$u['nome'].',<br />
	Você possui os seguintes itens na sua Caixa de Entrada do Módulo Monitoramento Estratégico:<br /><br />';
	$html.= '<table bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">';
	$html.= '<tr  bgcolor="#c5c5c5" >';
	$html.= '<td align="center" ><b>Nº</b></td><td align="center" ><b>Tipo</b></td><td align="center" ><b>Data da Meta</b></td><td align="center" ><b>Data da Validação</b></td><td align="center" ><b>Descrição</b></td><td align="center" ><b>Programa</b></td><td align="center" ><b>Situação Atual</b></td>';
	$html.= '</tr>';
	$n=1;
	foreach($u['pendencia'] as $p){
		$cor = $n%2==1 ? "#ffffff" : "";
		$html.= '<tr bgcolor="'.$cor.'" >';
		$html.= '<td>'.$n.'</td><td>'.$p['pendencia'].'</td><td>'.$p['data_execucao'].'</td><td>'.$p['data_validacao'].'</td><td>'.$p['descricao'].'</td><td>'.$p['programa'].'</td><td>'.$p['estado'].'</td>';
		$html.= '</tr>';
		$n++;
	}
	$html.= '</table><br />';
	$html.= 'Atenciosamente,<br /><br />
	Equipe '. SIGLA_SISTEMA. '<br /><br />
	Obs.: Este é um email automático enviado pelo sistema, favor não responder.</div>';
	echo $html."<br/><br/>";
	$e->setText($html);
	$e->setName(SIGLA_SISTEMA. " - Ministério da Educação");
	$e->setEmailOrigem("no-reply@mec.gov.br");
	$arrEmail = Array($u['email'],$_SESSION['email_sistema']);
	if( $u['projeto'] != '' ){
		$sql = "SELECT 
					usuemail
				FROM 
					pde.usuarioresponsabilidade ur
				INNER JOIN seguranca.usuario usu ON usu.usucpf = ur.usucpf
				WHERE
					ur.pflcod in (591,592) AND rpustatus = 'A' AND atiid = ".$u['projeto'];
		$emails = $db->carregarColuna($sql);
		foreach($emails as $email){
			$arrEmail[] = $email;
		}
	}
//	$arrEmail[] = $u['email'];
//	$arrEmail = $_SESSION['email_sistema'];
//	$e->setEmailsDestino($arrEmail);
//	$e->enviarEmails();	
}
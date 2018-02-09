<?php
/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações */

// carrega as funções gerais
include_once "/var/www/simec/global/config.inc";
//include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "pde/www/_constantes.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';

$whereProjetos = "AND (a._atinumero LIKE '26%' OR a._atinumero LIKE '18%' OR a._atinumero LIKE '19%' OR a._atinumero LIKE '24%' OR a._atinumero LIKE '25%' OR a._atinumero LIKE '28%' OR a._atinumero LIKE '37%' OR a._atinumero LIKE '38%')";

//EXECUÇÃO
$sql = "SELECT DISTINCT
			a._atinumero || ' - ' || a.atidescricao AS atividade, 
			i.iclid || ' - ' || i.icldsc AS item, 
			i.iclprazo, 
			CASE e.entnumcpfcnpj
				WHEN '11112123000174' THEN 'mokamura@modulo.com.br' --MODULO SECURITY SOLUTIONS S/A
				WHEN '42270181000116' THEN 'alvaro@cesgranrio.org.br' --FUNDACAO CESGRANRIO
				WHEN '00038174000143' THEN 'sonia.gouveia@cespe.unb.br;ac@cespe.unb.br;maria.gaspar@cespe.unb.br' --FUNDACAO UNIVERSIDADE DE BRASILIA
				WHEN '62004395000158' THEN 'andre.vissirini@rrd.com' --RR DONNELLEY MOORE EDITORA E GRAFICA LTDA
				WHEN '34028316000707' THEN 'rodrigorovai@correios.com.br' --EMPRESA BRASILEIRA DE CORREIOS E TELEGRAFOS
			ELSE usu.usuemail
			END AS emailusuario,
			CASE e.entnumcpfcnpj
				WHEN '11112123000174' THEN 'MARCOS SATORU OKAMURA' --MODULO SECURITY SOLUTIONS S/A
				WHEN '42270181000116' THEN 'ÁLVARO FREITAS' --FUNDACAO CESGRANRIO
				WHEN '00038174000143' THEN 'CESPE' --FUNDACAO UNIVERSIDADE DE BRASILIA
				--WHEN '00038174000143' THEN 'SONIA OLESKO' --FUNDACAO UNIVERSIDADE DE BRASILIA
				WHEN '62004395000158' THEN 'ANDRE AVILA VISSIRINI' --RR DONNELLEY MOORE EDITORA E GRAFICA LTDA
				WHEN '34028316000707' THEN 'RODRIGO VENEZIAN ROVAI' --EMPRESA BRASILEIRA DE CORREIOS E TELEGRAFOS
			ELSE usu.usunome
			END AS nomeusuario, 
			usudiretor.usuemail AS emaildiretor, 
			usudiretor.usunome AS nomediretor,
			a._atiordem, i.iclordem, e.entnumcpfcnpj, i.iclid
		FROM pde.itemchecklist i
		INNER JOIN pde.atividade a ON a.atiid = i.atiid
		INNER JOIN pde.checklistentidade ce ON ce.iclid = i.iclid AND ce.tpvid = 1
		INNER JOIN entidade.entidade e ON e.entid = ce.entid
		LEFT JOIN seguranca.usuario usu ON usu.usucpf = e.entnumcpfcnpj
		LEFT JOIN pde.enemservidor es ON es.usucpf = usu.usucpf
		LEFT JOIN pde.enemdiretor ed ON ed.ediid = es.ediid
		LEFT JOIN seguranca.usuario usudiretor ON usudiretor.usucpf = ed.usucpf
		WHERE a._atiprojeto = 114098
		$whereProjetos
		AND i.iclid NOT IN (SELECT DISTINCT v.iclid FROM pde.validacao v WHERE v.tpvid = 1)
		AND i.iclprazo IS NOT NULL
		AND CASE WHEN e.entnumcpfcnpj IN ('11112123000174', '42270181000116', '00038174000143', '62004395000158', '34028316000707') THEN 'SIM' ELSE usu.usunome END IS NOT NULL
		ORDER BY i.iclprazo, a._atiordem, i.iclordem";
$arrDados1 = $db->carregar($sql);
$contador = 0;
$itensCheckList = "";
if($arrDados1){
	foreach($arrDados1 as $dado){
		if($contador == 0){
			$itensCheckList = $dado['iclid'];
		}else{
			$itensCheckList = $itensCheckList . ', ' . $dado['iclid'];
		}
		if(date('Y-m-d') == $dado['iclprazo'] - 3){
			$newDate = date("d/m/Y", strtotime($dado['iclprazo']));
			$e = new EmailAgendado();
			$e->setTitle(SIGLA_SISTEMA. " - Exames MEC / INEP - EXECUÇÃO - Faltam 72 horas para vencer o prazo");
			$html = 'Prezado(a) senhor (a) '.$dado['nomeusuario'].',<br>
			<p>Informamos que a execução da atividade <b>'.$dado['atividade'].'</b>, item <b>'.$dado['item'].'</b> no SIMEC deve ser cumprida até o dia ' . $newDate . '.</p>';
			$e->setText($html);
			$e->setName(SIGLA_SISTEMA. " - Ministério da Educação");
			$e->setEmailOrigem("no-reply@mec.gov.br");
			$arrEmail = Array($dado['emailusuario']);
			//$arrEmail = Array($_SESSION['email_sistema']);
			$e->setEmailsDestino($arrEmail);
			$e->enviarEmails();	
		}elseif(date('Y-m-d') == $dado['iclprazo']){
			$e = new EmailAgendado();
			$e->setTitle(SIGLA_SISTEMA. " - Exames MEC / INEP - EXECUÇÃO - O prazo termina hoje");
			$html = 'Prezado(a) senhor (a) '.$dado['nomeusuario'].',<br>
			<p>Reitaramos que a execução da atividade <b>'.$dado['atividade'].'</b>, item <b>'.$dado['item'].'</b> no SIMEC, sob sua responsabilidade, deve ser cumprida até o dia de hoje.</p>';
			$e->setText($html);
			$e->setName(SIGLA_SISTEMA. " - Ministério da Educação");
			$e->setEmailOrigem("no-reply@mec.gov.br");
			$arrEmail = Array($dado['emailusuario']);
			//$arrEmail = Array($_SESSION['email_sistema']);
			$e->setEmailsDestino($arrEmail);
			$e->enviarEmails();	
		}elseif(date('Y-m-d') > $dado['iclprazo']){
			$e = new EmailAgendado();
			$e->setTitle(SIGLA_SISTEMA. " - Exames MEC / INEP - EXECUÇÃO - O prazo venceu");
			$html = 'Prezado(a) senhor (a) '.$dado['nomeusuario'].',<br>
			<p>Reiteramos que a execução da atividade <b>'.$dado['atividade'].'</b>, item <b>'.$dado['item'].'</b> no SIMEC, encontra-se em atraso, razão pela qual se faz necessário o emprego de diligências para sanear o procedimento.</p>';
			$e->setText($html);
			$e->setName(SIGLA_SISTEMA. " - Ministério da Educação");
			$e->setEmailOrigem("no-reply@mec.gov.br");
			if($dado['emaildiretor']){
				$arrEmail = Array($dado['emailusuario'],$dado['emaildiretor']);
			}else{
				$arrEmail = Array($dado['emailusuario']);
			}
			//$arrEmail = Array($_SESSION['email_sistema']);
			$e->setEmailsDestino($arrEmail);
			$e->enviarEmails();
		}
		$contador++;
	}
}

//VALIDAÇÃO
if($itensCheckList){
	$where = "AND i.iclid NOT IN ($itensCheckList) ";
}else{
	$where = "";
}

$sql = "SELECT DISTINCT
			a._atinumero || ' - ' || a.atidescricao AS atividade, 
			i.iclid || ' - ' || i.icldsc AS item, 
			i.iclprazo, 
			CASE e.entnumcpfcnpj
				WHEN '11112123000174' THEN 'mokamura@modulo.com.br' --MODULO SECURITY SOLUTIONS S/A
				WHEN '42270181000116' THEN 'alvaro@cesgranrio.org.br' --FUNDACAO CESGRANRIO
				WHEN '00038174000143' THEN 'sonia.gouveia@cespe.unb.br;ac@cespe.unb.br;maria.gaspar@cespe.unb.br' --FUNDACAO UNIVERSIDADE DE BRASILIA
				WHEN '62004395000158' THEN 'andre.vissirini@rrd.com' --RR DONNELLEY MOORE EDITORA E GRAFICA LTDA
				WHEN '34028316000707' THEN 'rodrigorovai@correios.com.br' --EMPRESA BRASILEIRA DE CORREIOS E TELEGRAFOS
			ELSE usu.usuemail
			END AS emailusuario,
			CASE e.entnumcpfcnpj
				WHEN '11112123000174' THEN 'MARCOS SATORU OKAMURA' --MODULO SECURITY SOLUTIONS S/A
				WHEN '42270181000116' THEN 'ÁLVARO FREITAS' --FUNDACAO CESGRANRIO
				WHEN '00038174000143' THEN 'CESPE' --FUNDACAO UNIVERSIDADE DE BRASILIA
				--WHEN '00038174000143' THEN 'SONIA OLESKO' --FUNDACAO UNIVERSIDADE DE BRASILIA
				WHEN '62004395000158' THEN 'ANDRE AVILA VISSIRINI' --RR DONNELLEY MOORE EDITORA E GRAFICA LTDA
				WHEN '34028316000707' THEN 'RODRIGO VENEZIAN ROVAI' --EMPRESA BRASILEIRA DE CORREIOS E TELEGRAFOS
			ELSE usu.usunome
			END AS nomeusuario, 
			usudiretor.usuemail AS emaildiretor, 
			usudiretor.usunome AS nomediretor,
			a._atiordem, i.iclordem, e.entnumcpfcnpj, i.iclid
		FROM pde.itemchecklist i
		INNER JOIN pde.atividade a ON a.atiid = i.atiid
		INNER JOIN pde.checklistentidade ce ON ce.iclid = i.iclid AND ce.tpvid = 2
		INNER JOIN entidade.entidade e ON e.entid = ce.entid
		LEFT JOIN seguranca.usuario usu ON usu.usucpf = e.entnumcpfcnpj
		LEFT JOIN pde.enemservidor es ON es.usucpf = usu.usucpf
		LEFT JOIN pde.enemdiretor ed ON ed.ediid = es.ediid
		LEFT JOIN seguranca.usuario usudiretor ON usudiretor.usucpf = ed.usucpf
		WHERE a._atiprojeto = 114098
		$whereProjetos
		AND i.iclid NOT IN (SELECT DISTINCT v.iclid FROM pde.validacao v WHERE v.tpvid = 2)
		AND i.iclprazo IS NOT NULL
		AND CASE WHEN e.entnumcpfcnpj IN ('11112123000174', '42270181000116', '00038174000143', '62004395000158', '34028316000707') THEN 'SIM' ELSE usu.usunome END IS NOT NULL
		$where
		ORDER BY i.iclprazo, a._atiordem, i.iclordem";
$arrDados2 = $db->carregar($sql);
if($arrDados2){
	foreach($arrDados2 as $dado){
		if($contador == 0){
			$itensCheckList = $dado['iclid'];
		}else{
			$itensCheckList = $itensCheckList . ', ' . $dado['iclid'];
		}
		if(date('Y-m-d') == $dado['iclprazo'] - 3){
			$newDate = date("d/m/Y", strtotime($dado['iclprazo']));
			$e = new EmailAgendado();
			$e->setTitle(SIGLA_SISTEMA. " - Exames MEC / INEP - VALIDAÇÃO - Faltam 72 horas para vencer o prazo");
			$html = 'Prezado(a) senhor (a) '.$dado['nomeusuario'].',<br>
			<p>Informamos que a validação da atividade <b>'.$dado['atividade'].'</b>, item <b>'.$dado['item'].'</b> no SIMEC deve ser cumprida até o dia ' . $newDate . '.</p>';
			$e->setText($html);
			$e->setName(SIGLA_SISTEMA. " - Ministério da Educação");
			$e->setEmailOrigem("no-reply@mec.gov.br");
			$arrEmail = Array($dado['emailusuario']);
			//$arrEmail = Array($_SESSION['email_sistema']);
			$e->setEmailsDestino($arrEmail);
			$e->enviarEmails();	
		}elseif(date('Y-m-d') == $dado['iclprazo']){
			$e = new EmailAgendado();
			$e->setTitle(SIGLA_SISTEMA. " - Exames MEC / INEP - VALIDAÇÃO - O prazo termina hoje");
			$html = 'Prezado(a) senhor (a) '.$dado['nomeusuario'].',<br>
			<p>Reitaramos que a validação da atividade <b>'.$dado['atividade'].'</b>, item <b>'.$dado['item'].'</b> no SIMEC, sob sua responsabilidade, deve ser cumprida até o dia de hoje.</p>';
			$e->setText($html);
			$e->setName(SIGLA_SISTEMA. " - Ministério da Educação");
			$e->setEmailOrigem("no-reply@mec.gov.br");
			$arrEmail = Array($dado['emailusuario']);
			//$arrEmail = Array($_SESSION['email_sistema']);
			$e->setEmailsDestino($arrEmail);
			$e->enviarEmails();	
		}elseif(date('Y-m-d') > $dado['iclprazo']){
			$e = new EmailAgendado();
			$e->setTitle(SIGLA_SISTEMA. " - Exames MEC / INEP - VALIDAÇÃO - O prazo venceu");
			$html = 'Prezado(a) senhor (a) '.$dado['nomeusuario'].',<br>
			<p>Reiteramos que a validação da atividade <b>'.$dado['atividade'].'</b>, item <b>'.$dado['item'].'</b> no SIMEC, encontra-se em atraso, razão pela qual se faz necessário o emprego de diligências para sanear o procedimento.</p>';
			$e->setText($html);
			$e->setName(SIGLA_SISTEMA. " - Ministério da Educação");
			$e->setEmailOrigem("no-reply@mec.gov.br");
			if($dado['emaildiretor']){
				$arrEmail = Array($dado['emailusuario'],$dado['emaildiretor']);
			}else{
				$arrEmail = Array($dado['emailusuario']);
			}
			//$arrEmail = Array($_SESSION['email_sistema']);
			$e->setEmailsDestino($arrEmail);
			$e->enviarEmails();	
		}
		$contador++;
	}
}

//CERTIFICAÇÃO
if($itensCheckList){
	$where = "AND i.iclid NOT IN ($itensCheckList) ";
}else{
	$where = "";
}
$sql = "SELECT DISTINCT
			a._atinumero || ' - ' || a.atidescricao AS atividade, 
			i.iclid || ' - ' || i.icldsc AS item, 
			i.iclprazo, 
			CASE e.entnumcpfcnpj
				WHEN '11112123000174' THEN 'mokamura@modulo.com.br' --MODULO SECURITY SOLUTIONS S/A
				WHEN '42270181000116' THEN 'alvaro@cesgranrio.org.br' --FUNDACAO CESGRANRIO
				WHEN '00038174000143' THEN 'sonia.gouveia@cespe.unb.br;ac@cespe.unb.br;maria.gaspar@cespe.unb.br' --FUNDACAO UNIVERSIDADE DE BRASILIA
				WHEN '62004395000158' THEN 'andre.vissirini@rrd.com' --RR DONNELLEY MOORE EDITORA E GRAFICA LTDA
				WHEN '34028316000707' THEN 'rodrigorovai@correios.com.br' --EMPRESA BRASILEIRA DE CORREIOS E TELEGRAFOS
			ELSE usu.usuemail
			END AS emailusuario,
			CASE e.entnumcpfcnpj
				WHEN '11112123000174' THEN 'MARCOS SATORU OKAMURA' --MODULO SECURITY SOLUTIONS S/A
				WHEN '42270181000116' THEN 'ÁLVARO FREITAS' --FUNDACAO CESGRANRIO
				WHEN '00038174000143' THEN 'CESPE' --FUNDACAO UNIVERSIDADE DE BRASILIA
				--WHEN '00038174000143' THEN 'SONIA OLESKO' --FUNDACAO UNIVERSIDADE DE BRASILIA
				WHEN '62004395000158' THEN 'ANDRE AVILA VISSIRINI' --RR DONNELLEY MOORE EDITORA E GRAFICA LTDA
				WHEN '34028316000707' THEN 'RODRIGO VENEZIAN ROVAI' --EMPRESA BRASILEIRA DE CORREIOS E TELEGRAFOS
			ELSE usu.usunome
			END AS nomeusuario, 
			usudiretor.usuemail AS emaildiretor, 
			usudiretor.usunome AS nomediretor,
			a._atiordem, i.iclordem, e.entnumcpfcnpj, i.iclid
		FROM pde.itemchecklist i
		INNER JOIN pde.atividade a ON a.atiid = i.atiid
		INNER JOIN pde.checklistentidade ce ON ce.iclid = i.iclid AND ce.tpvid = 3
		INNER JOIN entidade.entidade e ON e.entid = ce.entid
		LEFT JOIN seguranca.usuario usu ON usu.usucpf = e.entnumcpfcnpj
		LEFT JOIN pde.enemservidor es ON es.usucpf = usu.usucpf
		LEFT JOIN pde.enemdiretor ed ON ed.ediid = es.ediid
		LEFT JOIN seguranca.usuario usudiretor ON usudiretor.usucpf = ed.usucpf
		WHERE a._atiprojeto = 114098
		$whereProjetos
		AND i.iclid NOT IN (SELECT DISTINCT v.iclid FROM pde.validacao v WHERE v.tpvid = 3)
		AND i.iclprazo IS NOT NULL
		AND CASE WHEN e.entnumcpfcnpj IN ('11112123000174', '42270181000116', '00038174000143', '62004395000158', '34028316000707') THEN 'SIM' ELSE usu.usunome END IS NOT NULL
		$where
		ORDER BY i.iclprazo, a._atiordem, i.iclordem";
$arrDados3 = $db->carregar($sql);
if($arrDados3){
	foreach($arrDados3 as $dado){
		if(date('Y-m-d') == $dado['iclprazo'] - 3){
			$newDate = date("d/m/Y", strtotime($dado['iclprazo']));
			$e = new EmailAgendado();
			$e->setTitle(SIGLA_SISTEMA. " - Exames MEC / INEP - CERTIFICAÇÃO - Faltam 72 horas para vencer o prazo");
			$html = 'Prezado(a) senhor (a) '.$dado['nomeusuario'].',<br>
			<p>Informamos que a certificação da atividade <b>'.$dado['atividade'].'</b>, item <b>'.$dado['item'].'</b> no SIMEC deve ser cumprida até o dia ' . $newDate . '.</p>';
			$e->setText($html);
			$e->setName(SIGLA_SISTEMA. " - Ministério da Educação");
			$e->setEmailOrigem("no-reply@mec.gov.br");
			$arrEmail = Array($dado['emailusuario']);
			//$arrEmail = Array($_SESSION['email_sistema']);
			$e->setEmailsDestino($arrEmail);
			$e->enviarEmails();	
		}elseif(date('Y-m-d') == $dado['iclprazo']){
			$e = new EmailAgendado();
			$e->setTitle(SIGLA_SISTEMA. " - Exames MEC / INEP - CERTIFICAÇÃO - O prazo termina hoje");
			$html = 'Prezado(a) senhor (a) '.$dado['nomeusuario'].',<br>
			<p>Reitaramos que a certificação da atividade <b>'.$dado['atividade'].'</b>, item <b>'.$dado['item'].'</b> no SIMEC, sob sua responsabilidade, deve ser cumprida até o dia de hoje.</p>';
			$e->setText($html);
			$e->setName(SIGLA_SISTEMA. " - Ministério da Educação");
			$e->setEmailOrigem("no-reply@mec.gov.br");
			$arrEmail = Array($dado['emailusuario']);
			//$arrEmail = Array($_SESSION['email_sistema']);
			$e->setEmailsDestino($arrEmail);
			$e->enviarEmails();	
		}elseif(date('Y-m-d') > $dado['iclprazo']){
			$e = new EmailAgendado();
			$e->setTitle(SIGLA_SISTEMA. " - Exames MEC / INEP - CERTIFICAÇÃO - O prazo venceu");
			$html = 'Prezado(a) senhor (a) '.$dado['nomeusuario'].',<br>
			<p>Reiteramos que a certificação da atividade <b>'.$dado['atividade'].'</b>, item <b>'.$dado['item'].'</b> no SIMEC, encontra-se em atraso, razão pela qual se faz necessário o emprego de diligências para sanear o procedimento.</p>';
			$e->setText($html);
			$e->setName(SIGLA_SISTEMA. " - Ministério da Educação");
			$e->setEmailOrigem("no-reply@mec.gov.br");
			if($dado['emaildiretor']){
				$arrEmail = Array($dado['emailusuario'],$dado['emaildiretor']);
			}else{
				$arrEmail = Array($dado['emailusuario']);
			}
			//$arrEmail = Array($_SESSION['email_sistema']);
			$e->setEmailsDestino($arrEmail);
			$e->enviarEmails();	
		}
	}
}

//AVISO AO ANALISTA DO SISTEMA
$e = new EmailAgendado();
$e->setTitle(SIGLA_SISTEMA. " - Exames MEC / INEP - Envio de Alertas Executado");
$html = SIGLA_SISTEMA. ' - Exames MEC / INEP - Envio de Alertas Executado';
$e->setText($html);
$e->setName(SIGLA_SISTEMA. " - Ministério da Educação");
$e->setEmailOrigem("no-reply@mec.gov.br");
$arrEmail = Array($_SESSION['email_sistema']);
$e->setEmailsDestino($arrEmail);
$e->enviarEmails();	

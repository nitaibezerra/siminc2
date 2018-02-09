<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";

include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . "www/proinfantil/_constantes.php";
include_once APPRAIZ . "www/proinfantil/_funcoes.php";
include_once APPRAIZ . "includes/classes/Sms.class.inc";

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 


$db = new cls_banco();

session_start();
 
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

$Tinicio = getmicrotime();

$sql = "SELECT		
			estuf,
            muncod,
            mundescricao,
			turdsc,
		    data,
		    dias,
		    docid,
		    turid,
		    turano
		FROM(
		    SELECT DISTINCT
		        t.turdsc,
		        t.turid,
		        t.turano,
		        ntw.docid,
                mun.estuf,
                mun.muncod,
                mun.mundescricao,
		        to_char(max(h.htddata), 'DD/MM/YYYY HH24:MI:SS') as data,
		        --90 - (cast(now() as date) - cast(max(h.htddata) as date)) as dias,
				90 - SUM(CAST(
						COALESCE(
							(SELECT h2.htddata 
							FROM workflow.historicodocumento h2 
							INNER JOIN workflow.documento	doc2 ON doc2.docid = h2.docid AND doc2.tpdid = ".WF_TPDID_PROINFANTIL_NOVASTURMAS."
							WHERE 
								h2.hstid = (SELECT min(h3.hstid) 
										    FROM workflow.historicodocumento h3 
										    INNER JOIN workflow.documento	doc3 ON doc3.docid = h3.docid AND doc3.tpdid = ".WF_TPDID_PROINFANTIL_NOVASTURMAS."
										    WHERE 
										    	h3.docid = doc.docid 
										    	AND h3.hstid > h.hstid )
								AND h2.aedid = ".NOVASTURMAS_RETORNAR_ANALISE."
							),now()) as date) - cast(h.htddata as date)) as dias
		    FROM
		        proinfantil.turma t
			INNER JOIN proinfantil.analisenovasturmasaprovacao 	na  ON na.turid = t.turid
			INNER JOIN proinfantil.novasturmasworkflowturma 	ntw ON ntw.turid = t.turid
		  	INNER JOIN workflow.documento 						doc ON doc.docid = ntw.docid
		   	INNER JOIN workflow.historicodocumento 				h   ON h.docid = ntw.docid AND h.aedid = ".NOVASTURMAS_ENVIAR_DILIGENCIA."
           	INNER JOIN territorios.municipio 					mun ON mun.muncod = t.muncod
		    WHERE
		    	na.anatipo = 'D'
		        AND na.anastatus = 'A'
		        AND doc.esdid = ".WF_NOVASTURMAS_EM_DILIGENCIA."
		    GROUP BY
		        t.turdsc,
		        t.turid,
		        t.turano,
		        ntw.docid,
                mun.estuf,
                mun.muncod,
                mun.mundescricao
		) as foo
		--WHERE dias <= 15
        ORDER BY 
        	estuf,
            mundescricao";

$arrTurma = $db->carregar($sql);
$arrTurma = $arrTurma ? $arrTurma : Array();

$arrRegistro = array();
foreach ($arrTurma as $v) {
	$sql = "SELECT
			    hst.hstid,
			    hst.htddata,
			    hst.docid,
			    hst.aedid
			FROM
			    workflow.documento doc 
			    inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
			    inner join workflow.historicodocumento hst on hst.docid = doc.docid 
			WHERE
			    doc.docid = {$v['docid']}
			  order by hst.htddata asc";
	$arrWork = $db->carregar( $sql );
	$arrWork = $arrWork ? $arrWork : array();
	
	$dias = calculaDiasVigencia($arrWork, NOVASTURMAS_ENVIAR_DILIGENCIA, NOVASTURMAS_RETORNAR_ANALISE);
	 
	$diasDiligencia = (90 - (int)$dias);
	$diasDiligencia = ((int)$diasDiligencia < 0 ? 0 : $diasDiligencia);
	
	if( $diasDiligencia <= 15  ){
		array_push($arrRegistro, array(
									'estuf' => $v['estuf'],
									'muncod' => $v['muncod'],
									'mundescricao' => $v['mundescricao'],
									'turdsc' => $v['turdsc'],
									'data' => $v['data'],
									'dias' => $diasDiligencia,
									'docid' => $v['docid'],
									'turid' => $v['turid'],
									'turano' => $v['turano']
									)
				);
	}
}

$arrTurmaDiligencia = Array();
$arrTurmaTramitada 	= Array();

$arrMuncodSMSDiligencia15 	= Array();
$arrMuncodSMSDiligencia7 	= Array();
$arrMuncodSMSDiligencia2 	= Array();

$arMuncodEmailDiligencia 	= Array();
$arMuncodEmailDiligenciaAno = Array();
$arMuncodEmailIndeferido 	= Array();
$arMuncodEmailIndeferidoAno = Array();

$arDias = array(2, 7, 15);

foreach ($arrRegistro as $v) {
	
	if( $v['docid'] ){
		$arDados = Array('muncod' => $v['muncod']);
		
		if( (int)$v['dias'] < 1 ){
			$sql = "SELECT esdid FROM workflow.documento WHERE docid = {$v['docid']}";								
			$esdid 			= $db->pegaUm( $sql );
			$esdiddestino 	= WF_NOVASTURMAS_EM_INDEFERIDO_ARQUIVADO_SISTEMA;
			
			$sql = "select aedid from workflow.acaoestadodoc where esdidorigem = $esdid and esdiddestino = ".$esdiddestino;
			$aedid = $db->pegaUm( $sql );
			
			$texto = 'Informamos que o pleito foi indeferido por decurso de prazo, pois de acordo com o estabelecido no §3º Art. 5º da Resolução CD/FNDE nº 16, transcrito abaixo, o município não respondeu a diligência em tempo hábil.
Resolução CD/FNDE nº 16, Art. 5º, §3º: 
§ 3º O município ou o DF terá o prazo máximo de 90 (noventa) dias para esclarecera SEB/MEC sobre os estabelecimentos cuja situação seja apresentada no Simec como "em diligência".
Coordenação Geral de Educação Infantil';
			
			if(wf_alterarEstado( $v['docid'], $aedid, $texto, $arDados )){
				
				$sql = "UPDATE proinfantil.novasturmasworkflowturma SET ntwenviosistema = 'S' WHERE turid = {$v['turid']} and muncod = '{$v['muncod']}'";
				$db->executar($sql);
				$db->commit();
				
				if( !in_array($v['muncod'], $arMuncodEmailIndeferido) ) array_push($arMuncodEmailIndeferido, $v['muncod']);
				
				array_push($arrTurmaTramitada, Array(	'estuf' => $v['estuf'],
														'muncod' => $v['muncod'],
														'mundescricao' => $v['mundescricao'],
														'turdsc' => $v['turdsc'],
														'dias' => $v['dias']
												));
			}
		} else {
			
			if( !in_array($v['muncod'], $arMuncodEmailDiligencia) && in_array($v['dias'], $arDias) ){
				array_push($arMuncodEmailDiligencia, $v['muncod']);
				array_push($arMuncodEmailDiligenciaAno, array(	'muncod' => $v['muncod'], 
																'dias' => $v['dias'], 
																'ano' => $v['turano']
															)
							);
			}
			
			array_push($arrTurmaDiligencia, Array(	'estuf' => $v['estuf'],
													'muncod' => $v['muncod'],
													'mundescricao' => $v['mundescricao'],
													'turdsc' => $v['turdsc'],
													'dias' => $v['dias']
												));
			
			if( $v['dias'] == 15 ){
				array_push($arrMuncodSMSDiligencia15, $v['muncod']);
			}
			if( $v['dias'] == 7 ){
				array_push($arrMuncodSMSDiligencia7, $v['muncod']);
			}
			if( $v['dias'] == 2 ){
				array_push($arrMuncodSMSDiligencia2, $v['muncod']);
			}
		}
	}
}

$emailNildaDiligencia = array();

if( $arMuncodEmailDiligencia[0] ){
	$remetente = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");
	foreach ($arMuncodEmailDiligenciaAno as $v) {
		$strAssunto = 'Novas Turmas '.$v['ano'].' - Turmas em Diligência';
		$strMensagem = '<p>Seu processo referente a Novas Turmas de Educa&ccedil;&atilde;o Infantil encontra-se em dilig&ecirc;ncia, no SIMEC &ndash; M&oacute;dulo E.I. Manuten&ccedil;&atilde;o, informamos que faltam '.$v['dias'].' dias para expirar o prazo para corrigir as informa&ccedil;&otilde;es e enviar novamente para an&aacute;lise, pois de acordo com o estabelecido na Resolu&ccedil;&atilde;o CD/FNDE n&ordm; 16 o munic&iacute;pio (ou o DF) tem o prazo m&aacute;ximo de 90 (noventa) dias para esclarecer SEB/MEC sobre os estabelecimentos cuja situa&ccedil;&atilde;o esteja apresentada no Simec como "em dilig&ecirc;ncia".</p> <p>Coordena&ccedil;&atilde;o Geral de Educa&ccedil;&atilde;o Infantil</p>';
				
		$sql = "SELECT DISTINCT 
					us.usuemail 
				FROM proinfantil.usuarioresponsabilidade ur
				INNER JOIN seguranca.usuario us ON us.usucpf = ur.usucpf 
				WHERE 
					ur.muncod in ('".$v['muncod']."')
					AND ur.rpustatus = 'A'
	                AND us.usustatus = 'A'";			
		$strEmailTo = $db->carregarColuna($sql);
		
		if( !in_array($v['dias'], $emailNildaDiligencia) ){			
			array_push($emailNildaDiligencia, $v['dias']);
		}
		$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
	}
}
	
// Envio de SMS
if( $arrMuncodSMSDiligencia15[0] ){
	$sql = "SELECT DISTINCT 
				'55'||removeacento(ent.entnumdddcelular)||replace(removeacento(ent.entnumcelular),' ','') as celular
			FROM 
				proinfantil.usuarioresponsabilidade ur
			INNER JOIN seguranca.usuario us  ON us.usucpf = ur.usucpf 
			INNER JOIN entidade.entidade ent ON ent.entnumcpfcnpj = ur.usucpf
			WHERE 
				ur.muncod in ('".implode("', '", $arrMuncodSMSDiligencia15)."')
				AND ur.rpustatus = 'A'
				AND us.usustatus = 'A'";
	$contatos = $db->carregarColuna($sql);
	$contatos = filtraTamanhoStrings( $contatos, 12 );
// 	$contatos = Array('556181184192','556181485600');
	$conteudo = "Faltam 15 dias para corrigir as informações – Novas Turmas - e enviar no Simec.";
	if( $contatos[0] != '' ){
		//array_push($contatos, '556184023666'); #telefone da nilda
		//array_push($contatos, '556191485600'); #telefone da thiago tasca
		
		$sms = new Sms();
		$sms->enviarSms($contatos, $conteudo, null, 99);
	}
}
if( $arrMuncodSMSDiligencia7[0] ){
	$sql = "SELECT DISTINCT 
				'55'||removeacento(ent.entnumdddcelular)||replace(removeacento(ent.entnumcelular),' ','') as celular
			FROM 
				proinfantil.usuarioresponsabilidade ur
			INNER JOIN seguranca.usuario us  ON us.usucpf = ur.usucpf 
			INNER JOIN entidade.entidade ent ON ent.entnumcpfcnpj = ur.usucpf
			WHERE 
				ur.muncod in ('".implode("', '", $arrMuncodSMSDiligencia7)."')
				AND ur.rpustatus = 'A'
				AND us.usustatus = 'A'";
	$contatos = $db->carregarColuna($sql);
	$contatos = filtraTamanhoStrings( $contatos, 12 );
	
// 	$contatos = Array('556181184192','556181485600');
	$conteudo = "Faltam 07 dias para corrigir as informações – Novas Turmas – e enviar no Simec";
	if( $contatos[0] != '' ){
		//array_push($contatos, '556184023666'); #telefone da nilda
		//array_push($contatos, '556191485600'); #telefone da thiago tasca
	
		$sms = new Sms();
		$sms->enviarSms($contatos, $conteudo, null, 99);
	}
}
if( $arrMuncodSMSDiligencia2[0] ){
	$sql = "SELECT DISTINCT 
				'55'||removeacento(ent.entnumdddcelular)||replace(removeacento(ent.entnumcelular),' ','') as celular
			FROM 
				proinfantil.usuarioresponsabilidade ur
			INNER JOIN seguranca.usuario us  ON us.usucpf = ur.usucpf 
			INNER JOIN entidade.entidade ent ON ent.entnumcpfcnpj = ur.usucpf
			WHERE 
				ur.muncod in ('".implode("', '", $arrMuncodSMSDiligencia2)."')
				AND ur.rpustatus = 'A'
				AND us.usustatus = 'A'";
	$contatos = $db->carregarColuna($sql);
	$contatos = filtraTamanhoStrings( $contatos, 12 );
	
// 	$contatos = Array('556181184192','556181485600');
	$conteudo = "Faltam 02 dias para corrigir as informações – Novas Turmas – e enviar no Simec";
	if( $contatos[0] != '' ){
		//array_push($contatos, '556184023666'); #telefone da nilda
		//array_push($contatos, '556191485600'); #telefone da thiago tasca
	
		$sms = new Sms();
		$sms->enviarSms($contatos, $conteudo, null, 99);
	}
}
// FIM Envio de SMS
if( $arMuncodEmailIndeferido[0] ){
	$remetente = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");
	$strAssunto = 'Novas Turmas - Turmas em Diligência';
	$strMensagem = '<p>Informamos que o pleito foi indeferido por decurso de prazo, pois de acordo com o estabelecido no &sect;3&ordm; Art. 5&ordm; da Resolu&ccedil;&atilde;o CD/FNDE n&ordm; 16, transcrito abaixo, o munic&iacute;pio n&atilde;o respondeu a dilig&ecirc;ncia em tempo h&aacute;bil.</p> <p>Resolu&ccedil;&atilde;o CD/FNDE n&ordm; 16, Art. 5&ordm;, &sect;3&ordm;:</p> <p><em>&sect; 3&ordm; O munic&iacute;pio ou o DF ter&aacute; o prazo m&aacute;ximo de 90 (noventa) dias para esclarecera SEB/MEC sobre os estabelecimentos cuja situa&ccedil;&atilde;o seja apresentada no Simec como &ldquo;em dilig&ecirc;ncia&rdquo;.</em></p> <p>Coordena&ccedil;&atilde;o Geral de Educa&ccedil;&atilde;o Infantil</p> ';
								
	$sql = "SELECT DISTINCT us.usuemail 
			FROM proinfantil.usuarioresponsabilidade ur
			INNER JOIN seguranca.usuario us on us.usucpf = ur.usucpf 
			WHERE 
				ur.muncod IN ('".implode("', '", $arMuncodEmailIndeferido)."')
				AND ur.rpustatus = 'A'
				AND us.usustatus = 'A'";			
	$strEmailTo = $db->carregarColuna($sql);

	$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
}

$html = '<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
		<thead>
			<tr>
				<th colspan="5">Turma ancaminhada para Indeferido automaticamente via sistema</th>
			</tr>
			<tr>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">UF</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">IBGE</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Municipio</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Turmas</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Dias</label>
			</tr> 
		</thead>
		<tbody>';
$htmlT = '';
foreach ($arrTurmaTramitada as $key => $v) {
	$key % 2 ? $cor = "#dedfde" : $cor = "";
	
	$htmlT .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
					<td valign="top" title="UF">'.$v['estuf'].'</td>
					<td align="right" valign="top" style="color:#999999;" title="IBGE">'.$v['muncod'].'<br></td>
					<td valign="top" title="Municipio">'.$v['mundescricao'].'</td>
					<td valign="top" title="Turmas">'.$v['turdsc'].'</td>
					<td align="right" valign="top" style="color:#999999;" title="Dias">'.$v['dias'].'<br></td>
				</tr>';
}
$htmlT = $html.$htmlT.'</tbody></table>';

$html = '<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
		<thead>
			<tr>
				<th colspan="5">Turmas com prazo de diligência para expirar em 15 dias ou menos</th>
			</tr>
			<tr>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">UF</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">IBGE</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Municipio</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Turmas</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Dias</label>
			</tr> 
		</thead>
		<tbody>';
$htmlD = '';
foreach ($arrTurmaDiligencia as $key => $v) {
	$key % 2 ? $cor = "#dedfde" : $cor = "";
	
	$htmlD .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
					<td valign="top" title="UF">'.$v['estuf'].'</td>
					<td align="right" valign="top" style="color:#999999;" title="IBGE">'.$v['muncod'].'<br></td>
					<td valign="top" title="Municipio">'.$v['mundescricao'].'</td>
					<td valign="top" title="Turmas">'.$v['turdsc'].'</td>
					<td align="right" valign="top" style="color:#999999;" title="Dias">'.$v['dias'].'<br></td>
				</tr>';
	
}
$htmlD = $html.$htmlD.'</tbody></table>';

$Tfinal= getmicrotime() - $Tinicio;

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "SCRIPT AUTOMATICO";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress( $_SESSION['email_sistema'], SIGLA_SISTEMA );
$mensagem->Subject = "Novas Turmas - Turmas em Diligência";
$mensagem->Body = "<p>Turmas encaminhada para indeferido com sucesso! ".date("d/m/Y h:i:s")."</p>
				   <p>".$htmlT.$htmlD."</p>";

$mensagem->IsHTML( true );
$mensagem->Send();

?>
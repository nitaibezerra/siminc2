<?php

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once "/var/www/simec/global/config.inc";
//include_once "config.inc";
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
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$Tinicio = getmicrotime();

$sql = "SELECT DISTINCT
			d.docid as docid,
			ede.muncod,
			mun.mundescricao,
			mun.estuf,
			oi.obrnome,
			oi.obrid,
			pi.pinid
		FROM
			obras2.obras AS oi
			inner join entidade.endereco 			ede  ON ede.endid 	= oi.endid
			inner join territorios.municipio		mun  ON mun.muncod  = ede.muncod
			inner join obras2.empreendimento 		e 	 ON e.empid 	= oi.empid
			inner join obras2.orgao 				oo 	 ON oo.orgid 	= e.orgid 	AND oo.orgstatus = 'A'                                                 
			inner join workflow.documento 			d1 	 ON d1.docid 	= oi.docid
			inner join obras2.programafonte 		pf 	 ON pf.prfid 	= e.prfid
			inner join obras2.tipoobra 				tp 	 ON tp.tobid 	= oi.tobid
			inner join proinfantil.proinfantil 		pi 	 ON pi.obrid 	= oi.obrid
			inner join workflow.documento 			d 	 ON d.docid 	= pi.docid 	AND d.tpdid = ".WF_PROINFANTIL."
		WHERE
			oi.obrstatus = 'A'
			and pf.prfid = ".PRF_PROINFANCIA."
			and oi.obrpercentultvistoria >= 90
			and d1.esdid in (".OBR2_ESDID_OBJ_EXECUCAO.", ".OBR2_ESDID_OBJ_CONCLUIDO.")
			and oi.obridpai IS NULL 
			and d.esdid = ".WF_PROINFANTIL_EM_DILIGENCIA;

$arrTurma = $db->carregar($sql);
$arrTurma = $arrTurma ? $arrTurma : array();
//ver($sql, $arrTurma,d);
$arrPlanoTramitado = array();
$arrPlanoDiligencia = array();

$arrMuncodSMSDiligencia15 	= Array();
$arrMuncodSMSDiligencia7 	= Array();
$arrMuncodSMSDiligencia2 	= Array();

$remetente = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");
$strAssunto = 'Proinfância - Obras em Diligência';

$arMuncodEmailDiligencia 	= Array();
$arMuncodEmailDiligenciaAno = Array();
$arMuncodEmailIndeferido 	= Array();
$arMuncodEmailIndeferidoAno = Array();

$arDias = array(2, 7, 15);

foreach ($arrTurma as $v) {
	
	$sql = "SELECT
				hst.hstid,
				hst.htddata,
				hst.docid,
				hst.aedid
			FROM
				proinfantil.proinfantil pi
				inner join workflow.documento 		doc ON doc.docid = pi.docid
				inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
				inner join workflow.historicodocumento hst on hst.docid = doc.docid 
			WHERE
				pi.obrid = {$v['obrid']}
				order by hst.htddata asc";
	$arrWork = $db->carregar( $sql );
    $arrWork = $arrWork ? $arrWork : array();
    
    $dias = calculaDiasVigencia($arrWork, AEDID_PRO_ENCAMINHAR_DILIGENCIA, AEDID_PRO_DILIGENCIA_ENVIAR_ANALISE);
    $diasDiligencia = (90 - (int)$dias);
    $diasDiligencia = ((int)$diasDiligencia < 0 ? 0 : $diasDiligencia);
    
	if( $v['docid'] && ((int)$diasDiligencia < 15) ){
		$arDados = array('muncod' => $v['muncod']);
		if( (int)$diasDiligencia < 1 ){
			
			$aedid = AEDID_PRO_DILIGENCIA_INDEFERIR_ARQUIVADO_SISTEMA;
			
			if(wf_alterarEstado( $v['docid'], $aedid, 'Prazo de diligência expirado. Plano encaminhada para Indeferido automaticamente via sistema.', $arDados )){
				
				$boTem = $db->pegaUm("select count(praid) from proinfantil.proinfanciaanalise where pinid = {$v['pinid']} and prastatus = 'A'");
			
				if( $boTem > 0 ){
					$sql = "update proinfantil.proinfanciaanalise set prastatus = 'I' where pinid = {$v['pinid']}";
					$db->executar($sql);
				}
				//$parecer = 'As informações sobre o estabelecimento, inseridas no Sistema Integrado de Monitoramento, Execução e Controle do Ministério da Educação (Simec), para recebimento de recursos financeiros para apoio à manutenção de novos estabelecimentos de educação infantil públicos, construídos com recursos federais, foram analisadas conforme critérios estabelecidos pela Resolução CD/FNDE nº 15 de 16 de maio de 2013. Informamos que o pleito foi indeferido por decurso de prazo, pois a) o município não respondeu a diligência em tempo hábil; b) de acordo com o que estabelece o Art. 5º da Lei nº 12.499, transcrito abaixo, o município teve tempo hábil para informar no Censo Escolar da Educação Básica as matrículas do estabelecimento a fim de repasse de recursos do Fundeb. Lei nº 12.499, Art. 5º: Art. 5º Os novos estabelecimentos de educação infantil de que trata o art. 1º deverão ser cadastrados por ocasião da realização do Censo Escolar imediatamente após o início das atividades escolares, sob pena de interrupção do apoio financeiro e devolução das parcelas já recebidas. Coordenação Geral de Educação Infantil';
				$parecer = 'Informamos que o pleito foi indeferido por decurso de prazo, pois de acordo com o estabelecido no §4º Art. 5º da Resolução CD/FNDE nº 15, transcrito abaixo, o município não respondeu a diligência em tempo hábil. Resolução CD/FNDE nº 15, Art. 5º, §4º: § 4º O município ou o DF terá o prazo máximo de 90 (noventa) dias para esclarecera SEB/MEC sobre os estabelecimentos cuja situação seja apresentada no Simec como "em diligência". Coordenação Geral de Educação Infantil.';
				
				$sql = "INSERT INTO proinfantil.proinfanciaanalise(prapareceraprovacao, prastatus, pradata, usucpf, pinid, praanoanalise, praarquivada) 
						VALUES ('{$parecer}', 'A', now(), '00000000191', {$v['pinid']}, 1, true)";
				$db->executar($sql);
				$db->commit();
				if( !in_array($v['muncod'], $arMuncodEmailIndeferido) ) array_push($arMuncodEmailIndeferido, $v['muncod']);										
												
			
				array_push($arrPlanoTramitado, array(	'estuf' => $v['estuf'],
														'muncod' => $v['muncod'],
														'mundescricao' => $v['mundescricao'],
														'obrnome' => $v['obrnome'],
														'dias' => $diasDiligencia
												));
			}
		} else {
			if( !in_array($v['muncod'], $arMuncodEmailDiligencia) && in_array($diasDiligencia, $arDias) ){
				array_push($arMuncodEmailDiligencia, $v['muncod']);
				array_push($arMuncodEmailDiligenciaAno, array(	'muncod' => $v['muncod'], 
																'dias' => $diasDiligencia,
																'obrnome' => $v['obrnome'], 
																'ano' => $v['turano']
															)
							);
			}
			
			array_push($arrPlanoDiligencia, Array(	'estuf' => $v['estuf'],
													'muncod' => $v['muncod'],
													'mundescricao' => $v['mundescricao'],
													'obrnome' => $v['obrnome'],
													'dias' => $diasDiligencia
												));

			if( $diasDiligencia == 15 ){
				array_push($arrMuncodSMSDiligencia15, $v['muncod']);
			}
			if( $diasDiligencia == 7 ){
				array_push($arrMuncodSMSDiligencia7, $v['muncod']);
			}
			if( $diasDiligencia == 2 ){
				array_push($arrMuncodSMSDiligencia2, $v['muncod']);
			}
		}
	}
}

$emailNildaDiligencia = array();

if( $arMuncodEmailDiligencia[0] ){
	$remetente = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");
	foreach ($arMuncodEmailDiligenciaAno as $v) {
		$strMensagem = '<p>Seu processo referente a Unidades do Proinfância encontra-se em diligência, no SIMEC – Módulo E.I. Manutenção.
								Informamos que faltam '.$v['dias'].' dias para expirar o prazo para corrigir as informações e enviar novamente para análise
								pois, de acordo com o estabelecido na Resolução CD/FNDE nº 15, o município (ou o DF) tem o prazo máximo de 90 (noventa)
								dias para esclarecer SEB/MEC sobre os estabelecimentos cuja situação esteja apresentada no Simec como “em diligência”.</p>
								<p>Coordenação Geral de Educação Infantil</p>';
		
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

if( $arMuncodEmailIndeferido[0] ){
	$remetente = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");
	
	$strMensagem = '<p>Informamos que o pleito foi indeferido por decurso de prazo pois, de acordo com o estabelecido no 
								§4º Art. 5º da Resolução CD/FNDE nº 15, transcrito abaixo, o município não respondeu a diligência em tempo hábil.</p>
							<p>Resolução CD/FNDE nº 15, Art. 5º, §4º:</p>
							<p>§ 4º O município ou o DF terá o prazo máximo de 90 (noventa) dias para esclarecera SEB/MEC sobre os estabelecimentos cuja situação seja apresentada no Simec como “em diligência”.</p>
							<p>Coordenação Geral de Educação Infantil</p> ';
	
				
	$sql = "SELECT DISTINCT us.usuemail 
			FROM proinfantil.usuarioresponsabilidade ur
			INNER JOIN seguranca.usuario us on us.usucpf = ur.usucpf 
			WHERE 
				ur.muncod in ('".implode("', '", $arMuncodEmailIndeferido)."')
				AND ur.rpustatus = 'A'
            	AND us.usustatus = 'A'";			
	$strEmailTo = $db->carregarColuna($sql);
	
	$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
	
}

// Envio de SMS
if( $arrMuncodSMSDiligencia15[0] ){
	$sql = "SELECT DISTINCT
				'55'||ent.entnumdddcelular||ent.entnumcelular as celular
			FROM
				proinfantil.usuarioresponsabilidade ur
			INNER JOIN seguranca.usuario us  ON us.usucpf = ur.usucpf
			INNER JOIN entidade.entidade ent ON ent.entnumcpfcnpj = ur.usucpf
			WHERE
				ur.muncod in ('".implode("', '", $arrMuncodSMSDiligencia15)."')
				AND ur.rpustatus = 'A'
				AND us.usustatus = 'A'";
	$contatos = $db->carregarColuna($sql);
// 	$contatos = Array('556181184192','556181485600');
	//array_push($contatos, '556184023666'); #telefone da nilda
	//array_push($contatos, '556191485600'); #telefone da thiago tasca
	$conteudo = "Faltam 15 dias para corrigir as informações – ProInfância – e enviar no Simec";
	$sms = new Sms();
	$sms->enviarSms($contatos, $conteudo, null, 99);
}
if( $arrMuncodSMSDiligencia7[0] ){
	$sql = "SELECT DISTINCT
				'55'||ent.entnumdddcelular||ent.entnumcelular as celular
			FROM
				proinfantil.usuarioresponsabilidade ur
			INNER JOIN seguranca.usuario us  ON us.usucpf = ur.usucpf
			INNER JOIN entidade.entidade ent ON ent.entnumcpfcnpj = ur.usucpf
			WHERE
				ur.muncod in ('".implode("', '", $arrMuncodSMSDiligencia7)."')
				AND ur.rpustatus = 'A'
				AND us.usustatus = 'A'";
	$contatos = $db->carregarColuna($sql);
// 	$contatos = Array('556181184192','556181485600');
	//array_push($contatos, '556184023666'); #telefone da nilda
	//array_push($contatos, '556191485600'); #telefone da thiago tasca
	$conteudo = "Faltam 07 dias para corrigir as informações – ProInfância – e enviar no Simec";
	$sms = new Sms();
	$sms->enviarSms($contatos, $conteudo, null, 99);
}
if( $arrMuncodSMSDiligencia2[0] ){
	$sql = "SELECT DISTINCT
				'55'||ent.entnumdddcelular||ent.entnumcelular as celular
			FROM
				proinfantil.usuarioresponsabilidade ur
			INNER JOIN seguranca.usuario us  ON us.usucpf = ur.usucpf
			INNER JOIN entidade.entidade ent ON ent.entnumcpfcnpj = ur.usucpf
			WHERE
				ur.muncod in ('".implode("', '", $arrMuncodSMSDiligencia2)."')
				AND ur.rpustatus = 'A'
				AND us.usustatus = 'A'";
	$contatos = $db->carregarColuna($sql);
// 	$contatos = Array('556181184192','556181485600');
	//array_push($contatos, '556184023666'); #telefone da nilda
	//array_push($contatos, '556191485600'); #telefone da thiago tasca
	$conteudo = "Faltam 02 dias para corrigir as informações – ProInfância – e enviar no Simec";
	$sms = new Sms();
	$sms->enviarSms($contatos, $conteudo, null, 99);
}
// FIM Envio de SMS

$html = '<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
		<thead>
			<tr>
				<th colspan="5">Obras Proinfancia encaminhada para Indeferido automaticamente via sistema</th>
			</tr>
			<tr>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">UF</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">IBGE</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Municipio</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Obra</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Dias</label>
			</tr> 
		</thead>
		<tbody>';
$htmlT = '';
foreach ($arrPlanoTramitado as $key => $v) {
	$key % 2 ? $cor = "#dedfde" : $cor = "";
	
	$htmlT .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
					<td valign="top" title="UF">'.$v['estuf'].'</td>
					<td align="right" valign="top" style="color:#999999;" title="IBGE">'.$v['muncod'].'<br></td>
					<td valign="top" title="Municipio">'.$v['mundescricao'].'</td>
					<td valign="top" title="Obras">'.$v['obrnome'].'</td>
					<td align="right" valign="top" style="color:#999999;" title="Dias">'.$v['dias'].'<br></td>
				</tr>';
}
$htmlT = $html.$htmlT.'</tbody></table>';

$html = '<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
		<thead>
			<tr>
				<th colspan="5">Obras Proinfancia com prazo de diligência para expirar em 15 dias ou menos</th>
			</tr>
			<tr>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">UF</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">IBGE</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Municipio</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Obra</label>
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Dias</label>
			</tr> 
		</thead>
		<tbody>';
$htmlD = '';
foreach ($arrPlanoDiligencia as $key => $v) {
	$key % 2 ? $cor = "#dedfde" : $cor = "";
	
	$htmlD .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
					<td valign="top" title="UF">'.$v['estuf'].'</td>
					<td align="right" valign="top" style="color:#999999;" title="IBGE">'.$v['muncod'].'<br></td>
					<td valign="top" title="Municipio">'.$v['mundescricao'].'</td>
					<td valign="top" title="Obras">'.$v['obrnome'].'</td>
					<td align="right" valign="top" style="color:#999999;" title="Dias">'.$v['dias'].'<br></td>
				</tr>';
	
}
$htmlD = $html.$htmlD.'</tbody></table>';

$Tfinal= getmicrotime() - $Tinicio;

echo "<p>Obras Proinfancia encaminhada para indeferido com sucesso! ".date("d/m/Y h:i:s")."</p>
				   <p>".$htmlT.$htmlD."</p>";

	$strMensagem = "<p>Obras Proinfancia encaminhada para indeferido com sucesso! ".date("d/m/Y h:i:s")."</p>
					   <p>".$htmlT.$htmlD."</p>";
	
	$strEmailTo = array($_SESSION['email_sistema']);
	$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);

?>
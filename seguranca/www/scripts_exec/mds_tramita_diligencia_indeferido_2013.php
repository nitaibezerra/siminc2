<?php

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
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

$Tinicio = getmicrotime();

$sql = "select
	        mun.estuf,
	        mun.muncod,
	        mun.mundescricao,
	        doc.docid,
	        dcm.cpmid
	    from 
	        territorios.municipio mun
	        inner join proinfantil.mdsdadoscriancapormunicipio dcm on dcm.muncod = mun.muncod and dcm.cpmano = (2013 - 1)
	        inner join workflow.documento doc on doc.docid = dcm.docid
	        inner join proinfantil.procenso pc on pc.muncod = dcm.muncod and pc.prcano = (2013 - 1)
	    where 
	        doc.esdid = ".WF_MDS_EM_DIGILENCIA."
	    group by mun.muncod, mun.estuf, mun.mundescricao, doc.docid, dcm.cpmid
	    order by 
	        mun.estuf, mun.mundescricao";

$arrMds = $db->carregar($sql);
$arrMds = $arrMds ? $arrMds : Array();

$arrMdsTramitada			= Array();
$arMuncodEmailIndeferido 	= Array();
$arMuncodEmailIndeferidoAno = Array();

foreach ($arrMds as $v) {
	
	$sql = "SELECT
			    hst.hstid,
			    hst.htddata,
			    hst.docid,
			    hst.aedid
			FROM
			    proinfantil.mdsdadoscriancapormunicipio cm
			    inner join workflow.documento 		doc ON doc.docid = cm.docid
			    inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
			    inner join workflow.historicodocumento hst on hst.docid = doc.docid 
			WHERE
			    cm.cpmid = '{$v['cpmid']}'
			ORDER BY hst.htddata ASC";
	
	$arrWork = $db->carregar( $sql );
    $arrWork = $arrWork ? $arrWork : array();
	
	$dias = calculaDiasVigencia($arrWork, AEDID_MDS_ENCAMINHAR_ANALISE_DILIGENCIA, AEDID_MDS_ENCAMINHAR_DILIGENCIA_ANALISE);
	$diasDiligencia = (90 - (int)$dias);
    $diasDiligencia = ((int)$diasDiligencia < 0 ? 0 : $diasDiligencia);
    
	if( $v['docid'] && $diasDiligencia < 1 ){
		$arDados = Array('muncod' => $v['muncod']);
		
		$sql = "SELECT esdid FROM workflow.documento WHERE docid = {$v['docid']}";								
		$esdid 			= $db->pegaUm( $sql );
		$esdiddestino 	= WF_MDS_INDEFERIDO_ARQUIVADO_SISTEMA;
		
		$sql = "select aedid from workflow.acaoestadodoc where esdidorigem = $esdid and esdiddestino = ".$esdiddestino;
		$aedid = $db->pegaUm( $sql );
		
		if(wf_alterarEstado( $v['docid'], $aedid, 'Indeferido por decurso de prazo. Município encaminhado para Indeferido automaticamente via sistema.', $arDados )){
			
			$sql = "UPDATE proinfantil.mdsdadoscriancapormunicipio SET cpmenviosistema = 'S' WHERE cpmid = {$v['cpmid']}";
			$db->executar($sql);
			$db->commit();
			
			if( !in_array($v['muncod'], $arMuncodEmailIndeferido) ) array_push($arMuncodEmailIndeferido, $v['muncod']);
			
			array_push($arrMdsTramitada, Array(	'estuf' => $v['estuf'],
												'muncod' => $v['muncod'],
												'mundescricao' => $v['mundescricao'],
												'dias' => $diasDiligencia
											));
		}
	}
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
				<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Dias</label>
			</tr> 
		</thead>
		<tbody>';
$htmlT = '';
foreach ($arrMdsTramitada as $key => $v) {
	$key % 2 ? $cor = "#dedfde" : $cor = "";
	
	$htmlT .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
					<td valign="top" title="UF">'.$v['estuf'].'</td>
					<td align="right" valign="top" style="color:#999999;" title="IBGE">'.$v['muncod'].'<br></td>
					<td valign="top" title="Municipio">'.$v['mundescricao'].'</td>
					<td align="right" valign="top" style="color:#999999;" title="Dias">'.$v['dias'].'<br></td>
				</tr>';
}
$htmlT = $html.$htmlT.'</tbody></table>';

$Tfinal= getmicrotime() - $Tinicio;

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "SCRIPT AUTOMATICO";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress( $_SESSION['email_sistema'] );
$mensagem->Subject = "Suplementação MDS - Município em Diligência";
$mensagem->Body = "<p>Município encaminhado para indeferido com sucesso! ".date("d/m/Y h:i:s")."</p>
				   <p>".$htmlT."</p>";

$mensagem->IsHTML( true );
$mensagem->Send();

?>
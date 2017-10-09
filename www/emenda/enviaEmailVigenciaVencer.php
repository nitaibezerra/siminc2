<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once "_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select ptrcod, ptrexercicio, ptrid, maxdata, dias from(   
		    SELECT ptr.ptrcod, ptr.ptrexercicio, ptv.ptrid, max(ptv.vigdatafim) as maxdata, 
		        cast(to_char(case when cast(to_char(now(), 'YYYY-MM-DD') as date) >  max(ptv.vigdatafim) THEN
		             cast(to_char(now(), 'YYYY-MM-DD') as date) - max(ptv.vigdatafim) else 
		        '00' end , 'DD') as integer) as dias
		    FROM 
		        emenda.ptvigencia ptv
		        inner join emenda.planotrabalho ptr on ptr.ptrid = ptv.ptrid
		        inner join workflow.documento doc on doc.docid = ptr.docid
		    WHERE
		        ptv.vigdatafim is not null
		        and ptr.ptrid in (select max(ptrid) from emenda.planotrabalho /*where ptrcod = 822*/ group by ptrcod)
		        and doc.esdid not in (245, 344, 120)
		    GROUP BY
		        ptv.ptrid,
		        ptr.ptrcod,
		        ptr.ptrexercicio
		    order by
		        ptr.ptrcod,
		        ptv.ptrid
		) as foo
		where
			dias in (12, 30, 60)
		order by
			dias";

$arrDados = $db->carregar( $sql );
$arrDados = $arrDados ? $arrDados : array();

#agrupar por dias
$arrRegistro = array();
foreach ($arrDados as $key => $valor) {
	$arrRegistro[$valor['dias']][] = $valor;
}

foreach ($arrRegistro as $dias => $arrValor) {
	$arrPtrcod = array();
	$arrData = array();
	foreach ($arrValor as $chave => $valor) {
		$arrPtrcod[] = $valor['ptrcod'].'/'.$valor['ptrexercicio'];
		$strData = $valor['maxdata'];
	}
	ver($dias , implode('<br>', $arrPtrcod), $strData);
	
	$strAssunto = "Convênio que estão vencendo em ".$dias." dias";;
	$strMensagem = 'Nº do PTA:<br>'.implode('<br>', $arrPtrcod);;
	$strEmailTo = $_SESSION['email_sistema'];
	
	//enviaEmailAnalise($strAssunto, $strMensagem, $strEmailTo);
	
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= "Atualiza Entidades Emenda";
	$mensagem->From 		= $_SESSION['email_sistema'];
	$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
	$mensagem->Subject = "Convênio que estão vencendo em ".$dias." dias";
	$corpoemail = 'Nº do PTA:<br>'.implode('<br>', $arrPtrcod);
	
	$mensagem->Body = $corpoemail;
	$mensagem->IsHTML( true );
	$mensagem->Send();
	
}
ver($arrRegistro,d);
?>
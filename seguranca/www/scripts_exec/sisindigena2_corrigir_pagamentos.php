<?php

header( 'Content-Type: text/html; charset=ISO-8859-1' );
//header( 'Content-Type: text/html; charset=UTF-8' );

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );


error_reporting( E_ALL ^ E_NOTICE );

ini_set("memory_limit", "1024M");
set_time_limit(0);

ini_set( 'soap.wsdl_cache_enabled', '0' );
ini_set( 'soap.wsdl_cache_ttl', 0 );


$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funчѕes gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/sisindigena2/_constantes.php";
require_once APPRAIZ . "www/sisindigena2/_funcoes.php";


if(!$_SESSION['usucpf']) {
	// CPF do administrador de sistemas
	$_SESSION['usucpforigem'] = '00000000191';
	$_SESSION['usucpf'] = '00000000191';
}

function getmicrotime() {list($usec, $sec) = explode(" ", microtime()); return ((float)$usec + (float)$sec);}

$microtime = getmicrotime();
   
// abre conexчуo com o servidor de banco de dados
$db = new cls_banco();

$sql = "select m.iusd, m.fpbid, h.usucpf, pp.pflcod, pp.pfldsc, t.tpeid, i.iuscpf, i.iusnome, f.fpbmesreferencia, f.fpbanoreferencia, pl.plpvalor, un.uniid from sisindigena2.mensario m 
inner join workflow.documento d on d.docid = m.docid 
inner join workflow.historicodocumento h on h.hstid = d.hstid 
inner join sisindigena2.tipoperfil t on t.iusd = m.iusd 
inner join sisindigena2.identificacaousuario i on i.iusd = t.iusd 
inner join sisindigena2.pagamentoperfil pl on pl.pflcod = t.pflcod 
inner join seguranca.perfil pp on pp.pflcod = pl.pflcod 
inner join sisindigena2.folhapagamento f on f.fpbid = m.fpbid 
inner join sisindigena2.universidadecadastro un on un.uncid = i.uncid 
left join sisindigena2.pagamentobolsista p on p.iusd = m.iusd and m.fpbid = p.fpbid
where d.esdid=1356 and p.pboid is null";

$x = $db->carregar($sql);

if($x[0]) {
	
	foreach($x as $arrInfo) {
		
		$docid = wf_cadastrarDocumento(TPD_PAGAMENTOBOLSA, "Pagamento SISIndэgena - ".$arrInfo['pfldsc']." - ( ".$arrInfo['iuscpf']." )".$arrInfo['iusnome']." - ".$arrInfo['fpbmesreferencia']."/".$arrInfo['fpbanoreferencia']);
		
		$sql = "INSERT INTO sisindigena2.pagamentobolsista(
			            iusd, fpbid, docid, cpfresponsavel, pbodataenvio, pbovlrpagamento, 
			            pflcod, uniid, tpeid, pbocpfbolsista)
			    VALUES ('".$arrInfo['iusd']."', '".$arrInfo['fpbid']."', '".$docid."', '".$arrInfo['usucpf']."', NOW(), '".$arrInfo['plpvalor']."', 
			            '".$arrInfo['pflcod']."', '".$arrInfo['uniid']."', '".$arrInfo['tpeid']."', '".$arrInfo['iuscpf']."')";
		
		$db->executar($sql);
		$db->commit();
		
	}
	
	
}

$db->close();


echo 'FIMMMMMMMMMMMMMMMMMMMMM';


if($_SESSION['usucpf'] == '00000000191') {
	
	unset($_SESSION['usucpf']);
	unset($_SESSION['usucpforigem']);
	
}


?>
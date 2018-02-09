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

// carrega as fun��es gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/sismedio/_constantes.php";
require_once APPRAIZ . "www/sismedio/_funcoes.php";


if(!$_SESSION['usucpf']) {
	// CPF do administrador de sistemas
	$_SESSION['usucpforigem'] = '00000000191';
	$_SESSION['usucpf'] = '00000000191';
}

function getmicrotime() {list($usec, $sec) = explode(" ", microtime()); return ((float)$usec + (float)$sec);}

$microtime = getmicrotime();
   
// abre conex��o com o servidor de banco de dados
$db = new cls_banco();

$sql = "
select h.usucpf, t.tpeid, m.iusd, m.fpbid, pp.pflcod, pp.pfldsc, i.iuscpf, i.iusnome, f.fpbmesreferencia, f.fpbanoreferencia, pl.plpvalor, un.uniid from sismedio.mensario m 
inner join sismedio.identificacaousuario i on i.iusd = m.iusd 
inner join sismedio.tipoperfil t on t.iusd = i.iusd 
inner join seguranca.perfil pp on pp.pflcod = t.pflcod 
inner join sismedio.pagamentoperfil pl on pl.pflcod = t.pflcod
inner join sismedio.folhapagamento f on f.fpbid = m.fpbid and f.fpbstatus='A'
inner join workflow.documento d on d.docid = m.docid 
inner join workflow.historicodocumento h on h.hstid = d.hstid 
inner join sismedio.universidadecadastro un on un.uncid = i.uncid
left join sismedio.pagamentobolsista p on p.iusd = m.iusd and m.fpbid = p.fpbid 
where d.esdid=951 and p.pboid is null";

$x = $db->carregar($sql);

if($x[0]) {
	
	foreach($x as $arrInfo) {
		
			$docid = wf_cadastrarDocumento(TPD_PAGAMENTOBOLSA, "Pagamento - ".$arrInfo['pfldsc']." - (".$arrInfo['iuscpf'].")".$arrInfo['iusnome']." - ".$arrInfo['fpbmesreferencia']."/".$arrInfo['fpbanoreferencia']);
			
			$sql = "INSERT INTO sismedio.pagamentobolsista(
		            iusd, fpbid, docid, cpfresponsavel, pbodataenvio, pbovlrpagamento, 
		            pflcod, uniid, tpeid)
		    VALUES ('".$arrInfo['iusd']."', '".$arrInfo['fpbid']."', '".$docid."', '".$arrInfo['usucpf']."', NOW(), '".$arrInfo['plpvalor']."', 
		            '".$arrInfo['pflcod']."', '".$arrInfo['uniid']."', '".$arrInfo['tpeid']."');";
			
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
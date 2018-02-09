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
require_once APPRAIZ . "www/sispacto/_constantes.php";
require_once APPRAIZ . "www/sispacto/_funcoes.php";


if(!$_SESSION['usucpf']) {
	// CPF do administrador de sistemas
	$_SESSION['usucpforigem'] = '00000000191';
	$_SESSION['usucpf'] = '00000000191';
}
    
   
// abre conexчуo com o servidor de banco de dados
$db = new cls_banco();

$sql = "delete from obras2.vm_arquivos_obras;";
$db->executar($sql);

$sql = "insert into obras2.vm_arquivos_obras
     SELECT   oa.obrid,
sum(case when a.arqtipo not in ('image/jpeg', 'image/gif', 'image/png') then 1 else 0 end) as qtddocumentos,
sum(case when a.arqtipo in ('image/jpeg', 'image/gif', 'image/png') then 1 else 0 end) as qtdfotos
FROM     obras2.obras_arquivos oa    
    JOIN public.arquivo a ON a.arqid = oa.arqid  
    WHERE     oa.oarstatus = 'A' 
     group by oa.obrid order by obrid;";
$db->executar($sql);

$sql = "update obras2.obras t1 set obrsndocumentos = exists (select true from obras2.vm_arquivos_obras t2 where t2.qtddocumentos > 0 and t2.obrid = t1.obrid),
obrsnfotos = exists(select true from obras2.vm_arquivos_obras t2 where t2.qtdfotos > 0 and t2.obrid = t1.obrid) where t1.obrid in ( select t2.obrid from obras2.vm_arquivos_obras t2);";
$db->executar($sql);

$db->commit();

if($_SESSION['usucpf'] == '00000000191') {
	
	unset($_SESSION['usucpf']);
	unset($_SESSION['usucpforigem']);
	
}

?>
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
require_once APPRAIZ . "www/sispacto/_funcoes_coordenadorlocal.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
    
   
// abre conexчуo com o servidor de banco de dados
$db = new cls_banco();


$sql = "SELECT * FROM sispacto.pactoidadecerta";
$picids = $db->carregar($sql);

if($picids) {
	foreach($picids as $pic) {
		if($pic['muncod']) $_SESSION['sispacto']['esfera'] = 'Municipal';
		else $_SESSION['sispacto']['esfera'] = 'Estadual';
		
		$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']=$pic['picid'];
		$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['estuf']=$pic['estuf'];
		$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['muncod']=$pic['muncod'];
		
		calculaPorcentagemCadastroOrientadores(array("suaid"=>2,"picid"=>$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']));		
	}
}



die("fim");

?>
<?php
header( 'Content-Type: text/html; charset=ISO-8859-1' );
//header( 'Content-Type: text/html; charset=UTF-8' );

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );


error_reporting( E_ALL ^ E_NOTICE );

ini_set("memory_limit", "1024M");
set_time_limit(0);

ini_set( 'soap.wsdl_cache_enabled', '0' );
ini_set( 'soap.wsdl_cache_ttl', 0 );
ini_set( 'default_socket_timeout', '99999999' ); 


$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/sispacto/_constantes.php";
require_once APPRAIZ . "www/sispacto/_funcoes.php";
require_once APPRAIZ . "www/includes/webservice/cpf.php";


// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
    
   
// abre conexção com o servidor de banco de dados
$db = new cls_banco();


$sql = "select * from sispacto.professoresalfabetizadores where atualizadoreceita=false limit 100";

echo "INI : ".date("Y-m-d h:i:s")."<br>";

$arr = $db->carregar($sql);

if($arr[0]) {
	foreach($arr as $ar) {
		
		$objPessoaFisica = new PessoaFisicaClient("http://ws.mec.gov.br/PessoaFisica/wsdl");
		$xml 			 = $objPessoaFisica->solicitarDadosPessoaFisicaPorCpf($ar['cpf']);
		$obj 			 = (array) simplexml_load_string($xml);
		
		if($obj['PESSOA']->no_pessoa_rf) {
			$sql = "UPDATE sispacto.professoresalfabetizadores SET atualizadoreceita=true, docente='".$obj['PESSOA']->no_pessoa_rf."' WHERE cpf='".$ar['cpf']."'";
			$db->executar($sql);
			$db->commit();
		}
		
	}

} else {
	echo "FIM : ".date("Y-m-d h:i:s")."<br>";
	die("fim");
}

echo "FIM : ".date("Y-m-d h:i:s")."<br>";
echo "<script>window.location=window.location</script>";

?>
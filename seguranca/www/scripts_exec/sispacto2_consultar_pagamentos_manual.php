<?php

header( 'Content-Type: text/html; charset=ISO-8859-1' );
//header( 'Content-Type: text/html; charset=UTF-8' );

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

set_time_limit( 0 );
error_reporting( E_ALL ^ E_NOTICE );

ini_set( 'soap.wsdl_cache_enabled', '0' );
ini_set( 'soap.wsdl_cache_ttl', 0 );
ini_set( 'default_socket_timeout', '99999999' );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/sispacto2/_funcoes.php";

// produção
define( 'SISTEMA_SGB',  'PACTO' );
define( 'USUARIO_SGB',  'PCT' );
define( 'PROGRAMA_SGB', 'PCT' );
define( 'SENHA_SGB',    'AXD*0MI!4WBY1GI:LC+YQF@JHUN3|TMA' );
define( 'WSDL_CAMINHO', 'http://www.fnde.gov.br/spba/Servicos?wsdl');
define( 'WSDL_CAMINHO_CADASTRO', 'http://sgb.fnde.gov.br/sistema/ws/?wsdl');

$opcoes = Array(
                'exceptions'	=> 0,
                'trace'			=> true,
                //'encoding'		=> 'UTF-8',
                'encoding'		=> 'ISO-8859-1',
                'cache_wsdl'    => WSDL_CACHE_NONE
);

$soapClient = new SoapClient( WSDL_CAMINHO, $opcoes );

libxml_use_internal_errors( true );
    
// CPF do administrador de sistemas
if(!$_SESSION['usucpf']) {
	$_SESSION['usucpforigem'] = '00000000191';
	$_SESSION['usucpf'] = '00000000191';
}
    
ini_set("memory_limit", "2048M");

// abre conexção com o servidor de banco de dados

$arxml['bolsista']['autenticacao'] 		= array('sistema' => SISTEMA_SGB, 'login' => USUARIO_SGB,'senha' => SENHA_SGB);
$arxml['bolsista']['cpf'] 			= '51322870187';

//$consultarSituacaoDoPagamento_obj = $soapClient->consultarSituacaoDoPagamento( $arxml );
$consultarSituacaoDoPagamento_obj = $soapClient->consultarHistoricoAutorizacaoPagamento( $arxml );



echo '<pre>';
echo '11';
//echo $soapClient->__getLastRequest();
//echo $soapClient->__getLastResponse();

print_r($consultarSituacaoDoPagamento_obj);



echo "fim";


?>
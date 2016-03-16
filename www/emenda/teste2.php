<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");
error_reporting(-1);

//$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once "_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'includes/workflow.php';

echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';
	
$_SESSION['exercicio'] = '2012';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

include_once APPRAIZ."emenda/classes/WSIntegracaoSiconv.class.inc";

$usuario = 'rmedeiro';
$senha = 'R5659532';

//$urlWsdl = 'https://wshom.convenios.gov.br/siconv-services-interfaceSiconv/InterfaceSiconvHandlerSrv?wsdl';
//$urlWsdl = 'https://www.convenios.gov.br/siconv-services-interfaceSiconv/InterfaceSiconvHandlerSrv?wsdl';
$urlWsdl = 'http://172.20.65.93:8080/IntraSiconvWS/services/SimecWsFacade?wsdl';

$arrParam = array('ptrid' 	=> 3774,
				  'usuario' => $usuario,
				  'senha' 	=> $senha,
				  'url' 	=> $urlWsdl
				);

$obWS = new WSIntegracaoSiconv($arrParam);

$obWS->solicitarNotaEmpenhoWS();
//$obWS->consultaConvenioWS();


/*$wsdl = 'https://wshom.convenios.gov.br/siconv-services-interfaceSiconv/InterfaceSiconvHandlerSrv?wsdl';
$options = Array(
				'exceptions'	=> true,
		        'trace'			=> true,
				'encoding'		=> 'ISO-8859-1' );
$client = new SoapClient($wsdl, $options);
ver($client->__getFunctions(),d);*/

//echo insereFilhosPTA(1679);
//echo deletaFilhosPTA( 3477 );
die;

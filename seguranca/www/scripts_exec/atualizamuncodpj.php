<?php

$_REQUEST['baselogin'] = "simec_desenvolvimento";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "www/includes/webservice/pj.php";

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);}


$db = new cls_banco();

$sql = "SELECT cgc FROM carga.tabela_cgc2004";
$arr = $db->carregarColuna($sql);

foreach($arr as $ar) {
	$objPessoaJuridica = new PessoaJuridicaClient("http://ws.mec.gov.br/PessoaJuridica/wsdl");
	$xml = $objPessoaJuridica->solicitarDadosPessoaJuridicaPorCnpj($ar);
	
	$obj = (array) simplexml_load_string($xml);
	
	$db->executar("UPDATE carga.tabela_cgc2004 SET muncod='".($obj['PESSOA']->ENDERECOS->ENDERECO->co_cidade+0)."' WHERE cgc ='".$ar."'"); 
	$db->commit();

}
$db->close();
echo "fim";


?>
<?php
// carrega as funções gerais
define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );


// carrega as funções gerais
require_once BASE_PATH_SIMEC . '/global/config.inc';
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

$db = new cls_banco();

$dataAtual = date("c");

$anoAtual = date('Y') - 1;
$dt_inicio = $anoAtual.'1231';
$dt_fim = date('Ymd');

$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
  <header>
    <app>string</app>
    <version>string</version>
    <created>$dataAtual</created>
  </header>
  <body>
      <params>      
      <dt_inicio>$dt_inicio</dt_inicio>
      <dt_fim>$dt_fim</dt_fim>
    </params>
  </body>
</request>

XML;


$urlWS = "http://www.fnde.gov.br/webservices/wssape/index.php/convenio/consultar";
	try {
    	$xml = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWS)
			->setParams( array('xml' => $arqXml) )
			->execute();		
		
		$xmlRetorno = $xml;
		$xml = simplexml_load_string( stripslashes($xml));

        if ( (int) $xml->status->result ){
        	/**
			 * @var LogErroWS
			 * Bloco que grava o erro em nossa base
			 */
        	$obConvenio = (array) $xml->body;

        	$sql = '';
        	foreach ($obConvenio['nu_processo'] as $processo) {
        		$dcoid = $db->pegaUm( "SELECT count(dcoid) FROM painel.dadosconvenios WHERE dcoprocesso = '$processo'" );
        		
        		if( (int) $dcoid == 0 ){
        			$sql = "INSERT INTO painel.dadosconvenios(dcoprocesso) VALUES ('{$processo}'); ";
        			$db->executar($sql);
        		}
        	}
        	$db->commit();
		}
	} catch (Exception $e){
		/**
		 * @var LogErroWS
		 * Bloco que grava o erro em nossa base
		 */
	}
?>
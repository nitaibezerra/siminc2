<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once "_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

$db = new cls_banco();

$data_created = date('c');

$usuario = 'MECTIAGOT';
$senha = 'M3135689';

/* Paramentros */
$unidade_gestora_favorecida = '153173'; // String 6 digitos
$gestao_favorecida = '15253'; // String 5 digitos
$observacao = '25'; // String maxLenght 2, maxOccurs="1" minOccurs="1"
$complemento = 'XXXXXXX XXXXXXXXX'; // String maxLentght 240, maxOccurs="1" minOccurs="0"
$processo = '23034252324201109'; // String 17, maxOccurs="1" minOccurs="1"
$numero_documento_siafi_original = ''; // String, minOccurs="0"
$nc_original = ''; // String, maxOccurs="0"
$especie = '3'; // String 2, maxOccurs="1" minOccurs="1"
$programa = 'C7'; // String, maxOccurs="1" minOccurs="1"

$detalhamento = ''; // Previsões orçamentarias, maxOccurs="12" minOccurs="1"

$evento_contabil = '300300'; // String max 6, maxOccurs="1" minOccurs="1"
$esfera_orcamentaria = '1'; // String max 1, maxOccurs="1" minOccurs="1"
$unidade_orcamentaria = '26298'; // String, maxOccurs="1" minOccurs="1"
$centro_gestao = '51000000000'; // String max 11, maxOccurs="1" minOccurs="1"

$celula_orcamentaria = ''; // Dados das previsões

$ptres = '043930'; // String 6, pattern value="\w{6}"
$fonte_recurso = '0100479430'; // String 10, maxOccurs="1" minOccurs="1"
$natureza_despesa = '31909100'; // String 8, pattern value="\w{8}", teste erro 339048
$plano_interno = 'FFF53B0101N'; // String 11, pattern value="\w{11}"

$ano_exercicio = '2013'; // Year
$valor = '30000.00'; // Float
$unidade_gestora_emitente = '153173'; // String 6
$gestao_emitente = '15253'; // String 5

$sistema = '2'; // String, maxOccurs="1" minOccurs="1"
$termo_compromisso = '38'; // String,  maxOccurs="1" minOccurs="0"

$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<unidade_gestora_favorecida>$unidade_gestora_favorecida</unidade_gestora_favorecida>
			<gestao_favorecida>$gestao_favorecida</gestao_favorecida>
			<observacao>$observacao</observacao>			
			<complemento>$complemento</complemento>			
			<processo>$processo</processo>			
			<numero_documento_siafi_original>$numero_documento_siafi_original</numero_documento_siafi_original>			
			<nc_original>$nc_original</nc_original>			
			<especie>$especie</especie>			
			<programa>$programa</programa>			
			<detalhamento>
				<evento_contabil>$evento_contabil</evento_contabil>
				<esfera_orcamentaria>$esfera_orcamentaria</esfera_orcamentaria>					
				<unidade_orcamentaria>$unidade_orcamentaria</unidade_orcamentaria>
				<centro_gestao>$centro_gestao</centro_gestao>					
				<celula_orcamentaria>
					<ptres>$ptres</ptres>
					<fonte_recurso>$fonte_recurso</fonte_recurso>
					<natureza_despesa>$natureza_despesa</natureza_despesa>
					<plano_interno>$plano_interno</plano_interno>
				</celula_orcamentaria>										
				<ano_exercicio>$ano_exercicio</ano_exercicio>					
				<valor>$valor</valor>
				<unidade_gestora_emitente>$unidade_gestora_emitente</unidade_gestora_emitente>					
				<gestao_emitente>$gestao_emitente</gestao_emitente>
			</detalhamento>
			<detalhamento>
				<evento_contabil>$evento_contabil</evento_contabil>
				<esfera_orcamentaria>$esfera_orcamentaria</esfera_orcamentaria>					
				<unidade_orcamentaria>$unidade_orcamentaria</unidade_orcamentaria>
				<centro_gestao>$centro_gestao</centro_gestao>					
				<celula_orcamentaria>
					<ptres>$ptres</ptres>
					<fonte_recurso>$fonte_recurso</fonte_recurso>
					<natureza_despesa>$natureza_despesa</natureza_despesa>
					<plano_interno>$plano_interno</plano_interno>
				</celula_orcamentaria>										
				<ano_exercicio>$ano_exercicio</ano_exercicio>					
				<valor>$valor</valor>
				<unidade_gestora_emitente>$unidade_gestora_emitente</unidade_gestora_emitente>					
				<gestao_emitente>$gestao_emitente</gestao_emitente>
			</detalhamento>
			<sistema>$sistema</sistema>
			<termo_compromisso>$termo_compromisso</termo_compromisso>			
		</params>
	</body>
</request>
XML;

//teste de mais de um co 33323
// $urlWS = 'http://172.20.200.116/webservices/sigef/paulo/public/index.php/financeiro/nc/';
$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/nc';

$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
					->execute();
			;

$xml = simplexml_load_string( stripslashes($xml));
$identificador = (integer) $xml->body->identificador;
echo "<pre>";
echo "<br/>Retorno do SIGEF:<br/><br/>";
var_dump($xml);
echo "<br/><br/>XML de Envio:<br/><br/>";
var_dump(simec_htmlentities($arqXml));
?>
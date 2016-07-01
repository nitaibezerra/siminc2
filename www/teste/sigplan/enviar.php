<?php

$_REQUEST['baselogin'] = "simec_espelho_producao";

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

restore_error_handler();
restore_exception_handler();
error_reporting( E_ALL );
/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "2048M");
set_time_limit( 0 );
/* FIM configurações - Memoria limite de 1024 Mbytes */



# abre conexão com o banco
/*$nome_bd     = 'simec_espelho_producao';
$servidor_bd = 'simec-d';
$porta_bd    = '5432';
$usuario_db  = 'seguranca';
$senha_bd    = 'phpseguranca';
*/

// CPF do administrador de sistemas
//if(!$_SESSION['usucpf'])
//$_SESSION['usucpforigem'] = '';

$db          = new cls_banco();

include "sigplan.php";

$wsdl = "https://www.sigplan.gov.br/infrasig/sigtoinfra.asmx?WSDL";
//$wsdl = "https://homsigplan.serpro.gov.br/infrasig/INFRASIG.ASMX?WSDL";
$configuracao = array(
	"proxy_host" => "proxy.mec.gov.br",
	"proxy_port" => 8080,
	"encoding" => "ISO-8859-1",
	"compression" => SOAP_COMPRESSION_ACCEPT | SOAP_COMPRESSION_GZIP,
	"trace" => true
);
$usuario = "leo.kessel";
$senha = "kessel";
$exercicio = "2007";

$sigplan = new SoapClient( $wsdl, $configuracao );

$sql = "SELECT prgcod, prgano FROM monitora.programa GROUP BY prgcod, prgano";
$programas = $db->carregar($sql);

# enviar ações
try {
	if($programas[0]) {
		foreach($programas as $p) {
			$objeto = new stdClass();
			$objeto->usuario = $usuario;
			$objeto->senha = $senha;
			$objeto->PRGAno = $p['prgano'];
			$objeto->PRGCod = $p['prgcod'];
			$retorno = $sigplan->geracaoPorPrograma( $objeto );
			// Write the contents back to the file
			file_put_contents("../../../arquivos/resp_".$p['prgano']."_".$p['prgcod'].".xml", $sigplan->__getLastResponse());
		}
	}
} catch ( Exception $erro ) {
}


echo "FIM!!!!!!!!!!";

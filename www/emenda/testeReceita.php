<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

if(!$_SESSION['usucpf']) $_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

include_once APPRAIZ.'includes/classes/SimecWsCPFReceita.class.inc';

$wsdl = 'http://ws.mec.gov.br/PessoaFisica/wsdl';

$boReceita = new SimecWsCPFReceita($wsdl, array(
												'exceptions'	=> true,
										        'trace'			=> true,
												'encoding'		=> 'ISO-8859-1' )
												);

$retorno = $boReceita->solicitarDadosReceitaPorCpf( '' );

ver($retorno);

?>
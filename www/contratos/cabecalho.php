<?php
/*
 * rodrigo.rodrigues
 * Cabecalho padrao para script_exec
 * Este cabecalho realiza a conexao com o banco de dados
 * E instancia os includes padroes
 * Exemplo:  emailprestacaocontas.php
 *  */

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);
session_start();
date_default_timezone_set ('America/Sao_Paulo');


// CPF do administrador de sistemas
$_REQUEST['baselogin'] = "simec_espelho_producao";
$_SESSION['usucpforigem'] = '';
$_SESSION['usucpf'] = '';
$_SESSION['script_exec'] = true;

// carrega as funções gerais
include_once dirname(__FILE__) . "/../../../global/config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/funcoes.inc";


$ini_array      = parse_ini_file(dirname(__FILE__) . "/../../../global/config.ini", true);
$GLOBALS['servidor_bd'] = $ini_array['db']['servidor_bd'];
$GLOBALS['porta_bd'] = $ini_array['db']['porta_bd'];
$GLOBALS['nome_bd'] = $ini_array['db']['nome_bd'];
$GLOBALS["usuario_db"] = $ini_array['db']['usuario_db'];
$GLOBALS["senha_bd"] = $ini_array['db']['senha_bd'];

$db = new cls_banco();

?>
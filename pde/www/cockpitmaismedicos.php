<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "pde/www/_funcoes_enem.php";
include_once APPRAIZ . "pde/www/_funcoes_enem.php";
include_once APPRAIZ . "pde/www/_constantes.php";

//ver(APPRAIZ,d);
if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$nao_valida_acesso = true;
include APPRAIZ."pde/modulos/principal/cockpit_mais_medicos.inc";
?>
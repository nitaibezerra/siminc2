<?php

if($_SESSION['baselogin'] == 'simec_espelho_producao') {
    $nome_bd = '';
    $servidor_bd = '';
    $porta_bd = '';
    $usuario_db = '';
    $senha_bd = '';
} else {
    $nome_bd = '';
    $servidor_bd = '';
    $porta_bd = '';
    $usuario_db = '';
    $senha_bd = '';
}

$servidor_bd_siafi = '';
$porta_bd_siafi = '';
$nome_bd_siafi = '';
$usuario_db_siafi = '';
$senha_bd_siafi = '';

# Sistema PDDEInterativo - ESPELHO DE PRODUÇÃO
$configDbPddeinterativo = new stdClass();
$configDbPddeinterativo->host = '';
$configDbPddeinterativo->port = '';
$configDbPddeinterativo->dbname = '';
$configDbPddeinterativo->user = '';
$configDbPddeinterativo->password = '';

# Sistema SIGFOR
$configDbSigfor = new stdClass();
$configDbSigfor->host = '';
$configDbSigfor->port = '';
$configDbSigfor->dbname = '';
$configDbSigfor->user = '';
$configDbSigfor->password = '';

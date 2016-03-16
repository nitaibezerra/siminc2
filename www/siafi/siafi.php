<?php

/*
 * Banco de dados SIAFI em Produção
 */
$configDbSiafi = new stdClass();
$configDbSiafi->host = '';
$configDbSiafi->port = 5432;
$configDbSiafi->dbname = '';
$configDbSiafi->user = '';
$configDbSiafi->password = '';
$configDbSiafi->clientEncoding = 'LATIN5';

//Carrega parametros iniciais do simec
include_once "controleInicio.inc";
/**
 * Autoload de classes dos Módulos SPO.
 * @see autoload.php
 */
require_once APPRAIZ . 'spo/autoload.php';

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

// -- Export de XLS automático da Listagem
Simec_Listagem::monitorarExport($_SESSION['sisdiretorio']);

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";


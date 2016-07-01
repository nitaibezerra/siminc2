<?php

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

/*
 * Layout Novo
 */
//initAutoload();
////Carrega as funções de controle de acesso
//$_SESSION['sislayoutbootstrap'] = 'zimec';
//require_once APPRAIZ . 'includes/library/simec/view/Helper.php';
//$simec = new Simec_View_Helper();
/*
 * FIM Layout Novo
 */

// -- Export de XLS automático da Listagem
Simec_Listagem::monitorarExport($_SESSION['sisdiretorio']);

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>
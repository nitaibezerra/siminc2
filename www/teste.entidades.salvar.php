<?php
//
// $Id$
//


//require_once "config.inc";
if (!defined('APPRAIZ')) {
    define('APPRAIZ', realpath('..') . "/");
}

//include "../includes/classes_simec.inc";
//include "../includes/funcoes.inc";



global $nome_bd;
       $nome_bd     = 'simec_desenvolvimento';

global $servidor_bd;
       $servidor_bd = 'simec-d';

global $porta_bd;
       $porta_bd    = '5432';

global $usuario_db;
       $usuario_db  = 'seguranca';

global $senha_bd;
       $senha_bd    = 'phpseguranca';


//$db = new cls_banco();


require_once "../adodb/adodb.inc.php";
require_once "../includes/ActiveRecord/Decorator.php";
require_once "../includes/ActiveRecord/ActiveRecord.php";
require_once "../includes/ActiveRecord/classes/Entidade.php";

/*
require_once "../includes/ActiveRecord/classes/Endereco.php";




//                                                                          */




$entidade = new Entidade($_REQUEST['entid']);

foreach ($_REQUEST as $campo => $valor) {
    if (preg_match('|(d{2})/(d{2})/(d{4})|', $valor, $res) && checkdate($res[2], $res[1], $res[3]))
        $valor = formata_data_sql($valor);//$res[4] . '-' . $res[2] . '-' . $res[1];

    if ($valor == 0)
        $valor = null;

    $entidade->$campo = trim($valor) != '' ? $valor : null;
}


$entidade->save();

header('Location: teste.entidades.php');
//print_R($entidade);








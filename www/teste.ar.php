<?php

//require_once "config.inc";
if (!defined('APPRAIZ')) {
    define('APPRAIZ', realpath('..') . "/");
}

include "../includes/classes_simec.inc";
include "../includes/funcoes.inc";


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


$db = new cls_banco();

//header( 'Content-Type: text/plain' );


require_once "../adodb/adodb.inc.php";

require_once "../includes/ActiveRecord/Decorator.php";

require_once "../includes/ActiveRecord/ActiveRecord.php";
require_once "../includes/ActiveRecord/classes/Endereco.php";
require_once "../includes/ActiveRecord/classes/Entidade.php";



$a = new Entidade(9863);
$a->carregarEnderecos();


//print_r($a);

?>

<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Expires" content="-1">

    <title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>

    <script language="JavaScript" src="includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='includes/listagem.css'/>

    <script type="text/javascript" language="javascript" src="includes/agrupador.js"></script>
    <style type="text/css">
        .combo {
            width: 200px;
        }
    </style>
  </head>

  <body>
<?php

//$a = new Decorator();
$e = new Endereco();

//$a->teste();
$params = array();
$labels = array('label'          => 'Logradouro',
                'valor'          => $db->monta_combo('endlog', $e->getSelectSql(array('endid as codigo', 'endlog as descricao'), ' endid is not null limit 50'), 'S', 'Endereço', '', '', '', '', 'S', 'endid', true));

$params[] = $labels;

$labels = array('label'          => 'Número',
                'valor'          => '125',
                'callback'       => null,
                'campo'          => 'endnum',
                'paramsCallback' => array());

$params[] = $labels;


echo Decorator::form($a, array('action'   => 'action.go',
                               'onsubmit' => 'teste',
                               'input'    => array()), $params);
//                                                                          */


//Decorator::resetLabels();
//$params = array();


/*
foreach ($a->enderecos as $e) {
    $labels = array('label'          => 'Logradouro',
                    'valor'          => $db->monta_combo('endlog', $e->getSelectSql(array('endid as codigo', 'endlog as descricao'), ' endid is not null limit 50'), 'S', 'Endereço', '', '', '', '', 'S', 'endid', true));

    $params[] = $labels;

    $labels = array('label'          => 'Número',
                    'valor'          => '125',
                    'callback'       => null,
                    'campo'          => 'endnum',
                    'paramsCallback' => array());

    $params[] = $labels;

    Decorator::setConf($params);
    echo Decorator::decorateTest($e);
}



/*
echo Decorator::decorate($a->enderecos, array(array($a->enderecos,
                                                    'Endereços',
                                                    array('ComboBox', 'decorate'))));



/**
static protected $conf = array('name'         => null,
                               'id'           => null,
                               'onchange'     => null,
                               'defaultValue' => null,
                               'selected'     => null,
                               'value'        => null,
                               'label'        => null);
//                                                                          *

echo Decorator::loadPlugin('ComboBox')->decorate($a->enderecos, array('defaultValue' => 'Selecione',
                                                                      'selected'     => '0',
                                                                      'label'        => 'endlog'));

//                                                                          */



//$d->comboBox();
//print_r($d);


//$db->monta_lista($a->getSelectSql(array(), 'entid = 9863'), array('entid', 'entnome', 'entemail'), 10, 10, '', '', '', '');
//print_r($a);






<?php
/**
 * Sistema SCRUM
 * @package simec
 * @subpackage scrum
 */

require_once(APPRAIZ . 'www/scrum/funcoesEntregavel.php');
require_once(APPRAIZ . 'www/scrum/funcoesBoard.php');

$entid = str_replace('eid', '', $_POST['entid']);
$column = $_POST['column'];
$novaPosicao = (int)$_POST['pos'];
$prgid = $_POST['prgid'];
$columnClass = $_POST['columnClass'];

$columnClass = explode(' ', $columnClass);
$columnClass = explode('_', $columnClass[0]);

if($columnClass[0] == 'proximasprint' || $columnClass[0] == 'sprint' ){
    if($columnClass[1] != 0){
        $idSprint = $columnClass[1];
    }
}

if (!empty($entid) && !empty($column) && !is_null($novaPosicao) && !empty($prgid)) {
    switch ($column) {
        case 'sprint':
            updateSprint($prgid, $entid, $novaPosicao, $idSprint);
            break;
        case 'proximasprint':
            updateProximaSprint($prgid, $entid, $novaPosicao , $idSprint);
            break;
        case 'backlog':
            updateBacklog($prgid, $entid, $novaPosicao);
            break;
    }
}

//header('Content-Type: application/json; charset=ISO-8859-1');
//die(simec_json_encode(array('entid'=>$_POST['entid'],'column'=>$_POST['column'])));

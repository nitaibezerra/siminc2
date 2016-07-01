<?php
// monta cabeçalho
//include APPRAIZ . 'includes/cabecalho.inc';
include_once APPRAIZ."sca/classes/Visitante.class.inc";
$oVisitante = new Visitante();

$vttid = $_REQUEST['vttid'];

if(!empty($vttid) || !empty($nu_matricula_siape)){
    echo $oVisitante->recuperarFotoVisitante($vttid, $nu_matricula_siape);
}
?>
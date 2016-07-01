<?php
include APPRAIZ."fabrica/classes/Contrato.class.inc";

$entid = $_POST['entid'];

$c = new Contrato();
$result = $c->recuperaContratosPorEmpresa($entid);

echo simec_json_encode($result);

// termina aqui para não carregar os javascripts 
// inseridos de maneira incorreta
exit;
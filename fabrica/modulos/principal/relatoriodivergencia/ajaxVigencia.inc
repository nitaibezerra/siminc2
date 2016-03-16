<?php
include APPRAIZ."fabrica/classes/Contrato.class.inc";

$ctrid = $_POST['ctrid'];

$c = new Contrato();
$result = $c->recuperaVigenciaPorContrato($ctrid);

echo simec_json_encode($result);

// termina aqui para não carregar os javascripts 
// inseridos de maneira incorreta
exit;
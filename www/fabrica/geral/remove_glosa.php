<?php
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/Glosa.class.inc';
include APPRAIZ . 'fabrica/classes/OrdemServico.class.inc';

$glosa = new Glosa();
$retorno = $glosa->removeGlosa($_POST['glosaid'], $_POST['idOrdemServico']);
echo simec_json_encode($retorno);
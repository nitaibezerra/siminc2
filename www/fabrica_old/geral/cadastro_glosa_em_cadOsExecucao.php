<?php
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/Glosa.class.inc';
include APPRAIZ . 'fabrica/classes/OrdemServico.class.inc';

$glosa = new Glosa();
$glosa->setId($_POST['glosaid']);
$glosa->setDataInclusao($_POST['dataInclusao']);
$glosa->setJustificativa(utf8_decode($_POST['glosajustificativa']));
$glosa->setUsuarioResponsavel($_POST['cpfUsuarioResponsavel']);
$glosa->setValorEmPfComMascara($_POST['glosaqtdepf']);
$retorno = $glosa->salvar($_POST['idOrdemServico']);
echo  simec_json_encode($retorno);
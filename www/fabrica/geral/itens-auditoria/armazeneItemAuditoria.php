<?php
header('content-type: text/html; charset=iso-8859-1;');
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';

$itemAuditoriaRepositorio = new ItemAuditoriaRepositorio();

$itemAuditoria = new ItemAuditoria();
$itemAuditoria->setId(trim(base64_decode($_POST['idItemAuditoria'])));
$itemAuditoria->setNome(trim(utf8_decode($_POST['itemAuditoria'])));
$itemAuditoria->setDescricao(trim(utf8_decode($_POST['descricaoItemAuditoria'])));
$itemAuditoria->setSituacao(trim(utf8_decode($_POST['situacaoItemAuditoria']) == "A" ? "t" : "f"));
print simec_json_encode(array('status'=> $itemAuditoriaRepositorio->armazene($itemAuditoria)));


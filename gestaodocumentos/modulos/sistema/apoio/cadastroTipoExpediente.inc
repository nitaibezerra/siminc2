<?php 

include_once APPRAIZ . "gestaodocumentos/classes/GestaoDocumentosTemplates.class.inc";
include_once APPRAIZ . "gestaodocumentos/classes/TipoExpediente.class.inc";
$GestaoDocumentosTemplates = new GestaoDocumentosTemplates();
$TipoExpediente = new TipoExpediente();

if($_REQUEST['requisicao']) {
    $TipoExpediente->$_REQUEST['requisicao']($_REQUEST);
}

include_once APPRAIZ . "includes/cabecalho.inc"; 

$GestaoDocumentosTemplates->includesSistemaTabelasApoio();

$GestaoDocumentosTemplates->formCadastroTipoExpediente( $TipoExpediente );


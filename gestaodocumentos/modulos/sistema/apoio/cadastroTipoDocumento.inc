<?php 

include_once APPRAIZ . "gestaodocumentos/classes/GestaoDocumentosTemplates.class.inc";
include_once APPRAIZ . "gestaodocumentos/classes/TipoDocumento.class.inc";
$GestaoDocumentosTemplates = new GestaoDocumentosTemplates();
$TipoDocumento = new TipoDocumento();

if($_REQUEST['requisicao']) {
    $TipoDocumento->$_REQUEST['requisicao']($_REQUEST);
}

include_once APPRAIZ . "includes/cabecalho.inc"; 

$GestaoDocumentosTemplates->includesSistemaTabelasApoio();

$GestaoDocumentosTemplates->formCadastroTipoDocumento( $TipoDocumento );


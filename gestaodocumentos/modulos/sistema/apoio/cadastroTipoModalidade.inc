<?php 

include_once APPRAIZ . "gestaodocumentos/classes/GestaoDocumentosTemplates.class.inc";
include_once APPRAIZ . "gestaodocumentos/classes/TipoModalidade.class.inc";
$GestaoDocumentosTemplates = new GestaoDocumentosTemplates();
$TipoModalidade = new TipoModalidade();

if($_REQUEST['requisicao']) {
    $TipoModalidade->$_REQUEST['requisicao']($_REQUEST);
}

include_once APPRAIZ . "includes/cabecalho.inc"; 

$GestaoDocumentosTemplates->includesSistemaTabelasApoio();

$GestaoDocumentosTemplates->formCadastroTipoModalidade( $TipoModalidade );


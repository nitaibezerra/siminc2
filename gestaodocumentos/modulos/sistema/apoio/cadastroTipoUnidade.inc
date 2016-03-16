<?php 

include_once APPRAIZ . "gestaodocumentos/classes/GestaoDocumentosTemplates.class.inc";
include_once APPRAIZ . "gestaodocumentos/classes/TipoUnidade.class.inc";
$GestaoDocumentosTemplates = new GestaoDocumentosTemplates();
$TipoUnidade = new TipoUnidade();

if($_REQUEST['requisicao']) {
    $TipoUnidade->$_REQUEST['requisicao']($_REQUEST);
}
 
include_once APPRAIZ . "includes/cabecalho.inc"; 

$GestaoDocumentosTemplates->includesSistemaTabelasApoio();

$GestaoDocumentosTemplates->formCadastroTipoUnidade( $TipoUnidade );


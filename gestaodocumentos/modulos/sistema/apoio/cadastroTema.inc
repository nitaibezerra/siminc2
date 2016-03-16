<?php 

include_once APPRAIZ . "gestaodocumentos/classes/GestaoDocumentosTemplates.class.inc";
include_once APPRAIZ . "gestaodocumentos/classes/Tema.class.inc";
$GestaoDocumentosTemplates = new GestaoDocumentosTemplates();
$Tema = new Tema();

if($_REQUEST['requisicao']) {
    $Tema->$_REQUEST['requisicao']($_REQUEST);
}

include_once APPRAIZ . "includes/cabecalho.inc"; 

$GestaoDocumentosTemplates->includesSistemaTabelasApoio();

$GestaoDocumentosTemplates->formCadastroTema( $Tema );

?>
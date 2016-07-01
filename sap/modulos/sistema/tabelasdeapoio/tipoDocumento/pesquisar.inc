<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoDocumento/partialsControl/topo.inc';

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array( ABA_EDITAR_TIPO_DOCUMENTO );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Tipo de Documento', '');

// controller
extract($_POST);

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoDocumento/views/formPesquisa.inc';
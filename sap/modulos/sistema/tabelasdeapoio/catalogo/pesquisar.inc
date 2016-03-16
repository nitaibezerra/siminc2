<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/catalogo/partialsControl/topo.inc';

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array( ABA_EDITAR_CATALOGO );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Cat&aacute;logo','');

// controller
extract($_POST);

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/catalogo/views/formPesquisa.inc';
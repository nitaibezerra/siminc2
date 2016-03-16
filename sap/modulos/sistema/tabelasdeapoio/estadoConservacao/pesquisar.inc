<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/estadoConservacao/partialsControl/topo.inc';

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array( ABA_EDITAR_ESTADO_CONSERVACAO );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Estado de Conserva&ccedil;&atilde;o','');

// controller
extract($_POST);

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/estadoConservacao/views/formPesquisa.inc';
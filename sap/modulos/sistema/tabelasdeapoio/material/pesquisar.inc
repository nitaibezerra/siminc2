<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/partialsControl/topo.inc';

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array( ABA_EDITAR_MATERIAL );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Material', '');

// controller
extract($_POST);

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/views/formPesquisa.inc';
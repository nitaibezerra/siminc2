<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoEntradaSaida/partialsControl/topo.inc';

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array( ABA_EDITAR_TIPO_ENTRADASAIDA );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Tipos de Entrada/Sa&iacute;da','');

// controller
extract($_POST);

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoEntradaSaida/views/formPesquisa.inc';
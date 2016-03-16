<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoMovimentacao/partialsControl/topo.inc';

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array( ABA_EDITAR_TIPO_MOVIMENTACAO );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Tipo de Movimenta&ccedil;&atilde;o','');

// controller
extract($_POST);

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoMovimentacao/views/formPesquisa.inc';
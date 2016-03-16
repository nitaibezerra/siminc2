<?php
// topo
include_once APPSAP . 'modulos/principal/bensmovimentacao/partialsControl/topo.inc';

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array(ABA_EDITAR_MOVIMENTACAO_GERAL,ABA_EDITAR_MOVIMENTACAO_LOCALIDADE,ABA_EDITAR_MOVIMENTACAO_SIAPE);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o título da tela
monta_titulo('Listar Movimentação de Bens', '');

// controller
extract($_POST);
// definindo qual tipo de bem será mostrado
$filtroBenStatus = 'A';

// view
include_once APPSAP . 'modulos/principal/bensmovimentacao/views/formPesquisa.inc';

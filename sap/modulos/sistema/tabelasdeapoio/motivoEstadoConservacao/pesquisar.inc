<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/motivoEstadoConservacao/partialsControl/topo.inc';

// controller
extract($_POST);

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array( ABA_EDITAR_MOTIVO_ESTADO );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Motivos de Estados de Conserva&ccedil;&atilde;o', '');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/motivoEstadoConservacao/views/formPesquisa.inc';
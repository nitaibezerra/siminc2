<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/situacaoBem/partialsControl/topo.inc';

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array( ABA_EDITAR_SITUACAO_BEM );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Situa&ccedil;&atilde;o do Bem', '');

// controller
extract($_POST);

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/situacaoBem/views/formPesquisa.inc';
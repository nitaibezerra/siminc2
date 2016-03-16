<?php
// topo
include_once APPSAP . 'modulos/principal/baixadebens/partialsControl/topo.inc';

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array(ABA_EDITA_BENS_BAIXA,ABA_ADICIONA_BENS_BAIXA_ABERTO);
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Baixa de Bens','');

// controller
extract($_POST);
// definindo qual tipo de bem será mostrado
$filtroBenStatus = 'A';

// view
include_once APPSAP . 'modulos/principal/baixadebens/views/formPesquisa.inc';
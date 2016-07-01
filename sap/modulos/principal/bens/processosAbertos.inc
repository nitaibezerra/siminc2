<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topo.inc';

unset($_SESSION['benid'],$_SESSION['bmtid']);

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array(ABA_EDITAR_BEM_REDIRECIONA,ABA_INCLUIR_BEM,ABA_EDICAO_TOMBAMENTO,ABA_TOMBAMENTO_ITEM,ABA_EDICAO_ITEM_PROCESSO,ABA_INCLUSAO_EDICAO_TOMBAMENTO,ABA_INCLUSAO_EDICAO_ITEM_PROCESSO,ABA_EDITAR_BEM_ETAPA2,ABA_EDITAR_BEM_ABERTO,ABA_EDITAR_BEM,ABA_ADICIONAR_BEM_MATERIAL,ABA_ADICIONAR_BEM_RGP);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o título da tela
monta_titulo('Processos Abertos de Entrada de Bens','');

// controller
extract($_POST);
// definindo qual tipo de bem será mostrado
$filtroBenStatus = 'I';

// view
include_once APPSAP . 'modulos/principal/bens/views/formPesquisaBem.inc';

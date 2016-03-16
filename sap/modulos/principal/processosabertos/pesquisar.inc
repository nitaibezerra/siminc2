<?php
// topo
include_once APPSAP . 'modulos/principal/processosabertos/partialsControl/topo.inc';


// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// monta o título da tela
monta_titulo('Processos Abertos','');

// controller
extract($_POST);

// view
include_once APPSAP . 'modulos/principal/processosabertos/views/formPesquisa.inc';
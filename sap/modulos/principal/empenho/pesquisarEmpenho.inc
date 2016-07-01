<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topo.inc';

// monta o título da tela
monta_titulo('Nota de Empenho', '');

// controller
extract($_POST);

// view
include_once APPSAP . 'modulos/principal/empenho/views/formPesquisaEmpenho.inc';
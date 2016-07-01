<?php
// topo
include_once APPSAP . 'modulos/principal/fornecedor/partialsControl/topo.inc';

// monta o título da tela
monta_titulo('Fornecedor', '');

// controller
extract($_POST);

// view
include_once APPSAP . 'modulos/principal/fornecedor/views/formPesquisaFornecedor.inc';
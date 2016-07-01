<?php

// topo
include_once APPSAP . 'modulos/principal/endereco/partialsControl/topo.inc';

// monta o título da tela
monta_titulo('Consulta Cep', 'Filtros de Pesquisa');

// controller
extract($_POST);

// view
include_once APPSAP . 'modulos/principal/endereco/views/formPesquisaCep.inc';
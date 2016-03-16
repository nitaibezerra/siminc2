<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/partialsControl/topoPesquisaMaterial.inc';

// monta o título da tela
monta_titulo('Consulta Material', '');

// controller
extract($_POST);

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/views/formPesquisaMaterial.inc';
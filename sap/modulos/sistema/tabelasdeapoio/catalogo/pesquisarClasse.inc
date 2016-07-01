<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/catalogo/partialsControl/topoClasse.inc';



// monta o título da tela
monta_titulo('Busca de Classe', '');

// controller
extract($_POST);

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/catalogo/views/formPesquisaClasse.inc';
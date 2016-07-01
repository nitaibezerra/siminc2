<?php

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// incluindo as classes necessarias
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPPISO . 'classes/GestaoRecursos.class.inc';
$obGestao = new GestaoRecursos();
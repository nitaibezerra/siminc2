<?php
// topo
include_once APPSAP . 'modulos/principal/patrimonio/partialsControl/topo.inc';

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// monta o título da tela
monta_titulo('Consulta Patrimônio', '<b>Filtros de Pesquisa</b>');
extract( $_POST );
$bebdata = (!empty( $bebdata ) ? formata_data_sql( $bebdata ) : '');
$bendtentrada = (!empty( $bendtentrada ) ? formata_data_sql( $bendtentrada ) : '');
$bendtproc = (!empty( $bendtproc ) ? formata_data_sql( $bendtproc ) : '');

// view
include_once APPSAP . 'modulos/principal/patrimonio/views/formPesquisaPatrimonio.inc';


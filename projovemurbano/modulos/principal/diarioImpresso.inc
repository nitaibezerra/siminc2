<?php
header('Content-Type: text/html; charset=iso-8859-1');

if( $_REQUEST['tipo'] == 'D' )
{
    include_once APPRAIZ .'projovemurbano/modulos/principal/diario_frequencia_componente_curricular.php';
}
else
{
    include_once APPRAIZ .'projovemurbano/modulos/principal/diario_trabalho_componente_curricular.php';
}
    


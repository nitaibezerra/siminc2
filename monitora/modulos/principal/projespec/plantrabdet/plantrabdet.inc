<?php
//error_reporting( E_ALL );
$pjeid = $_REQUEST['id'];
if (! is_numeric($pjeid)) $pjeid=$_REQUEST['id'];
if ($_REQUEST['id'])  $_SESSION['pjeid']= $pjeid;

define( "APP_PLANO_TRABALHO" , APPRAIZ . "includes/planodetrabalho/tarefa_pt/" );

require_once( APP_PLANO_TRABALHO . 'arquivos_requeridosPT.inc' );
require_once( APP_PLANO_TRABALHO . "lista_tarefas_do_projetoPT.inc" );
?>
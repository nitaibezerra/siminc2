<?php 

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
$db = new cls_banco();

function fechaDb()
{
    global $db;
    $db->close();
}

register_shutdown_function('fechaDb');

//if(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'listar_proc'){		
	
//	$unpid = $_REQUEST['unpid'];
//	
//	$sqlProcedencia = "SELECT proid as codigo,	prodsc as descricao 
//					   FROM conjur.procedencia
//					   WHERE unpid = '".$unpid."' AND prodtstatus = 'A'
//					   ORDER BY	prodsc";	
//	$proid = $_REQUEST['proid'];
//	
//	die($db->monta_combo('proid', $sqlProcedencia, 'S', 'Selecione...', '', '', '', '', 'S','proid'));
	
//	$prodsc = $_REQUEST['prodsc'];
//	echo campo_texto('prodsc', 'N', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="prodsc"');
//	die;
	
//}

?>
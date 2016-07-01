<?php
function initialization() {
	if ( $_REQUEST['filtrosession'] ){
		$filtroSession = $_REQUEST['filtrosession'];
	}
	if ($_POST['agrupador']){
		header( 'Content-Type: text/html; charset=iso-8859-1' ); 
	}
	ini_set("memory_limit","350M");
	set_time_limit(0);
}

global $filtroSession; 
initialization();
  
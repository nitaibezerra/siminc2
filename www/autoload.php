<?php

function __autoload($class_name) {
	$arCaminho = array(
						APPRAIZ . "includes/classes/",
						APPRAIZ . "includes/classes/modelo/entidade/",
						APPRAIZ . "includes/classes/modelo/obras2/",
						APPRAIZ . "includes/classes/modelo/obras2/demanda/",
						APPRAIZ . "includes/classes/modelo/territorios/",
						APPRAIZ . "includes/classes/modelo/public/"
					  );
					  
	foreach($arCaminho as $caminho){
		$arquivo = $caminho . $class_name . '.class.inc';
		if ( file_exists( $arquivo ) ){
			require_once( $arquivo );
			break;	
		}
	}				  
}


<?php
	
	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	
	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();
	
	function funcao( $ptoid ){
		static $total = 1;
		
		$sql = sprintf( "SELECT ptoid_pai FROM monitora.planotrabalho WHERE ptoid = '%s'", $ptoid );
		$ptoid_pai = $db->pegaUm();
		if ( $ptoid_pai ) {
			$total++;
			funcao( $ptoid_pai );
		}
		return $total;
	}

	dbg( funcao( 377 ) );

?>
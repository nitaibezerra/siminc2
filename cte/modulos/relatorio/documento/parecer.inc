<?php
/*
function pegarPagina( $endereco, $mensagem ) {
		$snoopy = new Snoopy();
	$resultado = $snoopy->fetch( $endereco );
	if( $snoopy->status == "200" ) {
		return $snoopy->results;
	} else {
		throw new Exception( $mensagem );
	}
}
*/
//cte_emitirParecer( $_SESSION["inuid"] );
//$db->commit();

echo cte_pegarParecer( $_SESSION["inuid"] );

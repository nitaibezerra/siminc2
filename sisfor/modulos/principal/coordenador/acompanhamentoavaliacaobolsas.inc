<?php



if(!$iusd_aco) {

	$iusd_aco = $db->pegaUm("SELECT i.iusd FROM sisfor.sisfories s 
							 INNER JOIN sisfor.identificacaousuario i ON i.iuscpf = s.usucpf 
							 WHERE s.unicod='".$_SESSION['sisfor']['unicod']."'");

}


include_once APPRAIZ_SISFOR."acompanharavaliacaobolsas.inc";

?>
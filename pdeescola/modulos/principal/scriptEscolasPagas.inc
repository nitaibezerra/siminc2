<?php
include_once("pdeWs.php");

//begin single test 
//	$coProgramaFNDE = 96;
//	$ws = new pdeWs();
//	$pdeidPago = $ws->pdeEscolaWs( 'IsEscolaPaga', '2009', '26050161', $coProgramaFNDE );
//	echo $pdeidPago;
//	die();
//end

ini_set("memory_limit","5024M");
set_time_limit(0);

$sql = "SELECT * FROM pdeescola.pdeescola WHERE pdepafretorno IS NOT NULL";
$rs  = $db->carregar( $sql );
	echo"<h2>Escolas Pagas - WebService</h2>";
	echo"<table width=\"800\" class=\"listagem\">";
	echo"<tr>";
		echo"<th>ENTID</th><th>COD.INEP</th><th>PDEID</th><th>NOME</th>";
	echo"</tr>";
	 
	for( $i = 0; $i< count( $rs ); $i++ ){
		
		$entcodent 		= pegarEntCodEnt( $rs[$i]['entid'] );
		$coProgramaFNDE = 96;
		$ws[$i] = new pdeWs();
		$pdeidPago[$i] = $ws[$i]->pdeEscolaWs( 'IsEscolaPaga', '2009', $entcodent, $coProgramaFNDE );
		if( $pdeidPago[$i] == '1'){
			$sql = "UPDATE pdeescola.pdeescola SET pdepafretorno = 'now()' WHERE pdeid = {$rs[$i]['pdeid']};";
			$db->executar($sql );
	 		echo"<tr>";
	 			echo"<td> ".$rs[$i]['entid']."";
	 			echo"<td> ".$entcodent."";
	 			echo"<td> ".$rs[$i]['pdeid']."";
	 			echo"<td> ".$rs[$i]['pdenome']."";
	 		echo"</tr>";	 
	 	}else{
	 		$sql = "UPDATE pdeescola.pdeescola SET pdepafretorno = NULL WHERE pdeid = {$rs[$i]['pdeid']};";
			$db->executar($sql ); 
	 	}
	}
	$db->commit();
	echo"</table>";

?>
<?php

function filtraMunicipioEBS($estuf){
	global $db;
	$sql = "SELECT
				ter.muncod AS codigo,
				ter.mundescricao AS descricao
			FROM
				territorios.municipio ter
			WHERE
				ter.estuf = '$estuf'
			ORDER BY ter.mundescricao";

	echo $db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '',"","","","N","","");
}

?>
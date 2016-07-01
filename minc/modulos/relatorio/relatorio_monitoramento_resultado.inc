<?php

ini_set("memory_limit", "2048M");
// set_time_limit(120);

include APPRAIZ. 'includes/classes/relatorio.class.inc';

function monta_sql( $superuser ){
	
	global $db;
	
	extract($_REQUEST);
	$where = array();
	
	// monta o sql 
	$sql = "SELECT
				'A escola começou a preencher a aba Monitoramento?' as monitoramento,
				CASE
						WHEN moninicioupreenchimento = 't'
						THEN 'SIM'
						ELSE 'NÂO'
				END AS preenchimento,
				ende.estuf as estuf,
				mun.mundescricao as municipio,
				entcodent||'-'||entnome as escola,
				1 as qtd
			FROM
				minc.mcemaiscultura mce
			INNER JOIN workflow.documento doc ON doc.docid = mce.docid
			INNER JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
			INNER JOIN minc.monitoramento mon ON mon.mceid = mce.mceid
			INNER JOIN entidade.entidade ent ON ent.entid = mce.entid 
			INNER JOIN entidade.endereco ende ON ende.entid = ent.entid 
			INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod 
			WHERE
				mce.mcestatus = 'A'
			AND 	doc.esdid = 1105";
//             ver($sql,d);
	return $sql;
}

function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array("agrupador" => array(),
				 "agrupadoColuna" => array( "escola",
											"qtd" ) );
	
	array_push($agp['agrupador'], array("campo" => "monitoramento",
								 		"label" => "A escola começou a preencher a aba Monitoramento?") );					
	array_push($agp['agrupador'], array("campo" => "preenchimento",
								 		"label" => "Resposta") );					
	array_push($agp['agrupador'], array("campo" => "estuf",
								 		"label" => "Estado") );					
	array_push($agp['agrupador'], array("campo" => "municipio",
								 		"label" => "Municipio") );					
	array_push($agp['agrupador'], array("campo" => "escola",
								 		"label" => "Escola") );					
	return $agp;
}

function monta_coluna(){
	
	$coluna = array(	
					array(
						  "campo" => "qtd",
				   		  "label" => "Total",
					   	  "type"  => "numeric"	
					),		
			);
					  	
	return $coluna;			  	
}
?>
</body>
</html>
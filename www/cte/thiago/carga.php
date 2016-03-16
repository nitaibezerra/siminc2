<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();
pg_set_client_encoding($db->link,'UTF8');

/*
$sql = 'SELECT "TIPO DE PONTO" as tipo FROM carga.pontocultura';
$dados = $db->carregar($sql);
$sql = "insert into pdeescola.metipopontocultura (mtpdescricao)
		values ( ".$dado['tipo']." )";
*/

$sql = 'SELECT "PONTO DE CULTURA" as pontodecultura , "DESCRIวรO" as descricao, "ID_" as id, "ID" as idseg , * FROM carga.pdepontocultura';
$dados = $db->carregar($sql);
foreach($dados as $dado){
	$sql = "INSERT INTO carga.enderecoteste
			(entid, tpeid, endcep, endlog, muncod, estuf, endstatus, endcom, medlatitude, medlongitude)
			SELECT 	null 		AS entid, 
				null 		AS tpeid, 
				CASE WHEN  \"CEP\" = '' THEN null	ELSE \"CEP\" END AS endcep, 
				\"LOGRADOURO\"	AS endlog, 
				\"CODMUN\" 	AS muncod,
				\"UF\" 		AS estuf,
				'A' 		AS endstatus,
				\"COMPLEMENTO\" 	AS endcom,
				CASE WHEN substring(\"LATITUDE\", 1, 1) = '-' 
					THEN 
						CASE WHEN  substring(\"LATITUDE\", 2) = NULL OR substring(\"LATITUDE\", 2) = ''  
						THEN '00000000' 
						ELSE  	substring(to_char(substring(\"LATITUDE\",2)::numeric,'00000000'),2,2) ||'.'||  
							substring(to_char(substring(\"LATITUDE\",2)::numeric,'00000000'),4,2) ||'.'||
							substring(to_char(substring(\"LATITUDE\",2)::numeric,'00000000'),6,2) ||'.S' 
						END 
					ELSE 
						CASE WHEN  substring(\"LATITUDE\", 2) = NULL OR substring(\"LATITUDE\", 2) = ''  
						THEN '00000000' 
						ELSE  	substring(to_char(\"LATITUDE\"::numeric,'00000000'),2,2) ||'.'||  
							substring(to_char(\"LATITUDE\"::numeric,'00000000'),4,2) ||'.'||
							substring(to_char(\"LATITUDE\"::numeric,'00000000'),6,2) || '.N' 
						END 
				END AS medlatitude,
				CASE WHEN substring(\"LONGITUDE\", 1, 1) = '-' 
					THEN 
						CASE WHEN  substring(\"LONGITUDE\", 2) = NULL OR substring(\"LONGITUDE\", 2) = ''  
						THEN '00000000' 
						ELSE  	substring(to_char(substring(\"LONGITUDE\",2)::numeric,'00000000'),2,2) ||'.'||  
							substring(to_char(substring(\"LONGITUDE\",2)::numeric,'00000000'),4,2) ||'.'||
							substring(to_char(substring(\"LONGITUDE\",2)::numeric,'00000000'),6,2) ||'.W' 
						END 
					ELSE 
						CASE WHEN  substring(\"LONGITUDE\", 2) = NULL OR substring(\"LONGITUDE\", 2) = ''  
						THEN '00000000' 
						ELSE  	substring(to_char(\"LONGITUDE\"::numeric,'00000000'),2,2) ||'.'||  
							substring(to_char(\"LONGITUDE\"::numeric,'00000000'),4,2) ||'.'||
							substring(to_char(\"LONGITUDE\"::numeric,'00000000'),6,2) || '.O' 
						END 
				END AS medlongitude
			FROM carga.pontocultura 
			WHERE \"ID\" = ".$dado['idseg']." order by \"ID\"
			returning endid";
	dbg($sql,1);
	//$endid = $db->executar($sql);
	$sql = "insert into pdeescola.mepontocultura (mpcnome,mpcdescricao,mpccod,endid)
			values (".$dado['pontodecultura'].", ".$dado['descricao'].", ".$dado['id'].",".$endid." )";
	//$db->executar($sql);
	dbg($sql,1);
}
//$db->
/*
	
*/

?>
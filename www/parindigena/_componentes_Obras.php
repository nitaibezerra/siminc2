<?php

/**
 * Enter description here...
 *
 * @param integer $obrid
 * @return integer
 */
function obras_percentual_executado($obrid){
	
	global $db;
	
	$percentual = $db->carregar("
						SELECT ROUND(SUM(icopercexecutado),2) AS total 
						FROM obras.itenscomposicaoobra WHERE obrid =".$obrid);
	
	return $percentual[0]["total"];

}

/**
 * Enter description here...
 *
 * @param unknown_type $obrid
 * @return unknown
 */
function obras_ver_obras($obrid){
	
	global $db;
	
	$res = $db->executar("
		SELECT 
			ent.entnome as entidade,
			oie.obrdescundimplantada as unidade,
			oie.obrdesc as nome,
			oie.obrcustocontrato,
			org.orgdesc as orgao,
			ROUND(oie.obrpercexec,0)||'%' as executado
		FROM 
			(obras.obrainfraestrutura oie INNER JOIN obras.orgao org ON oie.orgid=org.orgid)
		INNER JOIN 
			entidade.entidade ent ON oie.entidunidade = ent.entid  
		WHERE 
			oie.obsstatus = 'A' and oie.obrid=".$obrid);
	
	return pg_fetch_assoc($res);

}

/**
 * Funзгo que formata o cep antes de inserir no banco 
 *
 * @param string $cep
 * @return string
 */
function obras_formata_cep($cep){
	$cep = str_replace("-", "", $cep);
	$cep = str_replace(".", "", $cep);
	return $cep;
}

/**
 * Funзгo que formata os nъmeros antes de inserir no banco
 *
 * @param string $numero
 * @return string
 */
function obras_formata_numero($numero){
	$numero = str_replace(".","",$numero);
	$numero = str_replace(",",".",$numero);
	return $numero;
}

?>
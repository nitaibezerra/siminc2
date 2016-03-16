<?php
set_time_limit(0);
ini_set("memory_limit", "12000M");

global $db;

$sql = "SELECT 
			itc.itcid,
			pto.ptodescricao,
			itc.itcdescricao, 
			pto.ptoid as ptoid, 
			itc.itcvalorunitario, 
			itc.umdid, 
			itc.itcquantidade, 
			itc.itccodigoitem, 
			itc.itccodigoitemcodigo, 
			itc.itcitemsigarp
		FROM 
			obras.preitenscomposicao itc 
		INNER JOIN obras.pretipoobra pto ON pto.ptoidpaiemenda = itc.ptoid AND pto.ptoid NOT IN (45, 44)
		WHERE
			itcidpai is null AND
			itcstatus = 'A'
		ORDER BY
			pto.ptoid, itccodigoitemcodigo";

$dados = $db->carregar($sql);

$str = "";
$vlr = "";

carregaFilho($dados);

function carregaFilho( $dados, $pai = null){
	
	global $db;
	
	if( is_array($dados) ){
		foreach( $dados as $dado ){
			
			$str = "";
			$vlr = "";

			if( $dado['umdid'] != '' ){
				$str = "umdid, ";
				$vlr = $dado['umdid'].", ";
			}
			if( $pai ){
				$str .= "itcidpai, ";
				$vlr .= $pai.", ";
			}
			if( $dado['itcquantidade'] != '' ){
				$str .= "itcquantidade, ";
				$vlr .= $dado['itcquantidade'].", ";
			}
			
			$sql = "INSERT INTO obras.preitenscomposicao (itcdescricao, itcstatus, itcdtinclusao, ptoid, itcvalorunitario, {$str} itccodigoitem, itccodigoitemcodigo) 
					VALUES ('".str_replace("'", "", $dado['itcdescricao'])."', 'A', NOW(), ".$dado['ptoid'].", '".trim($dado['itcvalorunitario'])."', {$vlr} '".$dado['itccodigoitem']."', '".$dado['itccodigoitemcodigo']."') RETURNING itcid";
			$itcid = $db->pegaUm($sql);
			

			$sql = "SELECT 
						itc.itcid,
						pto.ptodescricao,
						itc.itcdescricao, 
						pto.ptoid as ptoid, 
						itc.itcvalorunitario, 
						itc.umdid, 
						itc.itcquantidade, 
						itc.itccodigoitem, 
						itc.itccodigoitemcodigo, 
						itc.itcitemsigarp
					FROM 
						obras.preitenscomposicao itc 
					INNER JOIN obras.pretipoobra pto ON pto.ptoidpaiemenda = itc.ptoid AND pto.ptoid NOT IN (45, 44)
					WHERE
						itcidpai = ".$dado['itcid']." AND
						itcstatus = 'A'
					ORDER BY
						pto.ptoid, itccodigoitemcodigo";
			
			$dadosFilhos = $db->carregar($sql);
			
			if(is_array($dadosFilhos)){
				carregaFilho($dadosFilhos, $itcid);
			}
		}
	}
}

$db->commit();

echo "FIM:".date("d/m/Y h:i:s");
?>
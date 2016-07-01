<?php

	set_time_limit( 0 );
	
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";	
	
	$db = new cls_banco();	

	$sql = "
			select distinct ea.esaid, r.mundescricao
			from rel_escolaativasituacao r
				inner join cte.escolaativa ea on ea.esaid = r.esaid
			where 
			(
				coalesce( totalescolas, 0 ) != 0 
				or   coalesce( totalpredios, 0 ) != 0 
				or   coalesce( totalturmas , 0 ) != 0 
				or   coalesce( totalalunos , 0 ) != 0 
			)
			and ea.docid is null";
	
	$resultado = $db->carregar( $sql );
	$coEsaid = $resultado ? $resultado : array();
	
	foreach( $coEsaid as $count => $arEsaid ){
			
		$sql = "insert into workflow.documento ( tpdid, esdid, docdsc )
										values ( 7, 47, 'Escola Ativa - ".str_replace( "'", "", $arEsaid["mundescricao"] )."' )
				returning docid";
				
		$docid = $db->pegaUm( $sql );
		$db->commit();		
				
		$sql = "update cte.escolaativa set docid = $docid where esaid = {$arEsaid["esaid"]} ";
		$acao = "Criado Docid = $docid";
		
		$db->executar($sql);		
		$db->commit();		
		
		
		echo ( $count + 1 )." -> {$arEsaid["mundescricao"]} --> OK ( $acao ) <br />";
	}

?>
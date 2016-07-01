<?php

	set_time_limit( 0 );
	
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";	
	
	$db = new cls_banco();	

	$sql = "select distinct eaa.inuid, ea.esaid, e.mundescricao, docid
			from cte.escola_ativa_analisada eaa
				inner join cte.escola_ativa e on e.inuid = eaa.inuid
				inner join cte.escolaativa ea on ea.inuid = eaa.inuid
			where eaa.analisada >= 10";
	
	$resultado = $db->carregar( $sql );
	$coEsaid = $resultado ? $resultado : array();
	
	foreach( $coEsaid as $count => $arEsaid ){
		
		if( $arEsaid["docid"] ){
			$sql = "update workflow.documento set esdid = 48 where docid = {$arEsaid["docid"]}";
			$acao = "Alterado Docid = {$arEsaid["docid"]}";
		}
		else{
			
			$sql = "insert into workflow.documento ( tpdid, esdid, docdsc )
											values ( 7, 50, 'Escola Ativa - ".str_replace( "'", "", $arEsaid["mundescricao"] )."' )
					returning docid";
					
			$docid = $db->pegaUm( $sql );
			$db->commit();		
					
			$sql = "update cte.escolaativa set docid = $docid where esaid = {$arEsaid["esaid"]} ";
			$acao = "Criado Docid = $docid";
		}
		$db->executar($sql);		
		$db->commit();		
		
		echo ( $count + 1 )." -> {$arEsaid["mundescricao"]} --> OK ( $acao ) <br />";
	}

?>
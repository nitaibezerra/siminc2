<?php

	include "config.inc";
	include APPRAIZ."includes/classes_simec.inc";
	include APPRAIZ."includes/funcoes.inc";

	$db = new cls_banco();

	$arAnos = array( 2007, 2008, 2009, 2010, 2011 );
	
	$sql = "select distinct su.sbaid, ppsparecerpadrao
			from cte.subacaoindicador su 
				inner join cte.proposicaosubacao ps on su.ppsid = ps.ppsid
				inner join cte.acaoindicador ai on ai.aciid = su.aciid
				inner join cte.pontuacao p on p.ptoid = ai.ptoid
				inner join cte.instrumentounidade iu on iu.inuid = p.inuid
				inner join workflow.documento d on d.docid = iu.docid
			where p.ptostatus = 'A'
			and ppsindcobuni = true
			and d.esdid = 10";
			
	$coSubacoesUniversais = $db->carregar( $sql );
	
	$stSbaids = "";
	$inserts  = "";
	if( is_array( $coSubacoesUniversais ) ){

		foreach( $coSubacoesUniversais as $arSubacaoUniversal ){
			
			$stSbaids .= $arSubacaoUniversal["sbaid"].", ";
			
			foreach( $arAnos as $ano ){
												
				$inserts .= " 
							 ( {$arSubacaoUniversal["sbaid"]}, '{$arSubacaoUniversal["ppsparecerpadrao"]}', $ano, 3, 0, 0, 0 ), ";
					
			}
		}
		
		$stSbaids = substr( $stSbaids, 0, -2 );
		$inserts  = substr( $inserts , 0, -2 );
		
		$sql = "DELETE FROM cte.subacaoparecertecnico
				WHERE sbaid in ( $stSbaids );
				
				DELETE FROM cte.subacaobeneficiario
				WHERE sbaid in ( $stSbaids );
				
				DELETE FROM cte.composicaosubacao
				WHERE sbaid in ( $stSbaids );
				
				DELETE FROM cte.qtdfisicoano
				WHERE sbaid in ( $stSbaids );";
		
					
		if( !$db->executar( $sql ) ) {
			$db->rollback();
			echo "Erro";
			die();
		}

		$sql = "insert into cte.subacaoparecertecnico  ( sbaid, sptparecer, sptano, ssuid, sptinicio, sptfim, tppid ) 
												values $inserts";
																
		if( !$db->executar( $sql ) ) {
			$db->rollback();
			echo "Erro";
			die();
		}		
	
		$db->commit();
		echo "OK";
	}

?>
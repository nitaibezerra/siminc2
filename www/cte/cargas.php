<?php
	
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	include APPRAIZ . "www\cte\_funcoes.php";
	
	$db = new cls_banco();
	
/*	
	$res = $db->carregar( $sql );
	$linha = "";
	foreach( $res as $arResultado ){
		$linha .= $arResultado["sbaid"].", ";
	}
*/	
	
	/*$docid = cte_pegarDocid( $_SESSION['inuid'] );
	$estado_documento = wf_pegarEstadoAtual( $docid );
	
	// verifica se é Estado
	if ( cte_pegarItrid( $inuid ) == INSTRUMENTO_DIAGNOSTICO_ESTADUAL   ) {
		return true;
	}
	*/

	// Recuperando todas as subações
	$sql = "select distinct sai.sbaid
			from cte.subacaoindicador sai
				inner join cte.proposicaosubacao ps on sai.ppsid = ps.ppsid
				inner join cte.subacaoparecertecnico spt on spt.sbaid = sai.sbaid
				inner join cte.acaoindicador ai on ai.aciid = sai.aciid
				inner join cte.pontuacao p on p.ptoid = ai.ptoid
				inner join cte.instrumentounidade iu on iu.inuid = p.inuid
				inner join workflow.documento d on d.docid = iu.docid
			WHERE iu.itrid 		= 2 						-- municipal e subações do par
			and p.ptostatus 	= 'A'
			and sai.sbaporescola 	= false 				-- so globais
			and coalesce( ps.ppsparecerpadrao, '' ) != '' 	-- que tem parecer padrão
			and lower( sai.sbadsc ) like '%realizar levantamento da situação escolar, inclusive nas escolas indígenas e do campo%'
			and d.esdid in ( 10, 11, 13, 14, 15 ) 			-- a partir de tecnica
			-- and sai.sbaid = 580655
			and sai.sbaid > 580655
			order by sai.sbaid
			";

	$res = $db->carregar( $sql );
	
	// Criando o array onde serão armazenadas as validações 
	$arAnalisado = array();
	
	if( is_array( $res ) ){
		
		// Para cada subação verifica-se se esta está validada
		foreach( $res as $i => $subacao ){
			echo $i." --> ".$subacao["sbaid"]."<br />";
			$sql = "select sbaid, frmid, sbaporescola from cte.subacaoindicador where sbaid = ". $subacao["sbaid"];
			$subacao = $db->pegaLinha( $sql );
			
			//$fase = cte_possuiFormaExecucaoTecnica( $subacao["frmid"] ) ? FORMA_EXECUCAO_ASS_TEC : FORMA_EXECUCAO_ASS_FIN;

			// Array com o resultado das validações separados pela fase (Assistência Técnica ou Financeira).
			$arAnalisado[$subacao['sbaid']] = cte_validarSubAcaoFaseAnalise( $subacao );
			
			echo $i." --> ".$subacao["sbaid"]."<br />";
			
		} // Fim de foreach( $res as $subacao )

	} // Fim de if( is_array( $res ) )

	dbg( $arAnalisado ); 
	$novo[] = array_keys( $arAnalisado[4], false, true ); 
	dbg( $novo );

		
?>
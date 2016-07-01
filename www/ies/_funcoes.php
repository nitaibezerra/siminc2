<?php

function iesPossuiPerfil( $pflcods ){
	
	global $db;
	
	if ($db->testa_superuser()) {
		return true;
	}else{
		
		if ( is_array( $pflcods ) ){
			$pflcods = array_map( "intval", $pflcods );
			$pflcods = array_unique( $pflcods );
		}else{
			$pflcods = array( (integer) $pflcods );
		}
		
		if ( count( $pflcods ) == 0 ){
			return false;
		}
		
		$sql = "SELECT
					count(*)
				FROM 
					seguranca.perfilusuario
				WHERE
					usucpf = '" . $_SESSION['usucpf'] . "' AND
					pflcod in ( " . implode( ",", $pflcods ) . " ) ";
		
		return $db->pegaUm( $sql ) > 0;
			
	}
	
}


function iesVerificaPermissaoIes( $usucpf ){
	
	global $db;
	
	$sql = "SELECT iesid FROM ies.usuarioresponsabilidade 
			WHERE usucpf = '{$usucpf}' AND rpustatus = 'A' AND pflcod = " . IES_CADASTRADORIES;
	
	return $db->pegaUm( $sql );
	
}

function iesVerificaIes( $iesid ){
	
	global $db;
	
	$sql = "SELECT iesid FROM ies.ies WHERE iesid = {$iesid}";
	
	return $db->pegaUm( $sql );
	
}

function iesPegaResponsavel( $iesid ){
	
	global $db;
	
	$sql = "SELECT entidresponsavel FROM ies.projetobndesies WHERE iesid = {$iesid} AND pbistatus = 'A'";
	
	return $db->pegaUm( $sql );
	
}

function iesPegaProjeto( $iesid ){
	
	global $db;
	
	$sql = "SELECT pbiid FROM ies.projetobndesies WHERE iesid = {$iesid} AND pbistatus = 'A'";
	
	return $db->pegaUm( $sql );
	
}

function iesVerificaAnexo( $pbiid ){
	
	global $db;
	
	if ( $pbiid ){
		$sql = "SELECT count(aprid) FROM ies.arquivosprojeto WHERE pbiid = {$pbiid}";
		$aprid = $db->pegaUm( $sql );
	}
	
	return $aprid;
	
}

/********* Funчѕes do Workflow *********/

function validaCriterios( $pbiid, $acao, $iesid ){
	
	global $db;
	
	$entid = iesPegaResponsavel( $iesid );
	
	if ( !iesVerificaAnexo( $pbiid ) || $acao == 'A' || $entid || $db->testa_superuser() ){
		return true;	
	}else{
		return false;
	}
	
}

function iesCriaProtocolo( $iesid, $pbiid ){
	
	global $db;
	
	$sql = "SELECT max(pbiid), pbiprotocolo FROM ies.projetobndesies WHERE iesid = {$iesid} AND pbistatus = 'I' group by pbiprotocolo";
	$dados = $db->pegaLinha( $sql );
	
	$pbiprotocolo = $dados["pbiprotocolo"];
	

	
	if ( !empty($pbiprotocolo) ){
		
		$posicao1 = strpos( $pbiprotocolo, '.' );
		$num1 	  = substr( $pbiprotocolo, $posicao1 + 1 );
		
		$posicao2 = strpos( $num1, '.' );
		$valor 	  = substr( $num1, 0, $posicao2 );
			
	}else{
		$valor = 0;
	}
	
	$protocolonovo = $iesid . '.' . ($valor + 1) . '.' . Date('YmdGis');
	
	$sql = "UPDATE ies.projetobndesies SET pbiprotocolo = '{$protocolonovo}' WHERE pbiid = {$pbiid}";
	
	$db->executar( $sql );
	$db->commit();
	
	return true;
	
}

function iesVerificaEstado( $esdid ){
	
	global $db;
	
	$sql = "SELECT esdid FROM workflow.estadodocumento WHERE esdid = {$esdid}";
	
	return $db->pegaUm( $sql );
	
}

function iesCriarDocumento( $pbiid ) {
	
	global $db;
	
	$docid = iesPegarDocid( $pbiid );
	
	if( !$docid ) {
		
		// recupera o tipo do documento
		$tpdid = IES_TIPO_DOCUMENTO;
		
		// descriчуo do documento
		$docdsc = "Fluxo do IES (ies) - nА" . $pbiid;
		
		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza o projeto da IES
		$sql = "UPDATE
					ies.projetobndesies
				SET 
					docid = {$docid} 
				WHERE
					pbiid = {$pbiid}";

		$db->executar( $sql );
		$db->commit();
	}
	
	return $docid;
	
}

function iesPegarDocid( $pbiid ) {
	
	global $db;
	
	$sql = "SELECT
				docid
			FROM
				ies.projetobndesies
			WHERE
			 	pbiid = " . (integer) $pbiid;
	
	return (integer) $db->pegaUm( $sql );
	
}

function iesPegarEstadoAtual( $pbiid ) {
	
	global $db; 
	
	$docid = iesPegarDocid( $pbiid );
	 
	$sql = "select
				ed.esdid
			from 
				workflow.documento d
			inner join 
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				d.docid = " . $docid;
	
	$estado = (integer) $db->pegaUm( $sql );
	 
	return $estado;
	
}

function iesPegarNomeEstado( $esdid ){
	
	global $db;
	
	$sql = "SELECT esddsc FROM workflow.estadodocumento WHERE esdid = {$esdid}";
	
	return $db->pegaUm( $sql );
	
}

?>
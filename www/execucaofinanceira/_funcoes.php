<?php

function verificaPermissaoPerfil($categoria = 'geral') {
	global $db;
	$retorno = '';
	if( empty($categoria) ) $categoria = 'geral';
	$retorno = possuiPermissao($categoria);
	
	if( !$retorno ) {
		$retorno = 'disabled="disabled"';
	}
	return $retorno;
}

function possuiPermissao($categoria = 'geral') {
	global $db;
	include_once APPRAIZ . "www/execucaofinanceira/permissoesPerfil.php";

	if( empty($categoria) ) $categoria = 'geral';
	$pflcod = carregarPerfil();	
	$retorno = false;

	if($pflcod) {
		foreach($pflcod as $perfil){
			if($perfil == EXEC_PERFIL_SUPER_USUARIO) {
				$retorno = true;
				break;
			} else {
				if(!$retorno){
					$retorno = permissoesPerfil($perfil, $_REQUEST['modulo'], $categoria);
				}
			}
		}
	}
	
	$retorno = ($retorno == NULL) ? false : $retorno;
	return $retorno;
}

function carregarPerfil(){
	global $db;
	
	$sql = "SELECT pu.pflcod FROM seguranca.perfil as p 
				LEFT JOIN seguranca.perfilusuario as pu ON pu.pflcod = p.pflcod
			WHERE 
				p.sisid = '".$_SESSION['sisid']."'
			  	and pu.usucpf = '".$_SESSION['usucpf']."'";
	$pflcod = $db->carregarColuna($sql);
	$pflcod = ($pflcod ? $pflcod : array());
	return $pflcod;
}

function carregarFonteRecurso($dados){
	global $db;
	
	$sql = "SELECT DISTINCT
				fonte as codigo, 
				fonte || ' - ' || dscfonte as descricao 
			FROM 
				financeiro.empenhopar ep
			INNER JOIN par.subacaodetalhe sd ON sd.sbdptres = ep.ptres
			WHERE
				sbdid = ".$dados['sbdid'];

	$db->monta_combo( "fonte", $sql, 'S', 'Selecione', '', '' );
}

function carregarPlanoInterno( $dados ){
	global $db;
	
	$sql = "SELECT sd.sbdano, s.prgid FROM par.subacaodetalhe sd INNER JOIN par.subacao s ON s.sbaid = sd.sbaid WHERE sd.sbdid = ".$dados['sbdid'];
	$dadosSub = $db->pegaLinha( $sql );
	if( $dadosSub ){
		$sql = "SELECT 	DISTINCT
						plinumplanointerno as codigo,
						plinumplanointerno as descricao
				FROM
					par.planointerno
				WHERE pliano = ".$dadosSub['sbdano']."  AND prgid = ".$dadosSub['prgid']."              
				UNION
				SELECT 
					sd.sbdplanointerno as codigo,
					sd.sbdplanointerno as descricao
				FROM par.subacaodetalhe sd 
				WHERE 
					sd.sbdid = {$dados['sbdid']}";
	} else {
		$sql = "SELECT 
					sd.sbdplanointerno as codigo,
					sd.sbdplanointerno as descricao
				FROM par.subacaodetalhe sd 
				WHERE 
					sd.sbdid = {$dados['sbdid']}";
	}
	
	$planointerno = $db->pegaUm( "SELECT sbdplanointerno FROM par.subacaodetalhe WHERE sbdid = ".$dados['sbdid'] );

	$db->monta_combo( "planointerno", $sql, 'S', 'Selecione', 'filtraPTRES', '', '','','N', 'planointerno', false, $planointerno, 'Plano Interno' );
}

function carregarPtres( $dados ){
	global $db;
	
	$sql = "SELECT sbdptres FROM par.subacaodetalhe WHERE sbdid = ".$dados['sbdid'];
	$ptres = $db->pegaUm( $sql );
	
	$sql = "select DISTINCT
				pliptres as codigo,
				pliptres as descricao
			from par.planointerno
			where plinumplanointerno = '{$dados['plicod']}'";
	
	$db->monta_combo( "ptres", $sql, 'S', 'Selecione...', 'filtraFonteRecurso',"","","","N","","", $ptres);
}
?>
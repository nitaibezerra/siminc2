<?php

function possuiPerfil( $pflcods ){

	global $db;

	if($db->testa_superuser()){
		return true;
	}

	if ( is_array( $pflcods ) ){
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	} else {
		$pflcods = array( (integer) $pflcods );
	} if ( count( $pflcods ) == 0 ) {
		return false;
	}
	$sql = "SELECT
					count(*)
			FROM seguranca.perfilusuario
			WHERE
				usucpf = '" . $_SESSION['usucpf'] . "' and
				pflcod in ( " . implode( ",", $pflcods ) . " ) ";
	return $db->pegaUm( $sql ) > 0;
}

function pegaResponssabilidade( $tprcod ){

	global $db;

	$perfil = pegaPerfilGeral();
	$perfil = $perfil ? $perfil : array();

	if($tprcod == '1'){//Lista Estados
		#Verifica se o perfil é de cunsulta geral e equipe financeira, cunsulta geral e equipe técnica. Caso seja, mostra todos os estados, sem restrição, idependente de sua responsabilidade. Se não da continuidade ao processo "de forma normal".
		if(	in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$perfil) ||
		in_array(PAR_PERFIL_EQUIPE_TECNICA,$perfil)
		){
			$sql = "SELECT estuf FROM par.instrumentounidade where estuf is not null;";
			$r = $db->carregarColuna($sql);
			return $r;
		}else{
			$sql = "SELECT estuf FROM par.usuarioresponsabilidade WHERE usucpf = '".$_SESSION['usucpf']."' AND rpustatus = 'A'";
		}
	}elseif($tprcod == '2'){//Lista Município
		#Verifica se o perfil é de cunsulta geral e equipe financeira, cunsulta geral e equipe técnica. Caso seja, mostra todos os estados, sem restrição, idependente de sua responsabilidade. Se não da continuidade ao processo "de forma normal".
		if(	in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$perfil) ||
		in_array(PAR_PERFIL_PROFUNC_ANALISEPF,$perfil) ||
		in_array(PAR_PERFIL_EQUIPE_TECNICA,$perfil)
		){
			$sql = "SELECT muncod FROM par.instrumentounidade where muncod is not null;";
			$r = $db->carregarColuna($sql);
			return $r;
		}else{
			$sql = "SELECT muncod FROM par.usuarioresponsabilidade WHERE usucpf = '".$_SESSION['usucpf']."' AND rpustatus = 'A'";
		}
	}

	$r = $db->carregarColuna($sql);
	if( in_array(PAR_PERFIL_EQUIPE_TECNICA,$perfil) ||
			in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$perfil) ||
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$perfil) ||
			in_array(PAR_PERFIL_CONSULTA_ESTADUAL,$perfil) ||
			in_array(PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL,$perfil) ||
			in_array(PAR_PERFIL_CONSULTA_MUNICIPAL,$perfil) ||
			in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL,$perfil) ||
			in_array(PAR_PERFIL_PREFEITO,$perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO,$perfil) ||
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$perfil) ||
			in_array(PAR_PERFIL_ANALISTA_MERITOS,$perfil) ){
		$r = $r ? $r : Array('NULL');
	}
	return $r;
}

function criaAbaPar($bEstruturaAvaliacao = false){
	if(!$_SESSION['par']['boAbaMunicipio'] && !$_SESSION['par']['boAbaEstado']){
		$abasPar = array( 0 => array( "descricao" => "Lista de Estados", "link"	  => "par3.php?modulo=principal/listaEstados&acao=A"),
				1 => array( "descricao" => "Lista de Municípios", "link"	  => "par3.php?modulo=principal/listaMunicipios&acao=A")
		);
	} elseif($_SESSION['par']['boAbaMunicipio']){
		$abasPar = array(
				0 => array( "descricao" => "Lista de Municípios", "link"	  => "par3.php?modulo=principal/listaMunicipios&acao=A" )
		);
	} elseif($_SESSION['par']['boAbaEstado']){
		$abasPar = array(
				0 => array( "descricao" => "Lista de Estados", "link"	  => "par3.php?modulo=principal/listaEstados&acao=A" )
		);
	}
	return $abasPar;
}

function pegaArrayPerfil($usucpf){

	global $db;

	$sql = "SELECT
	pu.pflcod
	FROM
	seguranca.perfil AS p
	LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
	WHERE
	p.sisid = '{$_SESSION['sisid']}'
	AND pu.usucpf = '$usucpf'";

	$pflcod = $db->carregarColuna( $sql );

	return $pflcod;
}

function formata_numero_processo($str) {
    $str = substr($str,0,5).'.'.substr($str,5,6).'/'.substr($str,11,4).'-'.substr($str,15,2);
	return '<span class="processo_detalhe">'.$str.'</span>';
}

?>
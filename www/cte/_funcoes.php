<?php

require_once "_constantes.php";
include_once( APPRAIZ. "includes/classes/Modelo.class.inc" );
include_once( APPRAIZ. "cte/classes/EscolaAtiva.class.inc" );
include_once( APPRAIZ. "cte/classes/EscolaAtivaEscolas.class.inc" );
include_once( APPRAIZ. "cte/classes/EscolaAtivaPessoas.class.inc" );

if (!function_exists('wf_pegarMensagem')) { include_once APPRAIZ .'includes/workflow.php';}

// ----- PERMISSAO -------------------------------------------------------------
function cte_arrayPerfil(){
	global $db;
	
	$sql = sprintf("SELECT
					 pu.pflcod
					FROM
					 seguranca.perfilusuario pu
					 INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 	p.sisid = 13
					WHERE
					 pu.usucpf = '%s'
					ORDER BY
					 p.pflnivel",
				$_SESSION['usucpf']);
	return (array) $db->carregarColuna($sql,'pflcod');
}
function cte_pegarMunicipiosPermitidos()
{
	global $db;
	static $municipios = null;
	if ( $municipios === null )
	{
		
		
		if ( $db->testa_superuser() || cte_possuiPerfilSemVinculo() )
		{
			// pega todos os estados
			$sql = "
				select
					muncod
				from territorios.municipio
			";
		}
		/*
		else if(cte_pegarUfsPermitidas() && !cte_possuiPerfil(CTE_PERFIL_EQUIPE_MUNICIPAL) && !cte_possuiPerfil(CTE_PERFIL_EQUIPE_LOCAL)) {
			
			// pega estados do perfil do usuário
			$sql = "
				select
					muncod
				from territorios.municipio
				where estuf in ('". implode( "','", cte_pegarUfsPermitidas() ) ."')";	
		}	
		*/	
		else if(cte_possuiPerfil(CTE_PERFIL_CONSULTA_ESTADUAL)){
 
			$sql = "
				select
					m.muncod
				from territorios.municipio m
				inner join cte.usuarioresponsabilidade ur on ur.muncod = m.muncod or ur.estuf = m.estuf
				where
					ur.usucpf = '" . $_SESSION['usucpf'] . "' and
					rpustatus = 'A'
					and m.estuf = '".$_SESSION['uf']."'
				group by
					m.muncod";
		}
		else
		{
			// pega estados do perfil do usuário
			$sql = "
				select
					m.muncod
				from territorios.municipio m
					inner join cte.usuarioresponsabilidade ur on
						ur.muncod = m.muncod
				where
					ur.usucpf = '" . $_SESSION['usucpf'] . "' and
					rpustatus = 'A'
				group by
					m.muncod
			";
		}
		//echo $sql;
		//echo "<br>ok=".cte_possuiPerfil(CTE_PERFIL_EQUIPE_MUNICIPAL,CTE_PERFIL_EQUIPE_LOCAL);
		$dados = $db->carregar( $sql );
		$dados = $dados ? $dados : array();
		$municipios = array();
		foreach ( $dados as $linha )
		{
			array_push( $municipios, $linha['muncod'] );
		}
	}
	return $municipios;
}


function cte_pegarUfsPermitidas()
{
	global $db;
	static $ufs = null;
	if ( $ufs === null )
	{
		if ( $db->testa_superuser() || cte_possuiPerfilSemVinculo() )
		{
			// pega todos os estados
			$sql = "
				select
					estuf
				from territorios.estado
			";
		}
		else
		{
			// pega estados do perfil do usuário
			$sql = "
				select
					e.estuf
				from territorios.estado e
					inner join cte.usuarioresponsabilidade ur on
						ur.estuf = e.estuf
					inner join seguranca.perfil p on
						p.pflcod = ur.pflcod
					inner join seguranca.perfilusuario pu on
						pu.pflcod = ur.pflcod and
						pu.usucpf = ur.usucpf
				where
					ur.usucpf = '" . $_SESSION['usucpf'] . "' and
					ur.rpustatus = 'A' and
					p.sisid = " . CTE_SISTEMA . "
				group by
					e.estuf
			";
		}
		$dados = $db->carregar( $sql );
		$dados = $dados ? $dados : array();
		$ufs = array();
		foreach ( $dados as $linha )
		{
			array_push( $ufs, $linha['estuf'] );
		}
	}
	return $ufs;
}

/**
 * Descrição
 *
 * @param integer $inuid
 * @return boolean
 */
function cte_podeAnalisar( $inuid )
{
	global $db;
	$docid = cte_pegarDocid( $inuid );
	$documento = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $documento['esdid'];
	return
		$esdid == CTE_ESTADO_ANALISE &&
		cte_possuiPermissaoIndicador( $inuid ) &&
		(
			cte_possuiPerfil( CTE_PERFIL_EQUIPE_TECNICA ) ||
			$db->testa_superuser()
		);
}

function cte_podeEditarIndicador( $inuid, $indid = null )
{
	global $db;
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	$docid = cte_pegarDocid( $inuid );
	$documento = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $documento['esdid'];

	switch ( $esdid )
	{
		case CTE_ESTADO_DIAGNOSTICO:
		case CTE_ESTADO_PAR:
			$perfis = array(
				CTE_PERFIL_EQUIPE_MUNICIPAL,
				CTE_PERFIL_EQUIPE_LOCAL,
				CTE_PERFIL_EQUIPE_LOCAL_APROVACAO
			);
			break;
		case CTE_ESTADO_ANALISE:
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_EQUIPE_TECNICA_MEC_MUN );
			break;
		case CTE_ESTADO_ANALISE_FIN:
			$perfis = array( CTE_PERFIL_ADMINISTRADOR);
			break;
		case CTE_ESTADO_FINALIZADO:
		default:
			$perfis = array();
			break;
	}
	
	// verifica se usuario possui perfil para o estado atual
	if ( !cte_possuiPerfil( $perfis ) )
	{
		return false;
	}
	
	$indid = (integer) $indid;
	if ( $indid )
	{
		// verifica se indicador pertence ao inuid
		return cte_indidPertenceInuid( $indid, $inuid );
	}
	
	return true;
}

function cte_podeEditarParecer( $inuid )
{
	global $db;
	if ( $db->testa_superuser() )
	{
		return true;
	}
	$docid = cte_pegarDocid( $inuid );
	$documento = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $documento['esdid'];
	switch ( $esdid ) {
		case CTE_ESTADO_DIAGNOSTICO:
		case CTE_ESTADO_PAR:
			$perfis = array( CTE_PERFIL_EQUIPE_MUNICIPAL, 
							 CTE_PERFIL_CONSULTORES, 
							 CTE_PERFIL_EQUIPE_LOCAL,
							 CTE_PERFIL_EQUIPE_LOCAL,
							 CTE_PERFIL_EQUIPE_ESTADUAL_CADASTRO,
							 CTE_PERFIL_EQUIPE_LOCAL_APROVACAO );
			break;
		case CTE_ESTADO_ANALISE:
		case CTE_ESTADO_ANALISE_FIN:
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA,
							 CTE_PERFIL_EQUIPE_ESCOLA_ATIVA, 
							 CTE_PERFIL_EQUIPE_TECNICA_MEC_MUN );
			break;
		case CTE_ESTADO_FNDE:
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_EQUIPE_ESCOLA_ATIVA, CTE_PERFIL_ADMINISTRADOR);
			break;
		default:
			$perfis = array();
			break;
	}
	return cte_possuiPerfil( $perfis );
}

function cte_podeEditarQuestaoPontual( $inuid, $prgid = null )
{
	global $db;
	if ( $db->testa_superuser() )
	{
		return true;
	}
	$docid = cte_pegarDocid( $inuid );
	$documento = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $documento['esdid'];
	switch ( $esdid )
	{
		case CTE_ESTADO_DIAGNOSTICO:
		case CTE_ESTADO_PAR:
			$perfis = array(
				CTE_PERFIL_EQUIPE_MUNICIPAL,
				CTE_PERFIL_EQUIPE_LOCAL,
				CTE_PERFIL_EQUIPE_LOCAL_APROVACAO
			);
			break;
		case CTE_ESTADO_ANALISE:
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_EQUIPE_ESCOLA_ATIVA, CTE_PERFIL_EQUIPE_TECNICA_MEC_MUN );
			break;
		case CTE_ESTADO_FINALIZADO:
		default:
			$perfis = array();
			break;
	}
	// verifica se questao pertence ao instrumento atual
	$prgid = (integer) $prgid;
	$pertence = !$prgid || cte_prgidPertenceInuid( $prgid, $inuid );
	return $pertence && cte_possuiPerfil( $perfis );
}

function cte_podeEditarSubacao( $inuid )
{
	$documento = wf_pegarEstadoAtual( cte_pegarDocid( $inuid ) );
	$a = cte_possuiPerfil( CTE_PERFIL_SUPER_USUARIO );
	$b = ( cte_possuiPerfil( CTE_PERFIL_EQUIPE_LOCAL ) && $documento['esdid'] == CTE_ESTADO_PAR );
	$c = ( cte_possuiPerfil( array( CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_EQUIPE_ESCOLA_ATIVA, CTE_PERFIL_EQUIPE_TECNICA_MEC_MUN ) ) && $documento['esdid'] == CTE_ESTADO_ANALISE );
	$d = ( cte_possuiPerfil( CTE_PERFIL_EQUIPE_MUNICIPAL ) && $documento['esdid'] == CTE_ESTADO_PAR );
	return $a || $b || $c || $d;
}

function cte_podeElaborarPlanoDeAcoes( $inuid )
{
	global $db;
	if ( $db->testa_superuser() )
	{
		return true;
	}
	$docid     = cte_pegarDocid( $inuid );
	$documento = wf_pegarEstadoAtual( $docid );
	$esdid     = (integer) $documento['esdid'];
	/*
	switch ( $esdid )
	{
		case CTE_ESTADO_PAR:
			$perfis = array(
				CTE_PERFIL_EQUIPE_MUNICIPAL,
				CTE_PERFIL_EQUIPE_LOCAL,
				CTE_PERFIL_EQUIPE_LOCAL_APROVACAO
			);
			break;
		case CTE_ESTADO_ANALISE:
		case CTE_ESTADO_ANALISE_FIN:
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA );
			break;
		case CTE_ESTADO_DIAGNOSTICO:
		case CTE_ESTADO_FINALIZADO:
		default:
			$perfis = array();
			break;
	}
	*/
			$perfis = array(
				CTE_PERFIL_ALTA_GESTAO,
				CTE_PERFIL_CONSULTA_GERAL,
				CTE_PERFIL_CONSULTORES,
				CTE_PERFIL_EQUIPE_MUNICIPAL,
				CTE_PERFIL_EQUIPE_LOCAL,
				CTE_PERFIL_EQUIPE_LOCAL_APROVACAO,
				CTE_PERFIL_EQUIPE_TECNICA,
				CTE_PERFIL_EQUIPE_TECNICA_MEC_MUN,
				CTE_PERFIL_CONSULTA_MUNICIPAL,
				CTE_PERFIL_CONSULTA_ESTADUAL,
				CTE_PERFIL_CONSULTA_GERAL,
				CTE_PERFIL_ADMINISTRATOR_TEMP
			);
	
	return cte_possuiPerfil( $perfis );
}

function cte_podeVerIndicador( $inuid, $indid = null )
{
	global $db;
	if ( $db->testa_superuser() )
	{
		return true;
	}
	$docid = cte_pegarDocid( $inuid );
	$documento = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $documento['esdid'];
	/*
	switch ( $esdid )
	{
		case CTE_ESTADO_DIAGNOSTICO:
		case CTE_ESTADO_PAR:
		case CTE_ESTADO_ANALISE:
		case CTE_ESTADO_FINALIZADO:
			$perfis = array(
				CTE_PERFIL_ALTA_GESTAO,
				CTE_PERFIL_CONSULTA_GERAL,
				CTE_PERFIL_CONSULTORES,
				CTE_PERFIL_EQUIPE_MUNICIPAL,
				CTE_PERFIL_EQUIPE_LOCAL,
				CTE_PERFIL_EQUIPE_LOCAL_APROVACAO,
				CTE_PERFIL_EQUIPE_TECNICA
			);
			break;
		default:
			$perfis = array();
			break;
	}
	*/
			$perfis = array(
				CTE_PERFIL_ALTA_GESTAO,
				CTE_PERFIL_CONSULTA_GERAL,
				CTE_PERFIL_CONSULTORES,
				CTE_PERFIL_EQUIPE_MUNICIPAL,
				CTE_PERFIL_EQUIPE_LOCAL,
				CTE_PERFIL_EQUIPE_LOCAL_APROVACAO,
				CTE_PERFIL_EQUIPE_TECNICA,
				CTE_PERFIL_EQUIPE_TECNICA_MEC_MUN,
				CTE_PERFIL_CONSULTA_MUNICIPAL,
				CTE_PERFIL_CONSULTA_ESTADUAL,
				CTE_PERFIL_CONSULTA_GERAL,
				CTE_PERFIL_ADMINISTRATOR_TEMP
			);
	
	// verifica se indicador pertence ao instrumento atual
	$indid = (integer) $indid;
	$pertence = !$indid || cte_indidPertenceInuid( $indid, $inuid );
	return $pertence && cte_possuiPerfil( $perfis );
}

function cte_podeVerParecer( $inuid )
{
	global $db;
	$docid = cte_pegarDocid( $inuid );
	$documento = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $documento['esdid'];
	$estados = array( CTE_ESTADO_PAR, CTE_ESTADO_ANALISE, CTE_ESTADO_FINALIZADO );
	return in_array( $esdid, $estados );
}

function cte_podeVerQuestaoPontual( $inuid, $prgid = null )
{
	global $db;
	if ( $db->testa_superuser() )
	{
		return true;
	}
	$docid = cte_pegarDocid( $inuid );
	$documento = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $documento['esdid'];
	/*
	switch ( $esdid )
	{
		case CTE_ESTADO_DIAGNOSTICO:
		case CTE_ESTADO_PAR:
		case CTE_ESTADO_ANALISE:
		case CTE_ESTADO_FINALIZADO:
			$perfis = array(
				CTE_PERFIL_ALTA_GESTAO,
				CTE_PERFIL_CONSULTA_GERAL,
				CTE_PERFIL_CONSULTORES,
				CTE_PERFIL_EQUIPE_MUNICIPAL,
				CTE_PERFIL_EQUIPE_LOCAL,
				CTE_PERFIL_EQUIPE_LOCAL_APROVACAO,
				CTE_PERFIL_EQUIPE_TECNICA,
				CTE_PERFIL_EQUIPE_TECNICA_MEC_MUN
			);
			break;
		default:
			$perfis = array();
			break;
	}
	*/
			$perfis = array(
				CTE_PERFIL_ALTA_GESTAO,
				CTE_PERFIL_CONSULTA_GERAL,
				CTE_PERFIL_CONSULTORES,
				CTE_PERFIL_EQUIPE_MUNICIPAL,
				CTE_PERFIL_EQUIPE_LOCAL,
				CTE_PERFIL_EQUIPE_LOCAL_APROVACAO,
				CTE_PERFIL_EQUIPE_TECNICA,
				CTE_PERFIL_EQUIPE_TECNICA_MEC_MUN,
				CTE_PERFIL_CONSULTA_MUNICIPAL,
				CTE_PERFIL_CONSULTA_ESTADUAL,
				CTE_PERFIL_CONSULTA_GERAL,
				CTE_PERFIL_ADMINISTRATOR_TEMP
			);
	
	// verifica se questao pertence ao instrumento atual
	$prgid = (integer) $prgid;
	$pertence = !$prgid || cte_prgidPertenceInuid( $prgid, $inuid );
	return $pertence && cte_possuiPerfil( $perfis );
}

function cte_podeVerRelatorioParCopia( $inuid )
{
	global $db;
	if ( $db->testa_superuser() )
	{
		return true;
	}
	$docid = cte_pegarDocid( $inuid );
	$documento = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $documento['esdid'];
	return
		$esdid == CTE_ESTADO_ANALISE ||
		$esdid == CTE_ESTADO_FINALIZADO;
}

function cte_podeVerRelatorioParAtual( $inuid )
{
	global $db;
	if ( $db->testa_superuser() or cte_possuiPerfil(array(CTE_PERFIL_EQUIPE_TECNICA)))
	{
		return true;
	}
	$docid = cte_pegarDocid( $inuid );
	$documento = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $documento['esdid'];
	return $esdid == CTE_ESTADO_FINALIZADO;
}

function cte_possuiPerfil( $pflcods )
{	
	global $db;
	if ( is_array( $pflcods ) )
	{
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	}
	else
	{
		$pflcods = array( (integer) $pflcods );
	}
	if ( count( $pflcods ) == 0 )
	{
		return false;
	}
	$sql = "
		select
			count(*)
		from seguranca.perfilusuario
		where
			usucpf = '" . $_SESSION['usucpf'] . "' and
			pflcod in ( " . implode( ",", $pflcods ) . " ) ";
	
	return $db->pegaUm( $sql ) > 0;
}

function cte_possuiPerfilExclusivo( $pflcods ){
	
	global $db;
	if ( is_array( $pflcods ) ){
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	}
	else{
		$pflcods = array( (integer) $pflcods );
	}
	if ( count( $pflcods ) == 0 ){
		return false;
	}
	$sql = "select count(*)
			from seguranca.perfilusuario
			where usucpf = '" . $_SESSION['usucpf'] . "' 
			and pflcod in ( " . implode( ",", $pflcods ) . " ) ";
	
	if(  $db->pegaUm( $sql ) > 0 ){
		
		$sql = "select pflcod from seguranca.perfil
				where sisid = '" . $_SESSION['sisid'] . "'
				and pflcod not in ( ". implode( ",", $pflcods ) ." )";
		
		$resultado = $db->carregar( $sql );
		
		$arResultado = $resultado ? $resultado : array();
		
		foreach( $arResultado as $pflcod ){
			$arPflcod[] = $pflcod['pflcod'];
		}
		
		return !cte_possuiPerfil( $arPflcod );
		
	}
	return false;
}

function cte_possuiFormaExecucaoTecnica( $frmid )
{
	return in_array( $frmid, array( 1, 4, 5, 6, 7, 8, 9, 10 ) );	
}

function cte_possuiFormaExecucaoFinanceira( $frmid )
{
	return in_array( $frmid, array( 2, 3, 11 ) );	
}

function cte_possuiPerfilMunicipio( $pflcods, $muncod )
{
	global $db;

    if ($db->testa_superuser())
        return true;

	if ( is_array( $pflcods ) )
	{
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	}
	else
	{
		$pflcods = array( (integer) $pflcods );
	}
	if ( count( $pflcods ) == 0 )
	{
		return false;
	}
	$sql = "
		select
			count(*)
		from cte.usuarioresponsabilidade
		where
			pflcod in ( '" . implode( "','", $pflcods ) . "' ) and
			usucpf = '" . $_SESSION['usucpf'] . "' and
			rpustatus = 'A' and
			muncod = '" . $muncod . "'
	";
	return $db->pegaUm( $sql ) > 0;
}

function cte_possuiPermissaoIndicador( $inuid )
{
	$estuf = cte_pegarEstuf( $inuid );
	$muncod = cte_pegarMuncod( $inuid );
	return
		cte_possuiPermissaoUf( $estuf ) ||
		cte_possuiPermissaoMunicipio( $muncod );
}

function cte_possuiPermissaoMunicipio( $muncod )
{
	global $db;
	return
		$db->testa_superuser() || in_array( $muncod, cte_pegarMunicipiosPermitidos() );
}

function cte_possuiPermissaoUf( $estuf )
{
	global $db;
	return
		$db->testa_superuser() || in_array( $estuf, cte_pegarUfsPermitidas() );
}

function cte_possuiPermissaoUfEstrategico( $inuid )
{
	global $db;

    if ($db->testa_superuser())
        return true;

	$estuf = cte_pegarEstuf( $inuid );
	$sql = "
		select
			count(*)
		from cte.usuarioresponsabilidade
		where
			pflcod in ( '" . CTE_PERFIL_PLAN_ESTRATEGICO_EST . "' ) and
			usucpf = '" . $_SESSION['usucpf'] . "' and
			rpustatus = 'A' and
			estuf = '" . $estuf . "'";

	return $db->pegaUm( $sql ) > 0;
}

function cte_possuiPermissaoMunEstrategico( $inuid )
{
	global $db;

    if ($db->testa_superuser())
        return true;

	$muncod = cte_pegarMuncod( $inuid );
	$sql = "
		select
			count(*)
		from cte.usuarioresponsabilidade
		where
			pflcod in ( '" . CTE_PERFIL_PLAN_ESTRATEGICO_MUN . "' ) and
			usucpf = '" . $_SESSION['usucpf'] . "' and
			rpustatus = 'A' and
			muncod = '" . $muncod . "'
	";
	return $db->pegaUm( $sql ) > 0;
}

function cte_verificaSessao()
{
	if ( !$_SESSION['inuid'] )
	{
		header( "Location: ?modulo=inicio&acao=A" );
		exit();
	}
}


function cte_verificaGrandeMunicipio($inuid){
global $db;
	
	$sql = " select count(mu.muncod) as total from territorios.tipomunicipio mt
left outer join territorios.muntipomunicipio mtm ON mtm.tpmid=mt.tpmid
left outer join territorios.municipio mu ON mu.muncod=mtm.muncod
left outer join territorios.estado es ON es.estuf=mu.estuf
left outer join cte.instrumentounidade inu on inu.muncod=mu.muncod
where inu.muncod is not null and mt.gtmid = '1' 
and inu.inuid='".$inuid."'";
	
	return $db->pegaUm($sql) > 0 ;
		
	
}

function cte_verifica1827Priorizados($inuid){
global $db;
	
	$sql = " select count(mu.muncod) as total from territorios.tipomunicipio mt
left outer join territorios.muntipomunicipio mtm ON mtm.tpmid=mt.tpmid
left outer join territorios.municipio mu ON mu.muncod=mtm.muncod
left outer join territorios.estado es ON es.estuf=mu.estuf
left outer join cte.instrumentounidade inu on inu.muncod=mu.muncod
where inu.muncod is not null and mtm.tpmid in (11)
and inu.inuid='".$inuid."'";
	
	return $db->pegaUm($sql) > 0 ;
		
	
}

// ----- AÇÕES -----------------------------------------------------------------

function cte_estadoPosCopia( $inuid )
{
	global $db;
	$docid = cte_pegarDocid( $inuid );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	return 	$esdid != CTE_ESTADO_ANALISE || $esdid != CTE_ESTADO_FINALIZADO;
}

function cte_selecionarUf( $itrid, $tpdid, $estuf )
{
	global $db;
	if ( !cte_possuiPermissaoUf( $estuf ) )
	{
		return false;
	}
	$itrid = (integer) $itrid;
	$tpdid = (integer) $tpdid;
	if ( !$itrid || !$tpdid )
	{
		return null;
	}
	
	// cria instrumento da unidade
	$inuid = cte_criarInstrumentoUf( $itrid, $estuf );
	
	// cria documento
	$docid = cte_criarDocumento( $tpdid, $inuid );
	
	return $_SESSION['inuid'] = $inuid;
}

function cte_selecionarMunicipio( $itrid, $tpdid, $muncod )
{
	global $db;
	if ( !cte_possuiPermissaoMunicipio( $muncod ) )
	{
		return false;
	}
	$itrid = (integer) $itrid;
	$tpdid = (integer) $tpdid;
	if ( !$itrid || !$tpdid )
	{
		return null;
	}
	
	// cria instrumento da unidade
	$inuid = cte_criarInstrumentoMunicipio( $itrid, $muncod );
	
	// cria documento se usuário tiver perfil de equipe local no municipio
	if ( cte_possuiPerfilMunicipio( CTE_PERFIL_EQUIPE_MUNICIPAL, $muncod ) )
	{
		$docid = cte_criarDocumento( $tpdid, $inuid );
	}
	
	return $_SESSION['inuid'] = $inuid;
}

function cte_verificarPreenchimento( $inuid )
{
	return cte_pegarPercentagem( $inuid ) > 99;
}





// ----- APOIO -----------------------------------------------------------------

function cte_atualizarPlanosPequenoMunicipio( $inuid )
{
	global $db;
	$inuid = (integer) $inuid;
	$sql = "
		select
			ptoid
		from
            cte.pontuacao
		where
			inuid = " . $inuid;
    $linhas = $db->carregar( $sql );
	$linhas = $linhas ? $linhas : array();
    $return = true;

	foreach ( $linhas as $linha )
	{
		if (($return = cte_atualizarPlanoPequenoMunicipio( $linha['ptoid'] )) === false)
            break;
	}
	
    return $return;

}

function cte_atualizarPlanoGrandeMunicipio( $ptoid ) {
	global $db;
	$ptoid = (integer) $ptoid;
	$sql   = "select inuid from cte.pontuacao where ptoid = " . $ptoid;
	$inuid = (integer) $db->pegaUm( $sql );;


	// verifica se é grande município
	//if ( cte_verificaGrandeMunicipio( $inuid ) ) {
	//	return true;
	//}

	// captura o valor do critério
	$sql = "
		select
			c.ctrpontuacao
		from
            cte.pontuacao p
        inner join cte.criterio c on
            c.crtid = p.crtid
		where
			ptoid = " . $ptoid;

    $ctrpontuacao = (integer) $db->pegaUm( $sql );

	switch ( $ctrpontuacao )
	{
		case 1:
		case 2:
			break;
		default:
			$return = cte_removerPlano( $ptoid );
        	break;
	}

    return $return;
}


/******** FUNÇÕES QUE INSERE AÇÕES DE SUBAÇÃOES DAS PONTUAÇÕES 3,4 E 0 *************/

/**
 * function cte_insereAcoesSubacoesPontuacao034
 * @desc   : função inicial para insere as ações e subações para a pontuação passada.
 * @author : Thiago Tasca Barbosa
 * @param  : string $ptoid (id da pontuação)
 * @return : boleano return true em sucesso ou false em caso de algum erro.
 * @since 29/05/2009
 */
function cte_insereAcoesSubacoesPontuacao034( $ptoid )
{
	global $db;
	
	$ptoid = (integer) $ptoid;
	$sql   = "select inuid from cte.pontuacao where ptoid = " . $ptoid;
	$inuid = (integer) $db->pegaUm( $sql );;

	// captura o valor do critério
	$sql = "
		select
			c.ctrpontuacao
		from
            cte.pontuacao p
        inner join cte.criterio c on
            c.crtid = p.crtid
		where
			ptoid = " . $ptoid;
	
    $ctrpontuacao = (integer) $db->pegaUm( $sql );
    
	// captura nome da ação
	$sql      = "select acidsc from cte.acaoindicador where ptoid = " . $ptoid;
	$acidsc   = (string) $db->pegaUm( $sql );
	
	//valida se a ação já existe
	$sql = "select ptoid from cte.acaoindicador where ptoid = ".$ptoid;
	$valida = $db->pegaUm($sql);
	if($valida){
		return  false;
	}

	switch ( $ctrpontuacao )
	{	
		case 0:
			// captura descricao da ação para pontuação 0
			$dados    = cte_pegarPropostaAcao034( $ptoid, 0 );
			$ppadsc_0 = (string) $dados['ppadsc'];
			if($dados != false){
	            $return = cte_adicionarPlano( $ptoid, 0 );
			}else{
				$return = false;
			}
			break;
		
		case 3:
			// captura descricao da ação para pontuação 3
			$dados    = cte_pegarPropostaAcao034( $ptoid, 3 );
			$ppadsc_3 = (string) $dados['ppadsc'];
			if($dados != false){
	            $return = cte_adicionarPlano( $ptoid, 3 );
			}else{
				$return = false;
			}
			break;
			
		case 4:
			// captura descricao da ação para pontuação 4
			$dados    = cte_pegarPropostaAcao034( $ptoid, 4 );
			$ppadsc_4 = (string) $dados['ppadsc'];
			if($dados != false){
            	$return = cte_adicionarPlano( $ptoid, 4 );
			}else{
				$return = false;
			}
			break;

		default:
			$return = false;
            break;
	}

    return $return;
}

/**
 * function cte_pegarPropostaAcao034
 * @desc   : recupera as propostas para pontuação 0, 3 e 4 do guia.
 * @author : Thiago Tasca Barbosa
 * @param  : string $ptoid (id da pontuação)
 * @param  : string $ctrpontuacao (número da pontuação)
 * @return : boleano return true em sucesso.
 * @since 29/05/2009
 */
function cte_pegarPropostaAcao034( $ptoid, $ctrpontuacao )
{
	global $db;
	$ptoid = (integer) $ptoid;
	$sql = "
		select
			crtid
		from cte.pontuacao
		where
			ptoid = " . $ptoid . "
	";
	$crtid = $db->pegaUm( $sql );
	if ( !$crtid )
	{
		return false;
	}
	$sql = "
		select
			pro.ppaid,
			pro.ppadsc
		from cte.proposicaoacao pro
			left outer join cte.criterio crt on
				crt.crtid = pro.crtid
		where
			crt.crtid = " . $crtid . " and
			crt.ctrpontuacao = " . $ctrpontuacao . "
	";
	
	
	
	$propostas = $db->recuperar( $sql );
	$propostas = $propostas ? $propostas : false;
	return $propostas;
}

/**
 * function cte_adicionarPlano
 * @desc   : insere as ações e subações para a pontuação passada.
 * @author : Thiago Tasca Barbosa
 * @param  : string $ptoid (id da pontuação)
 * @param  : string $crtid (id do critério)
 * @return : boleano return true em sucesso.
 * @since 29/05/2009
 */
function cte_adicionarPlano( $ptoid, $crtid )
{
	global $db;
	$ptoid = (integer) $ptoid;
	$crtid = (integer) $crtid;
	
	$acao = cte_pegarPropostaAcao( $ptoid, $crtid );
	$subacoes = cte_pegarPropostaSubacoes( $acao['ppaid'] );
	$subacoes = $subacoes ? $subacoes : array();

	$acidsc = str_replace( "'", "\'", $acao['ppadsc'] );

	// cadastra ação
	$sqlAcao = "
		insert into cte.acaoindicador
		( ptoid, acidsc, acilocalizador, usucpf, ppaid )
		values
		( " . $ptoid . ", '" . $acidsc . "', 'M', '" . $_SESSION['usucpf'] . "', '". $acao['ppaid'] ."' )
		returning aciid
	";
	$aciid = (integer) $db->pegaUm( $sqlAcao );
	
	if ( !$aciid )
	{
        echo __LINE__;
        die();

		return;
	}
	
	// cadastra subações
	foreach ( $subacoes as $subacao )
	{
		$sbadsc      = str_replace( "'", "\'", $subacao["ppsdsc"] );
		$undid       = (integer) $subacao["undid"];
		$sbatexto    = str_replace( "'", "\'", $subacao["ppstexto"] );
		$sbaobjetivo = str_replace( "'", "\'", $subacao["ppsobjetivo"] );
		$prgid       = (integer) $subacao["prgid"];
		$sbaordem    = (integer) $subacao["ppsordem"];
		$ppsid       = (integer) $subacao["ppsid"];
		$sbastgmpl   = str_replace( "'", "\'", $subacao["ppsmetodologia"] );
		$frmid       = (integer) $subacao["frmid"];

		$undid = $undid ? $undid : "null";
		$prgid = $prgid ? $prgid : "null";
		$ppsid = $ppsid ? $ppsid : "null";
		$frmid = $frmid ? $frmid : "null";

		$sqlSubAcao = "
			 insert into cte.subacaoindicador
			 (
				 aciid,
				 sbadsc,
				 undid,
				 sbatexto,
				 sbaobjetivo,
				 prgid,
				 sbaordem,
				 ppsid,
				 sbastgmpl,
				 frmid
			 ) values (
				 "  . $aciid       . ",
				 '" . $sbadsc      . "',
				 "  . $undid       . ",
				 '" . $sbatexto    . "',
				 '" . $sbaobjetivo . "',
				 "  . $prgid       . ",
				 "  . $sbaordem    . ",
				 "  . $ppsid       . ",
				 '" . $sbastgmpl   . "',
				 "  . $frmid       . "
			 )";

		$return = $db->executar( $sqlSubAcao );
	}

    return true;
}

/**
 * function cte_pegarPropostaAcao
 * @desc   : retorna array com as propostas de ações 
 * @author : Thiago Tasca Barbosa
 * @param  : integer $ptoid (id da pontuação)
 * @param  : integer $ctrpontuacao (número da pontuação)
 * @return : array ou boleano $propostas (array com as propostas)
 * @since 29/05/2009
 */
function cte_pegarPropostaAcao( $ptoid, $ctrpontuacao ){
	global $db;
	$ptoid = (integer) $ptoid;
	$sql = "
		select
			crtid
		from cte.pontuacao
		where
			ptoid = " . $ptoid . "
	";
	$crtid = $db->pegaUm( $sql );
	if ( !$crtid )
	{
		return false;
	}
	$sql = "
		select
			pro.ppaid,
			pro.ppadsc
		from cte.proposicaoacao pro
			left outer join cte.criterio crt on
				crt.crtid = pro.crtid
		where
			crt.crtid = " . $crtid . " and
			crt.ctrpontuacao = " . $ctrpontuacao . "
	";

	$propostas = $db->recuperar( $sql );
	$propostas = $propostas ? $propostas : false;
	return $propostas;
}

/**
 * function cte_pegarPropostaSubacoes
 * @desc   : retorna array com as propostas de subações 
 * @author : Thiago Tasca Barbosa
 * @param  : integer $ppaid (id da proposição (proposta))
 * @return : array   $propostas (array com as propostas)
 * @since 29/05/2009
 */
function cte_pegarPropostaSubacoes( $ppaid )
{
	global $db;
	static $propostas = array();
	$ppaid = (integer) $ppaid;
	if ( !array_key_exists( $ppaid, $propostas ) )
	{
		
			$sql= " select * 
				from cte.proposicaosubacao 
				where ppaid = ".$ppaid;
		
		$propostas[$ppaid] = $db->carregar( $sql );	
	}
	return $propostas[$ppaid];
}

/******** FIM DAS FUNÇÕES INSERINDO AÇÕES DE SUBAÇÃOES DAS PONTUAÇÕES 3,4 E 0 *************/

function cte_atualizarPlanoPequenoMunicipio( $ptoid )
{
	global $db;

	$ptoid = (integer) $ptoid;
	$sql   = "select inuid from cte.pontuacao where ptoid = " . $ptoid;
	$inuid = (integer) $db->pegaUm( $sql );;

	// verifica se é muncipal
	$itrid = cte_pegarItrid( $inuid );
	if ( $itrid != INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {
		return true;
    }

	// verifica se é grande município
	if ( cte_verificaGrandeMunicipio( $inuid ) )
	{
		return true;
	}

	// captura o valor do critério
	$sql = "
		select
			c.ctrpontuacao
		from
            cte.pontuacao p
        inner join cte.criterio c on
            c.crtid = p.crtid
		where
			ptoid = " . $ptoid;

    $ctrpontuacao = (integer) $db->pegaUm( $sql );

	// captura nome da ação
	$sql      = "select acidsc from cte.acaoindicador where ptoid = " . $ptoid;
	$acidsc   = (string) $db->pegaUm( $sql );
	
	// captura descricao da ação para pontuação 1
	$dados    = cte_pegarPropostaAcaoPequenosMunicipios( $ptoid, 1 );
	$ppadsc_1 = (string) $dados['ppadsc'];

	// captura descricao da ação para pontuação 2
	$dados    = cte_pegarPropostaAcaoPequenosMunicipios( $ptoid, 2 );
	$ppadsc_2 = (string) $dados['ppadsc'];

	switch ( $ctrpontuacao )
	{
		case 1:
            $return = cte_removerPlano( $ptoid ) &&
                      cte_adicionarPlanoMunicipioPequeno( $ptoid, 1 );
			break;

		case 2:
            $return = cte_removerPlano( $ptoid ) &&
                      cte_adicionarPlanoMunicipioPequeno( $ptoid, 2 );
			break;

		default:
			$return = cte_removerPlano( $ptoid );
            break;
	}

    return $return;
}


function cte_removerPlano( $ptoid )
{
	global $db;
	$ptoid = (integer) $ptoid;
	
	$sql = "
		select
			sbaid
		from cte.subacaoindicador
		where
			aciid in (
				select
					aciid
				from cte.acaoindicador
				where
					ptoid = " . $ptoid . "
			)
	";

	$sbaids = $db->carregar( $sql );
	$sbaids = $sbaids ? $sbaids : array();

	foreach ( $sbaids as $linhas )
	{
		$sbaid = (integer) $linhas["sbaid"];
		
	$sql ="select sbaid from cte.monitoramentosubacao where sbaid = ".$sbaid;

		$passivelMonitoramento = $db->pegaUm($sql);
		$docid              = cte_pegarDocid($_SESSION['inuid']);
		$estadoDocumento    = wf_pegarEstadoAtual($docid);

		if($passivelMonitoramento && $estadoDocumento['esdid'] != CTE_ESTADO_DIAGNOSTICO){
			alert('Existem subações passíveis de monitoramento. Não é possivel enviar para fase de Elaboração.');
			return false;
		}

		$sql = '
            DELETE FROM
                cte.subacaoparecertecnico
            WHERE
                sbaid = ' . $sbaid . ';

            delete from
                cte.subacaobeneficiario
            where
                sbaid = ' . $sbaid;

		$db->executar( $sql );

		$sql = "
			delete from cte.composicaosubacao
			where
				sbaid = " . $sbaid;

		$db->executar( $sql );
		
		$sql = "
			delete from cte.composicaopessoa
			where
				sbaid = " . $sbaid;

		$db->executar( $sql );

		$sql = "
			delete from cte.qtdfisicoano
			where
				sbaid = " . $sbaid;

		$db->executar( $sql );

        $sql = "
            delete from cte.termosubacaoindicador
            where
                sbaid = " . $sbaid;

		$db->executar( $sql );
		
		$sql = "
			delete from cte.monitoramentosubacao
			where
				sbaid = " . $sbaid;

		$db->executar( $sql );

		$sql = "
			delete from cte.subacaoindicador
			where
				sbaid = " . $sbaid;

		$db->executar( $sql );
	}
	
	$sql = "select aciid from cte.acaoindicador where ptoid = ".$ptoid;
	$aciids = $db->carregar($sql);
	
	if(is_array($aciids)){
		foreach ( $aciids as $linhas )
		{
			$aciid = (integer) $linhas["aciid"];
			$sql = " delete from cte.pareceracaoindicador where aciid = ".$aciid;
			$db->executar( $sql );
		}
	}

	$sql = "
		delete from cte.acaoindicador
		where
			ptoid = " . $ptoid;

	$return = (boolean) $db->executar( $sql );
    $db->commit();

    return $return;
}


function cte_adicionarPlanoMunicipioPequeno( $ptoid, $crtid )
{
	global $db;
	$ptoid = (integer) $ptoid;
	$crtid = (integer) $crtid;
	
	$acao = cte_pegarPropostaAcaoPequenosMunicipios( $ptoid, $crtid );
	$subacoes = cte_pegarPropostaSubacoesPequenosMunicipios( $acao['ppaid'] );
	$subacoes = $subacoes ? $subacoes : array();
	
	$acidsc = str_replace( "'", "\'", $acao['ppadsc'] );
	
	// cadastra ação
	$sqlAcao = "
		insert into cte.acaoindicador
		( ptoid, acidsc, acilocalizador, usucpf, ppaid )
		values
		( " . $ptoid . ", '" . $acidsc . "', 'M', '" . $_SESSION['usucpf'] . "', '". $acao['ppaid'] ."' )
		returning aciid
	";
	$aciid = (integer) $db->pegaUm( $sqlAcao );
	
	if ( !$aciid )
	{
        echo __LINE__;
        die();

		return;
	}
	
	// cadastra subações
	foreach ( $subacoes as $subacao )
	{
		$sbadsc      = str_replace( "'", "\'", $subacao["ppsdsc"] );
		$undid       = (integer) $subacao["undid"];
		$sbatexto    = str_replace( "'", "\'", $subacao["ppstexto"] );
		$sbaobjetivo = str_replace( "'", "\'", $subacao["ppsobjetivo"] );
		$prgid       = (integer) $subacao["prgid"];
		$sbaordem    = (integer) $subacao["ppsordem"];
		$ppsid       = (integer) $subacao["ppsid"];
		$sbastgmpl   = str_replace( "'", "\'", $subacao["ppsmetodologia"] );
		$frmid       = (integer) $subacao["frmid"];

		$undid = $undid ? $undid : "null";
		$prgid = $prgid ? $prgid : "null";
		$ppsid = $ppsid ? $ppsid : "null";
		$frmid = $frmid ? $frmid : "null";

		$sqlSubAcao = "
			 insert into cte.subacaoindicador
			 (
				 aciid,
				 sbadsc,
				 undid,
				 sbatexto,
				 sbaobjetivo,
				 prgid,
				 sbaordem,
				 ppsid,
				 sbastgmpl,
				 frmid
			 ) values (
				 "  . $aciid       . ",
				 '" . $sbadsc      . "',
				 "  . $undid       . ",
				 '" . $sbatexto    . "',
				 '" . $sbaobjetivo . "',
				 "  . $prgid       . ",
				 "  . $sbaordem    . ",
				 "  . $ppsid       . ",
				 '" . $sbastgmpl   . "',
				 "  . $frmid       . "
			 )";

		$return = $db->executar( $sqlSubAcao );
	}

    return true;
}

function cte_pegarEstufDeMunicipio( $muncod )
{
	global $db;
	$sql = "
		select
			estuf
		from territorios.municipio
		where
			muncod = '" . $muncod . "'
	";
	return $db->pegaUm( $sql );
}

function cte_pegarPropostaSubacoesPequenosMunicipios( $ppaid )
{
	global $db;
	static $propostas = array();
	$ppaid = (integer) $ppaid;
	if ( !array_key_exists( $ppaid, $propostas ) )
	{
		$sqlrecuperamunicipio = "select sam.muncod 
								 from  cte.instrumentounidade iu, 
								       cte.subacaomunicipio as sam 
								 where inuid = ".$_SESSION['inuid']." 
								 and sam.muncod = iu.muncod";
		$codigoMunicipio = $db->carregar( $sqlrecuperamunicipio );
		$muncod = $codigoMunicipio[0]['muncod'];
	
		if(!$muncod){
			//se o municipio não existe na tabela subacaomunicipio busca as subações menos as relacionadas na tabela subacaomunicipio
			$sql= " select * 
				from cte.proposicaosubacao 
				where ppaid = ".$ppaid." and ppsid not in(  
					select psa.ppsid 
					from cte.proposicaosubacao psa
					INNER JOIN cte.subacaomunicipio sam on sam.ppsid = psa.ppsid 
					where ppaid = ".$ppaid.")";
					
		} else{
			// Se existe o codigo do municipio na subacaomunicipio ele recupera todos os dados.
			$sql= " select * 
						 from cte.proposicaosubacao psa 
						 left join  cte.subacaomunicipio sam on sam.ppsid = psa.ppsid
						 where psa.ppaid = ".$ppaid." 
						 and (
								sam.muncod = '".$muncod."' or
								sam.muncod is null
							  );";
		}
		
		/*
			$sql = "select *
					from cte.proposicaosubacao
					where
					ppaid = ".$ppaid."";
		*/
		$propostas[$ppaid] = $db->carregar( $sql );
		
	}
	return $propostas[$ppaid];
}

function cte_pegarPropostaAcaoPequenosMunicipios( $ptoid, $ctrpontuacao )
{
	global $db;
	$ptoid = (integer) $ptoid;
	$sql = "
		select
			crtid
		from cte.pontuacao
		where
			ptoid = " . $ptoid . "
	";
	$crtid = $db->pegaUm( $sql );
	if ( !$crtid )
	{
		return array();
	}
	$sql = "
		select
			pro.ppaid,
			pro.ppadsc
		from cte.proposicaoacao pro
			left outer join cte.criterio crt on
				crt.crtid = pro.crtid
		where
			crt.crtid = " . $crtid . " and
			crt.ctrpontuacao = " . $ctrpontuacao . "
	";

	$propostas = $db->recuperar( $sql );
	$propostas = $propostas ? $propostas : array();
	return $propostas;
}

function cte_criarInstrumentoMunicipio( $itrid, $muncod )
{
	global $db;
	$itrid = (integer) $itrid;
	$inuid = cte_pegarInuidDeMunicipio( $itrid, $muncod );
	if ( !$inuid )
	{
		$estuf = cte_pegarEstufDeMunicipio( $muncod );
		$sql = "
			insert into cte.instrumentounidade
			( itrid, muncod, mun_estuf )
			values ( " . $itrid . ", '" . $muncod . "', '" . $estuf . "' )
			returning inuid
		";
		$inuid = (integer) $db->pegaUm( $sql );
	}
	return $inuid;
}

function cte_criarInstrumentoUf( $itrid, $estuf )
{
	global $db;
	$itrid = (integer) $itrid;
	$inuid = cte_pegarInuidDeUf( $itrid, $estuf );
	if ( !$inuid )
	{
		$sql = "
			insert into cte.instrumentounidade
			( itrid, estuf )
			values ( " . $itrid . ", '" . $estuf . "' )
			returning inuid
		";
		$inuid = (integer) $db->pegaUm( $sql );
	}
	return $inuid;
}

function cte_criarDocumento( $tpdid, $inuid )
{
	global $db;
	$tpdid = (integer) $tpdid;
	$inuid = (integer) $inuid;
	$docid = cte_pegarDocid( $inuid );
	if ( !$docid )
	{
		// verifica se é municipal ou estadual para gerar o nome do documento
		$estuf = cte_pegarEstuf( $inuid );
		$muncod = cte_pegarMuncod( $inuid );
		if ( $estuf )
		{
			$sqlDescricao = "
				select
					estdescricao
				from territorios.estado
				where
					estuf = '" . $estuf . "'
			";
		}
		else
		{
			$sqlDescricao = "
				select
					mundescricao
				from territorios.municipio
				where
					muncod = '" . $muncod . "'
			";
		}
		$descricao = $db->pegaUm( $sqlDescricao );
		$docdsc = "Plano de Metas CTE - " . $descricao;
		
		// cria documento
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );
		if ( $docid )
		{
			$sql = "
				update cte.instrumentounidade
				set
					docid = " . $docid . "
				where
					inuid = " . $inuid;
			$db->executar( $sql );
		}
	}
	return $docid;
}

function cte_indidPertenceInuid( $indid, $inuid )
{
	global $db;
	$indid = (integer) $indid;
	$inuid = (integer) $inuid;
	$sql = "
		select
			count(*)
		from cte.instrumentounidade iu
			inner join cte.dimensao di on
				di.itrid = iu.itrid
			inner join cte.areadimensao ad on
				ad.dimid = di.dimid
			inner join cte.indicador ind on
				ind.ardid = ad.ardid
		where
			iu.inuid = " . $inuid . " and
			ind.indid = " . $indid;
	return $db->pegaUm( $sql ) > 0;
}

function cte_pegarDocid( $inuid )
{
	global $db;
	$inuid = (integer) $inuid;
	$sql = "
		select
			docid
		from cte.instrumentounidade
		where
			inuid = " . $inuid . "
	";
	return (integer) $db->pegaUm( $sql );
}

function cte_pegarDocidEscolaAtiva( $inuid ){
	
	global $db;
	$inuid = (integer) $inuid;
	$sql = "
		select
			docid
		from cte.instrumentounidade
		where
			inuid = " . $inuid . "
	";
	return (integer) $db->pegaUm( $sql );
}

function cte_pegarEstuf( $inuid )
{
	global $db;
	$inuid = (integer) $inuid;
	$sql = "
		select
			estuf
		from cte.instrumentounidade
		where
			inuid = " . $inuid . "
	";
	return $db->pegaUm( $sql );
}

function cte_pegarEstdescricao( $estuf )
{
	global $db;
	$sql = "
		select
			estdescricao
		from territorios.estado
		where
			estuf = '" . $estuf . "'
	";
	return $db->pegaUm( $sql );
}

function cte_pegarMundescricao( $muncod )
{
	global $db;
	$sql = "
		select
			mundescricao || ' - ' || estuf
		from territorios.municipio
		where
			muncod = '" . $muncod . "'
	";
	return $db->pegaUm( $sql );
}

function cte_pegarInuidDeUf( $itrid, $estuf )
{
	global $db;
	$itrid = (integer) $itrid;
	$sql = "
		select
			inuid
		from cte.instrumentounidade
		where
			itrid = " . $itrid . " and
			estuf = '" . $estuf . "'
	";
	return (integer) $db->pegaUm( $sql );
}

function cte_pegarInuidDeMunicipio( $itrid, $muncod )
{
	global $db;
	$itrid = (integer) $itrid;
	$sql = "
		select
			inuid
		from cte.instrumentounidade
		where
			itrid = " . $itrid . " and
			muncod = '" . $muncod . "'
	";
	return (integer) $db->pegaUm( $sql );
}

function cte_pegarItrid( $inuid )
{
	global $db;
	$inuid = (integer) $inuid;
	$sql = "
		select
			itrid
		from cte.instrumentounidade
		where
			inuid = " . $inuid . "
	";
	return (integer) $db->pegaUm( $sql );
}

function cte_pegarMuncod( $inuid )
{
	global $db;
	$inuid = (integer) $inuid;
	$sql = "
		select
			muncod
		from cte.instrumentounidade
		where
			inuid = " . $inuid . "
	";
	return (integer) $db->pegaUm( $sql );
}

function cte_pegarPercentagem( $inuid )
{
	$inuid = (integer) $inuid;
	$itrid = cte_pegarItrid( $inuid );
	global $db;
	$total = $db->pegaUm(
		"
		select
			count(*)
		from cte.instrumento i
			inner join cte.dimensao d on
				d.itrid = i.itrid and
				d.dimstatus = 'A'
			inner join cte.areadimensao a on
				a.dimid = d.dimid and
				a.ardstatus = 'A'
			inner join cte.indicador ind on
				ind.ardid = a.ardid and
				ind.indstatus = 'A'
		where
			i.itrid = " . $itrid . "
		"
	);
	$pontuados = $db->pegaUm(
		"
		select
			count(*)
		from cte.instrumento i
			inner join cte.dimensao d on
				d.itrid = i.itrid and
				d.dimstatus = 'A'
			inner join cte.areadimensao a on
				a.dimid = d.dimid and
				a.ardstatus = 'A'
			inner join cte.indicador ind on
				ind.ardid = a.ardid and
				ind.indstatus = 'A'
			inner join cte.pontuacao p on
				p.indid = ind.indid and
				p.inuid = " . $inuid . "
		where
			i.itrid = " . $itrid . "
		"
	);
	return $total > 0 ? abs( intval( ( $pontuados / $total ) * 100 ) ) : 0;
}

function cte_possuiAcao( $inuid, $indid )
{
	global $db;
	$inuid = (integer) $inuid;
	$indid = (integer) $indid;
	if ( !$inuid || !$indid )
	{
		return false;
	}
	$sql = "
		select
			count(*)
		from cte.pontuacao p
			inner join cte.acaoindicador a on
				a.ptoid = p.ptoid
		where
			p.inuid = " . $inuid . " and
			p.indid = " . $indid . " and
			p.ptostatus = 'A'
	";
	return $db->pegaUm( $sql ) > 0;
}

function cte_possuiPerfilSemVinculo()
{
	global $db;
	$sql = "
		select
			count(*)
		from seguranca.perfil p
			inner join seguranca.perfilusuario u on
				u.pflcod = p.pflcod
			left join cte.tprperfil tp on
				tp.pflcod = p.pflcod
			left join cte.tiporesponsabilidade tr on
				tr.tprcod = tp.tprcod
		where
			p.pflstatus = 'A' and
			p.sisid = " . CTE_SISTEMA . " and
			u.usucpf = '" . $_SESSION['usucpf'] . "' and
			tr.tprcod is null
	";
	return $db->pegaUm( $sql ) > 0;
}

function cte_prgidPertenceInuid( $prgid, $inuid )
{
	global $db;
	$prgid = (integer) $prgid;
	$inuid = (integer) $inuid;
	$sql = "
		select
			count(*)
		from cte.instrumentounidade iu
			inner join cte.dimensao di on
				di.itrid = iu.itrid
			inner join cte.pergunta pe on
				pe.dimid = di.dimid
		where
			iu.inuid = " . $inuid . " and
			pe.prgid = " . $prgid;
	return $db->pegaUm( $sql ) > 0;
}






// ----- RELATÓRIO -------------------------------------------------------------

function cte_agruparDadosRelatorio( array $agrupadores, array $itens )
{
	if ( count( $agrupadores ) == 0 || count( $itens ) == 0 )
	{
		return array();
	}
	
	// captura agrupador atual
	$agrupadorAtual = array_shift( $agrupadores );
	
	// inicia variavel resultante
	$resultado = array();
	
	// percorre itens (realiza agrupamento)
	foreach ( $itens as $item )
	{
		
		$chave = $item[$agrupadorAtual];
		if ( !array_key_exists( $chave, $resultado ) )
		{
			$resultado[$chave] = $item;
			
			unset( $resultado[$chave]['fis_0_original'] );
			unset( $resultado[$chave]['fis_1_original'] );
			unset( $resultado[$chave]['fis_2_original'] );
			unset( $resultado[$chave]['fis_3_original'] );
			unset( $resultado[$chave]['fis_4_original'] );
			unset( $resultado[$chave]['fis_0_copia'] );
			unset( $resultado[$chave]['fis_1_copia'] );
			unset( $resultado[$chave]['fis_2_copia'] );
			unset( $resultado[$chave]['fis_3_copia'] );
			unset( $resultado[$chave]['fis_4_copia'] );
			unset( $resultado[$chave]['fin_1_original'] );
			unset( $resultado[$chave]['fin_0_original'] );
			unset( $resultado[$chave]['fin_2_original'] );
			unset( $resultado[$chave]['fin_3_original'] );
			unset( $resultado[$chave]['fin_4_original'] );
			unset( $resultado[$chave]['fin_0_copia'] );
			unset( $resultado[$chave]['fin_1_copia'] );
			unset( $resultado[$chave]['fin_2_copia'] );
			unset( $resultado[$chave]['fin_3_copia'] );
			unset( $resultado[$chave]['fin_4_copia'] );
			
			// financeiro
				$resultado[$chave]['fin_sol'] = array();
					$resultado[$chave]['fin_sol'][0] = 0;
					$resultado[$chave]['fin_sol'][1] = 0;
					$resultado[$chave]['fin_sol'][2] = 0;
					$resultado[$chave]['fin_sol'][3] = 0;
					$resultado[$chave]['fin_sol'][4] = 0;
				$resultado[$chave]['fin_ate'] = array();
					$resultado[$chave]['fin_ate'][0] = 0;
					$resultado[$chave]['fin_ate'][1] = 0;
					$resultado[$chave]['fin_ate'][2] = 0;
					$resultado[$chave]['fin_ate'][3] = 0;
					$resultado[$chave]['fin_ate'][4] = 0;
			// fisico
				$resultado[$chave]['fis_sol'] = array();
					$resultado[$chave]['fis_sol'][0] = 0;
					$resultado[$chave]['fis_sol'][1] = 0;
					$resultado[$chave]['fis_sol'][2] = 0;
					$resultado[$chave]['fis_sol'][3] = 0;
					$resultado[$chave]['fis_sol'][4] = 0;
				$resultado[$chave]['fis_ate'] = array();
					$resultado[$chave]['fis_ate'][0] = 0;
					$resultado[$chave]['fis_ate'][1] = 0;
					$resultado[$chave]['fis_ate'][2] = 0;
					$resultado[$chave]['fis_ate'][3] = 0;
					$resultado[$chave]['fis_ate'][4] = 0;
			// total
				$resultado[$chave]['tot_sol'] = array();
					$resultado[$chave]['tot_sol'][0] = 0;
					$resultado[$chave]['tot_sol'][1] = 0;
					$resultado[$chave]['tot_sol'][2] = 0;
					$resultado[$chave]['tot_sol'][3] = 0;
					$resultado[$chave]['tot_sol'][4] = 0;
				$resultado[$chave]['tot_ate'] = array();
					$resultado[$chave]['tot_ate'][0] = 0;
					$resultado[$chave]['tot_ate'][1] = 0;
					$resultado[$chave]['tot_ate'][2] = 0;
					$resultado[$chave]['tot_ate'][3] = 0;
					$resultado[$chave]['tot_ate'][4] = 0;
			
			// filhos
			$resultado[$chave]['sub'] = array();
		}
		
		// detecta os posfixo dos campos de valores
		if ( cte_estadoPosCopia( $item['docid'] ) )
		{
			$campoSolicitacao = 'copia';
			$campoAtendimento = 'original';
		}
		else
		{
			$campoSolicitacao = 'original';
			$campoAtendimento = 'copia';
		}
		
		// adiciona valores do item ao agrupador
		
		$resultado[$chave]['fin_sol'][0] += $item['fin_0_' . $campoSolicitacao];
		$resultado[$chave]['fin_sol'][1] += $item['fin_1_' . $campoSolicitacao];
		$resultado[$chave]['fin_sol'][2] += $item['fin_2_' . $campoSolicitacao];
		$resultado[$chave]['fin_sol'][3] += $item['fin_3_' . $campoSolicitacao];
		$resultado[$chave]['fin_sol'][4] += $item['fin_4_' . $campoSolicitacao];
		
		$resultado[$chave]['fin_ate'][0] += $item['fin_0_' . $campoAtendimento];
		$resultado[$chave]['fin_ate'][1] += $item['fin_1_' . $campoAtendimento];
		$resultado[$chave]['fin_ate'][2] += $item['fin_2_' . $campoAtendimento];
		$resultado[$chave]['fin_ate'][3] += $item['fin_3_' . $campoAtendimento];
		$resultado[$chave]['fin_ate'][4] += $item['fin_4_' . $campoAtendimento];
		
		$resultado[$chave]['fis_sol'][0] += $item['fis_0_' . $campoSolicitacao];
		$resultado[$chave]['fis_sol'][1] += $item['fis_1_' . $campoSolicitacao];
		$resultado[$chave]['fis_sol'][2] += $item['fis_2_' . $campoSolicitacao];
		$resultado[$chave]['fis_sol'][3] += $item['fis_3_' . $campoSolicitacao];
		$resultado[$chave]['fis_sol'][4] += $item['fis_4_' . $campoSolicitacao];
		
		$resultado[$chave]['fis_ate'][0] += $item['fis_0_' . $campoAtendimento];
		$resultado[$chave]['fis_ate'][1] += $item['fis_1_' . $campoAtendimento];
		$resultado[$chave]['fis_ate'][2] += $item['fis_2_' . $campoAtendimento];
		$resultado[$chave]['fis_ate'][3] += $item['fis_3_' . $campoAtendimento];
		$resultado[$chave]['fis_ate'][4] += $item['fis_4_' . $campoAtendimento];
		
		// adicionar o item como filho do agrupador caso haja mais agrupadores
		
		if ( count( $agrupadores ) > 0 )
		{
			array_push( $resultado[$chave]['sub'], $item );
		}
		
	}
	
	// agrupa filhos dos filhos do agrupador atual caso haja mais agrupadores
	
	reset( $agrupadores );
	if ( count( $agrupadores ) > 0 )
	{
		reset( $resultado );
		foreach ( $resultado as &$item )
		{
			$item['sub'] = cte_agruparDadosRelatorio( $agrupadores, $item['sub'] );
		}
	}
	
	ksort( $resultado );
	reset( $resultado );
	return $resultado;
}





// ----- OUTROS ----------------------------------------------------------------

function cte_apagaCopiaPlanoAcao($inuid)
{	//	cte_copiarPlanoAcaoHistorico($inuid);
	if (trim((string) $inuid) == '')
        return false;

	global $db;

    $sba = '
    select sbaid from cte.subacaoindicador where aciid in
    (
        select aciid from cte.acaoindicador where ptoid in
        (
            select ptoid from cte.pontuacao where ptostatus = \'C\' and inuid = ' . $inuid  . '))';

    $res = $db->carregar($sba);
	//dbg($res,1);
    if (!$res)
        return true;

    $sba = array();

    while (list(, $dat) = each($res)) {
        $sba[] = $dat['sbaid'];
    }

    $sba = implode(',', $sba);

	$sql = '
    delete from cte.subacaoparecertecnico
    where
        sbaid in (' . $sba . ');

    delete from cte.subacaobeneficiario
    where
        sbaid in (' . $sba . ');

    delete from cte.composicaosubacao
    where
        sbaid in (' . $sba . ');

    delete from cte.qtdfisicoano
    where
        sbaid in (' . $sba . ');
        
    delete from cte.monitoramentosubacao
    where
        sbaid in (' . $sba . ');

    delete from cte.termosubacaoindicador
    where
        sbaid in (' . $sba . ');';

	$db->executar( $sql );

	$db->executar('delete from cte.subacaoindicador where sbaid in (' . $sba . ')');
	$db->executar('delete from cte.pareceracaoindicador where aciid in (
                       select aciid from cte.acaoindicador where ptoid in (select ptoid from cte.pontuacao where ptostatus = \'C\' and inuid = '. $inuid .'))');
	$db->executar('delete from cte.acaoindicador where ptoid in (
                       select ptoid from cte.pontuacao where ptostatus = \'C\' and inuid = ' . $inuid . ')');

	$db->executar('delete from cte.pontuacao where ptostatus = \'C\' and inuid = ' . $inuid);
    
	
    if(cte_copiarPlanoAcaoHistorico($inuid)){
    	$db->commit();
    	return true;
    }else{
    	return false;
    }
    
	//return true;
}

function cte_copiarPlanoAcao($inuid)
{
	
	if (!cte_apagaCopiaPlanoAcao($inuid))
        return false;

	global $db;

	$pontuacao = $db->carregar("select ptoid from cte.pontuacao where ptostatus = 'A' and inuid = $inuid ");

	foreach ( $pontuacao as $pontuacao1 ) {
		$idpontuacao = $pontuacao1['ptoid'];
		$sql = '
        insert into cte.pontuacao (
            crtid,
            ptojustificativa,
            ptodemandamunicipal,
            ptodemandaestadual,
            ptodata,
            usucpf,
            inuid,
            indid,
            ptostatus,
            ptoparecertecnico
        ) select
            crtid,
            ptojustificativa,
            ptodemandamunicipal,
            ptodemandaestadual,
            ptodata,
            usucpf,
            inuid,
            indid,
            \'C\',
            ptoparecertecnico
          from
            cte.pontuacao
          where
            ptoid = ' . $idpontuacao . ' returning ptoid';

		$novoidpontuacao = $db->pegaUm($sql);
		$acao            = $db->carregar('select aciid from cte.acaoindicador where ptoid = ' . $idpontuacao);

		if ( $acao != '' ) {
			foreach ( $acao as $acao1 ):
				$idacao = $acao1['aciid'];
				$sql1 = " insert into cte.acaoindicador 
							(    ptoid,
								  parid,
								  acidsc,
								  acirpns,
								  acicrg,
								  acidtinicial,
								  acidtfinal,
								  acirstd,
								  acilocalizador,
								  acidata,
								  usucpf)
							select 
							  	  $novoidpontuacao,
								  parid,
								  acidsc,
								  acirpns,
								  acicrg,
								  acidtinicial,
								  acidtfinal,
								  acirstd,
								  acilocalizador,
								  acidata,
								  usucpf
							from cte.acaoindicador
							where aciid = $idacao returning aciid; ";
				$novoidacao = $db->pegaUm($sql1);			
	
				$subacao = $db->carregar("select subin.*, props.ppsindcobuni from cte.subacaoindicador as subin 
										  left join cte.proposicaosubacao as props on subin.ppsid = props.ppsid 
										  where aciid =".$idacao);
				if ( $subacao != '' )
				{
					foreach ( $subacao as $subacao1 ):
						$idsubacao = $subacao1['sbaid'];
						$sql2 = " insert into cte.subacaoindicador 
									(    aciid ,
										  undid,
										  frmid,
										  sbadsc,
										  sbastgmpl,
										  sbaprm,
										  sbapcr,
										  sbadata,
										  usucpf,
										  sbaporescola,
										 -- sbaparecer,
										  psuid,
										  ssuid,
										  foaid,
										  prgid,
										  ppsid,
										  sbaordem,
										  sbaobjetivo,
										  sbatexto,
										  sbacategoria
										)
									select 
										  $novoidacao ,
										  undid,
										  frmid,
										  sbadsc,
										  sbastgmpl,
										  sbaprm,
										  sbapcr,
										  sbadata,
										  usucpf,
										  sbaporescola,
										  --sbaparecer,
										  psuid,
										  ssuid,
										  foaid,
										  prgid,
										  ppsid,
										  sbaordem,
										  sbaobjetivo,
										  sbatexto,
										  sbacategoria
									from cte.subacaoindicador
									where sbaid = $idsubacao returning sbaid; ";
									
							$novoidsubacao = $db->pegaUm($sql2);	
							$sqlspt = " insert into cte.subacaoparecertecnico
										(     	sbaid,
											--sptparecer,
											sptunt,
											sptuntdsc,
											sptano,
											sptinicio,
											sptfim,
											tppid
											--ssuid
											)
										select 
											$novoidsubacao,
											--sptparecer,
											sptunt,
											sptuntdsc,
											sptano,
											sptinicio,
											sptfim,
											tppid
											--ssuid
										from cte.subacaoparecertecnico
										where sbaid = ".$idsubacao;	
							$db->carregar($sqlspt);
								$sqlComposicao="
									 insert into cte.composicaosubacao
									(   sbaid,
										cosdsc,
										unddid,
										cosano,
										cosqtd,
										cosvlruni
										)
									select 
										$novoidsubacao,
										cosdsc,
										unddid,
										cosano,
										cosqtd,
										cosvlruni
									from cte.composicaosubacao
									where sbaid = ".$idsubacao;
							$db->carregar($sqlComposicao);
							$sqlQtd=" insert into cte.qtdfisicoano
								(   sbaid,
									qfaano,
									qfaqtd,
									entid
									)
								select 
									$novoidsubacao,
									qfaano,
									qfaqtd,
									entid
								from cte.qtdfisicoano
								where sbaid = ".$idsubacao;	
							$db->carregar($sqlQtd);
							
							$sqlBeneficiario="insert into cte.subacaobeneficiario 
												(	sbaid,
													benid,
													vlrurbano,
													vlrrural,
													sabano
												)
												select
													$novoidsubacao,
													benid,
													vlrurbano,
													vlrrural,
													sabano
												from cte.subacaobeneficiario
												where sbaid = ".$idsubacao;
							$db->carregar($sqlBeneficiario);
							$sqlQuantEscolas="insert into cte.escolacomposicaosubacao 
												(	ecsqtd, 
													qfaid, 
													cosid
												)
												select
													ecs.ecsqtd,
													ecs.qfaid,
													ecs.cosid
												from cte.escolacomposicaosubacao ecs
												inner join cte.composicaosubacao cos on ecs.cosid = cos.cosid
												inner join cte.qtdfisicoano qfa on ecs.qfaid = qfa.qfaid
												inner join cte.subacaoindicador sba on cos.sbaid = sba.sbaid and qfa.sbaid = sba.sbaid
												where sba.sbaid = ".$novoidsubacao;
							$db->carregar($sqlQuantEscolas);
								
					endforeach;
				}
			endforeach;
		}
	}
	
	if( !trataSubacaoCoberturaUniversal( $inuid ) )
		return false;
	
	return true;
}

function trataSubacaoCoberturaUniversal( $inuid ){
	
	global $db;
	
	$sql = "select distinct su.sbaid, ppsparecerpadrao 
			from cte.subacaoindicador su 
				inner join cte.proposicaosubacao ps on su.ppsid = ps.ppsid
				inner join cte.acaoindicador ai on ai.aciid = su.aciid
				inner join cte.pontuacao p on p.ptoid = ai.ptoid
				inner join cte.instrumentounidade iu on iu.inuid = p.inuid
				inner join workflow.documento d on d.docid = iu.docid
			where p.ptostatus = 'A'
			and ppsindcobuni = true
			and d.esdid in ( 2, 10 )
			and iu.inuid = $inuid";
			
	$coSubacoesUniversais = $db->carregar( $sql );
	
	if( is_array( $coSubacoesUniversais ) ){
		foreach( $coSubacoesUniversais as $arSubacaoUniversal ){
	
			$sql = "DELETE FROM cte.subacaoparecertecnico
					WHERE sbaid = {$arSubacaoUniversal["sbaid"]};
					
					DELETE FROM cte.subacaobeneficiario
					WHERE sbaid = {$arSubacaoUniversal["sbaid"]};
					
					DELETE FROM cte.composicaosubacao
					WHERE sbaid = {$arSubacaoUniversal["sbaid"]};
					
					DELETE FROM cte.qtdfisicoano
					WHERE sbaid = {$arSubacaoUniversal["sbaid"]};";
			
					
			if( !$db->executar( $sql ) ) {
				return false;
			}
			
			foreach( cte_recuperArArAno() as $ano ){
												
				$sql = "insert into cte.subacaoparecertecnico  ( sbaid, sptparecer, sptano, ssuid, sptinicio, sptfim, tppid ) 
														values ( {$arSubacaoUniversal["sbaid"]}, 
																'{$arSubacaoUniversal["ppsparecerpadrao"]}', $ano, 3, 0, 0, 4 )";
				
				if( !$db->executar( $sql ) ) {
					return false;
				}
			}					
		}
		$db->commit();
	}
	
	return true;
}



// ----- WORKFLOW --------------------------------------------------------------

function cte_podeEncaminharParaAnalise( $inuid )
{
	global $db;
	$inuid = (integer) $inuid;
	
	// verifica se instrmento é estadual
	$estuf = cte_pegarEstuf( $inuid );
	if ( $estuf )
	{
		return true;
	}
	
	// verifica se existe indicador com pontuação 1 ou 2 sem subacao
	$sql = "
		select
			count(*)
		from cte.pontuacao p
			inner join cte.criterio c on
				c.crtid = p.crtid
			inner join cte.instrumentounidade i on
				i.inuid = p.inuid
			left join cte.acaoindicador a on
				a.ptoid = p.ptoid
			left join cte.subacaoindicador s on
				s.aciid = a.aciid
		where
			p.inuid = " . $inuid . " and
			p.ptostatus = 'A' and
			s.sbaid is null and
			(
				c.ctrpontuacao = 1 or
				c.ctrpontuacao = 2
			)
	";
	$criterioSemSubacao = (integer) $db->pegaUm( $inuid );
	if ( $criterioSemSubacao > 0 )
	{
		return false;
	}
	
	// verifica se existe indicador com pontuação 0, 3 ou 4 com ação
	$sql = "
		select
			count(*)
		from cte.pontuacao p
			inner join cte.criterio c on
				c.crtid = p.crtid
			inner join cte.instrumentounidade i on
				i.inuid = p.inuid
			inner join cte.acaoindicador a on
				a.ptoid = p.ptoid
		where
			p.inuid = " . $inuid . " and
			(
				c.ctrpontuacao = 0 or
				c.ctrpontuacao = 3 or
				c.ctrpontuacao = 4
			)
	";
	$criterioComAcao = (integer) $db->pegaUm( $sql );
	if ( $criterioComAcao > 0 )
	{
		return false;
	}
	
	return true;
}

function cte_verifica_tipo( $inuid )
{
	$itrid = cte_pegarItrid( $inuid );
	return $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL;
}

function cte_verifica_tipo_por_indicador( $indid )
{
	global $db;
	
	$sql = "select ins.itrid from cte.indicador i
				inner join cte.areadimensao ad on i.ardid = ad.ardid
				inner join cte.dimensao d on ad.dimid = d.dimid
				inner join cte.instrumento ins on d.itrid = ins.itrid
			where indid = $indid";
	
	return $db->pegaUm($sql);
}

function verifica_preenchimento( $inuid )
{
	global $db;
	$docid = cte_pegarDocid( $inuid );
	$documento = wf_pegarEstadoAtual( $docid );
	// necessário para evitar que se verifique o preenchimento do PAR na fase de Diagnóstico.
	// Bruno Coura 	
	if (cte_pegarPercentagem( $inuid ) >= 100){
		if($documento['esdid']!=1){
			return !is_array(verifica_erros_preenchimento( $inuid ));
		}else{
			return true;
		}
	} 
	else {
		return false;
	} 
	//return cte_pegarPercentagem( $inuid ) > 99;
}


// ----- ? ---------------------------------------------------------------------

function cte_pegarComposicoes( $sbaid )
{
	global $db;
	$sbaid = (integer) $sbaid;
	$sql = "
		select
			cosord,
			cosdsc,
			cosunimed,
			cosqtd,
			cosvlruni
		from cte.composicaosubacao
		where
			sbaid = " . $sbaid . "
		order by
			cosord
	";
	$composicao = $db->carregar( $sql );
	return $composicao ? $composicao : array();
}

// ----- DOCUMENTOS --------------------------------------------------------------

define( "SUBACAO_ASSISTENCIA_TECNICA", 8 );
define( "FUNCAO_PREFEITURA", 1 );
define( "FUNCAO_PREFEITO", 2 );

function cte_emitirDocumentos( $inuid ){
	if ( cte_pegarItrid( $inuid ) != INSTRUMENTO_DIAGNOSTICO_MUNICIPAL )
	{
		return true;
	}
    $parecer = cte_emitirTermo( $inuid );
    $termo   = cte_emitirParecer( $inuid );
    
    $monitoramento = cte_definirMonitoramento( $inuid );

	return $parecer && $termo && $monitoramento;
}

function dadosPrefeitura($muncod){
	global $db;
	
	$sql = "select e.*, en.*, mnu.*, f.* 
			from entidade.entidade e 
			inner join entidade.funcaoentidade fen on fen.entid = e.entid 
			inner join entidade.funcao f on f.funid = fen.funid 
			inner join entidade.endereco en on en.entid = e.entid
			inner join territorios.municipio mnu on en.muncod = mnu.muncod
			where f.funid = ".FUNCAO_PREFEITURA." and mnu.muncod = '".$muncod."'";
		
		$prefeitura = $db->pegaLinha( $sql );
		return $prefeitura;
}

/**
 * function cte_emitirTermo($inuid);
 * @desc   : Gera e Grava o Termo de Cooperação no BD.
 * @author : ?  | Revisão 2 -> Thiago Tasca Barbosa (29/04/2009)
 * @param  : numeric $inuid (ID da unidade)
 * @return : boleano $terid  (Se Termo gerado retorna id do Termo se não false.)
 * @since ? | 29/04/2009 
 * @version 1 : Criação.
 * @version 2 : Reitrado a função para apagar o Termo.
 */
function cte_emitirTermo( $inuid ){
	global $db;
	if ( cte_pegarItrid( $inuid ) != INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {
		return false;
	}
	try {
		# recupera o numero do processo.
			$sqlnumprocesso = "select numprocesso from cte.instrumentounidadeprocesso where inuid = ".$inuid;
			$numeroProcesso = $db->pegaUm($sqlnumprocesso); 
		if(!$numeroProcesso){
			throw new Exception( "Não foi possível encontrar um numero de processo vinculado a unidade para geração do Termo." );
		}
		$muncod = $db->pegaUm( sprintf( "select muncod from cte.instrumentounidade where inuid = %d", $inuid ) );
		# pega os dados da prefeitura
		$prefeitura = dadosPrefeitura($muncod);
		$prefeitura = $prefeitura ? $prefeitura : null;
		if ( !$prefeitura ) {
			throw new Exception( "Não foi possível encontrar os dados da prefeitura." );
		}
		# pega os dados do prefeito
			$sql ="
			SELECT entprefeito.*, mun.*, entd.*
			FROM entidade.entidade entprefeito 
				INNER JOIN entidade.funcaoentidade funprefeito ON entprefeito.entid = funprefeito.entid 
				INNER JOIN entidade.funentassoc feaprefeito ON feaprefeito.fueid = funprefeito.fueid 
				INNER JOIN entidade.entidade entprefeitura ON entprefeitura.entid = feaprefeito.entid 
				INNER JOIN entidade.funcaoentidade funprefeitura ON funprefeitura.entid = entprefeitura.entid 
				INNER JOIN entidade.endereco entd ON entd.entid = entprefeitura.entid
				INNER JOIN territorios.municipio mun ON entd.muncod = mun.muncod 
			WHERE funprefeito.funid = 2 and mun.muncod ='".$muncod."'";
			
		$prefeito = $db->pegaLinha( $sql );
		$prefeito = $prefeito ? $prefeito : null;
		if ( !$prefeito ) {
			throw new Exception( "Não foi possível encontrar os dados do prefeito." );
		}
		# pega os dados das subações
		$subacoes = recuperaSubaçoesGeracaoTermo($inuid);
		$subacoes = $subacoes ? $subacoes : array();
		
		# cadastra o novo termo
		//cte_excluirTermo( $inuid );
		$cpf = $_SESSION['usucpf'];
		$sql = sprintf( "insert into cte.termo ( inuid, terdocumento, terdata, terusucpf ) values ( %d, '', now(),$cpf ) returning terid", $inuid );
		$terid = $db->pegaUm( $sql );
		if ( !$terid ) {
			throw new Exception( "Ocorreu um erro ao cadastrar o novo Termo de Compromisso." );
		}
		# pega o conteúdo do documento e coloca no banco
		ob_start();
		include APPRAIZ . "www/cte/documento/termo.php";
		$termo = $db->escape( str_replace( "#TERMO#", sprintf( "%05d", $terid ), ob_get_clean() ) );
		$sql = sprintf( "update cte.termo set terdocumento = %s where terid = %d", $termo, $terid );
		if ( !$db->executar( $sql ) ) {
			throw new Exception( "" );
		}
		
		$sqlProcesso =  "update cte.instrumentounidadeprocesso 
								 set indstatus = 'U',
								 	 terid = ".$terid." 
								 where inuid = ".$inuid; 
		if ( !$db->executar( $sqlProcesso ) ) {
			throw new Exception( "Não foi possível alterar o status do Termo." );
		}
		# relaciona as ações ao novo termo
		foreach ( $subacoes as $subacao ) {
			$sql = sprintf(
				"insert into cte.termosubacaoindicador ( terid, sbaid ) values ( %d, '%s' )",
				$terid,
				$subacao["sbaid"]
			);
			if ( !$db->executar( $sql ) ) {
				throw new Exception( "Ocorreu um erro ao cadastrar o novo Termo de Compromisso." );
			}
		}
		return $terid;
	} catch ( Exception $erro ) {
		wf_registrarMensagem( $erro->getMessage() );
		$db->rollback();
		return null;
	}	
}

/**
 * function cte_emitirTermo($inuid);
 * @desc   : Gera e Grava o Parecer no BD.
 * @author : ?  | Revisão 2 -> Thiago Tasca Barbosa (29/04/2009)
 * @param  : numeric $inuid (ID da unidade)
 * @return : boleano $parid  (Se Parecer gerado retorna id do Parecer se não false.)
 * @since ? | 29/04/2009 
 * @version 1 : Criação.
 * @version 2 : Reitrado a função para apagar o Parecer.
 */
function cte_emitirParecer( $inuid ){
	global $db;
	if ( cte_pegarItrid( $inuid ) != INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {
		return false;
	}
	try {
		# recupera o numero do processo.
			$sqlnumprocesso = "select numprocesso from cte.instrumentounidadeprocesso where inuid = ".$inuid;
			$numeroProcesso = $db->pegaUm($sqlnumprocesso); 
		if(!$numeroProcesso){
			throw new Exception( "Não foi possível encontrar um numero de processo vinculado a unidade para geração do Parecer." );
		}
		$muncod = $db->pegaUm( sprintf( "select muncod from cte.instrumentounidade where inuid = %d", $inuid ) );
		# pega os dados da prefeitura
		$prefeitura = dadosPrefeitura($muncod);
		$prefeitura = $prefeitura ? $prefeitura : null;
		if ( !$prefeitura ) {
			throw new Exception( "Não foi possível encontrar os dados da prefeitura." );
		}
			# Recupera os dados do prefeito. #
		$sql = "
			SELECT entprefeito.*, mun.*, entd.*
			FROM entidade.entidade entprefeito 
				INNER JOIN entidade.funcaoentidade funprefeito ON entprefeito.entid = funprefeito.entid 
				INNER JOIN entidade.funentassoc feaprefeito ON feaprefeito.fueid = funprefeito.fueid 
				INNER JOIN entidade.entidade entprefeitura ON entprefeitura.entid = feaprefeito.entid 
				INNER JOIN entidade.funcaoentidade funprefeitura ON funprefeitura.entid = entprefeitura.entid 
				INNER JOIN entidade.endereco entd ON entd.entid = entprefeitura.entid
				INNER JOIN territorios.municipio mun ON entd.muncod = mun.muncod 
			WHERE funprefeito.funid = 2 and mun.muncod = '".$muncod."'";
		
		$prefeito = $db->pegaLinha( $sql );
		$prefeito = $prefeito ? $prefeito : null;
		if ( !$prefeito ) {
			throw new Exception( "Não foi possível encontrar os dados do prefeito." );
		} 
		# cadastra o novo termo
		//cte_excluirParecer( $inuid );
		$cpf = $_SESSION['usucpf'];
		$sql = sprintf( "insert into cte.parecer ( inuid, pardocumento, pardata, usucpf ) values ( %d, '',now(),$cpf) returning parid", $inuid );
		$parid = $db->pegaUm( $sql );
		if ( !$parid ) {
			throw new Exception( "Ocorreu um erro ao cadastrar o novo Termo de Compromisso." );
		}
		# pega as ações
		$sql = sprintf(
			"select ai.aciid, i.indid, ad.ardid, d.dimid
			from cte.pontuacao p
			inner join cte.acaoindicador ai on ai.ptoid = p.ptoid
			inner join cte.indicador i on i.indid = p.indid and i.indstatus = 'A'
			inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
			inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
			where p.inuid = %d",
			$inuid
		);
		$acoes = $db->carregarColuna( $sql, "aciid" );
		$acoes = $acoes ? array_unique( $acoes ) : array();
		$indicadores = $db->carregarColuna( $sql, "indid" );
		$indicadores = $indicadores ? array_unique( $indicadores ) : array();
		$areas = $db->carregarColuna( $sql, "ardid" );
		$areas = $areas ? array_unique( $areas ) : array();
		$dimensoes = $db->carregarColuna( $sql, "dimid" );
		$dimensoes = $dimensoes ? array_unique( $dimensoes ) : array();
		ob_start();
		include APPRAIZ . "www/cte/documento/parecer.php";
		$parecer = $db->escape( str_replace( "#PARECER#", sprintf( "%05d", $parid ), ob_get_clean() ) );
		$sql = sprintf( "update cte.parecer set pardocumento = %s where parid = %d", $parecer, $parid );
		
		if ( !$db->executar( $sql, false ) ) {
			throw new Exception( "Ocorreu um erro ao cadastrar o novo Termo de Compromisso." );
		}
		# Atualiza a tabela instrumentounidadeprocesso
		$sqlProcesso =  "update cte.instrumentounidadeprocesso 
								 set indstatus = 'U',
								 	 parid = ".$parid." 
								 where inuid = ".$inuid; 
		if ( !$db->executar( $sqlProcesso ) ) {
			throw new Exception( "Não foi possível alterar o status do Parecer." );
		}
		
		# relaciona as ações ao novo termo
		foreach ( $acoes as $acao ) {
			$sql = sprintf(
				"insert into cte.pareceracaoindicador ( parid, aciid ) values ( %d, '%s' )",
				$parid,
				$acao
			);
			if ( !$db->executar( $sql ) ) {
				throw new Exception( "Ocorreu um erro ao cadastrar o novo Termo de Compromisso." );
			}
		}
		
		return $parid;
	} catch ( Exception $erro ) {
		wf_registrarMensagem( $erro->getMessage() );
		$db->rollback();
		return false;
	}
}

/**
 * function cte_visualizarTermo($inuid);
 * @desc   : Mostra na tela o último Termo de cooperação do Município.
 * @author : Thiago Tasca Barbosa
 * @param  : numeric $inuid (ID da unidade)
 * @return : string $termo (html com o Termo gerado.)
 * @since  : 29/04/2009
 * @version: 01
 */

function cte_visualizarTermo($inuid, $terid=NULL){
	global $db;
	if ( cte_pegarItrid( $inuid ) != INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {
		return false;
	}
	try {
	# Mostra termos antigos pelo $terid. #
		if($terid){
			$sql = "select terdocumento from cte.termo where inuid = ".$inuid." and terid = ".$terid;
			return (string) $db->pegaUm( $sql );
		}
		
	# Recupera o número do processo. #
		$sqlnumprocesso = "select numprocesso from cte.instrumentounidadeprocesso where inuid = ".$inuid;
		$numeroProcesso = $db->pegaUm($sqlnumprocesso); 
		if(!$numeroProcesso){
			throw new Exception( "Não foi possível encontrar um numero de processo vinculado a unidade para geração do Termo." );
		}
		$muncod = $db->pegaUm( sprintf( "select muncod from cte.instrumentounidade where inuid = %d", $inuid ) );
		
	# Recupera os dados da prefeitura. #
		$sql = sprintf(
			"select ent.*, ende.*, mnu.*, f.*
			from entidade.entidade ent 
			inner join entidade.funcaoentidade fen on fen.entid = ent.entid 
			inner join entidade.funcao f on f.funid = fen.funid 
			inner join entidade.endereco ende on ent.entid = ende.entid
			inner join territorios.municipio mnu on ende.muncod = mnu.muncod
			where mnu.muncod = '%s'
			and fen.funid = %d",
			$muncod,
			FUNCAO_PREFEITURA
		);

		$prefeitura = $db->pegaLinha( $sql );
		$prefeitura = $prefeitura ? $prefeitura : null;
		if ( !$prefeitura ) {
			throw new Exception( "Não foi possível encontrar os dados da prefeitura." );
		}
		
	# Recupera os dados do prefeito. #
		$sql = "
			SELECT entprefeito.*, mun.*, entd.*
			FROM entidade.entidade entprefeito 
				INNER JOIN entidade.funcaoentidade funprefeito ON entprefeito.entid = funprefeito.entid 
				INNER JOIN entidade.funentassoc feaprefeito ON feaprefeito.fueid = funprefeito.fueid 
				INNER JOIN entidade.entidade entprefeitura ON entprefeitura.entid = feaprefeito.entid 
				INNER JOIN entidade.funcaoentidade funprefeitura ON funprefeitura.entid = entprefeitura.entid 
				INNER JOIN entidade.endereco entd ON entd.entid = entprefeitura.entid
				INNER JOIN territorios.municipio mun ON entd.muncod = mun.muncod 
			WHERE funprefeito.funid = 2 and mun.muncod = '".$muncod."'";
		$prefeito = $db->pegaLinha( $sql );
		$prefeito = $prefeito ? $prefeito : null;
				
		if ( !$prefeito ) {
			throw new Exception( "Não foi possível encontrar os dados do prefeito." );
		}
	# Recupera os dados das subações. #
		$subacoes = recuperaSubaçoesGeracaoTermo($inuid);
		$subacoes = $subacoes ? $subacoes : array();
	# Recupera número do parecer que sera criado. #
		$sql = sprintf( "select terid + 1 as terid from cte.termo where inuid = %d order by terid desc limit 1", $inuid );
		$terid = $db->pegaUm( $sql );
		
	# Mostra o novo termo. #
		ob_start();
		include APPRAIZ . "www/cte/documento/termo.php";
		$termo = $db->escape( str_replace( "#TERMO#", sprintf( "%05d", $terid ), ob_get_clean() ) );
		//return utf8_encode($termo);
		return $termo;
		
	} catch ( Exception $erro ) {
		wf_registrarMensagem( $erro->getMessage() );
		echo "<script type=\"text/javascript\">"
            ."alert('".wf_pegarMensagem()."');"
            ."window.close();"
            ."</script>\n";
		return false;
	}
}	

function recuperaSubaçoesGeracaoTermo($inuid){
	global $db;
	$sql = "SELECT  sbaid, 
					sbatexto, 
					sbaobjetivo, 
					ppsid,
					dimdsc, 
					dimcod, 
					dimid,
					prgdsc,
					unddsc,
					sum(quantidadePorEscola + quantidadeGlobal) as quantidade
				FROM (
					SELECT si.sbaid,
						si.sbatexto, 
						si.sbaobjetivo, 
						ps.ppsid,
						d.dimdsc, 
						d.dimcod, 
						d.dimid,
						pg.prgdsc,
						um.unddsc,
						quantidadeGlobal,
						0 AS quantidadePorEscola
					FROM cte.pontuacao 		p
					INNER JOIN cte.acaoindicador 	ai ON ai.ptoid = p.ptoid
					INNER JOIN cte.subacaoindicador si ON si.aciid = ai.aciid AND ( si.sbatexto != '' and si.sbatexto is not null ) AND si.frmid = ".SUBACAO_ASSISTENCIA_TECNICA."
					INNER JOIN (
									SELECT sum(sptunt) as quantidadeGlobal, sa.sbaid 
									FROM cte.subacaoparecertecnico 	  s
									INNER JOIN cte.subacaoindicador   sa ON sa.sbaid = s.sbaid
									INNER JOIN cte.acaoindicador 	  a  ON a.aciid  = sa.aciid
									INNER JOIN cte.pontuacao 	  	  p  ON p.ptoid  = a.ptoid
									INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
									WHERE 	p.ptostatus = 'A' 
										AND iu.inuid = ".$inuid."
										AND s.ssuid = 3
										AND sa.sbaporescola = FALSE
										GROUP BY sa.sbaid 
							    ) spt ON si.sbaid = spt.sbaid 
					LEFT JOIN cte.proposicaosubacao ps on ps.ppsid = si.ppsid
					INNER JOIN cte.indicador 	 i  on i.indid  = p.indid  and i.indstatus  = 'A'
					INNER JOIN cte.areadimensao  ad on ad.ardid = i.ardid  and ad.ardstatus = 'A'
					INNER JOIN cte.dimensao 	 d  on d.dimid  = ad.dimid and d.dimstatus  = 'A'
					LEFT JOIN  cte.programa 	 pg on pg.prgid = si.prgid
					LEFT JOIN  cte.unidademedida um on um.undid = si.undid
					WHERE p.inuid = ".$inuid."
					AND   si.sbaporescola = FALSE
				
					UNION ALL
				
					SELECT si.sbaid,
						si.sbatexto, 
						si.sbaobjetivo, 
						ps.ppsid,
						d.dimdsc, 
						d.dimcod, 
						d.dimid,
						pg.prgdsc,
						um.unddsc,
						quantidadeGlobal,
						0 AS quantidadePorEscola
					FROM cte.pontuacao 		p
					INNER JOIN cte.acaoindicador 	ai ON ai.ptoid = p.ptoid
					INNER JOIN cte.subacaoindicador si ON si.aciid = ai.aciid AND ( si.sbatexto != '' and si.sbatexto is not null ) AND si.frmid = ".SUBACAO_ASSISTENCIA_TECNICA."
					INNER JOIN (
									SELECT sum(s.qfaqtd)  as quantidadeGlobal, sa.sbaid 
									FROM cte.qtdfisicoano s
									INNER JOIN cte.subacaoindicador   sa ON sa.sbaid = s.sbaid
									INNER JOIN cte.subacaoparecertecnico spt ON spt.sbaid = sa.sbaid and spt.sptano = s.qfaano
									INNER JOIN cte.acaoindicador 	  a  ON a.aciid  = sa.aciid
									INNER JOIN cte.pontuacao 	  	  p  ON p.ptoid  = a.ptoid
									INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
									WHERE 	p.ptostatus = 'A' 
										AND iu.inuid = ".$inuid."
										AND spt.ssuid = 3
										AND sa.sbaporescola = TRUE
										GROUP BY sa.sbaid 
						    	) spt ON si.sbaid = spt.sbaid 
					LEFT JOIN cte.proposicaosubacao ps on ps.ppsid = si.ppsid
					INNER JOIN cte.indicador 	 i  on i.indid  = p.indid  and i.indstatus  = 'A'
					INNER JOIN cte.areadimensao  ad on ad.ardid = i.ardid  and ad.ardstatus = 'A'
					INNER JOIN cte.dimensao 	 d  on d.dimid  = ad.dimid and d.dimstatus  = 'A'
					LEFT JOIN  cte.programa 	 pg on pg.prgid = si.prgid
					LEFT JOIN  cte.unidademedida um on um.undid = si.undid
					WHERE p.inuid = ".$inuid."
					AND   si.sbaporescola = TRUE
				) AS foo
				GROUP BY sbaid,
					 sbatexto, 
					 sbaobjetivo, 
					 ppsid,
					 dimdsc, 
					 dimcod, 
					 dimid,
					 prgdsc,
					 unddsc,
					 quantidadePorEscola,
					 quantidadeGlobal
				order by dimcod";
	//dbg($sql,1);
	$subacoes = $db->carregar( $sql );
	return $subacoes;
}
/**
 * function cte_visualizarParecer($inuid);
 * @desc   : Mostra na tela o último Parecer do Município.
 * @author : Thiago Tasca Barbosa
 * @param  : numeric $inuid (ID da unidade)
 * @return : string $termo (html com o Parecer gerado.)
 * @since  : 29/04/2009
 * @version: 01
 */
function cte_visualizarParecer($inuid, $parid=NULL){
	global $db;
	if ( cte_pegarItrid( $inuid ) != INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {
		return false;
	}
	try {
	# Mostra Pareceres antigos pelo $parid. #
		if($parid){
			
			$sql = "select pardocumento from cte.parecer where inuid = ".$inuid." and parid = ".$parid;	
			return (string) $db->pegaUm( $sql );
		}	
	# Recupera o número do processo. #
		$sqlnumprocesso = "select numprocesso from cte.instrumentounidadeprocesso where inuid = ".$inuid;
		$numeroProcesso = $db->pegaUm($sqlnumprocesso); 
		if(!$numeroProcesso){
			throw new Exception( "Não foi possível encontrar um numero de processo vinculado a unidade para geração do Parecer." );
		}
		
	# Código do Município #
		$muncod = $db->pegaUm( sprintf( "select muncod from cte.instrumentounidade where inuid = %d", $inuid ) );
		
	# Recupera os dados da prefeitura. #
		$sql = sprintf(
			"select ent.*, ende.*, mnu.*, f.*
			from entidade.entidade ent 
			inner join entidade.funcaoentidade fen on fen.entid = ent.entid
			inner join entidade.funcao f on f.funid = fen.funid 
			inner join entidade.endereco ende on ent.entid = ende.entid
			inner join territorios.municipio mnu on ende.muncod = mnu.muncod
			where mnu.muncod = '%s'
			and fen.funid = %d",
			$muncod,
			FUNCAO_PREFEITURA);
		$prefeitura = $db->pegaLinha( $sql );
		$prefeitura = $prefeitura ? $prefeitura : null;
		if ( !$prefeitura ) {
			throw new Exception( "Não foi possível encontrar os dados da prefeitura." );
		}
		
	# Recupera os dados do prefeito. #
		$sql = "
			SELECT entprefeito.*, mun.*, entd.*
			FROM entidade.entidade entprefeito 
				INNER JOIN entidade.funcaoentidade funprefeito ON entprefeito.entid = funprefeito.entid 
				INNER JOIN entidade.funentassoc feaprefeito ON feaprefeito.fueid = funprefeito.fueid 
				INNER JOIN entidade.entidade entprefeitura ON entprefeitura.entid = feaprefeito.entid 
				INNER JOIN entidade.funcaoentidade funprefeitura ON funprefeitura.entid = entprefeitura.entid 
				INNER JOIN entidade.endereco entd ON entd.entid = entprefeitura.entid
				INNER JOIN territorios.municipio mun ON entd.muncod = mun.muncod 
			WHERE funprefeito.funid = 2 and mun.muncod = '".$muncod."'";
		
		$prefeito = $db->pegaLinha( $sql );
		$prefeito = $prefeito ? $prefeito : null;
		if ( !$prefeito ) {
			throw new Exception( "Não foi possível encontrar os dados do prefeito." );
		} 
	# Recupera as ações. #
		$sql = sprintf(
			"select ai.aciid, i.indid, ad.ardid, d.dimid
			from cte.pontuacao p
			inner join cte.acaoindicador ai on ai.ptoid = p.ptoid
			inner join cte.indicador i on i.indid = p.indid and i.indstatus = 'A'
			inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
			inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
			where p.inuid = %d",
			$inuid
		);
		$acoes = $db->carregarColuna( $sql, "aciid" );
		$acoes = $acoes ? array_unique( $acoes ) : array();
		$indicadores = $db->carregarColuna( $sql, "indid" );
		$indicadores = $indicadores ? array_unique( $indicadores ) : array();
		$areas = $db->carregarColuna( $sql, "ardid" );
		$areas = $areas ? array_unique( $areas ) : array();
		$dimensoes = $db->carregarColuna( $sql, "dimid" );
		$dimensoes = $dimensoes ? array_unique( $dimensoes ) : array();
	# Recupera número do parecer que sera criado. #
		$sql = sprintf( "select parid + 1 as parid from cte.parecer where inuid = %d order by parid desc limit 1", $inuid );
		$parid = $db->pegaUm( $sql );
		
	# Mostra Parecer. #
		ob_start();
		include APPRAIZ . "www/cte/documento/parecer.php";
		$parecer = $db->escape( str_replace( "#PARECER#", sprintf( "%05d", $parid ), ob_get_clean() ) );
		return $parecer;
		
	} catch ( Exception $erro ) {
		wf_registrarMensagem( $erro->getMessage() );
		return false;
	}	
}


function cte_excluirDocumentos( $inuid ){
	if ( cte_pegarItrid( $inuid ) != INSTRUMENTO_DIAGNOSTICO_MUNICIPAL )
	{
		return true;
	}
	$parecer = cte_excluirParecer( $inuid );
	$termo = cte_excluirTermo( $inuid );
	return $parecer && $termo;
}

function cte_excluirTermo( $inuid ){
	
	global $db;
	
	try{
		$sql = sprintf(
			"delete from cte.termosubacaoindicador where terid in ( select terid from cte.termo where inuid = %d )",
			$inuid
		);
		if ( !$db->executar( $sql ) ) {
			throw new Exception( "Ocorreu um erro ao excluir relação das subações com o Termo de Compromisso antigo." );
		}
		$sql = sprintf(
			"delete from cte.termo where inuid = %d",
			$inuid
		);
		if ( !$db->executar( $sql ) ) {
			throw new Exception( "Ocorreu um erro ao excluir o Termo de Compromisso antigo." );
		}
		# Altera o status do numero de processo.
		$sqlProcesso = "update cte.instrumentounidadeprocesso 
								 set indstatus = 'D',
								 	 terid = null 
								 where inuid = ".$inuid; 

		if ( !$db->executar( $sqlProcesso ) ) {
			throw new Exception( "Ocorreu um erro ao tentar desvincular o número de processo ao  Termo de Compromisso antigo" );
		}
		
		return true;
	} catch ( Exception $erro ) {
		$db->rollback();
		return false;
	}
}

function cte_excluirParecer( $inuid ){
	global $db;
	try {
		$sql = sprintf(
			"delete from cte.pareceracaoindicador where parid in ( select parid from cte.parecer where inuid = %d )",
			$inuid
		);
		if ( !$db->executar( $sql ) ) {
			throw new Exception( "Ocorreu um erro ao excluir relação das subações com o Parecer antigo." );
		}
		$sql = sprintf(
			"delete from cte.parecer where inuid = %d",
			$inuid
		);
		if ( !$db->executar( $sql ) ) {
			throw new Exception( "Ocorreu um erro ao excluir o Parecer antigo." );
		}
		# Altera o status do numero de processo.
		$sqlProcesso = "update cte.instrumentounidadeprocesso 
								 set indstatus = 'D',
								 	 parid = null 
								 where inuid = ".$inuid; 

		if ( !$db->executar( $sqlProcesso ) ) {
			throw new Exception( "Ocorreu um erro ao tentar desvincular o número de processo ao  Parecer antigo" );
		}
		return true;
	} catch ( Exception $erro ) {
		$db->rollback();
		return false;
	}
}


function cte_verificaTermo($inuid)
{
	global $db;
	$sql = sprintf( "select count(*) as total from cte.termo where inuid = %d", $inuid );
	return $db->pegaUm( $sql ) > 0;
}


function cte_pegarTermo( $inuid ){
	global $db;	
	$sql = sprintf( "select terdocumento from cte.termo where inuid = %d order by terid desc limit 1", $inuid );
	return (string) $db->pegaUm( $sql );
}

function cte_pegarParecer( $inuid ){
	global $db;
	$sql = sprintf( "select pardocumento from cte.parecer where inuid = %d order by parid desc limit 1", $inuid );
	return (string) $db->pegaUm( $sql );
}


/**
 * function cte_assTecnAnalisada
 * Função que valida a fase de Análise - Assistência Técnica 
 * @param int $inuid - Identificação do Instrumento Unidade
 * @return bool - Retorna true ou false em caso de estar validado ou não, respectivamente   
 * @author Orion Teles de Mesquita
 * @since 21/11/2008
 */
function cte_assTecnAnalisada( $inuid ){
	
	$docid = cte_pegarDocid( $_SESSION['inuid'] );
	$estado_documento = wf_pegarEstadoAtual( $docid );
	
	global $db;
	
	// verifica se é Estado
	if ( cte_pegarItrid( $inuid ) == INSTRUMENTO_DIAGNOSTICO_ESTADUAL   ) {
		return true;
	}
	
	// Recuperando todas as subações de um Instrumento Unidade
	$sql = "select sbaid from cte.subacaoindicador su 
				inner join cte.acaoindicador ai on ai.aciid = su.aciid
				inner join cte.pontuacao p on p.ptoid = ai.ptoid
				inner join cte.instrumentounidade iu on iu.inuid = p.inuid
			where p.ptostatus = 'A'  and coalesce(sbasituacaoarvore,1) = 1
			and su.ppsid not in ( 278, 449, 448, 1016, 1023, 1024, 272, 1014, 1015, 1017, 1026, 1025, 818, 467, 1037, 1039, 1041, 89, 88, 1038, 1040, 1042, 273, 410, 411, 1018, 1027, 1028, 413, 412, 1019, 1029, 1030, 274, 275, 414, 415, 1020, 1031, 1032, 417, 416, 1021, 1033, 1034, 276, 418, 419, 1022, 1035, 1036, 277, 1048, 1044, 1045, 1046, 1047 )
			-- Deixar esse comentário para facilitar futuros debugs			
			-- and sbaid = 1321428
			and iu.inuid = " . $inuid;

	$res = $db->carregar( $sql );
	
	// Criando o array onde serão armazenadas as validações 
	$arAnalisado = array();
	
	if( is_array( $res ) ){
		
		// Para cada subação verifica-se se esta está validada
		foreach( $res as $subacao ){
			$sql = "select sbaid, frmid, sbaporescola from cte.subacaoindicador where sbaid = ". $subacao["sbaid"];
			$subacao = $db->pegaLinha( $sql );
			
			$fase = cte_possuiFormaExecucaoTecnica( $subacao["frmid"] ) ? FORMA_EXECUCAO_ASS_TEC : FORMA_EXECUCAO_ASS_FIN;

			// Array com o resultado das validações separados pela fase (Assistência Técnica ou Financeira).
			$arAnalisado[$fase][] = cte_validarSubAcaoFaseAnalise( $subacao );
			
		} // Fim de foreach( $res as $subacao )
	} // Fim de if( is_array( $res ) )
	
	$formaExecucao = $estado_documento["esdid"] == CTE_ESTADO_ANALISE ? FORMA_EXECUCAO_ASS_TEC : FORMA_EXECUCAO_ASS_FIN;
	
	// Se possuir pelo menos um valor falso no índice de Assistência Técnica, retorna falso, caso contrário, verdadeiro.
	if( isset( $arAnalisado[$formaExecucao] ) ){
		return !in_array( false, $arAnalisado[$formaExecucao], true );
	}
	else{
		return true;
	}
}

function cte_assFinanAnalisada( $inuid )
{
	global $db;
	$inuid = (integer) $inuid;
	$sql = "
		select
			count(*)
		from cte.instrumentounidade iu
			inner join cte.pontuacao p on
				p.inuid = iu.inuid
			inner join cte.acaoindicador ai on
				ai.ptoid = p.ptoid
			inner join cte.subacaoindicador si on
				si.aciid = ai.aciid
			inner join cte.subacaoparecertecnico spt on 
				si.sbaid = spt.sbaid
		where
			iu.inuid = " . $inuid . " and
			p.ptostatus = 'A' and
			si.frmid != " . FORMA_EXECUCAO_ASS_TEC . "  and
			spt.ssuid is null
	";
	return $db->pegaUm( $sql ) == 0;
}

function cte_parecerFinalizado($inuid)
{
    global $db;
    $sql = "SELECT count(*) FROM cte.parecerpar 
    		WHERE tppid <> 3 
    		AND inuid = ".$inuid." 
    		AND to_char(pardata, 'YYYY')::integer = date_part('year', current_date)";
    $res = $db->pegaUm($sql);

    return $res >= 1;
}


function cte_convenioFNDEConcluido($inuid)
{
	global $db;
	//$sql = "select count(*) from cte.convenio where inuid =".$inuid;
	$sql = "select count(*) from cte.projetosape where inuid =".$inuid;
	$resultado = $db->pegaUm($sql) ?  true :  false;
	return $resultado;
}

function cte_pegarMuncodEstatual($inuid){
	global $db;
	$inuid = (integer) $inuid;
	$sql = "select e.muncodcapital 
			from cte.instrumentounidade iu 
			INNER JOIN territorios.estado e 
			ON iu.estuf = e.estuf 
			where inuid =".$inuid;

	return (integer) $db->pegaUm( $sql );	
		
}

function cte_removeConvenio($inuid)
{
/*
	global $db;
	
	try{
		$sql = "delete from cte.subacaoconvenio where cnvid in ( select cnvid from cte.convenio where inuid = $inuid )";

		if (!$db->executar( $sql )) {
			throw new Exception( "Ocorreu um erro ao tentar excluir a relação das subações com o convênio." );
		}
		$sql = "delete from cte.convenio where inuid =".$inuid;
		if (!$db->executar( $sql )) {
			throw new Exception( "Ocorreu um erro ao tentar excluir o convênio." );
		}
		$db->commit();
		return true;
	} catch ( Exception $erro ) {
		$db->rollback();
		return false;
	}
*/
	return true;

}


/*
 * Função criada por Alexandre Dourado
 * Data: 06/08/2008
 * Validar se a subacao possui algum cronograma preenchido ( pelo menos de algum ano ), com exceção 
 * se for Cobertural Universal MEC
 * Atualizada por : Thiago Tasca Barbosa para a nova Versão do PAR (PorSubações) Data: 04/08/2008
 *                : Bruno Adann em 08/08/2008 - Correção da Rotina de Verificação de Erros.
 *                : Orion Teles em 29/10/2008 - Reformulação da Rotina de Verificação de Erros.
 */
function cte_validarSubAcao( $subacao, $nrAno = null ){

	$docid = cte_pegarDocid( $_SESSION['inuid'] );
	$estado_documento = wf_pegarEstadoAtual( $docid );	
	
	if( $subacao['ppsindcobuni'] == 't' )
        return false;

    global $db;

    $sbaid  = $subacao['sbaid'];
    $sbadsc = $subacao['sbadsc'];
	$arErro = array();

	// Inicializando arrays que verificarão consistência de cada ano da sub-ação
	$anoParecer = array();
	$anoEscola = array();
	$anoItens = array();
	$anoBeneficiario = array();
	
	// Recuperando Quantidade (somente cronograma global) e datas de início e fim do cronograma físico
	$sql = '
			SELECT  sbaid, coalesce(sptano, 0) as sptano, '; 
	$sql .= $subacao['sbaporescola'] == 'f' ? ' coalesce(sptunt, 0) AS sptunt, ' : ''; 
	$sql .= ' coalesce(sptinicio, 0) AS sptinicio,
				coalesce(sptfim, 0) AS sptfim 
			FROM cte.subacaoparecertecnico
			WHERE  sbaid = '. $sbaid .'
			AND sptano != 0 ';
	$sql .= $nrAno ? ' AND sptano = '. $nrAno : "";
	
	$res = $db->carregar($sql);
	
	if( is_array( $res ) ){
		
		// Varre todos os anos que tem cronograma físico (parecer) cadastrado e faz as verificações
		foreach( $res as $parecer ){
			
			// Definindo as condições para cronograma global e por escola do cronograma físico
			// Se o cronograma for global, deve-se ter quantidade e datas de início e fim do cronograma físico 
			// Se o cronograma for por escola, deve-se ter as datas de início e fim do cronograma físico
			// As outras verificações serão abordadas mais abaixo 
			if( $subacao['sbaporescola'] == 'f' ){
				$condicao1 = ( $parecer["sptunt"] && $parecer["sptinicio"] && $parecer["sptfim"] );
				$condicao2 = ( $parecer["sptunt"] || $parecer["sptinicio"] || $parecer["sptfim"] );
				$msgErro = "É obrigatório o preenchimento da Quantidade e das Datas de Início e Fim do Cronograma Físico em ". $parecer['sptano'];
			}
			else{
				$condicao1 = ( $parecer["sptinicio"] && $parecer["sptfim"] );
				$condicao2 = ( $parecer["sptinicio"] || $parecer["sptfim"] );
				$msgErro = "É obrigatório o preenchimento das Datas de Início e Fim do Cronograma Físico em ". $parecer['sptano'];
			}
			
			// Se obedecer a condição 1 definida acima, a subação não possui o erro inicial
			if( $condicao1 ){
				// Preenche o array com os anos que tem o cronograma físico cadastrados
				$anoParecer[] = $parecer["sptano"];
			}
			// Se não obedecer e tiver algum dos itens do cronograma físico em branco com algum outro preenchido (condição 2) tem erro
			elseif( in_array( "0", $parecer ) && ( $condicao2 ) ){
				$arErro[$sbaid]['sbadsc'] = $sbadsc;
				$arErro[$sbaid][] = $msgErro; 
			}
			
		} // Fim de foreach( $res as $parecer )
		
	} // Fim de if( is_array( $res ) )
	
	$arUnidades = cte_recuperarArUnidadesExigemCPF();
	$undid = $subacao["undid"];
	
	// Se cronograma for por escola, além de verificar validação do cronograma físico (realizado acima), 
	// deve-se verificar também se há pelo menos uma escola cadastrada, se essa escola tem itens de composição e
	// se os itens de composição tem quantidade cadastrada. 
	if( $subacao['sbaporescola'] == 't' ){
        
		// Verificação de Escolas cadastradas
		$sql = 'SELECT qfa.qfaqtd, coalesce(qfaano, 0) as qfaano
        		FROM cte.subacaoindicador sba
					INNER JOIN cte.qtdfisicoano qfa ON qfa.sbaid = sba.sbaid	
				WHERE sba.sbaid = '. $sbaid .'
				AND qfaano != 0';
		$sql .= $nrAno ? ' AND qfaano = '. $nrAno : "";

		$res = $db->carregar($sql);
		
		if( is_array( $res ) ){
			foreach( $res as $arQtd ){

				// Preenche o array com os anos das escolas cadastradas
				$anoEscola[] = $arQtd["qfaano"];
				
				// Verifica se escolas tem quantidade.
				if( !$arQtd["qfaqtd"] ){
					$arErro[$sbaid]['sbadsc'] = $sbadsc;
					$arErro[$sbaid][] = "Deve ser lançada quantidade para todas as escolas cadastradas em ". $arQtd["cosano"] .".";
				}
			}
		}
		
		// Retira anos repetidos de escolas
		$anoEscola = array_unique( $anoEscola );
		
		// Verificação de Itens de Composição		
		$sql = 'SELECT cos.cosid, coalesce(cos.cosano, 0) as cosano, cosqtd, sum( ecsqtd ) as qtdtotal
				FROM cte.subacaoindicador sba
					INNER JOIN cte.composicaosubacao cos ON sba.sbaid = cos.sbaid 
					LEFT JOIN cte.escolacomposicaosubacao ecs ON ecs.cosid = cos.cosid
				WHERE sba.sbaid = '. $sbaid .'
				AND cos.cosano != 0 ';
		$sql .= $nrAno ? ' AND cosano = '. $nrAno : "";				
		$sql .= ' GROUP BY cos.cosid, cos.cosano, cosqtd';

		$res = $db->carregar($sql);
		
		if( !in_array( $undid, $arUnidades ) ){
		
			if( is_array( $res ) ){
				foreach( $res as $arQtd ){
	
					// Preenche o array com os anos dos itens de composição cadastrados
					$anoItens[] = $arQtd["cosano"];
	
					// Verifica se tem itens tem quantidade.
					if( !$arQtd["qtdtotal"] && !$arQtd["cosqtd"] ){
						$arErro[$sbaid]['sbadsc'] = $sbadsc;
						$arErro[$sbaid][] = "Deve ser lançada quantidade para todos os itens de composição em ". $arQtd["cosano"] .".";
					}
				}
			}
		}
		
		// Retira anos repetidos dos itens
		$anoItens = array_unique( $anoItens );
		
		
		/****************************************************************************************
		* 						VERIFICAÇÕES DOS ERROS POR ESCOLA								*				
		****************************************************************************************/
		
		// Se existir um ano com cronograma físico preenchido e não tiver escolas, acusa o erro 
		// Somente para forma de execução "Assistência Financeira": Se existir um ano com cronograma físico preenchido e não tiver Itens de Composição, acusa o erro 
		foreach( $anoParecer as $ano ){
			if( !in_array( $ano, $anoEscola ) ){
				$arErro[$sbaid]['sbadsc'] = $sbadsc;
				$arErro[$sbaid][] = "Devem ser cadastradas Escolas em $ano.";
			}
			if( cte_possuiFormaExecucaoFinanceira( $subacao['frmid'] ) ){
				if( !in_array( $ano, $anoItens ) ){
					$arErro[$sbaid]['sbadsc'] = $sbadsc;
					$arErro[$sbaid][] = "Devem ser cadastrados Itens de Composição em $ano.";
				}
			}	
		}
		
		// Se existir um ano com escolas cadastradas e não tiver Cronograma Físico, acusa o erro 
		// Somente para forma de execução "Assistência Financeira": Se existir um ano com escolas cadastradas e não tiver Itens de Composição, acusa o erro 
		foreach( $anoEscola as $ano ){
			if( !in_array( $ano, $anoParecer ) ){
				$arErro[$sbaid]['sbadsc'] = $sbadsc;
				$arErro[$sbaid][] = "Devem ser cadastradas as Datas de Início e Fim do Cronograma Físico em $ano.";
			}
			if( cte_possuiFormaExecucaoFinanceira( $subacao['frmid'] ) ){
				if( !in_array( $ano, $anoItens ) ){
					$arErro[$sbaid]['sbadsc'] = $sbadsc;
					$arErro[$sbaid][] = "Devem ser cadastrados Itens de Composição em $ano.";
				}
			}
		}
		
		// Se existir um ano com Itens de Composição cadastrados e não tiver Cronograma Físico, acusa o erro 
		// Se existir um ano com Itens de Composição cadastrados e não tiver Escolas, acusa o erro 
		foreach( $anoItens as $ano ){
			if( !in_array( $ano, $anoParecer ) ){
				$arErro[$sbaid]['sbadsc'] = $sbadsc;
				$arErro[$sbaid][] = "Devem ser cadastradas as Datas de Início e Fim do Cronograma Físico em $ano.";
			}
			if( !in_array( $ano, $anoEscola ) ){
				$arErro[$sbaid]['sbadsc'] = $sbadsc;
				$arErro[$sbaid][] = "Devem ser cadastradas Escolas em $ano.";
			}
		}
	} // Fim de if( $subacao['sbaporescola'] == 't' )
	else{

		// Verificação de Itens de Composição	
		$sql = 'SELECT cosqtd, cosano FROM cte.composicaosubacao WHERE sbaid = '. $sbaid;
		$sql .= $nrAno ? ' AND cosano = '. $nrAno : "";
		
		$resItens = $db->carregar($sql);
		if(!in_array( $undid, $arUnidades ) || cte_possuiFormaExecucaoFinanceira( $subacao['frmid'] )){
			if( is_array( $resItens ) ){
				foreach( $resItens as $arQtd ){
					
					// Preenche o array com os anos dos itens de composição cadastrados
					$anoItens[] = $arQtd["cosano"];
					
					// Verifica se tem itens tem quantidade.				
					if( !$arQtd["cosqtd"] ){
						$arErro[$sbaid]['sbadsc'] = $sbadsc;
						$arErro[$sbaid][] = "Deve ser lançada quantidade para todos os itens de composição em ". $arQtd["cosano"] .".";
					}
				}
			}
			
		}else if( !in_array( $undid, $arUnidades ) ){
			
			if( is_array( $resItens ) ){
				foreach( $resItens as $arQtd ){
					
					// Preenche o array com os anos dos itens de composição cadastrados
					$anoItens[] = $arQtd["cosano"];
					
					// Verifica se tem itens tem quantidade.				
					if( !$arQtd["cosqtd"] ){
						$arErro[$sbaid]['sbadsc'] = $sbadsc;
						$arErro[$sbaid][] = "Deve ser lançada quantidade para todos os itens de composição em ". $arQtd["cosano"] .".";
					}
				}
			}	
		}

		$anoItens = array_unique( $anoItens );

		// Se existir um ano com Itens de Composição cadastrados e não tiver Cronograma Físico, acusa o erro		
		foreach( $anoItens as $ano ){
			if( !in_array( $ano, $anoParecer ) ){
				$arErro[$sbaid]['sbadsc'] = $sbadsc;
				$arErro[$sbaid][] = "Devem ser cadastrados Quantidade e Datas de Início e Fim do Cronograma físico em $ano.";
			}
		}		
	} // Fim de Else
        
	// Verificação de Beneficiários
	$sql = 'SELECT coalesce(sabano, 0) as sabano 
			FROM cte.subacaobeneficiario 
			WHERE sbaid = '. $sbaid .'
			AND sabano != 0 ';
	$sql .= $nrAno ? ' AND sabano = '. $nrAno : "";			
	$sql .= ' GROUP BY sabano';
	
	$resBeneficiario = $db->carregar($sql);
	
	if( is_array( $resBeneficiario ) ){
		foreach( $resBeneficiario as $ano ){
			// Preenche o array com os anos dos Beneficiários cadastrados
			$anoBeneficiario[] = $ano["sabano"];
		}
	}

	// Se existir um ano com beneficiários cadastrados e não tiver Cronograma Físico, acusa o erro
	foreach( $anoBeneficiario as $ano ){
		if( !in_array( $ano, $anoParecer ) ){
			$arErro[$sbaid]['sbadsc'] = $sbadsc;
			if( $subacao['sbaporescola'] == 't' )
				$arErro[$sbaid][] = "Devem ser cadastradas as Datas de Início e Fim do Cronograma Físico em $ano.";
			else	
				$arErro[$sbaid][] = "Devem ser cadastrados Quantidade e Datas de Início e Fim do Cronograma físico em $ano.";
		}
	}	
	
	// Se for assistência financeira
	if( cte_possuiFormaExecucaoFinanceira( $subacao['frmid'] ) ){

		// Obriga o preenchimento de beneficiários no ano que tem cronograma físico preenchido
		foreach( $anoParecer as $ano ){
			if( !in_array( $ano, $anoBeneficiario ) ){
				$arErro[$sbaid]['sbadsc'] = $sbadsc;
				$arErro[$sbaid][] = "Subações com forma de execução de assistência financeira deve conter beneficiário em $ano.";
			}
		}
		
		if( $subacao['sbaporescola'] == 'f' ){
			
			// Obriga o preenchimento de itens de composição no ano que tem cronograma físico preenchido e se for cronograma global
			foreach( $anoParecer as $ano ){
				if( !in_array( $ano, $anoItens ) ){
					$arErro[$sbaid]['sbadsc'] = $sbadsc;
					$arErro[$sbaid][] = "Subações com forma de execução de assistência financeira deve conter itens de composição em $ano.";
				}
			}
		}
	}

	// Retira as mensagens de erro repetidas
	if( is_array( $arErro[$sbaid] ) ){
		$arErro[$sbaid] = array_unique( $arErro[$sbaid] );
		 	
		$sql =	" select sbaid, indcod, ardcod, dimcod 
				  from cte.subacaoindicador s
					  inner join cte.acaoindicador ai on s.aciid = ai.aciid
					  inner join cte.pontuacao p on ai.ptoid = p.ptoid
					  inner join cte.indicador i on i.indid = p.indid
					  inner join cte.areadimensao a on i.ardid = a.ardid
					  inner join cte.dimensao d on d.dimid = a.dimid
				  where sbaid = $sbaid";
				  
		$res = $db->pegaLinha($sql);

		$arErro[$sbaid]["posicao"] = $res["dimcod"].".".$res["ardcod"].".".$res["indcod"]; 
	}
	
	if( $estado_documento["esdid"] == CTE_ESTADO_PAR ){
		
		// Se tiver o erro inicial seta mensagem de erro
		if( !count( $anoParecer ) && !count( $anoEscola ) && !count( $anoItens ) && !count( $anoBeneficiario ) ){
			$arErro[$sbaid]['sbadsc'] = $sbadsc;
			$arErro[$sbaid][] = "Não pode haver sub-ação sem nenhum valor lançado";
		}
	}
		
	// Retorna o array de erros	
    return count($arErro) ? $arErro : false;	
	
}




/*
 * Função criada por Alexandre Dourado
 * Data: 06/08/2008
 * Imprime os erros encontrados durante a tramitação do documento
 * Atualizada por : Thiago Tasca Barbosa para a nova Versão do PAR (PorSubações) Data: 04/08/2008
 * 					Thiago Tasca Barbosa inserção de verificação de Ações Data: 24/11/2008
 */

function cte_exibeErrosSubAcao($erros) {

    echo '<html>'
        ,'<head>'
        ,'<title>Verificação de pendências em subações</title>'
        ,'<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">'
        ,'<link rel="stylesheet" type="text/css" href="/includes/listagem.css">'
        ,'<script>'
        ,'function alterarSubacao(sbaid){'
        ,'var janela=window.open("/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid="+sbaid,"detalhesSubacao","height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes");'
        ,'janela.focus();}'
        ,'function alterarAcao(aciid){'
        ,'var janela=window.open("/cte/cte.php?modulo=principal/par_acao_pendencias&acao=A&aciid="+aciid,"detalhesAcao","height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes");'
        ,'janela.focus();}'
        ,'</script>'
        ,'</head>'
        ,'<body>'
        ,'<div style="width:100%;height:100%;overflow-y:scroll;">'
        ,'<table class="tabela">'
        ,'<tr>';

    if (is_array($erros) && count($erros)) {
    	$chaves = array_keys($erros);
    	if($chaves[0] == "subacoes" && array_key_exists("acao", $erros)){
				$erros = array_reverse($erros);
			}
        echo '<td colspan="2" style="text-align:center;font-size:14px;font-weight:bold;color:#900">O sistema verificou que alguns dados do plano de metas não foram preenchidos:<br/><span style="font-weight:normal;"></span></td>'
            ,'</tr>';
		foreach ($erros as $indice =>$dados) {
			if($indice == "acao"){
				 echo '<tr><td colspan="2" style="text-align:center;font-size:14px;font-weight:bold;color:#900">Ações com pendências de preenchidos:<br/><span style="font-weight:normal;">' , sizeof($erros['acao']) , ' pendencias encontradas</span></td>'
            ,'</tr>';
				foreach ($dados as $ind =>$acoes ) {
					echo '<tr style="background-color: #d9d9d9;">'
					                    ,'<td><img src="/imagens/consultar.gif" onclick="alterarAcao(' , $ind , ')"></td>'
					                    ,'<td><strong style="padding-left:5px;font-size:13px;cursor:pointer;" onclick="alterarAcao(' , $ind , ')">' ," - ".$acoes["descAcao"] , '</strong></td>'
					                    ,'</tr>';  
   	
					foreach ($acoes as $indi =>$dadosAcoes ) {
						if($indi !== "descAcao" ){
							 echo  '<tr>'
					               ,'<td colspan="2"><ul style="margin-bottom:0px; margin-top:0px;"><li style="font-size:12px; margin-bottom:0px; margin-top:0px;">' , $dadosAcoes , '</li></ul></td>'
					                ,'</tr>' ; 
						}
					}
				}	
			}else if($indice == "subacoes"){
				echo '<tr> 
					<td colspan="2" style="text-align:center;font-size:14px;font-weight:bold;color:#900">
					<div style="border: 1px solid #000000; margin-top:5px; margin-bottom:5px;"></div>
					Subações com pendências de preenchidos:<br/><span style="font-weight:normal;">' , sizeof($erros['subacoes']) , ' pendencias encontradas</span></td>'
            			,'</tr>';
				foreach($dados as $sub){
					foreach ($sub as $indice => $subacoes ) {
			           
				        	if( $indice === 0 ){
				        		echo '<tr>
				        				<td style=" padding: 10px; font-weight: bold; text-align: center; padding-left:5px;font-size:13px;"><span style="background-color: #d9d9d9; width: 100%; display: block; padding: 5px;">'. $subacoes["posicao"]." - ".$subacoes["sbadsc"] .'</span></td>
				        			  </tr>';
				        		return true;
				        	}
			
							$msgErro = "";
			            	foreach ($subacoes as $indiceArray => $dscErro){
			            		
			            		if ($indiceArray === 'sbadsc') continue;
			            		if ($indiceArray === 'posicao') continue;
			                        
			                  	$msgErro .= '<li style="font-size:12px; margin-bottom:0px; margin-top:0px;">' . $dscErro . '</li>';
			                }
			
			                echo '<tr style="background-color: #d9d9d9;">'
			                    ,'<td><img src="/imagens/consultar.gif" onclick="alterarSubacao(' , $indice , ')"></td>'
			                    ,'<td><strong style="padding-left:5px;font-size:13px;cursor:pointer;" onclick="alterarSubacao(' , $indice , ')">' , $subacoes["posicao"]." - ".$subacoes['sbadsc'] , '</strong></td>'
			                    ,'</tr>'
			                    ,'<tr>'
			                    ,'<td colspan="2"><ul>' , $msgErro , '</ul></td>'
			                    ,'</tr>';
		            	
	            	}
				}
			}
		}
		

        echo '</table></body></html>';

        return true;
    } else {
        echo '<td></td></tr></table></body></html>';

        return false;
    }
}
/*
 * Carrega erros encontrados durante a tramitação do documento
 * Atualizada por : Thiago Tasca Barbosa Desc: inserção de verificação de Ações Data: 24/11/2008
 */
function verifica_erros_preenchimento($inuid) {
	
    if (!$inuid)
        return array();

    global $db;

    $boPossuiAcao = false;
    $boPossuiSubacao = false;
    $pontuacao = $db->carregar('select ptoid from cte.pontuacao where ptostatus = \'A\' and inuid = ' . $inuid);
    $errossub  = array();

    if (!$pontuacao)
        return $errossub;    
        
    foreach ($pontuacao as $pontuacao1) {
        $idpontuacao = $pontuacao1['ptoid'];
        $acao        = $db->carregar('select aciid, 
        									 acirpns, 
        									 acidsc, 
        									 acidtinicial, 
        									 acidtfinal, 
        									 acicrg, 
        									 acirstd 
        							   from cte.acaoindicador where ptoid = ' . $idpontuacao);
        
        if (!is_array($acao))
            continue;
		
		$boPossuiAcao = true;
        foreach ($acao as $acao1) {
            $idacao  				= $acao1['aciid'];
            $descricaoAcao 			= $acao1['acidsc'];
            $responsavelPelaAcao 	= $acao1['acirpns'];
            $cargoResponsavel 		= $acao1['acicrg'];
            $periodoInicial 		= $acao1['acidtinicial'];
            $periodoFinal 			= $acao1['acidtfinal'];
            $resultadoEsperado 		= $acao1['acirstd'];
            
            if($responsavelPelaAcao == NULL){
            	$errossub['acao'][$idacao]['descAcao'] = $descricaoAcao;
            	$errossub['acao'][$idacao][] = "Não existe responsável cadastrado na Ação";
            }
            
        	if($cargoResponsavel == NULL){
            	$errossub['acao'][$idacao]['descAcao'] = $descricaoAcao;
            	$errossub['acao'][$idacao][] = "O cargo do responsável não está preenchido";
            }
        	if($periodoInicial == NULL){
            	$errossub['acao'][$idacao]['descAcao'] = $descricaoAcao;
            	$errossub['acao'][$idacao][] = "O periodo inicial não está preenchido";
            }
        	if($periodoFinal == NULL){
            	$errossub['acao'][$idacao]['descAcao'] = $descricaoAcao;
            	$errossub['acao'][$idacao][] = "O periodo final não está preenchido";
            }
        	if($resultadoEsperado == NULL){
            	$errossub['acao'][$idacao]['descAcao'] = $descricaoAcao;
            	$errossub['acao'][$idacao][] = "O campo resultado esperado não foi preenchido";
            }
           
            $subacao = $db->carregar('  select subin.*, props.ppsindcobuni
							            from cte.subacaoindicador as subin
											left join cte.proposicaosubacao as props on subin.ppsid = props.ppsid 
							            where coalesce(sbasituacaoarvore,1) = 1 AND
							            	 aciid = ' . $idacao);
            if (!is_array($subacao))
                continue;
				
			$boPossuiSubacao = true;
			$docid = cte_pegarDocid( $inuid );
			$estadoDocumento = wf_pegarEstadoAtual($docid);	
			if( $estadoDocumento["esdid"] == CTE_ESTADO_PAR ){
	            foreach ( $subacao as $subacao1 ){
	                if ($erros = cte_validarSubAcao($subacao1)) {
	                	$errossub['subacoes'][] = $erros;
	                }
	            }
            }
        }    
    }
    if( !$boPossuiAcao ){
    	$errossub['subacoes'][0][0]['sbadsc'] = "Não há Ação cadastrada";
    }
    elseif( !$boPossuiSubacao ){
    	$errossub['subacoes'][0][0]['sbadsc'] = "Não há Sub-ação cadastrada";
    }
    //dbg($errossub,1);
    return (is_array($errossub) && !count($errossub)) ? true : $errossub;
}


function cte_pegarSQLTodasEscolas( $itrid, $estuf, $muncod, $boEntidades = true ){

	if( $boEntidades ){
		$stClausulaSelect = ' t.entid ';
	}
	else{
		$stClausulaSelect = '\'<input type="checkbox" name="entid[]" id="entid_\' || t.entid || \'" value="\' || t.entid || \'" />\' as checkbox,
				    		 m.mundescricao,
				    		 \'<label for="entid_\' || t.entid || \'" style="cursor:pointer">\' || t.entcodent || \'</label>\' as entcodent,
				    		 entnome
				    		 ,\'<input onmouseout="MouseOut(this);" onmouseover="MouseOver(this);" class="normal" style="width:10ex;" size="10" onblur="MouseBlur(this)" type="text" name="qfaqtd[\' || t.entid || \']" id="qfaqtd[\' || t.entid || \']" />\' as qfaqtd';
	}

	if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL )
	{
		
	    $sqlComplemento = 'select distinct '.$stClausulaSelect.'					
	            from entidade.entidade t
		            inner join entidade.funcaoentidade f on f.entid = t.entid
		            left join entidade.entidadedetalhe ed on t.entid = ed.entid
		            inner join entidade.endereco d on t.entid = d.entid
		            left join territorios.municipio m on m.muncod = d.muncod
	            where (t.entescolanova = false or t.entescolanova is null)
	            and f.funid = 3 and
	            t.tpcid = 1 and
	            m.estuf = \'' . $estuf . '\'
	            group by t.entid, entnome, t.entcodent, m.mundescricao ';
	    $sqlComplemento .= $boEntidades ? "" : 'order by m.mundescricao, entnome';
	} else {
	    $sqlComplemento = 'select distinct '.$stClausulaSelect.'
	            from entidade.entidade t
		            left join entidade.entidadedetalhe entd on t.entcodent = entd.entcodent
		                and (
		                     entdreg_infantil_creche = \'1\' or
		                     entdreg_infantil_preescola = \'1\' or
		                     entdreg_fund_8_anos        = \'1\' or
		                     entdreg_fund_9_anos        = \'1\'
						)
		            inner join entidade.endereco ende on t.entid = ende.entid
					left join territorios.municipio m on m.muncod = ende.muncod	                
	            where (t.entescolanova = false or t.entescolanova is null)
	            and ende.muncod = \'' . $muncod . '\'
	            and t.tpcid = 3
                and
                t.entstatus = \'A\' ';
	    
	    $sqlComplemento .= $boEntidades ? "" : 'order by m.mundescricao, entnome';
	}
	//if( !$boEntidades ) dbg($sqlComplemento, 1);
	
	return $sqlComplemento;	
}

function cte_recuperArArAno($ano = NULL){
	if($ano != NULL ){//Se existir o ano e o ano do aditivo
		return array( $ano ); 
	}else{
		return array( 2007, 2008, 2009, 2010, 2011 ); 
	}
	
}

function cte_possuiSubacaoPorEscola( $sbaid ){
	
	global $db;
	
	$sql = "select sbaporescola from cte.subacaoindicador s
			where s.sbaid = $sbaid";
			
	return $db->pegaUm( $sql ) == 't' ? true : false;
}

function cte_recuperarAnosSubacaoConveniada( $sbaid ){
	
	global $db;
	//$sql="select sbcano from cte.subacaoconvenio where sbaid = ".trim( $sbaid );
	$sql="select pssano from cte.projetosapesubacao where sbaid = ".trim( $sbaid );
	
	$conveniada = $db->carregarColuna($sql);
	
	sort( $conveniada );
	return implode( ", ", $conveniada );
	
}

function cte_possuiAditivo( $sbaid ){
	
	global $db;
	$sql="select count(*) from cte.subacaoindicador s where sbaidpai = ".trim( $sbaid );
	
	return $db->pegaUm( $sql );
				
}

function cte_subacaoProgramaAnalisada( $sbaid ){
	
	global $db;
	$arDados = array();
	
	$sql = "select count( * ) 
		from cte.subacaoparecertecnico 
		where sptano in (2010, 2011)
		and ( 
			coalesce( ssuid, 0 ) != 0 and
			coalesce( sptparecer, '' ) != ''
		)
		and sbaid = ". $sbaid;
	
	return  $db->pegaUm( $sql );

}

function cte_subacaoProgramaPreenchida( $sbaid ){
	
	global $db;
	$arDados = array();
	
	$sql = "select count( * ) 
		from cte.subacaoparecertecnico 
		where sptano in (2010, 2011)
		and ( 
			coalesce( sptinicio, 0 ) != 0 and
			coalesce( sptfim, 0 ) != 0
		)
		and sbaid = ". $sbaid;
	
	return  $db->pegaUm( $sql );

}

/**
 * function cte_validarSubAcaoFaseAnalise
 * Função que verifica se uma subacao está validada ou não 
 * @param array $subacao - Array com os valores de uma subação
 * @return bool - Retorna true ou false em caso a subação esteja válida ou não, respectivamente   
 * @author Orion Teles de Mesquita
 * @since 21/11/2008
 */
function cte_validarSubAcaoFaseAnalise( $subacao ){
	
	global $db;
	$arDados = array();
	
	$sql = "select sptano, sptfim, sptinicio, sptunt, sptuntdsc 
			from cte.subacaoparecertecnico 
			where coalesce( sptunt, 99 ) != 0 
			and sbaid = ". $subacao["sbaid"];
	
	$resParecerTecnico = $db->carregar( $sql );
	
	$sql = "select distinct sabano 
			from cte.subacaobeneficiario
			where coalesce( sabano, 0 ) != 0
			and sbaid = ". $subacao["sbaid"];
	
	$resBeneficiario = $db->carregar( $sql );
	
	$sql = "select distinct cosano 
			from cte.composicaosubacao
			where coalesce( cosano, 0 ) != 0
			and sbaid = ". $subacao["sbaid"];
	
	$resComposicao = $db->carregar( $sql );
	
	$sql = "select distinct qfaano 
			from cte.qtdfisicoano
			where coalesce( qfaano, 0 ) != 0
			and sbaid = ". $subacao["sbaid"];
	
	$resEscola = $db->carregar( $sql );
	
	$sql = "select sptano, sptparecer, ssuid 
			from cte.subacaoparecertecnico
			where coalesce(ssuid, 0) <> 0
			and coalesce(sptparecer, '') <> '' 
			and sbaid = ". $subacao["sbaid"];
			
	$resParecerLancado = $db->carregar( $sql );
	
	// Recupera os dados do parecer de uma subação e monta um array de dados	
	if( is_array( $resParecerTecnico ) ){
		
		foreach( $resParecerTecnico as $arParecer ){
			$arDados[$arParecer["sptano"]]["sptinicio"] = $arParecer["sptinicio"];
			$arDados[$arParecer["sptano"]]["sptfim"] = $arParecer["sptfim"];
			$arDados[$arParecer["sptano"]]["sptuntdsc"] = $arParecer["sptuntdsc"];
			
			// Só verificará o campo sptano se a subação não tiver cronograma por escola
			if( $subacao["sbaporescola"] == 'f' )
				$arDados[$arParecer["sptano"]]["sptunt"] = $arParecer["sptunt"];
				
		}
	}
	if( is_array( $resComposicao ) ){
		
		foreach( $resComposicao as $arComposicao ){
			$arDados[$arComposicao["cosano"]]["boItensComposicao"] = true;
		}
	}
	if( is_array( $resBeneficiario ) ){
		
		foreach( $resBeneficiario as $arBeneficiario ){
			$arDados[$arBeneficiario["sabano"]]["boBeneficiario"] = true;
		}
	}
	if( is_array( $resEscola ) ){
		
		foreach( $resEscola as $arEscola ){
			$arDados[$arEscola["qfaano"]]["boEscola"] = true;
		}
	}
	if( is_array( $resParecerLancado ) ){
		
		foreach( $resParecerLancado as $arParecerLancado ){
			$arDados[$arParecerLancado["sptano"]]["sptparecer"] = $arParecerLancado["sptparecer"];
			$arDados[$arParecerLancado["sptano"]]["ssuid"] = $arParecerLancado["ssuid"];
		}
	}

	$arErro = false;
	
	if( ( !is_array( $resParecerTecnico ) && !is_array( $resComposicao ) && !is_array( $resBeneficiario ) && !is_array( $resEscola ) ) 
		  && cte_possuiFormaExecucaoFinanceira( $subacao["frmid"] ) 
 	  )
	{
		if( !is_array( $resParecerLancado ) )
			return true;
	}
	else{
		if( cte_possuiFormaExecucaoFinanceira( $subacao["frmid"] ) ) 
			$ano = date("Y");
		else 
			$ano = null;
		
		$arErro = cte_validarSubAcao( $subacao, $ano );
	}
/*
	dbg( $arDados );
	dbg( $arErro );
*/
	// Verificando se a subação é de cobertura universal do MEC
	$ppsindcobuni = $db->pegaUm('SELECT ppsindcobuni
								 FROM cte.subacaoindicador sba
								 INNER JOIN cte.proposicaosubacao pps ON sba.ppsid = pps.ppsid
								 WHERE sba.sbaid = ' . $subacao["sbaid"] );	
	
	if( ( !count( $arDados ) || $arErro ) && $ppsindcobuni == 'f' ){
		return false;
	}
	
	// Se for assistência financeira, o array de dados terá apenas o ano corrente
	if( cte_possuiFormaExecucaoFinanceira( $subacao["frmid"] ) ){
		// Se não tiver registros para o ano corrente, não é pendência para uma subação com assistência financeira
		if( !isset( $arDados[date('Y')] ) ){
			return true;
		}
		else{
			$arAnoCorrente[date('Y')] = $arDados[date('Y')];
			$arDados = $arAnoCorrente;
		}
	}	
	
	if( isset( $arDados["2007"] ) ) unset( $arDados["2007"] );	
	
	$boAnalisado = false;
	// Varre todos os anos que possuem registros em uma subação e a valida
	foreach( $arDados as $nrAno => $arDadosAno ){
		
		$boExisteDados = cte_possuiDadosLancados( $arDadosAno );
		$boExisteParecer = cte_possuiParecerLancado( $arDadosAno );
/*		
		dbg( $nrAno );
		dbg( $boExisteDados );
		dbg( $boExisteParecer );
*/

		// Se tiver dados preenchidos e existir parecer esta subação, neste ano está válida
		if( $boExisteDados && $boExisteParecer ){
			$boAnalisado = true;
		}
		elseif( !$boExisteDados && !$boExisteParecer ){
			if( cte_possuiFormaExecucaoFinanceira( $subacao["frmid"] ) ){
				return true;
			}
			continue;
		}		
		// Se não houver dados e tiver parecer dado, estará válido somente se status for já contemplada ou não atendida.
		elseif( !$boExisteDados && $boExisteParecer ){
			if( $arDadosAno["ssuid"] == STATUS_SUBACAO_JA_CONTEMPLADA || $arDadosAno["ssuid"] == STATUS_SUBACAO_NAO_ATENDIDA )
				$boAnalisado = true; 
			else
				if( $ppsindcobuni == 't' ){
					$boAnalisado = true; 
				}
				else
					return false;
		}
		// Se não houver dados porém o campo de detalhamento estiver preenchido há pendência no caso de Assistência Financeira.
		elseif( cte_possuiFormaExecucaoFinanceira( $subacao["frmid"] ) && ( !$boExisteDados && $arDadosAno["sptuntdsc"] ) ){
			return false;
		}
		// Se não tiver dados nem parecer para o ano corrente, não é pendência para uma subação com assistência financeira 
		elseif( cte_possuiFormaExecucaoFinanceira( $subacao["frmid"] ) && ( !$boExisteDados && !$boExisteParecer ) ){
			$boAnalisado = true;
		}
		// Para todos os outros casos, pendência
		else{
			return false;
		}
	}
	
	return $boAnalisado;
}

/**
 * function cte_possuiDadosLancados
 * Função que verifica se possui algum dado (quantidade e cronograma físico) lançado na subação 
 * @param array $arDadosAno - Array de dados do ano de uma subação
 * @return bool - Retorna true ou false em caso de haver dado preenchido ou não, respectivamente   
 * @author Orion Teles de Mesquita
 * @since 21/11/2008
 */
function cte_possuiDadosLancados( $arDadosAno ){
	
	$boDadoLancado = false;
	
	// Se existir o campo sptunt devolve true ou false, dependendo se o campo estiver preenchido
	if( isset( $arDadosAno["sptunt"] ) ){
		$boDadoLancado = (boolean) $arDadosAno["sptunt"];
	}
	
	// Se existir o campo sptinicio devolve true ou false, dependendo se o campo estiver preenchido
	if( isset( $arDadosAno["sptinicio"] ) ){

		// Se $boDadoLancado já foi preenchido com true anteriormente, mantém seu valor
		$boDadoLancado = $boDadoLancado ? "true" : (boolean) $arDadosAno["sptinicio"];
	}	
	
	// Se existir o campo sptfim devolve true ou false, dependendo se o campo estiver preenchido
	if( isset( $arDadosAno["sptfim"] ) ){
		
		// Se $boDadoLancado já foi preenchido com true anteriormente, mantém seu valor
		$boDadoLancado = $boDadoLancado ? "true" : (boolean) $arDadosAno["sptfim"];
	}
	
	return $boDadoLancado;
}

/**
 * function cte_possuiParecerLancado
 * Função que verifica se possui parecer (parecer e status) lançado na subação 
 * @param array $arDadosAno - Array de dados do ano de uma subação
 * @return bool - Retorna true ou false em caso de haver parecer dado ou não, respectivamente   
 * @author Orion Teles de Mesquita
 * @since 21/11/2008
 */
function cte_possuiParecerLancado( $arDadosAno ){

	// Para retornar true, obrigatoriamente deve ter valor tanto para sptparecer quanto para ssuid
	// Caso não exista para um deles, já é retornado false.
	
	// Se existir o campo sptparecer retorna true ou false, dependendo se o campo estiver preenchido
	if( isset( $arDadosAno["sptparecer"] ) ){
		return (boolean) $arDadosAno["sptparecer"];
	}
	else{
		return false;
	}
	
	// Se existir o campo ssuid retorna true ou false, dependendo se o campo estiver preenchido
	if( isset( $arDadosAno["ssuid"] ) ){
		return (boolean) $arDadosAno["ssuid"];
	}
	
	return false;
}

/**
 * function cte_definirMonitoramento
 * Função que define as subações que serão monitoradas no PAR 
 * @param int $inuid - Identificador do Instrumento Unidade
 * @return bool - Retorna true ou false em caso de sucesso ou fracasso, respectivamente.   
 * @author Orion Teles de Mesquita
 * @since 23/03/2009
 */
function cte_definirMonitoramento( $inuid ){
	
	global $db;
	
	$sql = "insert into cte.monitoramentosubacao ( sbaid, mntstatus )
			select sba.sbaid, 'A'
			from cte.subacaoindicador sba
				inner join cte.proposicaosubacao pps on sba.ppsid = pps.ppsid
				inner join cte.acaoindicador ai on ai.aciid = sba.aciid
				inner join cte.pontuacao p on p.ptoid = ai.ptoid
				inner join cte.instrumentounidade iu on iu.inuid = p.inuid
				left join cte.monitoramentosubacao ms on ms.sbaid = sba.sbaid  
			where ppsmonitoramento = true
			and p.inuid = $inuid
			and itrid = 2
			and ms.sbaid is null
			-- and sba.sbaid not in ( select sbaid from cte.monitoramentosubacao )";
	
	$return = (boolean) $db->executar( $sql );
	
	if( $return ){
		$db->commit();
	}
	return $return;
	
}

function cte_copiarPlanoAcaoHistorico($inuid)
{
	global $db;

	$pontuacao = $db->carregar("select ptoid from cte.pontuacao where ptostatus = 'A' and inuid = $inuid ");

	$sql = "select ptosequencial+1 as sequencia from cte.pontuacaohistorico where inuid = ".$inuid." ORDER BY ptosequencial DESC LIMIT 1";
	$sequencial = $db->pegaUm($sql);
	if(!$sequencial){
		$sequencial = 1;
	}
	
	foreach ( $pontuacao as $pontuacao1 ) {
		$idpontuacao = $pontuacao1['ptoid'];
		$sql = '
        insert into cte.pontuacaohistorico (
            crtid,
            ptojustificativa,
            ptodemandamunicipal,
            ptodemandaestadual,
            ptodata,
            usucpf,
            inuid,
            indid,
            ptostatus,
            ptoparecertecnico,
            ptosequencial
        ) select
            crtid,
            ptojustificativa,
            ptodemandamunicipal,
            ptodemandaestadual,
            ptodata,
            usucpf,
            inuid,
            indid,
            \'A\',
            ptoparecertecnico,
            '.$sequencial.' as seguencial
          from
            cte.pontuacao c
          where
            c.ptoid = ' . $idpontuacao . ' returning ptoid';
		
		$novoidpontuacao = $db->pegaUm($sql);
		$acao            = $db->carregar('select aciid from cte.acaoindicador where ptoid = ' . $idpontuacao);

		if ( $acao != '' ) {
			foreach ( $acao as $acao1 ):
				$idacao = $acao1['aciid'];
				$sql1 = " insert into cte.acaoindicadorhistorico 
							(    ptoid,
								  parid,
								  acidsc,
								  acirpns,
								  acicrg,
								  acidtinicial,
								  acidtfinal,
								  acirstd,
								  acilocalizador,
								  acidata,
								  usucpf,
								  acisequencial,
								  aciidoriginal)
							select 
							  	  $novoidpontuacao,
								  parid,
								  acidsc,
								  acirpns,
								  acicrg,
								  acidtinicial,
								  acidtfinal,
								  acirstd,
								  acilocalizador,
								  acidata,
								  usucpf,
								  (select $sequencial) as sequencial,
								  $idacao
							from cte.acaoindicador
							where aciid = $idacao returning aciid; ";
				$novoidacao = $db->pegaUm($sql1);			
	
				$subacao = $db->carregar("select subin.*, props.ppsindcobuni from cte.subacaoindicador as subin 
										  left join cte.proposicaosubacao as props on subin.ppsid = props.ppsid 
										  where aciid =".$idacao);
				if ( $subacao != '' )
				{
					foreach ( $subacao as $subacao1 ):
						$idsubacao = $subacao1['sbaid'];
						$sql2 = " insert into cte.subacaoindicadorhistorico 
									(    aciid ,
										  undid,
										  frmid,
										  sbadsc,
										  sbastgmpl,
										  sbaprm,
										  sbapcr,
										  sbadata,
										  usucpf,
										  sbaporescola,
										 -- sbaparecer,
										  psuid,
										  ssuid,
										  foaid,
										  prgid,
										  ppsid,
										  sbaordem,
										  sbaobjetivo,
										  sbatexto,
										  sbacategoria,
										  sbasequencial,
										  sbaidoriginal
										)
									select 
										  $novoidacao ,
										  undid,
										  frmid,
										  sbadsc,
										  sbastgmpl,
										  sbaprm,
										  sbapcr,
										  sbadata,
										  usucpf,
										  sbaporescola,
										  --sbaparecer,
										  psuid,
										  ssuid,
										  foaid,
										  prgid,
										  ppsid,
										  sbaordem,
										  sbaobjetivo,
										  sbatexto,
										  sbacategoria,
										  (select $sequencial) as sequencial,
										  $idsubacao
									from cte.subacaoindicador
									where sbaid = $idsubacao returning sbaid; ";
									
							$novoidsubacao = $db->pegaUm($sql2);	
							$sqlspt = " insert into cte.subacaoparecertecnicohistorico
										(     	sbaid,
											--sptparecer,
											sptunt,
											sptuntdsc,
											sptano,
											sptinicio,
											sptfim,
											tppid,
											sptsequencial
											--ssuid
											)
										select 
											$novoidsubacao,
											--sptparecer,
											sptunt,
											sptuntdsc,
											sptano,
											sptinicio,
											sptfim,
											tppid,
											(select $sequencial) as sequencial
											--ssuid
										from cte.subacaoparecertecnico
										where sbaid = ".$idsubacao;	
							$db->carregar($sqlspt);
								$sqlComposicao="
									 insert into cte.composicaosubacaohistorico
									(   sbaid,
										cosdsc,
										unddid,
										cosano,
										cosqtd,
										cosvlruni,
										cossequencial
										)
									select 
										$novoidsubacao,
										cosdsc,
										unddid,
										cosano,
										cosqtd,
										cosvlruni,
										(select $sequencial) as sequencial
									from cte.composicaosubacao
									where sbaid = ".$idsubacao." returning cosid;";
							
							$novocosid = $db->pegaUm($sqlComposicao);
							//dbg($sqlComposicao,1);	
							$sqlQtd=" insert into cte.qtdfisicoanohistorico
								(   sbaid,
									qfaano,
									qfaqtd,
									entid,
									qfasequencial
									)
								select 
									$novoidsubacao,
									qfaano,
									qfaqtd,
									entid,
									(select $sequencial) as sequencial
								from cte.qtdfisicoano
								where sbaid = ".$idsubacao;	
								
							$db->carregar($sqlQtd);
							
							$sqlBeneficiario="insert into cte.subacaobeneficiariohistorico
												(	sbaid,
													benid,
													vlrurbano,
													vlrrural,
													sabano,
													sabsequencial
												)
												select
													$novoidsubacao,
													benid,
													vlrurbano,
													vlrrural,
													sabano,
													(select $sequencial) as sequencial
												from cte.subacaobeneficiario
												where sbaid = ".$idsubacao;
							$db->carregar($sqlBeneficiario);
							if($novocosid){
								$sqlQuantEscolas="insert into cte.escolacomposicaosubacaohistorico 
													(	ecsqtd, 
														qfaid, 
														cosid,
														ecsequencial
													)
													select
														ecs.ecsqtd,
														ecs.qfaid,
														$novocosid,
														(select $sequencial) as sequencial
													from cte.escolacomposicaosubacao ecs
													inner join cte.composicaosubacao cos on ecs.cosid = cos.cosid
													inner join cte.qtdfisicoano qfa on ecs.qfaid = qfa.qfaid
													inner join cte.subacaoindicador sba on cos.sbaid = sba.sbaid and qfa.sbaid = sba.sbaid
													where sba.sbaid = ".$novoidsubacao;
													//dbg($sqlQuantEscolas,1);
								$db->carregar($sqlQuantEscolas);
							}
								
					endforeach;
				}
			endforeach;
		}
	}
	if($db->commit()){
		return true;
	}else{
		return false;
	}
	//if( !trataSubacaoCoberturaUniversal( $inuid ) )
		//return false;
	
	return true;

}


function recuperarEscolasPorMuncod( $muncod ){
	global $db;
	
	$sql = "select ent.entid as codigo, ent.entnome as descricao, ent.entcodent as inep
            from entidade.entidade ent
				left join entidade.entidadedetalhe entd on ent.entcodent = entd.entcodent
					and(
						entdreg_infantil_creche = '1' or
						entdreg_infantil_preescola = '1' or
						entdreg_fund_8_anos        = '1' or
						entdreg_fund_9_anos        = '1'
					)
				inner join entidade.endereco ende on ent.entid = ende.entid
            where (ent.entescolanova = false or ent.entescolanova is null)
			and ende.muncod = '$muncod'
            and ent.tpcid = 3
			and ent.entstatus = 'A'
            order by ent.entnome";

	$resultado = $db->carregar( $sql );
	
	return $resultado ? $resultado : array();
}

function montarRelacionamentoEscolasExecucaoPorMuncod( $muncod ){
	global $db;
	$sql = "select 
				'<input type=\"checkbox\" onclick=\"adiciona_item( '|| ent.entid ||', \''|| ent.entnome ||'\', this )\" name=\"entid[]\" id=\"entid_'|| ent.entid ||'\" value=\"'|| ent.entid ||'\" />' as checkbox,
				ent.entnome as descricao,
				ent.entcodent as inep
            from entidade.entidade ent
				left join entidade.entidadedetalhe entd on ent.entcodent = entd.entcodent
					and(
						entdreg_infantil_creche = '1' or
						entdreg_infantil_preescola = '1' or
						entdreg_fund_8_anos        = '1' or
						entdreg_fund_9_anos        = '1'
					)
				inner join entidade.endereco ende on ent.entid = ende.entid
            where ende.muncod = '$muncod'
            and ent.tpcid = 3
			and ent.entstatus = 'A'
			--and ent.entescolanova = false
            order by ent.entnome";            

	$resultado = $db->carregar( $sql );
	
	return $resultado ? $resultado : array();
	
}


function montarRelacionamentoEscolasAtivasPorMuncod( $muncod, $buscaNome, $buscaCod ){
	global $db;
	$sql = "select 
				'<input type=\"checkbox\" onclick=\"window.opener.adiciona_item( '|| ent.entid ||', \''|| ent.entnome ||'\', this.checked )\" name=\"entid[]\" id=\"entid_'|| ent.entid ||'\" value=\"'|| ent.entid ||'\" />' as checkbox,
				ent.entcodent as codigo,
				ent.entnome as descricao
            from entidade.entidade ent
				left join entidade.entidadedetalhe entd on ent.entcodent = entd.entcodent
					and(
						entdreg_infantil_creche = '1' or
						entdreg_infantil_preescola = '1' or
						entdreg_fund_8_anos        = '1' or
						entdreg_fund_9_anos        = '1'
					)
				inner join entidade.endereco ende on ent.entid = ende.entid
            where ent.entescolanova = false
			and ende.muncod = '$muncod'
            and ent.tpcid = 3
			and ent.entstatus = 'A'";			
            
    if ($buscaNome)
    {
    	$sql .= "and entnome ilike '%'||removeacento('{$buscaNome}')||'%'";
    } 
    if ($buscaCod) 
    {
    	$sql .= "and ent.entcodent = '".trim($buscaCod)."'";
    }    
    $sql .= "order by ent.entnome";

	$resultado = $db->carregar( $sql );
	
	return $resultado ? $resultado : array();
	
}

function montarRelacionamentoEscolasAtivasPorEstuf( $estuf, $buscaNome, $buscaCod ){
	
	global $db;
	$sql = "select 
				'<input type=\"checkbox\" onclick=\"window.opener.adiciona_item( '|| ent.entid ||', \''|| ent.entnome ||'\', this.checked )\" name=\"entid[]\" id=\"entid_'|| ent.entid ||'\" value=\"'|| ent.entid ||'\" />' as checkbox,
				ent.entcodent as codigo,
				ent.entnome as descricao
            from entidade.entidade ent
            	inner join entidade.endereco d on ent.entid = d.entid
            	INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
            	left join territorios.municipio m on m.muncod = d.muncod
            where ent.entescolanova = false
			and fe.funid = 3 
			and ent.tpcid = 1 
			and m.estuf = '$estuf'";

	if ($buscaNome)
    {
    	$sql .= "and entnome ilike '%'||removeacento('{$buscaNome}')||'%'";
    }
    
    if ($buscaCod) 
    {
    	$sql .= "and ent.entcodent = '".trim($buscaCod)."'";
    } 
	   
    $sql .= "group by ent.entid, ent.entcodent, ent.entnome, m.mundescricao
             order by m.mundescricao, ent.entnome";            

	$resultado = $db->carregar( $sql );
	return $resultado ? $resultado : array();
	
}

function cte_recuperarArUnidadesExigemCPF(){
	
	$arUnidades = array( CTE_UNIDADE_MEDIDA_SERVIDOR_MUN, 
						 CTE_UNIDADE_MEDIDA_SERVIDOR_EST, 
						 CTE_UNIDADE_MEDIDA_PROFESSOR_MULTIPLICADOR_MUN,
						 CTE_UNIDADE_MEDIDA_PROFESSOR_MULTIPLICADOR_EST,
						 CTE_UNIDADE_MEDIDA_PROFESSOR_CURSISTA_MUN,
						 CTE_UNIDADE_MEDIDA_PROFESSOR_CURSISTA_EST,
						 CTE_UNIDADE_MEDIDA_PROFESSOR_TUTOR_MUN,
						 CTE_UNIDADE_MEDIDA_PROFESSOR_TUTOR_EST,
						 CTE_UNIDADE_MEDIDA_DIRETOR_MUN,
						 CTE_UNIDADE_MEDIDA_DIRETOR_EST,
						 CTE_UNIDADE_MEDIDA_CONSELHEIRO_MUN,
						 CTE_UNIDADE_MEDIDA_CONSELHEIRO_EST,
						 CTE_UNIDADE_MEDIDA_TECNICO_MUN,
						 CTE_UNIDADE_MEDIDA_TECNICO_EST,
						 CTE_UNIDADE_MEDIDA_FUNCIONARIO_MUN, 
						 CTE_UNIDADE_MEDIDA_FUNCIONARIO_EST,
						 CTE_UNIDADE_MEDIDA_SERVIDOR_SME );
						 
	return $arUnidades;						 
	
}

function verificarPreenchimentoEscolaAtiva( $esaid ){

	$obEscolaAtiva = new EscolaAtiva( $esaid );
	return $obEscolaAtiva->verificarPreenchimento();
}

function verificarAnaliseEscolaAtiva( $esaid ){

	return true;
}

function gerarSubacoesEscolaAtivaAnalisadas( $esaid ){

	$obEscolaAtiva = new EscolaAtiva( $esaid );
	$obEscolaAtiva->gerarSubacoesEscolaAtivaAnalisadas();
			
	$obEscolaAtiva->commit();
	return true;
}

function excluirDadosSubacoesEscolaAtivaAnalisadas( $esaid ){

	$obEscolaAtiva = new EscolaAtiva( $esaid );
	$obEscolaAtiva->excluirDadosSubacoesEscolaAtivaAnalisadas();
			
	$obEscolaAtiva->commit();
	return true;
}

function recuperarQuantidadeSubacoesVigentesPorPeriodo( $arPeriodo, $anoFim, $boExecutados = false, $inuid, $perTerminoRef = '' ){
	global $db;
	
	//$anoFim += 1;
	
	$nrAnoInicio = substr( $arPeriodo["perdtinicioref"], 0, 4 );
	$nrMesInicio = substr( $arPeriodo["perdtinicioref"], 4, 2 );	
	
	$nrAnoFim = substr( $arPeriodo["perdtterminoref"], 0, 4 );
	$nrMesFim = substr( $arPeriodo["perdtterminoref"], 4, 2 );
	
	$sqlUnion		= 	"						
						union all
						
						select distinct 
						--count( * )
						mnt.mntid
						from cte.instrumentounidade iu
							inner join cte.pontuacao p on p.inuid = iu.inuid
							inner join cte.acaoindicador a on a.ptoid = p.ptoid  
							inner join cte.subacaoindicador s on s.aciid = a.aciid 
							inner join cte.proposicaosubacao ps on ps.ppsid = s.ppsid
							inner join cte.subacaoparecertecnico spt on s.sbaid = spt.sbaid
							inner join cte.monitoramentosubacao as mnt on mnt.sbaid = s.sbaid
							INNER JOIN cte.execucaosubacao AS exe ON exe.mntid = mnt.mntid
							INNER JOIN cte.periodoreferencia per ON per.perid = exe.perid
						where iu.inuid = {$inuid}
						and p.ptostatus = 'A'
						and mnt.mntstatus = 'A'
						and s.frmid in ( 1, 2, 4, 5, 6, 7, 8, 9, 10 )
						and spt.ssuid in (3)
						and a.acilocalizador = 'M'
						and ppsindcobuni = false
						and ( exeatual = true and per.perdtterminoref < '{$perTerminoRef}' and exe.estid = '5' )
						and spt.sptano = {$nrAnoInicio}
						and (
								( sptfim >= $nrMesInicio and spt.sptano = $nrAnoInicio ) and
						        ( sptinicio <= $nrMesFim and spt.sptano = $nrAnoInicio ) or
						        (
						            sptfim <= $nrMesFim and
						            coalesce( sptanoterminocurso, 0 ) > $nrAnoFim and
						            coalesce( sptanoterminocurso, 0 ) < $anoFim
						        )
						)";
		
	$stInner1 		= $boExecutados ? "INNER JOIN cte.execucaosubacao AS exe ON exe.mntid = mnt.mntid" : "";
	$stInner2 		= $boExecutados ? "INNER JOIN cte.periodoreferencia per ON per.perid = exe.perid" : "";	
	$stWhere 		= $boExecutados ? "( exeatual = true and exe.perid = ".$arPeriodo["perid"]." and ( " : "";
	$stWhereFecha 	= $boExecutados ? " ) ) " : "";
	$sqlUnion		= $boExecutados ? $sqlUnion : "";
	
	$sql = "
			select mntid from (
				select distinct 
				--count( * )
				mnt.mntid
				from cte.instrumentounidade iu
					inner join cte.pontuacao p on p.inuid = iu.inuid
					inner join cte.acaoindicador a on a.ptoid = p.ptoid  
					inner join cte.subacaoindicador s on s.aciid = a.aciid 
					inner join cte.proposicaosubacao ps on ps.ppsid = s.ppsid
					inner join cte.subacaoparecertecnico spt on s.sbaid = spt.sbaid
					inner join cte.monitoramentosubacao as mnt on mnt.sbaid = s.sbaid
					$stInner1
					$stInner2
				where iu.inuid = {$inuid}
				and p.ptostatus = 'A'
				and mnt.mntstatus = 'A'
				and s.frmid in ( 1, 2, 4, 5, 6, 7, 8, 9, 10 )
				and spt.ssuid in (3)
				and a.acilocalizador = 'M'
				and ppsindcobuni = false						
				and (
						$stWhere								 					 
						        ( sptfim >= $nrMesInicio and spt.sptano = $nrAnoInicio ) and
						        ( sptinicio <= $nrMesFim and spt.sptano = $nrAnoInicio ) or
						        (
						            sptfim <= $nrMesFim and
						            coalesce( sptanoterminocurso, 0 ) > $nrAnoFim and
						            coalesce( sptanoterminocurso, 0 ) < $anoFim
						        )
						$stWhereFecha
		        	)
		        
		        $sqlUnion
	        ) as foo"; 
	        
//	ver($sql);      	
	$arDados = $db->carregar( $sql );
	$arDados = count( $arDados ) ? $arDados : array();
	
	return $arDados;	
}

function recuperarPorcentagemParPorInuid($inuid) {
	
	global $db;
	
	$cor 	= "#cococo";
	$cor1 	= "black";
	$porcentagemTotal = 100;
	
	$sql = "SELECT
				floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal)) AS porcentagem
			FROM territorios.municipio as m
			LEFT JOIN cte.instrumentounidade iu ON iu.mun_estuf = m.estuf 
				AND iu.muncod = m.muncod AND iu.itrid = 2
			
			LEFT JOIN (    
				SELECT distinct COUNT(ex.exeid) AS monitora, isu.inuid
				FROM cte.instrumentounidade isu
					INNER JOIN cte.pontuacao  p				ON p.inuid   = isu.inuid
					INNER JOIN cte.acaoindicador ac			ON ac.ptoid  = p.ptoid
					INNER JOIN cte.subacaoindicador sbi		ON sbi.aciid = ac.aciid
					INNER JOIN cte.monitoramentosubacao mnt ON mnt.sbaid = sbi.sbaid
					INNER JOIN cte.execucaosubacao ex		ON ex.mntid  = mnt.mntid
					GROUP BY isu.inuid
				) AS monitoramentopar ON monitoramentopar.inuid = iu.inuid	
				
			LEFT JOIN workflow.documento d			ON d.docid = iu.docid
			LEFT JOIN workflow.estadodocumento ed	ON ed.esdid = d.esdid
			LEFT JOIN cte.indicadorespreenchidos b	ON b.inuid = iu.inuid
			LEFT JOIN cte.indicadorestotais a		ON a.itrid = iu.itrid
			LEFT JOIN cte.subacoestotal st			ON st.inuid = iu.inuid
			LEFT JOIN cte.subacoesanalisadas sa		ON sa.inuid = iu.inuid
			WHERE iu.inuid = {$inuid}";
			
			$valor = $db->pegaUm( $sql );
			
			$barra = '	<center>							
							<div class="barra1">
								<div style="text-align:center; position: absolute; width: 50px; height: 10px; padding: 0 0 0 0;  margin-bottom: 0px; " >
									<div style=" color:'.$cor.'; font-size: 10px; max-height: 10px;  ">'.$valor.'<span style="color:'.$cor1.'" >%</span></div>
								</div>
							<img class="imgBarra" style="width: '.$valor.'%; height: 10px" src="../imagens/cor1.gif"/>
							</div>				
						</center>';	
			
			return $barra;
}

function recuperarPorcentagemMonitoramentoPorInuid($inuid, $retornoSemHtml = NULL) {
	
	global $db;	

	$cor 	= "#cococo";
	$cor1 	= "black";
	$porcentagemTotal = 100;			
	
	$sql = "select min( perdtinicioref ) as inicio, max( perdtterminoref ) as final from cte.periodoreferencia";
	$arVigencia = $db->carregar( $sql );
	$arVigencia = $arVigencia ? $arVigencia : array();
	$anoFim = substr( $arVigencia[0]["final"], 0, 4 );
	
	$sql = "select perid, perdtinicioref, perdtterminoref from cte.periodoreferencia";
	$coPeriodo = $db->carregar( $sql );
	$coPeriodo = $coPeriodo ? $coPeriodo : array();
	unset($periQtdCrono);
	unset($periTotalPorcento);
	
	if ($inuid)
	{
	
		$arQuantidade = array();
		foreach( $coPeriodo as $arPeriodo ){			
			
			$nrAnoInicio = substr( $arPeriodo["perdtinicioref"], 0, 4 );
			$nrMesInicio = substr( $arPeriodo["perdtinicioref"], 4, 2 );	
			
			$nrAnoFim = substr( $arPeriodo["perdtterminoref"], 0, 4 );
			$nrMesFim = substr( $arPeriodo["perdtterminoref"], 4, 2 );
			
			//$anoFim = $nrAnoFim+1;
						
			$sql = "SELECT perdtterminoref FROM cte.periodoreferencia WHERE perid = ".$arPeriodo["perid"];
			$perTerminoRef = $db->pegaUm($sql);			
			
			$sql = "select distinct 
						count(*) 
					from (
						select distinct 
						--count( * )
						mnt.mntid
						from cte.instrumentounidade iu
							inner join cte.pontuacao p on p.inuid = iu.inuid
							inner join cte.acaoindicador a on a.ptoid = p.ptoid  
							inner join cte.subacaoindicador s on s.aciid = a.aciid 
							inner join cte.proposicaosubacao ps on ps.ppsid = s.ppsid
							inner join cte.subacaoparecertecnico spt on s.sbaid = spt.sbaid
							inner join cte.monitoramentosubacao as mnt on mnt.sbaid = s.sbaid
							
							
						where iu.inuid = {$inuid}
						and p.ptostatus = 'A'
						and mnt.mntstatus = 'A'
						and s.frmid in ( 1, 2, 4, 5, 6, 7, 8, 9, 10 )
						and spt.ssuid in (3)
						and a.acilocalizador = 'M'
						and ppsindcobuni = false						
						and (
																 					 
								        ( sptfim >= $nrMesInicio and spt.sptano = $nrAnoInicio ) and
								        ( sptinicio <= $nrMesFim and spt.sptano = $nrAnoInicio ) or
								        (
								            sptfim <= $nrMesFim and
								            coalesce( sptanoterminocurso, 0 ) > $nrAnoFim and
								            coalesce( sptanoterminocurso, 0 ) < $anoFim
								        )								
				        	)		    
				        	    
	        		) as foo";			
	        
	        $nrQuantidadeCronograma = $db->pegaUm($sql);

	        $sql = "select distinct 
	        			count(*) 
	        		from (
						select distinct 
						--count( * )
						mnt.mntid
						from cte.instrumentounidade iu
							inner join cte.pontuacao p on p.inuid = iu.inuid
							inner join cte.acaoindicador a on a.ptoid = p.ptoid  
							inner join cte.subacaoindicador s on s.aciid = a.aciid 
							inner join cte.proposicaosubacao ps on ps.ppsid = s.ppsid
							inner join cte.subacaoparecertecnico spt on s.sbaid = spt.sbaid
							inner join cte.monitoramentosubacao as mnt on mnt.sbaid = s.sbaid
							INNER JOIN cte.execucaosubacao AS exe ON exe.mntid = mnt.mntid
							INNER JOIN cte.periodoreferencia per ON per.perid = exe.perid
						where iu.inuid = {$inuid}
						and p.ptostatus = 'A'
						and mnt.mntstatus = 'A'
						and s.frmid in ( 1, 2, 4, 5, 6, 7, 8, 9, 10 )
						and spt.ssuid in (3)
						and a.acilocalizador = 'M'
						and ppsindcobuni = false						
						and (
								( exeatual = true and exe.perid = {$arPeriodo["perid"]} and ( 								 					 
								        ( sptfim >= $nrMesInicio and spt.sptano = $nrAnoInicio ) and
								        ( sptinicio <= $nrMesFim and spt.sptano = $nrAnoInicio ) or
								        (
								            sptfim <= $nrMesFim and
								            coalesce( sptanoterminocurso, 0 ) > $nrAnoFim and
								            coalesce( sptanoterminocurso, 0 ) < $anoFim
								        )
								 ) ) 
				        	)
				        
				        						
								union all
								
								select distinct 
								--count( * )
								mnt.mntid
								from cte.instrumentounidade iu
									inner join cte.pontuacao p on p.inuid = iu.inuid
									inner join cte.acaoindicador a on a.ptoid = p.ptoid  
									inner join cte.subacaoindicador s on s.aciid = a.aciid 
									inner join cte.proposicaosubacao ps on ps.ppsid = s.ppsid
									inner join cte.subacaoparecertecnico spt on s.sbaid = spt.sbaid
									inner join cte.monitoramentosubacao as mnt on mnt.sbaid = s.sbaid
									INNER JOIN cte.execucaosubacao AS exe ON exe.mntid = mnt.mntid
									INNER JOIN cte.periodoreferencia per ON per.perid = exe.perid
								where iu.inuid = {$inuid}
								and p.ptostatus = 'A'
								and mnt.mntstatus = 'A'
								and s.frmid in ( 1, 2, 4, 5, 6, 7, 8, 9, 10 )
								and spt.ssuid in (3)
								and a.acilocalizador = 'M'
								and ppsindcobuni = false
								and ( exeatual = true and per.perdtterminoref < '{$perTerminoRef}' and exe.estid = '5' )
								and spt.sptano = {$nrAnoInicio}
								and (
										( sptfim >= $nrMesInicio and spt.sptano = $nrAnoInicio ) and
								        ( sptinicio <= $nrMesFim and spt.sptano = $nrAnoInicio ) or
								        (
								            sptfim <= $nrMesFim and
								            coalesce( sptanoterminocurso, 0 ) > $nrAnoFim and
								            coalesce( sptanoterminocurso, 0 ) < $anoFim
								        )
								)
			        ) as foo";

			$nrQuantidadeExecutado = $db->pegaUm($sql);	        		
			//ver($nrQuantidadeExecutado, $nrQuantidadeCronograma);				
			$arQuantidade[$arPeriodo["perid"]]["executado"]   = $nrQuantidadeExecutado;
			$arQuantidade[$arPeriodo["perid"]]["cronograma"]  = $nrQuantidadeCronograma;
	
			if ($nrQuantidadeCronograma > 0)
			{				
				if($nrQuantidadeExecutado > $nrQuantidadeCronograma)
				{
					$nrQuantidadeExecutado = $nrQuantidadeCronograma;
				}			
				$arQuantidade[$arPeriodo["perid"]]["porcentagem"] = round( ( $nrQuantidadeExecutado/$nrQuantidadeCronograma )*100, 2 );
			} else {
				$arQuantidade[$arPeriodo["perid"]]["porcentagem"] = 0;
			}
			
			$peri[$arPeriodo["perid"]] = $nrQuantidadeCronograma > 0 ? 1 : 0;				
			$periQtdCrono += $peri[$arPeriodo["perid"]];
			$periTotalPorcento += $arQuantidade[$arPeriodo["perid"]]['porcentagem'];			
	
		}
		
		if($periQtdCrono > 0){
			$mediaIndice = $periTotalPorcento/$periQtdCrono;
		}
					
		$mediaIndice = round($mediaIndice, 2);		
		
		$barra = '	<center>							
			<div class="barra1">
				<div style="text-align:center; position: absolute; width: 50px; height: 10px; padding: 0 0 0 0;  margin-bottom: 0px; " >
					<div style=" color:'.$cor.'; font-size: 10px; max-height: 10px;  ">'.$mediaIndice.'<span style="color:'.$cor1.'" >%</span></div>
				</div>
			<img class="imgBarra" style="width: '.$mediaIndice.'%; height: 10px" src="../imagens/cor1.gif"/>
			</div>				
			</center>';			
						
		unset($arrListas[$i]['inuid']);
		unset($arQuantidade);
	}
	if($retornoSemHtml == true){
		return $mediaIndice;
	}else{
		return $barra;
	}
	
	
}

function verificaPerfisAdiminMonitoramentoInterno() {

	$perfis = cte_arrayPerfil();
	
	if( in_array( CTE_PERFIL_ADMINISTRADOR, $perfis ) || 
		in_array( CTE_PERFIL_SUPER_USUARIO, $perfis )  ) {
			
		return true;
			
	} else {
		
		return false;
	}
}

function parPegarEstadoAtual( $docid ) {
       global $db; 
       
       if($docid) {
               $docid = (integer) $docid;
                
               $sql = "
                       select
                               ed.esdid
                       from 
                               workflow.documento d
                       inner join 
                               workflow.estadodocumento ed on ed.esdid = d.esdid
                       where
                               d.docid = " . $docid;
               $estado = $db->pegaUm( $sql );
                
               return $estado;
       } else {
               return false;
       }
}

function pegaQrpid( $inuid, $queid ){
    global $db;
   
    $sql = "SELECT
                    c.qrpid
            FROM
                    cte.ctequestionario c
            INNER JOIN
                    questionario.questionarioresposta q ON q.qrpid = c.qrpid
            WHERE
                    c.inuid = {$inuid} AND
                    q.queid = {$queid}";
    $qrpid = $db->pegaUm( $sql );
   
    if(!$qrpid){
        $sql = "SELECT
                    case when itrid IN (1, 3) then e.estdescricao
                    else m.mundescricao end as descricao
                FROM
                    cte.instrumentounidade i
                LEFT JOIN
                    territorios.municipio m ON m.muncod = i.muncod
                LEFT JOIN
                    territorios.estado e ON e.estuf = i.estuf
                WHERE
                    inuid = {$inuid}";
        $titulo = $db->pegaUm( $sql );
        $arParam = array ( "queid" => $queid, "titulo" => "PAR-Plano de Metas (".$titulo.")" );
        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
        $sql = "INSERT INTO cte.ctequestionario (inuid, qrpid) VALUES ({$inuid}, {$qrpid})";
        $db->executar( $sql );
        $db->commit();
    }
    return $qrpid;
}

function verificaDesist( $inuid ){
	global $db;
	
	$sql = "SELECT
				*
			FROM
				cte.fluxoescolar
			WHERE
				inuid = ".$inuid;
	$dadosDesist = $db->pegaLinha( $sql );
	
	return $dadosDesist;
}

function atualizaDesist( $inuid, $param ){
	global $db;
	
	$sql = "UPDATE cte.fluxoescolar SET flestatus = '{$param}' WHERE inuid = {$inuid} RETURNING flestatus";
	$sucesso = $db->executar( $sql );
	
	$db->commit();
	
	return $sucesso;
}

function exibeMapaRegionalizador($cxpid = null){
		
	?>
	<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" class="tabela">
		<tr>
			<td valign="top" align="center" width="450">
				<fieldset style="background: #ffffff; width: 450px;"  >
					<legend> Selecione o Estado para emitir o relatório </legend>
					
					<div style="width: 100%;" id="containerMapa" >
						<img src="/imagens/mapa_brasil.png" width="444" height="357" border="0" usemap="#mapaBrasil" />
						<map name="mapaBrasil" id="mapaBrasil">
							<!--<area shape="rect" coords="388,15,427,48"   style="cursor:pointer;" onclick="document.getElementById('buscakey').value='';document.getElementById('hidden_letra_inicial').value='';exibeRegionalizador('todas', '<?=$arrDados['cxpid']?>');" title="Brasil"/>-->
							<area shape="rect" coords="388,15,427,48"   style="cursor:pointer;" onclick="exibeRegionalizador('todas', '<?=$arrDados['cxpid']?>');" title="Brasil"/>
							<area shape="rect" coords="48,124,74,151"   style="cursor:pointer;" onclick="exibeRegionalizador('AC');" title="Acre"/>
							<area shape="rect" coords="364,147,432,161" style="cursor:pointer;" onclick="exibeRegionalizador('AL');" title="Alagoas"/>
							<area shape="rect" coords="202,27,233,56"   style="cursor:pointer;" onclick="exibeRegionalizador('AP');" title="Amapá"/>
							<area shape="rect" coords="89,76,133,107"   style="cursor:pointer;" onclick="exibeRegionalizador('AM');" title="Amazonas"/>
							<area shape="rect" coords="294,155,320,183" style="cursor:pointer;" onclick="exibeRegionalizador('BA');" title="Bahia"/>
							<area shape="rect" coords="311,86,341,114"  style="cursor:pointer;" onclick="exibeRegionalizador('CE');" title="Ceará"/>
							<area shape="rect" coords="244,171,281,197" style="cursor:pointer;" onclick="exibeRegionalizador('DF');" title="Distrito Federal"/>
							<area shape="rect" coords="331,215,369,242" style="cursor:pointer;" onclick="exibeRegionalizador('ES');" title="Espírito Santo"/>
							<area shape="rect" coords="217,187,243,218" style="cursor:pointer;" onclick="exibeRegionalizador('GO');" title="Goiás"/>
							<area shape="rect" coords="154,155,210,186" style="cursor:pointer;" onclick="exibeRegionalizador('MT');" title="Mato Grosso"/>
							<area shape="rect" coords="156,219,202,246" style="cursor:pointer;" onclick="exibeRegionalizador('MS');" title="Mato Grosso do Sul"/>
							<area shape="rect" coords="248,80,301,111"  style="cursor:pointer;" onclick="exibeRegionalizador('MA');" title="Maranhão"/>
							<area shape="rect" coords="264,206,295,235" style="cursor:pointer;" onclick="exibeRegionalizador('MG');" title="Minas Gerais"/>
							<area shape="rect" coords="188,84,217,112"  style="cursor:pointer;" onclick="exibeRegionalizador('PA');" title="Pará"/>
							<area shape="rect" coords="368,112,433,130" style="cursor:pointer;" onclick="exibeRegionalizador('PB');" title="Paraíba"/>
							<area shape="rect" coords="201,262,231,289" style="cursor:pointer;" onclick="exibeRegionalizador('PR');" title="Paraná"/>
							<area shape="rect" coords="369,131,454,147" style="cursor:pointer;" onclick="exibeRegionalizador('PE');" title="Pernambuco"/>
							<area shape="rect" coords="285,116,313,146" style="cursor:pointer;" onclick="exibeRegionalizador('PI');" title="Piauí"/>
							<area shape="rect" coords="349,83,383,108"  style="cursor:pointer;" onclick="exibeRegionalizador('RN');" title="Rio Grande do Norte"/>
							<area shape="rect" coords="189,310,224,337" style="cursor:pointer;" onclick="exibeRegionalizador('RS');" title="Rio Grande do Sul"/>
							<area shape="rect" coords="302,250,334,281" style="cursor:pointer;" onclick="exibeRegionalizador('RJ');" title="Rio de Janeiro"/>
							<area shape="rect" coords="98,139,141,169"  style="cursor:pointer;" onclick="exibeRegionalizador('RO');" title="Rondônia"/>
							<area shape="rect" coords="112,24,147,56"   style="cursor:pointer;" onclick="exibeRegionalizador('RR');" title="Roraima"/>
							<area shape="rect" coords="228,293,272,313" style="cursor:pointer;" onclick="exibeRegionalizador('SC');" title="Santa Catarina"/>
							<area shape="rect" coords="233,243,268,270" style="cursor:pointer;" onclick="exibeRegionalizador('SP');" title="São Paulo"/>
							<area shape="rect" coords="337,161,401,178" style="cursor:pointer;" onclick="exibeRegionalizador('SE');" title="Sergipe"/>
							<area shape="rect" coords="227,130,270,163" style="cursor:pointer;" onclick="exibeRegionalizador('TO');" title="Tocantins"/>
							<!--<area shape="rect" coords="17,264,85,282"   style="cursor:pointer;" onclick="exibeRegionalizador('Norte','<?=$arrDados['cxpid']?>');" title="Norte" />
							<area shape="rect" coords="16,281,94,296"   style="cursor:pointer;" onclick="exibeRegionalizador('Nordeste','<?=$arrDados['cxpid']?>');" title="Nordeste" />
							<area shape="rect" coords="15,296,112,312"  style="cursor:pointer;" onclick="exibeRegionalizador('Centro-Oeste','<?=$arrDados['cxpid']?>');" title="Centro-Oeste" />
							<area shape="rect" coords="14,312,100,329"  style="cursor:pointer;" onclick="exibeRegionalizador('Sudeste','<?=$arrDados['cxpid']?>');" title="Sudeste" />
							<area shape="rect" coords="13,329,68,344"   style="cursor:pointer;" onclick="exibeRegionalizador('Sul','<?=$arrDados['cxpid']?>')" title="Sul" />
						--></map>
					</div>
				</fieldset>
			</td>
		</tr>
	</table>
	<?
}
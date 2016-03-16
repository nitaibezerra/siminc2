<?php

require_once "_constantes.php";

// ----- PERMISSAO -------------------------------------------------------------

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

/**
	 * function cte_pegarUfsPermitidas do BRASIL PRO.
	 * @description : Funcao que verifica UFs permitidas para o(s) perfil(s).
	 * @param  : string $perfis (opcional) String com os perfis separados por virgula para o filtro dos perfis/Ufs. 
	 * 									   Ex: $perfis = CTE_PERFIL1.','.CTE_PERFIL2;
	 * @author : --
	 * @since  : --
	 * @tutorial :
	 * 	Existe 1 parametros a serem passados:
	 * 	Sendo o 1º: String com os perfis separados por virgula.
	 * 				Ex: $perfis = CTE_PERFIL1.','.CTE_PERFIL2;
	 * 
	 * 	A função carrega os dados minimos para que se possa fazer a busca dos dados no banco de dados.
	 *  E carregado: a conexão com o banco, 
	 *  			 os perfis, caso não exista procura somente pelos dados da sessão.
	 * @example: 	$perfis = CTE_PERFIL1.','.CTE_PERFIL2; 
	 * 				$Ufs = cte_pegarUfsPermitidas( $perfis );
	 */

function cte_pegarUfsPermitidas( $perfil = NULL )
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
		else if( $perfil != NULL ){
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
					pu.pflcod    in ({$perfil}) and
					p.sisid = " . CTE_SISTEMA . "
				group by
					e.estuf
			";
		}else{
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
			cte_possuiPerfil( CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_CONSULTORES ) ||
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
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_CONSULTORES );
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
			$perfis = array(CTE_PERFIL_EQUIPE_MUNICIPAL,
                            CTE_PERFIL_EQUIPE_LOCAL,
                            CTE_PERFIL_EQUIPE_LOCAL_APROVACAO,
                            CTE_PERFIL_EQUIPE_TECNICA,
                            CTE_PERFIL_CONSULTORES,
                            CTE_PERFIL_ALTA_GESTAO,
                            CTE_PERFIL_CONSULTA_GERAL,
                            CTE_PERFIL_SUPER_USUARIO,
                            CTE_PARECERISTA_FNDE,
                            CTE_PERFIL_ADMINISTRATOR_TEMP);

			break;
		case CTE_ESTADO_ANALISE:
		case CTE_ESTADO_FIANCEIRA:
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_CONSULTORES,CTE_PERFIL_ADMINISTRATOR_TEMP );
			break;
		case CTE_ESTADO_FINALIZADO:
			$perfis = array();
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
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_CONSULTORES );
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
	//$a = cte_possuiPerfil( CTE_PERFIL_SUPER_USUARIO );
	global $db;
	$a = $db->testa_superuser();
	$b = ( cte_possuiPerfil( CTE_PERFIL_EQUIPE_LOCAL ) && $documento['esdid'] == CTE_ESTADO_BRASIL_PRO );
	$c = ( cte_possuiPerfil( CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_CONSULTORES ) && $documento['esdid'] == CTE_ESTADO_ANALISE );
	$d = ( cte_possuiPerfil( CTE_PERFIL_EQUIPE_MUNICIPAL ) && $documento['esdid'] == CTE_ESTADO_BRASIL_PRO );
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
	switch ( $esdid )
	{
		case CTE_ESTADO_PAR:
			$perfis = array(
				CTE_PERFIL_EQUIPE_MUNICIPAL,
				CTE_PERFIL_EQUIPE_LOCAL,
				CTE_PERFIL_EQUIPE_LOCAL_APROVACAO,
				CTE_PERFIL_ADMINISTRATOR_TEMP
			);
			break;
		case CTE_ESTADO_ANALISE:
		case CTE_ESTADO_FIANCEIRA:
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_CONSULTORES, CTE_PERFIL_EQUIPE_LOCAL, CTE_PERFIL_EQUIPE_LOCAL_APROVACAO, CTE_PERFIL_ADMINISTRATOR_TEMP );
			break;
		case CTE_ESTADO_DIAGNOSTICO:
		case CTE_ESTADO_FINALIZADO:
		default:
			$perfis = array();
			break;
	}
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
	switch ( $esdid )
	{
		case CTE_ESTADO_DIAGNOSTICO:
			
		case CTE_ESTADO_BRASIL_PRO:
			$perfis = array(
				CTE_PERFIL_ALTA_GESTAO,
				CTE_PERFIL_CONSULTORES,
				CTE_PERFIL_EQUIPE_MUNICIPAL,
				CTE_PERFIL_EQUIPE_LOCAL,
				CTE_PERFIL_EQUIPE_LOCAL_APROVACAO,
				CTE_PERFIL_EQUIPE_TECNICA,
				CTE_PERFIL_ADMINISTRATOR_TEMP
				);
			break;
		case CTE_ESTADO_ANALISE:
		case CTE_ESTADO_FIANCEIRA:
		case CTE_ESTADO_FINALIZADO:
			$perfis = array(
				CTE_PERFIL_ALTA_GESTAO,
				CTE_PERFIL_CONSULTA_GERAL,
				CTE_PERFIL_CONSULTORES,
				CTE_PERFIL_EQUIPE_MUNICIPAL,
				CTE_PERFIL_EQUIPE_LOCAL,
				CTE_PERFIL_EQUIPE_LOCAL_APROVACAO,
				CTE_PERFIL_EQUIPE_TECNICA,
				CTE_PERFIL_ADMINISTRATOR_TEMP
			);
			break;
		default:
			$perfis = array();
			break;
	}
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
	if(cte_possuiPerfil( CTE_PERFIL_EQUIPE_TECNICA )){
		return
		$esdid == CTE_ESTADO_ANALISE ||
		$esdid == CTE_ESTADO_DIAGNOSTICO ||
		$esdid == CTE_ESTADO_PAR ||
		$esdid == CTE_ESTADO_FIANCEIRA ||
		$esdid == CTE_ESTADO_PARECER ||
		$esdid == CTE_ESTADO_FNDE ||
		$esdid == CTE_ESTADO_FINALIZADO;
	}else{ 
		return
		$esdid == CTE_ESTADO_ANALISE ||
		$esdid == CTE_ESTADO_FINALIZADO;
	}
}

function cte_podeVerRelatorioParAtual( $inuid )
{
	global $db;
	if ( $db->testa_superuser() )
	{
		return true;
	}
	$docid = cte_pegarDocid( $inuid );
	$documento = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $documento['esdid'];
	
	if(cte_possuiPerfil( CTE_PERFIL_EQUIPE_TECNICA )){
		return 
		$esdid == CTE_ESTADO_ANALISE ||
		$esdid == CTE_ESTADO_DIAGNOSTICO ||
		$esdid == CTE_ESTADO_PAR ||
		$esdid == CTE_ESTADO_FIANCEIRA ||
		$esdid == CTE_ESTADO_PARECER ||
		$esdid == CTE_ESTADO_FNDE ||
		$esdid == CTE_ESTADO_FINALIZADO;
	}else{ 
		return $esdid == CTE_ESTADO_FINALIZADO;
	}
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

function cte_possuiFormaExecucaoTecnica( $frmid ){
	return in_array( $frmid, array( 12, 13, 14, 15, 18, 19 ) );	
}

function cte_possuiFormaExecucaoFinanceira( $frmid ){
	return in_array( $frmid, array( 16, 17 ) );	
}

function cte_possuiPerfilMunicipio( $pflcods, $muncod )
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
		$db->testa_superuser() ||
		in_array( $muncod, cte_pegarMunicipiosPermitidos() );
}

function cte_possuiPermissaoUf( $estuf )
{
	global $db;
	return
		$db->testa_superuser() ||
		in_array( $estuf, cte_pegarUfsPermitidas() );
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
	$inuid  = (integer) $inuid;
    $return = true;

	$sql    = "
		select
			ptoid
		from cte.pontuacao
		where
			inuid = " . $inuid;
	$linhas = $db->carregar( $sql );
	$linhas = $linhas ? $linhas : array();

	foreach ( $linhas as $linha )
	{
		if (($return = cte_atualizarPlanoPequenoMunicipio( $linha['ptoid'] )) === false)
            break;
	}

    return $return;
}

function cte_atualizarPlanoPequenoMunicipio( $ptoid )
{
	global $db;
	$ptoid = (integer) $ptoid;
	$sql = "select inuid from cte.pontuacao where ptoid = " . $ptoid;
	$inuid = (integer) $db->pegaUm( $sql );;
	
	// verifica se é muncipal
	$itrid = cte_pegarItrid( $inuid );
	if ( $itrid != INSTRUMENTO_DIAGNOSTICO_MUNICIPAL )
	{
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
		from cte.pontuacao p
			inner join cte.criterio c on
				c.crtid = p.crtid
		where
			ptoid = " . $ptoid;
	$ctrpontuacao = (integer) $db->pegaUm( $sql );
	
	// captura nome da ação
	$sql = "select acidsc from cte.acaoindicador where ptoid = " . $ptoid;
	$acidsc = (string) $db->pegaUm( $sql );
	
	// captura descricao da ação para pontuação 1
	$dados = cte_pegarPropostaAcaoPequenosMunicipios( $ptoid, 1 );
	$ppadsc_1 = (string) $dados['ppadsc'];
	
	// captura descricao da ação para pontuação 2
	$dados = cte_pegarPropostaAcaoPequenosMunicipios( $ptoid, 2 );
	$ppadsc_2 = (string) $dados['ppadsc'];
	
	switch ( $ctrpontuacao )
	{
		case 1:
			if ( $acidsc != $ppadsc_1 && $ppadsc_1 ) {
            $return = cte_removerPlano( $ptoid ) &&
                      cte_adicionarPlanoMunicipioPequeno( $ptoid, 1 );
			}
			break;
		case 2:
			if ( $acidsc != $ppadsc_2 && $ppadsc_2 )
			{
            $return = cte_removerPlano( $ptoid ) &&
                      cte_adicionarPlanoMunicipioPequeno( $ptoid, 2 );
			}
			break;
		case 3:
		case 4:
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
		$sql = "SELECT cosid FROM cte.composicaosubacao WHERE sbaid = ".$sbaid;
		$arrCosid = $db->carregarColuna( $sql );
		
		$sbaid = (integer) $linhas["sbaid"];
		$sql = "
			delete from cte.subacaobeneficiario
			where
				sbaid = " . $sbaid . "
		";
		$db->executar( $sql );
		$sql = "delete from
					cte.monitoramentocomposicao
				where
					cosid IN (".implode(', ', $arrCosid).")";

		$db->executar( $sql );
		$sql = "
			delete from cte.composicaosubacao
			where
				sbaid = " . $sbaid . "
		";
		$db->executar( $sql );
		$sql = "
			delete from cte.qtdfisicoano
			where
				sbaid = " . $sbaid . "
		";
		$db->executar( $sql );
		$sql = "
			delete from cte.subacaoindicador
			where
				sbaid = " . $sbaid . "
		";
		$db->executar( $sql );
	}
	
	$sql_acao = "
		delete from cte.acaoindicador
		where
			ptoid = " . $ptoid . "
	";

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
			 )
			 values
			 (
				 " . $aciid . ",
				 '" . $sbadsc . "',
				 " . $undid . ",
				 '" . $sbatexto . "',
				 '" . $sbaobjetivo . "',
				 " . $prgid . ",
				 " . $sbaordem . ",
				 " . $ppsid . ",
				 '" . $sbastgmpl . "',
				 " . $frmid . "
			 )
		";
		$db->executar( $sqlSubAcao );
	}
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
		$sql = "
			select
				*
			from cte.proposicaosubacao
			where
				ppaid = " . $ppaid . "
		";
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
			unset( $resultado[$chave]['fis_5_original'] );
			unset( $resultado[$chave]['fis_0_copia'] );
			unset( $resultado[$chave]['fis_1_copia'] );
			unset( $resultado[$chave]['fis_2_copia'] );
			unset( $resultado[$chave]['fis_3_copia'] );
			unset( $resultado[$chave]['fis_4_copia'] );
			unset( $resultado[$chave]['fis_5_copia'] );
			unset( $resultado[$chave]['fin_1_original'] );
			unset( $resultado[$chave]['fin_0_original'] );
			unset( $resultado[$chave]['fin_2_original'] );
			unset( $resultado[$chave]['fin_3_original'] );
			unset( $resultado[$chave]['fin_4_original'] );
			unset( $resultado[$chave]['fin_5_original'] );
			unset( $resultado[$chave]['fin_0_copia'] );
			unset( $resultado[$chave]['fin_1_copia'] );
			unset( $resultado[$chave]['fin_2_copia'] );
			unset( $resultado[$chave]['fin_3_copia'] );
			unset( $resultado[$chave]['fin_4_copia'] );
			unset( $resultado[$chave]['fin_5_copia'] );
			
			// financeiro
				$resultado[$chave]['fin_sol'] = array();
					$resultado[$chave]['fin_sol'][0] = 0;
					$resultado[$chave]['fin_sol'][1] = 0;
					$resultado[$chave]['fin_sol'][2] = 0;
					$resultado[$chave]['fin_sol'][3] = 0;
					$resultado[$chave]['fin_sol'][4] = 0;
					$resultado[$chave]['fin_sol'][5] = 0;
				$resultado[$chave]['fin_ate'] = array();
					$resultado[$chave]['fin_ate'][0] = 0;
					$resultado[$chave]['fin_ate'][1] = 0;
					$resultado[$chave]['fin_ate'][2] = 0;
					$resultado[$chave]['fin_ate'][3] = 0;
					$resultado[$chave]['fin_ate'][4] = 0;
					$resultado[$chave]['fin_ate'][5] = 0;
			// fisico
				$resultado[$chave]['fis_sol'] = array();
					$resultado[$chave]['fis_sol'][0] = 0;
					$resultado[$chave]['fis_sol'][1] = 0;
					$resultado[$chave]['fis_sol'][2] = 0;
					$resultado[$chave]['fis_sol'][3] = 0;
					$resultado[$chave]['fis_sol'][4] = 0;
					$resultado[$chave]['fis_sol'][5] = 0;
				$resultado[$chave]['fis_ate'] = array();
					$resultado[$chave]['fis_ate'][0] = 0;
					$resultado[$chave]['fis_ate'][1] = 0;
					$resultado[$chave]['fis_ate'][2] = 0;
					$resultado[$chave]['fis_ate'][3] = 0;
					$resultado[$chave]['fis_ate'][4] = 0;
					$resultado[$chave]['fis_ate'][5] = 0;
			// total
				$resultado[$chave]['tot_sol'] = array();
					$resultado[$chave]['tot_sol'][0] = 0;
					$resultado[$chave]['tot_sol'][1] = 0;
					$resultado[$chave]['tot_sol'][2] = 0;
					$resultado[$chave]['tot_sol'][3] = 0;
					$resultado[$chave]['tot_sol'][4] = 0;
					$resultado[$chave]['tot_sol'][5] = 0;
				$resultado[$chave]['tot_ate'] = array();
					$resultado[$chave]['tot_ate'][0] = 0;
					$resultado[$chave]['tot_ate'][1] = 0;
					$resultado[$chave]['tot_ate'][2] = 0;
					$resultado[$chave]['tot_ate'][3] = 0;
					$resultado[$chave]['tot_ate'][4] = 0;
					$resultado[$chave]['tot_ate'][5] = 0;
			
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
		$resultado[$chave]['fin_sol'][5] += $item['fin_5_' . $campoSolicitacao];
		
		$resultado[$chave]['fin_ate'][0] += $item['fin_0_' . $campoAtendimento];
		$resultado[$chave]['fin_ate'][1] += $item['fin_1_' . $campoAtendimento];
		$resultado[$chave]['fin_ate'][2] += $item['fin_2_' . $campoAtendimento];
		$resultado[$chave]['fin_ate'][3] += $item['fin_3_' . $campoAtendimento];
		$resultado[$chave]['fin_ate'][4] += $item['fin_4_' . $campoAtendimento];
		$resultado[$chave]['fin_ate'][5] += $item['fin_5_' . $campoAtendimento];
		
		$resultado[$chave]['fis_sol'][0] += $item['fis_0_' . $campoSolicitacao];
		$resultado[$chave]['fis_sol'][1] += $item['fis_1_' . $campoSolicitacao];
		$resultado[$chave]['fis_sol'][2] += $item['fis_2_' . $campoSolicitacao];
		$resultado[$chave]['fis_sol'][3] += $item['fis_3_' . $campoSolicitacao];
		$resultado[$chave]['fis_sol'][4] += $item['fis_4_' . $campoSolicitacao];
		$resultado[$chave]['fis_sol'][5] += $item['fis_5_' . $campoSolicitacao];
		
		$resultado[$chave]['fis_ate'][0] += $item['fis_0_' . $campoAtendimento];
		$resultado[$chave]['fis_ate'][1] += $item['fis_1_' . $campoAtendimento];
		$resultado[$chave]['fis_ate'][2] += $item['fis_2_' . $campoAtendimento];
		$resultado[$chave]['fis_ate'][3] += $item['fis_3_' . $campoAtendimento];
		$resultado[$chave]['fis_ate'][4] += $item['fis_4_' . $campoAtendimento];
		$resultado[$chave]['fis_ate'][5] += $item['fis_5_' . $campoAtendimento];
		
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
{
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

    if (!$res)
        return true;

    $sba = array();
	$arrCosid = array();
    
    while (list(, $dat) = each($res)) {
        $sba[] = $dat['sbaid'];
    }

    if(count($sba)>0){
	    $sba = implode(',', $sba);
		$sql = '
	    delete from cte.subacaoparecertecnico
	    where
	        sbaid in (' . $sba . ');
	
	    delete from cte.subacaobeneficiario
	    where
	        sbaid in (' . $sba . ');';
		
	    if(count($arrCosid)>0){
		    $cosid = implode(', ', $arrCosid);
			$sql .= '
			delete from cte.monitoramentocomposicao
		    where
		        cosid in ('.$cosid.');';
	    }
	    
	    $sql .= 'delete from cte.composicaosubacao
	    where
	        sbaid in (' . $sba . ');
	
	    delete from cte.qtdfisicoano
	    where
	        sbaid in (' . $sba . ');
	
	    delete from cte.termosubacaoindicador
	    where
	        sbaid in (' . $sba . ');';
    	
		$db->executar( $sql );
	
		$db->executar('delete from cte.subacaoindicador where sbaid in (' . $sba . ')');
		$db->executar('delete from cte.pareceracaoindicador where aciid in (
							select aciid from cte.acaoindicador where ptoid in (
	                       select ptoid from cte.pontuacao where ptostatus = \'C\' and inuid = ' . $inuid . '))');
	
		$db->executar('delete from cte.acaoindicador where ptoid in (
	                       select ptoid from cte.pontuacao where ptostatus = \'C\' and inuid = ' . $inuid . ')');
	
		$db->executar('delete from cte.pontuacao where ptostatus = \'C\' and inuid = ' . $inuid);
	    $db->commit();
    }

	return true;
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
		return true;
}


function cte_convenioFNDEConcluido($inuid)
{
	global $db;
	//$sql = "select count(*) from cte.convenio where inuid =".$inuid;
	$sql = "select count(*) from cte.projetosape where inuid =".$inuid;
	$resultado = $db->pegaUm($sql) ?  true :  false;
	return $resultado;
}


function cte_removeConvenio($inuid)
{
	global $db;
	/*
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
    return true;
	$itrid = cte_pegarItrid( $inuid );
	return $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL;
}



function verifica_preenchimento( $inuid )
{
	global $db;
	$docid = cte_pegarDocid( $inuid );
	$documento = wf_pegarEstadoAtual( $docid );
	// necessário para evitar que se verifique o preenchimento do PAR na fase de Diagnóstico.
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

function cte_emitirDocumentos( $inuid ){
	if ( cte_pegarItrid( $inuid ) != INSTRUMENTO_DIAGNOSTICO_ESTADUAL )
	{
		return true;
	}
   	$parecer = cte_emitirParecer( $inuid );    
    return $parecer;
 /*   $parecer   = cte_emitirParecer( $inuid );
	
    $termo = cte_emitirTermo( $inuid );

	return $parecer && $termo;
*/
}

function cte_emitirNotaTecnica( $inuid ){
	global $db;
	if ( cte_pegarItrid( $inuid ) != INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) {
		return false;
	}
	
    try{
		# pega os dados das subações
		$sql = "SELECT * FROM cte.subacaoindicador s
					inner join cte.acaoindicador ai on ai.aciid = s.aciid
					inner join cte.pontuacao p on p.ptoid = ai.ptoid
					inner join cte.instrumentounidade iu on iu.inuid = p.inuid
					LEFT JOIN cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid  AND sptano = date_part('year', current_date)	
				WHERE iu.inuid = $inuid	
				AND spt.ssuid = 3
				AND s.frmid in(16,17)";
		
		$subacoes = $db->carregar( $sql ) ? $db->carregar( $sql ) : array();
		
		$boGerarNotaTecnica = true;
		
		// Cadastra a nova nota técnica
		$sql = "select max(notseqano) + 1 as notseqano, notano 
				from cte.notatecnica 
				where notano = (to_char(now(), 'YYYY'::text))::integer
				group by notano";
		
		$arNotaTecnica = $db->pegaLinha( $sql ) ? $db->pegaLinha( $sql ) : array( "notseqano" => "01", "notano" => date("Y") );
		
		$numeroNotaTecnica = str_pad( $arNotaTecnica["notseqano"]."/".$arNotaTecnica["notano"], 7, "0", STR_PAD_LEFT );
		
		# pega o conteúdo do documento e coloca no banco
		ob_start();
		include APPRAIZ . "www/brasilpro/documento/notaTecnica.php";
		$notaTecnica = $db->escape( str_replace( "#NOTATECNICA#", $numeroNotaTecnica, ob_get_clean() ) );		
		
		$cpf = $_SESSION['usucpf'];
		$sql = "insert into cte.notatecnica( inuid, notdocumento, notdata, notusucpf, notseqano ) 
								    values ($inuid, $notaTecnica, now(), $cpf, {$arNotaTecnica["notseqano"]} )
						 returning notid";
						 						 
		$notid = $db->pegaUm( $sql );
		if ( !$notid ) {
			throw new Exception( "Ocorreu um erro ao cadastrar a nova Nota Técnica." );
		}

		# relaciona as ações ao novo termo
		foreach( $subacoes as $subacao ){
			$sql = sprintf(
				"insert into cte.notatecnicasubacaoindicador( notid, sbaid ) values ( %d, '%s' )",
				$notid,
				$subacao["sbaid"]
			);
			if ( !$db->executar( $sql ) ) {
				throw new Exception( "Ocorreu um erro ao cadastrar o novo Termo de Compromisso." );
			}
		}
		return $notid;
	} 
	catch ( Exception $erro ) {
		wf_registrarMensagem( $erro->getMessage() );
		$db->rollback();
		return null;
	}
}

function cte_emitirTermo( $inuid ){
	global $db;
	if ( cte_pegarItrid( $inuid ) != INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) {
		return false;
	}
	
    try{
		# pega os dados das subações
		$sql = sprintf(
			"
			SELECT
				si.sbaid, 
				si.sbatexto, 
				si.sbaobjetivo, 
				d.dimdsc, 
				d.dimcod, 
				d.dimid,
				pg.prgdsc,
				um.unddsc
			FROM
				cte.pontuacao p
				inner join cte.acaoindicador ai on ai.ptoid = p.ptoid
				inner join cte.subacaoindicador si on si.aciid = ai.aciid and si.frmid = %d and si.ssuid = 3
				inner join cte.indicador i on i.indid = p.indid and i.indstatus = 'A'
				inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
				inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
				left join cte.programa pg on pg.prgid = si.prgid
				left join cte.unidademedida um on um.undid = si.undid
			WHERE
				p.inuid = %d
			ORDER BY
				d.dimcod
			",
			FORMA_EXECUCAO_ASS_TEC,
			$inuid
		);
		
		$subacoes = $db->carregar( $sql );
		$subacoes = $subacoes ? $subacoes : array();
		# cadastra o novo termo
		cte_excluirTermo( $inuid );
		$cpf = $_SESSION['usucpf'];
		$sql = sprintf( "insert into cte.termo ( inuid, terdocumento, terdata, terusucpf ) values ( %d, '', now(),$cpf ) returning terid", $inuid );
		$terid = $db->pegaUm( $sql );
		if ( !$terid ) {
			throw new Exception( "Ocorreu um erro ao cadastrar o novo Termo de Compromisso." );
		}
		# pega o conteúdo do documento e coloca no banco
		ob_start();
		include APPRAIZ . "www/brasilpro/documento/termo.php";
		$termo = $db->escape( str_replace( "#TERMO#", sprintf( "%05d", $terid ), ob_get_clean() ) );
		$sql = sprintf( "update cte.termo set terdocumento = %s where terid = %d", $termo, $terid );
		if ( !$db->executar( $sql ) ) {
			throw new Exception( "" );
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

function cte_emitirParecer( $inuid ){
	global $db;
	if ( cte_pegarItrid( $inuid ) != INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) {
		return false;
	}
	try {
		# cadastra o novo termo
		cte_excluirParecer( $inuid );
		$cpf = $_SESSION['usucpf'];
		$sql = sprintf( "insert into cte.parecer ( inuid, pardocumento, pardata, usucpf ) values ( %d, '',now(),$cpf) returning parid", $inuid );
		$parid = $db->pegaUm( $sql );
		if ( !$parid ) {
			throw new Exception( "Ocorreu um erro ao cadastrar o novo Termo de Compromisso." );
		}
		$db->commit();
		# pega as ações
		$sql = sprintf(
			"SELECT 
				ai.aciid, 
				i.indid, 
				ad.ardid, 
				d.dimid
			 FROM
			 	cte.pontuacao p
				inner join cte.acaoindicador ai on ai.ptoid = p.ptoid
				inner join cte.indicador i on i.indid = p.indid and i.indstatus = 'A'
				inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
				inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
			 WHERE 
				p.inuid = %d",
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
		include APPRAIZ . "www/brasilpro/documento/parecer.php";
		$parecer = $db->escape( str_replace( "#PARECER#", sprintf( "%05d", $parid ), ob_get_clean() ) );
		$sql = sprintf( "update cte.parecer set pardocumento = %s where parid = %d", $parecer, $parid );
		if ( !$db->executar( $sql, false ) ) {
			throw new Exception( "Ocorreu um erro ao cadastrar o novo Termo de Compromisso." );
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

function cte_excluirDocumentos( $inuid ){
	
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
			"DELETE FROM cte.pareceracaoindicador WHERE parid in ( SELECT parid FROM cte.parecer WHERE inuid = %d )",
			$inuid
		);
		if ( !$db->executar( $sql ) ) {
			throw new Exception( "Ocorreu um erro ao excluir relação das subações com o Parecer antigo." );
		}
		$sql = sprintf(
			"DELETE FROM cte.parecer WHERE inuid = %d",
			$inuid
		);
		if ( !$db->executar( $sql ) ) {
			throw new Exception( "Ocorreu um erro ao excluir o Parecer antigo." );
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
	$sql = sprintf( "select terdocumento from cte.termo where inuid = %d", $inuid );	
	return (string) $db->pegaUm( $sql );
}

function cte_pegarParecer( $inuid ){
	global $db;
	$sql = sprintf( "select pardocumento from cte.parecer where inuid = %d", $inuid );
	return (string) $db->pegaUm( $sql );
}

function cte_pegarNotaTecnica( $inuid ){
	global $db;
	$sql = "select notdocumento 
			from cte.notatecnica 
			where inuid = $inuid
			and notano = (to_char(now(), 'YYYY'::text))::integer
			and notseqano = ( 
								select max(notseqano) 
								from cte.notatecnica 
								where notano = (to_char(now(), 'YYYY'::text))::integer 
							 );";

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

	// Recuperando todas as subações de um Instrumento Unidade
	$sql = "select sbaid from cte.subacaoindicador su 
				inner join cte.acaoindicador ai on ai.aciid = su.aciid
				inner join cte.pontuacao p on p.ptoid = ai.ptoid
				inner join cte.instrumentounidade iu on iu.inuid = p.inuid
			where p.ptostatus = 'A'
			-- Deixar esse comentário para facilitar futuros debugs
			-- and sbaid = 1825385 
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
			$arAnalisado[$fase][] = brp_validarSubAcaoFaseAnalise( $subacao );
			
		} // Fim de foreach( $res as $subacao )
	} // Fim de if( is_array( $res ) )
	
	$formaExecucao = $estado_documento["esdid"] == CTE_ESTADO_ANALISE ? FORMA_EXECUCAO_ASS_TEC : FORMA_EXECUCAO_ASS_FIN;
	
	// Se possuir pelo menos um valor falso no índice de Assistência Técnica, retorna falso, caso contrário, verdadeiro.
	return true;
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
		where
			iu.inuid = " . $inuid . " and
			p.ptostatus = 'A' and
			si.frmid != " . FORMA_EXECUCAO_ASS_TEC . "  and
			si.ssuid is null
	";
	
	return $db->pegaUm( $sql ) > 0;
	#return $db->pegaUm( $sql ) == 0;
	#return true;
}

/*
 * Função criada por Alexandre Dourado
 * Data: 06/08/2008
 * Validar se a subacao possui algum cronograma preenchido ( pelo menos de algum ano ), com exceção 
 * se for Cobertural Universal MEC
 */

/*
 * Função criada por Alexandre Dourado
 * Data: 06/08/2008
 * Validar se a subacao possui algum cronograma preenchido ( pelo menos de algum ano ), com exceção 
 * se for Cobertural Universal MEC
 * Atualizada por : Thiago Tasca Barbosa para a nova Versão do PAR (PorSubações) Data: 04/08/2008
 */

/*
 * Função criada por Alexandre Dourado
 * Data: 06/08/2008
 * Validar se a subacao possui algum cronograma preenchido ( pelo menos de algum ano ), com exceção 
 * se for Cobertural Universal MEC
 * Atualizada por : Thiago Tasca Barbosa para a nova Versão do PAR (PorSubações) Data: 04/08/2008
 *                : Bruno Adann em 08/08/2009 - Correção da Rotina de Verificação de Erros.
 */

function cte_validarSubAcao( $subacao, $nrAno = null ){

	$docid = cte_pegarDocid( $_SESSION['inuid'] );
	$estado_documento = wf_pegarEstadoAtual( $docid );		
	
	if( $subacao['ppsindcobuni'] == 't' )
        return false;

    global $db;

    $sbaid  = $subacao['sbaid'];
    $sbadsc = $subacao['sbadsc'];
	$frmid = $subacao['frmid'];
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
		if( !count( $anoParecer ) && !count( $anoEscola ) && !count( $anoItens ) && !count( $anoBeneficiario ) && ($frmid!=21) ){
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
 * Atualizada por : Thiago Tasca Barbosa para a nova Versão do PAR (PorSubações) Data: 05/08/2008
 * 					Thiago Tasca Barbosa  Desc: inserção de verificação de Ações Data: 24/11/2008
 */
function cte_exibeErrosSubAcao($erros) {

    echo '<html>'
        ,'<head>'
        ,'<title>Verificação de pendências em subações</title>'
        ,'<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">'
        ,'<link rel="stylesheet" type="text/css" href="/includes/listagem.css">'
        ,'<script>'
        ,'function alterarSubacao(sbaid){'
        ,'var janela=window.open("/brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&sbaid="+sbaid,"detalhesSubacao","height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes");'
        ,'janela.focus();}'
        ,'function alterarAcao(aciid){'
        ,'var janela=window.open("/brasilpro/brasilpro.php?modulo=principal/par_acao_pendencias&acao=A&aciid="+aciid,"detalhesAcao","height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes");'
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
        echo '<td colspan="2" style="text-align:center;font-size:14px;font-weight:bold;color:#900">O sistema verificou que alguns dados do plano não foram preenchidos:<br/><span style="font-weight:normal;"></span></td>'
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
        
	$docid = cte_pegarDocid( $inuid );
	$estadoDocumento = wf_pegarEstadoAtual($docid);
	
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
							            where aciid = ' . $idacao);
            if (!is_array($subacao))
                continue;
				
			$boPossuiSubacao = true;
			
			if( $estadoDocumento["esdid"] == CTE_ESTADO_PAR ){
	            foreach ( $subacao as $subacao1 ){
	                if ($erros = cte_validarSubAcao($subacao1)) {
	                	$errossub['subacoes'][] = $erros;
	                }
	            }
            }
        }    
    }
    
    if( $estadoDocumento["esdid"] != CTE_ESTADO_DIAGNOSTICO ){
	    if( !$boPossuiAcao ){
	    	$errossub['subacoes'][0][0]['sbadsc'] = "Não há Ação cadastrada";
	    }
	    elseif( !$boPossuiSubacao ){
	    	$errossub['subacoes'][0][0]['sbadsc'] = "Não há Sub-ação cadastrada";
	    }
    }
    
    return (is_array($errossub) && !count($errossub)) ? true : $errossub;
}


function brp_recuperArArAno($ano = NULL){
		if($ano != NULL ){//Se existir o ano e o ano do aditivo
		return array( $ano ); 
	}else{
		return array(2008, 2009, 2010, 2011, 2012, 2013, 2014 ); 
	}
	
}

function brp_possuiSubacaoPorEscola( $sbaid ){
	
	global $db;
	
	$sql = "select sbaporescola from cte.subacaoindicador s
			where s.sbaid = $sbaid";
			
	return $db->pegaUm( $sql ) == 't' ? true : false;
}

/**
 * function brp_validarSubAcaoFaseAnalise
 * Função que verifica se uma subacao está validada ou não 
 * @param array $subacao - Array com os valores de uma subação
 * @return bool - Retorna true ou false em caso a subação esteja válida ou não, respectivamente   
 * @author Orion Teles de Mesquita
 * @since 21/11/2008
 */
function brp_validarSubAcaoFaseAnalise( $subacao ){
	
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
	if( !count( $arDados ) || $arErro ){
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
	
	$boAnalisado = false;
	// Varre todos os anos que possuem registros em uma subação e a valida
	foreach( $arDados as $nrAno => $arDadosAno ){
			
		$boExisteDados = brp_possuiDadosLancados( $arDadosAno );
		$boExisteParecer = brp_possuiParecerLancado( $arDadosAno );
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
			if( $arDadosAno["ssuid"] == STATUS_SUBACAO_JA_CONTEMPLADA || $arDadosAno["ssuid"] == STATUS_SUBACAO_NAO_ATENDIDA ){
				$boAnalisado = true; 
			}
			else{
				return false;
			}
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
 * function brp_possuiDadosLancados
 * Função que verifica se possui algum dado (quantidade e cronograma físico) lançado na subação 
 * @param array $arDadosAno - Array de dados do ano de uma subação
 * @return bool - Retorna true ou false em caso de haver dado preenchido ou não, respectivamente   
 * @author Orion Teles de Mesquita
 * @since 21/11/2008
 */
function brp_possuiDadosLancados( $arDadosAno ){
	
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
 * function brp_possuiParecerLancado
 * Função que verifica se possui parecer (parecer e status) lançado na subação 
 * @param array $arDadosAno - Array de dados do ano de uma subação
 * @return bool - Retorna true ou false em caso de haver parecer dado ou não, respectivamente   
 * @author Orion Teles de Mesquita
 * @since 21/11/2008
 */
function brp_possuiParecerLancado( $arDadosAno ){

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

function brp_recuperarAnosSubacaoConveniada( $sbaid ){
	
	global $db;
	$sql="select pssano as sbcano from cte.projetosapesubacao where sbaid = ".trim( $sbaid );
	
	$conveniada = $db->carregarColuna($sql);
	
	sort( $conveniada );
	return implode( ", ", $conveniada );
	
}

function brp_possuiAditivo( $sbaid ){
	
	global $db;
	$sql="select count(*) from cte.subacaoindicador s where sbaidpai = ".trim( $sbaid );
	
	return $db->pegaUm( $sql );
				
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


	$pflcod = $db->carregar( $sql );
	
	foreach($pflcod as $dados){
		$arPflcod[] = $dados['pflcod'];
	}
	
	return $arPflcod;
}

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

function preCriarDocumento( $preid, $tpdid = FLUXO_OBRAS_BRASIL_PRO ) {
	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	$docid = prePegarDocid( $preid );
	
	if( !$docid ) {
		// descrição do documento
		switch ($tpdid){
			case WF_FLUXO_PRO_INFANCIA:
				$docdsc = "Fluxo pró infância";
				break;
			case WF_FLUXO_PRONATEC:
				$docdsc = "Fluxo Pronatec";
				break;
			case WF_FLUXO_OBRAS_PAR:
				$docdsc = "Fluxo Par";
				break;
		}
		
		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza pap do EMI
		$sql = "UPDATE
					obras.preobra
				SET 
					docid = {$docid} 
				WHERE
					preid = {$preid}";

		$db->executar( $sql );
		$db->commit();
	}
	
	return $docid;
	
}

function prePegarDocid( $preid ) {
	
	global $db;
	
	$sql = "SELECT
				docid
			FROM
				obras.preobra
			WHERE
			 	preid = " . (integer) $preid;
	
	return (integer) $db->pegaUm( $sql );
	
}

function prePegarEstadoAtual( $docid ) {
	
	global $db; 
	 
	$sql = "SELECT
				ed.esdid
			FROM 
				workflow.documento d
			INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
			WHERE
				d.docid = " . $docid;
	
	$estado = (integer) $db->pegaUm( $sql );
	 
	return $estado;
	
}

function pegaQrpidAnalisePAC( $preid, $queid ){
	
	include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
	
    global $db;
   
    $sql = "SELECT
            	po.qrpid as qrpid
            FROM
            	obras.preobraanalise po
            LEFT JOIN questionario.questionarioresposta q ON q.qrpid = po.qrpid
            WHERE
            	po.preid = {$preid}
            	AND q.queid = {$queid}";
    
    $dados = $db->pegaLinha( $sql );
    
    if( empty( $dados['qrpid'] ) ){
        $arParam = array ( "queid" => $queid, "titulo" => "OBRAS (".$preid.")" );
        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
        $sql = "UPDATE
                    obras.preobraanalise
            	SET
                    qrpid = {$qrpid}
            	WHERE
                    preid = {$preid}";
    	$db->executar( $sql );
    	$db->commit();
    } else {
    	$qrpid = $dados['qrpid'];
    }
    return $qrpid;
}

?>
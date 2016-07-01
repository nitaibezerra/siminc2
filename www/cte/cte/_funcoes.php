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
		else if(cte_pegarUfsPermitidas() && !cte_possuiPerfil(CTE_PERFIL_EQUIPE_MUNICIPAL)) {
			// pega estados do perfil do usuário
			$sql = "
				select
					muncod
				from territorios.municipio
				where estuf in ('". implode( "','", cte_pegarUfsPermitidas() ) ."')";	
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
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA );
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
			$perfis = array();
			break;
		case CTE_ESTADO_ANALISE:
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA );
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
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA );
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
	$c = ( cte_possuiPerfil( CTE_PERFIL_EQUIPE_TECNICA ) && $documento['esdid'] == CTE_ESTADO_ANALISE );
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
	if ( cte_verificaGrandeMunicipio( $inuid ) ) {
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

		$sql = "
			delete from cte.subacaobeneficiario
			where
				sbaid = " . $sbaid;

		$db->executar( $sql );

		$sql = "
			delete from cte.composicaosubacao
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
			delete from cte.subacaoindicador
			where
				sbaid = " . $sbaid;

		$db->executar( $sql );
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
						 left join  cte.subacaomunicipio sam on sam.ppsid = psa.ppsid and sam.muncod = '".$muncod."'
						 where psa.ppaid = ".$ppaid." ";
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

function cte_apagaCopiaPlanoAcao ( $inuid )
{
	global $db;
	
	if ( $inuid == '' || empty( $inuid ) ) return false;

	$db->executar("delete from cte.subacaoindicador where aciid in
				(select aciid from cte.acaoindicador where ptoid in
				(select ptoid from cte.pontuacao where ptostatus = 'C' and inuid = $inuid ))");
	
	$db->executar("delete from cte.acaoindicador where ptoid in
				(select ptoid from cte.pontuacao where ptostatus = 'C' and inuid = $inuid )");
	
	$db->executar("delete from cte.pontuacao where ptostatus = 'C' and inuid = $inuid ");
	return true;
}

function cte_copiarPlanoAcao ( $inuid )
{
	global $db;
	
	if ( $inuid == '' || empty( $inuid ) ) return false;
	
	$db->executar("delete from cte.subacaoindicador where aciid in
				(select aciid from cte.acaoindicador where ptoid in
				(select ptoid from cte.pontuacao where ptostatus = 'C' and inuid = $inuid ))");
	
	$db->executar("delete from cte.acaoindicador where ptoid in
				(select ptoid from cte.pontuacao where ptostatus = 'C' and inuid = $inuid )");
	
	$db->executar("delete from cte.pontuacao where ptostatus = 'C' and inuid = $inuid ");
	
	
	$pontuacao = $db->carregar("select ptoid from cte.pontuacao where ptostatus = 'A' and inuid = $inuid ");

	foreach ( $pontuacao as $pontuacao1 ):

		$idpontuacao = $pontuacao1['ptoid'];
		$sql = " insert into cte.pontuacao 
					(  crtid,
					  ptojustificativa,
					  ptodemandamunicipal,
					  ptodemandaestadual,
					  ptodata,
					  usucpf,
					  inuid,
					  indid,
					  ptostatus,
					  ptoparecertecnico)
					select 
					  crtid,
					  ptojustificativa,
					  ptodemandamunicipal,
					  ptodemandaestadual,
					  ptodata,
					  usucpf,
					  inuid,
					  indid,
					  'C',
					  ptoparecertecnico
					from cte.pontuacao
					where ptoid = $idpontuacao returning ptoid; ";
		$novoidpontuacao = $db->pegaUm($sql);

		$acao = $db->carregar("select aciid from cte.acaoindicador where ptoid = ".$idpontuacao);				
		
		if ( $acao != '' )
		{

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

						if($erros = cte_validarSubAcao($subacao1)) {
							$errossub[] = $erros;
						}
					
						$idsubacao = $subacao1['sbaid'];
						$sql2 = " insert into cte.subacaoindicador 
									(    aciid ,
										  undid,
										  frmid,
										  sbadsc,
										  sbastgmpl,
										  sbaprm,
										  sbapcr,
										  sba0ano,
										  sba1ano,
										  sba2ano,
										  sba3ano,
										  sba4ano,
										  sbaunt,
										  sbauntdsc,
										  sba0ini,
										  sba0fim,
										  sba1ini,
										  sba1fim,
										  sba2ini,
										  sba2fim,
										  sba3ini,
										  sba3fim,
										  sba4ini,
										  sba4fim,
										  sbadata,
										  usucpf
										)
									select 
										  $novoidacao ,
										  undid,
										  frmid,
										  sbadsc,
										  sbastgmpl,
										  sbaprm,
										  sbapcr,
										  sba0ano,
										  sba1ano,
										  sba2ano,
										  sba3ano,
										  sba4ano,
										  sbaunt,
										  sbauntdsc,
										  sba0ini,
										  sba0fim,
										  sba1ini,
										  sba1fim,
										  sba2ini,
										  sba2fim,
										  sba3ini,
										  sba3fim,
										  sba4ini,
										  sba4fim,
										  sbadata,
										  usucpf
									from cte.subacaoindicador
									where sbaid = $idsubacao returning aciid; ";
						$novoidsubacao = $db->pegaUm($sql2);			
						
					endforeach;
				}
				
			endforeach;
		}
	endforeach;
	
	if(count($errossub) > 0) {
		cte_exibeErrosSubAcao($errossub);
		exit;	
	} else {
		return true;
	}
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

function verifica_preenchimento( $inuid )
{
	return cte_pegarPercentagem( $inuid ) > 99;
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

	return $parecer && $termo;
}

function cte_emitirTermo( $inuid ){
	global $db;
	if ( cte_pegarItrid( $inuid ) != INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {
		return false;
	}
	try {
		$muncod = $db->pegaUm( sprintf( "select muncod from cte.instrumentounidade where inuid = %d", $inuid ) );
		# pega os dados da prefeitura
		$sql = sprintf(
			"select distinct ent.*, ende.*, mnu.*, f.*
			from entidade.entidade ent
			inner join entidade.funcaoentidade efe on efe.entid = ent.entid			
			inner join entidade.funcao f on f.funid = efe.funid
			inner join entidade.endereco ende on ent.entid = ende.entid
			inner join territorios.municipio mnu on ende.muncod = mnu.muncod
			where mnu.muncod = '%s'
			and f.funid = %d",
			$muncod,
			FUNCAO_PREFEITURA
		);
		$prefeitura = $db->pegaLinha( $sql );
		$prefeitura = $prefeitura ? $prefeitura : null;
		if ( !$prefeitura ) {
			throw new Exception( "Não foi possível encontrar os dados da prefeitura." );
		}
		# pega os dados do prefeito
		$sql = sprintf(
			"select distinct entpref.*, ende.*, mnu.*, f.*
			from entidade.entidade ent
			inner join entidade.funcaoentidade fe1 ON fe1.entid = ent.entid
			inner join entidade.funentassoc fea ON fea.fueid = fe1.fueid
			inner join entidade.entidade entpref on fea.entid = entpref.entid
			inner join entidade.funcaoentidade fe2 ON fe2.entid = entpref.entid
			inner join entidade.funcaoentidade efe on efe.entid = ent.entid			
			inner join entidade.funcao f on f.funid = efe.funid and f.funid = %d and entpref.entstatus = 'A'
			inner join entidade.endereco ende on ent.entid = ende.entid
			inner join territorios.municipio mnu on ende.muncod = mnu.muncod
			where mnu.muncod = '%s'
			and f.funid = %d",
			FUNCAO_PREFEITO,
			$muncod,
			FUNCAO_PREFEITURA
		);
		$prefeito = $db->pegaLinha( $sql );
		$prefeito = $prefeito ? $prefeito : null;
		if ( !$prefeito ) {
			throw new Exception( "Não foi possível encontrar os dados do prefeito." );
		}
		# pega os dados das subações
		$sql = sprintf(
			"
			select
			si.sbaid, si.sbatexto, si.sbaobjetivo, si.sba1ano, si.sba2ano, si.sba3ano, si.sba4ano, ps.ppsid,
			d.dimdsc, d.dimcod, d.dimid,
			pg.prgdsc,
			um.unddsc
			from cte.pontuacao p
			inner join cte.acaoindicador ai on ai.ptoid = p.ptoid
			inner join cte.subacaoindicador si on
				si.aciid = ai.aciid and ( si.sbatexto != '' and si.sbatexto is not null ) and si.frmid = %d and si.ssuid = 3
			inner join cte.proposicaosubacao ps on ps.ppsid = si.ppsid
			inner join cte.indicador i on i.indid = p.indid and i.indstatus = 'A'
			inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
			inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
			left join cte.programa pg on pg.prgid = si.prgid
			left join cte.unidademedida um on um.undid = si.undid
			where p.inuid = %d
			order by d.dimcod
			",
			SUBACAO_ASSISTENCIA_TECNICA,
			$inuid
		);;
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
		include APPRAIZ . "www/cte/documento/termo.php";
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
	if ( cte_pegarItrid( $inuid ) != INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {
		return false;
	}
	try {
		$muncod = $db->pegaUm( sprintf( "select muncod from cte.instrumentounidade where inuid = %d", $inuid ) );
		# pega os dados da prefeitura
		$sql = sprintf(
			"select ent.*, entend.*, ende.*, mnu.*, f.*
			from entidade.entidade ent
			inner join entidade.funcao f on f.funid = ent.funid 
			inner join entidade.entidadeendereco entend on ent.entid = entend.entid
			inner join entidade.endereco ende on entend.endid = ende.endid
			inner join territorios.municipio mnu on ende.muncod = mnu.muncod
			where mnu.muncod = '%s'
			and ent.funid = %d",
			$muncod,
			FUNCAO_PREFEITURA
		);
		$prefeitura = $db->pegaLinha( $sql );
		$prefeitura = $prefeitura ? $prefeitura : null;
		if ( !$prefeitura ) {
			throw new Exception( "Não foi possível encontrar os dados da prefeitura." );
		}
		# pega os dados do prefeito
		$sql = sprintf(
			"select entpref.*, entend.*, ende.*, mnu.*, f.*
			from entidade.entidade ent
			inner join entidade.funcao f on f.funid = ent.funid 
			inner join entidade.entidadeendereco entend on ent.entid = entend.entid
			inner join entidade.endereco ende on entend.endid = ende.endid
			inner join territorios.municipio mnu on ende.muncod = mnu.muncod
			inner join entidade.entidade entpref on ent.entid = entpref.entidassociado and entpref.funid = %d and entpref.entstatus = 'A'
			where mnu.muncod = '%s'
			and ent.funid = %d",
			FUNCAO_PREFEITO,
			$muncod,
			FUNCAO_PREFEITURA
		);
		$prefeito = $db->pegaLinha( $sql );
		$prefeito = $prefeito ? $prefeito : null;
		if ( !$prefeito ) {
			throw new Exception( "Não foi possível encontrar os dados do prefeito." );
		} 
		# cadastra o novo termo
		cte_excluirParecer( $inuid );
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
			"delete from cte.pareceracaoindicador where parid in ( select parid from cte.termo where inuid = %d )",
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

function cte_assTecnAnalisada( $inuid )
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
			si.frmid = " . FORMA_EXECUCAO_ASS_TEC . " and
			si.ssuid is null
	";
	return $db->pegaUm( $sql ) == 0;
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
	return $db->pegaUm( $sql ) == 0;
}

function cte_parecerFinalizado($inuid)
{
    global $db;
    $sql = 'SELECT
                count(*) as total
            FROM
                cte.parecerpar pp
            INNER JOIN
                cte.parecerinstrumento pi ON pp.parid = pi.parid
            WHERE
                pp.tppid <> 3
                AND pi.inuid = ' . $inuid;

    $res = $db->pegaUm($sql);

    return $res >= 1;
}


function cte_convenioFNDEConcluido($inuid)
{
	global $db;
	$sql = "select count(*) from cte.convenio where inuid =".$inuid;
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
		
	
	global $db;
	
	try{
		$sql = "delete from cte.subacaoconvenio where cnvid in ( select cnvid from cte.convenio where inuid = $inuid )";

		if (!$db->executar( $sql )) {
			throw new Exception( "Ocorreu um erro ao tentar excluir a relação das subações com o convênio." );
		}
		
		$sql = "delete from cte.convenioretorno where cnvid in ( select cnvid from cte.convenio where inuid = $inuid )";

		if (!$db->executar( $sql )) {
			throw new Exception( "Ocorreu um erro ao tentar excluir os números de processos." );
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
	

}

/*
 * Função criada por Alexandre Dourado
 * Data: 06/08/2008
 * Validar se a subacao possui algum cronograma preenchido ( pelo menos de algum ano ), com exceção 
 * se for Cobertural Universal MEC
 */

function cte_validarSubAcao($subacao) {
	// Configurações dos anos
	$anocampo = array('2007' => 'sba0',
				 	  '2008' => 'sba1',
				 	  '2009' => 'sba2',
					  '2010' => 'sba3',
					  '2011' => 'sba4');
	
	// Validando as subações
	// Verificando se (é Cobertural Universal MEC) e (Status Subação é Já Contemplada em Outro Indicador)
	if($subacao['ppsindcobuni'] == 'f' && $subacao['ssuid'] != '5') {
		$validasubacao = false;
		// Verificando se tem algum registro
		foreach($anocampo as $ano => $campo) {
			if($subacao[$anocampo[$ano].'ano'] && $subacao[$anocampo[$ano].'ini'] && $subacao[$anocampo[$ano].'fim']) {
				$validasubacao = true;
			}
		}
		if(!$validasubacao) {
			$errovalidacaosubacao[$subacao['sbaid']] = $subacao['sbadsc'];
			return $errovalidacaosubacao;
		}
	}
	return false;

}

/*
 * Função criada por Alexandre Dourado
 * Data: 06/08/2008
 * Imprime os erros encontrados durante a tramitação do documento
 */
function cte_exibeErrosSubAcao($erros) {
	
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../../includes/Estilo.css\">
		  <link rel=\"stylesheet\" type=\"text/css\" href=\"../../includes/listagem.css\">
		  <script>
			function alterarSubacao( sbaid ){
				var janela = window.open( \"../../cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=\" + sbaid, 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
				janela.focus();
			}
		  </script>
		  
		  <div style=\"width : 100%; height : 100%; overflow-y : scroll;\">
			<table class=\"tabela\">
			<tr>
			<td colspan='2'>O sistema verificou que alguns dados das seguintes subações não foram preenchidos :</td>
			</tr>";
	
		foreach($erros as $dados) {
			echo "<tr style=\"background-color: #d9d9d9;\">";
			echo "<td><img src='/imagens/consultar.gif' onclick='alterarSubacao(". key($dados) .")'></td>";
			echo "<td><b>". current($dados) ."</b></td>";
			echo "</tr>";
			
		}
		echo "</table>";	
}

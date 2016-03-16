<?php

include_once "constantes.php";

// funcões gerais

function reuni_parecerGlobalPendente( $unicod )
{
	// captura estado
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	
	// realiza verificacao de acordo com o estado
	switch ( $esdid )
	{
		case REUNI_ESTADO_SESU:
			return
				reuni_podeEditarParecerSesu( $unicod );
				/*reuni_podeEditarParecerSesu( $unicod ) &&
				reuni_parecerRespostaCompleto( $unicod, REUNI_PARECER_SESU );*/
		case REUNI_ESTADO_ADHOC:
			return
				reuni_podeEditarParecerAdhoc( $unicod ) &&
				reuni_parecerRespostaCompleto( $unicod, REUNI_PARECER_ADHOC );
		case REUNI_ESTADO_COMISSAO:
			return
				reuni_podeEditarParecerComissao( $unicod ) &&
				reuni_parecerRespostaCompleto( $unicod, REUNI_PARECER_COMISSAO );
		case REUNI_ESTADO_SESU_FINAL:
			return
				reuni_podeEditarParecerSesuFinal( $unicod ) &&
				reuni_parecerRespostaCompleto( $unicod, REUNI_PARECER_SESU_FINAL );
		default:
			return false;
	}
}

function reuni_podeVerParecerGlobal( $unicod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// captura estado
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	
	// realiza verificacao de acordo com o estado
	switch ( $esdid )
	{
		case REUNI_ESTADO_ELABORACAO:
		case REUNI_ESTADO_IFES:
			$perfis = array(
				REUNI_PERFIL_IFES_APR,
				REUNI_PERFIL_IFES_CAD,
				REUNI_PERFIL_IFES_CON
			);
			return reuni_possuiPerfisUnidade( $perfis, $unicod );
		case REUNI_ESTADO_ADHOC:
			$perfis = array(
				REUNI_PERFIL_ADHOC
			);
			return reuni_possuiPerfis( $perfis );
		case REUNI_ESTADO_COMISSAO:
			$perfis = array(
				REUNI_PERFIL_COMISSAO
			);
			return reuni_possuiPerfis( $perfis );
		case REUNI_ESTADO_SESU:
		case REUNI_ESTADO_SESU_FINAL:
		case REUNI_ESTADO_APROVADO:
			$perfis = array(
				REUNI_PERFIL_SESU_APR,
				REUNI_PERFIL_SESU_CON,
				REUNI_PERFIL_SESU_PAR
			);
			return reuni_possuiPerfis( $perfis );
		default:
			return false;
	}
	
}

function reuni_podeVerResponsaveis( $unicod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	$perfisUnidade = array(
		REUNI_PERFIL_IFES_APR,
		REUNI_PERFIL_IFES_CAD,
		REUNI_PERFIL_IFES_CON
	);
	$perfis = array(
		//REUNI_PERFIL_ADHOC,
		//REUNI_PERFIL_COMISSAO,
		REUNI_PERFIL_SESU_APR,
		REUNI_PERFIL_SESU_CON,
		REUNI_PERFIL_SESU_PAR
	);
	return
		reuni_possuiPerfis( $perfis ) ||
		reuni_possuiPerfisUnidade( $perfisUnidade, $unicod );
}

function reuni_podeVerAvisoParecer()
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	$perfis = array(
		REUNI_PERFIL_ADHOC,
		REUNI_PERFIL_COMISSAO,
		REUNI_PERFIL_SESU_APR,
		REUNI_PERFIL_SESU_CON,
		REUNI_PERFIL_SESU_PAR
	);
	return reuni_possuiPerfis( $perfis );
}

function reuni_podeEditarResposta( $prgcod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// captura unidade
	$rspcod = reuni_pegarRspcod( $prgcod );
	$unicod = reuni_pegarUnicod( $rspcod );
	
	// verifica se possui pefil de IFES da resposta
	$perfis = array(
		REUNI_PERFIL_IFES_APR,
		REUNI_PERFIL_IFES_CAD,
		REUNI_PERFIL_IFES_CON
	);
	if ( !reuni_possuiPerfisUnidade( $perfis, $unicod ) )
	{
		return false;
	}
	
	// verifica se proposta está em elaboração
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	return $estado['esdid'] == REUNI_ESTADO_ELABORACAO or $estado['esdid'] == REUNI_ESTADO_IFES;
}

function reuni_podeEditarParecerSesu( $unicod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// verifica se usuário possui perfil sesu
	$perfis = array(
		REUNI_PERFIL_SESU_PAR
	);
	if ( !reuni_possuiPerfis( $perfis ) )
	{
		return false;
	}
	
	// verifica se estado da proposta é sesu
	$unicod = (integer) $unicod;
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	if ( $esdid != REUNI_ESTADO_SESU )
	{
		return false;
	}
	
	// verifica se todas as respostas estão com parecer sesu
	return true;
	//reuni_parecerRespostaCompleto( $unicod, REUNI_PARECER_SESU );
}

function reuni_podeEditarParecerRespostaSesu( $prgcod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// verifica se estado permite
	$rspcod = reuni_pegarRspcod( $prgcod );
	$unicod = reuni_pegarUnicod( $rspcod );
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	if ( $esdid != REUNI_ESTADO_SESU )
	{
		return false;
	}
	
	// verifica se usuário possui perfil
	$perfis = array(
		REUNI_PERFIL_SESU_PAR
	);
	return reuni_possuiPerfis( $perfis );
}

function reuni_podeEditarParecerAdhoc( $unicod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// verifica se usuário possui perfil adhoc
	$perfis = array(
		REUNI_PERFIL_ADHOC
	);
	if ( !reuni_possuiPerfis( $perfis ) )
	{
		return false;
	}
	
	// verifica se estado da proposta é adhoc
	$unicod = (integer) $unicod;
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	if ( $esdid != REUNI_ESTADO_ADHOC )
	{
		return false;
	}
	
	// verifica se todas as respostas estão com parecer adhoc
	return reuni_parecerRespostaCompleto( $unicod, REUNI_PARECER_ADHOC );
}

function reuni_podeEditarParecerRespostaAdhoc( $prgcod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// verifica se estado permite
	$rspcod = reuni_pegarRspcod( $prgcod );
	$unicod = reuni_pegarUnicod( $rspcod );
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	if ( $esdid != REUNI_ESTADO_ADHOC )
	{
		return false;
	}
	
	// verifica se usuário possui perfil
	$perfis = array(
		REUNI_PERFIL_ADHOC
	);
	return reuni_possuiPerfis( $perfis );
}

function reuni_podeEditarParecerComissao( $unicod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// verifica se usuário possui perfil comissao
	$perfis = array(
		REUNI_PERFIL_COMISSAO
	);
	if ( !reuni_possuiPerfis( $perfis ) )
	{
		return false;
	}
	
	// verifica se estado da proposta é comissao
	$unicod = (integer) $unicod;
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	return $esdid == REUNI_ESTADO_COMISSAO;
	/*
	if ( $esdid != REUNI_ESTADO_COMISSAO )
	{
		return false;
	}
	
	// verifica se todas as respostas estão com parecer comissao
	return reuni_parecerRespostaCompleto( $unicod, REUNI_PARECER_COMISSAO );
	*/
}

function reuni_podeEditarParecerRespostaComissao( $prgcod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// verifica se estado permite
	$rspcod = reuni_pegarRspcod( $prgcod );
	$unicod = reuni_pegarUnicod( $rspcod );
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	if ( $esdid != REUNI_ESTADO_COMISSAO )
	{
		return false;
	}
	
	// verifica se usuário possui perfil
	$perfis = array(
		REUNI_PERFIL_COMISSAO
	);
	return reuni_possuiPerfis( $perfis );
}

function reuni_podeEditarParecerSesuFinal( $unicod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// verifica se usuário possui perfil sesu
	$perfis = array(
		REUNI_PERFIL_SESU_APR
	);
	if ( !reuni_possuiPerfis( $perfis ) )
	{
		return false;
	}
	
	// verifica se estado da proposta é sefu final
	$unicod = (integer) $unicod;
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	return $esdid == REUNI_ESTADO_SESU_FINAL;
}

function reuni_podeVerResposta( $prgcod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// captura unidade
	$rspcod = reuni_pegarRspcod( $prgcod );
	$unicod = reuni_pegarUnicod( $rspcod );
	
	// verifica se possui pefil de IFES da resposta
	$perfis = array(
		REUNI_PERFIL_IFES_APR,
		REUNI_PERFIL_IFES_CAD,
		REUNI_PERFIL_IFES_CON
	);
	if ( reuni_possuiPerfisUnidade( $perfis, $unicod ) )
	{
		return true;
	}
	
	// verifica se estado da proposta permite
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	$estadoNegados = array(
		REUNI_ESTADO_ELABORACAO,
		REUNI_ESTADO_IFES
	);
	return !in_array( $esdid, $estadoNegados );
}

function reuni_podeVerParecerSesu( $unicod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// captura estado
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	
	// realiza verificacao de acordo com o estado
	switch ( $esdid )
	{
		case REUNI_ESTADO_ELABORACAO:
		case REUNI_ESTADO_IFES:
			// verifica se existe parecer
			if ( reuni_existeParecerGlobal( REUNI_PARECER_SESU_GLOBAL, $unicod ) )
			{
				// caso exista o usuario pode ver se tiver algum perfil abaixo
				$perfis = array(
					REUNI_PERFIL_SESU_APR,
					REUNI_PERFIL_SESU_CON,
					REUNI_PERFIL_SESU_PAR,
					REUNI_PERFIL_ADHOC,
					REUNI_PERFIL_COMISSAO
				);
				// caso exista o usuario pode ver se tiver algum perfil abaixo na unidade especificada
				$perfisUnidade = array(
					REUNI_PERFIL_IFES_APR,
					REUNI_PERFIL_IFES_CAD,
					REUNI_PERFIL_IFES_CON
				);
				// realiza verificação
				return
					reuni_possuiPerfis( $perfis ) ||
					reuni_possuiPerfisUnidade( $perfisUnidade, $unicod );
			}
			return false;
		case REUNI_ESTADO_SESU:
			$perfis = array(
				REUNI_PERFIL_SESU_APR,
				REUNI_PERFIL_SESU_CON,
				REUNI_PERFIL_SESU_PAR
			);
			return reuni_possuiPerfis( $perfis );
		case REUNI_ESTADO_ADHOC:
			$perfis = array(
				REUNI_PERFIL_SESU_APR,
				REUNI_PERFIL_SESU_CON,
				REUNI_PERFIL_SESU_PAR,
				REUNI_PERFIL_ADHOC
			);
			return reuni_possuiPerfis( $perfis );
		case REUNI_ESTADO_COMISSAO:
		case REUNI_ESTADO_SESU_FINAL:
			$perfis = array(
				REUNI_PERFIL_SESU_APR,
				REUNI_PERFIL_SESU_CON,
				REUNI_PERFIL_SESU_PAR,
				REUNI_PERFIL_ADHOC,
				REUNI_PERFIL_COMISSAO
			);
			return reuni_possuiPerfis( $perfis );
		case REUNI_ESTADO_APROVADO:
			return true;
	}
	
}

function reuni_podeVerParecerRespostaSesu( $prgcod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	$perfis = array(
		REUNI_PERFIL_SESU_CON,
		REUNI_PERFIL_SESU_PAR
	);
	return reuni_possuiPerfis( $perfis );
	/*
	// verifica se usuário é do estado atual
	$perfis = array(
		REUNI_PERFIL_SESU_CON,
		REUNI_PERFIL_SESU_PAR
	);
	$perfilEstadoAtual = reuni_possuiPerfis( $perfis );
	
	// verifica se estado permite
	$rspcod = reuni_pegarRspcod( $prgcod );
	$unicod = reuni_pegarUnicod( $rspcod );
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	$estadosPossiveis = array(
		REUNI_ESTADO_ADHOC,
		REUNI_ESTADO_COMISSAO,
		REUNI_ESTADO_SESU_FINAL,
		REUNI_ESTADO_APROVADO
	);
	if ( $perfilEstadoAtual )
	{
		array_push( $estadosPossiveis, REUNI_ESTADO_SESU );
	}
	return in_array( $esdid, $estadosPossiveis );
	*/
}

function reuni_podeVerParecerAdhoc( $unicod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// captura estado
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	
	// realiza verificacao de acordo com o estado
	switch ( $esdid )
	{
		case REUNI_ESTADO_ELABORACAO:
		case REUNI_ESTADO_IFES:
		case REUNI_ESTADO_SESU:
			// verifica se existe parecer
			if ( reuni_existeParecerGlobal( REUNI_PARECER_ADHOC_GLOBAL, $unicod ) )
			{
				// caso exista o usuario pode ver se tiver algum perfil abaixo
				$perfis = array(
					REUNI_PERFIL_SESU_APR,
					REUNI_PERFIL_SESU_CON,
					REUNI_PERFIL_SESU_PAR,
					REUNI_PERFIL_ADHOC,
					REUNI_PERFIL_COMISSAO
				);
				// caso exista o usuario pode ver se tiver algum perfil abaixo na unidade especificada
				$perfisUnidade = array(
					REUNI_PERFIL_IFES_APR,
					REUNI_PERFIL_IFES_CAD,
					REUNI_PERFIL_IFES_CON
				);
				// realiza verificação
				return
					reuni_possuiPerfis( $perfis ) ||
					reuni_possuiPerfisUnidade( $perfisUnidade, $unicod );
			}
			return false;
		case REUNI_ESTADO_ADHOC:
			$perfis = array(
				REUNI_PERFIL_ADHOC
			);
			return reuni_possuiPerfis( $perfis );
		case REUNI_ESTADO_COMISSAO:
		case REUNI_ESTADO_SESU_FINAL:
			$perfis = array(
				REUNI_PERFIL_SESU_APR,
				REUNI_PERFIL_SESU_CON,
				REUNI_PERFIL_SESU_PAR,
				REUNI_PERFIL_ADHOC,
				REUNI_PERFIL_COMISSAO
			);
			return reuni_possuiPerfis( $perfis );
		case REUNI_ESTADO_APROVADO:
			return true;
	}
	
}

function reuni_podeVerParecerRespostaAdhoc( $prgcod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// verifica se usuário é do estado atual do parecer que esta sendo verificado
	$perfis = array(
		REUNI_PERFIL_ADHOC
	);
	$possuiPerfilEstadoAtual = reuni_possuiPerfis( $perfis );
	$rspcod = reuni_pegarRspcod( $prgcod );
	$unicod = reuni_pegarUnicod( $rspcod );
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$estadoAtual = (integer) $estado['esdid'];
	if ( $estadoAtual == REUNI_ESTADO_ADHOC && $possuiPerfilEstadoAtual )
	{
		return true;
	}
	
	// pode ver se exister parecer
	return reuni_existeParecerResposta( REUNI_PARECER_ADHOC, $unicod, $prgcod );
}

function reuni_podeVerParecerComissao( $unicod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// captura estado
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	
	// realiza verificacao de acordo com o estado
	switch ( $esdid )
	{
		case REUNI_ESTADO_ELABORACAO:
		case REUNI_ESTADO_IFES:
		case REUNI_ESTADO_SESU:
		case REUNI_ESTADO_ADHOC:
			// verifica se existe parecer
			if ( reuni_existeParecerGlobal( REUNI_PARECER_COMISSAO_GLOBAL, $unicod ) )
			{
				// caso exista o usuario pode ver se tiver algum perfil abaixo
				$perfis = array(
					REUNI_PERFIL_SESU_APR,
					REUNI_PERFIL_SESU_CON,
					REUNI_PERFIL_SESU_PAR,
					REUNI_PERFIL_ADHOC,
					REUNI_PERFIL_COMISSAO
				);
				// caso exista o usuario pode ver se tiver algum perfil abaixo na unidade especificada
				$perfisUnidade = array(
					REUNI_PERFIL_IFES_APR,
					REUNI_PERFIL_IFES_CAD,
					REUNI_PERFIL_IFES_CON
				);
				// realiza verificação
				return
					reuni_possuiPerfis( $perfis ) ||
					reuni_possuiPerfisUnidade( $perfisUnidade, $unicod );
			}
			return false;
		case REUNI_ESTADO_COMISSAO:
			$perfis = array(
				REUNI_PERFIL_COMISSAO
			);
			return reuni_possuiPerfis( $perfis );
		case REUNI_ESTADO_SESU_FINAL:
			$perfis = array(
				REUNI_PERFIL_SESU_APR,
				REUNI_PERFIL_SESU_CON,
				REUNI_PERFIL_SESU_PAR,
				REUNI_PERFIL_ADHOC,
				REUNI_PERFIL_COMISSAO
			);
			return reuni_possuiPerfis( $perfis );
		case REUNI_ESTADO_APROVADO:
			return true;
	}
	
}

function reuni_podeVerParecerRespostaComissao( $prgcod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// verifica se usuário é do estado atual do parecer que esta sendo verificado
	$perfis = array(
		REUNI_PERFIL_COMISSAO
	);
	$possuiPerfilEstadoAtual = reuni_possuiPerfis( $perfis );
	$rspcod = reuni_pegarRspcod( $prgcod );
	$unicod = reuni_pegarUnicod( $rspcod );
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$estadoAtual = (integer) $estado['esdid'];
	if ( $estadoAtual == REUNI_ESTADO_COMISSAO && $possuiPerfilEstadoAtual )
	{
		return true;
	}
	
	// pode ver se exister parecer
	return reuni_existeParecerResposta( REUNI_PARECER_COMISSAO, $unicod, $prgcod );
}

function reuni_podeVerParecerSesuFinal( $unicod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// captura estado
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	
	// realiza verificacao de acordo com o estado
	switch ( $esdid )
	{
		case REUNI_ESTADO_ELABORACAO:
		case REUNI_ESTADO_IFES:
		case REUNI_ESTADO_SESU:
		case REUNI_ESTADO_ADHOC:
		case REUNI_ESTADO_COMISSAO:
			// verifica se existe parecer
			if ( reuni_existeParecerGlobal( REUNI_PARECER_SESU_FINAL, $unicod ) )
			{
				// caso exista o usuario pode ver se tiver algum perfil abaixo
				$perfis = array(
					REUNI_PERFIL_SESU_APR,
					REUNI_PERFIL_SESU_CON,
					REUNI_PERFIL_SESU_PAR,
					REUNI_PERFIL_ADHOC,
					REUNI_PERFIL_COMISSAO
				);
				// caso exista o usuario pode ver se tiver algum perfil abaixo na unidade especificada
				$perfisUnidade = array(
					REUNI_PERFIL_IFES_APR,
					REUNI_PERFIL_IFES_CAD,
					REUNI_PERFIL_IFES_CON
				);
				// realiza verificação
				return
					reuni_possuiPerfis( $perfis ) ||
					reuni_possuiPerfisUnidade( $perfisUnidade, $unicod );
			}
			return false;
		case REUNI_ESTADO_SESU_FINAL:
			$perfis = array(
				REUNI_PERFIL_SESU_APR,
				REUNI_PERFIL_SESU_CON,
				REUNI_PERFIL_SESU_PAR
			);
			return reuni_possuiPerfis( $perfis );
		case REUNI_ESTADO_APROVADO:
			return true;
	}
	
}

function reuni_podeVerPdfSimulador( $unicod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// captura estado
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	
	// verifica se parecer está em elaboração
	if ( $esdid != REUNI_ESTADO_ELABORACAO and $esdid != REUNI_ESTADO_IFES)
	{
		return false;
	}
	
	// verifica se possui pefil de IFES cadastro
	$perfisUnidade = array(
		REUNI_PERFIL_IFES_APR,
		REUNI_PERFIL_IFES_CAD
	);
	$perfis = array(
		REUNI_PERFIL_SESU_APR,
		REUNI_PERFIL_SESU_PAR
	);
	return
		reuni_possuiPerfisUnidade( $perfisUnidade, $unicod ) ||
		reuni_possuiPerfis( $perfis );
}

function reuni_podeEditarSimulador( $unicod )
{
	global $db;
	// verifica se é super usuário
	if ( $db->testa_superuser() )
	{
		return true;
	}
	
	// captura estado
	$docid = reuni_pegarDocid( $unicod );
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	
	// verifica se parecer está em elaboração
	if ( $esdid != REUNI_ESTADO_ELABORACAO and $esdid != REUNI_ESTADO_IFES )
	{
		return false;
	}
	
	// verifica se possui pefil de IFES cadastro
	$perfisUnidade = array(
		REUNI_PERFIL_IFES_APR,
		REUNI_PERFIL_IFES_CAD
	);
	$perfis = array(
		REUNI_PERFIL_SESU_APR,
		REUNI_PERFIL_SESU_PAR
	);
	return
		reuni_possuiPerfisUnidade( $perfisUnidade, $unicod ) ||
		reuni_possuiPerfis( $perfis );
}

// funcões de apoio às funcões gerais

function reuni_pegarDocid( $unicod )
{
	global $db;
	static $docid = array();
	$unicod = (integer) $unicod;
	if ( !array_key_exists( $unicod, $docid ) )
	{
		$sql = "
			select
				docid
			from reuni.unidadeproposta
			where
				unicod = '" . $unicod . "'
		";
		$docid[$rspcod] = $db->pegaUm( $sql );
	}
	return $docid[$rspcod];
}

function reuni_pegarRspcod( $prgcod )
{
	global $db;
	static $rspcod = array();
	$prgcod = (integer) $prgcod;
	$unicod = (integer) $_SESSION['unicod'];
	if ( !array_key_exists( $prgcod, $rspcod ) )
	{
		$sql = "
			select
				rspcod
			from reuni.resposta
			where
				prgcod = '" . $prgcod . "' and
				unicod = '" . $unicod . "'
		";
		$rspcod[$prgcod] = $db->pegaUm( $sql );
	}
	return $rspcod[$prgcod];
}

function reuni_pegarUnicod( $rspcod )
{
	/*global $db;
	static $unicod = array();
	$rspcod = (integer) $rspcod;
	if ( !array_key_exists( $rspcod, $unicod ) )
	{
		$sql = "
			select
				unicod
			from reuni.resposta
			where
				rspcod = " . $rspcod;
		$unicod[$rspcod] = $db->pegaUm( $sql );
	}
	return (integer) $unicod[$rspcod];*/
	return (integer) $_SESSION['unicod'];
}

function reuni_possuiPerfis( array $perfis )
{
	global $db;
	if ( count( $perfis ) == 0 )
	{
		return false;
	}
	$perfis = array_unique( array_map( "intval", $perfis ) );
	$sql = "
		select
			count(*)
		from seguranca.perfilusuario
		where
			usucpf = '" . $_SESSION['usucpf'] . "' and
			pflcod in ( " . implode( ", ", $perfis ) . " )
	";
	return $db->pegaUm( $sql ) > 0;
}

function reuni_possuiPerfisUnidade( array $perfis, $unicod )
{
	global $db;
	$unicod = (integer) $unicod;
	$perfis = array_unique( array_map( "intval", $perfis ) );
	if ( count( $perfis ) == 0 || !$unicod )
	{
		return false;
	}
	$sql = "
		select
			count(*)
		from reuni.usuarioresponsabilidade
		where
			rpustatus = 'A' and
			unicod = '" . $unicod . "' and
			usucpf = '" . $_SESSION['usucpf'] . "' and
			pflcod in ( " . implode( ",", $perfis ) . " )
	";
	return $db->pegaUm( $sql ) > 0;
}

function reuni_parecerCompleto( $unicod, $parcod )
{
	global $db;
	$parcod = (integer) $parcod;
	$unicod = (integer) $unicod;
	$sql = "
		select
			count(*)
		from reuni.unidadeproposta u
			left join reuni.parecer p on p.unpid = u.unpid
			left join public.arquivo a on a.arqid = p.arqid
		where
			u.unicod = '" . $unicod . "' and
			p.tpacod = " . $parcod . " and
			(
				(
					p.pardsc is not null and
					p.pardsc != ''
				)
				or
				(
					a.arqid is not null and
					arqstatus = 'A'
				)
			)
	";
	return $db->pegaUm( $sql ) > 0;
}

function reuni_parecerRespostaCompleto( $unicod, $parcod )
{
	// todas as questões OBRIGATORIAS devem estar com parecer preenchidos
	global $db;
	$parcod = (integer) $parcod;
	$unicod = (integer) $unicod;
	$sql = "
		select
			count(*)
		from reuni.pergunta pe
			left join reuni.resposta up on
				up.prgcod = pe.prgcod and
				up.unicod =  '" . $unicod . "'
			left join reuni.parecer p on
				p.rspcod = up.rspcod and
				p.tpacod = " . $parcod . "
		where
			pe.prgobr = true and
			p.parcod is null
	";
	return $db->pegaUm( $sql ) == 0;
}

// funcões de apoio ao workflow

	// as função tratam da mudança de um estado para outro, e não de qualquer
	// estado para o destino desejado

function reuni_existeParecerGlobal( $tpacod, $unicod )
{
	global $db;
	$tpacod = (integer) $tpacod;
	$unicod = (integer) $unicod;
	$sql = "
		select
			count(*)
		from reuni.parecer p
			inner join reuni.unidadeproposta u on u.unpid = p.unpid
		where
			p.tpacod = " . $tpacod . " and
			u.unicod = '" . $unicod . "'
	";
	return $db->pegaUm( $sql ) > 0;
}

function reuni_existeParecerResposta( $tpacod, $unicod, $prgcod )
{
	
	global $db;
	$tpacod = (integer) $tpacod;
	$unicod = (integer) $unicod;
	$prgcod = (integer) $prgcod;
	$sql = "
		select
			count(*)
		from reuni.parecer p
			inner join reuni.resposta r on r.rspcod = p.rspcod
		where
			p.tpacod = " . $tpacod . " and
			r.prgcod = " . $prgcod . " and
			r.unicod = '" . $unicod . "'
	";
	return $db->pegaUm( $sql ) > 0;
}
	
// preenhimento para reitoria
function reuni_podeEncaminharAprovacaoUnidade( $unicod )
{
	// respostas OBRIGATORIAS devem estar preenchidas
	global $db;
	$unicod = (integer) $unicod;
	$sql = "
		select
			count(*)
		from reuni.pergunta p
			left join reuni.resposta r on
				r.prgcod = p.prgcod and
				r.unicod = '" . $unicod . "'
		where
			(
				r.rspdsc is null or
				r.rspdsc = ''
			) and
			p.prgobr = true
	";
	return $db->pegaUm( $sql ) == 0;
}

// sesu para adhoc
function reuni_podeEncaminharAdhoc( $unicod )
{
	// RETIRADO em 07-11-05 10:08 -> todas as questões devem estar com parecer sesu preenchidos
	// o parecer global sesu deve ter sido preenchido
	return
		//reuni_parecerRespostaCompleto( $unicod, REUNI_PARECER_SESU ) &&
		reuni_parecerCompleto( $unicod, REUNI_PARECER_SESU_GLOBAL );
}

// adhoc para comissao
function reuni_podeEncaminharComissao( $unicod )
{
	// todas as questões devem estar com parecer adhoc preenchidos
	// o parecer global adhoc deve ter sido preenchido
	return
		reuni_parecerRespostaCompleto( $unicod, REUNI_PARECER_ADHOC ) &&
		reuni_parecerCompleto( $unicod, REUNI_PARECER_ADHOC_GLOBAL );
}

// comissao para sesu final
function reuni_podeEncaminharSesuFinal( $unicod )
{
	// NAO MAIS --> todas as questões devem estar com parece comissao preenchidos
	// o parecer global comissao deve ter sido preenchido
	return
		//reuni_parecerRespostaCompleto( $unicod, REUNI_PARECER_COMISSAO ) &&
		reuni_parecerCompleto( $unicod, REUNI_PARECER_COMISSAO_GLOBAL );
}

// sesu final para finalização
function reuni_podeFinalizar( $unicod )
{
	// o parecer global sesu final deve ter sido preenchido
	return reuni_parecerCompleto( $unicod, REUNI_PARECER_SESU_GLOBAL );
}

/******************
 * Pega array com perfis
 ******************/
function arrayPerfil(){
	global $db;
	
	$sql = sprintf("SELECT
					 pu.pflcod
					FROM
					 seguranca.perfilusuario pu
					 INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 	p.sisid = ".$_SESSION['sisid']."
					WHERE
					 pu.usucpf = '%s'
					ORDER BY
					 p.pflnivel",
				$_SESSION['usucpf']);
	return (array) $db->carregarColuna($sql,'pflcod');
}

function verificaPerfilConsulta(){
	$perfis = arrayPerfil();
	if(in_array(REUNI_PERFIL_IFES_CON, $perfis) && (count($perfis) < 2 ) ) return true;
	else return false;
}

function monta_grafico_acompanhamento( $vl0, $vl1, $vl2, $vl3, $vltotal, $na )
{
	if($vltotal == 0){
		$vl0p = 100;
		$vl1p = 0;
		$vl2p = 0;
		$vl3p = 0;
		
	}else{
		$vl0p = number_format($vl0*100/$vltotal,0,'.',',');
		$vl1p = number_format($vl1*100/$vltotal,0,'.',',');
		$vl2p = number_format($vl2*100/$vltotal,0,'.',',');
		$vl3p = number_format($vl3*100/$vltotal,0,'.',',');
	}		
	$url = 'acompanhaMonitora.php' .		
	'?vl0=' . $vl0 .
	'&vl1=' . $vl1 .
	'&vl2=' . $vl2 .
	'&vl3=' . $vl3 .
	'&vltotal=' . $vltotal .
	'&na=' . $na;

	return "
		<span	onmousemove=\"SuperTitleAjax( '" . $url . "', this );\"
				onmouseout=\"SuperTitleOff( this );\">
			<img src='../imagens/cor1.gif' style='height:10;width:".($vl1p/3).";border:1px solid #888888;border-right:0'><img src='../imagens/cor2.gif' style='height:10;width:".($vl2p/3).";border:1px solid #888888;border-right:0;border-left:0;border-right:0;border-left:0'><img src='../imagens/cor3.gif' style='height:10;width:".($vl3p/3).";border:1px solid #888888;border-left:0;border-right:0'><img src='../imagens/cor0.gif' style='height:10;width:".($vl0p/3).";border:1px solid #888888;border-left:0'>
		</span>";
}
function redirecionar( $modulo, $acao, $parametros = array() ) {
	$parametros = http_build_query( (array) $parametros, '', '&' );
	header( "Location: ?modulo=$modulo&acao=$acao&$parametros" );
	exit();
}

function EnviarArquivo($arquivo,$dados,$dir = 'cadedital'){
	global $db;	
	// obtém o arquivo
	$arquivo = $_FILES['arquivo'];
	if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
	}
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = "INSERT INTO public.arquivo 	(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
	values('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".$dados["arqdescricao"]."','".$arquivo["type"]."','".$arquivo["size"]."','".date('d/m/Y')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .") RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
	
	//Insere o registro na tabela reuni.anexos
	$sql = "INSERT INTO reuni.anexos (lanid,monid,tpaid,arqid,usucpf,anxdtinclusao)
	values(".$dados["lanid"].", ".$dados["monid"].",".$dados["tpaid"].",". $arqid .",'".$_SESSION["usucpf"]."','". date("Y-m-d H:i:s") ."');";
	$db->executar($sql);

	if(!is_dir('../../arquivos/reuni/'.floor($arqid/1000))) {
			mkdir(APPRAIZ.'/arquivos/reuni/'.floor($arqid/1000), 0777);
	}
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;		
	//$caminho = APPRAIZ.'teste/'.$arqid;	
	
	if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
		$db->rollback();
		echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
		exit;
	}	
	$db->commit();	
}

function DownloadArquivo($param){
		global $db;
		$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$param['arqid'];
        $arquivo = current($db->carregar($sql));
        $caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo['arqid']/1000) .'/'.$arquivo['arqid'];
        if ( !is_file( $caminho ) ) {
            $_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
        }
        $filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
        header( 'Content-type: '. $arquivo['arqtipo'] );
        header( 'Content-Disposition: attachment; filename='.$filename);
        readfile( $caminho );
        exit();
}
	
function DeletarDocumento($documento){
	global $db;
	$sql = "UPDATE reuni.anexos SET anxstatus = 'I' where anxid=".$documento["anxid"];
	$db->executar($sql);

	$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$documento["arqid"];
	$db->executar($sql);

	$db->commit();
}

?>
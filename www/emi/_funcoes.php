<?php

function emiRecuperaPerfil()
{
	global $db;
	
	$sql = "SELECT
				pu.pflcod
			FROM
				seguranca.perfilusuario pu
			INNER JOIN
				seguranca.perfil p ON p.pflcod = pu.pflcod
								  AND p.sisid = ".$_SESSION["sisid"]."
								  AND p.pflstatus = 'A'
			WHERE
				pu.usucpf = '".$_SESSION["usucpf"]."'";
	return $db->carregarColuna($sql);
}

function emiPossuiPerfil( $pflcods ){
	
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

function emiVerificaUf( $estuf ){
	
	global $db;
	
	$sql = "SELECT estuf FROM territorios.estado WHERE estuf = '{$estuf}'";
	
	return $db->pegaUm( $sql );
	
}

function emiVerificaResponsavel( $entid ){
	
	global $db;
	
	$sql = "SELECT entid FROM entidade.entidade WHERE entid = {$entid}";
	
	return $db->pegaUm( $sql );
	
}

function emiVerificaEntidade( $emeid ){
	
	global $db;
	
	$sql = "SELECT emeid FROM emi.ementidade WHERE emeid = {$emeid}";
	
	return $db->pegaUm( $sql );
	
}

function emiVerificaComponente( $comid ){
	
	global $db;
	
	$sql = "SELECT comid FROM emi.emcomponentes WHERE comid = {$comid}";
	
	return $db->pegaUm( $sql );
	
}

function emiVerificaPap( $papid ){
	
	global $db;
	
	$sql = "SELECT papid FROM emi.empap WHERE papid = {$papid}";
	
	return $db->pegaUm( $sql );
	
}

function emiVerificaGap( $papid ){
	
	global $db;
	
	$sql = "SELECT papid FROM emi.emgap WHERE papid = {$papid}";
	
	return $db->pegaUm( $sql );
	
}

function emiBuscaDadosSecretaria( $estuf ){
	
	global $db;
	
	$sql = "SELECT
				ee.entid as id,
				ee.entnome as nome
			FROM
				entidade.entidade ee
			INNER JOIN
				entidade.funcaoentidade ef ON ee.entid = ef.entid and funid = 6
			INNER JOIN
				entidade.endereco ed ON ee.entid = ed.entid
			WHERE
				estuf = '{$estuf}'";
	
	return $db->pegaLinha( $sql );
	
}

function emiBuscaQtdEscolas( $emeidPai ){
	
	global $db;
	
	$sql = "SELECT emeqtdescolas FROM emi.ementidade WHERE emeid = {$emeidPai}";
	
	return $db->pegaUm( $sql );
	
	
}

function emiListaMatrizPap( $papid, $acao, $disCadastro=false ) {
	
	global $db, $emiHabilitado, $totalGeralAxB;
	
	// inicializa a variável
	$totalGeralAxB = 0;
	
	// Verifica se a opção de cadastro de crítica deve ser exibida.
	$mostraCritica = emiExibeCritica();
	
	$sql = "SELECT
				mdoid, 
				undddsc, 
				itfdsc, 
				mdoespecificacao, 														 
				mdoqtd, 
				mdovalorunitario, 
				mdototal,
				mdoflagalterado
			FROM 
				emi.emmatrizdistribuicaoorcamentar em
			INNER JOIN
				cte.unidademedidadetalhamento cu ON cu.unddid = em.unddid
			INNER JOIN
				emi.emitemfinanciavel ei ON ei.itfid = em.itfid 
			WHERE 
				papid = {$papid} AND mdostatus = 'A'
			ORDER BY
				itfdsc";
	
	$dados = $db->carregar( $sql );
	
	if( $dados ){
		
		for( $i = 0; $i < count( $dados ); $i++ ){
			
			$img = ($emiHabilitado && !$disCadastro)
								  ? "<img src='/imagens/alterar.gif' style='cursor: pointer;' title='Editar' onclick='alterarItemMatriz({$dados[$i]["mdoid"]}, \"{$acao}\");'>
									 <img src='/imagens/excluir.gif' style='cursor: pointer;' title='Excluir' onclick='excluiItemMatriz({$dados[$i]["mdoid"]}, \"{$acao}\");'>
									 ".(($mostraCritica) ? "<img src='/imagens/editar_nome.gif' style='cursor: pointer;' title='Crítica' onclick='preencheCriticaMatriz({$dados[$i]["mdoid"]});'>" : "")
								  : "<img src='/imagens/alterar_01.gif'/>
								  	 <img src='/imagens/excluir_01.gif'/>";
			
			$cor = ($i % 2) ? "#f4f4f4": "#e0e0e0";
			
			// Recupera se a crítica realizada validou ou não o item da matriz.
			$crmvalidado 	= $db->pegaUm("SELECT crmvalidado FROM emi.critricamatriz WHERE mdoid = ".$dados[$i]["mdoid"]);
			$crmobs 		= $db->pegaUm("SELECT crmobs FROM emi.critricamatriz WHERE mdoid = ".$dados[$i]["mdoid"]);
			
			
			// calcula o total geral do campo 'Total (R$) A x B'
			$totalGeralAxB += $dados[$i]["mdototal"];
			
			// Mostra o texto em vermelho se não tiver sido validado.
			if($dados[$i]['mdoflagalterado'] == "t"){
				$item 			= "<font color='blue'>".$dados[$i]["itfdsc"]."</font>" 			;
				$especificacao 	= "<font color='blue'>".$dados[$i]["mdoespecificacao"]."</font>" ;
				$unidade 		= "<font color='blue'>".$dados[$i]["undddsc"]."</font>" 			;
				$quantidade 	= "<font color='blue'>".$dados[$i]["mdoqtd"]."</font>" 			;
				$valorunitario 	= "<font color='blue'>".number_format($dados[$i]["mdovalorunitario"], 2, ",", ".")."</font>" ;
				$total 			= "<font color='blue'>".number_format($dados[$i]["mdototal"], 2, ",", ".")."</font>" 		;				
			} else {
				$item 			= ($crmvalidado == "f") ? "<font color='red'>".$dados[$i]["itfdsc"]."</font>" 			: (($crmobs == "t") ? "<font color='orange'>".$dados[$i]["itfdsc"]."</font>" : $dados[$i]["itfdsc"]);
				$especificacao 	= ($crmvalidado == "f") ? "<font color='red'>".$dados[$i]["mdoespecificacao"]."</font>" : (($crmobs == "t") ? "<font color='orange'>".$dados[$i]["mdoespecificacao"]."</font>" : $dados[$i]["mdoespecificacao"]);
				$unidade 		= ($crmvalidado == "f") ? "<font color='red'>".$dados[$i]["undddsc"]."</font>" 			: (($crmobs == "t") ? "<font color='orange'>".$dados[$i]["undddsc"]."</font>" : $dados[$i]["undddsc"]);
				$quantidade 	= ($crmvalidado == "f") ? "<font color='red'>".$dados[$i]["mdoqtd"]."</font>" 			: (($crmobs == "t") ? "<font color='orange'>".$dados[$i]["mdoqtd"]."</font>" : $dados[$i]["mdoqtd"]);
				
				$valorunitario 	= ($crmvalidado == "f") ? "<font color='red'>".number_format($dados[$i]["mdovalorunitario"], 2, ",", ".")."</font>" : (($crmobs == "t") ? "<font color='orange'>".number_format($dados[$i]["mdovalorunitario"], 2, ",", ".")."</font>" : number_format($dados[$i]["mdovalorunitario"], 2, ",", "."));
				$total 			= ($crmvalidado == "f") ? "<font color='red'>".number_format($dados[$i]["mdototal"], 2, ",", ".")."</font>" 		: (($crmobs == "t") ? "<font color='orange'>".number_format($dados[$i]["mdototal"], 2, ",", ".")."</font>" : number_format($dados[$i]["mdototal"], 2, ",", "."));
			}
			
			print "<tr bgColor='{$cor}'>
					   <td align='center' width='10%'>{$item}</td>
					   <td align='justify' width='30%'>{$especificacao}</td>
					   <td align='center' width='10%'>{$unidade}</td>
					   <td align='right' width='10%'>{$quantidade}</td>
					   <td align='right' width='15%'>" . $valorunitario . "</td>
					   <td align='right' width='15%'>" . $total . "</td>
					   <td align='center' width='10%'>
					       {$img}
					   </td>
				   </tr>";
			
		}
		
	}
	
}

function emiListaMatrizGap( $papid, $acao, $disCadastro=false ) {
	
	global $db, $emiHabilitado, $totalGeralAxB;
	
	// inicializa a variável
	$totalGeralAxB = 0;
	
	// Verifica se a opção de cadastro de crítica deve ser exibida.
	$mostraCritica = emiExibeCritica();
	
	$sql = "SELECT
				mdoid, 
				undddsc, 
				itfdsc, 
				mdoespecificacao, 														 
				mdoqtd, 
				mdovalorunitario, 
				mdototal,
				mdoflagalterado
			FROM 
				emi.emmatrizdistribuicaoorcamentargap em
			INNER JOIN
				cte.unidademedidadetalhamento cu ON cu.unddid = em.unddid
			INNER JOIN
				emi.emitemfinanciavel ei ON ei.itfid = em.itfid 
			WHERE 
				papid = {$papid} AND mdostatus = 'A'
			ORDER BY
				itfdsc";
	
	$dados = $db->carregar( $sql );
	
	if( $dados ){
		
		for( $i = 0; $i < count( $dados ); $i++ ){
			
			$img = ($emiHabilitado && !$disCadastro)
								  ? "<img src='/imagens/alterar.gif' style='cursor: pointer;' title='Editar' onclick='alterarItemMatrizGap({$dados[$i]["mdoid"]}, \"{$acao}\");'>
									 <img src='/imagens/excluir.gif' style='cursor: pointer;' title='Excluir' onclick='excluiItemMatrizGap({$dados[$i]["mdoid"]}, \"{$acao}\");'>
									 ".(($mostraCritica) ? "<img src='/imagens/editar_nome.gif' style='cursor: pointer;' title='Crítica' onclick='preencheCriticaMatrizGap({$dados[$i]["mdoid"]});'>" : "")
								  : "<img src='/imagens/alterar_01.gif'/>
								  	 <img src='/imagens/excluir_01.gif'/>";
			
			$cor = ($i % 2) ? "#f4f4f4": "#e0e0e0";
			
			// Recupera se a crítica realizada validou ou não o item da matriz.
			$crmvalidado 	= $db->pegaUm("SELECT crmvalidado FROM emi.critricamatriz WHERE mdoid = ".$dados[$i]["mdoid"]);
			$crmobs 		= $db->pegaUm("SELECT crmobs FROM emi.critricamatriz WHERE mdoid = ".$dados[$i]["mdoid"]);
			
			
			// calcula o total geral do campo 'Total (R$) A x B'
			$totalGeralAxB += $dados[$i]["mdototal"];
			
			// Mostra o texto em vermelho se não tiver sido validado.
			if($dados[$i]['mdoflagalterado'] == "t"){
				$item 			= "<font color='blue'>".$dados[$i]["itfdsc"]."</font>" 			;
				$especificacao 	= "<font color='blue'>".$dados[$i]["mdoespecificacao"]."</font>" ;
				$unidade 		= "<font color='blue'>".$dados[$i]["undddsc"]."</font>" 			;
				$quantidade 	= "<font color='blue'>".$dados[$i]["mdoqtd"]."</font>" 			;
				$valorunitario 	= "<font color='blue'>".number_format($dados[$i]["mdovalorunitario"], 2, ",", ".")."</font>" ;
				$total 			= "<font color='blue'>".number_format($dados[$i]["mdototal"], 2, ",", ".")."</font>" 		;				
			} else {
				$item 			= ($crmvalidado == "f") ? "<font color='red'>".$dados[$i]["itfdsc"]."</font>" 			: (($crmobs == "t") ? "<font color='orange'>".$dados[$i]["itfdsc"]."</font>" : $dados[$i]["itfdsc"]);
				$especificacao 	= ($crmvalidado == "f") ? "<font color='red'>".$dados[$i]["mdoespecificacao"]."</font>" : (($crmobs == "t") ? "<font color='orange'>".$dados[$i]["mdoespecificacao"]."</font>" : $dados[$i]["mdoespecificacao"]);
				$unidade 		= ($crmvalidado == "f") ? "<font color='red'>".$dados[$i]["undddsc"]."</font>" 			: (($crmobs == "t") ? "<font color='orange'>".$dados[$i]["undddsc"]."</font>" : $dados[$i]["undddsc"]);
				$quantidade 	= ($crmvalidado == "f") ? "<font color='red'>".$dados[$i]["mdoqtd"]."</font>" 			: (($crmobs == "t") ? "<font color='orange'>".$dados[$i]["mdoqtd"]."</font>" : $dados[$i]["mdoqtd"]);
				
				$valorunitario 	= ($crmvalidado == "f") ? "<font color='red'>".number_format($dados[$i]["mdovalorunitario"], 2, ",", ".")."</font>" : (($crmobs == "t") ? "<font color='orange'>".number_format($dados[$i]["mdovalorunitario"], 2, ",", ".")."</font>" : number_format($dados[$i]["mdovalorunitario"], 2, ",", "."));
				$total 			= ($crmvalidado == "f") ? "<font color='red'>".number_format($dados[$i]["mdototal"], 2, ",", ".")."</font>" 		: (($crmobs == "t") ? "<font color='orange'>".number_format($dados[$i]["mdototal"], 2, ",", ".")."</font>" : number_format($dados[$i]["mdototal"], 2, ",", "."));
			}
			
			print "<tr bgColor='{$cor}'>
					   <td align='center' width='10%'>{$item}</td>
					   <td align='justify' width='30%'>{$especificacao}</td>
					   <td align='center' width='10%'>{$unidade}</td>
					   <td align='right' width='10%'>{$quantidade}</td>
					   <td align='right' width='15%'>" . $valorunitario . "</td>
					   <td align='right' width='15%'>" . $total . "</td>
					   <td align='center' width='10%'>
					       {$img}
					   </td>
				   </tr>";
			
		}
		
	}
	
}

function emiBuscaDadosCoordenador( $emeid ){
	
	global $db;
	
	if(!$emeid) {
		die("<script>
				alert('Problemas com variaveis');
				window.location='emi.php?modulo=inicio&acao=C';
			 </script>");
	}
	
	$sql = "SELECT
				ee.entid, 
				entnome 
			FROM
				entidade.entidade ee
			INNER JOIN
				emi.emresponsavel er ON er.entid = ee.entid
			INNER JOIN
				emi.ementidade em ON em.rspid = er.rspid
			WHERE
				em.emeid = {$emeid}";
	
	return $db->pegaLinha( $sql );
	
}

function emiBuscaDadosArquivo( $emeid , $dettipo = "P"){
	
	global $db;
	
	insereDetalheEntidade($emeid,$dettipo);
	
	$sql = "SELECT
				ar.arqid, 
				ar.arqnome,
				ar.arqdescricao,
				ar.arqextensao 
			FROM
				emi.detalheentidade dt
			INNER JOIN
				public.arquivo ar ON ar.arqid=dt.arqid
			WHERE
				dt.emeid = {$emeid}
			and
				dt.dettipo = '$dettipo'";
	
	return $db->pegaLinha( $sql );
	
}

function emiBuscaPap( $emeid ){
	
	global $db;
	
	$sql = "SELECT papid FROM emi.empap WHERE emeid = {$emeid} AND papstatus='A' AND papexercicio = '{$_SESSION['exercicio']}'";
	
	return $db->pegaUm( $sql );
	
}

function emiBuscaGap( $emeid ){
	
	global $db;
	
	$sql = "SELECT papid FROM emi.emgap WHERE emeid = {$emeid} AND papstatus='A' AND papexercicio = '{$_SESSION['exercicio']}'";
	
	return $db->pegaUm( $sql );
	
}

function emiBuscaPapSecretaria( $emeid ){
	
	global $db;
	
	$sql = "SELECT 
				papid 
			FROM 
				emi.empap  emp
			INNER JOIN
				emi.ementidade ent ON emp.emeid = ent.emeid
			WHERE 
				emeidpai = {$emeid} 
			AND 
				papstatus='A' 
			AND 
				papexercicio = '{$_SESSION['exercicio']}'";
	
	return $db->pegaUm( $sql );
	
}

function emiBuscaEscolasCadastradas( $entid ){
	
	global $db;
	
	$sql = "SELECT
				emeid,
				ee.entcodent || ' - ' || entnome || ' ( ' || tm.mundescricao || ' )' as entnome
			FROM
				emi.ementidade em
			INNER JOIN
				entidade.entidade ee ON ee.entid = em.entid
			INNER JOIN
				entidade.endereco eed ON eed.entid = ee.entid
			LEFT JOIN
				territorios.municipio tm ON tm.muncod = eed.muncod 
			WHERE
				emeidpai = {$entid} AND emestatus = 'A'
			and
				emiexercicio = '{$_SESSION['exercicio']}'
			ORDER BY
				emeid";
	
	return $db->carregar( $sql );
	
}

function emiVerificaValidacaoCritica( $emeid ) {
	global $db;
	
	$retorno = true;
	
	// Dimensões
	$sql = "SELECT 
				emdid as id,
				dimcod as codigo,
				dimdsc as descricao
			FROM 
				emi.emdimensao ed
			INNER JOIN 
				cte.dimensao cd ON ed.dimid = cd.dimid";
	$dadosDimensao = $db->carregar( $sql );
	
	if ( $dadosDimensao ) {
		for( $i = 0; $i < count( $dadosDimensao ); $i++ ) {
			// Linhas de Ação
			$sql = "SELECT 
						lacid as id, 
						laccod as codigo, 
						lacdsc as descricao
					FROM 
						emi.emlinhaacao
					WHERE
						emdid = {$dadosDimensao[$i]["id"]} AND
						tppid = ".EMI_TIPO_ENTIDADE_ESCOLA;
			$dadosLinhaAcao = $db->carregar( $sql );
			
			if ( $dadosLinhaAcao ){
				for( $j = 0; $j < count( $dadosLinhaAcao ); $j++ ) {
					// Componentes
					$sql = "SELECT
								comid as id,
								comcod as codigo,
								comdsc as descricao
							FROM
								emi.emcomponentes
							WHERE
								lacid = {$dadosLinhaAcao[$j]["id"]}
							ORDER BY
								codigo";
					$dadosComponentes = $db->carregar( $sql );
		
					if ( $dadosComponentes ){
						for( $k = 0; $k < count( $dadosComponentes ); $k++ ) {
							// PAPS
							$sql = "SELECT
										papid as id,
										trim(papcaoatividade) as atividade,
										trim(papmeta) as meta
									FROM
										emi.empap
									WHERE
										comid = {$dadosComponentes[$k]["id"]} AND
										emeid = {$emeid} AND
										papstatus = 'A'";
							$dadosPap = $db->carregar( $sql );
							
							if($dadosPap) {
								for( $l = 0; $l < count( $dadosPap ); $l++ ) {
									// Recupera se a crítica realizada validou ou não as Ações/Atividades.
									$crpvalidado = $db->pegaUm("SELECT crpvalidado FROM emi.critricapap WHERE papid = ".$dadosPap[$l]["id"]);
									
									if($crpvalidado == "f") {
										return false;
										exit;
									} else {
										// recupera se foi feita alguma observação
											$crpobs	= $db->pegaUm("SELECT crpobs FROM emi.critricapap WHERE papid = ".$dadosPap[$l]["id"]);
											if($crpobs == "t") $retorno = "observacao";
									} 
									
									$sql = "SELECT
												mdoid
											FROM 
												emi.emmatrizdistribuicaoorcamentar em
											INNER JOIN
												cte.unidademedidadetalhamento cu ON cu.unddid = em.unddid
											INNER JOIN
												emi.emitemfinanciavel ei ON ei.itfid = em.itfid 
											WHERE 
												papid = {$dadosPap[$l]["id"]} AND mdostatus = 'A'
											ORDER BY
												itfdsc";
									$dadosMatriz = $db->carregar( $sql );
									
									if( $dadosMatriz[0] ) {
										for( $m = 0; $m < count( $dadosMatriz ); $m++ ) {
											// Recupera se a crítica realizada validou ou não o item da matriz.
											$crmvalidado = $db->pegaUm("SELECT crmvalidado FROM emi.critricamatriz WHERE mdoid = ".$dadosMatriz[$m]["mdoid"]);
											
											if($crmvalidado == "f") {
												return false;
												exit;
											} else {
												// recupera se foi feita alguma observação
												$crmobs = $db->pegaUm("SELECT crmobs FROM emi.critricamatriz WHERE mdoid = ".$dadosMatriz[$m]["mdoid"]);
												if($crmobs == "t") $retorno = "observacao";
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	return $retorno;
}

function emiExibeCritica() {
	global $db;
	
	$retorno = false;
	
	// Testa se é 'Cadastrador' ou 'Analista COEM' e verifica o estado do documento para mostrar botão de cadastro de crítica.
	if( emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) || emiPossuiPerfil(EMI_PERFIL_ANALISTACOEM) || emiPossuiPerfil(EMI_PERFIL_SUPERUSER) || emiPossuiPerfil(EMI_PERFIL_ADMINISTRADOR) ) {
		
		$docid = emiPegarDocid( $_SESSION["emi"]["emeidPai"] );
		
		// O teste com o aedid é para verificar se o documento já foi enviado para preenchimento ou para correção.
		// Enviado para preenchimento: aedid(245) = esdidorigem(62) => esdiddestino(63)
		// Enviado para correção: 	   aedid(246) = esdidorigem(63) => esdiddestino(62)
		if($docid) {
			if( emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) )
				$aedid = "aedid = 246";
			if( emiPossuiPerfil(EMI_PERFIL_ANALISTACOEM) )
				$aedid = "aedid = 245";
			if( emiPossuiPerfil(EMI_PERFIL_SUPERUSER) || emiPossuiPerfil(EMI_PERFIL_ADMINISTRADOR) )
				$aedid = "aedid in (245,246)";
			
			$sql = "SELECT
						count(*)
					FROM
						workflow.historicodocumento
					WHERE
						{$aedid} AND 
						docid = {$docid}";
			
			if( (integer)$db->pegaUm($sql) > 0 ) $retorno = true;
		}
		
		// se tiver observação, cadastrador vê mesmo assim a crítica
		if( emiPossuiPerfil(EMI_PERFIL_CADASTRADOR) && !$retorno ) {
			if( (string) emiVerificaValidacaoCritica( $_SESSION["emi"]["emeidPai"] ) == 'observacao' )
				$retorno = true;
		}
	}
	
	return $retorno;
}

function emiMontaPapEscola( $emeid, $tipo = "escola" ){
	
	global $db, $emiDisabled, $emiSomenteLeitura, $emiHabilitado;
	
	$tppid = $tipo == "escola" ? EMI_TIPO_ENTIDADE_ESCOLA : EMI_TIPO_ENTIDADE_SEC;
	
	// Verifica se a opção de cadastro de crítica deve ser exibida.
	$mostraCritica = emiExibeCritica();
	
	// Marca a flag como 'true' se o estado atual for 'Correção de Aprovados'
	$correcaoAprovados = (emiPegarEstadoAtual($emeid) == EMI_CORRECAO_APROVADOS) ? true : false;
	
	// Dimensões
	$sql = "SELECT 
				emdid as id,
				dimcod as codigo,
				dimdsc as descricao
			FROM 
				emi.emdimensao ed
			INNER JOIN 
				cte.dimensao cd ON ed.dimid = cd.dimid";

	$dadosDimensao = $db->carregar( $sql );
	
	if ( $dadosDimensao ){
		
		
		for( $i = 0; $i < count( $dadosDimensao ); $i++ ){
						
			/*** Verifica se na dimensão existe algum pap aprovado ***/
			if($correcaoAprovados)
			{
				
				$sql = "SELECT 	
							count(pap.papid)
						FROM
						   	emi.emlinhaacao ela
						INNER JOIN
							emi.emcomponentes ecp ON ecp.lacid = ela.lacid
						INNER JOIN
							emi.empap pap ON pap.comid = ecp.comid AND
											 emeid in ({$emeid}) AND
											 papstatus = 'A' AND papflagaprovado = 't' AND pap.papexercicio = '{$_SESSION['exercicio']}'
						WHERE
							ela.emdid = {$dadosDimensao[$i]["id"]} AND
							ela.tppid = {$tppid}";

				if( $db->pegaUm($sql) < 1)
				{
					continue;
				}
				
			}
			
			$cor = ( $i % 2 ) ? "#e0e0e0" : "#f4f4f4";
			
			print "<tr bgColor='{$cor}' >"
				. "	   <td> {$dadosDimensao[$i]["codigo"]}. {$dadosDimensao[$i]["descricao"]} </td>"
				. "	   <td colspan='5'>"
				. "		   <table width='100%' cellspacing='0' cellpadding='4'  style='border-collapse: collapse; border: 1px solid #ccc;'>";
	
			// Linhas de Ação
			$sql = "SELECT 
						lacid as id, 
						laccod as codigo, 
						lacdsc as descricao
					FROM 
						emi.emlinhaacao
					WHERE
						emdid = {$dadosDimensao[$i]["id"]} AND
						tppid = {$tppid}";
			
			$dadosLinhaAcao = $db->carregar( $sql );
			
			if ( $dadosLinhaAcao ){
				for( $j = 0; $j < count( $dadosLinhaAcao ); $j++ ){
					
					/*** Verifica se na linha de ação existe algum pap aprovado ***/
					if($correcaoAprovados)
					{
						$sql = "SELECT 	
									count(pap.papid)
								FROM
									emi.emcomponentes ecp
								INNER JOIN
									emi.empap pap ON pap.comid = ecp.comid AND
													 emeid in ({$emeid}) AND
													 papstatus = 'A' AND papflagaprovado = 't' AND pap.papexercicio = '{$_SESSION['exercicio']}'
								WHERE
									ecp.lacid = {$dadosLinhaAcao[$j]["id"]}";
						if( $db->pegaUm($sql) < 1)
						{
							continue;
						}
					}
					
					print "<tr>"
						. "    <td align='justify' width='27%' style='border-collapse: collapse; border-right: 1px solid #ccc; border-bottom: 1px solid #ccc;'>" 
						. 		    $dadosLinhaAcao[$j]["codigo"] . ". " . $dadosLinhaAcao[$j]["descricao"] 
						. "	   </td>"
						. "	   <td>"
						. "        <table border=0 width='100%' cellspacing='0' cellpadding='4' style='border-collapse: collapse; border: 1px solid #ccc;'>";
					
					// Componentes
					$sql = "SELECT
								comid as id,
								comcod as codigo,
								comdsc as descricao
							FROM
								emi.emcomponentes
							WHERE
								lacid = {$dadosLinhaAcao[$j]["id"]}
							ORDER BY
								codigo";
					
					$dadosComponentes = $db->carregar( $sql );
		
					if ( $dadosComponentes ){
						for( $k = 0; $k < count( $dadosComponentes ); $k++ ){
							
							$papFlagAprovado = '';
							/*** Verifica se no componente existe algum pap aprovado ***/
							if($correcaoAprovados)
							{
								$sql = "SELECT 	
											count(pap.papid)
										FROM
											emi.empap pap
										WHERE
											pap.comid = {$dadosComponentes[$k]["id"]}
											AND emeid in ({$emeid}) 
											AND papstatus = 'A' 
											AND papflagaprovado = 't'
											AND pap.papexercicio = '{$_SESSION['exercicio']}'";
								if( $db->pegaUm($sql) < 1)
								{
									continue;
								}
								else
								{
									$papFlagAprovado = "AND papflagaprovado = 't'";
								}
							}
							
							print "<tr>"
								. "    <td align='justify' width='37%' style='border-collapse: collapse; border-right: 1px solid #ccc; border-bottom: 1px solid #ccc;'>" 
								. 		    $dadosComponentes[$k]["codigo"] . ". " . $dadosComponentes[$k]["descricao"] 
								. "	   </td>";
								
							// PAPS
							$sql = "SELECT
										papid as id,
										trim(papcaoatividade) as atividade,
										trim(papmeta) as meta,
										papflagalterado
									FROM
										emi.empap
									WHERE
										comid = {$dadosComponentes[$k]["id"]} AND
										emeid in ({$emeid}) AND
										papstatus = 'A' AND
										papexercicio = '{$_SESSION['exercicio']}'  
										{$papFlagAprovado}";
							
							$dadosPap = $db->carregar( $sql );
							
							
							/*** testa se eh cadastrador e o estado do documento ***/
							if( in_array(EMI_PERFIL_CADASTRADOR, emiRecuperaPerfil()) )
							{
								$estadoAtual = emiPegarEstadoAtual($emeid);
								
								if( $estadoAtual == EMI_NAO_INICIADO || $estadoAtual == EMI_EM_PRENCHIMENTO )
								{
									$disCadastro = false;
								}
								else
								{
									$disCadastro = true;
								}
							}
							else
							{
								$disCadastro = false;
							}

							
							print "	   <td colspan=3 style='border-collapse: collapse; border-bottom: 1px solid #ccc;'>"
								. "        <table class='listagem' width='100%' cellspacing='0' cellpadding='4' >"
								. "			   <tr>"
								. "				   <th class=''>Ação/Atividade</th>"
								. " 			   <th class=''>Meta</th>"
								. "				   <th class=''>Matriz</th>"
								. "				   ".(($mostraCritica && !$disCadastro && !$correcaoAprovados) ? "<th class=''>Crítica</th>" : "")
								. "			   </tr>";
							
							if ( !$dadosPap ){
								print "<tr>"
									. "	   <td width='40%' style='border-collapse: collapse; border-right: 1px solid #ccc; border-bottom: 1px solid #ccc; color:#cc0000;'>Não informado</td>"
									. "	   <td width='40%' style='border-collapse: collapse; border-right: 1px solid #ccc; border-bottom: 1px solid #ccc; color:#cc0000;'>Não informado</td>"
									. "	   <td width='5%' align='center' style='border-collapse: collapse; border-right: 1px solid #ccc; border-bottom: 1px solid #ccc;'>-</td>"
									. "    ".(($mostraCritica && !$disCadastro && !$correcaoAprovados) ? "<td width='5%' align='center' style='border-collapse: collapse; border-right: 1px solid #ccc; border-bottom: 1px solid #ccc;'>-</td>" : "")
									. "</tr>";
									
							}else{
								
								for( $l = 0; $l < count( $dadosPap ); $l++ ) {
									
									// Recupera se a crítica realizada validou ou não a Ação/Atividade.
									$crpvalidado = $db->pegaUm("SELECT crpvalidado FROM emi.critricapap WHERE papid = ".$dadosPap[$l]["id"]);
									
									$boValidacao = true;
									// Mostra o texto em vermelho se não tiver sido validada.
									if($crpvalidado == "f") {
										$boValidacao = false;
									} else {
										$crpobs	= $db->pegaUm("SELECT crpobs FROM emi.critricapap WHERE papid = ".$dadosPap[$l]["id"]);
										if($crpobs == "t") $boValidacao = "observacao";
										
										$sql = "SELECT
													mdoid
												FROM 
													emi.emmatrizdistribuicaoorcamentar em
												INNER JOIN
													cte.unidademedidadetalhamento cu ON cu.unddid = em.unddid
												INNER JOIN
													emi.emitemfinanciavel ei ON ei.itfid = em.itfid 
												WHERE 
													papid = {$dadosPap[$l]["id"]} AND mdostatus = 'A'
												ORDER BY
													itfdsc";
										$dadosMatriz = $db->carregar( $sql );
										
										if( $dadosMatriz[0] ) {
											for( $m = 0; $m < count( $dadosMatriz ); $m++ ) {
												// Recupera se a crítica realizada validou ou não o item da matriz.
												$crmvalidado = $db->pegaUm("SELECT crmvalidado FROM emi.critricamatriz WHERE mdoid = ".$dadosMatriz[$m]["mdoid"]);
												
												if($crmvalidado == "f") { 
													$boValidacao = false;
												} else {
													$crmobs = $db->pegaUm("SELECT crmobs FROM emi.critricamatriz WHERE mdoid = ".$dadosMatriz[$m]["mdoid"]);
													if($crmobs == "t") $boValidacao = "observacao";
												}
											}
										}
									}
									
									
									
									
									
									if( $boValidacao ) {
										$atividade 	= 	$dadosPap[$l]["atividade"];
										$meta		=	$dadosPap[$l]["meta"];
									}
									if( (string) $boValidacao == 'observacao' ) {
										$atividade 	= 	"<font color='orange'>".$dadosPap[$l]["atividade"]."</font>";
										$meta		=	"<font color='orange'>".$dadosPap[$l]["meta"]."</font>";
									}
									if( !$boValidacao ) {
										$atividade 	= 	"<font color='red'>".$dadosPap[$l]["atividade"]."</font>";
										$meta		=	"<font color='red'>".$dadosPap[$l]["meta"]."</font>";
									}
									
									if( $dadosPap[$l]["papflagalterado"] == "t" ) {
										$atividade 	= 	"<font color='blue'>".$dadosPap[$l]["atividade"]."</font>";
										$meta		=	"<font color='blue'>".$dadosPap[$l]["meta"]."</font>";
									}
									
									
									print "<tr>"
										. "    <td width='40%' align='justify' valign='top' style='border-collapse: collapse; border-right: 1px solid #ccc; border-bottom: 1px solid #ccc;'>"
										. 		    $atividade
										. "	   </td>"
										. "	   <td width='40%' align='justify' valign='top' style='border-collapse: collapse; border-right: 1px solid #ccc; border-bottom: 1px solid #ccc;'>{$meta}</td>"
										. "	   <td width='5%' align='center' style='border-collapse: collapse; border-right: 1px solid #ccc; border-bottom: 1px solid #ccc;'>"
										. "		   <img src='/imagens/gif_inclui.gif' style='cursor: pointer;' onclick='preencheMatriz({$dadosPap[$l]["id"]}, {$tppid});' title='Preencher Matriz'>"
										. "	   </td>"
										. "    ".(($mostraCritica && !$disCadastro && !$correcaoAprovados) ? "<td width='5%' align='center' style='border-collapse: collapse; border-right: 1px solid #ccc; border-bottom: 1px solid #ccc;'>" : "")
										. "        ".(($mostraCritica && !$disCadastro && !$correcaoAprovados) ? "<img src='/imagens/editar_nome.gif' style='cursor: pointer;' onclick='preencheCriticaPap({$dadosPap[$l]["id"]});' title='Preencher Crítica'>" : "")
										. "	   ".(($mostraCritica && !$disCadastro && !$correcaoAprovados) ? "</td>" : "")
										. "</tr>";
									
								}
								
							}
							
							print "			   <tr>"
								. "    			   <td align='right' colspan='".(($mostraCritica && !$disCadastro) ? "4" : "3")."'>";
								
							
								
							if( $emiHabilitado && !$disCadastro && !$correcaoAprovados ){
								print "				   <img src='/imagens/alterar.gif' align='absmiddle'> <a onclick='insereAcaoPap({$dadosComponentes[$k]["id"]}, {$tppid});' style='cursor:pointer;'>Inserir Ação/Atividade e Meta</a>";
							}else{
								print "				   <img src='/imagens/alterar_01.gif' align='absmiddle'> Inserir Ação/Atividade e Meta";
							}
							print "				   </td>"
								. "			   </tr>"
								. "        </table>"
								. "    </td>";
						
							print "</tr>";
							
						}
					
					}
					
					print "		   </table>" 
						. "	   </td>"
						. "</tr>";
					
				}
			
			}	
			
			print "		   </table>"
				. "	   </td>"
				. "</tr>";
				
		}
	
	}
	
}

function emiMontaGapEscola( $emeid, $tipo = "escola" ){
	
	global $db, $emiDisabled, $emiSomenteLeitura, $emiHabilitado;
	
	$tppid = $tipo == "escola" ? EMI_TIPO_ENTIDADE_ESCOLA : EMI_TIPO_ENTIDADE_SEC;
	
	$arrMacroCampos = pegaMacroCampos();
	$n = 1;
	foreach($arrMacroCampos as $mc)
	{
		$cor = $n%2 == 0 ? "#e9e9e9" : "";
		$cor2 = $n%2 == 1 ? "#e9e9e9" : "#f5f5f5";
		echo "<tr bgcolor='$cor' >";
			echo "<td>$n - {$mc['mcpdsc']}</td>";
			echo "<td>";
				$arrGaps = emiPegaGapsPorMacro($emeid,$mc['mcpid']);
				echo "<table class='tabela' width='100%' cellspacing=\"2\" cellpadding=\"4\" align=\"center\"  >";
					echo "<tr bgcolor=\"#D5D5D5\" >";
						echo "<td align='center' ><b>Ação/Atividade</b></td>";
						echo "<td align='center' width=\"40%\" ><b>Meta</b></td>";
						echo "<td align='center' width=\"10%\"  ><b>Matriz</b></td>";
						echo "<td align='center' width=\"10%\" ><b>Crítica</b></td>";
					echo "</tr>";
					$y = 0;
					foreach($arrGaps as $gap)
					{
						$cor3 = $y%2 == 1 ? $cor2 : "#ffffff";
						echo "<tr bgcolor=\"$cor3\" >";
							echo "<td align='center' >{$gap['papcaoatividade']}</td>";
							echo "<td align='center' >{$gap['papmeta']}</td>";
							echo "<td align='center' ><img title=\"Preencher Matriz\" onclick=\"preencheMatrizGAP({$gap['papid']}, $tppid);\" style=\"cursor: pointer;\" src=\"/imagens/gif_inclui.gif\"></td>";
							echo "<td align='center' ><img title=\"Preencher Crítica\" onclick=\"preencheCriticaGap({$gap['papid']});\" style=\"cursor: pointer;\" src=\"/imagens/editar_nome.gif\"></td>";
						echo "</tr>";
						$y++;
					}
					if(count($arrGaps) == 0){
						$cor3 = $y%2 == 1 ? $cor2 : "#f5f5f5";
						echo "<tr bgcolor=\"$cor3\" >";
							echo "<td align='center' ><span style=\"color:#990000\" >Não informado.</span></td>";
							echo "<td align='center' ><span style=\"color:#990000\" >Não informado.</span></td>";
							echo "<td align='center' >-</td>";
							echo "<td align='center' >-</td>";
						echo "</tr>";
					}
					echo "<tr bgcolor=\"#D5D5D5\" >";
						echo "<td colspan='4' align='right' ><img style=\"cursor:pointer;background-color:#FFFFFF\" onclick='addAcaoAtividade($emeid,{$mc['mcpid']},$tppid)' align=\"absmiddle\" src=\"/imagens/alterar.gif\"> <span style=\"cursor:pointer;\" onclick='addAcaoAtividade($emeid,{$mc['mcpid']},$tppid)' >Inserir Ação/Atividade e Meta</span></td>";
					echo "</tr>";
					exibeProfissionais($mc['mcpid'],$emeid);
				echo "</table>";
			echo "</td>";
		echo "</tr>";
		$n++;
	}
	
}

function exibeProfissionais($mcpid,$emeid)
{
	global $db; ?>
	
	<tr>
		<td style="font-weight:bold" bgcolor="#DCDCDC" align="center" colspan="5" >Número de Profissionais Envolvidos</td>
	</tr>
	<tr bgcolor="#e9e9e9" >
		<td style="font-weight:bold" align="center" >Professor(a)</td>
		<td style="font-weight:bold" align="center" >Equipe Direção</td>
		<td style="font-weight:bold" align="center" >Outros Profissionais</td>
		<td style="font-weight:bold" align="center" colspan="2" >Total</td>
	</tr>
	<?php $arrProf = pegaProfissionaisGAP($mcpid,$emeid);?>
	<tr>
		<td align="center" ><?php $num_prof_{$mcpid}   = number_format($arrProf['preqtdprofessor'],'',2,'.'); echo campo_texto("num_prof_{$mcpid}","S","S","",10,20,"[.###]","","right","","","","calculaTotalProfissionais($mcpid)",$num_prof_{$mcpid}) ?></td>
		<td align="center" ><?php $num_equipe_{$mcpid} = number_format($arrProf['preqtddirecao'],'',2,'.');   echo campo_texto("num_equipe_{$mcpid}","S","S","",10,20,"[.###]","","right","","","","calculaTotalProfissionais($mcpid)",$num_equipe_{$mcpid}) ?></td>
		<td align="center" ><?php $num_outros_{$mcpid} = number_format($arrProf['preqtdoutros'],'',2,'.');    echo campo_texto("num_outros_{$mcpid}","S","S","",10,20,"[.###]","","right","","","","calculaTotalProfissionais($mcpid)",$num_outros_{$mcpid}) ?></td>
		<td align="center" colspan="2" id="td_total_profissionais_<?php echo $mcpid ?>" ><?php echo $arrProf ? number_format($arrProf['preqtdprofessor'] + $arrProf['preqtddirecao'] + $arrProf['preqtdoutros'],'',2,'.') : 0 ?></td>
	</tr>
	<tr bgcolor="#e9e9e9" >
		<td align="center" colspan="5" >
			<input type="button" value="Salvar Profissionais" name="btn_salvar" onclick="salvarProfissionais('<?php echo $mcpid ?>')" />
		</td>
	</tr>
<?php	
}

function pegaProfissionaisGAP( $mcpid , $emeid ){
	global $db;
	$sql = "select * from emi.profissionalenvolvido WHERE mcpid = {$mcpid} and emeid = $emeid";
	return $db->pegaLinha( $sql );		
}

function emiPegaGapsPorMacro($emeid,$mcpid)
{
	global $db;
	$sql = "select * from emi.emgap where emeid = $emeid and mcpid = $mcpid and papstatus = 'A' order by papid";
	$arrDados = $db->carregar($sql);
	return $arrDados ? $arrDados : array();
}

function pegaItensCusteio()
{
	global $db;
	$sql = "select * from emi.emitemfinanciavel where itfstatus = 'a' and itfid not in (9,10) order by itfdsc";
	$arrDados = $db->carregar($sql);
	return $arrDados ? $arrDados : array();
}

function pegaItensCapital()
{
	global $db;
	$sql = "select * from emi.emitemfinanciavel where itfstatus = 'a' and itfid in (9,10) order by itfdsc";
	$arrDados = $db->carregar($sql);
	return $arrDados ? $arrDados : array();
}


function pegaMacroCampos()
{
	global $db;
	$sql = "select * from emi.macrocampo where mcpstatus = 'A' order by mcpid";
	$arrDados = $db->carregar($sql);
	return $arrDados ? $arrDados : array();
}

function emiBuscaDadosPAP( $papid ){
	
	global $db;
	
	$dados = $db->pegaLinha("SELECT 
								papid,
								trim(papcaoatividade) as papcaoatividade,
								trim(papmeta) as papmeta
							FROM 
								emi.empap 
							WHERE 
								papid = '{$papid}'");
	
	$dados["papcaoatividade"] = iconv("ISO-8859-1", "UTF-8", $dados["papcaoatividade"]);
	$dados["papmeta"] 		  = iconv("ISO-8859-1", "UTF-8", $dados["papmeta"]);  
	
	echo simec_json_encode($dados);
	
}

/*** FUNÇÕES WORKFLOW***/


function emiVerificaEstado( $esdid ){
	
	global $db;
	
	$sql = "SELECT esdid FROM workflow.estadodocumento WHERE esdid = {$esdid}";
	
	return $db->pegaUm( $sql );
	
}

function emiCriarDocumento( $emeid , $dettipo = "P") {
	
	global $db;
	
	$docid = emiPegarDocid( $emeid , $dettipo);
	
	if( !$docid ) {
		
		// recupera o tipo do documento
		$tpdid = EMI_TIPO_DOCUMENTO;
		
		// descrição do documento
		$docdsc = "Fluxo do EMI (emi) - n°" . $emeid;
		
		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza pap do EMI
		$sql = "UPDATE
					emi.detalheentidade
				SET 
					docid = {$docid} 
				WHERE
					emeid = {$emeid}
				AND
					dettipo = '$dettipo'";

		$db->executar( $sql );
		$db->commit();
	}
	
	return $docid;
	
}

function emiPegarDocid( $emeid , $dettipo = "P") {
	
	global $db;
	
	$sql = "SELECT
				docid
			FROM
				emi.detalheentidade
			WHERE
			 	emeid = " . (integer) $emeid . " and dettipo = '$dettipo'";
	
	return (integer) $db->pegaUm( $sql );
	
}

function emiPegarEstadoAtual( $emeid , $tipo = "P") {
	
	global $db; 
	
	$docid = emiPegarDocid( $emeid , $tipo);
	 
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

function emiPegarNomeEstado( $esdid ){
	
	global $db;
	
	$sql = "SELECT esddsc FROM workflow.estadodocumento WHERE esdid = {$esdid}";
	
	return $db->pegaUm( $sql );
	
}

function emiValidaPreenchimentoParaEnvio( $emeidPai ){
	
	global $db;
	
	if ( $db->testa_superuser() ){
		return true;
	}else{
		
		$sql = "SELECT papid FROM emi.empap WHERE emeid = {$emeidPai} AND papstatus = 'A'";
		$pap = $db->pegaUm( $sql );
		
		if ( $pap ){
			return true;
		}else{
			return false;
		}
		
	}
	
}

function verificaSeCorrecao($docid){
	global $db;
	return $db->pegaUm("SELECT count(1) as count FROM workflow.historicodocumento WHERE aedid = '".EMI_ACAO_ESTADO_DOC."' and docid = '".$docid."' ");
}

function atualizarDocidEscolasEMI()
{
	global $db;
	
	// Passo 1 - Selecionar as escolas e docids dos pais.
	$sql = "select 
				emeid,
				emeidpai,
				ent.entnome,
				emi.estuf,
				( select docid from emi.ementidade emi2 where emi2.emeid = emi.emeidpai) as docid
			from 
				emi.ementidade emi
			inner join
				entidade.entidade ent ON ent.entid = emi.entid
			where 
				docid is null 
			and 
				emeidpai is not null
			and
				emiexercicio = '2009'
			order by
				emeid
			--limit 2;";
	$arrEmeid = $db->carregar($sql);
	
	if(!$arrEmeid):
		return "Não existem docids para serem atualizados.";
	endif;
	
	// Passo 2 - Obter o histórico do workflow dos docis selecionados
	foreach($arrEmeid as $emeid):
		$sql = "select 
					* 
				from 
					workflow.documento doc
				inner join
					workflow.historicodocumento hiw ON doc.docid = hiw.docid
				left join
					workflow.comentariodocumento cow ON doc.docid = cow.docid
				where 
					doc.docid = {$emeid['docid']}
				order by
					doc.docid,hiw.hstid,cow.cmdid";

		$arrWorkFlow = $db->carregar($sql);
		
		if($arrWorkFlow):
			
			foreach($arrWorkFlow as $workflow):
				
				$arrSql[ $emeid['emeid'] ]['workflow.documento'] = "insert into workflow.documento (tpdid,esdid,docdsc,unicod) VALUES (".(!$workflow['tpdid'] ? "NULL" : "'{$workflow['tpdid']}'").",".(!$workflow['esdid'] ? "NULL" : "'{$workflow['esdid']}'").",".(!$workflow['docdsc'] ? "NULL" : "'".str_replace("\\'","",$workflow['docdsc'])." - ".str_replace("\\'","",$emeid['entnome'])."'").",".(!$workflow['unicod'] ? "NULL" : "'{$workflow['unicod']}'").") returning docid;";
				
				$arrSql[ $emeid['emeid'] ]['emi.ementidade'] = "update emi.ementidade set docid = {docid} where emeid = {$emeid['emeid']};";
				
				$arrSql[ $emeid['emeid'] ]['workflow.historicodocumento'][$workflow['hstid']][] = "insert into workflow.historicodocumento (aedid,docid,usucpf,pflcod,htddata) VALUES (".(!$workflow['aedid'] ? "NULL" : "'{$workflow['aedid']}'").",{docid},".(!$workflow['usucpf'] ? "NULL" : "'{$workflow['usucpf']}'").",".(!$workflow['pflcod'] ? "NULL" : "'{$workflow['pflcod']}'").",".(!$workflow['htddata'] ? "NULL" : "'{$workflow['htddata']}'").") returning hstid;";
				
				$arrSql[ $emeid['emeid'] ]['workflow.comentariodocumento'][$workflow['hstid']] = "insert into workflow.comentariodocumento (docid,cmddsc,cmdstatus,cmddata,hstid) VALUES ({docid},".(!$workflow['cmddsc'] ? "NULL" : "'".str_replace("\\'","",$workflow['cmddsc'])."'").",".(!$workflow['cmdstatus'] ? "NULL" : "'{$workflow['cmdstatus']}'").",".(!$workflow['cmddata'] ? "NULL" : "'{$workflow['cmddata']}'").",{hstid});";
			
			endforeach;
		endif;
	endforeach;

	if(!$arrSql):
		return "Não existem docids para serem atualizados.";
	endif;
	
	foreach($arrSql as $key => $arrSQL):
	
		$docid = $db->pegaUm( $arrSql[$key]['workflow.documento']);
		
		$db->executar( str_replace("{docid}",$docid,$arrSql[$key]['emi.ementidade']) );
		
		if(is_array($arrSql[$key]['workflow.historicodocumento'])):
			$n = 0;
			foreach($arrSql[$key]['workflow.historicodocumento'] as $chave => $sqlHD ):
				
				$hstid = $db->pegaUm( str_replace(array("{docid}"),array($docid),$arrSql[$key]['workflow.historicodocumento'][$chave][$n]));

				if($arrSql[ $key ]['workflow.comentariodocumento'][$chave] && $hstid):
				
					$db->executar( str_replace( array("{docid}","{hstid}") ,array($docid,$hstid),$arrSql[ $key ]['workflow.comentariodocumento'][$chave]) );
				
				endif;
			$n++;	
			endforeach;
			
		endif;
		
	endforeach;
	
	if($db->commit()):
		$msg = "Atualizada(s) a(s) seguinte(s) escola(s):<br />";
		foreach($arrEmeid as $eme):
			$msg.= " - ".$eme['entnome']." / ".$eme['estuf']." <br />";
		endforeach;
		return $msg;
	endif;
}

function insereDetalheEntidade($emeid, $tipo)
{
	global $db;
	
	$sql = "SELECT
				emeid
			FROM
				emi.detalheentidade
			where 
				emeid = $emeid
			and
				dettipo = '$tipo'";
	if(!$db->pegaUm($sql)){
		$sql = "insert into emi.detalheentidade (emeid,docid,arqid,dettipo) values ($emeid,null,null,'$tipo')";
		$db->executar($sql);
		$db->commit();
	}
	
}

?>

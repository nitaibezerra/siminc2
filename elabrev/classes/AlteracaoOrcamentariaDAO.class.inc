<?php
class AlteracaoOrcamentariaDAO{
	private $totalFisico;
	private $totalFinanceiro;
	private $totalPedido;
	private $totalResposta;
	
	private $totalClassificacao;
	private $totalSitPedidoAlt;
	private $totalTipoAlteracao;
	private $totalTipoFonteRecurso;
	private $totalTipInstLegal;
	
	//acao, programa, unidade, localizador, exercicio
	public function manterCadastrarPedidoAlteracaoResponse( $arrDados = array(), $requisicao = '', $obCadastrarPedidoAlteracao, $strTotal ){
		global $db;
		
		$arrRegistro = $arrDados->return->registros;
		$sucesso	 = $arrDados->return->sucesso;
		
		if( (int) $sucesso == 1 ){
			$arrFisico = $arrRegistro->fisicosPedidoAlteracao;
			$identificadorUnico = $arrRegistro->identificadorUnico;
			$financeiro = $arrRegistro->fisicosPedidoAlteracao->listaFinanceiroPedidoAlteracaoDTO;
			
			if( is_object( $arrFisico ) ){
				$sql = "UPDATE elabrev.despesaacao SET dpaidentificadorunico = $identificadorUnico
							WHERE dpaid in (select d.dpaid from 
											elabrev.despesaacao d 
											inner join monitora.acao a on  a.acaid = d.acaidloa    
										where
											a.unicod = '".$arrFisico->codigoUO."'
										    and a.acacod = '".$arrFisico->codigoAcao."'
										    and a.prgcod = '".$arrFisico->codigoPrograma."'
										    and a.loccod = '".$arrFisico->codigoLocalizador."'
										    and a.prgano = '".$arrFisico->exercicio."'
										    and d.tcrid is not null
										    and d.mcrid is not null)";
				$db->executar( $sql );
				$db->commit();
			} else {
				foreach ($arrFisico as $fisico) {
					$sql = "UPDATE elabrev.despesaacao SET dpaidentificadorunico = $identificadorUnico
							WHERE dpaid in (select d.dpaid from 
											elabrev.despesaacao d 
											inner join monitora.acao a on  a.acaid = d.acaidloa    
										where
											a.unicod = '".$fisico->codigoUO."'
										    and a.acacod = '".$fisico->codigoAcao."'
										    and a.prgcod = '".$fisico->codigoPrograma."'
										    and a.loccod = '".$fisico->codigoLocalizador."'
										    and a.prgano = '".$fisico->exercicio."'
										    and d.tcrid is not null
										    and d.mcrid is not null)";
					$db->executar( $sql );
					$db->commit();
				}
			}
			return 'ok';
		} else {
			$mensagensErro = $arrDados->return->mensagensErro;
			
			if( is_array($mensagensErro) ){
				$arrMSG = array();
				foreach ($mensagensErro as $msg) {
					if( !in_array( $msg, $arrMSG ) ){
						array_push( $arrMSG, $msg );
					}
				}
				$mensagensErro = implode(', ', $arrMSG); //($arrMSG,d);
			}
			
			$arrDadosErro = $obCadastrarPedidoAlteracao->pedidoAlteracaoDTO->fisicosPedidoAlteracao;
			foreach ($arrDadosErro as $erro) {
				$sql = "INSERT INTO elabrev.logerrows(logtipo, acacod, unicod, prgcod, loccod, prgano, usucpf, logerro) 
						VALUES ('".$requisicao."', '".$erro->codigoAcao."', '".$erro->codigoUO."', '".$erro->codigoPrograma."', 
								'".$erro->codigoLocalizador."', '".$erro->exercicio."', '".$_SESSION['usucpf']."', '".$mensagensErro."')";
					
				$db->executar( $sql );
				$db->commit();
			}
			return 'erro';
		}
	}
	
	public function carregarPedido( $mcrid, $arrTipoAlteracao, $arrCodigoUO ){
		
		global $db;
		
		$sql = "SELECT 
					codigoclassificacaoalteracao, codigoMomento, codigoOrgao, codigoTipoAlteracao, descricao, exercicio, identificadorunico, 
                    idcredito, mcrid, codigouo
				FROM
				( 
				SELECT 
				    d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma,  
				    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, un.unicod||' - '||un.unidsc as descricao,
				    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa, 
				    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza, 
				    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d 
				    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod like '26%' and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
				        															--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' ) 
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				        															--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920') 
				    INNER JOIN public.idoc 				 i  on i.idoid = d.idoid 
				    LEFT JOIN public.naturezadespesa 	 n  on n.ndpid = d.ndpid 
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid 
                    left join public.unidade 			un on un.unicod = a.unicod
				WHERE 
					d.dpavalor > 0 
				    and d.mcrid = $mcrid
					--and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				    
				union all 
				 
				SELECT
					d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma, 
				    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, un.unicod||' - '||un.unidsc as descricao,
				    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa,
				    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza,
				    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d
					INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod like '26%' and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
				    																--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' )
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				    																--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920')
				    INNER JOIN public.idoc 				 i on i.idoid = d.idoid
				    LEFT JOIN public.naturezadespesa 	 n on n.ndpid = d.ndpid
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid
                    left join public.unidade 			un on un.unicod = a.unicod
				WHERE 
					d.dpavalor < 0 
				    and d.mcrid = $mcrid
				   -- and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				) as foo
				where 
					identificadorunico is null
					and codigouo = '$arrCodigoUO'
					and codigoTipoAlteracao = '$arrTipoAlteracao'
				group by 
					codigoclassificacaoalteracao, codigoMomento, codigoOrgao, codigoTipoAlteracao, descricao, exercicio, 
					identificadorunico, idcredito, mcrid, codigouo $filtro";
		
		$arrDados = $db->pegaLinha( $sql );
		$arrDados = $arrDados ? $arrDados : array();
		return $arrDados;
	}
	
	public function carregarPedidoIdentificador( $mcrid, $tipoAlteracao, $identificador ){
		global $db;
		
		$sql = "SELECT 
					codigoclassificacaoalteracao, codigoMomento, codigoOrgao, codigoTipoAlteracao, descricao, exercicio, identificadorunico, 
                    idcredito, mcrid, codigouo
				FROM
				( 
				SELECT 
				    d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma,  
				    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, un.unicod||' - '||un.unidsc as descricao,
				    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa, 
				    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza, 
				    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d 
				    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod like '26%' and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
				        															--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' ) 
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				        															--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920') 
				    INNER JOIN public.idoc 				 i  on i.idoid = d.idoid 
				    LEFT JOIN public.naturezadespesa 	 n  on n.ndpid = d.ndpid 
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid 
                    left join public.unidade 			un on un.unicod = a.unicod
				WHERE 
					d.dpavalor > 0 
				    and d.mcrid = $mcrid
					--and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				    
				union all 
				 
				SELECT
					d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma, 
				    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, un.unicod||' - '||un.unidsc as descricao,
				    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa,
				    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza,
				    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d
					INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod like '26%' and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
				    																--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' )
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				    																--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920')
				    INNER JOIN public.idoc 				 i on i.idoid = d.idoid
				    LEFT JOIN public.naturezadespesa 	 n on n.ndpid = d.ndpid
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid
                    left join public.unidade 			un on un.unicod = a.unicod
				WHERE 
					d.dpavalor < 0 
				    and d.mcrid = $mcrid
				    --and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				) as foo
				where 
					identificadorunico = $identificador
					and codigoTipoAlteracao = '$tipoAlteracao'
				group by 
					codigoclassificacaoalteracao, codigoMomento, codigoOrgao, codigoTipoAlteracao, descricao, exercicio, 
					identificadorunico, idcredito, mcrid, codigouo";
		
		$arrDados =$db->pegaLinha( $sql );
		$arrDados = $arrDados ? $arrDados : array();
		return $arrDados;
	}
	
	public function carregarFisico($mcrid, $codigouo, $codigotipoalteracao){
		global $db;
		
		$sql = "SELECT 
					identificadorunico, codigoacao, codigoesfera, codigofuncao, codigolocalizador, codigoprograma, codigosubfuncao, 
                    codigotipoinclusaolocalizador, codigouo, exercicio, quantidadeacrescimo, quantidadereducao, codigotipoalteracao
				FROM
				( 
				SELECT 
				    d.dpaidentificadorunico as identificadorunico, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma,  
				    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, 'Integração - 01' as descricao,
				    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa, 
				    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza, 
				    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d 
				    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod like '26%' and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
				        															--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' ) 
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				        															--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920') 
				    INNER JOIN public.idoc 				 i  on i.idoid = d.idoid 
				    LEFT JOIN public.naturezadespesa 	 n  on n.ndpid = d.ndpid 
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid 
				WHERE 
					d.dpavalor > 0 
				    and d.mcrid = $mcrid
					--and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				    
				union all 
				 
				SELECT
					d.dpaidentificadorunico as identificadorunico, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma, 
				    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, 'Integração - 01' as descricao,
				    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa,
				    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza,
				    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d
					INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod like '26%' and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
				    																--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' )
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				    																--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920')
				    INNER JOIN public.idoc 				 i on i.idoid = d.idoid
				    LEFT JOIN public.naturezadespesa 	 n on n.ndpid = d.ndpid
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid
				WHERE 
					d.dpavalor < 0 
				    and d.mcrid = $mcrid
				    --and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				) as foo
				where  
					/*identificadorunico is null
                    and*/ codigouo = '$codigouo'
                    and codigotipoalteracao = '$codigotipoalteracao'
				group by 
					identificadorunico, codigoacao, codigoesfera, codigofuncao, codigolocalizador, codigoprograma, codigosubfuncao, 
                    codigotipoinclusaolocalizador, codigouo, exercicio, quantidadeacrescimo, quantidadereducao, codigotipoalteracao
				order by codigouo";
		
		$arrDados = $db->carregar( $sql );
		$arrDados = $arrDados ? $arrDados : array();
		return $arrDados;
	}
	
	public function carregarTotalCredito( $limit ){
		global $db;
		
		$filtro = '';
		if( $limit ) $filtro = " limit $limit ";
		
		$sql = "select count(*) from (
					SELECT 
										idcredito, codigoTipoAlteracao, identificadorunico, codigouo
									FROM
									( 
									SELECT 
									    d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, a.unicod as codigouo, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao
									FROM elabrev.despesaacao d 
									    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod like '26%' and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
									        															--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' ) 
									    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
									        															--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920') 
									    INNER JOIN public.idoc 				 i  on i.idoid = d.idoid 
									    LEFT JOIN public.naturezadespesa 	 n  on n.ndpid = d.ndpid 
									    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid 
					                    left join public.unidade 			un on un.unicod = a.unicod
									WHERE 
										d.dpavalor > 0 
									    and d.mcrid = 28
										--and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
									    
									union all 
									 
									SELECT
										d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, a.unicod as codigouo, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao
									FROM elabrev.despesaacao d
										INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod like '26%' and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
									    																--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' )
									    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
									    																--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920')
									    INNER JOIN public.idoc 				 i on i.idoid = d.idoid
									    LEFT JOIN public.naturezadespesa 	 n on n.ndpid = d.ndpid
									    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid
					                    left join public.unidade 			un on un.unicod = a.unicod
									WHERE 
										d.dpavalor < 0 
									    and d.mcrid = 28
									    --and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
									) as foo
									where 
										identificadorunico is null
									group by 
										codigoTipoAlteracao, identificadorunico, idcredito, codigouo $filtro
					) as foo1";
		
		$total = $db->pegaUm( $sql );		
		return $total;
	}
	
	public function carregarFinanceiro($mcrid, $codigouo, $codigotipoalteracao, $codigoacao, $codigolocalizador){
		global $db;
		
		$sql = "SELECT 
					identificadorunico, plocod, codigouo, codigotipoalteracao, codigofonte, 
                    codigoidoc, codigoiduso, codigonatureza, sum( valordespesa ) as valordespesa,
				    Case 
				     when codigoacao in ('0005', '0022', '002F', '00H2', '0C04', '20TP', '0110', '0181', '0486', '0515', '0625', '0716', '0920', '0969', '0E36', '2004', '2010', '2011', '2012',  '20CE', '20CW', '8744', '8790' ) THEN '1'
				     when codigoacao in ('2992', '4001', '4009', '4086', '6318', '6321') then
				      Case substr(codigonatureza, 2, 1) 
				       when '1' THEN '1' else '2' end 
				     when codigoacao in ('12KU', '12KV' ) THEN '3'
				     when codigoacao in ('00G5', '00GP', '0283', '0284', '09HB', '00H7' ) THEN '0'
				     ELSE '2'
				    end as codigorp,
                    codigotipofonterecurso
				FROM
				( 
				SELECT 
				    d.dpaidentificadorunico as identificadorunico, d.plocod, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma,  
				    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, 'Integração - 01' as descricao,
				    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa, 
				    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza, 
				    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d 
				    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod like '26%' and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
				        															--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' ) 
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				        															--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920') 
				    INNER JOIN public.idoc 				 i  on i.idoid = d.idoid 
				    LEFT JOIN public.naturezadespesa 	 n  on n.ndpid = d.ndpid 
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid 
				WHERE 
					d.dpavalor > 0 
				    and d.mcrid = $mcrid
					--and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				    
				union all 
				 
				SELECT
					d.dpaidentificadorunico as identificadorunico, d.plocod, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma, 
				    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, 'Integração - 01' as descricao,
				    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa,
				    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza,
				    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d
					INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod like '26%' and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
				    																--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' )
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				    																--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920')
				    INNER JOIN public.idoc 				 i on i.idoid = d.idoid
				    LEFT JOIN public.naturezadespesa 	 n on n.ndpid = d.ndpid
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid
				WHERE 
					d.dpavalor < 0 
				    and d.mcrid = $mcrid
				   -- and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				) as foo
				where 
					/*identificadorunico is null
                    and*/ codigouo = '$codigouo'
                    and codigotipoalteracao = '$codigotipoalteracao'
                    and codigoacao = '$codigoacao'
                    and codigolocalizador = '$codigolocalizador'
				group by 
					identificadorunico, plocod, codigouo, codigotipoalteracao, codigofonte, 
                    codigoidoc, codigoiduso, codigonatureza, codigorp, codigotipofonterecurso, codigoacao
				order by codigouo, codigotipoalteracao";
		
		$arrDados = $db->carregar( $sql );
		$arrDados = $arrDados ? $arrDados : array();
		return $arrDados;
	}
	
	public function carregarPedidoAlteracao(){	
		global $db;
			
		$sql = "SELECT 
					identificadorunico, idcredito, mcrid, codigouo, codigotipoalteracao, codigoprograma, codigoacao, codigolocalizador, codigofonte, descricao, 
					codigoidoc, codigoiduso, codigotipofonterecurso, quantidadeacrescimo, quantidadereducao, exercicio, codigotipoinclusaolocalizador,
                    codigoorgao, codigomomento, codigoclassificacaoalteracao, identificadorunico,
				    Case 
				     when codigoacao in ('0005', '0022', '002F', '00H2', '0C04', '20TP', '0110', '0181', '0486', '0515', '0625', '0716', '0920', '0969', '0E36', '2004', '2010', '2011', '2012',  '20CE', '20CW', '8744', '8790' ) THEN '1'
				     when codigoacao in ('2992', '4001', '4009', '4086', '6318', '6321') then
				      Case substr(codigonatureza, 2, 1) 
				       when '1' THEN '1' else '2' end 
				     when codigoacao in ('12KU', '12KV' ) THEN '3'
				     when codigoacao in ('00G5', '00GP', '0283', '0284', '09HB', '00H7' ) THEN '0'
				     ELSE '2'
				    end as codigorp,
					sum( valordespesa ) as valordespesa,
					codigonatureza, codigoesfera, codigofuncao, codigosubfuncao  
				FROM
				( 
				SELECT 
				    d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma,  
				    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, 'Integração - 01' as descricao,
				    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa, 
				    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza, 
				    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d 
				    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod like '26%' and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
				        															--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' ) 
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				        															--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920') 
				    INNER JOIN public.idoc 				 i  on i.idoid = d.idoid 
				    LEFT JOIN public.naturezadespesa 	 n  on n.ndpid = d.ndpid 
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid 
				WHERE 
					d.dpavalor > 0 
				    and d.mcrid = 28
					--and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				    
				union all 
				 
				SELECT
					d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma, 
				    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, 'Integração - 01' as descricao,
				    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa,
				    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza,
				    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d
					INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod like '26%' and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
				    																--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' )
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				    																--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920')
				    INNER JOIN public.idoc 				 i on i.idoid = d.idoid
				    LEFT JOIN public.naturezadespesa 	 n on n.ndpid = d.ndpid
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid
				WHERE 
					d.dpavalor < 0 
				    and d.mcrid = 28
				    --and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				) as foo
				where --codigouo = '26239' 
					identificadorunico is null
                	--and codigotipoalteracao <> '103a' -- and codigoacao = '4002' and codigouo = '26278'
				group by 
					idcredito, mcrid, codigouo, codigotipoalteracao, codigoprograma, 
				    codigoacao, codigolocalizador, codigofonte, 
				    codigoidoc, codigoiduso, codigorp, descricao,
				    codigonatureza, codigoesfera, codigofuncao, codigosubfuncao, codigotipofonterecurso,
                    quantidadeacrescimo, quantidadereducao, exercicio, codigotipoinclusaolocalizador,
                    codigoorgao, codigomomento, codigoclassificacaoalteracao, identificadorunico
				order by codigouo, codigotipoalteracao";
		
		$arrDados = $db->carregar( $sql );
		$arrDados = $arrDados ? $arrDados : array();
		return $arrDados;
	}
	
	public function getTipoInclusaoAcao($arPedido = array(), $arFisico = array()){
		global $db;
		
		extract($arPedido);
		extract($arFisico);
		
		$sql = "SELECT codigotipoinclusaoacao FROM elabrev.ws_acaodto where exercicio = '$exercicio'
		and codigoesfera = '$codigoesfera' 
		and codigoorgao = '$codigouo'
		and codigofuncao = '$codigofuncao'
		and codigosubfuncao = '$codigosubfuncao'
		and codigoprograma = '$codigoprograma'
		and codigoacao = '$codigoacao' ";

		$acatipoinclusao = $db->pegaUm( $sql );
		if( !$acatipoinclusao || $acatipoinclusao == '0' )
			$acatipoinclusao = '1';
			
		return $acatipoinclusao;
	}
	
	public function getTipoInclusaoLocalizador($arPedido = array(), $arFisico = array()){
		global $db;
		
		extract($arPedido);
		extract($arFisico);
		
//		$sql = "SELECT acatipoinclusaolocalizador FROM monitora.acao 
//				WHERE prgano ='$exercicio' and acasnrap = false 
//					and esfcod = $codigoesfera and unicod = '$codigouo' and funcod = '$codigofuncao' 
//					and sfucod = '$codigosubfuncao' and prgcod = '$codigoprograma' 
//				    and acacod = '$codigoacao' and loccod = '$codigolocalizador'";

		$sql = "select codigotipoinclusao from elabrev.ws_localizadoresdto loc 
					inner join elabrev.ws_acaodto as acao on acao.identificadorunico = loc.identificadorunicoacao
				    where acao.exercicio = '$exercicio' and codigoorgao = '$codigouo' and codigoesfera = '$codigoesfera' and codigosubfuncao = '$codigosubfuncao' 
				    and codigofuncao = '$codigofuncao' and codigoprograma = '$codigoprograma' and codigoacao = '". str_to_upper( $codigoacao ) ."' and codigolocalizador = '$codigolocalizador'";

//		$arrLocalizadoresTipoDois = array(1, 52, 31, 41);
//		
//		if( $codigoacao == '4572' || $codigoacao == '0509' || $codigoacao == '20RX' ){
//			if( in_array((int) $codigolocalizador, $arrLocalizadoresTipoDois ) ){
//				$acatipoinclusao = '2';
//			}elseif( $codigolocalizador == '0101' ){
//				$acatipoinclusao = '6';
//			}elseif( $codigolocalizador == '7004' ){
//				$acatipoinclusao = '2';
//			}else{
//				$acatipoinclusao = '1';
//			}
//		}elseif(  $codigoacao == '4086' || $codigoacao == '0487' || $codigoacao == '00FA'  ){
//			if( in_array((int) $codigolocalizador, $arrLocalizadoresTipoDois ) ){
//				$acatipoinclusao = '1';
//			}elseif( $codigolocalizador == '0101' ){
//				$acatipoinclusao = '6';
//			}elseif( $codigolocalizador == '7004' ){
//				$acatipoinclusao = '2';
//			}else{
//				$acatipoinclusao = '2';
//			}
//		}
		$acatipoinclusao = $db->pegaUm( $sql );
		//REVER!
		return $acatipoinclusao;
	}
	
	public function carregarPerguntaJustificativa( $dados = array() ){
		global $db;
		
		extract($dados);
		$arrRegistro = array();
		$sql = "SELECT 
					case when trim(jsccaracterizacao) = '' then '-' else jsccaracterizacao end as jsccaracterizacao,
					case when trim(jscconsequencia) = '' then '-' else jscconsequencia end as jscconsequencia,
				    case when trim(jscreflexo) = '' then '-' else jscreflexo end as jscreflexo,
				    case when trim(jscrepercussao) = '' then '-' else jscrepercussao end as jscrepercussao,
				    case when trim(jscmemoria) = '' then '-' else jscmemoria end as jscmemoria
				FROM elabrev.justificativacredito
				WHERE tcrid = $idcredito and  unicod = '$codigouo' and  mcrid = $mcrid";
		
		$arrDados = $db->pegaLinha( $sql );		
		return $arrDados;
	}
	
	private function objectToArray ($object) {
		if ( count($object) > 1 ) {
			$arr = array();
			for ( $i = 0; $i < count($object); $i++ ) {
				$arr[] = get_object_vars($object[$i]);
			}
	 
	    	return $arr;
	 
		} else {
			return get_object_vars($object);
		}
	}
	
	function arr2xml($array){
		if(is_array($array) || is_object($array) ){
			$array = (array)$array;
			function create_tag($XML,$array){
				foreach($array as $tag_name => $value){
					$tag_name=is_numeric($tag_name)?"_".$tag_name:$tag_name;
					$tag_name = (strpos( $tag_name, '_' ) == '' ? $tag_name : substr( $tag_name, 0, strlen( $tag_name ) -2 ));
					if(is_array($value) || is_object($value) ){
						$value = (array)$value;
						$XML->startElement($tag_name);
						create_tag($XML,$value);
					}else{
						$XML->writeElement($tag_name,$value);
					}
				}
				$XML->endElement();
			}
			$XML= new XMLWriter();
			$XML->openMemory();
			$XML->startDocument('1.0','UTF-8');
			create_tag($XML,$array);
			$XML->endDocument();
			return utf8_encode($XML->outputMemory());
		}else{
			return false;
		}
	}
	
	public function gravaRetornoObterPedidosAlteracao( $obRetorno ){
		global $db;
		
		if( is_array($obRetorno->return->registros) || is_object($obRetorno->return->registros) ){
			$obPedido = $obRetorno->return->registros;
			$totalPedido = 0;
			$this->totalFisico = 0;
			$this->totalFinanceiro = 0;
			$this->totalResposta = 0;			
			$arrTotal = array();
			
			if( is_array($obPedido) ){
				foreach ($obPedido as $pedido) {
					$idopsa = $this->gravaPedido( $pedido );
					
					if($pedido->fisicosPedidoAlteracao){
						if( is_array($pedido->fisicosPedidoAlteracao) ){
							foreach ($pedido->fisicosPedidoAlteracao as $fisico) {
								$this->gravaFisicoPedido( $idopsa, $fisico, $tipo = 'array' );
							}
						} else {
							$this->gravaFisicoPedido( $idopsa, $pedido->fisicosPedidoAlteracao, $tipo = 'nao_array' );
						}
					}
					$resposta = $pedido->respostasJustificativa;
					if( is_array($resposta) || is_object($resposta) ){
						
						foreach ($resposta as $resp) {							
							$this->totalResposta++;
							$sql = "INSERT INTO elabrev.ws_respostasjustificativa(idopsa, codigopergunta, resposta) 
									VALUES ($idopsa, '{$resp->codigoPergunta}', '{$resp->resposta}')";
							
							$db->executar($sql);
						}
					}
				}
			} else {
				$idopsa = $this->gravaPedido( $obPedido );
				
				if($obPedido->fisicosPedidoAlteracao){
					if( is_array($obPedido->fisicosPedidoAlteracao) ){
						foreach ($obPedido->fisicosPedidoAlteracao as $fisico) {
							$this->gravaFisicoPedido( $idopsa, $fisico );
						}
					} else {
						$this->gravaFisicoPedido( $idopsa, $obPedido->fisicosPedidoAlteracao );
					}
				}
				$resposta = $obPedido->respostasJustificativa;
				if( is_array($resposta) || is_object($resposta) ){
					
					foreach ($resposta as $resp) {						
						$this->totalResposta++;
						$sql = "INSERT INTO elabrev.ws_respostasjustificativa(idopsa, codigopergunta, resposta) 
								VALUES ($idopsa, '{$resp->codigoPergunta}', '{$resp->resposta}')";
						
						$db->executar($sql);
					}
				}
			}
			$db->commit();
			$arrTotal = array("Total Pedido: ".$this->totalPedido, "Total Fisico: ".$this->totalFisico,
							  "Total Financeiro: ".$this->totalFinanceiro,"Total Resposta: ".$this->totalResposta);
		}
		return $arrTotal;
	}
	
	private function gravaPedido( $pedido ){
		global $db;
		$this->totalPedido++;
		
		$sql = "INSERT INTO elabrev.ws_obterpedidosalteracao(codigoclassificacaoalteracao, codigomomento, codigoorgao, codigotipoalteracao,
  					descricao, exercicio, codigosituacaopedidoalteracao, datacriacao, dataenvio, identificadorunico, identificadorunicopedidoorigem,
  					loginusuariocriacao, loginusuarioenvio, nomeusuariocriacao, nomeusuarioenvio, snagregadora, snatual, snemvalidacaoexterna,
  					snenviadocongressonacional, snexclusaologica, snintegracao, snorcamentoInvestimento) 
				VALUES (
					  '{$pedido->codigoClassificacaoAlteracao}',
					  '{$pedido->codigoMomento}',
					  '{$pedido->codigoOrgao}',
					  '{$pedido->codigoTipoAlteracao}',
					  '{$pedido->descricao}',
					  '{$pedido->exercicio}',
					  '{$pedido->codigoSituacaoPedidoAlteracao}',
					  '{$pedido->dataCriacao}',
					  '{$pedido->dataEnvio}',
					  '{$pedido->identificadorUnico}',
					  '{$pedido->identificadorUnicoPedidoOrigem}',
					  '{$pedido->loginUsuarioCriacao}',
					  '{$pedido->loginUsuarioEnvio}',
					  '{$pedido->nomeUsuarioCriacao}',
					  '{$pedido->nomeUsuarioEnvio}',
					  '{$pedido->snAgregadora}',
					  '{$pedido->snAtual}',
					  '{$pedido->snEmValidacaoExterna}',
					  '{$pedido->snEnviadoCongressoNacional}',
					  '{$pedido->snExclusaoLogica}',
					  '{$pedido->snIntegracao}',
					  '{$pedido->snOrcamentoInvestimento}') returning idopsa";
		
		$idopsa	= $db->pegaUm( $sql );
		return $idopsa;
	}
	
	public function gravaFisicoPedido( $idopsa, $obFisico ){
		global $db;
		$this->totalFisico++;
		$sqlFisco = "INSERT INTO elabrev.ws_fisicopedidoalteracao(idopsa, codigoacao, codigoesfera, codigofuncao, codigolocalizador, codigoprograma,
  					codigosubfuncao, codigotipoinclusaolocalizador, codigouo, exercicio, quantidadeacrescimo, quantidadereducao) 
				VALUES (
				  $idopsa,
				  '{$obFisico->codigoAcao}',
				  '{$obFisico->codigoEsfera}',
				  '{$obFisico->codigoFuncao}',
				  '{$obFisico->codigoLocalizador}',
				  '{$obFisico->codigoPrograma}',
				  '{$obFisico->codigoSubFuncao}',
				  '{$obFisico->codigoTipoInclusaoLocalizador}',
				  '{$obFisico->codigoUO}',
				  '{$obFisico->exercicio}',
				  '{$obFisico->quantidadeAcrescimo}',
				  '{$obFisico->quantidadeReducao}'
				) returning idfpa";
		
		$idfpa = $db->pegaUm( $sqlFisco );
		$financeiro = $obFisico->listaFinanceiroPedidoAlteracaoDTO;
		
		if( is_array($financeiro) ){		
			foreach ($financeiro as $f) {
  				$this->totalFinanceiro++;
				$sql = "INSERT INTO elabrev.ws_financeiropedidoalteracao(idfpa, codigofonte, codigoidoc, codigoiduso, codigonatureza,
  							codigorp, codigorplei, codigotipofonterecurso, valorcancelamento, valorsuplementacao) 
						VALUES (
						  $idfpa,
						  '{$f->codigoFonte}',
						  '{$f->codigoIdOC}',
						  '{$f->codigoIdUso}',
						  '{$f->codigoNatureza}',
						  '{$f->codigoRP}',
						  '{$f->codigoRPLei}',
						  '{$f->codigoTipoFonteRecurso}',
						  '{$f->valorCancelamento}',
						  '{$f->valorSuplementacao}')";
				
				$db->executar( $sql );
			}					
		} else {
			$this->totalFinanceiro++;
			$sql = "INSERT INTO elabrev.ws_financeiropedidoalteracao(idfpa, codigofonte, codigoidoc, codigoiduso, codigonatureza,
  						codigorp, codigorplei, codigotipofonterecurso, valorcancelamento, valorsuplementacao) 
					VALUES (
					  $idfpa,
					  '{$financeiro->codigoFonte}',
					  '{$financeiro->codigoIdOC}',
					  '{$financeiro->codigoIdUso}',
					  '{$financeiro->codigoNatureza}',
					  '{$financeiro->codigoRP}',
					  '{$financeiro->codigoRPLei}',
					  '{$financeiro->codigoTipoFonteRecurso}',
					  '{$financeiro->valorCancelamento}',
					  '{$financeiro->valorSuplementacao}')";
			
			$db->executar( $sql );
		}
		return true;
	}
	
	public function gravaRetornoCadastrarPedido( $arrRegistro ){
		global $db;
			
			$tcrcod = substr($arrRegistro['tipoalteracao'],0,3);
			$mcrid = $arrRegistro['mcrid'];
			$sql = "select tcrid from elabrev.tipocredito where tcrano = '{$_SESSION['exercicio']}' and tcrcod = '$tcrcod' and tcrstatus = 'A'";
			$tcrid = $db->pegaUm( $sql );
			
			$arrRegistro = $arrRegistro['retorno']->return->registros;
			
			$arrFisico = $arrRegistro->fisicosPedidoAlteracao;
			$identificadorUnico = $arrRegistro->identificadorUnico;
			$financeiro = $arrRegistro->fisicosPedidoAlteracao->listaFinanceiroPedidoAlteracaoDTO;
			
		if( is_object( $arrFisico ) ){
			$sql = "UPDATE elabrev.despesaacao SET dpaidentificadorunico = $identificadorUnico
						WHERE dpaid in (select d.dpaid from 
										elabrev.despesaacao d 
										inner join monitora.acao a on  a.acaid = d.acaidloa    
									where
										a.unicod = '".$arrFisico->codigoUO."'
									    and a.acacod = '".$arrFisico->codigoAcao."'
									    and a.prgcod = '".$arrFisico->codigoPrograma."'
									    and a.loccod = '".$arrFisico->codigoLocalizador."'
									    and a.prgano = '".$arrFisico->exercicio."'
									    and d.tcrid = '$tcrid'
									    and d.mcrid = '$mcrid')";
			
			$db->executar( $sql );
		} else {
			foreach ($arrFisico as $fisico) {
				$sql = "UPDATE elabrev.despesaacao SET dpaidentificadorunico = $identificadorUnico
						WHERE dpaid in (select d.dpaid from 
										elabrev.despesaacao d 
										inner join monitora.acao a on  a.acaid = d.acaidloa    
									where
										a.unicod = '".$fisico->codigoUO."'
									    and a.acacod = '".$fisico->codigoAcao."'
									    and a.prgcod = '".$fisico->codigoPrograma."'
									    and a.loccod = '".$fisico->codigoLocalizador."'
									    and a.prgano = '".$fisico->exercicio."'
									    and d.tcrid = '$tcrid'
									    and d.mcrid = '$mcrid')";
				
				$db->executar( $sql );
			}
		}
		return $db->commit();
	}
	
	public function gravaPerguntaResposta( $arrResposta ){
		global $db;
		
		if( is_array( $arrResposta ) ){
			foreach ($arrResposta as $resposta) {
				$db->executar( "DELETE FROM elabrev.ws_perguntajustificativa WHERE codigopergunta = '{$resposta->codigoPergunta}'" )	;
				$sql = "INSERT INTO elabrev.ws_perguntajustificativa(codigopergunta, pergunta) 
						VALUES ('{$resposta->codigoPergunta}', '{$resposta->pergunta}')";
				$db->executar( $sql );
			}			
		} else {
			$db->executar( "DELETE FROM elabrev.ws_perguntajustificativa WHERE codigopergunta = '{$arrResposta->codigoPergunta}'" )	;
			$sql = "INSERT INTO elabrev.ws_perguntajustificativa(codigopergunta, pergunta) 
					VALUES ('{$arrResposta->codigoPergunta}', '{$arrResposta->pergunta}')";
			$db->executar( $sql );
		}
		return $db->commit();
	}
	
	public function gravaRetornoApoioAlteracoesOrcamentarias( $arrRetorno ){
		global $db;
		
		if( is_array( $arrRetorno->classificacoesAlteracaoDTO ) ){
			foreach ($arrRetorno->classificacoesAlteracaoDTO as $retorno) {
				$db->executar("DELETE FROM elabrev.ws_classificacoesalteracao WHERE codigoclassificacaoalteracao = '{$retorno->codigoClassificacaoAlteracao}'");
				
				$sql = "INSERT INTO elabrev.ws_classificacoesalteracao(codigoclassificacaoalteracao, descricao, snativo, sntipocredito) 
						VALUES (
						  '{$retorno->codigoClassificacaoAlteracao}',
						  '{$retorno->descricao}',
						  '{$retorno->snAtivo}',
						  '{$retorno->snTipoCredito}'
						)";
				$db->executar( $sql );
				$this->totalClassificacao++;
			}			
		} else {
			$db->executar("DELETE FROM elabrev.ws_classificacoesalteracao WHERE codigoclassificacaoalteracao = '{$retorno->classificacoesAlteracaoDTO->codigoClassificacaoAlteracao}'");
			
			$sql = "INSERT INTO elabrev.ws_classificacoesalteracao(codigoclassificacaoalteracao, descricao, snativo, sntipocredito) 
					VALUES (
					  '{$arrRetorno->classificacoesAlteracaoDTO->codigoClassificacaoAlteracao}',
					  '{$arrRetorno->classificacoesAlteracaoDTO->descricao}',
					  '{$arrRetorno->classificacoesAlteracaoDTO->snAtivo}',
					  '{$arrRetorno->classificacoesAlteracaoDTO->snTipoCredito}'
					)";
			$db->executar( $sql );
			$this->totalClassificacao++;
		}
		
		if( is_array( $arrRetorno->situacoesPedidoAlteracaoDTO ) ){
			foreach ($arrRetorno->situacoesPedidoAlteracaoDTO as $retorno) {
				$db->executar("DELETE FROM elabrev.ws_situacoespedidoalteracao WHERE codigosituacaopedidoAlteracao = '{$retorno->codigoSituacaoPedidoAlteracao}'");
				
				$sql = "INSERT INTO elabrev.ws_situacoespedidoalteracao(codigosituacaopedidoAlteracao, descricao, snativo) 
						VALUES (
						  '{$retorno->codigoSituacaoPedidoAlteracao}',
						  '{$retorno->descricao}',
						  '{$retorno->snAtivo}'
						)";
				$db->executar( $sql );
				$this->totalSitPedidoAlt++;
			}			
		} else {
			$db->executar("DELETE FROM elabrev.ws_situacoespedidoalteracao WHERE codigosituacaopedidoAlteracao = '{$retorno->situacoesPedidoAlteracaoDTO->codigoSituacaoPedidoAlteracao}'");
			
			$sql = "INSERT INTO elabrev.ws_situacoespedidoalteracao(codigosituacaopedidoAlteracao, descricao, snativo) 
					VALUES (
					  '{$arrRetorno->situacoesPedidoAlteracaoDTO->codigoSituacaoPedidoAlteracao}',
					  '{$arrRetorno->situacoesPedidoAlteracaoDTO->descricao}',
					  '{$arrRetorno->situacoesPedidoAlteracaoDTO->snAtivo}'
					)";
			$db->executar( $sql );
			$this->totalSitPedidoAlt++;
		}
		
		$db->executar("DELETE FROM elabrev.ws_tiposalteracao");
		if( is_array( $arrRetorno->tiposAlteracaoDTO ) ){
			foreach ($arrRetorno->tiposAlteracaoDTO as $retorno) {
				
				$sql = "INSERT INTO elabrev.ws_tiposalteracao(baselegal, codigoclassificacaoalteracao, codigotipoalteracao,
  								descricao, exercicio, snorcamentoinvestimento) 
						VALUES (
						  '".str_ireplace("'", "", $retorno->baseLegal)."',
						  '{$retorno->codigoClassificacaoAlteracao}',
						  '{$retorno->codigoTipoAlteracao}',
						  '".str_ireplace("'", "", $retorno->descricao)."',
						  '{$retorno->exercicio}',
						  '{$retorno->snOrcamentoInvestimento}'
						)";
				$db->executar( $sql );
				$this->totalTipoAlteracao++;
			}			
		} else {			
			$sql = "INSERT INTO elabrev.ws_tiposalteracao(baselegal, codigoclassificacaoalteracao, codigotipoalteracao,
  							descricao, exercicio, snorcamentoinvestimento) 
					VALUES (
				  '".str_ireplace("'", "", $arrRetorno->tiposAlteracaoDTO->baseLegal)."',
				  '{$arrRetorno->tiposAlteracaoDTO->codigoClassificacaoAlteracao}',
				  '{$arrRetorno->tiposAlteracaoDTO->codigoTipoAlteracao}',
				  '".str_ireplace("'", "", $arrRetorno->tiposAlteracaoDTO->descricao)."',
				  '{$arrRetorno->tiposAlteracaoDTO->exercicio}',
				  '{$arrRetorno->tiposAlteracaoDTO->snOrcamentoInvestimento}'
				)";
				$db->executar( $sql );
				$this->totalTipoAlteracao++;
		}
		
		if( is_array( $arrRetorno->tiposFonteRecursoDTO ) ){
			foreach ($arrRetorno->tiposFonteRecursoDTO as $retorno) {
				$db->executar("DELETE FROM elabrev.ws_tiposfonterecurso WHERE codigotipofonterecurso = '{$retorno->codigoTipoFonteRecurso}'");
				
				$sql = "INSERT INTO elabrev.ws_tiposfonterecurso(codigotipofonterecurso, descricao)
						VALUES (
						  '{$retorno->codigoTipoFonteRecurso}',
						  '{$retorno->descricao}'
						)";
				$db->executar( $sql );
				$this->totalTipoFonteRecurso++;
			}			
		} else {
			$db->executar("DELETE FROM elabrev.ws_tiposfonterecurso WHERE codigotipofonterecurso = '{$retorno->tiposFonteRecursoDTO->codigoTipoFonteRecurso}'");
			
			$sql = "INSERT INTO elabrev.ws_tiposfonterecurso(codigotipofonterecurso, descricao)
					VALUES (
				  		'{$arrRetorno->tiposFonteRecursoDTO->codigoTipoFonteRecurso}',
				  		'{$arrRetorno->tiposFonteRecursoDTO->descricao}'
					)";
			$db->executar( $sql );
			$this->totalTipoFonteRecurso++;
		}
		
		if( is_array( $arrRetorno->tiposInstrumentoLegalDTO ) ){
			foreach ($arrRetorno->tiposInstrumentoLegalDTO as $retorno) {
				$db->executar("DELETE FROM elabrev.ws_tiposinstrumentolegal WHERE codigotipoinstrumentolegal = '{$retorno->codigoTipoInstrumentoLegal}'");
				
				$sql = "INSERT INTO elabrev.ws_tiposinstrumentolegal(codigotipoinstrumentolegal, descricao, snativo)
						VALUES (
						  '{$retorno->codigoTipoInstrumentoLegal}',
						  '{$retorno->descricao}',
						  '{$retorno->snAtivo}'
						)";
				$db->executar( $sql );
				$this->totalTipInstLegal++;
			}			
		} else {
			$db->executar("DELETE FROM elabrev.ws_tiposinstrumentolegal WHERE codigotipoinstrumentolegal = '{$retorno->tiposInstrumentoLegalDTO->codigoTipoInstrumentoLegal}'");
			
			$sql = "INSERT INTO elabrev.ws_tiposinstrumentolegal(codigotipoinstrumentolegal, descricao, snativo)
					VALUES (
				  		'{$arrRetorno->tiposInstrumentoLegalDTO->codigoTipoInstrumentoLegal}',
				  		'{$arrRetorno->tiposInstrumentoLegalDTO->descricao}',
				  		'{$arrRetorno->tiposInstrumentoLegalDTO->snAtivo}'
					)";
			$db->executar( $sql );
			$this->totalTipInstLegal++;
		}
		
		$db->commit();
		
		$arrTotal = array("Total classificacoesAlteracaoDTO: ".$this->totalClassificacao,
						  "Total situacoesPedidoAlteracaoDTO: ".$this->totalSitPedidoAlt,
						  "Total tiposAlteracaoDTO: ".$this->totalTipoAlteracao,
						  "Total tiposFonteRecursoDTO: ".$this->totalTipoFonteRecurso,
						  "Total tiposInstrumentoLegalDTO: ".$this->totalTipInstLegal
						);
		
		return $arrTotal;
	}
}
?>
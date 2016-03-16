<?php
class AlteracaoOrcamentariaDAO extends cls_banco {
	
	//acao, programa, unidade, localizador, exercicio
	public function manterCadastrarPedidoAlteracaoResponse( $arrDados = array(), $requisicao = '', $obCadastrarPedidoAlteracao, $strTotal ){
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
										    and d.tcrid = ( select tcrid from elabrev.tipocredito where tcrcod = replace('".$arrRegistro->codigoTipoAlteracao."','a','') and tcrano = '".$arrFisico->exercicio."' and tcrstatus = 'A' )
										    and d.mcrid = 28)";
				//ver($sql);
				self::executar( $sql );
				self::commit();
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
										    and d.tcrid = ( select tcrid from elabrev.tipocredito where tcrcod = replace('".$arrRegistro->codigoTipoAlteracao."','a','') and tcrano = '".$fisico->exercicio."' and tcrstatus = 'A' )
										    and d.mcrid = 28)";
					//ver($sql);
					self::executar( $sql );
					self::commit();
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
					
				self::executar( $sql );
				self::commit();
			}
			return 'erro';
		}
	}
	
	public function carregarPedido( $limit = '' ){
		$filtro = '';
		if( $limit ) $filtro = " limit $limit ";
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
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '2012' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d 
				    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod like '26%' and a.prgano = '2012' and a.acasnrap = 'f' 
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
					and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				    
				union all 
				 
				SELECT
					d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma, 
				    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, un.unicod||' - '||un.unidsc as descricao,
				    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa,
				    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza,
				    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '2012' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d
					INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod like '26%' and a.prgano = '2012' and a.acasnrap = 'f' 
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
				    and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				) as foo
				where 
					identificadorunico is null
					--  and codigotipoalteracao != '175a' 
					 -- and codigotipoalteracao != '103a'
				group by 
					codigoclassificacaoalteracao, codigoMomento, codigoOrgao, codigoTipoAlteracao, descricao, exercicio, 
					identificadorunico, idcredito, mcrid, codigouo $filtro";
		
		$arrDados = self::carregar( $sql );
		$arrDados = $arrDados ? $arrDados : array();
		return $arrDados;
	}
	
	public function carregarFisico($codigouo, $codigotipoalteracao){
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
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '2012' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d 
				    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod like '26%' and a.prgano = '2012' and a.acasnrap = 'f' 
				        															--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' ) 
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				        															--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920') 
				    INNER JOIN public.idoc 				 i  on i.idoid = d.idoid 
				    LEFT JOIN public.naturezadespesa 	 n  on n.ndpid = d.ndpid 
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid 
				WHERE 
					d.dpavalor > 0 
				    and d.mcrid = 28
					and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				    
				union all 
				 
				SELECT
					d.dpaidentificadorunico as identificadorunico, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma, 
				    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, 'Integração - 01' as descricao,
				    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa,
				    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza,
				    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '2012' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d
					INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod like '26%' and a.prgano = '2012' and a.acasnrap = 'f' 
				    																--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' )
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				    																--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920')
				    INNER JOIN public.idoc 				 i on i.idoid = d.idoid
				    LEFT JOIN public.naturezadespesa 	 n on n.ndpid = d.ndpid
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid
				WHERE 
					d.dpavalor < 0 
				    and d.mcrid = 28
				    and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				) as foo
				where --codigouo = '26239' 
					identificadorunico is null
                    and codigouo = '$codigouo'
                    and codigotipoalteracao = '$codigotipoalteracao'
                	--and codigotipoalteracao <> '103a' -- and codigoacao = '4002' and codigouo = '26278'
				group by 
					identificadorunico, codigoacao, codigoesfera, codigofuncao, codigolocalizador, codigoprograma, codigosubfuncao, 
                    codigotipoinclusaolocalizador, codigouo, exercicio, quantidadeacrescimo, quantidadereducao, codigotipoalteracao
				order by codigouo";
		
		$arrDados = self::carregar( $sql );
		$arrDados = $arrDados ? $arrDados : array();
		return $arrDados;
	}
	
	public function carregarTotalCredito( $limit ){
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
									    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod like '26%' and a.prgano = '2012' and a.acasnrap = 'f' 
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
										and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
									    
									union all 
									 
									SELECT
										d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, a.unicod as codigouo, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao
									FROM elabrev.despesaacao d
										INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod like '26%' and a.prgano = '2012' and a.acasnrap = 'f' 
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
									    and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
									) as foo
									where 
										identificadorunico is null
									group by 
										codigoTipoAlteracao, identificadorunico, idcredito, codigouo $filtro
					) as foo1";
		
		$total = self::pegaUm( $sql );		
		return $total;
	}
	
	public function carregarFinanceiro($codigouo, $codigotipoalteracao, $codigoacao){
		$sql = "SELECT 
					identificadorunico, codigouo, codigotipoalteracao, codigofonte, 
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
				    d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma,  
				    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, 'Integração - 01' as descricao,
				    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa, 
				    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza, 
				    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '2012' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d 
				    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod like '26%' and a.prgano = '2012' and a.acasnrap = 'f' 
				        															--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' ) 
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				        															--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920') 
				    INNER JOIN public.idoc 				 i  on i.idoid = d.idoid 
				    LEFT JOIN public.naturezadespesa 	 n  on n.ndpid = d.ndpid 
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid 
				WHERE 
					d.dpavalor > 0 
				    and d.mcrid = 28
					and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				    
				union all 
				 
				SELECT
					d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma, 
				    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, 'Integração - 01' as descricao,
				    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa,
				    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza,
				    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '2012' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d
					INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod like '26%' and a.prgano = '2012' and a.acasnrap = 'f' 
				    																--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' )
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				    																--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920')
				    INNER JOIN public.idoc 				 i on i.idoid = d.idoid
				    LEFT JOIN public.naturezadespesa 	 n on n.ndpid = d.ndpid
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid
				WHERE 
					d.dpavalor < 0 
				    and d.mcrid = 28
				    and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				) as foo
				where 
					identificadorunico is null
                    and codigouo = '$codigouo'
                    and codigotipoalteracao = '$codigotipoalteracao'
                    and codigoacao = '$codigoacao'
				group by 
					identificadorunico, codigouo, codigotipoalteracao, codigofonte, 
                    codigoidoc, codigoiduso, codigonatureza, codigorp, codigotipofonterecurso, codigoacao
				order by codigouo, codigotipoalteracao";
		
		$arrDados = self::carregar( $sql );
		$arrDados = $arrDados ? $arrDados : array();
		return $arrDados;
	}
	public function carregarPedidoAlteracao(){		
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
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '2012' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d 
				    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod like '26%' and a.prgano = '2012' and a.acasnrap = 'f' 
				        															--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' ) 
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				        															--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920') 
				    INNER JOIN public.idoc 				 i  on i.idoid = d.idoid 
				    LEFT JOIN public.naturezadespesa 	 n  on n.ndpid = d.ndpid 
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid 
				WHERE 
					d.dpavalor > 0 
				    and d.mcrid = 28
					and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
				    
				union all 
				 
				SELECT
					d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma, 
				    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, 'Integração - 01' as descricao,
				    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa,
				    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza,
				    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '2012' as exercicio, '6' as codigotipoinclusaolocalizador,
                    '26000' as codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
				FROM elabrev.despesaacao d
					INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod like '26%' and a.prgano = '2012' and a.acasnrap = 'f' 
				    																--and a.acacod not in ( '2004', '2010', '2011', '2012', '20CW', '20CE', '09HB', '0181' )
				    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
				    																--and tc.tcrcod in ('100', '102', '103', '107', '110', '111', '112', '120', '150', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920')
				    INNER JOIN public.idoc 				 i on i.idoid = d.idoid
				    LEFT JOIN public.naturezadespesa 	 n on n.ndpid = d.ndpid
				    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid
				WHERE 
					d.dpavalor < 0 
				    and d.mcrid = 28
				    and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
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
		
		$arrDados = self::carregar( $sql );
		$arrDados = $arrDados ? $arrDados : array();
		return $arrDados;
	}
	
	function getTipoInclusaoLocalizador($arPedido = array(), $arFisico = array()){
		extract($arPedido);
		extract($arFisico);
		
		$sql = "SELECT acatipoinclusao FROM monitora.acao 
				WHERE prgano ='$exercicio' and acasnrap = false 
					and esfcod = $codigoesfera and unicod = '$codigouo' and funcod = '$codigofuncao' 
					and sfucod = '$codigosubfuncao' and prgcod = '$codigoprograma' 
				    and acacod = '$codigoacao' and loccod = '$codigolocalizador'";
		
		$acatipoinclusao = self::pegaUm( $sql );
		return $acatipoinclusao;
	}
	
	public function carregarPerguntaJustificativa( $dados = array() ){
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
		
		$arrDados = self::pegaLinha( $sql );		
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
}
?>
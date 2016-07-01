select  'EYX' as login, 
							MD5('COISUCA') as senha, 
							'26000' as orgao, 
							acao.acaid, 
							acao.acaid::varchar(5)||case when acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nd.ndpcod, 1,2)<>'31' then '1' else tda.tpdid::varchar(2) end as codigo,							
							acao.esfcod, 
							acao.unicod, 
							acao.funcod, 
							acao.sfucod, 
							acao.prgcod, 
							acao.acacod, 
							acao.loccod, 
							case when acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nd.ndpcod, 1,2)<>'31' then 1 else tda.tpdid end as tipodetalhamento, 
							Case 
								when tda.tpdid in (1,2,3,4,6,7,8,10,12) and trim(coalesce(acadscunmsof,'')) <> '' then coalesce(acaqtdefisico,1) 
								when tda.tpdid in (1,2,3,4,6,7,8,10,12) and trim(coalesce(acadscunmsof,'')) = '' then null 
								when tda.tpdid in (5) and acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','00H1') and substr(nd.ndpcod, 1,2)<>'31' and trim(coalesce(acadscunmsof,'')) <> '' then coalesce(acaqtdefisico,1)
								when tda.tpdid in (5) and acao.acacod in ('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110','0181','00H1') and substr(nd.ndpcod, 1,2)<>'31' and trim(coalesce(acadscunmsof,'')) = '' then null
								else coalesce(acaqtdefisico,1) end as quantidadefisico, 
							z.valor as valorfisico, 
							justificativa as justificativa, 
							'2012' as ano, 
							da.iducod, 
							idoc.idocod, 
							substr(nd.ndpcod,1,6)||'00' as ndpcod, 
							da.foncod, 
							coalesce ( SUM(da.dpavalor), 0 ) as valor, 
						
							'' as nrcod, 
							0 as valor_receita 
						 from elabrev.despesaacao da 
						 	inner join elabrev.ppaacao_orcamento acao on acao.acaid = da.acaid 
						 	inner join ( select a.acaid, coalesce ( SUM(a.dpavalor), 0 ) as valor from elabrev.despesaacao a 
						 							 	inner join elabrev.ppaacao_orcamento acao on acao.acaid = a.acaid 
						 								inner join naturezadespesa nd ON nd.ndpid = a.ndpid 
																		where prgano ='2011' and substr(nd.ndpcod, 1,2)<>'31'
																	group by a.acaid ) z ON z.acaid = da.acaid
							inner join naturezadespesa nd ON nd.ndpid = da.ndpid 
							inner join idoc on idoc.idoid = da.idoid 
							inner join ( select ao.acaid, max(tpdid) as tpdid from elabrev.tipodetalhamentoacao a 
									inner join elabrev.ppaacao_orcamento ao ON ao.acaid = a.acaid 
									where ao.prgano ='2011' 
									group by ao.acaid ) tda ON tda.acaid = acao.acaid
							--left join naturezareceita nr ON nr.nrcid = da.nrcid
							where 1=1  and da.ppoid = 159 and acao.acacod not in ( '2004','2010','2011','2012','20CW','20CE', '0284' ) and substr(nd.ndpcod, 1,2)<>'31' and nd.ndpcod not in  (".$ndps.")
						 group by 
							acao.acaid,
							 acao.esfcod, 
							acao.unicod, 
							acao.funcod, 
							acao.sfucod, 	
							acao.prgcod, 
							acao.acacod, 
							acao.loccod, 
							tda.tpdid,
							acaqtdefisico , 
							acaqtdefinanceiro , 
							justificativa ,	
							substr(nd.ndpcod,1,2),
							substr(nd.ndpcod,1,6)||'00', 
							da.iducod, 
							da.foncod, 
							idoc.idocod, 
							acao.acaqtdefisico, 
							acao.tdecod, 
							acao.justificativa,
							acadscunmsof,
							z.valor
<?php
 
// configurações
ini_set("memory_limit", "3000M");
set_time_limit(30000);

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
//include_once "config.inc";
include_once "/var/www/simec/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf']) $_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

include_once APPRAIZ . "www/sic/_constantes.php";

$sql = "select
 
			sc.slcid,
			
			case when slcdtinclusao is not null and slcprorrogado = 't' 
					then
					
					to_char((
						 	case when extract('dow' from slcdtinclusao+30) BETWEEN 1 AND 5 AND slcdtinclusao+30 not in (select feddata from public.feriados)
								then slcdtinclusao+30
							else
								case when extract('dow' from slcdtinclusao+31) BETWEEN 1 AND 5 AND slcdtinclusao+31 not in (select feddata from public.feriados)
									then slcdtinclusao+31
								else
									case when extract('dow' from slcdtinclusao+32) BETWEEN 1 AND 5 AND slcdtinclusao+32 not in (select feddata from public.feriados)
										then slcdtinclusao+32
									else
										case when extract('dow' from slcdtinclusao+33) BETWEEN 1 AND 5 AND slcdtinclusao+33 not in (select feddata from public.feriados)
											then slcdtinclusao+33
										else
											case when extract('dow' from slcdtinclusao+34) BETWEEN 1 AND 5 AND slcdtinclusao+34 not in (select feddata from public.feriados)
												then slcdtinclusao+34
											else
												case when extract('dow' from slcdtinclusao+35) BETWEEN 1 AND 5 AND slcdtinclusao+35 not in (select feddata from public.feriados)
													then slcdtinclusao+35
												end
											end
										end
									end
								end				
							end
						)::date, 'dd/MM/yyyy')
						
				 when slcdtinclusao is not null and (slcprorrogado = 'f' or slcprorrogado is null) 
				 	then 
				 		to_char((
						 	case when extract('dow' from slcdtinclusao+20) BETWEEN 1 AND 5 AND slcdtinclusao+20 not in (select feddata from public.feriados)
								then slcdtinclusao+20
							else
								case when extract('dow' from slcdtinclusao+21) BETWEEN 1 AND 5 AND slcdtinclusao+21 not in (select feddata from public.feriados)
									then slcdtinclusao+21
								else
									case when extract('dow' from slcdtinclusao+22) BETWEEN 1 AND 5 AND slcdtinclusao+22 not in (select feddata from public.feriados)
										then slcdtinclusao+22
									else
										case when extract('dow' from slcdtinclusao+23) BETWEEN 1 AND 5 AND slcdtinclusao+23 not in (select feddata from public.feriados)
											then slcdtinclusao+23
										else
											case when extract('dow' from slcdtinclusao+24) BETWEEN 1 AND 5 AND slcdtinclusao+24 not in (select feddata from public.feriados)
												then slcdtinclusao+24
											else
												case when extract('dow' from slcdtinclusao+25) BETWEEN 1 AND 5 AND slcdtinclusao+25 not in (select feddata from public.feriados)
													then slcdtinclusao+25
												end
											end
										end
									end
								end				
							end
						)::date, 'dd/MM/yyyy')
			else to_char(slcdtinclusao, 'dd/MM/yyyy') end as data_resposta,
					
			case when slcdtinclusao is not null then
				case when slcprorrogado = 't' then			
					case when 
							((
							 	case when extract('dow' from slcdtinclusao+30) BETWEEN 1 AND 5 AND slcdtinclusao+30 not in (select feddata from public.feriados)
									then slcdtinclusao+30
								else
									case when extract('dow' from slcdtinclusao+31) BETWEEN 1 AND 5 AND slcdtinclusao+31 not in (select feddata from public.feriados)
										then slcdtinclusao+31
									else
										case when extract('dow' from slcdtinclusao+32) BETWEEN 1 AND 5 AND slcdtinclusao+32 not in (select feddata from public.feriados)
											then slcdtinclusao+32
										else
											case when extract('dow' from slcdtinclusao+33) BETWEEN 1 AND 5 AND slcdtinclusao+33 not in (select feddata from public.feriados)
												then slcdtinclusao+33
											else
												case when extract('dow' from slcdtinclusao+34) BETWEEN 1 AND 5 AND slcdtinclusao+34 not in (select feddata from public.feriados)
													then slcdtinclusao+34
												else
													case when extract('dow' from slcdtinclusao+35) BETWEEN 1 AND 5 AND slcdtinclusao+35 not in (select feddata from public.feriados)
														then slcdtinclusao+35
													end
												end
											end
										end
									end				
								end
							)::date)-current_date < 0 							  
						then 0
					else
							((
							 	case when extract('dow' from slcdtinclusao+30) BETWEEN 1 AND 5 AND slcdtinclusao+30 not in (select feddata from public.feriados)
									then slcdtinclusao+30
								else
									case when extract('dow' from slcdtinclusao+31) BETWEEN 1 AND 5 AND slcdtinclusao+31 not in (select feddata from public.feriados)
										then slcdtinclusao+31
									else
										case when extract('dow' from slcdtinclusao+32) BETWEEN 1 AND 5 AND slcdtinclusao+32 not in (select feddata from public.feriados)
											then slcdtinclusao+32
										else
											case when extract('dow' from slcdtinclusao+33) BETWEEN 1 AND 5 AND slcdtinclusao+33 not in (select feddata from public.feriados)
												then slcdtinclusao+33
											else
												case when extract('dow' from slcdtinclusao+34) BETWEEN 1 AND 5 AND slcdtinclusao+34 not in (select feddata from public.feriados)
													then slcdtinclusao+34
												else
													case when extract('dow' from slcdtinclusao+35) BETWEEN 1 AND 5 AND slcdtinclusao+35 not in (select feddata from public.feriados)
														then slcdtinclusao+35
													end
												end
											end
										end
									end				
								end
							)::date)-current_date
						end
				else
					case when 
							((
							 	case when extract('dow' from slcdtinclusao+20) BETWEEN 1 AND 5 AND slcdtinclusao+15 not in (select feddata from public.feriados)
									then slcdtinclusao+20
								else
									case when extract('dow' from slcdtinclusao+21) BETWEEN 1 AND 5 AND slcdtinclusao+16 not in (select feddata from public.feriados)
										then slcdtinclusao+21
									else
										case when extract('dow' from slcdtinclusao+22) BETWEEN 1 AND 5 AND slcdtinclusao+17 not in (select feddata from public.feriados)
											then slcdtinclusao+22
										else
											case when extract('dow' from slcdtinclusao+23) BETWEEN 1 AND 5 AND slcdtinclusao+18 not in (select feddata from public.feriados)
												then slcdtinclusao+23
											else
												case when extract('dow' from slcdtinclusao+24) BETWEEN 1 AND 5 AND slcdtinclusao+19 not in (select feddata from public.feriados)
													then slcdtinclusao+24
												else
													case when extract('dow' from slcdtinclusao+25) BETWEEN 1 AND 5 AND slcdtinclusao+20 not in (select feddata from public.feriados)
														then slcdtinclusao+25
													end
												end
											end
										end
									end				
								end
							)::date)-current_date < 0 
						then 0
					else
							((
							 	case when extract('dow' from slcdtinclusao+20) BETWEEN 1 AND 5 AND slcdtinclusao+15 not in (select feddata from public.feriados)
									then slcdtinclusao+20
								else
									case when extract('dow' from slcdtinclusao+21) BETWEEN 1 AND 5 AND slcdtinclusao+16 not in (select feddata from public.feriados)
										then slcdtinclusao+21
									else
										case when extract('dow' from slcdtinclusao+22) BETWEEN 1 AND 5 AND slcdtinclusao+17 not in (select feddata from public.feriados)
											then slcdtinclusao+22
										else
											case when extract('dow' from slcdtinclusao+23) BETWEEN 1 AND 5 AND slcdtinclusao+18 not in (select feddata from public.feriados)
												then slcdtinclusao+23
											else
												case when extract('dow' from slcdtinclusao+24) BETWEEN 1 AND 5 AND slcdtinclusao+19 not in (select feddata from public.feriados)
													then slcdtinclusao+24
												else
													case when extract('dow' from slcdtinclusao+25) BETWEEN 1 AND 5 AND slcdtinclusao+20 not in (select feddata from public.feriados)
														then slcdtinclusao+25
													end
												end
											end
										end
									end				
								end
							)::date)-current_date
						 end
				end
			else
				0 end as dias
		from
			sic.solicitacao sc
		left join 
			entidade.entidade ent on ent.entid = sc.entid
		left join 
			entidade.endereco ede on ede.entid = sc.entid
		left join 
			territorios.municipio mun on mun.muncod = ede.muncod
		left join 
			workflow.documento doc on doc.docid = sc.docid
		left join 
			workflow.estadodocumento esd on esd.esdid = doc.esdid		
		where 
			slcstatus = 'A'
		and
			doc.esdid = ".WF_ESDID_ANALISE_AREA_RESPONSAVEL."
		order by
			ent.entnome, ede.estuf, mun.mundescricao";

$rs = $db->carregar($sql);

if($rs){
	
	foreach($rs as $dados){
		
		if($dados['dias'] <= 7){
			
			$arEmail[] = EMAIL_PRINCIPAL_SIC;
		
			$sql = "select secemail, slcnumsic, slcpergunta from sic.solicitacao sc
					inner join sic.secretaria se on se.secid = sc.secid
					where sc.slcid = {$dados['slcid']}";
			
			$solicitacao = $db->pegaLinha($sql);
			
			if($emailSecretaria){
				$arEmail[] = $solicitacao['secemail']; 
			}
		
			$sql = "select 
						usuemail 
					from sic.solicitacao sc
					inner join sic.usuarioresponsabilidade ur on sc.secid = ur.secid
					inner join seguranca.usuario us on us.usucpf = ur.usucpf
					where sc.slcid = {$dados['slcid']}
					AND ur.rpustatus = 'A'";
			
			$responsaveis = $db->carregar($sql);
			
			if($responsaveis){
				foreach($responsaveis as $responsavel){
					$arEmail[] = $responsavel['usuemail'];
				}
			}
			
			$remetente 	= ''; 
			$assunto	= 'SIC: Atenção com o prazo no protocolo '.$solicitacao['slcnumsic'];
			 
			$conteudo	= ' 
							<p>Prezados,</p>

							<p>O protocolo '.$solicitacao['slcnumsic'].' está com '.$dados['dias'].' dias para expirar o prazo. Favor providenciar a resposta.</p>
							
							<p><b>Pergunta:</b>&nbsp;'.$solicitacao['slcpergunta'].'</p>
						  ';
			 
			$cc			= array($_SESSION['email_sistema']);
			$cco		= ''; 
			$arquivos 	= array();
					
			enviar_email( $remetente, $arEmail, $assunto, $conteudo, $cc, $cco, $arquivos );
			
			$arEmail = array();
		}		
	}
}
	
?>
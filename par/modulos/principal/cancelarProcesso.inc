<?php

/** 
 * @desc Classe parte de Controle da tela de cancelamento de processos
 * @author EliasOliveira
 * @since 04/08/2014
 */
class cancelarProcesso
{
	
	// Variavel do banco
	public $db;
	//Construtor 
	public function __construct(&$db = null)
	{
		if($db){
			$this->db = $db;			
		} else {
			$this->db = new cls_banco(); 
		}
	}
	
	// Função que inativa o processo do PAR
	function inativaProcessoPar( $prp, $nprocesso = '' )
	{
		$sql = "UPDATE par.processopar SET prpstatus = 'I' WHERE prpid = {$prp}";
	
		if($this->db->executar( $sql ))
		{
			$sql2 = "UPDATE par.processoparcomposicao SET ppcstatus = 'I' WHERE prpid = {$prp}";
			if($this->db->executar( $sql2)) 
			{
				$this->db->commit();
				echo "Processo  PAR nº{$nprocesso} inativado com sucesso";
			}
			else
			{
				echo "Houve um erro ao tentar cancelar o processo Obras PAR nº {$nprocesso}";
			}
			
		}
		else
		{
			echo "Houve um erro ao tentar cancelar o processo Obras PAR nº {$nprocesso}";
		}
	}
	
	
	// Função que inativa o processo de obras do PAR
	function inativaProcessoParObras($pro, $nprocesso = '')
	{
		$sql = "UPDATE  par.processoobraspar SET prostatus = 'I' WHERE proid = {$pro}";
	
		if($this->db->executar( $sql ))
		{
			$sql2 = "UPDATE  par.processoobrasparcomposicao SET pocstatus = 'I' WHERE proid = {$pro}";
			if($this->db->executar( $sql2)) 
			{
				$this->db->commit();
				echo "Processo Obras PAR nº {$nprocesso} inativado com sucesso";
			}
			else 
			{
				echo "Houve um erro ao tentar cancelar o processo Obras PAR nº {$nprocesso}";
			}
		}
		else
		{
			echo "Houve um erro ao tentar cancelar o processo Obras PAR nº {$nprocesso}";
		}
	}
	
	// Função que inativa o processo obras do PAC
	function inativaProcessoPac($pro, $nprocesso = '')
	{
		$sql = "UPDATE par.processoobra SET prostatus = 'I' WHERE proid = {$pro}";
	
		if($this->db->executar( $sql ))
		{
			$sql2 = "UPDATE  par.processoobraspaccomposicao SET pocstatus = 'I' WHERE proid =  {$pro}";
			if($this->db->executar( $sql2)) 
			{
				$this->db->commit();
				echo "Processo Obras PAC nº {$nprocesso} inativado com sucesso";
			}
			else
			{
				echo "Houve um erro ao tentar cancelar o processo Obras PAR nº {$nprocesso}";
			}
		}
		else
		{
			echo "Houve um erro ao tentar cancelar o processo Obras PAR nº {$nprocesso}";
		}
	}
	
	// Carrega itens subações par
	function carregaDadosItens()
	{
	
		$cronograma = $this->db->pegaUm( "SELECT sbacronograma FROM par.subacao s INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid WHERE sd.sbdid = ".$_POST['sbdid'] );
			        
		if( $cronograma == 1 ){ // GLOBAL
		
		$sql = "SELECT DISTINCT
					sic.icodescricao,
					sic.icoquantidadetecnico as quantidade,
					TO_CHAR( sic.icovalor, '999G999G999D99'),
					TO_CHAR( ( sic.icoquantidadetecnico * sic.icovalor), '999G999G999D99') as valortotal,
	                emi.adesao
				FROM
					par.subacaoitenscomposicao sic
				INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid
				INNER JOIN par.subacaodetalhe sd ON sd.sbaid = sic.sbaid AND sd.sbdano = sic.icoano
				LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = sic.sbaid AND emi.epistatus = 'A' AND emi.epiano = sd.sbdano
	            --LEFT JOIN par.processoitemadesaopregao piap ON sic.icoid = piap.icoid AND piap.piastatus = 'A'
				WHERE
					sic.icovalidatecnico = 'S' AND
					sd.sbdid = ".$_POST['sbdid'];
	
		} else { // ESCOLA
			
		$sql = "SELECT          
	                        sic.icodescricao,
	                        par.recuperaquantidadeitemvalidado(sic.icoid) as quantidade,
	                        TO_CHAR( sic.icovalor, '999G999G999D99'),
	                        TO_CHAR(SUM( sse.seiqtdtecnico * sic.icovalor ), '999G999G999D99') as valortotal,
	                        emi.adesao
	                FROM
	                        par.subacaoitenscomposicao sic
	                INNER JOIN par.subescolas_subitenscomposicao  sse ON sse.icoid = sic.icoid
	                INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid
					INNER JOIN par.subacaodetalhe sd ON sd.sbaid = sic.sbaid AND sd.sbdano = sic.icoano
					LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = sic.sbaid AND emi.epistatus = 'A' AND emi.epiano = sd.sbdano
	                --LEFT JOIN par.processoitemadesaopregao piap ON sic.icoid = piap.icoid
	                WHERE
	                        sic.icovalidatecnico = 'S' AND
	                        sd.sbdid = ".$_POST['sbdid']."
	                GROUP BY
	                        sic.icodescricao, sic.icovalor, sic.icoquantidadetecnico, emi.adesao, sic.icoid";
		}
	
		$cabecalho = array("Descrição do Item", "Quantidade", "Valor", "Valor Total", 'Número da adesão' );
		$this->db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N');

	}
	
	// Carrega subações da obra par	
	function carregaDadosSubacaoObrasPar(){
		global $db;
		
		$status = $_POST['processosinativos'];
		
		$sql = "SELECT DISTINCT
	                                '<center><img style=\"cursor:pointer\" title=\"Visualizar Subação\" src=\"/imagens/icone_lupa.png\" onclick=\"janela(\'par.php?modulo=principal/subacao&acao=A&sbaid=' || sd.sbaid || '\',800,600,\'Subação\');\" border=\"0\" />'
	                                ||
					'<img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbaid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaObrasPar(this.id,\'' || sd.sbdid || '\');\" border=\"0\" /></center>' as acao,
					s.sbadsc,
					sd.sbdano||'&nbsp;' as sbdano,
					TO_CHAR( SUM(pre.prevalorobra), '999G999G999D99') || '</td></tr>
									            	<tr style=\"display:none\" id=\"listaDocumentos5_' || sd.sbdid || '\" >
									            		<td id=\"trO5_' || sd.sbdid || '\" colspan=8 ></td>
									            </tr>' as valor
				FROM 
					par.processoobrasparcomposicao ppc
				INNER JOIN par.subacaoobra so ON so.preid = ppc.preid
				INNER JOIN par.subacaodetalhe sd ON sd.sbaid = so.sbaid AND sd.sbdano = so.sobano
				INNER JOIN par.subacao s ON s.sbaid = sd.sbaid AND s.sbastatus <> 'C'
				INNER JOIN obras.preobra pre ON pre.preid = ppc.preid
				WHERE
					ppc.proid = ".$_POST['proid']."
					and ppc.pocstatus = '{$status}'
				GROUP BY
					sd.sbaid,
					sd.sbdid,
					s.sbadsc,
					sd.sbdano";
	//ver(simec_htmlentities($sql));
		$cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação", "Ano da Subação", "Valor da Subação" );
		$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S');
	}
	
	// Carrega itens do obra par
	function carregaDadosObrasPar(){
		
		global $db;
		$sql = "SELECT
	                            
	                            pre.predescricao,
	                            pto.ptodescricao,
	                            prevalorobra
				FROM 
	                            par.subacaoobra so
				INNER JOIN par.subacaodetalhe sd ON sd.sbaid = so.sbaid AND sd.sbdano = so.sobano
				INNER JOIN par.subacao s ON s.sbaid = sd.sbaid AND s.sbastatus <> 'C'
				INNER JOIN obras.preobra pre ON pre.preid=so.preid AND pre.prestatus = 'A'
				inner join par.processoobrasparcomposicao pc on pc.preid = pre.preid and pc.pocstatus = 'A'
				INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
				WHERE
	                            sd.sbdid = ".$_POST['sbdid'];
	
		$cabecalho = array( "Descrição da Obra", "Tipo da Obra", "Valor da Obra" );
		$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S');
	}
	
	// Carrega subações par
	function carregaDadoSubacao(){
		
		
		$status = $_POST['processosinativos'];
		
		$sql = "SELECT DISTINCT
	                                '<img style=\"cursor:pointer\" title=\"Visualizar Subação\" src=\"/imagens/icone_lupa.png\" onclick=\"janela(\'par.php?modulo=principal/subacao&acao=A&inuid='|| pp.inuid || '&sbaid=' || sd.sbaid || '\',800,600,\'Subação\');\" border=\"0\" />' || (
					CASE WHEN epi.prpid IS NOT NULL THEN -- quer dizer que ele perdeu itens.
						'' --'<center><img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbdid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaItens(this.id,\'' || sd.sbdid || '\');\" border=\"0\" />&nbsp;<a href=\"javascript:aderirPregao(\'' || sd.sbdid || '\', \'' || pp.inuid || '\', \'' || ppc.prpid || '\')\" ><img src=\"../imagens/envio_sigarp2.png\" width=\"12px\" title=\"Aderir ao pregão\" border=\"0\"></a></center>'
					ELSE 
					--	CASE WHEN s.prgid = 153 THEN
					--		'<center><img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbdid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaItens(this.id,\'' || sd.sbdid || '\');\" border=\"0\" />&nbsp;<a href=\"javascript:aderirPregao(\'' || sd.sbdid || '\', \'' || pp.inuid || '\', \'' || ppc.prpid || '\')\" ><img src=\"../imagens/envio_sigarp2.png\" width=\"12px\" title=\"Aderir ao pregão\" border=\"0\"></a></center>'
					--	ELSE
							'<center><img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbdid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaItens(this.id,\'' || sd.sbdid || '\');\" border=\"0\" /></center>'
					--	END
					END ) AS acao,
					s.sbadsc,
					sd.sbdano||'&nbsp;' as sbdano,
					TO_CHAR( (SELECT par.recuperavalorvalidadossubacaoporano(sd.sbaid, sd.sbdano)), '999G999G999D99') || '</td></tr>
									            	<tr style=\"display:none\" id=\"listaDocumentos2_' || sd.sbdid || '\" >
									            		<td id=\"trI_' || sd.sbdid || '\" colspan=8 ></td>
									            </tr>' as valor
				FROM 
					par.processoparcomposicao ppc
				INNER JOIN par.processopar pp ON pp.prpid = ppc.prpid
				INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
				INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
				LEFT JOIN par.empenhosubacao ems ON ems.sbaid = sd.sbaid AND ems.eobano = sd.sbdano and eobstatus = 'A' 
				LEFT JOIN  par.empenhopregaoitemperdido epi ON epi.sbdid = sd.sbdid AND epi.prpid = ppc.prpid
				WHERE
					ppc.ppcstatus = '{$status}' and
					ppc.prpid = ".$_POST['prpid'];
					
		
		$cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação", "Ano da Subação", "Valor da Subação" );
		$this->db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S');
	
	}
	
	function carregaDadosSubacaoObras(){
		global $db;
		$status = $_POST['processosinativos'];
		
		$sql = "SELECT
	                                '<img border=\"0\" src=\"/imagens/icone_lupa.png\" onclick=\"abreReformulacao('||pre.preid||', '||pre.muncod||');\" style=\"cursor:pointer\" />' AS acao,
					pre.predescricao,
					pto.ptodescricao,
					prevalorobra
				FROM 
					par.processoobraspaccomposicao ppc
				INNER JOIN obras.preobra pre ON pre.preid=ppc.preid AND pre.prestatus = 'A'
				INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
				WHERE
					ppc.pocstatus = '{$status}' and
					ppc.proid = ".$_POST['proid'];
		
		$cabecalho = array("Ação","Descrição da Obra", "Tipo da Obra", "Valor da Obra" );
		$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S');

	}
	
	function getAspasBDItensArray($array)
	{
		$arrayRetorno = Array();
		foreach( $array as $k => $v )
		{
			$arrayRetorno[] = "'$v'";
		}
		return $arrayRetorno;
	}
	// Monta a lista dos processos que podem ser inativados de acordo com os filtros
	function montaListaProcesso($post)
	{   
		// Filtro uf
		$arrUfs 		= ( $post['estuf'][0] ) ? implode(", ", $this->getAspasBDItensArray($post['estuf'])) : false;
		// Filtro Município
		$arrMunicipios	= ( $post['muncod'][0] ) ? implode(", ", $this->getAspasBDItensArray($post['muncod'])) : false;
		// Filtro nº do processo
		$nprocesso				= ($post['numeroprocesso']) ? $post['numeroprocesso'] : false;
		// Filtro anoprocesso
		$anoprocesso				= ($post['anoprocesso']) ? $post['anoprocesso'] : false;
		// Filtro Esfera
		$esfera					= ($post['esfera']) ? $post['esfera'] : false;
		// Filtro inativos
		$inativados					= ($post['inativados']) ? $post['inativados'] : false;
		// Filtro classificacao
		//$classificacaoProcesso	= ($post['classificacaoprocesso']) ? $post['classificacaoprocesso'] : false;
		// Recupera o tipo de processo
		$tipoProcesso = ($post['tipoprocesso']) ? $post['tipoprocesso'] : '' ;
		// Trata PAR
		if( ($tipoProcesso == '') || ($tipoProcesso == 'PAR') )
		{
			
			$imagemMais = "<left><img style=\"cursor:pointer\" id=\"img_dimensao_' || prp.prpid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaSubacao(this.id,\'' || prp.prpid || '\');\" border=\"0\" />";
			if( $inativados )
			{
				$btnCancelar = "";
				$status = "I";
			}
			else
			{
				$status = "A";
				$btnCancelar = "<img style=\"cursor:pointer\" id=\"img_inativa_par_' || prp.prpid || '\" src=\"/imagens/excluir.gif\" 
						onclick=\"inativaProcessoPar( '|| prp.prpid|| ', 
													   \'' ||
						 									substring(prp.prpnumeroprocesso from 1 for 5)||'.'||
															substring(prp.prpnumeroprocesso from 6 for 6)||'/'||
															substring(prp.prpnumeroprocesso from 12 for 4)||'-'||
															substring(prp.prpnumeroprocesso from 16 for 2) 
															|| 
														'\' );\" border=\"0\" />";
			}
			
			$sqlPAR = "
				SELECT DISTINCT
						'&nbsp;'|| '$imagemMais' ||
					 	'&nbsp;' || '$btnCancelar' as
						acao,
						prp.prpnumeroprocesso, 
                		prp.prpcnpj,
						'PAR' AS tipo,
						'' AS classificacao,
						CASE WHEN iu.itrid = 2 THEN 'Municipal' ELSE 'Estadual' END AS esfera,
						CASE WHEN iu.itrid = 2 THEN iu.mun_estuf ELSE iu.estuf END AS uf,
						m.mundescricao,'</td>
										</tr>
						            	<tr style=\"display:none\" id=\"listaDocumentos_' || prpid || '\" >
						            		<td id=\"trV_' || prpid || '\" colspan=8 ></td>
						            </tr>'
					FROM 
						par.processopar prp 
					INNER JOIN par.instrumentounidade iu ON iu.inuid = prp.inuid
					LEFT JOIN territorios.municipio  m ON m.muncod = iu.muncod
					WHERE prp.prpnumeroprocesso NOT IN (  SELECT empnumeroprocesso FROM par.empenho WHERE empsituacao IN ( '2 - EFETIVADO','8 - SOLICITAÇÃO APROVADA' ) and empstatus = 'A' )
					AND prp.prpstatus = '{$status}'
					
					";
			
			
			if( $nprocesso )
			{
				$nprocesso = str_replace(array('.','/','-') ,array('','',''), $nprocesso);
				$whereClausePAR[] = " prp.prpnumeroprocesso = '{$nprocesso}'";
			}
			if( $esfera )
			{
				$whereClausePAR[] = " iu.itrid = {$esfera} ";
			}
			/*
			 *  // Filtro classificacao
			 if( $classificacaoProcesso )
			{
				$whereClausePAR[] = " tip.tipid = {$classificacaoProcesso} ";
			}
			*/
			$whereClausePAR[] = "  not   exists (
				select ppc.prpid from par.processoparcomposicao ppc
				INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
				INNER JOIN par.empenhosubacao emps ON emps.sbaid = sd.sbaid and emps.eobano = sd.sbdano
				INNER JOIN par.empenho emp ON emps.empid = emp.empid and empstatus = 'A' and eobstatus = 'A' and prp.prpnumeroprocesso = emp.empnumeroprocesso
				where ppc.prpid = prp.prpid AND emps.eobstatus = 'A'
				and (select sum(vrlempenhocancelado) from par.v_vrlempenhocancelado where processo = prp.prpnumeroprocesso) > 0
			)";
			if( $arrUfs )
			{
				$whereClausePAR[] = " 
					CASE WHEN ( iu.itrid = 1) THEN 
						iu.estuf in ({$arrUfs})
					WHEN ( iu.itrid = 2) THEN 
						iu.mun_estuf in ({$arrUfs}) 
					END
				";
			}
			if( $arrMunicipios )
			{
				$whereClausePAR[] = " 
					CASE WHEN ( iu.itrid = 2) THEN 
						m.muncod in ({$arrMunicipios}) 
					END";
			}
			if( $anoprocesso )
			{
				$whereClausePAR[] = " 
					substring(prp.prpnumeroprocesso from 12 for 4) = '{$anoprocesso}'	
				";
			}
			
			$whereClausePAR = ($whereClausePAR) ? $whereClausePAR : Array();
			
			if( count($whereClausePAR) )
			{
				foreach( $whereClausePAR as $kClause => $vClause )
				{
					$sqlPAR .= '
					AND 
						' . $vClause;
				}
			}
			$arrSql[] = $sqlPAR;
		}
		// Trata OBRAS PAR
		if( ($tipoProcesso == '') || ($tipoProcesso == 'OBRAS_PAR') )
		{
			
			
			$imagemMais = "<left><img style=\"cursor:pointer\" id=\"img_dimensao_' || pop.proid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaSubacaoObrasPar(this.id,\'' || pop.proid || '\');\" border=\"0\" /> ";
			if( $inativados )
			{
				$btnCancelar = "";
				$status = "I";
			}
			else
			{
				$status = "A";
				$btnCancelar = "<img style=\"cursor:pointer\" id=\"img_inativa_par_' || pop.proid || '\" src=\"/imagens/excluir.gif\" 
			
						onclick=\"inativaProcessoParObras( '|| pop.proid|| ', 
													   \'' ||
						 									substring(pop.pronumeroprocesso from 1 for 5)||'.'||
															substring(pop.pronumeroprocesso from 6 for 6)||'/'||
															substring(pop.pronumeroprocesso from 12 for 4)||'-'||
															substring(pop.pronumeroprocesso from 16 for 2) 
															|| 
														'\' );\" border=\"0\" />";
			}
			$sqlObrasPAR = "
			
				SELECT 
					DISTINCT
						'&nbsp;'|| '$imagemMais' ||
					 	'&nbsp;' || '$btnCancelar' 
					as acao, 
					pop.pronumeroprocesso,
                	pop.procnpj,
                	'Obras do PAR' AS tipo,
                	'' AS classificacao,
                	CASE WHEN iu.itrid = 2 THEN 'Municipal' ELSE 'Estadual' END AS esfera,
                	CASE WHEN iu.itrid = 2 THEN iu.mun_estuf ELSE iu.estuf END AS uf,
                	m.mundescricao, '</td></tr>
						            	<tr style=\"display:none\" id=\"listaDocumentos3_' || proid || '\" >
						            		<td id=\"trO_' || proid || '\" colspan=8 ></td>
						            </tr>' 
				FROM
					 par.processoobraspar pop 
				INNER JOIN par.instrumentounidade iu ON iu.inuid = pop.inuid
				LEFT JOIN territorios.municipio  m ON m.muncod = iu.muncod
				WHERE  pop.pronumeroprocesso NOT IN (  SELECT empnumeroprocesso FROM par.empenho WHERE empsituacao IN ( '2 - EFETIVADO','8 - SOLICITAÇÃO APROVADA' ) 
														and empstatus = 'A' 
														and (select sum(vrlempenhocancelado) from par.v_vrlempenhocancelado where processo = empnumeroprocesso) > 0
													)
				AND pop.prostatus = '{$status}'			
			";
			
			if( $nprocesso )
			{
				$nprocesso = str_replace(array('.','/','-') ,array('','',''), $nprocesso);
				$whereClauseOBRASPAR[] = " pop.pronumeroprocesso = '{$nprocesso}'";
			}
			if( $esfera )
			{
				$whereClauseOBRASPAR[] = " iu.itrid = {$esfera} ";
			}
			if( $anoprocesso )
			{
				$whereClauseOBRASPAR[] = " 
					substring(pop.pronumeroprocesso from 12 for 4) = '{$anoprocesso}'	
				";
			}
			/*
			 * // Filtro classificacao
			 * if( $classificacaoProcesso )
			{
				$whereClauseOBRASPAR[] = " tip.tipid = {$classificacaoProcesso} ";
			}*/
			if( $arrUfs )
			{
				$whereClauseOBRASPAR[] = " 
					CASE WHEN ( iu.itrid = 1) THEN 
						iu.estuf in ({$arrUfs})
					WHEN ( iu.itrid = 2) THEN 
						iu.mun_estuf in ({$arrUfs}) 
					END
				";
			}
			if( $arrMunicipios )
			{
				$whereClauseOBRASPAR[] = " 
					CASE WHEN ( iu.itrid = 2) THEN 
						m.muncod in ({$arrMunicipios}) 
					END";
			}
			
			$whereClauseOBRASPAR = ($whereClauseOBRASPAR) ? $whereClauseOBRASPAR : Array();
			
			if( count($whereClauseOBRASPAR) )
			{
				foreach( $whereClauseOBRASPAR as $kClause => $vClause )
				{
					$sqlObrasPAR .= '
					AND 
						' . $vClause;
				}
			}
			
			$arrSql[] = $sqlObrasPAR;
		}
		// Trata PAC
		if( ($tipoProcesso == '') || ($tipoProcesso == 'PAC') )
		{
			$imagemMais = "<img style=\"cursor:pointer\" id=\"img_dimensao_' || po.proid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaSubacaoObras(this.id,\'' || po.proid || '\');\" border=\"0\" /> ";
			if( $inativados )
			{
				$btnCancelar = "";
				$status = "I";
			}
			else
			{
				$status = "A";
				$btnCancelar = "<img style=\"cursor:pointer\" id=\"img_inativa_par_' || po.proid || '\" src=\"/imagens/excluir.gif\" 
						onclick=\"inativaProcessoPac( '|| po.proid|| ', 
													   \'' ||
						 									substring(po.pronumeroprocesso from 1 for 5)||'.'||
															substring(po.pronumeroprocesso from 6 for 6)||'/'||
															substring(po.pronumeroprocesso from 12 for 4)||'-'||
															substring(po.pronumeroprocesso from 16 for 2) 
															|| 
														'\' );\" border=\"0\" />";
			}
			$sqlPAC = "
			
				SELECT 
					DISTINCT
						'&nbsp;'|| '$imagemMais' ||
					 	'&nbsp;' || '$btnCancelar' 
					as acao, 
				 	po.pronumeroprocesso,
				 	po.procnpj,
				    'PAC2' AS tipo,
				    CASE WHEN protipo = 'Q' 
				    	THEN 'Quadra' 
				        	ELSE 
				            	CASE WHEN protipo = 'C' 
				                	THEN 'Cobertura' 
				                    	ELSE 'Proinfancia'
				                    END
				                END
					AS classificacao,
				    CASE WHEN po.muncod is not null THEN 'Municipal' ELSE 'Estadual' END AS esfera,
				    CASE WHEN po.muncod is not null THEN m.estuf ELSE po.estuf END AS uf,
				    m.mundescricao,'</td></tr>
						            	<tr style=\"display:none\" id=\"listaDocumentos4_' || po.proid || '\" >
						            		<td id=\"trO2_' || po.proid || '\" colspan=8 ></td>
						            </tr>'
				FROM par.processoobra  po 
				LEFT JOIN territorios.municipio  m ON m.muncod = po.muncod
				LEFT JOIN territorios.estado e ON e.estuf = po.estuf
				WHERE po.pronumeroprocesso NOT IN (  
					SELECT empnumeroprocesso FROM par.empenho WHERE empsituacao IN ( '2 - EFETIVADO','8 - SOLICITAÇÃO APROVADA' ) 
					and empstatus = 'A'
					and (select sum(vrlempenhocancelado) from par.v_vrlempenhocancelado where processo = empnumeroprocesso) > 0 
				)
			
				AND po.prostatus = '{$status}'			
			";
			if( $nprocesso )
			{
				$nprocesso = str_replace(array('.','/','-') ,array('','',''), $nprocesso);
				$whereClausePAC[] = " po.pronumeroprocesso = '{$nprocesso}'";
			}
			if( $esfera )
			{
				if($esfera == 2)
				{
					$whereClausePAC[] = " po.muncod is not null ";
				}
				else 
				{
					$whereClausePAC[] = " po.muncod is null ";					
				}
			}
			/*
			 *  // Filtro classificacao
			 if( $classificacaoProcesso )
			{
				$whereClausePAC[] = " tip.tipid = {$classificacaoProcesso} ";
			}
			*/
			if( $arrUfs )
			{
				$whereClausePAC[] = " 
					 CASE WHEN po.muncod is not null THEN m.estuf in ({$arrUfs})  ELSE po.estuf  in ({$arrUfs})  END 
				";
			}
			if( $arrMunicipios )
			{
				$whereClausePAC[] = " 
					
					CASE WHEN po.muncod is not null THEN m.muncod in ({$arrMunicipios}) END";
			}
			if( $anoprocesso )
			{
				$whereClausePAC[] = " 
					substring(po.pronumeroprocesso from 12 for 4) = '{$anoprocesso}'	
				";
			}
			
			$whereClausePAC = ($whereClausePAC) ? $whereClausePAC : Array();
			
			if( count($whereClausePAC) )
			{
				foreach( $whereClausePAC as $kClause => $vClause )
				{
					$sqlPAC .= '
					AND 
						' . $vClause;
				}
			}
			$arrSql[] = $sqlPAC;
		}
		
		if( count($arrSql) > 1 )
		{
			$i = 0;
			foreach($arrSql as $k => $sqlProc)
			{
				$i++;
				if( count($arrSql) == $i )
				{
					$sql .= $sqlProc;
				}
				else
				{
					$sql .= $sqlProc . '
					UNION ALL
					'
					;
				}
			}
		}
		else 
		{
			$sql = 	$arrSql[0];
		}
		
		$cabecalho = array("Ação","Nº Processo", "CNPJ", "Tipo", "Classificação","Esfera", "UF", "Município");
		$this->db->monta_lista($sql,$cabecalho,100,5,'N','center','N','formprocesso');
		
	}
	
	function retornaClassificacao()
	{
		/// Refazer filtro pois classificação será diferente
		/*
		$tipoProcesso = $_REQUEST['tipoprocesso'];
		
		switch ($tipoProcesso) {
		    case 'PAR':
				$sql = "select tipid as codigo, tipdescricao as descricao from execucaofinanceira.tipoprocesso  WHERE tipstatus = 'A' and tpdid = 62 ";
		    break;
		    
		    case 'OBRAS_PAR':
		    	$sql = "select tipid as codigo, tipdescricao as descricao from execucaofinanceira.tipoprocesso  WHERE tipstatus = 'A' and tpdid = 45";
		   	break;
		   	
		    case 'PAC':
		    	$sql = "select tipid as codigo, tipdescricao as descricao from execucaofinanceira.tipoprocesso  WHERE tipstatus = 'A' and tpdid = 37";	
		    break;
		    default:
		    	$sql = array();	
		    	
		}
		 $classificacaoprocesso = $_REQUEST['valuecp'];
		 echo $this->db->monta_combo( "classificacaoprocesso", $sql, 'S', 'Selecione a Classificação do Processo', '', '','','','','', $classificacaoprocesso );
		*/
	}
}

if ($_POST['ajaxUf']){
	
	if($_POST['ajaxUf'] != 'XX'){
		echo "<b>Estado(s): {$_POST['ajaxUf']}</b><br>";
		$stAnd = "AND estuf in ('".str_replace(",","','",$_POST['ajaxUf'])."')";
	}
	
	$sql_combo = "SELECT
						mun.muncod as codigo,
					    mun.muncod||' - '||mun.mundescricao as descricao
					FROM
						territorios.municipio mun
					where 1=1
						$stAnd
					ORDER BY
						mun.mundescricao asc";
	combo_popup( 'muncod', $sql_combo, 'Selecione o(s) Municípios(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);

	exit;
}

$obCancelaProcesso = new cancelarProcesso();
switch ($_REQUEST['requisicao'])
{
	
	case 'retornaClassificacao' :
		$obCancelaProcesso->retornaClassificacao();
		exit();
	break;
	case 'inativaProcessoPar' :
		$obCancelaProcesso->inativaProcessoPar( $_REQUEST['prpid'], $_REQUEST['nprocesso'] );
		exit();
	break;
	case 'inativaProcessoPac' :
		$obCancelaProcesso->inativaProcessoPac( $_REQUEST['proid'], $_REQUEST['nprocesso'] );
		exit();
	break;
	case 'inativaProcessoParObras' :
		$obCancelaProcesso->inativaProcessoParObras( $_REQUEST['proid'], $_REQUEST['nprocesso'] );
		exit();
	break;
	
	case 'carregaDadosSubacao' :
		$obCancelaProcesso->carregaDadoSubacao();
		exit();
	break;
	case 'carregaDadosItens' :
		$obCancelaProcesso->carregaDadosItens();
		exit();
	break;
	case 'carregaDadosSubacaoObrasPar' :
		$obCancelaProcesso->carregaDadosSubacaoObrasPar();
		exit();
	break;
	case 'carregaDadosObrasPar' :
		$obCancelaProcesso->carregaDadosObrasPar();
		exit();
	break;
	case 'carregaDadosSubacaoObras' :
		$obCancelaProcesso->carregaDadosSubacaoObras();
		exit();
	break;
		
}


include APPRAIZ."includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';
monta_titulo( 'Gestão de Processos', $titulo_modulo );

?>

<script type="text/javascript" src="/library/fancybox/helpers/jquery.mousewheel-3.0.6.pack.js"></script>


<script type="text/javascript" src="../includes/prototype.js"></script>

<script type="text/javascript" src="../includes/funcoes.js"></script>


<!-- Add fancyBox main JS and CSS files -->
<script type="text/javascript" src="/library/fancybox/jquery.fancybox.js"></script>
<link rel="stylesheet" type="text/css" href="/library/fancybox/jquery.fancybox.css" media="screen" />

<!-- Add Button helper (this is optional) -->
<link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-buttons.css" />
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-buttons.js"></script>

<style type="text/css">
.fancybox img{
	box-shadow: 1px 1px 4px 1px #000;
	width: 10px !important;
	height: 10px !important;
	margin-right: 10px;
}

.fancybox img {
    box-shadow: 0px 0px 0px 0px ;
    height:17px !important;
    margin-right: 0;
    width: 17px !important;
}
</style>

<script type="text/javascript">

$1_11(document).ready( function(){

	var valueTipoProcesso = $1_11("select[name='tipoprocesso']").val();
	
	if( valueTipoProcesso == '')
	{
		
		$1_11("#tr_classificacao").hide();
		$1_11("#span_classificacao").hide();
		$1_11("#span_classificacao").html('');
	}
	else
	{
		/*
		// Filtro classificação
		var valuecp = $1_11('#valuecp').val();
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=retornaClassificacao&tipoprocesso="+valueTipoProcesso+"&valuecp="+valuecp,
	   		async: false,
	   		success: function(msg){
	   			$1_11("#span_classificacao").html(msg);
	   			divCarregado();
	   		}
		});

		$1_11("#tr_classificacao").show();
		$1_11("#span_classificacao").show();
		*/
	}

});

function inativaProcessoPar( prpid, nprocesso )
{
	
	var r = confirm("Tem certeza que deseja inativar o processo " + nprocesso);
	if (r == true) {
		jQuery.ajax({
       		type: "POST",
       		url: window.location.href,
       		data: "requisicao=inativaProcessoPar&prpid="+prpid+"&nprocesso=" + nprocesso,
       		async: false,
       		success: function(msg){
       			alert(msg);
       			//document.location.href = document.location.href;
       			enviarFormulario('');
       		}
    	});
	} else {
	  
	}

}

function inativaProcessoParObras(  proid, nprocesso  )
{
	var r = confirm("Tem certeza que deseja inativar o processo " + nprocesso);
	if (r == true) {
		jQuery.ajax({
       		type: "POST",
       		url: window.location.href,
       		data: "requisicao=inativaProcessoParObras&proid="+proid+"&nprocesso==" + nprocesso,
       		async: false,
       		success: function(msg){
       			alert(msg);
       			//document.location.href = document.location.href;
       			enviarFormulario('');
       		}
    	});
	} else {
	  
	}
}

function inativaProcessoPac(  proid, nprocesso  )
{
	var r = confirm("Tem certeza que deseja inativar o processo " + nprocesso);
	if (r == true) {
		jQuery.ajax({
       		type: "POST",
       		url: window.location.href,
       		data: "requisicao=inativaProcessoPac&proid="+proid+"&nprocesso=" + nprocesso,
       		async: false,
       		success: function(msg){
       			alert(msg);
       			//document.location.href = document.location.href;
       			enviarFormulario('');
       		}
    	});
	} else {
	  
	}
}



function carregarListaSubacao(idImg, prpid){
	
	var img 	 = $1_11( '#'+idImg );
	var tr_nome = 'listaDocumentos_'+ prpid;
	var td_nome  = 'trV_'+ prpid;
	
	if($1_11('#'+tr_nome).css('display') == 'none' && $1_11('#'+td_nome).html() == ""){
		$1_11('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		
		carregaListaSubacao(prpid, td_nome);
	}
	if($1_11('#'+tr_nome).css('display') == 'none' && $1_11('#'+td_nome).html() != ""){
		$1_11('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else {
		$1_11('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}
function carregaListaSubacao(prpid, td_nome){
	divCarregando();

	if( jQuery('#inativados_submit').length > 0 ){
		var processosinativos = 'I';
	}
	else
	{
		var processosinativos = 'A';
	}
	
	jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=carregaDadosSubacao&prpid="+prpid+"&processosinativos="+processosinativos,
	   		async: false,
	   		success: function(msg){
	   			jQuery('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});

}

function carregarListaItens(idImg, sbdid){
	var img 	 = $1_11( '#'+idImg );
	var tr_nome = 'listaDocumentos2_'+ sbdid;
	var td_nome  = 'trI_'+ sbdid;

	if($1_11('#'+tr_nome).css('display') == 'none' && $1_11('#'+td_nome).html() == ""){
		$1_11('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaItens(sbdid, td_nome);
	}
	if($1_11('#'+tr_nome).css('display') == 'none' && $1_11('#'+td_nome).html() != ""){
		$1_11('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else {
		$1_11('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}

function carregaListaItens(sbdid, td_nome){
	//divCarregando();
		jQuery.ajax({
   			type: "POST",
   			url: window.location.href,
	   		data: "requisicao=carregaDadosItens&sbdid="+sbdid,
	   		async: false,
	   		success: function(msg){
	   			jQuery('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}


function carregarListaSubacaoObrasPar(idImg, proid){
	var img 	 = $1_11( '#'+idImg );
	var tr_nome = 'listaDocumentos3_'+ proid;
	var td_nome  = 'trO_'+ proid;
	
	if( $1_11('#'+tr_nome).css('display') == 'none' && $1_11('#'+td_nome).html() == ""){
		$1_11('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaSubacaoObrasPar(proid, td_nome);
	}
	if($1_11('#'+tr_nome).css('display') == 'none' && $1_11('#'+td_nome).html() != ""){
		$1_11('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else {
		$1_11('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}

function carregaListaSubacaoObrasPar(proid, td_nome){
	divCarregando();

	if( jQuery('#inativados_submit').length > 0 ){
		var processosinativos = 'I';
	}
	else
	{
		var processosinativos = 'A';
	}
	
	jQuery.ajax({
			type: "POST",
			url: window.location.href,
	   		data: "requisicao=carregaDadosSubacaoObrasPar&proid="+proid+"&processosinativos="+processosinativos,
	   		async: false,
	   		success: function(msg){
	   			jQuery('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}

function carregarListaObrasPar(idImg, sbdid){
	var img 	 = $1_11( '#'+idImg );
	var tr_nome = 'listaDocumentos5_'+ sbdid;
	var td_nome  = 'trO5_'+ sbdid;
	
	if($1_11('#'+tr_nome).css('display') == 'none' && $1_11('#'+td_nome).html() == ""){
		$1_11('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaObrasPar(sbdid, td_nome);
	}
	if($1_11('#'+tr_nome).css('display') == 'none' && $1_11('#'+td_nome).html() != ""){
		$1_11('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else {
		$1_11('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}


function carregaListaObrasPar(sbdid, td_nome){
	
	divCarregando();
		jQuery.ajax({
			type: "POST",
			url: window.location.href,
			data: "requisicao=carregaDadosObrasPar&sbdid="+sbdid,
	   		async: false,
	   		success: function(msg){
	   			jQuery('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}

function carregarListaSubacaoObras(idImg, proid){
	var img 	 = $1_11( '#'+idImg );
	var tr_nome = 'listaDocumentos4_'+ proid;
	var td_nome  = 'trO2_'+ proid;

	if($1_11('#'+tr_nome).css('display') == 'none' && $1_11('#'+td_nome).html() == ""){
		$1_11('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaSubacaoObras(proid, td_nome);
	}
	if($1_11('#'+tr_nome).css('display') == 'none' && $1_11('#'+td_nome).html() != ""){
		$1_11('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else {
		$1_11('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}


function carregaListaSubacaoObras(proid, td_nome){
	divCarregando();

		if( jQuery('#inativados_submit').length > 0 ){
			var processosinativos = 'I';
		}
		else
		{
			var processosinativos = 'A';
		}
		jQuery.ajax({
			type: "POST",
			url: window.location.href,
	   		data: "requisicao=carregaDadosSubacaoObras&proid="+proid+"&processosinativos="+processosinativos,
	   		async: false,
	   		success: function(msg){
	   			jQuery('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}


function abreReformulacao(preid, muncod) {
    return window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dados&preid='+preid+'&muncod='+muncod,
        'ProInfancia', "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
}

//Função responsável por fazer a requisição para o formulário 
function enviarFormulario(req){
	
	var formulario = document.formulario;
	selectAllOptions( formulario.muncod );
	selectAllOptions( formulario.estuf );
	// Recupera obj do formulário
	var formularioJquery = $1_11('#formulario');
	formularioJquery.submit(); 

}

function changeTipoProcesso( value )
{
	if( value == '')
	{
		
		$1_11("#tr_classificacao").hide();
		$1_11("#span_classificacao").hide();
		$1_11("#span_classificacao").html('');
	}
	else
	{
		/*
		//Filtro classificação
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=retornaClassificacao&tipoprocesso="+value,
	   		async: false,
	   		success: function(msg){
	   			$1_11("#span_classificacao").html(msg);
	   			divCarregado();
	   		}
		});

		$1_11("#tr_classificacao").show();
		$1_11("#span_classificacao").show();
		*/
	}
}

//Função que filtra os municípios 
function filtraMunicipio(){

		//mostraMunicipio();
		
		var uf = document.getElementById('estuf');
		var listauf = "";

		for(i=0;i<uf.length;i++){
			listauf = listauf + "," + uf[i].value;
		}

		if(listauf) listauf = listauf.substr(1);
		
		if(!listauf) listauf = 'XX';

		var req = new Ajax.Request('par.php?modulo=relatorio/relatorioProgramas&acao=A', {
				        method:     'post',
				        parameters: '&ajaxUf='+listauf,
				        onComplete: function (res)
				        {
							td_municipio.innerHTML = res.responseText;
				        }
					});

		//filta o campo escola
		var td_escolas   = document.getElementById('td_escolas');

		
}

// Mostra os municípios 
function mostraMunicipio(){

	//document.getElementById('agrupador').options.length=0;
	
	var tr_municipio   = document.getElementById('tr_municipio');
	var td_municipio   = document.getElementById('td_municipio');

	var agrupador = document.getElementById('agrupadorOrigem');
	var total = agrupador.length;

	var agrupador = document.getElementById('agrupador');
	var total = agrupador.length;

	for(i=0; i<total; i++){
		if(agrupador[i]){
			if(agrupador[i].value == 'coordenador') {
				document.getElementById('agrupador').remove(i);
			}
		}
	}
		
	var item = document.createElement('option');
	
	if(document.formulario.esfera[1].checked == true || document.formulario.esfera[2].checked == true){
		tr_municipio.style.display = '';
		td_municipio.style.display = '';

	}	
	else{
		td_municipio.style.display = 'none';
		tr_municipio.style.display = 'none';
		
		 item.text = 'Nome do Coordenador';
		 item.value = 'coordenador';
		 
		  try {
			document.getElementById('agrupadorOrigem').add(item, null); // standards compliant; doesn't work in IE
		  }
		  catch(ex) {
			  document.getElementById('agrupadorOrigem').add(item); // IE only
		  }
		
	}
	
}

</script>

<form method="post" name="formulario" id="formulario">
	<input type="hidden" name="pesquisa" value="1"/>
	<input type="hidden" name="valuecp" id="valuecp" value="<?=$_REQUEST['classificacaoprocesso']?>"/>
	<input type="hidden" id="exporta" name="exporta" value="<?=$exporta; ?>">
	
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
			       	<td align='right' class="SubTituloDireita">Número de Processo:</td>
			        <td>
			        	<?$filtro = simec_htmlentities( $_REQUEST['numeroprocesso'] );
							$numeroprocesso = $filtro;
							//echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '', '');
							echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', '','','','','','',$numeroprocesso);
						?>
						
		        	</td>
		      	</tr>
		      	<tr>
			        <td align='right'  class="SubTituloDireita">Tipo de Processo:</td>
			        <td>
			        	 <?php 
				     		
				        	 $sql = array(
				        	 	array('codigo'=> 'PAR', 		'descricao'=>'PAR'),
				        	 	array('codigo'=> 'OBRAS_PAR', 	'descricao'=>'OBRAS PAR'),
				        	 	array('codigo'=> 'PAC', 		'descricao'=>'PAC')
				        	 );
							$tprocesso = simec_htmlentities( $_REQUEST['tipoprocesso'] );			
							$db->monta_combo( "tipoprocesso", $sql, 'S', 'Selecione o Tipo de Processo', 'changeTipoProcesso', '' , '', '', '', '', '', $tprocesso);
						?>
			        </td>
		      	</tr>
		      	<tr style="display:none;" id="tr_classificacao">
			        <td align='right'  class="SubTituloDireita">Classificação do Processo:</td>
			        <td>
			        	 <span id="span_classificacao" style="display:none;">
			        	 		
			        	 </span>
			        </td>
		      	</tr>
		      	
		      	<tr>
			        <td align='right'  class="SubTituloDireita">Esfera:</td>
			        <td>
			        	 <?php 
				     		$sql = array(
				     			array('codigo'=> 1, 'descricao' => 'Estadual' ),
				     			array('codigo'=> 2, 'descricao' => 'Municipal' )
				     		
				     		);
							$esfera = simec_htmlentities( $_REQUEST['esfera'] );
							$db->monta_combo( "esfera", $sql, 'S', 'Selecione a esfera', '', '');
						?>
			        </td>
		      	</tr>
		      	
		      	<tr>
					<td class="SubTituloDireita" valign="top">Estado:</td>	
					<td >
						<?php
						$filtroEstuf = '';
						if( $_REQUEST['estuf'][0] ){
						
							$sql = "SELECT
										es.estuf as codigo,
									    es.estdescricao as descricao
									FROM
										territorios.estado es
 									WHERE
 										es.estuf in ('".implode("', '", $_REQUEST['estuf'])."')
									ORDER BY
										es.estuf asc";
							
							$estuf = $db->carregar($sql);
							
							$filtroEstuf = " and estuf in ('".implode("', '", $_REQUEST['estuf'])."')";
						}
						
						$sql_combo = "SELECT
										es.estuf as codigo,
									    es.estdescricao as descricao
									FROM
										territorios.estado es
									ORDER BY
										es.estuf asc;";
						combo_popup( 'estuf', $sql_combo, 'Selecione o(s) Estado(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, 'filtraMunicipio()', true);
						
				    	?>		
			
					</td>
				</tr>
				<tr id="tr_municipio" >
					<td class="SubTituloDireita" valign="top">Município:</td>	
					<td id="td_municipio">
						<?php

						if( $_REQUEST['muncod'][0] ){
						
							$sql = "SELECT
										mun.muncod as codigo,
									    mun.mundescricao as descricao
									FROM
										territorios.municipio mun
 									WHERE
 										mun.muncod in ('".implode("', '", $_REQUEST['muncod'])."')
									ORDER BY
										mun.mundescricao asc";
								
							$muncod = $db->carregar($sql);
						}
						
						$sql_combo = "SELECT
										mun.muncod as codigo,
									    mun.estuf||' - '||mun.mundescricao as descricao
									FROM
										territorios.municipio mun
									WHERE
										1=1
										$filtroEstuf
									ORDER BY
										mun.estuf, mun.mundescricao asc";
						
						combo_popup( 'muncod', $sql_combo, 'Selecione o(s) Municípios(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);			
						
						?>
					</td>
				</tr>
				<tr id="tr_municipio" >
					<td class="SubTituloDireita" valign="top">Processos Inativos:</td>	
					<td id="td_municipio">
						<?php if( $_REQUEST['inativados']) {?>			
						<input type="checkbox" value='1' name='inativados' id='inativados' checked="checked">
						<input type="hidden" value='1' name='inativados_submit' id='inativados_submit' >
						<?php }else{ ?> 
						<input type="checkbox" value='1' id='inativados' name='inativados'> 
						<?php } ?>
							
					</td>
				</tr>
				<tr id="tr_municipio" >
					<td class="SubTituloDireita" valign="top">Ano do processo:</td>	
					<td id="td_municipio">
						<input type="text" size="4" maxlength="4"  onblur="MouseBlur(this);this.value=mascaraglobal('[#]',this.value);this.value=mascaraglobal('[#]',this.value);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal('[#]',this.value);" value='<?=$_REQUEST['anoprocesso']?>' name='anoprocesso'>
					</td>
				</tr>
				<tr>
					<td colspan="2" align="center">
						<input type="button" name="pesquisar" value="Pesquisar" onclick="javascript:enviarFormulario();"/>
					</td>
				</tr>
		      	
 	</table>
</form>

<?php
	
	$obCancelaProcesso->montaListaProcesso($_REQUEST);
	
?>
</table>
<div id="divDebug"></div>
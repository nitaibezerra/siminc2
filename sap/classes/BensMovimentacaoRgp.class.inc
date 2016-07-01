<?php

/**
 * Modelo de Situacao do bem
 * @author Silas Matheus
 */
class BensMovimentacaoRgp extends Modelo{
	
	/**
	* Nome da tabela especificada
	* @name $stNomeTabela
	* @var string
	* @access protected
	*/
	protected $stNomeTabela = "sap.bensmovimentacaorgp";
	
	/**
	* Chave primária
	* @name $arChavePrimaria
	* @var array
	* @access protected
	*/
	protected $arChavePrimaria = array('mvbid', 'rgpid');
	
	/**
	* Atributos da Tabela
	* @name $arAtributos
	* @var array
	* @access protected
	*/
	protected $arAtributos = array(
		'mvbid' => null,
		'rgpid' => null,
		'bmvrstatus' => null
	);
	
	/**
	 * Monta grid de rgps para a movimentacao
	 * @name listarRgps
	 * @author Silas Matheus
	 * @access public
	 * @param int $mvbid - Código da Movimentação
	 * @return void
	 */
	public function listarRgps($mvbid,$pagina){
		
		$sql = "SELECT 
					(CASE WHEN (bmvrstatus = 'A') THEN
						'<center><a style=\"cursor:pointer;\" onclick=\"redirecionaExcluir(\'' || rgp.rgpid || '\',\'".$pagina."\')\">
							<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
						</a>'
					ELSE
						''
					END) as comandos,
					rgp.rgpnum||' ' AS rgpnum, 
					mat.matdsc,
					eco.ecodsc
				FROM 
					$this->stNomeTabela bmvr
				INNER JOIN
					sap.rgp rgp
					ON rgp.rgpid=bmvr.rgpid
				INNER JOIN
					sap.bensmaterial bmt
					ON bmt.bmtid=rgp.bmtid
				INNER JOIN
					sap.material mat
					ON mat.matid=bmt.matid
				INNER JOIN
					sap.estadomotivos esm
					ON esm.esmid=rgp.esmid
				INNER JOIN
					sap.estadoconservacao eco
					ON eco.ecoid=esm.ecoid
				WHERE 
					bmvr.mvbid=$mvbid
				ORDER BY 
					mat.matdsc ASC";
				
		$cabecalho = array("Comando", "RGP", "Material", "Estado de Conserva&ccedil;&atilde;o");
		$this->monta_lista($sql, $cabecalho, 20, 50, false, "center",'S','','',array('center','center','center','center'));
	
	}
	
	/**
	 * Monta grid de rgps para a movimentacao
	 * @name listarRgps
	 * @author Silas Matheus
	 * @access public
	 * @param int $mvbid - Código da Movimentação
	 * @return void
	 */
	public function listarRgpsGeral($mvbid,$pagina){
		
		$sql = "SELECT 
					'<center>
						<a style=\"cursor:pointer;\" onclick=\"redirecionaEditar(\'' || rgp.rgpid || '\')\">
							<img src=\"/imagens/editar_nome.gif\" border=\"0\" title=\"Alterar\" />
						</a>' || 
					(CASE WHEN (bmvrstatus = 'A') THEN
						'<a style=\"cursor:pointer;\" onclick=\"redirecionaExcluir(\'' || rgp.rgpid || '\',\'".$pagina."\')\">
							<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
						</a>'
					ELSE
						''
					END) as comandos,
					rgp.rgpnum||' ' AS rgpnum, 
					mat.matdsc,
					eco.ecodsc
				FROM 
					$this->stNomeTabela bmvr
				INNER JOIN
					sap.rgp rgp
					ON rgp.rgpid=bmvr.rgpid
				INNER JOIN
					sap.bensmaterial bmt
					ON bmt.bmtid=rgp.bmtid
				INNER JOIN
					sap.material mat
					ON mat.matid=bmt.matid
				INNER JOIN
					sap.estadomotivos esm
					ON esm.esmid=rgp.esmid
				INNER JOIN
					sap.estadoconservacao eco
					ON eco.ecoid=esm.ecoid
				WHERE 
					bmvr.mvbid=$mvbid
				ORDER BY 
					mat.matdsc ASC";
				
		$cabecalho = array("Comando", "RGP", "Material", "Estado de Conserva&ccedil;&atilde;o");
		$this->monta_lista($sql, $cabecalho, 20, 50, false, "center",'S','','',array('center','center','center','center'));
	
	}
	
	/**
	 * Exclui registro
	 * @name excluir
	 * @author Silas Matheus
	 * @access public
	 * @param int $rgpid - Identificador do RGP
	 * @param int $mvbid - Identificador da Movimentacao
	 * @return bool
	 */
	public function excluir($rgpid, $mvbid){
	
		$rgpid = addslashes($rgpid);
		$mvbid = addslashes($mvbid);
		
		
		$sql = "DELETE FROM 
					$this->stNomeTabela 
				WHERE 
					mvbid=$mvbid
				AND
					rgpid=$rgpid";
					
		// se excluir
		if($this->executar($sql)){
		
			$oRgp = new Rgp();
			
			//verifica se tem alguma movimentação anterior à corrente envolvendo o rgp
			//se houver, atualiza os registros. se não houver atualiza apenas a situação do rgp
			$oMovimentacao = new BensMovimentacao();
			$dadosMov = $oMovimentacao->pegaTipoStatusMovimentacaoAnterior($mvbid,$rgpid);
			if(is_array($dadosMov) && count($dadosMov) >= 1){
				if($dadosMov['mvbstatus'] == 'I'){
					if($dadosMov['tmvid'] == 4){
						$oRgp->atualizarSituacao($rgpid, SBM_DEVOLUCAO);
					}
					else if($dadosMov['tmvid'] != 4){
						$oRgp->atualizarSituacao($rgpid, SBM_MOVIMENTACAO);
					}
				}	
				else if($dadosMov['mvbstatus'] == 'A'){
					if($dadosMov['tmvid'] == 4){
						$oRgp->atualizarSituacao($rgpid, SBM_NORMAL);
					}
					else if($dadosMov['tmvid'] != 4){
						$oRgp->atualizarSituacao($rgpid, SBM_USO);
					}
				}
				$this->ativarUltimaMovimentacaoRgp($dadosMov['mvbid'],$rgpid);
			}
			else{
				$oRgp->atualizarSituacao($rgpid, SBM_NORMAL);
			}
			
			
			
			
			
			
			
			
			
			
		
			
			/*if($oRgp->pegarSituacao($rgpid) == SBM_MOVIMENTACAO){
	
				//se existe movimentacao para o rgp (que não seja a movimentação corrente)
				if($tmvid = $this->atualizarRegistroAnterior($rgpid, $mvbid)){
					
					// se o tipo da ultima movimentacao é devolução
					if($tmvid == 4){
						
						//volta a situacao do rgp para o estado normal
						$oRgp->atualizarSituacao($rgpid, SBM_NORMAL);
						
					// se nao existe movimentacao anterior (tmvid vem vazio)	
					}else if(empty($tmvid)){
						
						//volta a situacao do rgp para o estado normal
						$oRgp->atualizarSituacao($rgpid, SBM_NORMAL);
						
					}else{
						
						//volta a situacao do rgp para o estado em uso
						$oRgp->atualizarSituacao($rgpid, SBM_USO);
						
					}					
					
				}else{
	
					//volta a situacao do rgp para o estado normal
					$oRgp->atualizarSituacao($rgpid, SBM_NORMAL);
					
				}
			
			}else if($oRgp->pegarSituacao($rgpid) == SBM_DEVOLUCAO){
			
				// volta a situacao do rgp para o estado em uso
				$oRgp->atualizarSituacao($rgpid, SBM_USO);
			
			}*/
			
		}
		
		$this->commit();
		return $return;
		
	}
	
	/**
	 * Atualiza o ultimo registro como status A
	 * Verifica se o rgp ja foi movimentado retornando o tipo da ultima movimentacao (diferente da corrente)
     * @name atualizarRegistroAnterior
     * @author Silas Matheus
     * @access public
	 * @param int $rgpid - Identificador do RGP
	 * @param int $mvbid - Identificador da Movimentacao
	 * @return int
	 */
	public function atualizarRegistroAnterior($rgpid, $mvbid){
	
		$tmvid = null;
		
		// verificando se tem movimentacao
		$sql = "SELECT
					mvb.tmvid, mvb.mvbid
				FROM
					sap.bensmovimentacaorgp bmvr
				INNER JOIN
					sap.movimentacaobens mvb
					ON mvb.mvbid=bmvr.mvbid
				WHERE
					bmvr.rgpid=$rgpid
				AND
					mvb.mvbid<>$mvbid
				AND 
					mvb.mvbstatus='A'
				ORDER BY 
					mvb.mvbdata DESC";
		
	 	$movimentacao = $this->pegaLinha($sql);
	 	
	 	if($movimentacao){
	 		
			// colocando cursor de "ultimo registro" no registro anterior encontrado
			$sql = "UPDATE 
						$this->stNomeTabela 
					SET 
						bmvrstatus='A' 
					WHERE 
						rgpid=$rgpid
					AND
						mvbid={$movimentacao['mvbid']}";
			
			$this->executar($sql);
			$this->commit();
			 
	 	}	 	
	 	
		return $movimentacao['tmvid'];
	
	}
	
	
	/**
	 * Salva os registros filhos da movimentação
	 * @name salvar
	 * @author Alysson Rafael
	 * @access public
	 * @param array $rgps - array de rgps
	 * @param int $mvbid - Identificador da Movimentacao
	 * @param int $tmvid - Tipo de Movimentacao
	 * @return void
	 */
	public function salvar($rgps,$mvbid, $tmvid = ""){
		
		$sqlTotal = '';
		foreach($rgps as $key => $value){
			
			//colcoca os registros anteriores do rgp corrente com status I
			$sqlUpdate = "UPDATE $this->stNomeTabela SET bmvrstatus='I' WHERE rgpid='".$value['rgpid']."' ";
			$this->executar($sqlUpdate);
			$this->commit();
			
			//atualiza a situação(estado) para em movimentação de acordo com o tipo da movimentacao
			$oRgp = new Rgp();
			if($tmvid == TMV_DEVOLUCAO)
				$oRgp->atualizarSituacao($value['rgpid'], SBM_DEVOLUCAO);
			else
				$oRgp->atualizarSituacao($value['rgpid'], SBM_MOVIMENTACAO);
			
			//insere na tabela
			$sql = "INSERT INTO $this->stNomeTabela(mvbid,rgpid,bmvrstatus) ";
       		$sql .= "VALUES('".$mvbid."','".$value['rgpid']."','A'); ";
       		$sqlTotal .= $sql;
       	}
       	
       	$sqlTotal = substr($sqlTotal, 0, strrpos($sqlTotal,';'));
       	
       	$this->pegaUm($sqlTotal);
       	$this->commit();
	}
	
	
	/**
	 * Retorna a quantidade de rgps de uma movimentação
	 * @name verificaExistenciaBens
	 * @author Alysson Rafael
	 * @access public
	 * @param int $mvbid - Identificador da Movimentacao
	 * @return int
	 */
	public function verificaExistenciaBens($mvbid){
		$sql = "SELECT COUNT(*) AS qtd FROM $this->stNomeTabela WHERE mvbid='".$mvbid."' ";
		return $this->pegaUm($sql);
	}
	
	/**
	 * Retorna se existem bens que não estejam com status "em devolucao"
	 * @name verificaSituacaoParaDevolucao
	 * @author Silas Matheus
	 * @access public
	 * @param int $mvbid - Identificador da Movimentacao
	 * @return int
	 */
	public function verificaSituacaoParaDevolucao($mvbid){
	
		$oMovimentacao = new BensMovimentacao();
		$dados = $oMovimentacao->pegarMovimentacao($mvbid);
		extract($dados);
		
		// se a movimentacao é inativa
		if($mvbstatus == 'I'){

			// permite concluir apenas os bens em processo de devolucao
			$sql = "SELECT
				count(*)
			FROM
				sap.vwrgps
			WHERE
				mvbid=$mvbid
			AND
				sbmid<>" . SBM_DEVOLUCAO;

		}else{
		
			// se a movimetacao continua sendo de devolucao
			if($tmvid == TMV_DEVOLUCAO){
			 
				// permite concluir apenas os bens em processo de devolucao ou normal
				$sql = "SELECT
					count(*)
				FROM
					sap.vwrgps
				WHERE
					mvbid=$mvbid
				AND
					sbmid<>" . SBM_DEVOLUCAO ." AND sbmid<>" . SBM_NORMAL;		
		
			}else{
			
				// permite concluir apenas os bens em processo de devolucao ou em uso
				$sql = "SELECT
					count(*)
				FROM
					sap.vwrgps
				WHERE
					mvbid=$mvbid
				AND
					sbmid<>" . SBM_DEVOLUCAO ." AND sbmid<>" . SBM_USO;	
				
			}
			
		}
		
		return ($this->pegaUm($sql) < 1);
	
	}
	
	/**
	 * Retorna se existem bens que não estejam com status "em movimentacao"
	 * @name verificaSituacaoParaDevolucao
	 * @author Silas Matheus
	 * @access public
	 * @param int $mvbid - Identificador da Movimentacao
	 * @return int
	 */
	public function verificaSituacaoDiferenteDevolucao($mvbid){

		$oMovimentacao = new BensMovimentacao();
		$dados = $oMovimentacao->pegarMovimentacao($mvbid);
		extract($dados);
		
		// se a movimentacao é inativa
		if($mvbstatus == 'I'){

			// permite concluir apenas os bens em processo de movimentaaco
			$sql = "SELECT
				count(*)
			FROM
				sap.vwrgps
			WHERE
				mvbid=$mvbid
			AND
				sbmid<>" . SBM_MOVIMENTACAO;
		
		}else{
		
			// se a movimetacao continua sendo de movmentacao
			if($tmvid == TMV_DEVOLUCAO){
			 
				// permite concluir apenas os bens em processo de devolucao ou normal
				$sql = "SELECT
					count(*)
				FROM
					sap.vwrgps
				WHERE
					mvbid=$mvbid
				AND
					sbmid<>" . SBM_NORMAL ." AND sbmid<>" . SBM_MOVIMENTACAO;		
		
			}else{
			
				// permite concluir apenas os bens em processo de devolucao ou em uso
				$sql = "SELECT
					count(*)
				FROM
					sap.vwrgps
				WHERE
					mvbid=$mvbid
				AND
					sbmid<>" . SBM_MOVIMENTACAO ." AND sbmid<>" . SBM_USO;	
				
			}
			
		}
		
		return ($this->pegaUm($sql) < 1);
	
	}
	
	/**
	 * Retorna se existeo bem para a movimentacao
	 * @name existe
	 * @author Silas Matheus
	 * @access public
	 * @param int $mvbid - Identificador da Movimentacao
	 * @param int $rgpid - Identificador do Rgp
	 * @return int
	 */
	public function existe($mvbid, $rgpid){
	
		$sql = "SELECT
					count(*)
				FROM
					$this->stNomeTabela
				WHERE
					mvbid=$mvbid
				AND
					rgpid=$rgpid";		
		
		return ($this->pegaUm($sql) < 1);
		
	}
	
	/**
	 * Atualiza situacao dos rgps de acordo com o tipo da movimentaaco
	 * @name atualizarRgps
	 * @author Silas Matheus
	 * @access public
	 * @param int $mvbid
	 * @param int $tmvid
	 */
	public function atualizarRgps($mvbid, $tmvid){
	
		// buscando todos os rgps da movimentacao
		$sql = "SELECT 
					rgpid 
				FROM 
					sap.bensmovimentacaorgp 
				WHERE 
					mvbid=$mvbid";
		$rgps = $this->carregar($sql);
	
		// atualizando o status
		$oRgp = new Rgp();
		foreach($rgps as $rgp){

			if($tmvid == TMV_DEVOLUCAO)
				$oRgp->atualizarSituacao($rgp['rgpid'], SBM_NORMAL);
			else
				$oRgp->atualizarSituacao($rgp['rgpid'], SBM_USO);
	
		}
		
		$this->commit();
	}
	
	
	
	public function ativarUltimaMovimentacaoRgp($mvbid,$rgpid){
		$sql = "UPDATE $this->stNomeTabela SET bmvrstatus='A' WHERE mvbid='{$mvbid}' AND rgpid='{$rgpid}' ";
		$this->executar($sql);
	}
	
	
	
	
}
<?php

/**
 * Modelo de Registro Geral de Patrimonio
 * @author Silas Matheus
 */
class Rgp extends Modelo{
	
	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sap.rgp";

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('rgpid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos	= array(
		'rgpid' => null,
		'esmid' => null,
		'bmtid' => null,
		'rgpnumserie' => null,
		'rgpnumnotasiafi' => null,
		'rgpdatatomb' => null,
		'rgpanterior' => null,
		'sbmid' => null,
		'rgpjust' => null,
		'rgpnum' => null
	);
	
	/**
	 * Monta grid de rgps de acordo com o bem
	 * @name filtrar
	 * @author Silas Matheus
	 * @access public
	 * @param int $benid - Código do Bem
	 * @param int $bmtid - Código do bem material
	 * @param string $pesquisadsc - Filtro da pesquisa
	 * @return void
	 */
	public function filtrar($benid,$bmtid,$pesquisadsc=''){
	
		// recuperando a quantidade de rgps por bem material que não foram tombados 
		$sql = "SELECT 
					bmt.bmtid,
					matdsc,
					bmtvlrunitario,			 
					(SELECT count(*) FROM sap.rgp WHERE bmtid=bmt.bmtid) subtracao,
					bmt.bmtitmat - (SELECT count(*) FROM sap.rgp WHERE bmtid=bmt.bmtid) resultado
				FROM 
					sap.bensmaterial bmt
				INNER JOIN
					sap.material mat
					ON mat.matid=bmt.matid
				WHERE 
					bmt.benid='{$benid}' AND bmt.bmtid='{$bmtid}' ";
		
		$materiais = $this->carregar($sql);
		
		$rgps = array();
		
		if(is_array($materiais)){
			foreach($materiais as $material){
	
				// seleciona apenas se existir algum registro
				if($material['subtracao'] > 0){
					
					// selecionando registros tombados pelo id do bem material
					$sql = "SELECT 
									'<center>
										<a style=\"cursor:pointer;\" onclick=\"redirecionaEditar(\'' || rgp.rgpid || '\')\">
											<img src=\"/imagens/editar_nome.gif\" border=\"0\" title=\"Alterar\" />
										</a>
									</center>'
								as comandos,
								matdsc,
								rgpnum||' ' AS rgpnum, 
								rgp.rgpanterior||' ' AS rgpanterior,
								eco.ecodsc,
								bmtvlrunitario,
								'<span style=\"display:none;\">' || rgpdatatomb || '</span>' || to_char(rgpdatatomb, 'DD/MM/YYYY') rgpdatatomb 
							FROM
								$this->stNomeTabela rgp 
							INNER JOIN
								sap.bensmaterial bmt
								ON bmt.bmtid=rgp.bmtid
							INNER JOIN
								sap.bens ben
								ON ben.benid=bmt.benid
							INNER JOIN
								sap.estadomotivos esm
								ON esm.esmid=rgp.esmid
							INNER JOIN
								sap.estadoconservacao eco
								ON eco.ecoid=esm.ecoid	
							INNER JOIN
								sap.motivoestadoconservacao motivo
								ON esm.mecid=motivo.mecid					
							INNER JOIN
								sap.material mat
								ON mat.matid=bmt.matid
							WHERE
								bmt.bmtid='{$material['bmtid']}' ";
								
					if(!empty($pesquisadsc)){
						$sql .= "AND (CAST(rgpnum AS text)='{$pesquisadsc}' OR CAST(rgpanterior AS text)='{$pesquisadsc}' ";
						$sql .= "OR rgpnumserie ILIKE '%{$pesquisadsc}%' OR eco.ecodsc ILIKE '%{$pesquisadsc}%' ";
						$sql .= "OR motivo.mecdsc ILIKE '%{$pesquisadsc}%') ";
					}
					
					$sql .= "ORDER BY rgpnum ";
								
					
					$tombados = $this->carregar($sql);
					
					// adicionando tombados para a lista
					if(is_array($tombados) && count($tombados) >= 1){
						foreach($tombados as $tombado){
							$rgps[] = array($tombado['comandos'], '<center>'.$tombado['matdsc'].'</center>','<center>'.$tombado['rgpnum'].'</center>', '<center>'.$tombado['rgpanterior'].'</center>', '<center>'.$tombado['ecodsc'].'</center>', $tombado['bmtvlrunitario'], '<center>'.$tombado['rgpdatatomb'].'</center>');
							$cont++;
						}
					}
					
				}
				
				
				
				// se existir registros que não foram tombados
			 	if($material['resultado'] > 0 && empty($pesquisadsc)){
				
			 		// adiciona não tombandos a lista
			 		for($i = 0;  $i < $material['resultado']; $i++){

			 			$img = '<center>
			 						<a style="cursor:pointer;" onclick="redirecionaAdicionar(\'' . $material['bmtid'] .'\')">
										<img src="/imagens/alterar.gif" border="0" title="Tombar" />
									</a>
			 					</center>';
			 		
				 		$rgps[] = array($img, '<center>'.$material['matdsc'].'</center>','', '', '', $material['bmtvlrunitario'], '');
					}
				
			 	}
				
				
				
			}
						
			 
			
		}
				
		$cabecalho = array("Comando","Material","RGP","RGP Anterior", "Est. de Conservação", "Valor", "Dt. de Tombamento");
		$this->monta_lista_array($rgps, $cabecalho, 20, 10, false, "center");
	
	}
	
	
	
	/**
	 * Conta os registros que a pesquisa retornará
	 * @name contarRegistrosPesquisaTombamento
	 * @author Alysson Rafael
	 * @access public
	 * @param int $benid - Código do Bem
	 * @param string $pesquisadsc - Filtro da pesquisa
	 * @return void
	 */
	public function contarRegistrosPesquisaTombamento($benid,$pesquisadsc=''){
	
		// recuperando a quantidade de rgps por bem material que não foram tombados 
		$sql = "SELECT 
					bmt.bmtid,
					matdsc,
					bmtvlrunitario,			 
					(SELECT count(*) FROM sap.rgp WHERE bmtid=bmt.bmtid) subtracao,
					bmt.bmtitmat - (SELECT count(*) FROM sap.rgp WHERE bmtid=bmt.bmtid) resultado
				FROM 
					sap.bensmaterial bmt
				INNER JOIN
					sap.material mat
					ON mat.matid=bmt.matid
				WHERE 
					bmt.benid=$benid";
		
		$materiais = $this->carregar($sql);
		
		$rgps = array();
		$cont = 0;
		
		if(is_array($materiais)){
			foreach($materiais as $material){
	
				// seleciona apenas se existir algum registro
				if($material['subtracao'] > 0){
					
					// selecionando registros tombados pelo id do bem material
					$sql = "SELECT 
									'<center>
										<a style=\"cursor:pointer;\" onclick=\"redirecionaEditar(\'' || rgp.rgpid || '\')\">
											<img src=\"/imagens/alterar_01.gif\" border=\"0\" title=\"Alterar\" />
										</a>
									</center>'
								as comandos,
								matdsc,
								rgpnum||' ' AS rgpnum, 
								rgp.rgpanterior||' ' AS rgpanterior,
								eco.ecodsc,
								bmtvlrunitario,
								'<span style=\"display:none;\">' || rgpdatatomb || '</span>' || to_char(rgpdatatomb, 'DD/MM/YYYY') rgpdatatomb 
							FROM
								$this->stNomeTabela rgp 
							INNER JOIN
								sap.bensmaterial bmt
								ON bmt.bmtid=rgp.bmtid
							INNER JOIN
								sap.bens ben
								ON ben.benid=bmt.benid
							INNER JOIN
								sap.estadomotivos esm
								ON esm.esmid=rgp.esmid
							INNER JOIN
								sap.estadoconservacao eco
								ON eco.ecoid=esm.ecoid	
							INNER JOIN
								sap.motivoestadoconservacao motivo
								ON esm.mecid=motivo.mecid					
							INNER JOIN
								sap.material mat
								ON mat.matid=bmt.matid
							WHERE
								bmt.bmtid='{$material['bmtid']}' ";
								
					if(!empty($pesquisadsc)){
						$sql .= "AND (CAST(rgpnum AS text)='{$pesquisadsc}' OR CAST(rgpanterior AS text)='{$pesquisadsc}' ";
						$sql .= "OR rgpnumserie ILIKE '%{$pesquisadsc}%' OR eco.ecodsc ILIKE '%{$pesquisadsc}%' ";
						$sql .= "OR motivo.mecdsc ILIKE '%{$pesquisadsc}%') ";
					}
					
					$sql .= "ORDER BY rgpnum ";
								
					$tombados = $this->carregar($sql);
					
					// adicionando tombados para a lista
					if(is_array($tombados) && count($tombados) >= 1){
						foreach($tombados as $tombado){
							$rgps[] = array($tombado['comandos'], $tombado['matdsc'],$tombado['rgpnum'], $tombado['rgpanterior'], $tombado['ecodsc'], $tombado['bmtvlrunitario'], $tombado['rgpdatatomb']);
							$cont++;
						}
					}
					
				}
				
				
				
				// se existir registros que não foram tombados
			 	if($material['resultado'] > 0){
				
			 		// adiciona não tombandos a lista
			 		for($i = 0;  $i < $material['resultado']; $i++){

			 			$img = '<center>
			 						<a style="cursor:pointer;" onclick="redirecionaAdicionar(\'' . $material['bmtid'] .'\')">
										<img src="/imagens/alterar.gif" border="0" title="Tombar" />
									</a>
			 					</center>';
			 		
				 		$rgps[] = array($img, $material['matdsc'],'', '', '', $material['bmtvlrunitario'], '');
					}
				
			 	}
				
				
				
			}
						
			 
			
		}
				
		return $cont;
	
	}
	
	
	
	
	/**
	 * Pega a quantidade de materiais tombados pelo bem material
	 * @name pegarQuantidadeRgp
	 * @author Silas Matheus
	 * @access public
	 * @param int $bmtid - Identificador do Bem Material
	 * @return int
	 */
	public function pegarQuantidadeRgp($bmtid){
	
		$sql = "SELECT 
					count(*) 
				FROM 
					sap.rgp 
				WHERE 
					bmtid=" . $bmtid;
		
		return $this->pegaUm($sql);
	
	}
	
	/**
	 * Recupera registro pelo id
	 * @name pegarRegistro
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador 
	 * @return array
	 */
	public function pegarRegistro($id){
	
		$sql = "SELECT 
					*
				FROM 
					$this->stNomeTabela rgp
				INNER JOIN
					sap.estadomotivos esm
					ON esm.esmid=rgp.esmid
				INNER JOIN
					sap.bensmaterial bmt
					ON bmt.bmtid=rgp.bmtid
				INNER JOIN
					sap.bens ben
					ON ben.benid=bmt.benid
				LEFT JOIN
					sap.empenho emp
					ON emp.empid=ben.empid
				INNER JOIN
					sap.material mat
					ON mat.matid=bmt.matid
				INNER JOIN
					sap.catalogo cat
					ON cat.catid=mat.catid
				INNER JOIN
					sap.itemcontacontabil icb
					ON icb.icbid=cat.icbid AND cat.ccbid=icb.ccbid
				INNER JOIN
					sap.contacontabil ccb
					ON ccb.ccbid=icb.ccbid
				WHERE 
					rgp.rgpid=$id";					
				
		$return = $this->carregar($sql);		
		return $return[0];
		
	}

	/**
	 * Salva novo registro
	 * @name salvar
	 * @author Silas Matheus
	 * @access public
	 * @param array $dados - Campos para inserir
	 * @return int
	 */
	public function salvar($dados){
		
		extract($dados);
		
		//$rgpdatatomb = formata_data_sql($rgpdatatomb);
		$rgpdatatomb = date('Y-m-d');
		$rgpanterior = empty($rgpanterior) ? "null" : $rgpanterior;
		
		// pegando id do estado motivo
		$oEstadoMotivos = new EstadoMotivos();
		$esmid = $oEstadoMotivos->pegarId($ecoid, $mecid);
		
		$sql = "INSERT INTO 
					sap.rgp
					(esmid, bmtid, rgpnum, rgpnumserie, 
					rgpnumnotasiafi, rgpdatatomb, sbmid, rgpanterior)
			    VALUES 
			    	($esmid, $bmtid, '$rgpnum', '$rgpnumserie', '$rgpnumnotasiafi', 
					'$rgpdatatomb', '1', $rgpanterior) 
				RETURNING rgpid";
		
	 	$rgpid = $this->pegaUm($sql);
	 	$this->commit();
		return $rgpid;
		
	}
	
	/**
	 * Atualiza registro
	 * @name atualizar
	 * @author Silas Matheus
	 * @access public
	 * @param array $dados - Campos que serao atualizados
	 * @return int
	 */
	public function atualizar($dados){
		
		extract($dados);
		
		$rgpdatatomb = formata_data_sql($rgpdatatomb);
		$rgpanterior = empty($rgpanterior) ? "null" : $rgpanterior;
		
		// pegando id do estado motivo
		$oEstadoMotivos = new EstadoMotivos();
		$esmid = $oEstadoMotivos->pegarId($ecoid, $mecid);
		
		$sql = "UPDATE 
					sap.rgp
				SET 
					esmid=$esmid,
					rgpnum='$rgpnum',
					rgpnumserie='$rgpnumserie',
					rgpnumnotasiafi='$rgpnumnotasiafi',
					rgpanterior=$rgpanterior
				WHERE 
					rgpid={$rgpid} 
				RETURNING rgpid";

		$rgpid = $this->pegaUm($sql);
		
		$this->commit();
		
		//grava um registro na tabela de histórico
       	if($rgpid){
       		$oBensHistorico = new BensHistorico(); 
			$oBensHistorico->gravar($rgpjust, 'NULL',$benid);
       	}
		
		
		return $rgpid;
	
	}
	
	/**
	 * Exclui registro
	 * @name excluir
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador do Rgp
	 * @return bool
	 */
	public function excluir($id){
	
		$sql = "DELETE FROM $this->stNomeTabela WHERE rgpid=$id";
		$return = $this->executar($sql);
		$this->commit();
		return $return;
		
	}
	
	/**
	 * Verifica se registro já existe
	 * @name existe
	 * @author Silas Matheus
	 * @access public
	 * @param array $campos - Campos para verificar
	 * @return bool
	 */	
	public function existe($campos){
		
		// verifica se o rgp ja esta cadastrado no sistema
		$sql = "SELECT 
					count(*) count 
				FROM 
					$this->stNomeTabela rgp
				WHERE 
					rgpnum='{$campos['rgpnum']}'";
	
		if(!empty($campos['rgpid']))
			$sql .= " AND rgpid<>{$campos['rgpid']}";		
		
		$result = $this->carregar($sql);
 	   	$qtd = $result[0]['count'];
		
 	   	if($qtd > 0)
 	   		$resultado = true;

		return $resultado;
		
	}
	
	/**
	 * Valida se existe algum material que não tenha todos os rgps tombados
	 * @name validaTotalTombamentos
	 * @author Silas Matheus
	 * @access public
	 * @param int $benid - Identificador do Bem
	 * @return string
	 */	
	public function validaTotalTombamentos($benid){
		
		// total de tombamentos
		$sql = "SELECT
					matdsc
				FROM
					sap.bensmaterial bmt 
				INNER JOIN
					sap.material mat
					ON mat.matid=bmt.matid AND bmt.benid=$benid
				WHERE
					bmt.bmtitmat > (SELECT count(*) FROM sap.rgp rgp WHERE bmt.bmtid=rgp.bmtid)
				LIMIT 1";
		
		$matdsc = $this->pegaUm($sql);
		
		return $matdsc;
		
	}

	/**
	 * Recupera a quantidade de rgps para serem tombados
	 * @name quantidadeTombar
	 * @author Silas Matheus
	 * @access public
	 * @param int matid - Identificador do Material
	 * @param int $benid - Identificador do Bem
	 * @return string
	 */	
	public function quantidadeTombar($benid){
	
		$oBensMaterial = new BensMaterial();
		$totTombar = $oBensMaterial->pegaQtdMaterial($benid);
		
		$sql = "SELECT COUNT(*) AS qtd FROM $this->stNomeTabela rgp ";
		$sql .= "JOIN sap.bensmaterial bmt ON rgp.bmtid=bmt.bmtid WHERE bmt.benid='{$benid}' ";
				
		$totTombado = $this->pegaUm($sql);
		
		$totReal = $totTombar - $totTombado;
		
		
		
		
		
		/*$sql = "SELECT
					(bmt.bmtitmat - (select count(*) from sap.rgp rgp where bmt.bmtid=rgp.bmtid)) qtd
				FROM 
					sap.bensmaterial bmt
				WHERE
					benid=$benid";
		
		if(!empty($matid)){	
		
			$sql .= " AND bmt.matid=$matid";
	
		}
		
		$qtd = $this->pegaUm($sql);*/
		
		return $totReal;
	
	}
	
	/**
	 * Recupera o responsavel atual do rgp
	 * @name pegarResponsavel
	 * @author Silas Matheus
	 * @access public
	 * @param int $rgpid - Identificador do Rgp
	 * @return array
	 */	
	public function pegarResponsavel($rgpid){
			
		// recupera o ultimo responsavel de acordo com o historico
		$sql = "SELECT 
					nu_matricula_siape
				FROM 
					sap.bensmovimentacaorgp bmr	
				INNER JOIN
					sap.movimentacaobens mvb
					ON bmr.mvbid=mvb.mvbid
				WHERE 
					rgpid=$rgpid
				AND
					(mvb.mvbtipo='G' or mvb.mvbtipo='S')
				ORDER BY 
					mvb.mvbdata DESC
				LIMIT 1";
		
		$nu_matricula_siape = $this->pegaUm($sql);		
		
		// se nao encontrar, pega o responsavel inicial do bem
		if(!$nu_matricula_siape){
		
			$sql = "SELECT 
						nu_matricula_siape
					FROM 
						sap.rgp
					INNER JOIN
						sap.bensmaterial bmt
						ON bmt.bmtid=rgp.bmtid
					INNER JOIN
						sap.bens ben
						ON ben.benid=bmt.benid 
					WHERE 
						rgpid=$rgpid";
			
			$nu_matricula_siape = $this->pegaUm($sql);
		
		}
		
		// recupera os dados do responsavel
		$sql = "SELECT 
					*
				FROM 
					siape.vwservidorativo
				WHERE 
					nu_matricula_siape='$nu_matricula_siape'";
		
		$arDados = $this->pegaLinha($sql);
		$arDados = $arDados ? $arDados : array();
		
		return $arDados;
	
	}
	
	/**
	 * Recupera o endereco de unidade atual do rgp
	 * @name pegarEnderecoUnidade
	 * @author Silas Matheus
	 * @access public
	 * @param int $rgpid - Identificador do Rgp
	 * @return array
	 */	
	public function pegarEnderecoUnidade($rgpid){
			
		// recupera o ultima unidade de acordo com o historico
		$sql = "SELECT 
					uendid
				FROM 
					sap.bensmovimentacaorgp bmr	
				INNER JOIN
					sap.movimentacaobens mvb
					ON bmr.mvbid=mvb.mvbid
				WHERE 
					rgpid=$rgpid
				AND
					(mvb.mvbtipo='G' or mvb.mvbtipo='L')
				ORDER BY 
					mvb.mvbdata DESC
				LIMIT 1";
		
		$uendid = $this->pegaUm($sql);
		
		// se nao encontrar, pega a unidade inicial do bem
		if(!$uendid){
		
			$sql = "SELECT 
						uendid
					FROM 
						sap.rgp
					INNER JOIN
						sap.bensmaterial bmt
						ON bmt.bmtid=rgp.bmtid
					INNER JOIN
						sap.bens ben
						ON ben.benid=bmt.benid 
					WHERE 
						rgpid=$rgpid";
			
			$uendid = $this->pegaUm($sql);
	
		}
		
		// recupera os dados da unidade
		$sql = "SELECT
					unidade.uorno,
					unidade.uorco_uorg_lotacao_servidor,
					endandarsala.easdescricao,
					endandar.enadescricao,
					endereco.endcep,
					endereco.enduf,
					endereco.endcid,
					endereco.endlog,
					endereco.endnum,
					endereco.endcom
				FROM 
					siorg.uorgendereco uorgend
				JOIN 
					siorg.unidadeorganizacional unidade 
					ON uorgend.uorco_uorg_lotacao_servidor=unidade.uorco_uorg_lotacao_servidor
				JOIN 
					siorg.enderecoandarsala endandarsala 
					ON uorgend.easid=endandarsala.easid
				JOIN 
					siorg.enderecoandar endandar 
					ON endandarsala.enaid=endandar.enaid
				JOIN 
					siorg.endereco endereco 
					ON endandar.endid=endereco.endid
				WHERE 
					uorgend.uendid=$uendid";
		
		$arDados = $this->pegaLinha($sql);
		
		$arDados = $arDados ? $arDados : array();
		return $arDados;
	
	}
	
	/**
     * Carrega dados do RGP
     * @name carregaDadosRGP
     * @author Silas Matheus
     * @access public
     * @param int $rgpnum - Número do RGP
     * @return array
     */
	public function carregaDadosRGP($rgpnum){
		
		$arDados = array();
		
		// recupera os dados do rgp pelo numero
		$sql = "SELECT 
					rgp.rgpid,
					rgp.rgpnum,
					mat.matdsc,
					esm.ecoid,
					esm.mecid,
					rgp.rgpnumserie,
					sbm.sbmdsc
				FROM
					sap.rgp rgp
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
					sap.situacaobem sbm
					ON sbm.sbmid=rgp.sbmid
				WHERE
					rgpnum='$rgpnum'";
		
		$rgp = $this->pegaLinha($sql);
		
		if($rgp){
		
			// busca o endereco
			$enderecoUnidade = $this->pegarEnderecoUnidade($rgp['rgpid']);
			// busca o responsavel
			$responsavel = $this->pegarResponsavel($rgp['rgpid']);
			//monta array com todos os dados
			$arDados = array_merge($rgp, $enderecoUnidade);
			$arDados = array_merge($arDados, $responsavel);
		}
		
		return $arDados;
	}
		
	/**
     * Pega situação atual do rgp informado
     * @name pegarSituacao
     * @author Silas Matheus
     * @access public
     * @param int $rgpid - Id do RGP
     * @return array
     */
	public function pegarSituacao($rgpid){		
		
		$sql = "SELECT
					sbmid
				FROM
					$this->stNomeTabela 
				WHERE 
					rgpid=$rgpid";
		
	 	$sbmid = $this->pegaUm($sql);
		return $sbmid;
		
	}
	
	/**
     * Seta a situacao do rgp 
     * @name atualizarSituacao
     * @author Silas Matheus
     * @access public
     * @param int $rgpid - Id do RGP
     * @param int $sbmid - Código da Situação
     * @return array
     */
	public function atualizarSituacao($rgpid, $sbmid){		
		
		$sql = "UPDATE 
					$this->stNomeTabela 
				SET 
					sbmid=$sbmid 
				WHERE 
					rgpid=$rgpid";
		
		$this->executar($sql);
		$this->commit(); 
		
	}
	
	/**
     * Seta o estado e motivo do rgp
     * @name atualizarEstadoMotivo
     * @author Silas Matheus
     * @access public
	 * @param int $rgpid - Id do RGP
     * @param int $ecoid - Id do Estado de Conservação
     * @param int $mecid - Id do Motivo do Estado de Conservação
     * @return array
     */
	public function atualizarEstadoMotivo($rgpid, $ecoid, $mecid){		
		
		// pegando id do estado motivo
		$oEstadoMotivos = new EstadoMotivos();
		$esmid = $oEstadoMotivos->pegarId($ecoid, $mecid);
		
		$sql = "UPDATE 
					$this->stNomeTabela 
				SET 
					esmid='$esmid' 
				WHERE 
					rgpid=$rgpid";
		
		$this->executar($sql);
		$this->commit(); 
		
	}

	/**
	 * Busca todos os rgps ativos de um determiando responsavel
     * @name rgpsPorResponsavel
     * @author Silas Matheus
     * @access public
	 * @param int $nu_matricula_siape - Matricula do Responsavel
	 * @param int $sbmid - Situação do Bem
	 * @return array
	 */
	public function rgpsPorResponsavel($nu_matricula_siape, $sbmid){
	
		$sql = "SELECT 
					rgp.rgpid 
				FROM 

					(SELECT
						rgp.rgpid
					FROM 
						sap.rgp rgp
					INNER JOIN
						sap.bensmaterial bmt
						ON bmt.bmtid=rgp.bmtid
					INNER JOIN
						sap.bens ben
						ON ben.benid=bmt.benid
					WHERE 
						rgpid NOT IN (SELECT rgpid FROM sap.bensmovimentacaorgp)
					AND
						ben.benstatus='A'
					AND
						ben.nu_matricula_siape='$nu_matricula_siape'
				
				UNION
				
					SELECT 
						bmvr.rgpid
					FROM 
						sap.vwrgps vwrgps
					INNER JOIN 
						sap.bensmovimentacaorgp bmvr 
						ON bmvr.mvbid=vwrgps.mvbid and bmvr.rgpid=vwrgps.rgpid
					WHERE 
						bmvr.bmvrstatus='A' 
					AND 
						vwrgps.nu_matricula_siape_atual='$nu_matricula_siape') filtro
					
				INNER JOIN 
					sap.rgp rgp
					ON rgp.rgpid=filtro.rgpid
				WHERE
					rgp.sbmid IN(".SBM_NORMAL.",".SBM_USO.")";
	
		return $this->carregar($sql);	
		
	}
	
	/**
	 * Busca todos os rgps ativos de uma determinada unidade
     * @name rgpsPorEnderecoUnidade
     * @author Silas Matheus
     * @access public
	 * @param int $uendid - Identificador do Endereco de Unidade
	 * @param int $sbmid - Situação do Bem
	 * @return array
	 */
	public function rgpsPorEnderecoUnidade($uendid, $sbmid){
	
		$sql = "SELECT 
					rgp.rgpid 
				FROM 

					(SELECT
						rgp.rgpid
					FROM 
						sap.rgp rgp
					INNER JOIN
						sap.bensmaterial bmt
						ON bmt.bmtid=rgp.bmtid
					INNER JOIN
						sap.bens ben
						ON ben.benid=bmt.benid
					WHERE 
						rgpid NOT IN (SELECT rgpid FROM sap.bensmovimentacaorgp)
					AND
						ben.benstatus='A'
					AND
						ben.uendid='$uendid'
				
				UNION
				
					SELECT 
						bmvr.rgpid
					FROM 
						sap.vwrgps vwrgps
					INNER JOIN 
						sap.bensmovimentacaorgp bmvr 
						ON bmvr.mvbid=vwrgps.mvbid and bmvr.rgpid=vwrgps.rgpid
					WHERE 
						bmvr.bmvrstatus='A' 
					AND 
						vwrgps.uendid_atual='$uendid') filtro
											
				INNER JOIN 
					sap.rgp rgp
					ON rgp.rgpid=filtro.rgpid
				WHERE
					rgp.sbmid IN (2,4)";
	
		return $this->carregar($sql);
		
	}
	
	
	
	
	
	public function carregaRgpsDeUmaEntrada($benid){
		
		$sql = "SELECT rgp.rgpid FROM $this->stNomeTabela rgp ";
		$sql .= "WHERE CAST(rgp.rgpnum AS integer) IN ( ";
		
		$sql .= "SELECT rgp.rgpanterior FROM $this->stNomeTabela rgp ";
		$sql .= "JOIN sap.bensmaterial mat ON rgp.bmtid=mat.bmtid ";
		$sql .= "JOIN sap.bens bem ON mat.benid=bem.benid ";
		$sql .= "WHERE bem.benid='{$benid}' ";
		
		$sql .= ")";
		
		$result = $this->carregar($sql);
		return $result;
	}
	
	
	public function carregaRgpsDeUmaMovimentacao($mvbid){
		$sql = "SELECT rgp.rgpid FROM $this->stNomeTabela rgp ";
		$sql .= "JOIN sap.bensmovimentacaorgp bmr ON rgp.rgpid=bmr.rgpid ";
		$sql .= "JOIN sap.movimentacaobens mov ON bmr.mvbid=mov.mvbid ";
		$sql .= "WHERE mov.mvbid='{$mvbid}' ";
		
		$result = $this->carregar($sql);
		return $result;
	}
	
	
	public function pegaEstadoConservacao($rgpid){
		$sql = "SELECT estmot.ecoid FROM $this->stNomeTabela rgp ";
		$sql .= "JOIN sap.estadomotivos estmot ON rgp.esmid=estmot.esmid ";
		$sql .= "WHERE rgp.rgpid='{$rgpid}' ";
		
		$result = $this->pegaUm($sql);
		return $result;
	}
	
	
	
	
}
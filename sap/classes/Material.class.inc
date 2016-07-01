<?php

/**
 * Modelo de Material
 * @author Silas Matheus
 */
class Material extends Modelo{
	
	/**
	* Nome da tabela especificada
	* @name $stNomeTabela
	* @var string
	* @access protected
	*/
	protected $stNomeTabela = "sap.material";
	
	/**
	* Chave primária
	* @name $arChavePrimaria
	* @var array
	* @access protected
	*/
	protected $arChavePrimaria = array('matid');
	
	/**
	* Atributos da Tabela
	* @name $arAtributos
	* @var array
	* @access protected
	*/
	protected $arAtributos = array(
		'matid' => null,
		'catid' => null,
		'matdsc' => null,
		'matcodsiasg' => null,
		'matobs' => null,
		'matcodigo' => null
	);
	
	/**
	 * Monta grid de materiais de acordo com o filtro
	 * @name filtrar
	 * @author Silas Matheus
	 * @access public
	 * @param int $matid - Código do Material
	 * @param string $matdsc - Descrição do Material
	 * @param string $matcodsiasg - Código SIASG
	 * @return void
	 */
	public function filtrar($matid, $matdsc, $matcodsiasg){
	
		$sql = "SELECT 
						'<center><a style=\"cursor:pointer;\" onclick=\"redirecionaEditar(\'' || material.matid || '\')\">
							<img src=\"/imagens/editar_nome.gif\" border=\"0\" title=\"Alterar\" />
						</a>'
					|| 
						'<a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN (SELECT COUNT(*) FROM sap.bensmaterial bensmaterial WHERE bensmaterial.matid=material.matid) = 0 THEN 'redirecionaExcluir(\'' || material.matid || '\');' ELSE 'alert(\'Registro não pode ser excluído!\');' END || '\">
							<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
						</a></center>'					
					as comandos,
					material.matid||' ' AS matid, 
					material.matdsc,
					material.matcodsiasg
				FROM 
					$this->stNomeTabela material
				WHERE 1=1 ";
		
		if($matid){
			$matid = addslashes($matid);
			$sql .= " AND material.matid=$matid";
		}
			
		if($matdsc){	
			$matdsc = addslashes($matdsc);
			$sql .= " AND material.matdsc ILIKE '%$matdsc%'";
		}

		if($matcodsiasg){	
			$matcodsiasg = addslashes($matcodsiasg);
			$sql .= " AND material.matcodsiasg ILIKE '%$matcodsiasg%'";
		}
			
		$sql .= " ORDER BY material.matdsc ASC";
				
		$cabecalho = array("Comando","Código","Descrição", "Código SIASG");
		$this->monta_lista($sql, $cabecalho, 20, 50, false, "center",'S','','',array('center','center','center','center'));
	
	}
	
	/**
	 * Monta grid de materiais por classe 
	 * @name filtrarPorClasse
	 * @author Silas Matheus
	 * @access public
	 * @param int $clscodclasse - Código da Classe
	 * @return void
	 */
	public function filtrarPorClasse($clscodclasse){
	
		$sql = "SELECT 
						'<center><a style=\"cursor:pointer;\" onclick=\"redirecionaEditar(\'' || material.matid || '\')\">
							<img src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar\" />
						</a>'
					|| 
						'<a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN (SELECT COUNT(*) FROM sap.bensmaterial bensmaterial WHERE bensmaterial.matid=material.matid) = 0 THEN 'redirecionaExcluir(\'' || material.matid || '\');' ELSE 'alert(\'Registro não pode ser excluído!\');' END || '\">
							<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
						</a></center>'					
					as comandos,
					classe.clsdescclasse,
					material.matdsc as descricao
				FROM 
					$this->stNomeTabela material
				JOIN
					 sap.catalogo catalogo ON catalogo.catid=material.catid
				JOIN
				     sap.classe classe ON catalogo.clscodclasse=classe.clscodclasse
				WHERE 1=1 ";
		
		if($clscodclasse){
			$clscodclasse = addslashes($clscodclasse);
			$sql .= "AND classe.clscodclasse=$clscodclasse";
		}
			
		$sql .= " ORDER BY descricao ";
				
		$cabecalho = array("Comando","Classe","Descrição");
		$this->monta_lista($sql, $cabecalho, 20, 50, false, "center",'S','','',array('center','center','center'));
	
	}
	
	/**
	 * Monta grid de materiais para selecao (popup)
	 * @name filtrarMaterial
	 * @author Silas Matheus
	 * @access public
	 * @param int $matid - Código do Material
	 * @param string $matdsc - Descrição do Material
	 * @param string $matcodsiasg - Código SIASG
	 * @return void
	 */
	public function filtrarMaterial($matid, $matdsc, $matcodsiasg){

		$sql = "SELECT 
					'<center>
						<a style=\"cursor:pointer;\" onclick=\"selecionar(\''||mat.matid||'\');\">
                    	<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
                    	</a>
                    	<a style=\"cursor:pointer;\" onclick=\"redirecionaEditar(\''||mat.matid||'\');\">
                    	<img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
                    	</a>
                    	<a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN(SELECT COUNT(*) FROM sap.bensmaterial bensmaterial WHERE bensmaterial.matid=mat.matid) = 0 THEN ' redirecionaExcluir(\''||mat.matid||'\'); ' ELSE 'alert(\'Registro não pode ser excluído!\');' END || '\">
                    	<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                    	</a>	
					</center>'				
					as comandos,
					mat.matid||' ' AS matid, 
					mat.matdsc,
					mat.matcodsiasg,
					mat.matcodigo||' ' AS matcodigo,
					ccb.ccbdsc,
					icb.icbdsc,
					classe.clsdescclasse					
				FROM 
					$this->stNomeTabela mat
				INNER JOIN
					sap.catalogo cat
					ON cat.catid = mat.catid
				JOIN
				    sap.classe
				    ON cat.clscodclasse=classe.clscodclasse
				INNER JOIN
					sap.itemcontacontabil icb
					ON icb.icbid = cat.icbid AND icb.ccbid=cat.ccbid
				INNER JOIN
					sap.contacontabil ccb
					ON ccb.ccbid = icb.ccbid				  
				WHERE 1=1 ";
		
		if($matid){
			$matid = addslashes($matid);
			$sql .= " AND mat.matid=$matid";
		}
			
		if($matdsc){	
			$matdsc = addslashes($matdsc);
			$sql .= " AND mat.matdsc ilike '%$matdsc%'";
		}

		if($matcodsiasg){	
			$matcodsiasg = addslashes($matcodsiasg);
			$sql .= " AND mat.matcodsiasg ilike '%$matcodsiasg%'";
		}
			
		$sql .= " ORDER BY mat.matdsc ASC";
				
		$cabecalho = array("Comando","Código","Descrição", "Código SIASG", "Código do Material", "Conta Contábil", "Item Conta Contábil", "Classe");
		$this->monta_lista($sql, $cabecalho, 20, 50, false, "center",'S','','',array('center','center','center','center','center','center','center','center'));
		
	}
	
	/**
	 * Salva novo registro
	 * @name salvar
	 * @author Silas Matheus
	 * @access public
	 * @param array $campos - Campos para inserir
	 * @return int
	 */
	public function salvar($campos){
	
		$campos['clscodclasse'] = addslashes($campos['clscodclasse']);
		$campos['icbid'] = addslashes($campos['icbid']);
		$campos['ccbid'] = addslashes($campos['ccbid']);
		$campos['matdsc'] = addslashes($campos['matdsc']);
		$campos['matcodsiasg'] = addslashes($campos['matcodsiasg']);
		$campos['matobs'] = addslashes($campos['matobs']);
		
		$catid = $this->gerarCatId($campos['clscodclasse'], $campos['ccbid'], $campos['icbid']);
		$matcodigo = $this->gerarMatCodigo($campos['ccbid'], $campos['icbid']);
		
		$sql = "INSERT INTO 
					$this->stNomeTabela 
					(catid, matdsc, matcodsiasg, matobs, matcodigo) 
				VALUES 
					('{$catid}', '{$campos['matdsc']}', '{$campos['matcodsiasg']}', '{$campos['matobs']}', '{$matcodigo}') 
				RETURNING matid";
				
	 	$matid = $this->pegaUm($sql);
	 	$this->commit(); 
		return $matid;
	
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
		
		$campos['clscodclasse'] = addslashes($campos['clscodclasse']);
		$campos['icbid'] = addslashes($campos['icbid']);
		$campos['ccbid'] = addslashes($campos['ccbid']);
		$campos['matdsc'] = addslashes($campos['matdsc']);
		$campos['matcodsiasg'] = addslashes($campos['matcodsiasg']);
		
		$catid = $this->gerarCatId($campos['clscodclasse'], $campos['ccbid'], $campos['icbid']);
		
		$sql = "SELECT 
					count(*) count 
				FROM 
					$this->stNomeTabela 
				WHERE 
					(
						(matdsc='{$campos['matdsc']}' AND catid='{$catid}') 
					OR
						matcodsiasg='{$campos['matcodsiasg']}'
					)";
		
		if(!empty($campos['matid'])){
			$campos['matid'] = addslashes($campos['matid']);
			$sql .= "AND matid<>{$campos['matid']}";
		}		
		
		$result = $this->carregar($sql);
 	   	$qtd = $result[0]['count'];
		
 	   	if($qtd > 0)
 	   		$resultado = true;

		return $resultado;
		
	}
	
	/**
	 * Recupera registro para edicao
	 * @name pegarRegistro
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador do material
	 * @return array
	 */
	public function pegarRegistro($id){
	
		$id = addslashes($id);
		
		$sql = "SELECT 
					* 
				FROM 
					$this->stNomeTabela mt
				JOIN
					sap.catalogo cat
					ON cat.catid = mt.catid
				JOIN 
				    sap.classe 
				    ON cat.clscodclasse=classe.clscodclasse
				JOIN
					sap.itemcontacontabil icb
					ON icb.icbid = cat.icbid AND icb.ccbid=cat.ccbid
				JOIN
					sap.contacontabil ccb
					ON ccb.ccbid = icb.ccbid
				WHERE 
					matid=$id";
					
		$return = $this->carregar($sql);
		
		return $return[0];
		
	}
	
	/**
	 * Atualiza registro
	 * @name atualizar
	 * @author Silas Matheus
	 * @access public
	 * @param array $campos - Campos que serão atualizados
	 * @return int
	 */
	public function atualizar($campos){
	
		$campos['clscodclasse'] = addslashes($campos['clscodclasse']);
		$campos['icbid'] = addslashes($campos['icbid']);
		$campos['ccbid'] = addslashes($campos['ccbid']);
		$campos['matdsc'] = addslashes($campos['matdsc']);
		$campos['matcodsiasg'] = addslashes($campos['matcodsiasg']);
		$campos['matobs'] = addslashes($campos['matobs']);
		$campos['matid'] = addslashes($campos['matid']);
		
		$catid = $this->gerarCatId($campos['clscodclasse'], $campos['ccbid'], $campos['icbid']);
		$matcodigo = $this->gerarMatCodigo($campos['ccbid'], $campos['icbid']);
		
		$sql = "UPDATE 
					$this->stNomeTabela 
				SET 
					catid ='{$catid}',
					matdsc='{$campos['matdsc']}',
					matcodsiasg ='{$campos['matcodsiasg']}',
					matobs ='{$campos['matobs']}',
					matcodigo ='{$matcodigo}'					
				WHERE 
					matid='{$campos['matid']}' RETURNING matid";
					
		$matid = $this->pegaUm($sql);
		$this->commit();
		return $matid;
	
	}
	
	/**
	 * Exclui registro
	 * @name excluir
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador do Material
	 * @return bool
	 */
	public function excluir($id){
	
		$id = addslashes($id);
		
		$sql = " DELETE FROM $this->stNomeTabela WHERE matid=$id";
		$return = $this->executar($sql);
		$this->commit();
		return $return;
		
	}
	
	/**
	 * Recupera o codigo do catalogo
	 * @name gerarCatId
	 * @author Silas Matheus
	 * @access public
	 * @param int $clscodclasse - Código da Classe
	 * @param int $ccbid - Código da Conta Contábil
	 * @param int $icbid - Código do item da Conta Contábil
	 * @return int
	 */
	public function gerarCatId($clscodclasse, $ccbid, $icbid){
	
		$clscodclasse = addslashes($clscodclasse);
		$icbid = addslashes($icbid);
		
		$sql = "SELECT 
					catalogo.catid 
				FROM 
					sap.catalogo catalogo
				WHERE 
					catalogo.clscodclasse='$clscodclasse' 
				AND
				    catalogo.ccbid='$ccbid'
				AND 
					catalogo.icbid='$icbid'";
		
		return $this->pegaUm($sql);
	
	}
	
	/**
	 * Recupera o codigo do catalogo
	 * @name gerarMatCodigo
	 * @author Silas Matheus
	 * @access public
	 * @param int $ccbid - Código da Conta Contábil
	 * @param int $icbid - Código do item da Conta Contábil
	 * @return int
	 */
	public function gerarMatCodigo($ccbid, $icbid){
		
		$ccbid = addslashes($ccbid);
		$icbid = addslashes($icbid);
		
		// garantindo tamanho do codigo
		while(strlen($ccbid) < 2)
			$ccbid = '0' . $ccbid;
		while(strlen($icbid) < 3)
			$icbid = '0' . $icbid;		
		
		$matcodigo = $ccbid . $icbid;
		
		$sql = "SELECT 
					matcodigo 
				FROM 
					$this->stNomeTabela
				WHERE 
					CAST(matcodigo as varchar(8)) LIKE '$matcodigo%'
				ORDER BY matcodigo DESC
				";
		
		$return = $this->pegaUm($sql);
		
		// se retornar  
		if($return){
			
			// soma o incremento
			$incremento = substr($return, strlen($matcodigo), 3) + 1;

			// garantindo tamanho do codigo
			while(strlen($incremento) < 3)
				$incremento = '0' . $incremento;
								
			$matcodigo = $matcodigo . $incremento;
			
		}else{
			
			// inicia a contagem			
			$matcodigo = $matcodigo . '000';
			
		} 
		
		return $matcodigo;	
	
	}
	
}


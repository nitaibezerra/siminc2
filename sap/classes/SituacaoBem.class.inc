<?php

/**
 * Modelo de Situacao do bem
 * @author Silas Matheus
 */
class SituacaoBem extends Modelo{
	
	/**
	* Nome da tabela especificada
	* @name $stNomeTabela
	* @var string
	* @access protected
	*/
	protected $stNomeTabela = "sap.situacaobem";
	
	/**
	* Chave primária
	* @name $arChavePrimaria
	* @var array
	* @access protected
	*/
	protected $arChavePrimaria = array('sbmid');
	
	/**
	* Atributos da Tabela
	* @name $arAtributos
	* @var array
	* @access protected
	*/
	protected $arAtributos = array(
		'sbmid' => null,
		'sbmdsc' => null,
		'sbmcod' => null
	);
	
	/**
	 * Monta grid de situações de acordo com o filtro
	 * @name filtrar
	 * @author Silas Matheus
	 * @access public
	 * @param int $sbmcod - Código da Situação do Bem
	 * @param string $sbmdsc - Descrição da Situação do Bem
	 * @return void
	 */
	public function filtrar($sbmcod, $sbmdsc){
	
		$sql = "SELECT 
						'<center><a style=\"cursor:pointer;\" onclick=\"redirecionaEditar(\'' || sbmid || '\')\">
							<img src=\"/imagens/editar_nome.gif\" border=\"0\" title=\"Alterar\" />
						</a>'
					as comandos,
					sbmcod, 
					sbmdsc 
				FROM 
					$this->stNomeTabela 
				WHERE 1=1 ";
		
		
		$sbmcod = strtoupper($sbmcod);
		if($sbmcod){
			$sbmcod = addslashes($sbmcod);
			$sql .= "AND sbmcod='$sbmcod'";
		}			
			 
		if($sbmdsc){	
			$sbmdsc = addslashes($sbmdsc);
			$sql .= "AND sbmdsc ilike '%$sbmdsc%'";
		}	
			
		$sql .= " ORDER BY sbmdsc ASC";
				
		$cabecalho = array("Comando","Código","Descrição");
		$this->monta_lista($sql, $cabecalho, 20, 50, false, "center",'S','','',array('center','center','center'));
	
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
	
		$campos['sbmdsc'] = addslashes($campos['sbmdsc']);
		
		$sql = " INSERT INTO $this->stNomeTabela (sbmdsc) VALUES ('{$campos['sbmdsc']}') RETURNING sbmid";
	 	$sbmid = $this->pegaUm($sql);
	 	$this->commit(); 
		return $sbmid;
	
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
		
		$campos['sbmdsc'] = addslashes($campos['sbmdsc']);
		
		$sql = " SELECT count(*) count FROM $this->stNomeTabela WHERE sbmdsc='{$campos['sbmdsc']}' ";
		
		if(!empty($campos['sbmid'])){
			$campos['sbmid'] = addslashes($campos['sbmid']);
			$sql .= "AND sbmid<>{$campos['sbmid']}";
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
	 * @param int $id - Identificador da situacao
	 * @return array
	 */
	public function pegarRegistro($id){
	
		$id = addslashes($id);
		
		$sql = " SELECT * FROM $this->stNomeTabela WHERE sbmid=$id";
		$return = $this->carregar($sql);
		
		return $return[0];
		
	}
	
	/**
	 * Atualiza registro
	 * @name atualizar
	 * @author Silas Matheus
	 * @access public
	 * @param array $campos - Campos que serao atualizados
	 * @return int
	 */
	public function atualizar($campos){
	
		$campos['sbmdsc'] = addslashes($campos['sbmdsc']);
		$campos['sbmid'] = addslashes($campos['sbmid']);
		
		$sql = " UPDATE $this->stNomeTabela SET sbmdsc='{$campos['sbmdsc']}' WHERE sbmid='{$campos['sbmid']}' RETURNING sbmid";
		$sbmid = $this->pegaUm($sql);
		$this->commit();
		return $sbmid;
	
	}
	
	/**
	 * Exclui registro
	 * @name excluir
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador da Situação
	 * @return bool
	 */
	public function excluir($id){
	
		$id = addslashes($id);
		
		$sql = " DELETE FROM $this->stNomeTabela WHERE sbmid=$id";
		$return = $this->executar($sql);
		$this->commit();
		return $return;
		
	}
	
	
	/**
     * Gerar a combo de situação do bem
     * @name montaComboSituacao
     * @author Alysson Rafael
     * @access public
     * @return void
     */
	public function montaComboSituacao(){
		
		$sql = "SELECT sbmid AS codigo,sbmdsc AS descricao FROM sap.situacaobem ORDER BY sbmdsc ";

		$resultado = $this->carregar($sql);

		$this->monta_combo("sbmid",$resultado,'S',"Todos...","","","","200","N","sbmid","",$sbmid);
	}
	
}


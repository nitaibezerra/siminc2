<?php

/**
 * Modelo de Tipo de Documento
 * @author Silas Matheus
 */
class TipoDocumento extends Modelo{
	
	/**
	* Nome da tabela especificada
	* @name $stNomeTabela
	* @var string
	* @access protected
	*/
	protected $stNomeTabela = "sap.tipodocumento";
	
	/**
	* Chave primária
	* @name $arChavePrimaria
	* @var array
	* @access protected
	*/
	protected $arChavePrimaria = array('tdcid');
	
	/**
	* Atributos da Tabela
	* @name $arAtributos
	* @var array
	* @access protected
	*/
	protected $arAtributos = array(
		'tdcid' => null,
		'tdcdsc' => null
	);
	
	/**
	 * Monta grid de tipos de documento de acordo com filtro
	 * @name filtrar
	 * @author Silas Matheus
	 * @access public
	 * @param int $tdcid - Código do Tipo de Documento
	 * @param string $tdcdsc - Descrição do Tipo de Documento
	 * @return void
	 */
	public function filtrar($tdcid, $tdcdsc){

		
		$sql = "SELECT 
						'<center><a style=\"cursor:pointer;\" onclick=\"redirecionaEditar(\'' || tipodocumento.tdcid || '\')\">
							<img src=\"/imagens/editar_nome.gif\" border=\"0\" title=\"Alterar\" />
						</a>'
					|| 
						'<a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN (SELECT COUNT(*) FROM sap.tipoentradasaida tipoentradasaida WHERE tipoentradasaida.tdcid=tipodocumento.tdcid ) = 0 THEN ' redirecionaExcluir(\'' || tipodocumento.tdcid || '\');' ELSE 'alert(\'Registro não pode ser excluído!\');' END || '\">
							<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
						</a></center>'					
					as comandos,
					tipodocumento.tdcid||' ' AS tdcid, 
					tipodocumento.tdcdsc 
				FROM 
					$this->stNomeTabela tipodocumento 
				WHERE 1=1 ";
		
		if($tdcid){
			$tdcid = addslashes($tdcid);	
			$sql .= "AND tipodocumento.tdcid=$tdcid";
		}	
		
		if($tdcdsc){	
			$tdcdsc = addslashes($tdcdsc);	
			$sql .= "AND tipodocumento.tdcdsc ilike '%$tdcdsc%'";
		}	
			
		$sql .= " ORDER BY tipodocumento.tdcdsc ASC";
				
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
	
		$campos['tdcdsc'] = addslashes($campos['tdcdsc']);
		
		$sql = " INSERT INTO $this->stNomeTabela (tdcdsc) VALUES ('{$campos['tdcdsc']}') RETURNING tdcid";
	 	$tdcid = $this->pegaUm($sql);
	 	$this->commit(); 
		return $tdcid;
	
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
		
		$campos['tdcdsc'] = addslashes($campos['tdcdsc']);
		
		$sql = " SELECT count(*) count FROM $this->stNomeTabela WHERE tdcdsc='{$campos['tdcdsc']}' ";
		
		if(!empty($campos['tdcid'])){
			$campos['tdcid'] = addslashes($campos['tdcid']);
			$sql .= "AND tdcid<>{$campos['tdcid']}";		
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
	 * @param int $id - Identificador do tipo de documento
	 * @return array
	 */
	public function pegarRegistro($id){
	
		$id = addslashes($id);
		
		$sql = " SELECT * FROM $this->stNomeTabela WHERE tdcid=$id";
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
	
		$campos['tdcdsc'] = addslashes($campos['tdcdsc']);
		$campos['tdcid'] = addslashes($campos['tdcid']);
		
		$sql = " UPDATE $this->stNomeTabela SET tdcdsc='{$campos['tdcdsc']}' WHERE tdcid='{$campos['tdcid']}' RETURNING tdcid";
		$tdcid = $this->pegaUm($sql);
		$this->commit();
		return $tdcid;
	
	}
	
	/**
	 * Exclui registro
	 * @name excluir
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador do tiop de documento
	 * @return bool
	 */
	public function excluir($id){
	
		$id = addslashes($id);
		
		$sql = " DELETE FROM $this->stNomeTabela WHERE tdcid=$id";
		$return = $this->executar($sql);
		$this->commit();
		return $return;
		
	}
	
}


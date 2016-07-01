<?php

/**
 * Modelo de Motivo do Estado de Conservação
 * @author Silas Matheus
 */
class MotivoEstadoConservacao extends Modelo{
	
	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sap.motivoestadoconservacao";

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('mecid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'mecid' => null,
		'mecdsc' => null
	);	
	
	/**
	 * Monta grid de tipos de documento de acordo com filtro
	 * @name filtrar
	 * @author Silas Matheus
	 * @access public
	 * @param int $mecid - Código do Motivo
	 * @param string $mecdsc - Descrição do Motivo
	 * @return void
	 */
	public function filtrar($mecid, $mecdsc){

		$sql = "SELECT 
						'<center><a style=\"cursor:pointer;\" onclick=\"redirecionaEditar(\'' || mec.mecid || '\')\">
							<img src=\"/imagens/editar_nome.gif\" border=\"0\" title=\"Alterar\" />
						</a>'
					|| 
						'<a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN (SELECT COUNT(*) FROM sap.estadomotivos esm WHERE esm.mecid=mec.mecid ) = 0 THEN ' redirecionaExcluir(\'' || mec.mecid || '\');' ELSE 'alert(\'Registro não pode ser excluído!\');' END || '\">
							<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
						</a></center>'					
					as comandos,
					mec.mecid||' ' AS mecid, 
					mec.mecdsc 
				FROM 
					$this->stNomeTabela mec 
				WHERE 1=1 ";
		
		if($mecid){
			$mecid = addslashes($mecid);
			$sql .= "AND mec.mecid=$mecid";
		}
			
		if($mecdsc){	
			$mecdsc = addslashes($mecdsc);
			$sql .= "AND mec.mecdsc ilike '%$mecdsc%'";
		}	
			
		$sql .= " ORDER BY mec.mecdsc ASC";
				
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
		
		$campos['mecdsc'] = addslashes($campos['mecdsc']);
		
		$sql = " INSERT INTO $this->stNomeTabela (mecdsc) VALUES ('{$campos['mecdsc']}') RETURNING mecid";
	 	$mecid = $this->pegaUm($sql);
	 	$this->commit(); 
		return $mecid;
	
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
		
		$campos['mecdsc'] = addslashes($campos['mecdsc']);
		
		$sql = " SELECT count(*) count FROM $this->stNomeTabela WHERE mecdsc='{$campos['mecdsc']}' ";
		
		if(!empty($campos['mecid'])){
			$campos['mecid'] = addslashes($campos['mecid']);
			$sql .= "AND mecid<>{$campos['mecid']}";
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
	 * @param int $id - Identificador 
	 * @return array
	 */
	public function pegarRegistro($id){
	
		$id = addslashes($id);
		
		$sql = " SELECT * FROM $this->stNomeTabela WHERE mecid=$id";
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
	
		$campos['mecdsc'] = addslashes($campos['mecdsc']);
		$campos['mecid'] = addslashes($campos['mecid']);
		
		$sql = " UPDATE $this->stNomeTabela SET mecdsc='{$campos['mecdsc']}' WHERE mecid='{$campos['mecid']}' RETURNING mecid";
		$mecid = $this->pegaUm($sql);
		$this->commit();
		return $mecid;
	
	}
	
	/**
	 * Exclui registro
	 * @name excluir
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador
	 * @return bool
	 */
	public function excluir($id){
	
		$id = addslashes($id);
		
		$sql = " DELETE FROM $this->stNomeTabela WHERE mecid=$id";
		$return = $this->executar($sql);
		$this->commit();
		return $return;
		
	}
	
	/**
	 * Retorna todos os motivos de estados de conservacao
	 * @name montaComboMotivoEstadoConservacao
	 * @author Silas Matheus
	 * @access public
	 * @param int $ecoid - Código do Estado de Conservação
	 * @param int $mecid - Qual option receberá o selected 
	 * @param string $obrigatorio - Indica se a imagem de obrigatoriedade aparecerá
	 * @return void
	 */
	public function montaComboMotivoEstadoConservacao($ecoid, $mecid = '',$obrigatorio='S'){
			
		$sql = "SELECT 
					mec.mecid AS codigo,
					mecdsc AS descricao 
				FROM 
					$this->stNomeTabela mec";
		
		if(!empty($ecoid)){
			
			$ecoid = addslashes($ecoid);
			
			$sql .= " 	INNER JOIN 
							sap.estadomotivos esm
							ON esm.mecid=mec.mecid
						WHERE 
							ecoid=$ecoid";
			
		}
		
		$sql .= " ORDER BY mecdsc";
		
		$resultado = $this->carregar($sql);
		$this->monta_combo("mecid", $resultado, 'S', "Todos...", "", "", "", "200", $obrigatorio, "mecid", "", $mecid);
	
	}
	
	/**
	 * Retorna todos os motivos de estados de conservacao em um multiselect
	 * @name montaComboMultMotivoEstadoConservacao
	 * @author Silas Matheus
	 * @access public
	 * @param int $motivos - Quais options receberão o selected 
	 * @return void
	 */
	public function montaComboMultMotivoEstadoConservacao($motivos = null){
		
		$sql = "SELECT mecid AS codigo,mecdsc AS descricao FROM $this->stNomeTabela ORDER BY mecdsc";
		$resultado = $this->carregar($sql);

		$this->monta_combo_multiplo("mecid", $resultado, 'S', '', "", "", "", "10", "500", $motivos);
		
	}
	
}
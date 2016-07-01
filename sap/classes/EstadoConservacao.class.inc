<?php

/**
 * Modelo de Estado de Conservação
 * @author Alysson Rafael
 */
class EstadoConservacao extends Modelo{
	
	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sap.estadoconservacao";

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('ecoid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'ecoid' => null,
		'ecodsc' => null
	);	
	
	/**
     * Listar estados de conservação através dos filtros informados
     * @name listarEstadoConservacaoPorFiltro
     * @author Alysson Rafael
     * @access public
     * @param int $ecoid - Código do estado de conservação
     * @param int $ecodsc - Descrição do estado de conservação
     * @return void
     */
	function listarEstadoConservacaoPorFiltro($ecoid,$ecodsc){
		
		$where = " WHERE 1=1 ";
		if(!empty($ecoid)){
			$ecoid = addslashes($ecoid);
			$where .= " AND estado.ecoid = '".$ecoid."' ";
		}
		if(!empty($ecodsc)){
			$ecodsc = addslashes($ecodsc);
			$where .= " AND estado.ecodsc ILIKE '%".$ecodsc."%' ";
		}
		
		$sql = "SELECT 
					'<center>
                    	<a style=\"cursor:pointer;\" onclick=\"detalharEstadoConservacao(\''||estado.ecoid||'\');\">
                    		<img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
                    	</a>
                    	<a style=\"cursor:pointer;\" onclick=\"' || 
                    		CASE WHEN 
                    			(SELECT COUNT(*) FROM sap.estadomotivos esm WHERE esm.ecoid=estado.ecoid) = 0 
                    		THEN 
                    			'if(confirm(\'Deseja excluir o registro?\')){removerEstadoConservacao(\''||estado.ecoid||'\');}' 
                    		ELSE 
                    			'alert(\'Registro não pode ser excluído!\');' 
                    		END || '\">
                    		<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                    	</a>
                    </center>' as acao,
                    estado.ecoid||' ' as ecoid,
                    estado.ecodsc as ecodsc
                    FROM 
                    	sap.estadoconservacao estado 
                    	".$where." 
                    ORDER BY 
                    	estado.ecodsc ";

		$rows = $this->carregar($sql);
		
		// adicionando os motivos a lista
		foreach($rows as &$row){
		
			$sql = "SELECT 
						mecdsc 
					FROM 
						sap.estadomotivos esm
					INNER JOIN
						sap.motivoestadoconservacao mec
						ON mec.mecid=esm.mecid
					WHERE
						ecoid=" . $row['ecoid'] . "
					ORDER BY
						mecdsc";
			
			$motivos = $this->carregarColuna($sql);
			$row['motivosAssociados'] = " - " . join($motivos, '<br /> - ');
		
		}
		
		$cabecalho = array("Comando","Código","Descrição", "Motivos associados");
		$this->monta_lista_array($rows,$cabecalho,100,50,false,"center");
		
	}
	
	/**
     * Verificar se já existe uma descrição igual à digitada
     * @name verificaEstadoRepetido
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return bool
     */
	function verificaEstadoRepetido($post){
		
		extract($post);
		
		$ecodsc = addslashes($ecodsc);
		
		//verifica se existe no banco um registro com a descrição informada no form
		$sql = "SELECT COUNT(*) AS tot FROM sap.estadoconservacao WHERE ecodsc='".$ecodsc."' ";
		if(!empty($ecoid)){
			$ecoid = addslashes($ecoid);
			$sql .= "AND ecoid <> '".$ecoid."' ";
		}
		
		$result = $this->carregar($sql);
		if($result[0]['tot'] >= 1){
			return true;
		}
		else{
			return false;
		}
		
	}
	
	/**
     * Cadastrar/editar estados de conservação
     * @name salvarEstadoConservacao
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return void
     */
	function salvarEstadoConservacao($post){
		extract($post);
		
		$ecodsc = addslashes($ecodsc);
		$operacao = '';
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($ecoid)){
			$operacao = 'adicionar';
			$sql = "INSERT INTO sap.estadoconservacao(ecodsc) VALUES('".$ecodsc."') ";
		}
		else{
			$ecoid = addslashes($ecoid);
			$operacao = 'editar';
			$sql = "UPDATE sap.estadoconservacao SET ecodsc='".$ecodsc."' WHERE ecoid='".$ecoid."' ";
		}
		
		$sql .= " returning ecoid ";
		$ecoid = $this->pegaUm($sql);
		
		// associando motivos
		$oEstadoMotivos = new EstadoMotivos();
		$estados = $mecid ? $mecid : array();
		$relacionamentoMotivoEstado = $oEstadoMotivos->relacionarMotivosEstados($ecoid, $estados);
		
		
       	if($relacionamentoMotivoEstado){

	       	if(!empty($ecoid)){
	       		$this->commit();
	       		return array(0=>true,1=>$ecoid,2=>$operacao);
	       	}
	       	else{
	       		return array(0=>false,1=>$ecoid,2=>$operacao);
	       	}
	       	
       	}else{
       		direcionar('?modulo=sistema/tabelasdeapoio/estadoConservacao/'.$operacao.'&acao=A&ecoid='.$ecoid,'Operação falhou! Verifique se os Motivos desmarcados para o Estado estão associados para um rgp!');
       	}
       	
	}
	
	/**
     * Carregar um estado de conservação pelo id
     * @name carregaEstadoConservacaoPorId
     * @author Alysson Rafael
     * @access public
     * @param int $ecoid - Id a ser pesquisado
     * @return array
     */
	function carregaEstadoConservacaoPorId($ecoid){
		
		$ecoid = addslashes($ecoid);
		
		$sql = "SELECT estado.ecoid,estado.ecodsc FROM sap.estadoconservacao estado WHERE estado.ecoid='".$ecoid."'";
        return $this->pegaLinha($sql);
		
	}
	
	/**
     * Excluir um estado de conservação
     * @name excluirEstadoConservacao
     * @author Alysson Rafael
     * @access public
     * @param int $ecoid - Id do registro a ser excluído
     * @return void
     */
	function excluirEstadoConservacao($ecoid){
		
		$ecoid = addslashes($ecoid);
		
		$sql = "DELETE FROM sap.estadoconservacao WHERE ecoid='".$ecoid."' ";
		$this->executar($sql);
		$this->commit();
        direcionar('?modulo=sistema/tabelasdeapoio/estadoConservacao/pesquisar&acao=A','Operação realizada com sucesso!');
        
	}
	
	/**
	 * Retorna todos os estados de conservacao
	 * @name montaComboEstadoConservacao
	 * @author Silas Matheus
	 * @access public
	 * @param int $ecoid - Qual option receberá o selected 
	 * @param string $obrigatorio - Indica se a imagem de obrigatoriedade aparecerá
	 * @return void
	 */
	public function montaComboEstadoConservacao($ecoid = '',$obrigatorio='S'){
			
		$sql = "SELECT ecoid AS codigo,ecodsc AS descricao FROM $this->stNomeTabela";
		
		$resultado = $this->carregar($sql);

		$this->monta_combo("ecoid", $resultado, 'S', "Todos...", "montaComboMotivoEstadoConservacao(this.value,'','".$obrigatorio."');", "", "", "200", $obrigatorio, "ecoid", "", $ecoid);
	
	}
	
}
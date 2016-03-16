<?php

/**
 * Modelo de Catálogo
 * @author Alysson Rafael
 */
class Catalogo extends Modelo{
	
	
	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sap.catalogo";

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('catid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'catid' => null,
		'icbid' => null,
		'ccbid' => null,
		'clscodclasse' => null
	);
	
	/**
     * Listar em grid os catálogos através dos filtros informados
     * @name listarCatalogoPorFiltro
     * @author Alysson Rafael
     * @access public
     * @param int $clscodclasse - Id da classe
     * @param int $ccbid - Id da conta contábil
     * @param int $icbid - Id do ítem da conta contábil
     * @return void
     */
	public function listarCatalogoPorFiltro($clscodclasse,$ccbid,$icbid){
		
		$where = " WHERE 1=1 ";
		if(!empty($clscodclasse)){
			$clscodclasse = addslashes($clscodclasse);
			$where .= " AND catalogo.clscodclasse = '".$clscodclasse."' ";
		}
		if(!empty($ccbid)){
			$ccbid = addslashes($ccbid);
			$where .= "AND conta.ccbid='".$ccbid."' ";
		}
		if(!empty($icbid)){
			$icbid = addslashes($icbid);
			$where .= " AND item.icbid = '".$icbid."' ";
		}

	
		$sql = "SELECT '<center>
                    <a style=\"cursor:pointer;\" onclick=\"detalharCatalogo(\''||catalogo.catid||'\');\">
                    <img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
                    </a>
                    <a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN (SELECT COUNT(*) FROM sap.material material WHERE material.catid=catalogo.catid) = 0 THEN 'if(confirm(\'Deseja excluir o registro?\')){removerCatalogo(\''||catalogo.catid||'\');}' ELSE 'alert(\'Registro não pode ser excluído!\');' END || '\">
                    <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                    </a>
                    </center>' as acao,
                    classe.clscodclasse||' ' as clscodclasse,
                    classe.clsdescclasse as clsdescclasse,
                    conta.ccbdsc as ccbdsc,
                    item.icbdsc as icbdsc
                    FROM sap.classe classe
                    JOIN sap.catalogo catalogo ON classe.clscodclasse=catalogo.clscodclasse 
                    JOIN sap.itemcontacontabil item ON catalogo.icbid=item.icbid AND catalogo.ccbid=item.ccbid 
                    JOIN sap.contacontabil conta ON item.ccbid=conta.ccbid ";
				
		$sql .= $where." ORDER BY classe.clsdescclasse ";
		
		$cabecalho = array("Comando","Código da Classe","Classe","Conta Contábil","Ítem da Conta Contábil");
		$this->monta_lista($sql,$cabecalho,20,50,false,"center",'S','','',array('center','center','center','center','center'));
		
	}
	
	
	
	/**
     * Verificar se já existe uma combinação catcodclasse+ccbid+icbid, pois não poderá repetir
     * @name verificaUniqueCatalogo
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return bool
     */
	public function verificaUniqueCatalogo($post){
		
		extract($post);
		
		$clscodclasse = addslashes($clscodclasse);
		$icbid = addslashes($icbid);
		$ccbid = addslashes($ccbid);
		
		//veririca se existe no banco um registro com a combinação
		//catcodclasse+ccbid+icbid, pois não poderá repetir
		$sql = " SELECT COUNT(*) AS tot FROM $this->stNomeTabela ";
		$sql .= "WHERE clscodclasse='".$clscodclasse."' AND ccbid='".$ccbid."' AND icbid='".$icbid."' ";
		if(!empty($catid)){
			$catid = addslashes($catid);
			$sql .= "AND catid <> '".$catid."' ";
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
     * Cadastrar/editar catálogos
     * @name salvarCatalogo
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return void
     */
	public function salvarCatalogo($post){
		extract($post);
		
		$operacao = '';
		$ccbid = addslashes($ccbid);
		$icbid = addslashes($icbid);
		$clscodclasse = addslashes($clscodclasse);
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($catid)){
			$operacao = 'adicionar';
			$sql = "INSERT INTO $this->stNomeTabela(ccbid,icbid,clscodclasse) ";
			$sql .= "VALUES('".$ccbid."','".$icbid."','".$clscodclasse."') ";
		}
		else{
			$catid = addslashes($catid);
			$operacao = 'editar';
			$sql = "UPDATE $this->stNomeTabela SET ccbid='".$ccbid."',icbid='".$icbid."',clscodclasse='".$clscodclasse."' ";
			$sql .= "WHERE catid='".$catid."' ";
		}
		
		$sql .= " RETURNING catid ";
       	$catid = $this->pegaUm($sql);

       	if(!empty($catid)){
       		$this->commit();
       		return array(0=>true,1=>$catid,2=>$operacao);
       	}
       	else{
       		$this->rollback();
       		return array(0=>false,1=>$catid,2=>$operacao);
       	}
		
	}
	
	/**
     * Carregar um catálogo pelo id
     * @name carregaCatalogoPorId
     * @author Alysson Rafael
     * @access public
     * @param int $catid - Id a ser pesquisado
     * @return array
     */
	public function carregaCatalogoPorId($catid){
		
		$catid = addslashes($catid);
		
		$sql = "SELECT classe.clscodclasse,conta.ccbid,item.icbid ";
		$sql .= "FROM sap.classe classe ";
		$sql .= "JOIN sap.catalogo catalogo ON classe.clscodclasse=catalogo.clscodclasse ";
		$sql .= "JOIN sap.itemcontacontabil item ON catalogo.icbid=item.icbid AND catalogo.ccbid=item.ccbid ";
		$sql .= "JOIN sap.contacontabil conta ON item.ccbid=conta.ccbid ";
		$sql .= "WHERE catalogo.catid='".$catid."' ";
        return $this->pegaLinha($sql);
		
	}
	
	/**
     * Excluir um catálogo
     * @name excluirCatalogo
     * @author Alysson Rafael
     * @access public
     * @param int $catid - Id do registro a ser excluído
     * @return void
     */
	function excluirCatalogo($catid){
		
		$catid = addslashes($catid);
		
		$sql = "DELETE FROM $this->stNomeTabela WHERE catid='".$catid."' ";
		$this->executar($sql);
		$this->commit();
        direcionar('?modulo=sistema/tabelasdeapoio/catalogo/pesquisar&acao=A','Operação realizada com sucesso!');
	}
	
	
	
	
	
}
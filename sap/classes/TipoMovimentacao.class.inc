<?php

/**
 * Modelo de Tipo de Movimentação
 * @author Alysson Rafael
 */
class TipoMovimentacao extends Modelo{
	
	
	/**
	* Nome da tabela especificada
	* @name $stNomeTabela
	* @var string
	* @access protected
	*/
	protected $stNomeTabela = "sap.tipomovimentacao";

	/**
	* Chave primária
	* @name $arChavePrimaria
	* @var array
	* @access protected
	*/
	protected $arChavePrimaria = array('tmvid');

	/**
	* Atributos da Tabela
	* @name $arAtributos
	* @var array
	* @access protected
	*/
	protected $arAtributos = array(
		'tmvid' => null,
		'tmvdsc' => null,
		'tmvstatus' => null
	);
		
	/**
     * Listar tipos de movimentação através dos filtros informados
     * @name listarTipoMovimentacaoPorFiltro
     * @author Alysson Rafael
     * @access public
     * @param int $tmvid - Código do tipo de movimentação
     * @param string $tmvdsc - Descrição do tipo de movimentação
     * @param string $tmvstatus - Status do tipo de movimentação
     * @return void
     */
	public function listarTipoMovimentacaoPorFiltro($tmvid,$tmvdsc,$tmvstatus){
		
		$where = " WHERE 1=1 ";
		if(!empty($tmvid)){
			$tmvid = addslashes($tmvid);
			$where .= " AND tipo.tmvid = '".$tmvid."' ";
		}
		if(!empty($tmvdsc)){
			$tmvdsc = addslashes($tmvdsc);
			$where .= " AND tipo.tmvdsc ILIKE '%".$tmvdsc."%' ";
		}
		if(!empty($tmvstatus)){
			$tmvstatus = addslashes($tmvstatus);
			if($tmvstatus == 'T'){
				$where .= " AND (tipo.tmvstatus = 'A' OR tipo.tmvstatus = 'I') ";
			}
			else{
				$where .= " AND tipo.tmvstatus = '".$tmvstatus."' ";
			}
		}
		
		$sql = "SELECT '<center>
                    <a style=\"cursor:pointer;\" onclick=\"detalharTipoMovimentacao(\''||tipo.tmvid||'\');\">
                    <img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
                    </a>
                    <a style=\"cursor:pointer;\" onclick=\"if(confirm(\'Deseja excluir o registro?\')){removerTipoMovimentacao(\''||tipo.tmvid||'\');}\">
                    <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                    </a>
                    </center>' as acao,
                    tipo.tmvid||' ' as tmvid,
                    tipo.tmvdsc as tmvdsc,
                    CASE WHEN tipo.tmvstatus='A' THEN 'Ativo' ELSE 'Inativo' END 
                    FROM sap.tipomovimentacao tipo ".$where." ORDER BY tipo.tmvdsc ";
		
		$cabecalho = array("Comando","Código","Descrição","Situação");
		$this->monta_lista($sql,$cabecalho,20,50,false,"center",'S','','',array('center','center','center','center'));
		
	}
	
	/**
     * Verificar se já existe uma descrição igual à digitada
     * @name verificaTipoRepetido
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return bool
     */
	public function verificaTipoRepetido($post){
		
		extract($post);
		
		$tmvdsc = addslashes($tmvdsc);
		
		//verifica se existe no banco um registro com a descrição informada no form
		$sql = "SELECT COUNT(*) AS tot FROM sap.tipomovimentacao WHERE tmvdsc='".$tmvdsc."' ";
		if(!empty($tmvid)){
			$tmvid = addslashes($tmvid);
			$sql .= "AND tmvid <> '".$tmvid."' ";
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
     * Cadastrar/editar tipos de movimentação
     * @name salvarTipoMovimentacao
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return array
     */
	public function salvarTipoMovimentacao($post){
		extract($post);
		
		$tmvdsc = addslashes($tmvdsc);
		$tmvstatus = addslashes($tmvstatus);
		$operacao = '';
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($tmvid)){
			$operacao = 'adicionar';
			$sql = "INSERT INTO sap.tipomovimentacao(tmvdsc,tmvstatus) VALUES('".$tmvdsc."','".$tmvstatus."') ";
		}
		else{
			$tmvid = addslashes($tmvid);
			$operacao = 'editar';
			$sql = "UPDATE sap.tipomovimentacao SET tmvdsc='".$tmvdsc."',tmvstatus='".$tmvstatus."' WHERE tmvid='".$tmvid."' ";
		}
		
		
		$sql .= " returning tmvid ";
       	$tmvid = $this->pegaUm($sql);

       	if(!empty($tmvid)){
       		$this->commit();
       		return array(0=>true,1=>$tmvid,2=>$operacao);
       	}
       	else{
       		return array(0=>false,1=>$tmvid,2=>$operacao);
       	}
		
	}
	
	/**
     * Carregar um tipo de movimentação pelo id
     * @name carregaTipoMovimentacaoPorId
     * @author Alysson Rafael
     * @access public
     * @param int $tmvid - Id a ser pesquisado
     * @return array
     */
	public function carregaTipoMovimentacaoPorId($tmvid){
		
		$tmvid = addslashes($tmvid);
		
		$sql = "SELECT tipo.tmvid,tipo.tmvdsc,tipo.tmvstatus FROM sap.tipomovimentacao tipo WHERE tipo.tmvid='".$tmvid."' ";
        return $this->pegaLinha($sql);
		
	}
	
	/**
     * Excluir um tipo de movimentação
     * @name excluirTipoMovimentacao
     * @author Alysson Rafael
     * @access public
     * @param int $tmvid - Id do registro a ser excluído
     * @return void
     */
	public function excluirTipoMovimentacao($tmvid){
		
		$tmvid = addslashes($tmvid);
		
		$sql = "DELETE FROM sap.tipomovimentacao WHERE tmvid='".$tmvid."' ";
		$this->executar($sql);
		$this->commit();
        direcionar('?modulo=sistema/tabelasdeapoio/tipoMovimentacao/pesquisar&acao=A','Operação realizada com sucesso!');
	}
	
	
	/**
     * Gera a combo de tipo de movimentação
     * @name montaComboTipoMovimentacao
     * @author Alysson Rafael
	 * @access public
	 * @param int    $tmvid       - Qual option receberá o selected
	 * @param string $obrigatorio - Se a combo vai ou não exibir a imagem de obrigatório(S ou N)
	 * @param bool   $devolucao   - Indica se o tipo de movimentação "devolução" poderá aparecer 
     * @return void
     */
	function montaComboTipoMovimentacao($tmvid = '',$obrigatorio='N', $devolucao = true, $verificaTipoMovimentacao = 'N'){
		
		if(!$devolucao)
			$where = " AND tmvid<>4 ";
			
		if($verificaTipoMovimentacao == 'S'){
			$acao = 'verificaTipoMovimentacao';
		}
		
		$sql = "SELECT tmvid AS codigo,tmvdsc AS descricao FROM sap.tipomovimentacao WHERE tmvstatus='A' $where ORDER BY tmvdsc ";
		
		$resultado = $this->carregar($sql);

		
		$this->monta_combo('tmvid',$resultado,'S','Todos...',$acao,'','','',$obrigatorio,'tmvid','',$tmvid);
		
	}
	
	/**
     * Pega o tipo de movimentação de uma movimentção
     * @name pegaTipoMovimentacao
     * @author Alysson Rafael
	 * @access public
	 * @param int    $mvbid       - Identificador da movimentação
     * @return array
     */
	public function pegaTipoMovimentacao($mvbid){
		if(!empty($mvbid)){
			$mvbid = pg_escape_string($mvbid);
			
			$sql = "SELECT tipo.tmvid,tipo.tmvdsc 
					FROM $this->stNomeTabela tipo
					JOIN sap.movimentacaobens mov ON tipo.tmvid=mov.tmvid
					WHERE mov.mvbid='{$mvbid}' ";
			
			return $this->pegaLinha($sql);
			
		}
	}
	
}
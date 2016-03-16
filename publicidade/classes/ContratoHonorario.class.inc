<?php

/**
 * Modelo de ContratoHonorario
 * @author Alysson Rafael
 */
class ContratoHonorario extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.contratohonorario';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('cthid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'cttid' => null,
		'cthhonorario' => null,
		'cthdsc' => null,
		'cthstatus' => null,
		'tseid' => null
	);
	
	
	/**
     * Cadastrar/editar 
     * @name salvar
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return array
     */
	public function salvar($post){
		extract($post);
		
		$cthhonorario = str_replace(',','.',pg_escape_string($cthhonorario));
		$tseid = pg_escape_string($tseid);
		$cthdsc = pg_escape_string($cthdsc);
		$cttid = pg_escape_string($cttid);
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($cthid)){
			$sql = "INSERT INTO $this->stNomeTabela(cttid,cthhonorario,cthdsc,tseid) ";
			$sql .= "VALUES('{$cttid}','{$cthhonorario}','{$cthdsc}','{$tseid}') ";
		}
		else if(!empty($cthid)){
			$cthid = pg_escape_string($cthid);
			$sql = "UPDATE $this->stNomeTabela SET cttid='{$cttid}',cthhonorario='{$cthhonorario}',";
			$sql .= "cthdsc='{$cthdsc}',tseid='{$tseid}' ";
			$sql .= "WHERE cthid='{$cthid}' ";
		}
		
		$sql .= "RETURNING cthid ";
		
       	$cthid = $this->pegaUm($sql);

       	if(!empty($cthid)){
       		$this->commit();
       		return array(0=>true,1=>$cttid,2=>$cthid,3=>'cadHonorario');
       	}
       	else{
       		return array(0=>false,1=>$cttid,2=>$cthid,3=>'cadHonorario');
       	}
		
	}
	
	/**
     * Listagem dos dados
     * @name listar_por_id
     * @author Alysson Rafael
     * @param int $cttid - identificador do registro pai
     * @param string $cthstatus - opção de filtrar por status
     * @access public
     * @return void
     */
	public function listar_por_id($cttid,$cthstatus=''){
		
		if(!empty($cttid)){
			$cttid = pg_escape_string($cttid);
			
			$where = " WHERE conthon.cttid='{$cttid}' ";
			
			if(!empty($cthstatus)){
				$cthstatus = pg_escape_string($cthstatus);
				$where .= "AND conthon.cthstatus='{$cthstatus}' ";
			}
			
				
			//só mostra este trecho caso o fluxo NãO seja advindo do ícone visualizar
			if($_SESSION['visualizar'] != 'S'){
				$sql = "SELECT 
					   '<center>
	                    <a style=\"cursor:pointer;\" onclick=\"if(confirm(\'Deseja realmente excluir?\')){removerHonorario(\''||conthon.cthid||'\');}\">
	                    <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir Honor&aacute;rio\">
	                    </a>
	                    </center>' as acao,";
			}
			else if($_SESSION['visualizar'] == 'S'){
				$sql = "SELECT 
					   '' as acao,";
			}
			
			$sql .= " tiposervico.tsedsc,
	                  conthon.cthhonorario,
	                  conthon.cthdsc
	                  
	                  FROM $this->stNomeTabela conthon 
	                  JOIN publicidade.tiposervico tiposervico ON conthon.tseid=tiposervico.tseid
	                  {$where} ";
			
			$cabecalho = array('A&ccedil;&atilde;o','Tipo de Servi&ccedil;o','Honor&aacute;rio %','Descri&ccedil;&atilde;o');
			$this->monta_lista($sql,$cabecalho,20,50,false,"center");
		}
		
		
	}
		
	/**
     * Pega a qtd de honorários de um contrato
     * @name pega_qtd_honorarios
     * @author Alysson Rafael
     * @access public
     * @param int $cttid - Identificador do registro pai
     * @param string $cthstatus - Opção de filtrar por status
     * @access public
     * @return int
     */
	public function pega_qtd_honorarios($cttid,$cthstatus=''){
		if(!empty($cttid)){
			$cttid = pg_escape_string($cttid);
			$sql = "SELECT COUNT(*) AS qtd FROM $this->stNomeTabela WHERE cttid='{$cttid}' ";
			
			if(!empty($cthstatus)){
				$cthstatus = pg_escape_string($cthstatus);
				$sql .= "AND cthstatus='{$cthstatus}' ";
			}
			
			$resultado = $this->pegaUm($sql);
			return $resultado;
		}
	}
	
	
	/**
     * Inativa o registro
     * @name inativar
     * @author Alysson Rafael
     * @access public
     * @param array $cthid - Identificador do registro
     * @return void
     */
	public function inativar($cthid){
		if(!empty($cthid)){
			$cthid = pg_escape_string($cthid);
			$sql = "UPDATE $this->stNomeTabela SET cthstatus='I' WHERE cthid='{$cthid}' ";
			$this->executar($sql);
			$this->commit();
		}
	}
	
	/**
     * Inativa registros filhos
     * @name inativar_honorarios_filhos
     * @author Alysson Rafael
     * @access public
     * @param int $cttid - Identificador do registro pai
     * @return void
     */
	public function inativar_honorarios_filhos($cttid){
		if(!empty($cttid)){
			$cttid = pg_escape_string($cttid);
			$sql = "UPDATE $this->stNomeTabela SET cthstatus='I' WHERE cttid='{$cttid}' ";
			$this->executar($sql);
			$this->commit();
		}
	}
	
	
	/**
	 * Recupera os registros
	 * @name carrega_registros
	 * @author Alysson Rafael
	 * @access public
	 * @param int $cttid - Identificador do registro pai
	 * @param int $tseid - Tipo de serviço 
	 * @return array
	 */
	public function carrega_registros($cttid,$tseid=''){
		if(!empty($cttid)){
			$sql = "SELECT cthhonorario FROM $this->stNomeTabela ";
			$sql .= "WHERE cthstatus='A' AND cttid='{$cttid}' ";
			if(!empty($tseid)){
				$sql .= "AND tseid='{$tseid}' ";
			}
			$sql .= "ORDER BY cthhonorario ";
			
			$resultado = $this->carregar($sql);
			return $resultado;
		}
		
	}
	
	
	/**
     * Verifica registro único
     * @name verifica_unico
     * @author Alysson Rafael
     * @param array $post - array com os dados do form
     * @access public
     * @return bool
     */
	public function verifica_unico($post){
		if(!empty($post['cthhonorario']) && !empty($post['tseid'])){
			
			$post['cthhonorario'] = str_replace(',','.',pg_escape_string($post['cthhonorario']));
			$post['tseid'] = pg_escape_string($post['tseid']);
			$post['cttid'] = pg_escape_string($post['cttid']);
			
			$sql = "SELECT COUNT(*) AS qtd FROM $this->stNomeTabela ";
			$sql .= "WHERE cthhonorario='{$post['cthhonorario']}' AND tseid='{$post['tseid']}' AND cttid='{$post['cttid']}' AND cthstatus='A' ";
			if(!empty($post['cthid'])){
				$post['cthid'] = pg_escape_string($post['cthid']);
				$sql .= "AND cthid <> '{$post['cthid']}' ";
			}
			$qtd = $this->pegaUm($sql);
			if($qtd >= 1){
				return true;
			}
			else{
				return false;
			}
			
		}
		
	}

	
}
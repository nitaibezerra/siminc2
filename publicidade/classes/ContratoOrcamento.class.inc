<?php

/**
 * Modelo de ContratoOrcamento
 * @author Alysson Rafael
 */
class ContratoOrcamento extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.contratoorcamento';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('ctoid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'cttid' => null,
		'ctoanoexercicio' => null,
		'ctovalor' => null,
		'ctostatus' => null
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
		
		$ctoanoexercicio = pg_escape_string($ctoanoexercicio);
		$ctovalor = str_replace(array('.',','),array('','.'),pg_escape_string($ctovalor));
		$cttid = pg_escape_string($cttid);
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($ctoid)){
			$sql = "INSERT INTO $this->stNomeTabela(cttid,ctoanoexercicio,ctovalor) ";
			$sql .= "VALUES('{$cttid}','{$ctoanoexercicio}','{$ctovalor}') ";
		}
		else if(!empty($ctoid)){
			$ctoid = pg_escape_string($ctoid);
			$sql = "UPDATE $this->stNomeTabela SET cttid='{$cttid}',ctoanoexercicio='{$ctoanoexercicio}',";
			$sql .= "ctovalor='{$ctovalor}' WHERE ctoid='{$ctoid}' ";
		}
		
		$sql .= "RETURNING ctoid ";
		
       	$ctoid = $this->pegaUm($sql);

       	if(!empty($ctoid)){
       		$this->commit();
       		return array(0=>true,1=>$cttid,2=>$ctoid,3=>'cadOrcamento');
       	}
       	else{
       		return array(0=>false,1=>$cttid,2=>$ctoid,3=>'cadOrcamento');
       	}
		
	}
	
	/**
     * Listagem dos dados
     * @name listar_por_id
     * @author Alysson Rafael
     * @param int $cttid - identificador do registro pai
     * @access public
     * @return void
     */
	public function listar_por_id($cttid){
		
		if(!empty($cttid)){
			$cttid = pg_escape_string($cttid);
			
			$where = " WHERE contorc.cttid='{$cttid}' ";
				
			//só mostra este trecho caso o fluxo NÃO seja advindo do ícone visualizar
			if($_SESSION['visualizar'] != 'S'){
            	$sql = "SELECT 
					   '<center>
	                    <a style=\"cursor:pointer;\" onclick=\"detalharOrcamento(\''||contorc.ctoid||'\');\">
	                    <img src=\"/imagens/alterar.gif \" border=0 title=\"Editar Exercício\">
	                    </a>
	                    </center>' as acao,";
			}
			else if($_SESSION['visualizar'] == 'S'){
				$sql = "SELECT 
					   '' as acao,";
			}
			
			$sql .= "  contorc.ctoanoexercicio||' ',
	                  contorc.ctovalor
	                  
	                  FROM $this->stNomeTabela contorc 
	                  {$where} ";
			
			$cabecalho = array('A&ccedil;&atilde;o','Ano de Exerc&iacute;cio','Verba (R$)');
			$this->monta_lista($sql,$cabecalho,20,50,true,"center");
		}
	}
	
	
	/**
     * Pega a qtd de orçamentos de um contrato
     * @name pega_qtd_orcamentos
     * @author Alysson Rafael
     * @access public
     * @param int $cttid - Identificador do registro pai
     * @return int
     */
	public function pega_qtd_orcamentos($cttid){
		if(!empty($cttid)){
			$cttid = pg_escape_string($cttid);
			$sql = "SELECT COUNT(*) AS qtd FROM $this->stNomeTabela WHERE cttid='{$cttid}' ";
			$resultado = $this->pegaUm($sql);
			return $resultado;
		}
	}
	
	
	/**
     * Carrega um registro pelo id
     * @name carrega_registro_por_id
     * @author Alysson Rafael
     * @access public
     * @param int $ctoid - Identificador do registro
     * @return array
     */
	public function carrega_registro_por_id($ctoid){
		if(!empty($ctoid)){
			$ctoid = pg_escape_string($ctoid);
			$sql = "SELECT contorc.ctoid,
						   contorc.ctoanoexercicio,
						   contorc.ctovalor,
						   contorc.ctostatus	
						   FROM $this->stNomeTabela contorc WHERE contorc.ctoid='{$ctoid}' ";
			
			return $this->pegaLinha($sql);
		}
	}
	
	/**
     * Inativa registros filhos
     * @name inativar_orcamentos_filhos
     * @author Alysson Rafael
     * @access public
     * @param int $cttid - Identificador do registro pai
     * @return void
     */
	public function inativar_orcamentos_filhos($cttid){
		if(!empty($cttid)){
			$cttid = pg_escape_string($cttid);
			$sql = "UPDATE $this->stNomeTabela SET ctostatus='I' WHERE cttid='{$cttid}' ";
			$this->executar($sql);
			$this->commit();
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
		if(!empty($post['ctoanoexercicio'])){
			
			$post['ctoanoexercicio'] = pg_escape_string($post['ctoanoexercicio']);
			$post['cttid'] = pg_escape_string($post['cttid']);
			$post['ctovalor'] = str_replace(array('.',','),array('','.'),pg_escape_string($post['ctovalor']));
			
			$sql = "SELECT COUNT(*) AS qtd FROM $this->stNomeTabela ";
			$sql .= "WHERE ctoanoexercicio='{$post['ctoanoexercicio']}' AND cttid='{$post['cttid']}' AND ctovalor='{$post['ctovalor']}' ";
			if(!empty($post['ctoid'])){
				$post['ctoid'] = pg_escape_string($post['ctoid']);
				$sql .= "AND ctoid <> '{$post['ctoid']}' ";
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
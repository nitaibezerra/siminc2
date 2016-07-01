<?php

/**
 * Modelo de Parametro
 * @author Alysson Rafael
 */
class Parametro extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.parametro';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('parid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'parfiscaltitular' => null,
		'parfiscalsubstituto' => null,
		'parchefetitular' => null,
		'parchefesubstituto' => null,
		'parstatus' => null
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
		
		$parfiscaltitular = pg_escape_string($parfiscaltitular);
		$parfiscalsubstituto = pg_escape_string($parfiscalsubstituto);
		$parchefetitular = pg_escape_string($parchefetitular);
		$parchefesubstituto = pg_escape_string($parchefesubstituto);
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($parid)){
			$sql = "INSERT INTO $this->stNomeTabela(parfiscaltitular,parfiscalsubstituto,";
			$sql .= "parchefetitular,parchefesubstituto)";
			$sql .= "VALUES('{$parfiscaltitular}','{$parfiscalsubstituto}',";
			$sql .= "'{$parchefetitular}','{$parchefesubstituto}') ";
			
		}
		else if(!empty($parid)){
			$parid = pg_escape_string($parid);
			$sql = "UPDATE $this->stNomeTabela SET parfiscaltitular='{$parfiscaltitular}',";
			$sql .= "parfiscalsubstituto='{$parfiscalsubstituto}',parchefetitular='{$parchefetitular}',";
			$sql .= "parchefesubstituto='{$parchefesubstituto}' ";
			$sql .= "WHERE parid='{$parid}' ";
		}
		
		$sql .= "RETURNING parid ";
		
       	$parid = $this->pegaUm($sql);

       	if(!empty($parid)){
       		$this->commit();
       		return array(0=>true,1=>$parid,2=>'cadParametros');
       	}
       	else{
       		return array(0=>false,1=>$parid,2=>'cadParametros');
       	}
		
	}
	
	
	/**
     * Carrega um registro pelo id
     * @name carrega_registro
     * @author Alysson Rafael
     * @access public
     * @param int $parid - Identificador do registro
     * @return array
     */
	public function carrega_registro(){
		$sql = "SELECT par.parid,
						   par.parfiscaltitular,
						   par.parfiscalsubstituto,
						   par.parchefetitular,
						   par.parchefesubstituto,
						   par.parstatus
						   FROM $this->stNomeTabela par ";
			
		return $this->pegaLinha($sql);
	}
	
	/**
     * Pega o nome do fiscal
     * @name pegar_fiscal
     * @author Alysson Rafael
     * @access public
     * @param string $tipo - Se é para pegar o substituto ou o titular
     * @return string
     */
	public function pegar_fiscal($tipo){
		if(!empty($tipo)){
			$tipo = pg_escape_string($tipo);
			if($tipo == 'S'){
				$sql = "SELECT parfiscalsubstituto FROM $this->stNomeTabela ";
			}
			else if($tipo == 'T'){
				$sql = "SELECT parfiscaltitular FROM $this->stNomeTabela ";
			}
			$fiscal = $this->pegaUm($sql);
			return $fiscal;
		}
	}
	
	/**
     * Pega o nome do chefe
     * @name pegar_chefe
     * @author Alysson Rafael
     * @access public
     * @param string $tipo - Se é para pegar o substituto ou o titular
     * @return string
     */
	public function pegar_chefe($tipo){
		if(!empty($tipo)){
			$tipo = pg_escape_string($tipo);
			if($tipo == 'S'){
				$sql = "SELECT parchefesubstituto FROM $this->stNomeTabela ";
			}
			else if($tipo == 'T'){
				$sql = "SELECT parchefetitular FROM $this->stNomeTabela ";
			}
			$chefe = $this->pegaUm($sql);
			return $chefe;
		}
	}
	

	
}
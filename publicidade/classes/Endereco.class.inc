<?php

/**
 * Modelo da visão de Endereço
 * @author Alysson Rafael
 */
class Endereco extends Modelo{
	
	/**
	* Nome da tabela especificada
	* @name $stNomeTabela
	* @var string
	* @access protected
	*/
	protected $stNomeTabela = 'cep.v_endereco2 ';
	
	/**
	* Chave primária
	* @name $arChavePrimaria
	* @var array
	* @access protected
	*/
	protected $arChavePrimaria = array('cep');
	
	/**
	* Atributos da Tabela
	* @name $arAtributos
	* @var array
	* @access protected
	*/
	protected $arAtributos = array(
		'logradouro' => null,
		'bairro' => null,
		'cidade' => null,
		'estado' => null,
		'latitude' => null,
		'hemisferio' => null,
		'longitude' => null,
		'meridiano' => null,
		'altitude' => null,
		'medidaarea' => null,
		'medidaraio' => null,
		'muncod' => null,
		'muncodcompleto' => null
	);
	
	/**
	 * Recupera endereço pelo cep
	 * @name pegar_registro
	 * @author Alysson Rafael
	 * @access public
	 * @param int $cep - CEP a ser pesquisado
	 * @return array
	 */
	public function pegar_registro($cep){
	
		$cep = pg_escape_string($cep);
		
		$sql = "SELECT * FROM $this->stNomeTabela WHERE cep='{$cep}' ";

		$return = $this->carregar($sql);
		
		return $return[0];
		
	}
	
	
	
}
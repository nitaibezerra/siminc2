<?php
class Municipio extends Modelo {
	
	/**
	 * Nome da tabela especificada
	 * 
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela =  "territorios.municipio";
	
	/**
	 * Chave primaria.
	 * 
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
			"muncod"
	);
	
	/**
	 * Atributos
	 * 
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
			'muncod' => null,
			'estuf' => null,
			'mundescricao' => null,
	);
}
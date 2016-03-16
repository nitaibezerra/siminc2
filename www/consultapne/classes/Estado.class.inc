<?php
class Estado extends Modelo {
	
	/**
	 * Nome da tabela especificada
	 * 
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela =  "territorios.estado";
	
	/**
	 * Chave primaria.
	 * 
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
			"estuf"
	);
	
	/**
	 * Atributos
	 * 
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
			'estuf' => null,
			'muncodcapital' => null,
			'regcod' => null,
			'estcod' => null,
			'estdescricao' => null,
	);
}
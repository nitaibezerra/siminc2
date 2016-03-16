<?php
class Configuracao extends Modelo {
	
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "premiopnae.configuracao";
	
	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
			"conid" 
	);
	
	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
			'conid' => null,
			'dataexpiracao' => null 
	);
}
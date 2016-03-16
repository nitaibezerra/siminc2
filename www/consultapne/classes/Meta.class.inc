<?php
class Meta extends Modelo {

	
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "consultapne.meta";
	
	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
			"metid" 
	);
	
	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
			'metid' => null,
			'mettitulo' => null,
			'metfontemunicipio' => null,
			'metfonteestado' => null,
			'metstatus' => null
	);
}
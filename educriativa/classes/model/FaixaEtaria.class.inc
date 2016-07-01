<?php
class Educriativa_Model_FaixaEtaria extends Modelo {

	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "criatividadeeducacao.faixaetaria";

	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
		"faeid"
	);

	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	*/
	protected $arAtributos = array (
		'faeid' => null,
		'faedsc' => null,
	);
}
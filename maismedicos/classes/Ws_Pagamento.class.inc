<?php
	
class Ws_Pagamento extends Modelo{
	
	/**
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "maismedicos.ws_pagamento";
	
	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array( "wspid" );
	
	/**
	 * Atributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributos     = array(
								  'wspid'=> null,
								  'wspdata'=> null,
								  'cpfAutorizador'=> null,
								  'nomeAutorizador'=> null,
								  'tipoBolsista'=> null,
								  'cpfBolsista'=> null,
								  'nomeBolsista'=> null,
								  'mesReferencia'=> null,
								  'anoReferencia'=> null,
								  'descricao'=> null,
								  'autorizado'=> null,
								  'dataRegistro'=> null,
								  'wspstatus'=> null,
								);
	 
}
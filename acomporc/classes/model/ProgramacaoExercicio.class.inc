<?php

class Acomporc_Model_ProgramacaoExercicio extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.programacaoexercicio";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "prsano" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
	  	'prsano' => null,
	  	'prsdata_inicial' => null,
	  	'prsdata_termino' => null,
	  	'prsexerccorrente' => null,
	  	'prsstatus' => null,
	  	'prsativo' => null,
	  	'prsexercicioaberto' => null,
	);
}
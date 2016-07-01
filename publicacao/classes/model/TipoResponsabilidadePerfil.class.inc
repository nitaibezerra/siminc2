<?php

class Acomporc_Model_TipoResponsabilidadePerfil extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.tprperfil";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "prfid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
	  	'prfid' => null,
	  	'tprcod' => null,
	  	'pflcod' => null,
	);
}
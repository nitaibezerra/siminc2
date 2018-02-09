<?php

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 *
 */
class Emendas_Model_Pronac extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emendas.pronac";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "proid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'proid' => null,
        'benid' => null,
        'pronumero' => null,
    );
}

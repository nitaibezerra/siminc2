<?php

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 *
 */
class Emendas_Model_Sniic extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emendas.sniic";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sniid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'sniid' => null,
        'benid' => null,
        'sninumero' => null,
    );
}

<?php
/**
 * Classe de mapeamento da entidade aspar.referencia.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade aspar.referencia.
 *
 * @see Modelo
 */
class Aspar_Model_Referencia extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'aspar.referencia';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'refid' => null,
        'refdsc' => null,
    );

}
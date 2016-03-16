<?php
/**
 * Classe de mapeamento da entidade aspar.tipoimpacto.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade aspar.tipoimpacto.
 *
 * @see Modelo
 */
class Aspar_Model_TipoImpacto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'aspar.tipoimpacto';

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
        'tipid' => null,
        'tipdsc' => null,
    );

}
<?php
/**
 * Classe de mapeamento da entidade aspar.prioridade.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade aspar.prioridade.
 *
 * @see Modelo
 */
class Aspar_Model_Prioridade extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'aspar.prioridade';

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
        'priid' => null,
        'pridsc' => null,
    );

}
<?php
/**
 * Classe de mapeamento da entidade aspar.posicionamento.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade aspar.posicionamento.
 *
 * @see Modelo
 */
class Aspar_Model_Posicionamento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'aspar.posicionamento';

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
        'posid' => null,
        'posdsc' => null,
    );

}
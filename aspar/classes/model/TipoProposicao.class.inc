<?php
/**
 * Classe de mapeamento da entidade aspar.tipoproposicao.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade aspar.tipoproposicao.
 *
 * @see Modelo
 */
class Aspar_Model_TipoProposicao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'aspar.tipoproposicao';

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
        'tprid' => null,
        'tprsigla' => null,
        'tprdsc' => null,
        'tprcasa' => null,
    );

}
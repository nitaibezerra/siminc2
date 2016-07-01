<?php
/**
 * Classe de mapeamento da entidade par3.endereco.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade par3.endereco.
 *
 * @see Modelo
 */
class Par3_Model_Endereco extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.endereco';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'endid',
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
        'endid' => null,
        'endcep' => null,
        'endlogradouro' => null,
        'endcomplemento' => null,
        'endnumero' => null,
        'endbairro' => null,
    );


}//end Class
?>
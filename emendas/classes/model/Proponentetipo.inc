<?php
/**
 * Classe de mapeamento da entidade emendas.proponentetipo
 *
 * @category Class
 * @package  A1
 * @author   Adriano <teste@teste.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-12-2017
 * @link     no link
 */



/**
 * Emendas_Model_Proponentetipo
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_Proponentetipo extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.proponentetipo';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'prtid',
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
        'prtid' => null,
        'prtdsc' => null,
        'prtstatus' => null,
    );

}//end Class
?>
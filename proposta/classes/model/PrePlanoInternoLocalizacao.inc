<?php
/**
 * Classe de mapeamento da entidade proposta.preplanointernolocalizacao
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <teste@teste.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-04-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Proposta_Model_PrePlanoInternoLocalizacao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Proposta_Model_PrePlanoInternoLocalizacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proposta.preplanointernolocalizacao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'ploid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'paiid' => array('tabela' => 'territorios.pais', 'pk' => 'paiid'),
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
        'pliid' => array('tabela' => 'proposta.preplanointerno', 'pk' => 'pliid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ploid' => null,
        'pliid' => null,
        'paiid' => null,
        'estuf' => null,
        'muncod' => null,
    );

}//end Class
?>
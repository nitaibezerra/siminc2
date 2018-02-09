<?php
/**
 * Classe de mapeamento da entidade acompanhamento.acompanhamentomedida
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-01-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_AcompanhamentoMedida
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_AcompanhamentoMedida extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.acompanhamentomedida';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'acmid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'medid' => array('tabela' => 'acompanhamento.medida', 'pk' => 'medid'),
        'acoid' => array('tabela' => 'acompanhamento.acompanhamento', 'pk' => 'acoid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'acmid' => null,
        'acoid' => null,
        'medid' => null,
    );

}//end Class
?>
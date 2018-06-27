<?php

/**
 * Classe de mapeamento da entidade acompanhamento.indicador_pnc
 */

require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Indicadorpnc
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Indicadorpnc extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.indicador_pnc';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'idpid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'acpid' => array('tabela' => 'acompanhamento.acompanhamento_pnc', 'pk' => 'acpid')
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'acpid' => null,
        'ipncod' => null,
        'unocod' => null,
        'suocod' => null,
        'prsano' => null,
        'idpquantidade' => null,
        'idpdata' => null,
        'idpjustificativa' => null,
        'idpstatus' => null,
    );

}

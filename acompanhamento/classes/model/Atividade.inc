<?php
/**
 * Classe de mapeamento da entidade acompanhamento.atividade
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-02-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Atividade
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Atividade extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.atividade';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'atiid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
        'suoid' => array('tabela' => 'subunidadeorcamentaria', 'pk' => 'suoid'),
        'mpnid' => array('tabela' => 'metapnc', 'pk' => 'mpnid'),
        'janid' => array('tabela' => 'acompanhamento.janela', 'pk' => 'janid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'atiid' => null,
        'atidsc' => null,
        'atidata' => null,
        'atiproduto' => null,
        'atiquantidade' => null,
        'estuf' => null,
        'muncod' => null,
        'mpnid' => null,
        'suoid' => null,
        'janid' => null,
        'atistatus' => null,
    );

}//end Class
?>
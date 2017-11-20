<?php
/**
 * Classe de mapeamento da entidade emendas.usuarioresponsabilidade
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Emendas_Model_Usuarioresponsabilidade
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_Usuarioresponsabilidade extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.usuarioresponsabilidade';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'rpuid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
        'pflcod' => array('tabela' => 'perfil', 'pk' => 'pflcod'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'rpuid' => null,
        'pflcod' => null,
        'usucpf' => null,
        'rpustatus' => null,
        'rpudata_inc' => null,
    );

}//end Class
?>
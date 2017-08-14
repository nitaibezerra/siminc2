<?php
/**
 * Classe de mapeamento da entidade public.objetivoppa
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-05-2017
 * @link     no link
 */


require_once APPRAIZ . 'includes/classes/Modelo.class.inc';


/**
 * Public_Model_Objetivoppa
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_ProgramaPpa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.programappa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'prgid',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'prgid' => null,
        'prsano' => null,
        'prgcod' => null,
        'prgnome' => null,
        'prgdsc' => null,
        'prgstatus' => null,
    );
}//end Class
?>
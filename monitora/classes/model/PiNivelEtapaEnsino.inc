<?php
/**
 * Classe de mapeamento da entidade monitora.pi_niveletapaensino
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-12-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Monitora_Model_Pi_niveletapaensino
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Monitora_Model_PiNivelEtapaEnsino extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'monitora.pi_niveletapaensino';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'neeid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'mdeid' => array('tabela' => 'pi_modalidadeensino', 'pk' => 'mdeid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'neeid' => null,
        'needsc' => null,
        'neeano' => null,
        'neestatus' => null,
        'mdeid' => null,
        'neecod' => null,
    );

    /**
     * Retorna os Segmentos por Àrea Cultural.
     *
     * @param $mdeid
     * @return array
     * @access public
     */
    public function RecuperaSegmentoPorArea($mdeid)
    {
        global $db;

        $sql ="SELECT
                    neeid,
                    needsc,
                    mdeid,
                    neecod
                FROM
                    monitora.pi_niveletapaensino
                WHERE
                    mdeid = $mdeid
                    AND neestatus = 'A'
        ";

        return $db->carregar($sql);
    }

}//end Class
?>
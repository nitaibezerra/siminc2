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
class Public_Model_ObjetivoPpa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.objetivoppa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'oppid',
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
        'oppid' => null,
        'prsano' => null,
        'oppdsc' => null,
        'oppnome' => null,
        'oppcod' => null,
        'oppstatus' => null,
    );

    public function recuperarSqlCombo()
    {
        return "SELECT
                    oppid AS codigo,
                    oppcod || '-' || oppnome AS descricao
                FROM public.objetivoppa
                WHERE
                    oppstatus = 'A'
                    AND prsano = '{$_SESSION['exercicio']}'
                ORDER BY
                    oppcod";
    }

}//end Class
?>
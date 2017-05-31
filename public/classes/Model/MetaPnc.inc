<?php
/**
 * Classe de mapeamento da entidade public.metapnc
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-05-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Public_Model_Metapnc
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_MetaPnc extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.metapnc';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'mpnid',
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
        'mpnid' => null,
        'mpndsc' => null,
        'mpnstatus' => null,
        'mpncod' => null,
        'mpnnome' => null,
        'prsano' => null,
    );

    public function recuperarSqlCombo()
    {
        return "SELECT
                    mpncod::NUMERIC AS ordem,
                    mpnid AS codigo,
                    mpncod || '-' || mpnnome AS descricao
                FROM public.metapnc
                WHERE mpnstatus = 'A'
                AND prsano  = '{$_SESSION['exercicio']}'
                ORDER BY ordem";
    }

}//end Class
?>
<?php
/**
 * Classe de mapeamento da entidade public.areacultural
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
 * Public_Model_Areacultural
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_AreaCultural extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.areacultural';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'acuid',
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
        'acuid' => null,
        'acucod' => null,
        'acudsc' => null,
        'acuano' => null,
        'acustatus' => null,
    );

    public function recuperarSqlCombo()
    {
        return "SELECT 
                        MAX(acuid) AS codigo,
                        upper(public.removeacento(acudsc)) AS descricao
                        FROM monitora.pi_modalidadeensino
                        WHERE acustatus = 'A'
                        AND acuano = '{$_SESSION['exercicio']}' -- Resolver 2014
                        GROUP BY descricao
                        ORDER BY descricao ASC";
    }


}//end Class
?>
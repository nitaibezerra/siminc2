<?php
/**
 * Classe de mapeamento da entidade emendas.siconvbeneficiario
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-12-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Emendas_Model_SiconvBeneficiario
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Emendas_Model_SiconvBeneficiario extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.siconvbeneficiario';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'sibid',
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
        'sibid' => null,
        'sicid' => null,
        'procnpj' => null,
        'emenumero' => null,
        'benid' => null,
    );
    public function recuperarListagem($emenumero = null)
    {
        $sql = "
            SELECT
                sibid AS codigo,
                procnpj || ' - ' || emenumero AS beneficiario, benid
            FROM emendas.siconvbeneficiario
            WHERE emenumero = '$emenumero'
            ORDER BY sibid";
            return $sql;
      }
}//end Class
?>

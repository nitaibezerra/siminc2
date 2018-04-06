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
    public function recuperarListagem($benid){

          $sql = "select sib.sibid, sib.procnpj || ' - ' || sib.emenumero AS beneficiario, sib.benid, ben.benid AS ben
                  from emendas.beneficiario ben
                	inner join emendas.proponente pro on pro.proid = ben.proid
                	inner join emendas.siconvbeneficiario sib on sib.procnpj = pro.procnpj
                	inner join emendas.emenda eme on eme.emeid = ben.emeid
                  where ben.benid = '$benid'
                  and eme.emenumero = sib.emenumero
                  Order By sib.emenumero
                  ";

        return $this->carregar($sql);
    }
}//end Class
?>

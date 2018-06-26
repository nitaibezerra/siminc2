<?php
/**
 * Classe de mapeamento da entidade emendas.beneficiariodetalhe
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 07-12-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Emendas_Model_BeneficiarioDetalhe
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_BeneficiarioDetalhe extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.beneficiariodetalhe';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'bedid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'emdid' => array('tabela' => 'emendas.emendadetalhe', 'pk' => 'emdid'),
        'benid' => array('tabela' => 'emendas.beneficiario', 'pk' => 'benid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'bedid' => null,
        'benid' => null,
        'emdid' => null,
        'bedvalor' => null,
        'bedvalorpriorizado' => null,
        'bedstatus' => null,
    );

    public function recuperarListagem($benid)
    {
        $sql = "
            SELECT
                *
            FROM emendas.beneficiariodetalhe bed
                JOIN emendas.emendadetalhe emd on emd.emdid = bed.emdid
            WHERE
                bed.benid = ". (int)$benid. "
            ORDER BY
                gndcod
        ";
        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
    
    /**
     * Retorna a somatoria dos valores dos beneficiarios agrupados pelo emdid
     * @param type $benid
     * @return type
     */
    public function recuperarSomaGnd($benid, $bedid=null){
        $sql = "SELECT emd.emdid,
                       sum(bedvalor) as valor_total
                  FROM emendas.beneficiariodetalhe bed
                  JOIN emendas.emendadetalhe emd on emd.emdid = bed.emdid
                 WHERE bed.benid = ". (int)$benid. "
                   AND bed.bedid <> ".(int)$bedid. "
                   AND bed.bedstatus = 'A'
                 GROUP BY emd.emdid
                 ORDER BY emd.emdid
        ";
        $dados = $this->carregar($sql);
        return $dados ? $dados : [];        
    }

}//end Class
?>
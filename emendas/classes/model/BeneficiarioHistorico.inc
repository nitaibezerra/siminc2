<?php
/**
 * Classe de mapeamento da entidade emendas.beneficiariohistorico
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Emendas_Model_BeneficiarioHistorico
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Emendas_Model_BeneficiarioHistorico extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.beneficiariohistorico';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'behid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'benid' => array('tabela' => 'emendas.beneficiario', 'pk' => 'benid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'behid' => null,
        'behdsc' => null,
        'behdata' => null,
        'usucpf' => null,
        'behstatus' => null,
        'benid' => null,
    );

    public function montarRecuperarPorUnidade($listaUnidades,$benid){
      $sql = "SELECT *
        FROM emendas.beneficiariohistorico bh
        LEFT JOIN emendas.usuarioresponsabilidade  ur  ON bh.usucpf = ur.usucpf
        WHERE
           bh.behstatus = 'A'
           and bh.benid = " . (int)$benid
         ;

         if(count($listaUnidades)){
           $sql .= " and ur.ungcod IN('" . join($listaUnidades, "','") . "')";
         }
        return $sql;
    }

    public function recuperarPorUnidade($listaUnidades,$benid){
      $sql = $this->montarRecuperarPorUnidade($listaUnidades,$benid);
      return $this->carregar($sql);
    }

}//end Class
?>

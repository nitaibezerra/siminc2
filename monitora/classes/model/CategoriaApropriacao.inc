<?php
/**
 * Classe de mapeamento da entidade monitora.pi_categoriaapropriacao
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-08-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Monitora_Model_Pi_categoriaapropriacao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Monitora_Model_CategoriaApropriacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'monitora.pi_categoriaapropriacao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'capid',
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
        'capid' => null,
        'capcod' => null,
        'capdsc' => null,
        'capano' => null,
        'capstatus' => null,
        'capsiconv' => null,
    );

    public function existeVinculo($dados = null)
    {
        $sql = "select count(*) qtd from monitora.pi_planointerno pli where capid = {$this->capid}";
        return $this->pegaUm($sql);
    }

}//end Class
?>
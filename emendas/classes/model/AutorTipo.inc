<?php
/**
 * Classe de mapeamento da entidade emendas.autortipo
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Emendas_Model_AutorTipo
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_AutorTipo extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.autortipo';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'atpid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'agrid' => array('tabela' => 'emendas.autorgrupo', 'pk' => 'agrid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'atpid' => null,
        'atpdsc' => null,
        'agrid' => null,
        'atpstatus' => null,
    );

    /**
     * Monta consulta para retornar dados para a combo.
     * 
     * @param stdClass $filtro
     * @return string
     */
    public static function buscarTodosCombo(stdClass $filtro)
    {
        $where = '';
//        if($filtro->unicod){
//            $where = "\n                AND suo.unocod::INTEGER IN(". join(',', $filtro->unicod). ") ";
//        }
        
        $sql = "
            SELECT
                atpid AS codigo,
                atpdsc AS descricao
            FROM emendas.autortipo
            WHERE
                atpstatus = 'A' $where
            ORDER BY
                descricao
        ";
//ver($sql,d);
        return $sql;
    }
    
}//end Class
?>
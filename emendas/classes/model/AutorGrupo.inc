<?php
/**
 * Classe de mapeamento da entidade emendas.autorgrupo
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
 * Emendas_Model_AutorGrupo
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_AutorGrupo extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.autorgrupo';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'agrid',
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
        'agrid' => null,
        'agrdsc' => null,
        'agrstatus' => null,
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
                agrid AS codigo,
                agrdsc AS descricao
            FROM emendas.autorgrupo
            WHERE
                agrstatus = 'A' $where
            ORDER BY
                descricao
        ";
//ver($sql,d);
        return $sql;
    }
    
}//end Class
?>
<?php

/**
 * Emendas_Model_Proponentetipo
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_Proponentetipo extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.proponentetipo';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'prtid',
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
        'prtid' => null,
        'prtdsc' => null,
        'prtstatus' => null,
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
                prtid AS codigo,
                prtdsc AS descricao
            FROM emendas.proponentetipo
            WHERE
                prtstatus = 'A' $where
            ORDER BY
                descricao
        ";
//ver($sql,d);
        return $sql;
    }
    
}//end Class
?>
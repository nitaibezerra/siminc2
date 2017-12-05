<?php
/**
 * Classe de mapeamento da entidade emendas.partido
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
 * Emendas_Model_Partido
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_Partido extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.partido';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'parid',
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
        'parid' => null,
        'pardsc' => null,
        'parsigla' => null,
        'parcod' => null,
        'parstatus' => null,
        'parlado' => null,
    );
    
    /**
     * Monta consulta pra buscar dados da entidade.
     * 
     * @param stdClass $filtros
     * @return string
     */
    public static function montarSqlBuscarTodos(stdClass $filtros) {
        $where = '';
//        $where .= $filtros->prsano? "\n AND pij.prsano = '".(int) $filtros->prsano. "'": NULL;
//        $where .= $filtros->periodoVigente? "\n AND NOW() BETWEEN pij.pijinicio AND pij.pijfim ": NULL;
        
        $sql = "
            SELECT
                autid,
                autcod,
                a.autnome AS autnome,
                aut.atpdsc,
                p.parsigla,
                a.autemail autemail,
                a.estuf
            FROM emendas.autor a -- SELECT * FROM emendas.autor a
                JOIN emendas.autortipo aut ON a.atpid = aut.atpid -- SELECT * FROM emendas.autortipo
                JOIN emendas.partido p ON a.parid = p.parid -- SELECT * FROM emendas.partido
            WHERE
                autstatus = 'A' $where
            ORDER BY
                autnome
        ";
//ver($sql,d);
        return $sql;
    }
    
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
                parid AS codigo,
                pardsc || '(' || parsigla || ')' AS descricao
            FROM emendas.partido
            WHERE
                parstatus = 'A' $where
            ORDER BY
                descricao
        ";
//ver($sql,d);
        return $sql;
    }

}//end Class
?>
<?php
/**
 * Classe de mapeamento da entidade emendas.autor
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
 * Emendas_Model_Autor
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_Autor extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.autor';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'autid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
        'parid' => array('tabela' => 'emendas.partido', 'pk' => 'parid'),
        'atpid' => array('tabela' => 'emendas.autortipo', 'pk' => 'atpid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'autid' => null,
        'atpid' => null,
        'autnome' => null,
        'autemail' => null,
        'estuf' => null,
        'autcod' => null,
        'parid' => null,
        'autstatus' => null,
        'autsexo' => null
    );

    /**
     * Monta consulta pra buscar dados das janelas.
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
                a.autsexo,
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
     * Buscar dados da entidade.
     * 
     * @global type $db
     * @param stdClass $filtros
     * @return array/boolean
     */
    public static function buscarTodos(stdClass $filtros) {
        global $db;
        $sql = self::montarSqlBuscarTodos($filtros);
        $resultado = $db->carregar($sql);
        $lista = $resultado? $resultado: array();
        return $lista;
    }
    
}//end Class
?>
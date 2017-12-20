<?php
/**
 * Classe de mapeamento da entidade emendas.proponente
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
 * Emendas_Model_Proponente
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_Proponente extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.proponente';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'proid',
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
        'proid' => null,
        'prtid' => null,
        'pronome' => null,
        'procnpj' => null,
        'protelefone' => null,
        'proemail' => null,
        'prostatus' => null,
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
                p.proid,
                pt.prtdsc AS tipo,
                p.pronome,
                p.procnpj,
                p.protelefone,
                p.proemail
            FROM emendas.proponente p -- SELECT * FROM emendas.autor a
                LEFT JOIN emendas.proponentetipo pt ON p.prtid = pt.prtid -- SELECT * FROM emendas.autortipo
            WHERE
                prostatus = 'A' $where
            ORDER BY
                pronome
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
    
}

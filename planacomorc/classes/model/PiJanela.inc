<?php
/**
 * Classe de mapeamento da entidade planacomorc.pi_janela
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 21-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Planacomorc_Model_PiJanela
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Planacomorc_Model_PiJanela extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planacomorc.pi_janela';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'pijid',
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
        'pijid' => null,
        'pijdsc' => null,
        'pijinicio' => null,
        'pijfim' => null,
        'pijcadastro' => null,
        'usucpf' => null,
        'prsano' => null,
        'pijstatus' => null,
    );
    
    /**
     * Monta consulta pra buscar dados das janelas.
     * 
     * @param stdClass $filtros
     * @return string
     */
    public static function montarSqlBuscarTodos(stdClass $filtros) {
        $where = '';
        $where .= $filtros->prsano? "\n AND pij.prsano = '".(int) $filtros->prsano. "'": NULL;
        $where .= $filtros->periodoVigente? "\n AND NOW() BETWEEN pij.pijinicio AND pij.pijfim ": NULL;
        
        $sql = "
            SELECT
                pij.pijid,
                pij.pijdsc,
                TO_CHAR(pij.pijinicio, 'DD/MM/YYYY') AS pijinicio,
                TO_CHAR(pij.pijfim, 'DD/MM/YYYY') AS pijfim,
                INITCAP(ucad.usunome) AS usunome
            FROM planacomorc.pi_janela pij
                JOIN seguranca.usuario ucad ON pij.usucpf = ucad.usucpf
            WHERE
                pij.pijstatus = 'A' $where
        ";
        return $sql;
    }
    
    /**
     * Buscar dados das janelas.
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
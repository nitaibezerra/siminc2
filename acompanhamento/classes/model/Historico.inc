<?php
/**
 * Classe de mapeamento da entidade acompanhamento.historico
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-01-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Historico
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Historico extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.historico';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'hisid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
        'acoid' => array('tabela' => 'acompanhamento.acompanhamento', 'pk' => 'acoid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'hisid' => null,
        'acoid' => null,
        'usucpf' => null,
        'hisdata' => null,
    );

    public function recuperarHistorico($acoid)
    {
        $sql = "select his.*, usu.usunome, to_char(hisdata, 'DD/MM/YYYY HH24:MI:SS') hisdata
                from acompanhamento.historico his
                        inner join seguranca.usuario usu on usu.usucpf = his.usucpf
                where his.acoid = " . (int)$acoid . "
                order by his.hisdata desc";

        return $this->carregar($sql);
    }

}//end Class
?>
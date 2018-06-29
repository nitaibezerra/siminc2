<?php
/**
 * Classe de mapeamento da entidade acompanhamento.historicopnc
 *
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Historicopnc
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Historicopnc extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.historicopnc';

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
        'acpid' => array('tabela' => 'acompanhamento.acompanhamento_pnc', 'pk' => 'acpid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'hisid' => null,
        'acpid' => null,
        'usucpf' => null,
        'hisdata' => null,
    );

    public function recuperarHistorico($acpid)
    {
        $sql = "
            select
                his.*,
                usu.usunome,
                to_char(hisdata, 'DD/MM/YYYY HH24:MI:SS') hisdata
            from acompanhamento.historico his
                join seguranca.usuario usu on usu.usucpf = his.usucpf
            where
                his.acpid = " . (int)$acpid . "
            order by
                his.hisdata desc
        ";

        return $this->carregar($sql);
    }

}//end Class
?>
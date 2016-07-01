<?php

/**
 * Class Ted_Model_Emenda
 */
class Ted_Model_Emenda extends Modelo
{
    /**
     * Nome da Tabela
     * @var String
     */
    protected $stNomeTabela = 'emenda.emenda';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('emeid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'emeid' => NULL,
        'emecod' => NULL,
        'autid' => NULL,
        'acaid' => NULL,
        'resid' => NULL,
        'emetipo' => NULL,
        'emeano' => NULL,
        'emedescentraliza' => NULL,
        'emelibera' => NULL,
        'emevalor' => NULL,
        'etoid' => NULL
    );

    /**
     * @var mixed
     */
    protected $_tcpid;

    /**
     * Construtor da classe
     */
    public function __construct()
    {
        $this->_tcpid = Ted_Utils_Model::capturaTcpid();
        if (is_null($this->_tcpid)) {
            throw new Exception("Nenhum Termo encontrado.");
        }
    }

    public static function getPartialTed()
    {
        $strSQL = "

        ";

        Ted_Utils_Model::dbGetInstance()->carregar($strSQL);
    }

    /**
     * Insere um novo vinculo entre a emenda e o ted
     * @param array $data
     */
    public function update(array $data)
    {
        extract($data);
        #ver($data,d);
        if (isset($data['emeid'])&&$data['emeid']!='') {
            $strSQL = "
                UPDATE ted.emendas
                SET tcpid = {$data['tcpid']}
                WHERE emeid = '{$data['emeid']}'
            ";
            $this->executar($strSQL);
            $this->commit();
        }
    }

    /**
     * Busca os dados da emenda no sistema de Emendas
     * @return array|void
     */
    public function get()
    {
        $strSQL = "
            SELECT DISTINCT
                e.emeid,
                e.emecod,
                a.autnome,
                vf.fupfuncionalprogramatica,
                vf.fupdsc,
                ed.gndcod,
                ed.mapcod,
                ed.foncod,
                ede.edeid,
                ede.edevalor as valorentidade,
                ed.emdid
            FROM
                emenda.emenda e
            INNER JOIN
                emenda.emendadetalhe ed ON (ed.emeid = e.emeid)
            INNER JOIN emenda.autor a  ON (a.autid = e.autid)
            INNER JOIN emenda.v_funcionalprogramatica vf ON (vf.acaid = e.acaid AND vf.acastatus = 'A')
            INNER JOIN ted.emendas tem ON (tem.emecod::text = e.emecod::text  AND tem.emeano = e.emeano::text)
            INNER JOIN emenda.emendadetalheentidade ede ON (ede.emdid = ed.emdid AND edestatus = 'A')
            WHERE
                tem.emeano = '{$_SESSION['exercicio']}'
            AND tem.tcpid = {$this->_tcpid}
            AND vf.unicod = '".UO_MEC."'
            AND ed.emdid IN (SELECT emdid FROM emenda.emendadetalheentidade)
        ";
        //ver($strSQL, d);
        $results = $this->carregar($strSQL);
        return ($results) ? $results : array();
    }

    /**
     * Monta grid de dados com os valores das emendas
     * @return String
     */
    public function getGrid()
    {
        /**
         * Componente para listagens.
         * @see Simec_Listagem
         */
        require_once APPRAIZ.'includes/library/simec/Listagem.php';

        $colunms = array(
            'código',
            'Autor',
            'Funcional Programática',
            'Subtítulo',
            'GND',
            'Mod',
            'Fonte',
            'Valor (R$)'
        );

        $list = new Simec_Listagem();
        $list->setCabecalho($colunms)
            //->addAcao('edit', 'preview')
            ->addAcao('edit', array('func' => 'preview', 'extra-params' => array('emdid', 'edeid', 'valorentidade')))
            ->setDados($this->get())
            ->esconderColunas(array('emdid', 'edeid'))
            ->addCallbackDeCampo('valorentidade', 'mascaraMoeda')
            ->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
}
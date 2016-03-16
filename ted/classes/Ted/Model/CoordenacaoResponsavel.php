<?php

/**
 * Class Ted_Model_CoordenacaoResponsavel
 */
class Ted_Model_CoordenacaoResponsavel extends Modelo
{
    /**
     * Nome da Tabela
     * @var String
     */
    protected $stNomeTabela = 'ted.coordenacao_responsavel';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('corid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'corid' => NULL,
        'tcpid' => NULL,
        'ungcod' => NULL,
        'nomecoordenacao' => NULL,
        'dddcoordenacao' => NULL,
        'telefonecoordenacao' => NULL,
        'datainsert' => 'NOW()',
    );

    /**
     * @var mixed
     */
    protected $_tcpid;

    /**
     * Construtor da classe
     */
    public function __construct($tcpid = null)
    {
        $this->_tcpid = ($tcpid) ? $tcpid : Ted_Utils_Model::capturaTcpid();
        if (is_null($this->_tcpid)) {
            throw new Exception("Nenhum Termo encontrado.");
        }
    }

    /**
     * @param array $dados
     */
    public function save(array $dados)
    {
        if (!$this->isValid($dados)) {
            return false;
        }

        //ver($dados,d );
        $this->popularDadosObjeto($dados);
        if (empty($dados['corid'])) {
            $this->inserir();
        } else {
            $this->alterar();
        }

        return ($this->commit()) ? true : false;
    }

    /**
     * @param $ungcod
     * @param $tcpid
     * @return array|bool|string|void
     */
    public function getByUngcod($ungcod, $tcpid)
    {
        $strSQL = "
            SELECT
                cr.corid,
                cr.nomecoordenacao,
                cr.dddcoordenacao,
                cr.telefonecoordenacao
            FROM
                {$this->stNomeTabela} cr
            WHERE cr.ungcod = '{$ungcod}' AND cr.tcpid = {$tcpid}
        ";

        //ver($strSQL, d);
        $results = $this->pegaLinha($strSQL);
        return ($results) ? $results : false;
    }

    /**
     * @param $ungcod
     * @return array|bool|null|void
     */
    public function get($ungcod)
    {
        $strSQL = "
            SELECT * FROM
                {$this->stNomeTabela} cr
            WHERE cr.ungcod = '{$ungcod}' AND cr.tcpid = {$this->_tcpid}
        ";

        //ver($strSQL, d);
        $results = $this->pegaLinha($strSQL);
        return ($results) ? $results : null;
    }

    /**
     * @param array $data
     * @return bool
     */
    public function isValid(array $data)
    {
        $array_keys = array('nomecoordenacao', 'dddcoordenacao', 'telefonecoordenacao');
        foreach ($data as $k => $value) {
            foreach ($array_keys as $i => $v) {
                if (($array_keys[$i] == $k) && empty($value))
                    return false;
            }
        }

        return true;
    }
}

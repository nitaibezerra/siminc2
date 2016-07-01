<?php

/**
 * Class Ted_Model_Parecer
 */
class Ted_Model_ParecerJuridico extends Modelo
{
    /**
     * Nome da Tabela
     * @var String
     */
    protected $stNomeTabela = 'ted.parecerjuridico';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('pcjid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pcjid'       => NULL,
        'tcpid'       => NULL,
        'ungcod'      => NULL,
        'obsparecer'  => NULL,
        'usucpf' 	  => NULL,
        'pjdata'      => NULL
    );

    public function __construct($tcpid = null)
    {
        $this->arAtributos['tcpid'] = ($tcpid) ? $tcpid : Ted_Utils_Model::capturaTcpid();
        if (is_null($this->arAtributos['tcpid'])) {
            throw new Exception("Nenhum Termo encontrado.");
        }
    }

    /**
     * Campos Obrigatórios da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arAtributosObrigatorios = array(
        'tcpid', 'ungcod', 'obsparecer'
    );

    /**
     * Valida campos obrigatorios no objeto populado
     *
     * @author Sávio Resende - Copiador por Lindalberto Filho
     * @return bool
     */
    public function validaCamposObrigatorios()
    {
        foreach ($this->arAtributosObrigatorios as $chave => $valor)
            if (!isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]))
                return false;

        return true;
    }

    /**
     * Cadastrar Parecer Juridico para um termo
     *
     * @return bool  - retorna 'false' caso existam campos obrigatorios vazios
     * @author Sávio Resende
     */
    function cadastrar()
    {
        if ($this->validaCamposObrigatorios()) {
            $this->arAtributos['ptecid'] = $this->inserir();
            return $this->commit();
        }

        return false;
    }

    /**
     * Atualizar Parecer Juridico para um termo
     *
     * @return bool  - retorna 'false' caso existam campos obrigatorios vazios
     * @author Sávio Resende
     */
    public function atualizar()
    {
        if ($this->validaCamposObrigatorios()) {
            $this->alterar();
            return $this->commit();
        }
        return false;
    }

    /**
     * Captura dados para utilização na Aba de Parecer Juridico.
     * @return Ambigous <boolean, multitype:>
     */
    public function capturaDados($ungcod)
    {
        $strSQL ="
            SELECT
                p.pcjid,
                p.tcpid,
                p.ungcod,
                p.obsparecer,
                p.usucpf ||' - '|| seg.usunome as usunome,
                p.pjdata
            FROM
                {$this->stNomeTabela} p
            LEFT JOIN seguranca.usuario seg on(seg.usucpf = p.usucpf)
            WHERE
                p.tcpid = {$this->arAtributos['tcpid']}
                AND p.ungcod = '{$ungcod}'
	    ";

        $result = $this->pegaLinha($strSQL);
        return ($result != null) ? $result : null;
    }
}
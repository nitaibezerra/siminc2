<?php

/**
 * Class Ted_Model_Historico
 */
class Ted_Model_Historico extends Modelo
{
    /**
     * @var
     */
    protected $_tcpid;

    /**
     * @var
     */
    protected $_version;

    /**
     *
     */
    const PROPONENTE = 'proponente';

    /**
     *
     */
    const CONCEDENTE = 'concedente';

    /**
     * @var
     */
    protected $_historico = array();

    /**
     *
     */
    public function __construct($version = null)
    {
        $this->_tcpid = Ted_Utils_Model::capturaTcpid();
        if (is_null($this->_tcpid)) {
            throw new Exception("Nenhum termo foi setado para a operação de histórico.");
        }

        if (null !== $version) {
            $this->setVersion($version);
        }
    }

    /**
     * @param $version
     * @return $this
     */
    public function setVersion($version)
    {
        if ($version) {
            $this->_version = $version;
        }

        return $this;
    }

    public function getVersion()
    {
        if (!empty($this->_version)) {
            return $this->_version;
        } else {
            return false;
        }
    }

    /**
     * @param $version
     */
    public function get()
    {
        $strSQL = "
            select * from
            ted.historico_termocompromisso
            where tcpid = {$this->_tcpid} and tcpversion = {$this->_version}
        ";

        $this->_historico['ted'] = $this->pegaLinha($strSQL);
        $this->getUnidadeGestoraProponente();
        $this->getUnidadeGestoraConcedente();
        $this->getJustificativa();
        $this->getPrevisaoOcamentaria();
        $this->getParecerTecnico();
        $this->getAnexosTermo();
        //ver($this->_historico);
        return $this->_historico;
    }

    /**
     * Pega os dados completos da Unidade Proponente
     */
    public function getUnidadeGestoraProponente()
    {
        $ug = new Ted_Model_UnidadeGestora();
        $this->_historico[self::PROPONENTE] = $ug->pegaUnidade($this->_historico['ted']['ungcodproponente']);
        $this->_pegarCoordenacao($this->_historico['ted']['ungcodproponente'], self::PROPONENTE);
        $this->_PegarRL($this->_historico['ted']['ungcodproponente'], self::PROPONENTE);
    }

    /**
     * Pega os dados completos da Unidade Concedente
     */
    public function getUnidadeGestoraConcedente()
    {
        $ug = new Ted_Model_UnidadeGestora();
        $this->_historico[self::CONCEDENTE] = $ug->pegaUnidade($this->_historico['ted']['ungcodconcedente']);
        $this->_pegarCoordenacao($this->_historico['ted']['ungcodconcedente'], self::CONCEDENTE);
        $this->_PegarRL($this->_historico['ted']['ungcodconcedente'], self::CONCEDENTE);
    }

    /**
     * Pega os dados da coordenação  da unidade informada
     * @param $ungcod
     * @param $context
     */
    public function _pegarCoordenacao($ungcod, $context)
    {
        $strSQL = "
            select * from ted.historico_coordenacao_responsavel
            where tcpversion = {$this->_version} and tcpid = {$this->_tcpid}
            and ungcod = '%s'
        ";

        $stmt = sprintf($strSQL, $ungcod);
        $this->_historico[$context]['coordenacao'] = $this->pegaLinha($stmt);
    }

    /**
     * Pega os dados do Representante Legal da Unidade Informada
     * @param $ungcod
     * @param $context
     */
    public function _PegarRL($ungcod, $context)
    {
        $strSQL = "
            select * from ted.historico_representantelegal
            where tcpversion = {$this->_version} and tcpid = {$this->_tcpid}
            and ug = '{$ungcod}' and substituto = '%s'
        ";

        $this->_historico[$context]['rlp'] = $this->pegaLinha(sprintf($strSQL, 'f'));
        $this->_historico[$context]['rlps'] = $this->pegaLinha(sprintf($strSQL, 't'));
    }

    /**
     * Pega a Justificativa do Termo
     */
    public function getJustificativa()
    {
        $strSQL = "
            select * from ted.historico_justificativa
            where tcpversion = {$this->_version} and tcpid = {$this->_tcpid}
        ";

        $this->_historico['justificativa'] = $this->pegaLinha($strSQL);
    }

    /**
     * Pega toda previsao orçamentária do historico
     */
    public function getPrevisaoOcamentaria()
    {
        $strSQL = "
            select
                t.proanoreferencia,
                mp.acacod AS acao,
                mp.ptres || ' - ' || mp.funcod||'.'||mp.sfucod||'.'||mp.prgcod||'.'||mp.acacod||'.'||mp.unicod||'.'||mp.loccod AS programatrabalho,
                plicod || ' - ' || plidsc AS planointerno,
                a.acadsc,
                ndpcod || ' - ' || ndpdsc AS naturezadespesa,
                --t.provalor,
                case
                    when (select sum(cr.valor) from ted.historico_creditoremanejado cr where cr.proid = t.proid and t.tcpversion = {$this->_version} and tcpid = {$this->_tcpid}) IS NOT NULL then
                        to_char(t.provalor - (select sum(cr.valor) from ted.historico_creditoremanejado cr where cr.proid = t.proid and t.tcpversion = {$this->_version} and t.tcpid = {$this->_tcpid}), '999G999G999G999G999D99')
                    else
                        to_char(t.provalor, '999G999G999G999G999D99')
                    end as provalor,
                case
                    when (select sum(cr.valor) from ted.historico_creditoremanejado cr where cr.proid = t.proid and t.tcpversion = {$this->_version} and tcpid = {$this->_tcpid}) IS NOT NULL then
                        coalesce(t.provalor - (select sum(cr.valor) from ted.historico_creditoremanejado cr where cr.proid = t.proid and t.tcpversion = {$this->_version} and t.tcpid = {$this->_tcpid}), 0)
                    else
                        coalesce(t.provalor, 0)
                end as valor,
                t.crdmesliberacao, t.crdmesexecucao
            from ted.historico_previsaoorcamentaria t
            join monitora.ptres mp on (mp.ptrid = t.ptrid)
            join monitora.pi_planointerno pi on (pi.pliid = t.pliid)
            join naturezadespesa nd on (nd.ndpid = t.ndpid)
            join monitora.acao a on (a.acaid = mp.acaid)
            where t.tcpversion = {$this->_version} and t.tcpid = {$this->_tcpid}
        ";

        $this->_historico['previsao'] = $this->carregar($strSQL);
    }

    /**
     * Pega todo parecer tecnico
     */
    public function getParecerTecnico()
    {
        $strSQL = "
            select * from ted.historico_parecertecnico
            where tcpversion = {$this->_version} and tcpid = {$this->_tcpid}
        ";

        $this->_historico['parecer'] = $this->pegaLinha($strSQL);
    }

    public function getAnexosTermo()
    {
        $strSQL = "
            select * from ted.historico_arquivoprevorcamentaria
            where tcpversion = {$this->_version} and tcpid = {$this->_tcpid}
        ";

        $this->_historico['anexos'] = $this->carregar($strSQL);
    }

    /**
     * @return array
     */
    public function getComboHistorico()
    {
        $strSQL = sprintf("
            select htcpid, tcpversion from ted.historico_termocompromisso where tcpid = %d
        ", $this->_tcpid);
        $results = $this->carregar($strSQL);
        $tmpArr = array();

        if (is_array($results)) {
            foreach ($results as $row) {
                $tmpValue = ($row['tcpversion']-1);
                $tmpArr[$row['tcpversion']] = $tmpValue;
            }
        }

        return $tmpArr;
    }

}
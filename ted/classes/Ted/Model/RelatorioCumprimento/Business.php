<?php

class Ted_Model_RelatorioCumprimento_Business extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'ted.termocompromisso';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('tcpid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'docid' => null,
        'ungcodproponente' => null,
        'ungcodconcedente' => null,
        'ungcodgestaorecebedora' => null,
        'rugid' => null,
        'pliid' => null,
        'unridproponente' => null,
        'unridconcedente' => null,
        'tcobjptxtrelacao' => null,
        'dircod' => null,
        'usucpfconcedente' => null,
        'usucpfproponente' => null,
        'cooid' => null,
        'tcpstatusanalise' => null,
        'entid' => null,
        'tcpobsrelatorio' => null,
        'ungcodpoliticafnde' => null,
        'dircodpoliticafnde' => null,
        'tcpnumtransfsiafi' => null,
        'tcpidentificadorsigef' => null,
        'tcpnumprocessofnde' => null,
        'tcpprogramafnde' => null,
        'tcpobsfnde' => null,
        'uniid' => null,
        'ungcodemitente' => null,
        'gescodemitente' => null,
        'codsigefnc' => null,
        'tcpstatus' => null,
        'tcpobscomplemento' => null,
        'tcpusucpfparecer' => null,
        'tcpbancofnde' => null,
        'tcpagenciafnde' => null,
        'tcptipoemenda' => null,
        'retornosigefnc' => null,
    );

    /**
     * @var
     */
    private $_termosComuns;

    /**
     * @var
     */
    private $_ted;

    /**
     * @var
     */
    private $_tcpid;

    /**
     * @var array
     */
    private $_pendencias = array();

    /**
     * Data da publicação 2015-01-06
     * Portaria SE/MEC nº 1.529/2014.
     */
    const DATA_PORTARIA_2015 = '2015-01-05';

    /**
     *
     */
    public function __construct($tcpid = null)
    {
        $this->_tcpid = (Ted_Utils_Model::capturaTcpid() == false) ? $tcpid : Ted_Utils_Model::capturaTcpid();
        if (!$this->_tcpid) {
            throw new Exception('Valor do tpcid precisa estar declarado');
        }

        $this->_carregaTed();
        $this->_carregaTermosEmComum();

        //Debug mode on
        //$this->_carregaTermosVencidos();
        //$this->_debugDump();
    }

    /**
     * Debug true
     * @return void
     */
    public function _debugDump()
    {
        //ver($this->_termosComuns);
        ver($this->_pendencias);
        //ver($this->_ted);
    }

    /**
     * Metodo que buscas as os dados base para a estrutura da classe
     * @param bool $where
     * @return array|null|void
     */
    protected function _buildQuery($where = null, $tcpid = null)
    {
        if (null !== $tcpid) $this->_tcpid = $tcpid;

        $strSQL = "
            SELECT * FROM
            (select distinct
                t.tcpid,
                t.ungcodproponente,
                t.ungcodconcedente,
                t.docid,
                to_char((select htddata from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = t.docid order by hstid asc limit 1), 'YYYY-MM-DD') as data_execucao,
                case
                when (select count(*) from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = t.docid) = 1 then
                    (select crdmesexecucao from ted.previsaoorcamentaria where tcpid = t.tcpid and prostatus = 'A' and crdmesexecucao is not null order by crdmesexecucao asc limit 1)
                when (select count(*) from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = t.docid) > 1 then
                    (select crdmesexecucao from ted.previsaoorcamentaria where tcpid = t.tcpid and prostatus = 'A' and crdmesexecucao is not null order by crdmesexecucao desc limit 1)
                else
                    null
                end AS vigencia,
                (select TO_CHAR(htddata, 'YYYY-MM-DD') from workflow.historicodocumento
                    where docid = (
                      select docid from ted.termocompromisso where tcpid = 2018
                    ) and aedid in (2476, 2442, 1612) order by htddata desc limit 1) as assinatura,
                (select count(*) from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = t.docid) as qtd_execucao
                from ted.termocompromisso t
                left join unidadegestora g on g.ungcod = t.ungcodconcedente
                where
                t.ungcodproponente = (select ungcodproponente from ted.termocompromisso where tcpid = {$this->_tcpid})
                and
                t.ungcodconcedente = (select ungcodconcedente from ted.termocompromisso where tcpid = {$this->_tcpid})
            ) AS vTable
        %s
        ";

        if (null === $where) {
            $where = 'where vTable.qtd_execucao > 0';
        }

        $stmt = sprintf($strSQL, $where);
        //ver($stmt);
        $rs = $this->carregar($stmt);
        return ($rs) ? $rs : null;
    }

    /**
     * Pega as informações sobre o termo que esta sendo acessado
     * @return void
     */
    protected function _carregaTed()
    {
        $where = sprintf('WHERE vTable.tcpid = %d', $this->_tcpid);
        $this->_ted = $this->_buildQuery($where);
    }

    /**
     * Pega todos os termos que comum para o termo acessado
     * mesmo proponente e mesmo concedente
     * @return void
     */
    protected function _carregaTermosEmComum()
    {
        //$where = $this->_tcpid, 'WHERE vTable.tcpid <> {$this->_tcpid}';
        $this->_termosComuns = $this->_buildQuery();
    }

    /**
     * Verifica se o relatorio de cumprimento do objeto
     * foi preehcindo
     * @param $tcpid
     * @return bool
     */
    protected function _isRelatorioPreenchido($tcpid)
    {
        $stmt = sprintf("SELECT * FROM ted.relatoriocumprimento WHERE recstatus = '%s' AND tcpid = %d", 'A', $tcpid);
        $result = $this->pegaLinha($stmt);
        return ($result) ? true : false;
    }

    /**
     * Dentre os termos encontrado,
     * filtra os que existe pendencia de preenchido com prazo vencido
     * @return void
     */
    public function _carregaTermosVencidos()
    {
        if (is_array($this->_termosComuns)) {
            foreach ($this->_termosComuns as &$resultado) {

                //echo $resultado['tcpid'] . ' - ' . !$this->_isRelatorioPreenchido($resultado['tcpid']) . ' - ' . !empty($resultado['data_execucao']) .  '<br>';

                if (!$this->_isRelatorioPreenchido($resultado['tcpid']) && !empty($resultado['data_execucao'])) {

                    //$resultado['prazo_extra'] = 2; //60 dias = dois meses
                    $resultado['expira'] = $resultado['vigencia'];

                    $data = new DateTime($resultado['data_execucao']);
                    $data->modify("+{$resultado['expira']} month");
                    $resultado['data_expira'] = $data->format('Y-m-d');
                    //ver($resultado);

                    $dateNow = new DateTime();
                    //var_dump($data < $dateNow);
                    if (($data < $dateNow) && ($resultado['data_execucao'] > $resultado['assinatura'])) {
                        $this->_pendencias[] = $resultado;
                    }
                }
            }
        }
    }

    /**
     * Se houver termos com pendencia
     * monta o html de output
     * @return null|string
     */
    public function mostraPendencias()
    {
        $this->_carregaTermosVencidos();

        if ($this->_pendencias) {
            $html = '<table class="table table-bordered table-striped table-hover">';
            $html = '<tr>';
            $html.= '<td>
                        <div class="alert alert-danger text-center" role="alert">
                            <font size="3">
                                O proponente possui Relatório(s) de Prestação de Contas do Objeto com prazo de apresentação ao Concedente expirado, <br />
                                impedindo nova descentralização, conforme disposto pela Portaria SE/MEC nº 1.529/2014.<br />
                                Termos de Execução Descentralizada com pendência na apresentação do Relatório de Prestação de Contas do Objeto:
	                        </font>
	                    </div>
	                </td>';
            $html.= '</tr>';
            $html.= '<tr>';
            $html.= '<td>
                    <table class="table table-bordered table-striped table-hover">';
            $html.= '<tr>
                        <td align="center">
                        <strong>Termo de Compromisso</strong>
                        </td>';
            $html.= '<td align="center">
                        <strong>Prazo para preenchimento do Relatório de Prestação de Contas do Objeto</strong>
                     </td>
                     </tr>';
            foreach ($this->_pendencias as $result) {
                $html.= '<tr>';
                $html.= '<td align="center">'.$result['tcpid'].'</td>';
                $html.= '<td align="center">'.$result['data_expira'].'</td>';
                $html.= '</tr>';
            }
            $html.= '</td></table>';
            $html.= '</tr>';
            $html.= '</table>';
        }

        return ($html) ? $html : null;
    }

    /**
     * Verifica de forma avulsa, se existe pendencia para o termo
     * @param $tcpid
     * @return bool
     */
    public function termoVencido($tcpid)
    {
        $where = 'WHERE vTable.vigencia IS NOT NULL And vTable.tcpid = '.$tcpid;
        $row = $this->_buildQuery($where, $tcpid);
        $row = (is_array($row)) ? current($row) : array();
        $estadoAtual = Ted_Utils_Model::pegaSituacaoTed();

        if ($row && (!$this->_isRelatorioPreenchido($tcpid)
            || !$this->isRelatorioEmcaminhado($tcpid) || $estadoAtual['esdid'] == TERMO_EM_DILIGENCIA_RELATORIO)
            && !empty($row['data_execucao'])) {

            //$row['prazo_extra'] = 2; //60 dias = dois meses
            $row['expira'] = $row['vigencia']; //+ $row['prazo_extra'];

            $data = new DateTime($row['data_execucao']);
            $data->modify("+{$row['expira']} month");
            $row['data_expira'] = $data->format('Y-m-d');
            //ver($row, d);

            $dateNow = new DateTime();
            if (($data < $dateNow) && ($resultado['data_execucao'] > $resultado['assinatura']))
                return $row['tcpid'];
            else
                return false;
        }
    }

    /**
     * Verifica se o relatório de cumprimento do objeto
     * já foi enviado para analise do gestor
     * @param $tcpid
     * @return array|bool|void
     */
    private function isRelatorioEmcaminhado($tcpid)
    {
        $strSQL = "
            select * from workflow.historicodocumento where aedid in (
                SELECT
                    ae.aedid
                FROM workflow.acaoestadodoc ae
                    INNER JOIN workflow.estadodocumento ed ON (ed.esdid = ae.esdidorigem)
                    INNER JOIN workflow.estadodocumentoperfil dp ON (dp.aedid = ae.aedid)
                where
                    ed.tpdid = 97 and aedstatus = 'A'
                    and aeddscrealizar ilike '%Encaminhar o relatório de cumprimento do objeto%'
                order by ae.esdidorigem
            )
            and docid = (select docid from ted.termocompromisso where tcpid = {$tcpid})
            order by htddata desc
        ";

        $resultado = $this->carregar($strSQL);
        return ($resultado) ? $resultado : false;
    }

    public function getPendenciaTermoRelacionado()
    {
        return ($this->_pendencias)? true : false;
    }

    /**
     * Pega RCO com prazo de aprovação pela Coordenação vencido
     * prazo de 60 dias para aprovação, apos tramitação da equipe tecnica
     * @return array|bool|null|void
     */
    public function getAlertaPrazoAprovacaoCoordenacao()
    {
        $strSQL = "
            SELECT * FROM (
                SELECT
                    CASE WHEN DATE_PART('days', NOW() - hd.htddata) < 60 AND hd.htddata > '".self::DATA_PORTARIA_2015."' THEN
                        'Faltam menos de 60 dias para o prazo de aprovação da Coordenação expirar'
                    WHEN DATE_PART('days', NOW() - hd.htddata) < 30 AND hd.htddata > '".self::DATA_PORTARIA_2015."' THEN
                        'Faltam menos de 30 dias para o prazo de aprovação da Coordenação expirar'
                    WHEN DATE_PART('days', NOW() - hd.htddata) < 15 AND hd.htddata > '".self::DATA_PORTARIA_2015."' THEN
                        'Faltam menos de 15 dias para o prazo de aprovação da Coordenação expirar'
                    WHEN DATE_PART('days', NOW() - hd.htddata) < 5 AND hd.htddata > '".self::DATA_PORTARIA_2015."' THEN
                        'Faltam menos de 5 dias para o prazo de aprovação da Coordenação expirar'
                    WHEN DATE_PART('days', NOW() - hd.htddata) > 60 AND hd.htddata > '".self::DATA_PORTARIA_2015."' THEN
                        'O prazo de aprovação pela Coordenação expirou'
                    ELSE
                        ''
                    END AS mensagem,
                    CASE WHEN hd.htddata > '".self::DATA_PORTARIA_2015."' THEN
                        TO_CHAR(hd.htddata + INTERVAL '60' day, 'DD/MM/YYYY')
                    ELSE
                        ''
                    END AS deadline
                FROM
                    workflow.historicodocumento hd
                WHERE
                    docid = (SELECT docid FROM ted.termocompromisso WHERE tcpid = {$this->_tcpid})
                    AND aedid = 1652
                ORDER BY hstid DESC
            ) AS vTable
            WHERE vTable.mensagem <> '' AND vTable.deadline <> ''
        ";

        //ver($strSQL, d);
        $linha = $this->pegaLinha($strSQL);
        if (is_array($linha)) {
            $html = "
                <table class='table table-bordered table-striped table-hover'>
                    <tr>
                        <th>Alertas - Prestação de Contas do Objeto</th>
                    </tr>
                    <tr>
                        <td><div class='alert alert-danger text-center' role='alert'>{$linha['mensagem']}</div></td>
                    </tr>
                    <tr>
                        <td><strong>Prazo final: <span style='font-size: 13px;'>{$linha['deadline']}</span></strong></td>
                    </tr>
                </table>
            ";
        } else {
            return false;
        }

        return $html;
    }

    /**
     * Verifica se existe RCO pendente de analise por parte da Coordenação
     * @return bool|string
     */
    public function termosPendenciaAprovacaoCoordenacao()
    {
        $gestor = $this->pegaLinha("
            select
                ungcodproponente as proponente,
                ungcodconcedente as concedente,
                ungcodpoliticafnde,
                dircodpoliticafnde
            from monitora.termocooperacao
            where tcpid = {$this->_tcpid}
        ");

        if (!is_array($gestor))
            return false;

        if ($gestor['concedente'] == UG_FNDE) {

            $secretarias = array(
                Ted_Model_Responsabilidade::SECADI,
                Ted_Model_Responsabilidade::SETEC,
                Ted_Model_Responsabilidade::SEB
            );

            if (in_array($gestor['ungcodpoliticafnde'], $secretarias)) {
                $concedente = $gestor['ungcodpoliticafnde'];
            } else {
                $concedente = $gestor['concedente'];
            }
        }

        $strSQL = "
            select * from (
                select
                    (select tcpid from ted.termocompromisso where docid = hd.docid) as tcpid,
                    CASE WHEN DATE_PART('days', NOW() - hd.htddata) > 60 AND hd.htddata > '".self::DATA_PORTARIA_2015."' THEN
                        true
                    else
                        false
                    END as prazo_expirado,
                    to_char(hd.htddata + interval '60' day, 'DD/MM/YYYY') as deadline,
                    (select ungcodproponente from ted.termocompromisso where docid = hd.docid) as proponente,
                    (select ungcodconcedente from ted.termocompromisso where docid = hd.docid) as concedente,
                    (select esdid from workflow.documento where docid = hd.docid) as situacao
                from
                    workflow.historicodocumento hd
                where
                    aedid = ".AEDID_EM_ANALISE_PELA_COORDENACAO."
                order by hstid desc
            ) vTable
            where
                vTable.prazo_expirado = 't'
                AND vTable.proponente = '{$gestor['proponente']}'
                AND vTable.concedente = '{$concedente}'
                AND vTable.situacao NOT IN (".TERMO_FINALIZADO.", ".TERMO_ARQUIVADO.")
        ";

        //ver($strSQL);
        $collection = $this->carregar($strSQL);
        $html = '';
        if (is_array($collection)) {
            $html.= '<table class="table table-bordered table-striped table-hover">';
            $html.= '<tr>';
            $html.= '<th colspan="2">Prestação de Contas do Objeto <br />
                     com pendência de aprovação pela Coordenação</th>';
            $html.= '</tr>';
            $html.= '<tr>';
            $html.= "<td class='text-center' width='50%'><strong>Termo de Execução</strong></td>";
            $html.= "<td class='text-center' width='50%'><strong>Data Limite</strong></td>";
            $html.= '<tr>';
            foreach ($collection as $row) {
                $html.= '<tr>';
                $html.= "<td class='text-center'>{$row['tcpid']}</td>";
                $html.= "<td class='text-center'>{$row['deadline']}</td>";
                $html.= '</tr>';
            }
            $html.= '</table>';
        } else {
            return false;
        }

        return $html;
    }

}
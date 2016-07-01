<?php

class RelatorioCumprimentoObjeto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'monitora.termocooperacao';

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
        'tcpdscobjetoidentificacao' => null,
        'tcpobjetivoobjeto' => null,
        'tcpobjetojustificativa' => null,
        'tcobjptxtrelacao' => null,
        'dircod' => null,
        'usucpfconcedente' => null,
        'usucpfproponente' => null,
        'tcpparecertecnico' => null,
        'tcpconsidentproponente' => null,
        'tcpconsidproposta' => null,
        'tcpconsidobjeto' => null,
        'tcpconsidobjetivo' => null,
        'tcpconsidjustificativa' => null,
        'tcpconsidvalores' => null,
        'tcpconsidcabiveis' => null,
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
    public function __construct()
    {
        $this->_tcpid = (int) $_SESSION['elabrev']['tcpid'];
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
    public function _buildQuery($where = null, $tcpid = null)
    {
        if (null !== $tcpid) $this->_tcpid = $tcpid;

        $strSQL = "
            SELECT * FROM
            (select distinct
                t.tcpid,
                t.ungcodproponente,
                t.ungcodconcedente,
                t.docid,
                to_char((select htddata from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = t.docid order by hstid desc limit 1), 'DD-MM-YYYY') as data_execucao,
                case
                when (select count(*) from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = t.docid) = 1 then
                    (select crdmesexecucao from monitora.previsaoorcamentaria where tcpid = t.tcpid and prostatus = 'A' and crdmesexecucao is not null order by crdmesexecucao asc limit 1)
                when (select count(*) from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = t.docid) > 1 then
                    (select crdmesexecucao from monitora.previsaoorcamentaria where tcpid = t.tcpid and prostatus = 'A' and crdmesexecucao is not null order by crdmesexecucao desc limit 1)
                else
                    null
                end AS vigencia,
                (select count(*) from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = t.docid) as qtd_execucao
                from monitora.termocooperacao t
                left join unidadegestora g on g.ungcod = t.ungcodconcedente
                where
                t.ungcodproponente = (select ungcodproponente from monitora.termocooperacao where tcpid = {$this->_tcpid})
                and
                t.ungcodconcedente = (select ungcodconcedente from monitora.termocooperacao where tcpid = {$this->_tcpid})
            ) AS vTable
        %s
        ";

        if (null === $where) {
            $where = 'where vTable.qtd_execucao > 0';
        }

        $stmt = sprintf($strSQL, $where);
        //ver($stmt, d);
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
        $stmt = sprintf("SELECT * FROM elabrev.relatoriocumprimento WHERE recstatus = '%s' AND tcpid = %d", 'A', $tcpid);
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

                if (!$this->_isRelatorioPreenchido($resultado['tcpid'])
                    && !empty($resultado['data_execucao'])
                    && !empty($resultado['vigencia'])
                ) {

                    $resultado['prazo_extra'] = 2; //60 dias = dois meses
                    $resultado['expira'] = $resultado['vigencia'] + $resultado['prazo_extra'];

                    $data = new DateTime($resultado['data_execucao']);
                    $data->modify("+{$resultado['expira']} month");
                    $resultado['data_expira'] = $data->format('d-m-Y');

                    $dateNow = new DateTime();
                    if ($data < $dateNow) {
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
            $html = '<tr>';
            $html.= '<td>
                        <center style="color:#B00707;">
                            <font size="2">
                                O proponente possui Relatório(s) de Cumprimento do Objeto com prazo de apresentação ao Concedente expirado, <br />
                                impedindo nova descentralização, conforme disposto pela Portaria SE/MEC nº 1.529/2014.
                                Termos de Execução Descentralizada com pendência na apresentação do Relatório de Cumprimento do Objeto:
	                        </font>
	                    </center>
	                </td>';
            $html.= '</tr>';
            $html.= '<tr>';
            $html.= '<td>
                    <table style="color: #B00707;border-collapse: collapse;" border="1" width="40%" align="center">';
            $html.= '<tr>
                        <td align="center">
                        <strong>Termo de Compromisso</strong>
                        </td>';
            $html.= '<td align="center">
                        <strong>Prazo para preenchimento do Relatório de Cumprimento do Objeto</strong>
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
        $row = $this->_buildQuery(null, $tcpid);

        $row = (is_array($row)) ? $row : array();
        //$estadoAtual = _pegaEstadoAtual($tcpid);termoVencido(

        /*&& (!$this->_isRelatorioPreenchido($tcpid)
        || !$this->isRelatorioEmcaminhado($tcpid)
        || $estadoAtual == TERMO_EM_DILIGENCIA_RELATORIO)*/
        //ver($row);

        if ($row && !empty($row['data_execucao'])) {

            $row['prazo_extra'] = 2; //60 dias = dois meses
            $row['expira'] = $row['vigencia']; // + $row['prazo_extra'];

            $data = new DateTime($row['data_execucao']);
            $data->modify("+{$row['expira']} month");
            $row['data_expira'] = $data->format('d-m-Y');
            //ver($row, d);

            $dateNow = new DateTime();
            if ($data < $dateNow)
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
            and docid = (select docid from monitora.termocooperacao where tcpid = {$tcpid})
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
            select TO_CHAR(htddata, 'YYYY-MM-DD') as dt_execucao from workflow.historicodocumento
            where docid = (select docid from monitora.termocooperacao where tcpid = {$this->_tcpid})
            and aedid = 1618 order by hstid desc limit 1
        ";
        $dataExecucao = $this->pegaUm($strSQL);

        $strSQL = "
            select
                crdmesliberacao, crdmesexecucao, proanoreferencia
            from monitora.previsaoorcamentaria
            where tcpid = {$this->_tcpid} and prostatus = 'A'
            order by crdmesliberacao desc, crdmesexecucao desc, proanoreferencia desc
            limit 1
        ";
        $dadosPrevisao = $this->pegaLinha($strSQL);

        $strSQL = "
            select tcpobsrelatorio from monitora.termocooperacao where tcpid = {$this->_tcpid};
        ";
        $rcoParecer = $this->pegaLinha($strSQL);

        //ver($dataExecucao, $dadosPrevisao, d);
        if ($dataExecucao && is_array($dadosPrevisao) && empty($rcoParecer['tcpobsrelatorio'])) {

            $prazo_extra = 2;
            $expira = $dadosPrevisao['crdmesexecucao'] + $prazo_extra;

            $data = new DateTime($dataExecucao);
            $data->modify("+{$expira} month");
            $data_expira_br = $data->format('d-m-Y');
            $data_expira_us = $data->format('Y-m-d');

            $dateExpire = new DateTime($data_expira_us);
            $dateNow = new DateTime();
            $interval = $dateNow->diff($dateExpire);
            $dias = $interval->format('%R%a dias');
            //ver($dias, $interval);
        }

        if ($data_expira_br) {
            $html = "
                <table width='35%' border='1' align='center' cellspacing='2' cellpadding='2' style='border-collapse:collapse;'>
                    <tr>
                        <td class='subtitulocentro'>Alertas - Relatório de Cumprimento do Objeto</td>
                    </tr>
                    <tr>
                        <td style='color: #FF0000'><strong>Faltam {$dias} para o prazo de aprovação pela Coordenação expirar</strong></td>
                    </tr>
                    <tr>
                        <td><strong>Prazo final: <span style='font-size: 13px;'>{$data_expira_br}</span></strong></td>
                    </tr>
                </table>
            ";
        } else {
            return false;
        }

        return $html;
    }

    /**
     * @return bool|string
     */
    public function termosPendenciaAprovacaoCoordenacao()
    {
        $gestor = $this->pegaLinha("
            select
                ungcodproponente as proponente,
                ungcodconcedente as concedente
            from monitora.termocooperacao
            where tcpid = {$this->_tcpid}
        ");

        if (!is_array($gestor))
            return false;

        $strSQL = "
            select * from (
                select
                    (select tcpid from monitora.termocooperacao where docid = hd.docid) as tcpid,
                    CASE WHEN DATE_PART('days', NOW() - hd.htddata) > 60 AND hd.htddata > '".self::DATA_PORTARIA_2015."' THEN
                        true
                    else
                        false
                    END as prazo_expirado,
                    --to_char(hd.htddata + interval '60' day, 'DD/MM/YYYY') as deadline,
                    to_char(hd.htddata, 'DD/MM/YYYY') as deadline,
                    (select ungcodproponente from monitora.termocooperacao where tcpid = (select tcpid from monitora.termocooperacao where docid = hd.docid)) as proponente,
                    (select ungcodconcedente from monitora.termocooperacao where tcpid = (select tcpid from monitora.termocooperacao where docid = hd.docid)) as concedente,
                    (select tcpobsrelatorio from monitora.termocooperacao where tcpid = (select tcpid from monitora.termocooperacao where docid = hd.docid)) as tcpobsrelatorio,
                    (select esdid from workflow.documento where docid = hd.docid) as esdid
                from
                    workflow.historicodocumento hd
                where
                    aedid = 1652
                order by hstid desc
            ) vTable
            where
                vTable.prazo_expirado = 't'
                AND vTable.proponente = '{$gestor['proponente']}'
                AND vTable.concedente = '{$gestor['concedente']}'
                AND vTable.tcpobsrelatorio is null
                AND vTable.esdid = 656
        ";

        //ver($strSQL, d);
        $collection = $this->carregar($strSQL);
        $html = '';
        if (is_array($collection)) {
            $html.= '<table border="1" align="center" width="40%" cellpadding="2" cellspacing="2" style="border-collapse: collapse;">';
            $html.= '<tr>';
            $html.= '<td colspan="2" class="subtitulocentro">Relatório de Cumprimento do Objeto <br />
                                com pendência de aprovação pela Coordenação</td>';
            $html.= '</tr>';
            $html.= '<tr>';
            $html.= "<td align='center' width='50%'><strong>Termo de Execução</strong></td>";
            $html.= "<td align='center' width='50%'><strong>Data Limite</strong></td>";
            $html.= '<tr>';
            foreach ($collection as $row) {
                $html.= '<tr>';
                $html.= "<td align='center'>{$row['tcpid']}</td>";
                $html.= "<td align='center'>{$row['deadline']}</td>";
                $html.= '</tr>';
            }
            $html.= '</table>';
        } else {
            return false;
        }

        return $html;
    }

}
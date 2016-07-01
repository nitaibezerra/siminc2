<?php

/**
 * Class Ted_Model_ExtratoDOU
 */
class Ted_Model_ExtratoDOU extends Modelo
{
    /**
     * 60 dias de prazo extra == 2 meses
     */
    const PRAZO_EXTRA = '2';

    /**
     * @var string
     * PROCESSO Nº: {#nProcesso}.
     */
    public $template = "
        <p><strong>TERMO DE EXECUÇÃO DESCENTRALIZADA Nº:</strong> {#ted}/{#ano}.<br/><strong>ENTRE:</strong>
        {#proponente}, <strong>CNPJ nº:</strong> {#cnpjProponente} e {#concedente} - {#siglaConcedente}
        <strong>CNPJ Nº</strong> {#cnpjConcedente}<br /><strong>OBJETO:</strong> {#objetoTitulo}<br /><strong>VIGÊNCIA:</strong>
        {#inicio} a {#fim};<br><strong>DATA DA ASSINATÚRA:</strong> {#dataAssinatura}.</p>
        ======================================================================
    ";

    /**
     * @var
     */
    protected $_tcpid;

    /**
     * @throws Exception
     */
    public function __construct()
    {

    }

    /**
     * @return array|bool|null|void
     */
    private function get($tcpid)
    {
        if (!$tcpid) {
            throw new Exception('tcpid is null');
        }

        $strSQL = "
            select
                tcp.tcpid,
                (select ungdsc from public.unidadegestora where ungcod = tcp.ungcodproponente) as proponente,
                tcp.ungcodproponente,
                tcp.usucpfproponente,
                (select ungdsc from public.unidadegestora where ungcod = tcp.ungcodconcedente) as concedente,
                (select ungabrev from public.unidadegestora where ungcod = tcp.ungcodconcedente) as sigla,
                tcp.ungcodconcedente,
                tcp.usucpfconcedente,
                (select identificacao from ted.justificativa where tcpid = tcp.tcpid) as tcpdscobjetoidentificacao,
                to_char((select htddata from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = tcp.docid order by hstid asc limit 1), 'DD-MM-YYYY') as data_execucao,
                to_char((select htddata from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = tcp.docid order by hstid asc limit 1), 'DD/MM/YYYY') as inicio,
                to_char((select htddata from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = tcp.docid order by hstid asc limit 1), 'YYYY') as ano,
                case
                when (select count(*) from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = tcp.docid) = 1 then
                    (select crdmesexecucao from ted.previsaoorcamentaria where tcpid = tcp.tcpid and prostatus = 'A' and crdmesexecucao is not null order by crdmesexecucao asc limit 1)
                when (select count(*) from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = tcp.docid) > 1 then
                    (select crdmesexecucao from ted.previsaoorcamentaria where tcpid = tcp.tcpid and prostatus = 'A' and crdmesexecucao is not null order by crdmesexecucao desc limit 1)
                else
                    null
                end as vigencia,
                (select
                    to_char(wh.htddata, 'DD/MM/YYYY')
                from workflow.historicodocumento wh
                inner join seguranca.usuario u on (u.usucpf = wh.usucpf)
                where
                    wh.docid = tcp.docid
                    and wh.aedid = 1612
                order by hstid desc limit 1) as assinatura
            from ted.termocompromisso tcp
            where tcpid = {$tcpid}
        ";

        $linha = $this->pegaLinha($strSQL);
        return ($linha) ? $linha : null;
    }

    /**
     * @param null $ungcodproponente
     * @param null $ungcodconcedente
     * @return array|null|void
     */
    private function getAll(array $tcpid = array())
    {
        $strSQL = "
            select
                tcp.tcpid,
                (select ungdsc from public.unidadegestora where ungcod = tcp.ungcodproponente) as proponente,
                tcp.ungcodproponente,
                tcp.usucpfproponente,
                (select ungdsc from public.unidadegestora where ungcod = tcp.ungcodconcedente) as concedente,
                (select ungabrev from public.unidadegestora where ungcod = tcp.ungcodconcedente) as sigla,
                tcp.ungcodconcedente,
                tcp.usucpfconcedente,
                (select identificacao from ted.justificativa where tcpid = tcp.tcpid) as tcpdscobjetoidentificacao,
                to_char((select htddata from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = tcp.docid order by hstid asc limit 1), 'DD-MM-YYYY') as data_execucao,
                to_char((select htddata from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = tcp.docid order by hstid asc limit 1), 'YYYY') as ano,
                case
                when (select count(*) from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = tcp.docid) = 1 then
                    (select crdmesexecucao from ted.previsaoorcamentaria where tcpid = tcp.tcpid and prostatus = 'A' and crdmesexecucao is not null order by crdmesexecucao asc limit 1)
                when (select count(*) from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = tcp.docid) > 1 then
                    (select crdmesexecucao from ted.previsaoorcamentaria where tcpid = tcp.tcpid and prostatus = 'A' and crdmesexecucao is not null order by crdmesexecucao desc limit 1)
                else
                    null
                end as vigencia,
                (select
                    to_char(wh.htddata, 'DD/MM/YYYY')
                from workflow.historicodocumento wh
                inner join seguranca.usuario u on (u.usucpf = wh.usucpf)
                where
                    wh.docid = tcp.docid
                    and wh.aedid = 1612
                order by hstid desc limit 1) as assinatura
            from ted.termocompromisso tcp
            JOIN workflow.documento doc ON (doc.docid = tcp.docid)
            --doc.esdid = 639
        ";

        if (count($tcpid)) {
            $strSQL .= " where tcp.tcpid IN (".implode(',', $tcpid).")";
        }

        //ver($strSQL, d);
        $collection = $this->carregar($strSQL);
        return ($collection) ? $collection : null;
    }

    /**
     * @param array $linha
     * @return mixed
     */
    private function getHtml(array $linha)
    {
        extract($linha);

        $expira = $vigencia + self::PRAZO_EXTRA;

        $data = new DateTime($data_execucao);
        $data->modify("+{$expira} month");
        //$data->modify("+{$vigencia} month");
        $data_expira = $data->format('d/m/Y');

        //'{#nProcesso}'
        $search = array('{#ted}', '{#ano}', '{#proponente}','{#cnpjProponente}', '{#concedente}', '{#siglaConcedente}',
            '{#cnpjConcedente}', '{#objetoTitulo}', '{#inicio}', '{#fim}', '{#dataAssinatura}');

        $replace = array($tcpid, $ano, $proponente, formatar_cpf($usucpfproponente), $concedente,
            trim($sigla), formatar_cpf($usucpfconcedente), $tcpdscobjetoidentificacao, $data_execucao,
            $data_expira, $assinatura);

         return str_replace($search, $replace, $this->template);
    }

    /**
     * @param array $tcpid
     * @return string
     */
    public function getLote(array $tcpid = array())
    {
        $htmlOutPut = '';
        $collection = $this->getAll($tcpid);
        foreach ($collection as $row) {
            $htmlOutPut .= $this->getHtml($row);
        }

        return utf8_encode($htmlOutPut);
    }

    /**
     * @return string
     */
    public function getSigle()
    {
        return $this->getHtml($this->get());
    }
}
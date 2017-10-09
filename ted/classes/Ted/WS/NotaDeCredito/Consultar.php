<?php

class Ted_WS_NotaDeCredito_Consultar
{
    private $tcpid;

    private $loginsigef;

    private $senhasigef;

    private $erro;

    private $message;

    private $endPoint = 'https://www.fnde.gov.br/webservices/sigef/index.php/financeiro/nc';

    /**
     * @param array $dados
     */
    public function __construct(array $dados)
    {
        $this->loginsigef = $dados['sigefusername'];
        $this->senhasigef = $dados['sigefpassword'];
        $this->tcpid = $dados['tcpid'];
        $this->erro = false;

        return $this;
    }

    /**
     * @return bool
     */
    public function isError()
    {
        return ($this->erro) ? true : false;
    }

    /**
     * @return mixed
     */
    public function getResult()
    {
        if (!empty($this->message)) return $this->message;
    }

    /**
     * @param $tcpid
     */
    public function verificaNcSigef()
    {
        $strSQL = sprintf("
            SELECT
                sigefid
            FROM
                ted.previsaoorcamentaria
            WHERE
                tcpid = %d AND prostatus = 'A'
            AND sigefid IS NOT NULL
            AND codsigefnc IS NULL LIMIT 1
        ", $this->tcpid);

        $retorno = Ted_Utils_Model::dbGetInstance()->pegaLinha($strSQL);
        if ($retorno) {
            $this->_requestSigefNc($retorno['sigefid']);
        }
    }

    /**
     * @param $sigefid
     * @return bool
     */
    protected function _requestSigefNc($sigefid)
    {
        $retorno = $this->_consultarSigefNC($sigefid);

        if (!is_array($retorno)) {
            $this->message = 'A nota de crédito ainda não foi efetivada junto ao SIGEF, tente mais tarde!';
            $this->erro = true;
            return false;
        }

        $rsProid = Ted_Utils_Model::dbGetInstance()->carregar(
            sprintf("select proid, provalor from ted.previsaoorcamentaria where sigefid = %d", $sigefid));

        if (!$rsProid) {
            $this->message = 'Não foi encontrado programação orçamentária para o Termo solicitado!';
            $this->erro = true;
            return false;
        }

        $strSQL = sprintf("
                UPDATE ted.previsaoorcamentaria
                SET codsigefnc = '%s'
                WHERE sigefid = %d
            ", $retorno['numero_documento_siafi'], $sigefid);
        Ted_Utils_Model::dbGetInstance()->executar($strSQL);

        foreach ($rsProid as $previsao) {

            $sqlInsert = "insert into ted.previsaoparcela(proid, ppavlrparcela, codsigefnc, tcpnumtransfsiafi, ppacadastradosigef)
                values('{$previsao['proid']}', {$previsao['provalor']}, '{$retorno['numero_documento_siafi']}', '{$retorno['termo_compromisso']}', 'f')";
            Ted_Utils_Model::dbGetInstance()->executar($sqlInsert);
            Ted_Utils_Model::dbGetInstance()->commit();
        }

        $this->message = 'Nota de crédito efetivada com sucesso junto ao SIGEF!';
        return true;
    }

    /**
     * @param $sigefid
     * @return array|bool
     */
    protected function _consultarSigefNC($sigefid)
    {
        $xmlSigefNC = new SimpleXMLElement('<?xml version="1.0" encoding="ISO-8859-1"?><request></request>');
        $header = $xmlSigefNC->addChild('header');
        $header->addChild('app', SIGLA_SISTEMA);
        $header->addChild('version', '1.4.1');
        $header->addChild('created', date('c'));
        $body = $xmlSigefNC->addChild('body');
        $auth = $body->addChild('auth');

        if (IS_PRODUCAO) {
            $auth->addChild('usuario', $this->loginsigef);
            $auth->addChild('senha', $this->senhasigef);
        } else {
            $auth->addChild('usuario', 'luciab');
            $auth->addChild('senha', 'paulo005');
        }

        $params = $body->addChild('params');
        $params->addChild('sequencial', $sigefid);
        //ver(simec_htmlentities($xmlSigefNC->asXML()), d);

        require_once APPRAIZ . 'includes/classes/Fnde_Webservice_Client.class.inc';

        $arrayParams = array(
            'xml' => $xmlSigefNC->asXML(),
            'method' => 'consultar'
        );

        $xml = Fnde_Webservice_Client::CreateRequest()
            ->setURL($this->endPoint)
            ->setParams($arrayParams)
            ->execute();

        $resultXml = new SimpleXMLElement($xml);

        $domEnvio = dom_import_simplexml($xmlSigefNC)->ownerDocument;
        $domEnvio->formatOutput = true;
        //ver(simec_htmlentities($domEnvio->saveXML()));

        $domRetorno = dom_import_simplexml($resultXml)->ownerDocument;
        $domRetorno->formatOutput = true;
        //ver(simec_htmlentities($domRetorno->saveXML()), d);

        $result = (int) $resultXml->status->result;
        //$sequencial = $resultXml->body->ncs->sequencial;
        $numero_documento_siafi = (string) $resultXml->body->ncs->numero_documento_siafi;
        $termo_compromisso = (string) $resultXml->body->ncs->termo_compromisso;

        $param = array(
            'tcpid' => $this->tcpid,
            'logmsg' => '',
            'logtipo' => Ted_Model_Log::VERIFICA_EFETIVACAO_NC_SIGEF,
            'logurl' => $this->endPoint,
            'logxmlenvio' => str_replace("'","\'", $domEnvio->saveXML()),
            'logdtretorno' => date("d/m/Y H:i:s"),
            'logxmlretorno' => str_replace("'", "\'", $domRetorno->saveXML()),
            'logerro' => ($result) ? true : false
        );
        $log = new Ted_Model_Log($param);
        $log->salvar();
        $log->commit();

        if ($result && !empty($numero_documento_siafi) && !empty($termo_compromisso)) {
            return array(
                'numero_documento_siafi' => $numero_documento_siafi,
                'termo_compromisso' => $termo_compromisso
            );
        } else {
            return false;
        }
    }

}
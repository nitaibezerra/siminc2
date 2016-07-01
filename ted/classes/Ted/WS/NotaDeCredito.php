<?php

require_once APPRAIZ . 'includes/classes/Fnde_Webservice_Client.class.inc';

/**
 * Class Ted_WS_NotaDeCredito
 */
class Ted_WS_NotaDeCredito
{
    /**
     * @var string
     */
    private $endPointProd = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/nc';

    /**
     * @var string
     */
    private $endPointDev = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/financeiro/nc';

    /**
     * tp_processo
     */
    const TP_PROCESSO = 1;

    /**
     * nu_sistema
     */
    const NU_SISTEMA = '';

    /**
     * unicod FNDE
     */
    const UNICOD_FNDE = '26298';
	
    /**
     * Mensagens
     */
    const MSG_SUCESSO_SOLICITACAO = 'Solicitação realizada com sucesso.';
    const MSG_ERRO_SOLICITACAO = 'Falha a o enviar a solicitação.';
    const MSG_ERRO_CHEKCEL_VAZIO = 'Selecione uma previsão orçamentária.';
    
    /**
     * dados consolidado do ted
     */
    protected $_dados = array();

    /**
     * @var string
     */
    private $wsusuario, $wssenha, $result, $error, $msgSigef;

    /**
     * @param string $wsusuario
     * @param string $wssenha
     */
    public function __construct($wsusuario = '', $wssenha = '')
    {
        //ver($_POST);
		$this->wsusuario = $wsusuario;
		$this->wssenha = $wssenha;
        $this->_dados = $_POST;
        $this->error = false;
	}

    /**
     * @return mixed
     */
    public function getResult()
    {
		return $this->result;
	}

    /**
     * @return mixed
     */
    public function getError()
    {
        return $this->error;
    }

    /**
     * @return string
     */
    public function getMessageSigef()
    {
        return $this->msgSigef;
    }

    /**
     * @param $tcpid
     * @return bool|string
     */
    public function montaXmlNotaCreditoFnde($tcpid)
    {
		if (empty($this->_dados['chekCel'])) {
			$this->result = Ted_WS_NotaDeCredito::MSG_ERRO_CHEKCEL_VAZIO;
			return false;
		}
		
		$sql = "
            select
                tcp.tcpid,
                unp.ungcod as unidade_gestora_favorecida,
                unp.gescod as gestao_favorecida
            from ted.termocompromisso tcp
            join public.unidadegestora unp on unp.ungcod = tcp.ungcodproponente
            where tcp.tcpid = {$tcpid}
		";
	
		$rsTC = Ted_Utils_Model::dbGetInstance()->pegaLinha($sql);
	
		if ($rsTC) {
	
            $data_created = date('c');

            // Dados do termo
            $unidade_gestora_favorecida = $rsTC['unidade_gestora_favorecida'];
            $gestao_favorecida = $rsTC['gestao_favorecida'];
            $observacao = $this->_dados['tcpobsfnde'];
            $complemento = $this->_dados['tcpobscomplemento'] ? substr($this->_dados['tcpobscomplemento'], 0, 239) : '';
            $processo = str_replace(array('.','/','-'), '', $this->_dados['tcpnumprocessofnde']);
            $numero_documento_siafi_original = '';
            $nc_original = '';
            $especie = $this->_dados['especie'];
            $programa = str_pad($this->_dados['tcpprogramafnde'], 2, '0', STR_PAD_LEFT);
            $sistema = $this->_dados['sistema'];
            $termo_compromisso = $this->_dados['tcpnumtransfsiafi'];

            $sql = "
                update ted.termocompromisso set
                    tcpnumtransfsiafi 	= '{$termo_compromisso}',
                    tcpnumprocessofnde 	= '{$processo}',
                    tcpprogramafnde 	= '{$programa}',
                    tcpobsfnde 			= '{$observacao}',
                    ungcodemitente 		= '{$this->_dados['ungcodemitente']}',
                    gescodemitente 		= '{$this->_dados['gescodemitente']}',
                    tcpobscomplemento 	= '{$complemento}'
                where tcpid = '{$tcpid}'
            ";
            Ted_Utils_Model::dbGetInstance()->executar($sql);

            if ($rsTC['tcpid']) {
                $detalhamento = '';
                $rsPO = $this->_buscaCelulasDeCredito($this->_dados);

                if($rsPO) {

                    foreach ($rsPO as $po) {

                        if (!$this->_dados['prgid'][$po['proid']])
                            continue;

                        $strSQL = "select * from ted.dadosprogramasfnde where prgid = '{$this->_dados['prgid'][$po['proid']]}'";
                        $rsPrograma = Ted_Utils_Model::dbGetInstance()->pegaLinha($strSQL);

                        $strSQL = "
                            update ted.previsaoorcamentaria set
                                prgidfnde = '{$this->_dados['prgid'][$po['proid']]}',
                                prgfonterecurso = '{$this->_dados['prgfonterecurso'][$po['proid']]}',
                                espid = '{$this->_dados['espid'][$po['proid']]}',
                                esfid = '{$this->_dados['esfid'][$po['proid']]}'
                            where proid = {$po['proid']}
                        ";
                        Ted_Utils_Model::dbGetInstance()->executar($strSQL);
                        //ver($this->_dados, $rsPrograma, $po);

                        $evento_contabil 			= $this->_dados['evento_contabil'];
                        $esfera_orcamentaria 		= $this->_dados['esfid'][$po['proid']];
                        $unidade_orcamentaria 		= $this->_dados['unicod'];
                        $centro_gestao 				= $rsPrograma['gescod'];
                        $ptres 						= $po['ptres'];
                        $fonte_recurso 				= $this->_dados['prgfonterecurso'][$po['proid']];
                        $natureza_despesa 			= $po['ndpcod'];
                        $plano_interno 				= $po['plicod'];
                        $ano_exercicio 				= $po['proanoreferencia'];
                        $valor 						= $po['provalor'];
                        $unidade_gestora_emitente 	= $this->_dados['ungcodemitente'];
                        $gestao_emitente 			= $this->_dados['gescodemitente'];

                        $detalhamento .= "<detalhamento>
                            <evento_contabil>$evento_contabil</evento_contabil>
                            <esfera_orcamentaria>$esfera_orcamentaria</esfera_orcamentaria>
                            <unidade_orcamentaria>$unidade_orcamentaria</unidade_orcamentaria>
                            <centro_gestao>$centro_gestao</centro_gestao>
                            <celula_orcamentaria>
                            <ptres>$ptres</ptres>
                            <fonte_recurso>$fonte_recurso</fonte_recurso>
                            <natureza_despesa>$natureza_despesa</natureza_despesa>
                            <plano_interno>$plano_interno</plano_interno>
                            </celula_orcamentaria>
                            <ano_exercicio>$ano_exercicio</ano_exercicio>
                            <valor>$valor</valor>
                            <unidade_gestora_emitente>$unidade_gestora_emitente</unidade_gestora_emitente>
                            <gestao_emitente>$gestao_emitente</gestao_emitente>
                        </detalhamento>
                        ";
                    }
                }
            }
        $arqXml = <<<XML
<?xml version="1.0" encoding="iso-8859-1"?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>{$this->wsusuario}</usuario>
			<senha>{$this->wssenha}</senha>
		</auth>
		<params>
			<unidade_gestora_favorecida>$unidade_gestora_favorecida</unidade_gestora_favorecida>
			<gestao_favorecida>$gestao_favorecida</gestao_favorecida>
			<observacao>$observacao</observacao>
			<complemento>$complemento</complemento>
			<processo>$processo</processo>
			<numero_documento_siafi_original>$numero_documento_siafi_original</numero_documento_siafi_original>
			<nc_original>$nc_original</nc_original>
			<especie>$especie</especie>
			<programa>$programa</programa>
			$detalhamento
			<sistema>$sistema</sistema>
				<termo_compromisso>$termo_compromisso</termo_compromisso>
			</params>
		</body>
	</request>
XML;
	
	        if (Ted_Utils_Model::dbGetInstance()->commit()) {
	            return $arqXml;
	        }
	    }
	
	    return false;
	}
	
	public function solicitaNC($arrParam = array())
    {
		$strSQL = "
            SELECT * FROM ted.previsaoorcamentaria po
            JOIN monitora.pi_planointerno pi ON pi.pliid = po.pliid
            WHERE tcpid = {$arrParam['tcpid']} AND po.sigefid IS NULL AND po.codsigefnc IS NULL
            AND po.proid NOT IN(
                SELECT pre.proid FROM ted.previsaoparcela pre
                JOIN ted.previsaoorcamentaria pro ON pro.proid = pre.proid
                WHERE pro.tcpid = {$arrParam['tcpid']}
            )
		";
		
		//ver($strSQL, d);
		$rsPO = Ted_Utils_Model::dbGetInstance()->carregar($strSQL);
		
		$sqlCnpj = "
            select
                ungcnpj
            from ted.termocompromisso t
            join unidadegestora u on (u.ungcod = t.ungcodproponente)
            where t.tcpid = {$arrParam['tcpid']}
        ";

		$arrParamProcesso['nu_processo'] 		= str_replace(array('.','/','-'), '', $arrParam['tcpnumprocessofnde']);
		$arrParamProcesso['tp_processo'] 		= $arrParam['tpprocesso'];
		$arrParamProcesso['co_programa_fnde'] 	= $arrParam['tcpprogramafnde'];
		$arrParamProcesso['nu_sistema']			= $arrParam['nusistema'];
		$arrParamProcesso['nu_cnpj_favorecido'] = Ted_Utils_Model::dbGetInstance()->pegaUm($sqlCnpj);

		if (IS_PRODUCAO) {
			$urlWS = $this->endPointProd;
		} else {
			$urlWS = $this->endPointDev;
		}
		
		$arqXml = $this->montaXmlNotaCreditoFnde($arrParam['tcpid']);
        //ver(htmlentities($arqXml));
        //ver($this->_dados, d);
		
		if ($arqXml && $rsPO) {
	
            $xml = Fnde_Webservice_Client::CreateRequest()
                   ->setURL($urlWS)
                   ->setParams(array('xml' => $arqXml, 'method' => 'solicitar'))
                   ->execute();

            // -- Processamento do retorno do XML
            $xmlRetorno    = $xml;
            $xml           = simplexml_load_string( stripslashes($xml) );
            $identificador = (integer) $xml->body->identificador;
            $erroText      = (string) utf8_decode(str_replace(array("'"), '', $xml->status->error->message->text));
            $erroCod       = (string) $xml->status->error->message->code;
            $erro          = str_replace(array("'", '"'),'',simec_json_encode($xml->status->error));
            $message       = (string) utf8_decode($xml->status->message->text);
            $result        = (integer) $xml->status->result;
            $txt           = "Erro: {$erroCod} - {$erroText}, Mensagem: {$message}";

            if (false != $identificador) {

                $sql = "
                    UPDATE ted.previsaoorcamentaria SET
                        sigefid = '{$identificador}',
                        codsigefnc = null
                    WHERE proid IN (".implode(',', $this->_dados['chekCel']).")
                ";

                Ted_Utils_Model::dbGetInstance()->executar($sql);
                Ted_Utils_Model::dbGetInstance()->commit();

                $param = array(
                    'tcpid' => $this->_dados['ted'],
                    'logmsg' => 'Sucesso',
                    'logtipo' => Ted_Model_Log::SOLICITA_NOTA_CREDITO_FNDE,
                    'logurl' => $urlWS,
                    'logxmlenvio' => str_replace(array("'", '"'),"",$arqXml),
                    'logdtretorno' => date("d/m/Y H:i:s"),
                    'logxmlretorno' => str_replace(array("'", '"'), "", $xmlRetorno)
                );
                $log = new Ted_Model_Log($param);
                $log->salvar();
                $log->commit();

            } else {

                $param = array(
                    'tcpid' => $this->_dados['ted'],
                    'logmsg' => $erro,
                    'logtipo' => Ted_Model_Log::SOLICITA_NOTA_CREDITO_FNDE,
                    'logurl' => $urlWS,
                    'logxmlenvio' => str_replace("'","\'",$arqXml),
                    'logdtretorno' => date("d/m/Y H:i:s"),
                    'logxmlretorno' => str_replace("'", "\'", $xmlRetorno),
                    'logerro' => true
                );
                $log = new Ted_Model_Log($param);
                $log->salvar();
                $log->commit();
            }

            $this->result =  Ted_WS_NotaDeCredito::MSG_SUCESSO_SOLICITACAO;

            if ($result) {
                $this->error = true;
                $this->msgSigef = $txt;
            } else {
                $this->msgSigef = $txt;
            }



        } else {
            $this->result =  Ted_WS_NotaDeCredito::MSG_ERRO_SOLICITACAO;
            $this->error = true;
            $this->msgSigef = $txt;
        }
    }

    /**
     * @param array $dados
     * @return array|void
     */
    protected function _buscaCelulasDeCredito(array $dados)
    {
        $strSQL = "
            SELECT DISTINCT
                proid,
                ptres,
                provalor,
                proanoreferencia,
                plicod,
                ndpcod
            FROM ted.previsaoorcamentaria pro
            LEFT JOIN monitora.pi_planointerno pi 		 ON pi.pliid = pro.pliid
            LEFT JOIN monitora.pi_planointernoptres pts  ON pts.pliid = pi.pliid
            LEFT JOIN public.naturezadespesa ndp 		 ON ndp.ndpid = pro.ndpid
            LEFT JOIN monitora.ptres p 					 ON p.ptrid = pro.ptrid
            LEFT JOIN monitora.acao a 					 ON a.acaid = p.acaid
            LEFT JOIN public.unidadegestora u 			 ON u.unicod = p.unicod
            LEFT JOIN monitora.pi_planointernoptres pt 	 ON pt.ptrid = p.ptrid
            WHERE pro.prostatus = 'A'
            AND pro.proid in (".implode(',', $dados['chekCel']).")
		";

        return Ted_Utils_Model::dbGetInstance()->carregar($strSQL);
    }

}
<?php

include_once APPRAIZ.'includes/classes/ProcessoFNDE.class.php';

include_once APPRAIZ . "includes/classes_simec.inc";

/**
 * Class Ted_WS_Documenta
 */
class Ted_WS_Documenta
{
    /**
     * @var
     */
    protected $_tcpid;

    /**
     * @var
     */
    protected $_dadosTermo;

    /**
     * @var array
     */
    protected $_credenciais = array();

    /**
     * @var
     */
    protected $_wsResult;

    /**
     * @var
     */
    protected $_db;

    /**
     * @var
     */
    protected $_tipo;

    /**
     * @var bool
     */
    protected $_errors = array();

    /**
     * @var array
     */
    protected $_success = array();

    /**
     *
     */
    const NU_CPF = '';

    /**
     *
     */
    const CO_ASSUNTO = '051.21';

    /**
     *
     */
    const DS_RESUMO = '051.21 - DESCENTRALIZAÇÃO DE RECURSOS';

    /**
     *
     */
    const ERROR_MESSAGE_1 = 'Falha ao Gerar o Processo.';

    /**
     *
     */
    const ERROR_MESSAGE_2 = 'Falha ao Gerar o Processo. CNPJ da Unidade Gestora não Encontrado.';
	
    /**
     * 
     */
    const ERROR_MESSAGE_3 = 'Processo já cadastrado.';
    
    /**
     *
     */
    const SUCCESS_MESSAGE = 'Processo Gerado com Sucesso.';

    /**
     * @param array $request
     * @return $this
     */
    public function __construct(array $request)
    {
        if (!isset($request['tcpid'])) {
            throw new Exception('A classe precisa do número do Termo de Execução Descentralizada');
        }

        $this->_db = new cls_banco();
        $this->_tcpid = (int) $request['tcpid'];
        $this->_credenciais = array(
            'login'    => strip_tags($request['login']),
            'password' => strip_tags($request['senha'])
        );

        $this->_tipo = $request['tipo'];
        return $this;
    }

    /**
     * Executa todos os processo
     * @return $this
     */
    public function processaWs()
    {
        if ($this->_existeCnpjUnidade()) {
            if ($this->_submitWs()) {
                $this->_salvaRetornoWs();
            }
        }

        return $this;
    }

    /**
     * Verifica se houve algum registro de erro em alguma operação
     * @return string
     */
    public function getWsResult()
    {
        if (count($this->_errors)) {
            return $this->_errors['message'];
        }

        return self::SUCCESS_MESSAGE;
    }

    /**
     * Retorna true se houve alguma captura de error via WS
     * @return bool
     */
    public function isWsErrors()
    {
        return (count($this->_errors)) ? true : false;
    }

    /**
     * Verifica se existe no TED os dados necessários para o envio ao WS FNDE
     * @return bool
     */
    protected function _existeCnpjUnidade()
    {
        $this->_dadosTermo = $this->_db->pegaLinha("
            SELECT
                ug.ungcnpj, tc.tcpnumprocessofnde
            FROM ted.termocompromisso tc
            JOIN public.unidadegestora ug ON (ug.ungcod = tc.ungcodproponente)
            WHERE tc.tcpid = {$this->_tcpid}
        ");

        if ($this->_dadosTermo['ungcnpj'] && empty($this->_dadosTermo['tcpnumprocessofnde'])) {
            return true;
        } else {
        	if (!empty($this->_dadosTermo['tcpnumprocessofnde'])){
        		$this->_errors['message'] = self::ERROR_MESSAGE_3;
        	} else {
            	$this->_errors['message'] = self::ERROR_MESSAGE_2;
        	}
            return false;
        }
    }

    /**
     * Envia os dados para o WS e guarda o retorno
     * @return bool
     */
    protected function _submitWs()
    {
        $fromWSPost = array(
            'nu_cpf' => self::NU_CPF,
            'co_assunto' => self::CO_ASSUNTO,
            'ds_resumo' => self::DS_RESUMO,
            'nu_cnpj' => $this->_dadosTermo['ungcnpj']
        );

        $_processoFNDE = new ProcessoFNDE($this->_credenciais['login'], $this->_credenciais['password']);
        $this->_wsResult = $_processoFNDE->gerarProcessoFNDE($fromWSPost);
        if ($this->_wsResult) {
            return true;
        } else {
            $this->_errors['message'] = self::ERROR_MESSAGE_1;
            return false;
        }
    }

    /**
     * Salva o retorno do WS Documenta FNDE no TED
     * @return mixed
     */
    protected function _salvaRetornoWs()
    {
        $strSQL = "
            UPDATE ted.termocompromisso SET
                tcpbancofnde = '%s',
                tcpagenciafnde = '%s',
                tcpnumprocessofnde = '%s'
            WHERE tcpid = %d
        ";

        $stmt = sprintf($strSQL,
            $this->_wsResult['banco'],
            $this->_wsResult['agencia'],
            $this->_wsResult['processo'],
            $this->_tcpid
        );

        $this->_db->executar($stmt);
        return $this->_db->commit();
    }

}
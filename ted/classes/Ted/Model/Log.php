<?php

class Ted_Model_Log extends modelo
{
	/**
	 * Nome da Tabela
	 * @var String
	 */
	protected $stNomeTabela = 'ted.log';
	
	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('logid');
	
	const GERA_NUMERO_PROCESSO_FNDE = '1'; // Log da geração de número de processo do FNDE
	
	const SOLICITA_NOTA_CREDITO_FNDE = '2'; // Log de solicitação de nota de crédito do FNDE
	
	const ENVIA_NOTA_CREDITO_PAGAMENTO_FNDE = '3'; // Log de envio de nota de crédito do FNDE

    const VERIFICA_EFETIVACAO_NC_SIGEF = '4'; //Log de efetivação de pedido de NC
	
	/**
	 * Atributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributos = array(
			'logid' => NULL,
			'usucpf' => NULL,
			'tcpid' => NULL,
			'logmsg' => NULL,
			'logdata' => NULL,
			'logtipo' => NULL,
			'logwebservice' => NULL,
			'logurl' => NULL,
			'logxmlenvio' => NULL,
			'logdtretorno' => NULL,
			'logxmlretorno' => NULL,
			'logerro' => NULL,
			'logmetodo' => NULL
	);
	
	public function __construct($arParam = array())
	{
		$this->arAtributos['tcpid']         = Ted_Utils_Model::capturaTcpid();
		$this->arAtributos['usucpf']        = $_SESSION['usucpf'];
		$this->arAtributos['logmsg']        = $arParam['logmsg']        != '' ? $arParam['logmsg']        : NULL;
		$this->arAtributos['logtipo']       = $arParam['logtipo']       != '' ? $arParam['logtipo']       : NULL;
		$this->arAtributos['logwebservice'] = $arParam['logwebservice'] != '' ? $arParam['logwebservice'] : NULL;
		$this->arAtributos['logurl']        = $arParam['logurl']        != '' ? $arParam['logurl']        : NULL;
		$this->arAtributos['logxmlenvio']   = $arParam['logxmlenvio']   != '' ? $arParam['logxmlenvio']   : NULL;
		$this->arAtributos['logdtretorno']  = $arParam['logdtretorno']  != '' ? $arParam['logdtretorno']  : NULL;
		$this->arAtributos['logxmlretorno'] = $arParam['logxmlretorno'] != '' ? $arParam['logxmlretorno'] : NULL;
		$this->arAtributos['logerro']       = $arParam['logerro']       != '' ? $arParam['logerro']       : NULL;
		$this->arAtributos['logmetodo']     = $arParam['logmetodo']     != '' ? $arParam['logmetodo']     : NULL;
	}
	
	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
			'logid',
			'usucpf',
			'logdata',
			'logtipo'
	);
	
	/**
	 * Valida campos obrigatorios no objeto populado
	 *
	 * @author Sávio Resende - Copiado por Lindalberto Filho
	 * @return bool
	*/
	public function validaCamposObrigatorios()
	{
		foreach ($this->arAtributosObrigatorios as $chave => $valor)
		if (!isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]))
			return false;
			
		return true;
	}

}
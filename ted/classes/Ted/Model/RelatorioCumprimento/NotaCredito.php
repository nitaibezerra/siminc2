<?php

class Ted_Model_RelatorioCumprimento_NotaCredito extends modelo
{
    /**
	 * Nome da Tabela
	 * @var String
	 */
	protected $stNomeTabela = 'ted.ncrelatoriocumprimento';
	
	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('rcnid');
	
	/**
	 * Atributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributos = array(
		'rcnid' => NULL,
  		'tcpid' => NULL,
  		'recid' => NULL,
  		'rcnnumnc' => NULL,
  		'rcndevolucao' => NULL,
  		'rpustatus' => NULL
	);
	
	public function __construct()
    {
		$this->arAtributos['tcpid'] = Ted_Utils_Model::capturaTcpid();
	}
	
	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		
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
        
    /**
     * @return array|null|void
     */
    public function pegaListaPrograma()
    {
        $strSQL = "
            SELECT DISTINCT 
                prgcodfnde as codigo, 
                prgcodfnde as descricao 
            FROM ted.dadosprogramasfnde
            WHERE eventocontabil = '300300' 
            ORDER BY prgcodfnde;
        ";

        $list = $this->carregar($strSQL);

        $options = array();
        if ($list) {
            foreach($list as $item) {
                $options[$item['codigo']] = $item['descricao'];
            }
        }

        return ($options) ? $options : array();
    }
    
    /**
     * @return array|null|void
     */
    public function pegaListaObservacao($tcpprogramafnde)
    {
        $strSQL = "
            SELECT 
                DISTINCT obscod as codigo, 
                obscod as descricao 
            FROM ted.dadosprogramasfnde
            WHERE eventocontabil = '300300' and prgcodfnde = '{$tcpprogramafnde}' 
            ORDER BY obscod;
        ";

        $list = $this->carregar($strSQL);
        $options = array();
        if ($list) {
            foreach($list as $item) {
                $options[$item['codigo']] = $item['descricao'];
            }
        }

        return ($options) ? $options : array();
    }
	
}
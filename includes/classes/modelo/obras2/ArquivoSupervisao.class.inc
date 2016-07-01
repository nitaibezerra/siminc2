<?php
	
class ArquivoSupervisao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.arquivosupervisao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "aqsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'aqsid' => null, 
									  	'sueid' => null, 
									  	'smiid' => null,
									  	'sfndeid' => null, 
									  	'arqid' => null, 
									  	'aqsstatus' => null, 
									  );

	/**
	 * Retorna os dados a partir de um arqid
	 *
	 * @param int $arqid
	 * @param int $sueid
	 */
	public function carregaPorArqidSueid($arqid,$sueid) {
		 $sql = "SELECT * FROM {$this->stNomeTabela} where arqid = " . $arqid . " and sueid = " . $sueid;
		 $dados = $this->pegaLinha($sql);
		 $this->popularDadosObjeto($dados);
	}
	
	/**
	 * Retorna os dados a partir de um arqid
	 *
	 * @param int $arqid
	 * @param int $smiid
	 */
	public function carregaPorArqidSmiid($arqid, $smiid) {
		 $sql = "SELECT * FROM {$this->stNomeTabela} where arqid = " . $arqid . " and smiid = " . $smiid;
		 $dados = $this->pegaLinha($sql);
		 $this->popularDadosObjeto($dados);
	}

	/**
	 * Retorna os dados a partir de um arqid
	 *
	 * @param int $arqid
	 * @param int $sfndeid
	 */
	public function carregaPorArqidSfndeid($arqid, $sfndeid) {
		 $sql = "SELECT * FROM {$this->stNomeTabela} where arqid = " . $arqid . " and sfndeid = " . $sfndeid;
		 $dados = $this->pegaLinha($sql);
		 $this->popularDadosObjeto($dados);
	}
}
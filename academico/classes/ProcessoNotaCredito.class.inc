<?php
	
class ProcessoNotaCredito extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.processonotacredito";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pncid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pncid' => null, 
									  	'prcid' => null, 
									  	'pncnotacredito' => null, 
									  	'pncdtinclusao' => null, 
									  	'usucpf' => null,
									  );
									  
	public function excluirPorPrcid($prcid){
		$this->executar("delete from $this->stNomeTabela where prcid = $prcid");	
	}
}
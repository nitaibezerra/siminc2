<?php
	
class PrestacaoContasResposta extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "rehuf.prestacaocontasresposta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pcrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pcrid' => null,
									  	'prcid' => null,
    									'qrpid' => null
									  );
									  
									  
									  
	public function buscarPrestacaoContasRespostaPorPrestacaoContas($prcid){
				$sql = "select
								pcrid
							from 
								{$this->stNomeTabela}  
							where
								prcid = '$prcid'";
				return  $this->pegaUm($sql);
		}
}
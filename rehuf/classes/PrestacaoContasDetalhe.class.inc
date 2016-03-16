<?php
	
class PrestacaoContasDetalhe extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "rehuf.prestacaocontasdetalhe";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pcdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pcdid' => null,
									  	'prcid' => null,
    									'prcdempenho' => null, 
    									'prcdnaturezadespesadetalhada' => null,
									  	'prcdcnpjfavorecido' => null, 
									  	'prcdvalorempenhoemitido' => null 
									  );
									  
	public function buscarPrestacaoContasDetalhePorPrestacaoContas($prcid){
				$sql = "select
								pcdid
							from 
								{$this->stNomeTabela}  
							where
								prcid = '$prcid'";
				return  $this->pegaUm($sql);
		}
}
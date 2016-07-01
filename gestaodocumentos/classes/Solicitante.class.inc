<?php
	
class Solicitante extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "gestaodocumentos.solicitante";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "solid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'solid' => null, 
									  	'soltiposolicitante' => null, 
									  	'soltipopessoa' => null, 
									  	'tarid' => null, 
									  	'entid' => null, 
    									'iesid' => null,
									  	'unaid' => null, 
									  	'solcpf' => null, 
									  	'solnome' => null, 
									  	'soltelefone' => null, 
									  	'solemail' => null, 
									  	'estuf' => null, 
									  	'muncod' => null, 
									  	'solendereco' => null, 
									  	'solddd' => null, 
									  );
									  
	public function carregaPorTarefaLista($tarid){
		return $this->carregar("select s.solid, 
									   u.unasigla ||' - '|| u.unadescricao as unidade,
									   s.solnome
								from gestaodocumentos.solicitante s 
									left join gestaodocumentos.unidade u on s.unaid = u.unaid
								where s.tarid = $tarid ");
	}
							  
	public function carregaPorTarefa($tarid){
		return $this->carregar("select solid, 
									   tarid,
									   soltiposolicitante,
									   soltipopessoa,
									   unaid,
									   estuf,
									   muncod,
									   solnome,
									   soltelefone,
									   solemail,									   
									   entid,
									   iesid,
									   solcpf,
									   solendereco,
									   solddd
								from gestaodocumentos.solicitante 
								where tarid = $tarid ");
								
	}
	
	public function deletaPorTarid( $tarid ){
		$sql = "DELETE FROM gestaodocumentos.solicitante WHERE tarid = '$tarid'";
		if ( !$this->executar($sql)){
			return false;
		}
	}
}
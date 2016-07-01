<?php
	
class ObraLicitacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.obralicitacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "oblid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'oblid' => null, 
									  	'obrid' => null, 
									  	'licid' => null, 
									  	'oblstatus' => null, 
									  );

	public function apagaPorLicitacao( $licid ){
		$sql = "UPDATE obras2.obralicitacao SET oblstatus = 'I' WHERE licid = {$licid};";
		$this->executar( $sql );	
	}

	public function listaObraByLicid( $licid ){
		$sql = "SELECT
					o.*
				FROM
					obras2.obralicitacao ol
				JOIN obras2.obras o ON o.obrid = ol.obrid AND
									   o.obrstatus = 'A'
				WHERE
					ol.oblstatus = 'A' AND 
					ol.licid = {$licid} AND
					ol.obrid NOT IN ( SELECT 
											obrid 
										 FROM
										 	obras2.obrascontrato oc
										 JOIN obras2.contrato c ON c.crtid = oc.crtid AND
										 						   c.crtstatus = 'A'
										 WHERE
										 	oc.ocrstatus = 'A' )
				ORDER BY
					o.obrnome";
		
		$dados = $this->carregar( $sql );
		
		return ( $dados ? $dados : array() );
	}	
	
	public function pegaLicidPorObrid( $obrid ){
		$sql = "SELECT
					licid
				FROM
					obras2.obralicitacao
				WHERE
					oblstatus = 'A' AND
					obrid = {$obrid}";
		
		$licid = $this->pegaUm( $sql );
		return ($licid ? $licid : '');
	}
	
	public function pegaOblidPorObrid( $obrid ){
		$sql = "SELECT
					oblid
				FROM
					obras2.obralicitacao
				WHERE
					oblstatus = 'A' AND
					obrid = {$obrid}";
		
		$oblid = $this->pegaUm( $sql );
		return ($oblid ? $oblid : '');
	}
}
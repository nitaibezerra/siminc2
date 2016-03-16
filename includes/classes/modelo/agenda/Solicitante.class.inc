<?php
	
class Solicitante extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.solicitante";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sltid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sltid' => null, 
									  	'usucpf' => null, 
									  	'sltdsc' => null, 
									  	'sltdddfone' => null, 
									  	'sltfone' => null, 
									  	'sltdddcelular' => null, 
									  	'sltcelular' => null, 
									  	'sltexpressao' => null, 
									  	'sltcargo' => null, 
									  	'sltemail' => null, 
									  	'sltobs' => null, 
									  	'sltdtinclusao' => null, 
									  	'sltstatus' => null, 
									  );

	public function listaComboPopupSQL(){
		$sql = "SELECT 
				  	sltid as codigo,
				  	COALESCE(sltcargo, '') || ' ' || COALESCE(sltexpressao, '') || ' ' || sltdsc as descricao
				FROM 
				  	agenda.solicitante
				WHERE
					sltstatus = 'A'";									  
		
		return $sql;
	}

	public function carregaCampoPopup( $sltid ){
		if ( empty($sltid) ){
			return array();
		}	
		$sql = "SELECT 
					sltid AS codigo,
					sltid AS value,
					COALESCE(sltcargo, '') || ' ' || COALESCE(sltexpressao, '') || ' ' || sltdsc AS descricao
				FROM
					agenda.solicitante
				WHERE
					sltid = {$sltid}";
	
		$dados = $this->pegaLinha( $sql );
		
		return $dados;
	}
	
	public function carregaComboPopupBySltid( Array $sltid = array() ){
		if ( empty($sltid[0]) ){
			return array();
		}	
		$sql = "SELECT 
					sltid AS codigo,
					COALESCE(sltexpressao, '') || ' ' || sltdsc AS descricao
				FROM
					agenda.solicitante
				WHERE
					sltid IN( " . implode(", ", $sltid) . " )";
	
		$dados = $this->carregar( $sql );
		
		return ($dados ? $dados : array());
	}
}
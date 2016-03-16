<?php
	
class ContatosEvento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.contatosevento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "cevid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'cevid' => null, 
									  	'muncod' => null, 
									  	'usucpf' => null, 
									  	'cevdsc' => null, 
									  	'cevexpressao' => null, 
									  	'cevcargo' => null, 
									  	'cevlogradouro' => null, 
									  	'cevcomplemento' => null, 
									  	'cevnumero' => null, 
									  	'cevcep' => null, 
									  	'cevdddfone' => null, 
									  	'cevfone' => null, 
									  	'cevdddcelular' => null, 
									  	'cevcelular' => null, 
									  	'cevobs' => null, 
									  	'cevemail' => null, 
									  	'cevdtinclusao' => null, 
									  	'cevstatus' => null, 
									  );

	public function listaComboPopupSQL( Array $where = array() ){
		
		$arWhere = array();
//		switch (true){
//			case isset($where['sltid']):
//				$arWhere[] = "se.sltid = {$where['sltid']}"; 
//				break;
//		}
		
		$sql = "SELECT 
				  	cevid AS codigo,
				  	COALESCE(cevcargo, '') || ' ' || COALESCE(cevexpressao, '') || ' ' || cevdsc AS descricao
				FROM 
				  	agenda.contatosevento
				WHERE
					cevstatus = 'A'" . 
				(count( $arWhere ) ? " AND " . implode(" AND ", $arWhere) : "");									  
		
		return $sql;
	}								  

	public function carregaComboPopup( $eveid ){
		if ( empty($eveid) ){
			return array();
		}
			
		$sql = "SELECT 
					ce.cevid AS codigo,
					COALESCE(cevcargo, '') || ' ' || COALESCE(cevexpressao, '') || ' ' || cevdsc AS descricao
				FROM
					agenda.eventocontato ec
				JOIN agenda.contatosevento ce ON ce.cevid = ec.cevid AND
											     ce.cevstatus = 'A'
				WHERE
					ec.eveid = {$eveid}";
	
		$dados = $this->carregar( $sql );
		
		return $dados;
	}
	
	public function carregaComboPopupByCevid( Array $cevid = array() ){
		if ( empty($cevid[0]) ){
			return array();
		}	
		$sql = "SELECT 
					cevid AS codigo,
					cevcargo ||' '|| cevexpressao ||' '|| cevdsc AS descricao
				FROM
					agenda.contatosevento
				WHERE
					cevid IN( " . implode(", ", $cevid) . " )
				order by 2";
	
		$dados = $this->carregar( $sql );
		
		return ($dados ? $dados : array());
	}
	
}
<?php
	
class Participantes extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.participantes";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "prtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'prtid' => null, 
									  	'muncod' => null, 
									  	'usucpf' => null, 
									  	'prtdsc' => null, 
									  	'prtdddfonecontato' => null, 
									  	'prtfonecontato' => null, 
									  	'prtemailcontato' => null, 
									  	'prtdddcelularcontato' => null, 
									  	'prtcelularcontato' => null, 
									  	'prtdtinclusao' => null, 
									  	'prtstatus' => null, 
									  );
									  
	public function listaComboPopupSQL( Array $where = array() ){
		
//		$arWhere = array();
//		switch (true){
//			case isset($where['sltid']):
//				$arWhere[] = "se.sltid = {$where['sltid']}"; 
//				break;
//		}
		
		$sql = "SELECT 
				  	prtid AS codigo,
				  	prtdsc AS descricao
				FROM 
				  	agenda.participantes
				WHERE
					prtstatus = 'A'" . 
				(count( $arWhere ) ? " AND " . implode(" AND ", $arWhere) : "");									  
		
		return $sql;
	}								  
									  
	public function carregaComboPopup( $eveid ){
		if ( empty($eveid) ){
			return array();
		}
			
		$sql = "SELECT 
					p.prtid AS codigo,
					p.prtdsc AS descricao
				FROM
					agenda.eventoparticipante ep
				JOIN agenda.participantes p ON p.prtid = ep.prtid AND
											   p.prtstatus = 'A'
				WHERE
					ep.eveid = {$eveid}";
	
		$dados = $this->carregar( $sql );
		
		return $dados;
	}
	
}
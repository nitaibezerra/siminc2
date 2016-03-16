<?php
	
class EntidadeAgenda extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.entidadeagenda";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "etaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'etaid' => null, 
									  	'muncod' => null, 
									  	'usucpf' => null, 
									  	'etadsc' => null, 
									  	'etalogradouro' => null, 
									  	'etalogracomplemento' => null, 
									  	'etalogranumero' => null, 
									  	'etacep' => null, 
									  	'etadddfone' => null, 
									  	'etafone' => null, 
									  	'etadddcelular' => null, 
									  	'etacelular' => null, 
									  	'etaobs' => null, 
									  	'etaemail' => null, 
									  	'etadtinclusao' => null, 
									  	'etastatus' => null, 
									  );
									  
	public function listaComboPopupSQL( Array $where = array() ){
		
		$arWhere = array();
		$arJoin  = array();
		switch (true){
			case isset($where['sltid']):
				$arWhere[] = "se.sltid = {$where['sltid']}"; 
				break;
		}
		
		if ( isset($where['sltid']) ){
			$arJoin[] = "agenda.solicitanteentidade se ON se.etaid = ea.etaid";
		}
		
		$sql = "SELECT 
				  	ea.etaid as codigo,
				  	ea.etadsc as descricao
				FROM 
				  	agenda.entidadeagenda ea " . 
				(count( $arJoin ) ? " JOIN " . implode(" JOIN ", $arJoin) : '') . "
				WHERE
					ea.etastatus = 'A'" . 
				(count( $arWhere ) ? " AND " . implode(" AND ", $arWhere) : "");									  
//		dbg($sql, d);
		return $sql;
	}								  

	public function carregaCampoPopup( $etaid ){
		if ( empty($etaid) ){
			return array();
		}	
		$sql = "SELECT 
					etaid AS codigo,
					etaid AS value,
					etadsc AS descricao
				FROM
					agenda.entidadeagenda
				WHERE
					etaid = {$etaid}";
	
		$dados = $this->pegaLinha( $sql );
		
		return $dados;
	}
	
	public function carregaComboPopupByEtaid( Array $etaid = array() ){
		if ( empty($etaid[0]) ){
			return array();
		}	
		$sql = "SELECT 
					etaid AS codigo,
					etadsc AS descricao
				FROM
					agenda.entidadeagenda
				WHERE
					etaid IN( " . implode(", ", $etaid) . " )";
	
		$dados = $this->carregar( $sql );
		
		return ($dados ? $dados : array());
	}
	
}
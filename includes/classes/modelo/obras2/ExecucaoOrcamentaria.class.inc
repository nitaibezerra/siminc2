<?php
	
class ExecucaoOrcamentaria extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.execucaoorcamentaria";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "eorid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'eorid' => null, 
									  	'teoid' => null, 
									  	'empid' => null, 
									  	'obrid' => null, 
									  	'usucpf' => null, 
									  	'eorvlrcusteio' => null, 
									  	'eorvlrcapital' => null, 
									  	'eordtinclusao' => null, 
									  	'eorstatus' => null, 
									  	'eorid_1' => null
									  );

	public function carregaPorEmpid( $empid, Array $param = array() ){
		$arWhere = array();
		
		if ( !empty($param['teoid']) ){
			$param['teoid'] = (array) $param['teoid'];
			$arWhere[] 		 = "teoid IN('" . implode("', '", $param['teoid']) . "')";
		}
		
		$sql = "SELECT 
					*
				FROM
					obras2.execucaoorcamentaria
				WHERE
					eorstatus = 'A' AND
					empid = {$empid}" . 
					(count($arWhere) ? " AND " . implode(' AND ',$arWhere) : "");
		
		$arResultado = $this->pegaLinha( $sql );
		$arResultado = ($arResultado ? $arResultado : array());
		$this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
		return $arResultado;
	}
	
	public function carregaPorObrid( $obrid, Array $param = array() ){
		$arWhere = array();
		
		if ( !empty($param['teoid']) ){
			$param['teoid'] = (array) $param['teoid'];
			$arWhere[] 		= "teoid IN('" . implode("', '", $param['teoid']) . "')";
		}
		
		$sql = "SELECT 
					*
				FROM
					obras2.execucaoorcamentaria
				WHERE
					eorstatus = 'A' AND
					obrid = {$obrid}" . 
					(count($arWhere) ? " AND " . implode(' AND ',$arWhere) : "");
										
		$arResultado = $this->pegaLinha( $sql );
		$arResultado = ($arResultado ? $arResultado : array());
		$this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
		return $arResultado;
	}
}
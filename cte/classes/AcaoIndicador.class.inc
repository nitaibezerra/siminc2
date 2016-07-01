<?php
	
class AcaoIndicador extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.acaoindicador";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "aciid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'aciid' => null, 
									  	'ptoid' => null, 
									  	'parid' => null, 
									  	'acidsc' => null, 
									  	'acirpns' => null, 
									  	'acicrg' => null, 
									  	'acidtinicial' => null, 
									  	'acidtfinal' => null, 
									  	'acirstd' => null, 
									  	'acilocalizador' => null, 
									  	'acidata' => null, 
									  	'usucpf' => null, 
									  	'aciparecer' => null, 
									  	'ppaid' => null, 
									  );
									  
	public function recuperarAcoesPorPpaid( $arPpaid, $inuid = null ){
									
		$inuid = $inuid ? $inuid : $_SESSION["inuid"];
		
		if( !is_array( $arPpaid ) ){
			$arPpaid = explode( ", ", $arPpaid ? $arPpaid : 0 );
		}
		
		$sql = "select distinct ppaid, aciid
				from cte.acaoindicador ai 
					INNER JOIN cte.pontuacao p on p.ptoid = ai.ptoid
				where ppaid in ( '". implode( "', '", $arPpaid ) ."' )
				and p.inuid = $inuid
				and p.ptostatus = 'A'"; 
		//dbg($sql);
		$resultado = $this->carregar( $sql );
		return $resultado ? $resultado : array();
	}									  
									  
}
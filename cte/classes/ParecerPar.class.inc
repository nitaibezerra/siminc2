<?php
	
class ParecerPar extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.parecerpar";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "parid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'parid' => null, 
									  	'usucpf' => null, 
									  	'tppid' => null, 
									  	'pardata' => null, 
									  	'partexto' => null, 
									  	'parhistorico' => null,
    									'inuid'			=> null
									  );
	
	public function recuperaDadosPorID($inuid, $tppid){
		$sql= "	SELECT 	pp.parid, 
						pp.partexto, 
						pp.parhistorico, 
						pp.tppid, 
						to_char(pp.pardata, 'DD/MM/YYYY') as pardata
				FROM cte.parecerpar pp
				WHERE pp.inuid =".$inuid." AND pp.tppid = ".$tppid."
				ORDER BY pp.parhistorico DESC LIMIT 1";
		return $this->carregar($sql);
	}
	
	public function recuperaMaiorIdHistorico($inuid, $tppid){
		$sql = "SELECT parhistorico 
				FROM cte.parecerpar 
				WHERE inuid = ".$inuid." AND tppid = ".$tppid." 
				ORDER BY parhistorico DESC LIMIT 1;";
		$parhistorico =  $this->pegaUm($sql);
		if($parhistorico){
			$parhistorico = $parhistorico + 1;
		}else{
			$parhistorico = 1;
		}
		return $parhistorico;
	}
	
	public function recuperaPorID($parid){
		$sql= "	SELECT  pp.partexto, 
						pp.parhistorico, 
						pp.tppid, 
						to_char(pp.pardata, 'DD/MM/YYYY') as pardata
				FROM cte.parecerpar pp
				WHERE pp.parid =".$parid."
				ORDER BY pp.parhistorico ASC LIMIT 1";
		return $this->carregar($sql);
	}
}
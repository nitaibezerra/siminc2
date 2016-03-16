<?php
	
class PlanoInterno extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "rehuf.planointerno";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pliid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pliid' => null,
    									'plinumero' => null, 
    									'plidtinicio' => null,
    									'plidttermino' => null,
									  );
									  
		public function validarPI($plinumero,$pliid){
			$sql .= "SELECT"; 
			$sql .=	" plinumero";
			$sql .=	" FROM";
			$sql .=	" rehuf.planointerno";
			$sql .= " WHERE"; 
			$sql .=  " plinumero = '" .$plinumero."'";
			if($pliid){
				$sql .= " AND";
				$sql .= " pliid <> " .$pliid;
			}
			
				$retorno = $this->PegaUm($sql); 
		return $retorno;
			
		
		}
		
}
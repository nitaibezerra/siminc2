<?php

class MembroComissao extends Modelo 
{
	/*
	 * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.instrumento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "itrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'itrid' => null, 
									  	'itrdsc' => null, 
									  ); 
									  
	public function recuperarIntrumentosGuia()
	{
		$sql = "SELECT DISTINCT 
					it.itrid, 
					it.itrdsc
			    FROM cte.instrumento it    
			    WHERE it.itrid IN ( 1, 2 )
			    --WHERE it.itrid = 1
			    ORDER BY it.itrdsc
			    ";
		
		return $this->carregar($sql);
	}
	
	
}
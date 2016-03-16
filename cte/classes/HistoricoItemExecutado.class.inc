<?php 

class HistoricoItemExecutado extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.historicoitemexecutado";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hieid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'hieid' => null, 
									  	'hmsid' => null, 
									  	'sbaid' => null, 
									  	'hievalorunitario' => null, 
									  	'hieqtd' => null, 
									  	'hievalortotal' => null, 
									  	'hiejustificativa' => null, 
									  	'hiedata' => null, 
									  	'hiedescricao' => null,
    									'unddid' => null, 
									  );
									  
									  
									  
}
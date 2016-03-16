<?php
	
class MeMaisEducacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pdeescola.memaiseducacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "memid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'memid' => null, 
									  	'entid' => null, 
									  	'docid' => null, 
									  	'mesid' => null, 
									  	'memdataatualizacao' => null, 
									  	'memdataaprovacao' => null, 
									  	'memvlrmonitor' => null, 
									  	'memvlrservico' => null, 
									  	'memvlrtotalkit' => null, 
									  	'memanoreferencia' => null, 
									  	'memvlrprimeiraparcela' => null, 
    									'memvlrpago' => null,
									  	'memvlrsuplementar' => null, 
									  	'memvlrsaldodisponivel' => null, 
									  	'memvlrsaldosuplementar' => null, 
									  	'entcodent' => null, 
									  	'memmodalidadeensino' => null,
									  );
}
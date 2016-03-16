<?php
	
class FisicoFinanceiro extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.fisicofinanceiro";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "fsfid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'fsfid' => null, 
									  	'prcid' => null, 
									  	'fsfdsc' => null, 
									  	'fsfprogramado' => null, 
									  	'fsfexecutado' => null, 
									  	'fsfvlrconcedente' => null,
    									'fsfvlrexecutor' => null, 
									  	'fsfvlrtotal' => null, 
									  	'fsfstatus' => null, 
									  	'fsfdtinclusao' => null, 
									  );
									  
}
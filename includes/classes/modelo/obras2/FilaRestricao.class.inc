<?php
	
class FilaRestricao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.filarestricao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "frtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'frtid' => null, 
									  	'tprid' => null, 
									  	'fsrid' => null, 
									  	'empid' => null, 
									  	'usucpf' => null, 
									  	'usucpfdeferimento' => null, 
									  	'frtdsc' => null, 
									  	'frtdtprevisaoregularizacao' => null, 
									  	'frtdtdeferimento' => null, 
									  	'frtdscprovidencia' => null, 
									  	'frtsituacao' => null, 
									  	'frtdtinclusao' => null, 
									  	'frtstatus' => null, 
									  	'obrid' => null, 
									  	'frtitem' => null, 
									  	'rsqid' => null, 
									  	'qtsid' => null, 
									  	'frtdeferimento' => null, 
									  );
}
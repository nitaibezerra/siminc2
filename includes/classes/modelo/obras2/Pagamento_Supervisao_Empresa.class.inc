<?php
	
class Pagamento_Supervisao_Empresa extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.pagamento_supervisao_empresa";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pseid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pseid' => null, 
									  	'pagid' => null, 
									  	'sueid' => null,
									  );
}

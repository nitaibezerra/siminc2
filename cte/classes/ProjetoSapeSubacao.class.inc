<?php
include_once(APPRAIZ .'cte/classes/RegrasFinanceiro.class.inc');	
class ProjetoSapeSubacao extends Regrasfinanceiro{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.projetosapesubacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pssid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pssid' => null, 
									  	'pssdata' => null, 
									  	'pssano' => null, 
									  	'prsid' => null, 
									  	'sbaid' => null
									  );
}
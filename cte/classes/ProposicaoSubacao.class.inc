<?php
	
class ProposicaoSubacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.proposicaosubacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ppsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ppsid' => null, 
									  	'ppaid' => null, 
									  	'undid' => null, 
									  	'frmid' => null, 
									  	'ppsdsc' => null, 
									  	'ppsmetodologia' => null, 
									  	'ppsprograma' => null, 
									  	'prgid' => null, 
									  	'ppsordem' => null, 
									  	'ppsobjetivo' => null, 
									  	'ppstexto' => null, 
									  	'ppsrel' => null, 
									  	'ppsobra' => null, 
									  	'ppsvaluni' => null, 
									  	'ppspeso' => null, 
									  	'ppsmonitoramento' => null, 
									  	'ppsindcobuni' => null, 
									  	'indqtdporescola' => null, 
									  	'indid' => null, 
									  	'crtid' => null, 
									  	'ppsparecerpadrao' => null
									  );
}
<?php
	
class Infraestrutura extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.infraestrutura";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "iexid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'iexid' => null, 
									  	'umdidareaconstruida' => null, 
									  	'umdidareareforma' => null, 
									  	'umdidareaampliada' => null, 
									  	'iexsitdominialimovelregulariza' => null, 
									  	'iexinfexistedimovel' => null, 
									  	'iexareaconstruida' => null, 
									  	'iexdescsumariaedificacao' => null, 
									  	'iexedificacaoreforma' => null, 
									  	'iexqtdareapreforma' => null, 
									  	'iexvlrareapreforma' => null, 
									  	'iexampliacao' => null, 
									  	'iexqtdareaampliada' => null, 
									  	'iexvlrareaampliada' => null, 
									  	'aqiid' => null, 
									  );
}
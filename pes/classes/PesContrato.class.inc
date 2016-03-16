<?php
	
class PesContrato extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pes.pescontrato";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("concodigo");

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'concodigo' => null, 
									  	'entcodigo' => null, 
									  	'tidcodigo' => null, 
									  	'contitulo' => null, 
									  	'connumero' => null, 
									  	'conobservacao' => null, 
									  	'conusunome' => null, 
									  	'conano' => null, 
									  	'coniniciovigencia' => null, 
									  	'confimvigencia' => null, 
									  	'condatarepactuacao' => null, 
									  	'convalor' => null, 
									  	'condatacriacao' => null, 
									  	'conusucpfcriacao' => null, 
									  	'consissiglacriacao' => null, 
									  	'condataalteracao' => null, 
									  	'conusucpfalteracao' => null, 
									  	'consissiglaalteracao' => null, 
									  	'conendereco' => null, 
									  	'conarea' => null, 
									  	'concomplemento' => null, 
									  	'consnexclusaologica' => null, 
									  );
}
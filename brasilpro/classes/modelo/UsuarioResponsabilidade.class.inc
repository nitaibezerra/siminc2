<?php
	
class UsuarioResponsabilidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.usuarioresponsabilidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rpuid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'rpuid' => null, 
									  	'pflcod' => null, 
									  	'usucpf' => null, 
									  	'rpustatus' => null, 
									  	'rpudata_inc' => null, 
									  );
}
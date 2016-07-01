<?php
	
class Meses extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "public.meses";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "mescod" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'mescod' => null, 
									  	'mesdsc' => null
									  );
    
    /**
     * Recupera Lista de Registros
     * @author Rondomar França
     * @access public
     * @name pegarDados
     * return array
     */
    public function pegarDados(){
    
    	$sql = "SELECT * FROM $this->stNomeTabela";
    
    	return $this->carregar( $sql );
    }
}
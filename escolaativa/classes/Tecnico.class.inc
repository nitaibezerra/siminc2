<?php
	
class Tecnico extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "escolaativa.tecnico";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tecid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tecid' => null, 
									  	'entid' => null, 
									  	'estuf' => null, 
									  	'muncod' => null, 
									  	'fudid' => null, 
									  );
									  
	public function pesquisaTecnicoPorEntid( $entid ){
		$sql = "select tecid from escolaativa.tecnico where entid = '$entid'";
		return $this->pegaUm($sql);
	}
	
}
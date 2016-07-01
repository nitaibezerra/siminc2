<?php
	
class CoUasg extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "evento.uasg";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "usgid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'usgid' 	=> null, 
									  	'usgdsc' 	=> null, 
									  	'usgcod' 	=> null, 
									  	'entid' 	=> null, 
									  	'usggestaouasg' => null, 
									  );
									  
	public function insereUasgContratacao( $conid, $usgid ){
	    $sql = "INSERT INTO evento.couasgnivelcontratacao ( conid, usgid ) VALUES ( '$conid', '$usgid' )";
	    if( !$this->executar( $sql )){
	        return false;
	    }   
	}
	
	public function deletaUasgContratacao( $usgid ){
		$sql = "DELETE FROM evento.couasgnivelcontratacao WHERE usgid = '$usgid'";
		if ( !$this->executar($sql)){
			return false;
		}
	}
}
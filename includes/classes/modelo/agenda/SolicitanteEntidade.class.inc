<?php
	
class SolicitanteEntidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.solicitanteentidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sltid", "etaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sltid' => null, 
									  	'etaid' => null, 
									  );

	public function salvar(){
		$sql = "INSERT INTO agenda.solicitanteentidade
					(sltid, etaid)
    			VALUES 
    				({$this->sltid}, {$this->etaid});";
		
		$this->executar( $sql );
	}									  
									  
}
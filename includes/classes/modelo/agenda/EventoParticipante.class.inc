<?php
	
class EventoParticipante extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.eventoparticipante";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "eveid", "prtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'eveid' => null, 
									  	'prtid' => null, 
									  );

	public function salvar(){
		$sql = "INSERT INTO agenda.eventoparticipante
					(eveid, prtid)
    			VALUES 
    				({$this->eveid}, {$this->prtid});";
		
		$this->executar( $sql );
	}									  
									  
	public function deletaTodosPorEvento( $eveid ){
		$sql = "DELETE FROM agenda.eventoparticipante WHERE eveid = {$eveid}";
		
		$this->executar( $sql );
	}
}
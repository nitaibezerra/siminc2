<?php
	
class EventoContato extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.eventocontato";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "evcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'evcid' => null, 
									  	'cevid' => null, 
									  	'eveid' => null, 
									  );

	public function deletaTodosPorEvento( $eveid ){
		$sql = "DELETE FROM agenda.eventocontato WHERE eveid = {$eveid}";
		
		$this->executar( $sql );
	}

	public function listaPorEvento( $eveid ){
		$sql = "SELECT 
				  	cevcargo,
				  	COALESCE(cevexpressao, '') || ' ' || cevdsc AS cevdsc,
				  	cevemail,
				  	'(' || COALESCE(cevdddfone, '') || ') ' || COALESCE(cevfone) AS fone,
				  	'(' || COALESCE(cevdddcelular, '') || ') ' || COALESCE(cevcelular) AS cel
				FROM 
				  	agenda.eventocontato ec
				JOIN agenda.contatosevento ce ON ce.cevid = ec.cevid AND
												 ce.cevstatus = 'A'
				WHERE
					ce.cevstatus = 'A' AND
					ec.eveid = {$eveid}";									  
		
		$dados = $this->carregar( $sql );				
				
		return ($dados ? $dados : array());
	}
}
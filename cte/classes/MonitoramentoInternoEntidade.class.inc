<?php
	
class MonitoramentoInternoEntidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.monitoramentointernoentidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "mneid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'mneid' => null, 
									  	'mniid' => null, 
									  	'entid' => null, 
									  	'scuid' => null, 
									  	'estid' => null, 
									  	'mneqtd' => null, 
									  );
									  
	public function excluirTodosPorMniid( $mniid ){
		
		$sql = " delete from {$this->stNomeTabela} where mniid = $mniid ";
		return $this->executar( $sql );
		 
	}

}
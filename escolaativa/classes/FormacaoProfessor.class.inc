<?php
	
class FormacaoProfessor extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "escolaativa.formacaoprofessor";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "fopid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tecid' => null, 									  	 
									  	'tpcid' => null, 
									  	'fopid' => null, 
									  	'fopinconcluido' => null, 
									  	'fopdtinicio' => null, 
									  	'fopdtconclusao' => null,
    									'fopdsccurso' => null, 
									  );
									  
	function deletarFormacaoPorTecid($tecid){
		
		$sql = "delete from {$this->stNomeTabela} where tecid = '$tecid'";
		if ( !$this->executar($sql)){
			return false;
		}
	}
}
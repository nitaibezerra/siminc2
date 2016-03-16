<?php
	
class ExperienciaProfessor extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "escolaativa.experienciaprofessor";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "expid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'expid' => null, 
									  	'tecid' => null, 
									  	'expinstituicao' => null, 
									  	'expcargo' => null, 
									  	'expdtadmissao' => null, 
									  );
									  
	function deletarExperienciaPorTecid($tecid){
		
		$sql = "delete from {$this->stNomeTabela} where tecid = '$tecid'";
		if ( !$this->executar($sql)){
			return false;
		}
	}
	
}
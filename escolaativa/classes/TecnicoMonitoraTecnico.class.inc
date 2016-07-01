<?php
	
class TecnicoMonitoraTecnico extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "escolaativa.tecnicomonitoratecnico";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tmtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tmtid' => null, 
									  	'tecidsuper' => null, 
									  	'tecidmonitorado' => null, 
									  );
									  
	public function excluiProfessorPorTecidMonitorado($tecidmonitorado, $tecidsuper){
		if($tecidmonitorado && $tecidsuper){
			$sql = " delete from {$this->stNomeTabela} where tecidsuper = $tecidsuper and tecidmonitorado = $tecidmonitorado ";
			if ( !$this->executar($sql)){
				return false;
			}
		}
	}
	
	public function excluiProfessorPorTecidSuper($tecid){
		if($tecid){
			$sql = " delete from {$this->stNomeTabela} where tecidsuper = $tecid ";
			if ( !$this->executar($sql)){
				return false;
			}
		}
	}
	
}
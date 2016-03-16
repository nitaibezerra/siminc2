<?php
	
class TecnicoResponsabilidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "escolaativa.tecnicoresponsabilidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( 'muncod' );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tecid' => null, 
									  	'estuf' => null, 
									  	'muncod' => null, 
									  );

	protected $tabelaAssociativa = true;
									  
	public function excluiResponsabilidadePorTecid($tecid){
		$sql = "delete from {$this->stNomeTabela} where tecid = '$tecid'";
		if ( !$this->executar($sql)){
			return false;
		}
	}
}
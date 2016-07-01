<?php
	
class ComposicaoPessoa extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.composicaopessoa";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "cspid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'cspid' => null, 
									  	'entid' => null, 
									  	'sbaid' => null, 
									  	'cspano' => null, 
									  );
	public function excluirPorEntid( $entid ){
		$sql = " DELETE FROM $this->stNomeTabela WHERE entid = $entid; ";
		
		return $this->executar( $sql );
	}
}
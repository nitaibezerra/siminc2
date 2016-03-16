<?php
	
class Supervisao_Grupo_Mesoregiao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.supervisao_grupo_mesoregiao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sgmid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sgmid' => null, 
									  	'sgrid' => null, 
									  	'mescod' => null, 
									  	'sgmqtda' => null, 
									  	'sgmqtdb' => null, 
									  	'sgmqtdc' => null, 
									  	'sgmstatus' => null, 
									  );

	public function pegaMescodPorSgrid( $sgrid ){
		
		$sql = "SELECT
					mescod
				FROM
					obras2.supervisao_grupo_mesoregiao
				WHERE
					sgrid = {$sgrid}";
		
		$dados = $this->carregarColuna( $sql );
		
		return ($dados ? $dados : array());
	}
}
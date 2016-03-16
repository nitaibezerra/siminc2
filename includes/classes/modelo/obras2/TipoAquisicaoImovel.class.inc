<?php
	
class TipoAquisicaoImovel extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tipoaquisicaoimovel";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "aqiid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'aqiid' => null, 
									  	'aqidsc' => null, 
									  );

	public function listaCombo( Array $param = array() ){
    	
    	$sql = "SELECT 
					aqiid as codigo,
  					aqidsc as descricao
				FROM 
					obras2.tipoaquisicaoimovel;";
    	$dados = $this->carregar($sql);
    	
    	return (is_array($dados) ? $dados : array());
    }
}
<?php
	
class DemandaClassificacaoObra extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.demanda_classificacaoobra";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dclid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'dclid' => null, 
									  	'dcldesc' => null, 
									  	'dclstatus' => null, 
									  );

    public function listaCombo(){
    	$sql = "SELECT DISTINCT
					dclid as codigo, 
					dcldesc as descricao
				FROM 
					obras2.demanda_classificacaoobra
				WHERE
					dclstatus = 'A'
				ORDER BY
					2";
    
    	$dados = $this->carregar($sql);
    
    	return (is_array($dados) ? $dados : array());
    }
}
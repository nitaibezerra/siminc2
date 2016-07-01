<?php
	
class DemandaSituacaoProjeto extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.demanda_situacaoprojeto";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dspid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'dspid' => null, 
									  	'dspdesc' => null, 
									  	'dspstatus' => null, 
									  );

    public function listaCombo(){
    	$sql = "SELECT DISTINCT
					dspid as codigo, 
					dspdesc as descricao
				FROM 
					obras2.demanda_situacaoprojeto
				WHERE
					dspstatus = 'A'
				ORDER BY
					2";
    
    	$dados = $this->carregar($sql);
    
    	return (is_array($dados) ? $dados : array());
    }
}
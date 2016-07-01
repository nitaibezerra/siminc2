<?php
	
class DemandaTipoObra extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.demanda_tipoobra";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dtoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'dtoid' => null, 
									  	'dtodesc' => null, 
									  	'dtostatus' => null, 
									  );

    public function listaCombo(){
    	$sql = "SELECT 
					dtoid as codigo, 
					dtodesc as descricao 
				FROM 
					obras2.demanda_tipoobra
				WHERE
					dtostatus = 'A'
				ORDER BY
					2";
    
    	$dados = $this->carregar($sql);
    
    	return (is_array($dados) ? $dados : array());
    }
}
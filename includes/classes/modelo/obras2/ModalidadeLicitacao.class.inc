<?php
	
class ModalidadeLicitacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.modalidadelicitacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "molid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'molid' => null, 
									  	'moldsc' => null, 
									  	'moldias' => null, 
									  );

	public function listaCombo(){
		$sql = "SELECT 
			   		molid AS codigo, 
			   		moldsc AS descricao 
			    FROM 
					obras2.modalidadelicitacao
			    ORDER BY 
					moldsc";
		$dados = $this->carregar($sql);
		
		return ($dados ? $dados : array());
	}
}
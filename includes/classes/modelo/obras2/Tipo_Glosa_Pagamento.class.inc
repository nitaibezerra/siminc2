<?php
	
class Tipo_Glosa_Pagamento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tipo_glosa_pagamento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tgpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tgpid' => null, 
									  	'tgpdsc' => null, 
									  	'tgpstatus' => null, 
									  );

	public function listaCombo(){
				
		$sql = "
	 			SELECT 
			   		tgpid AS codigo, 
			   		tgpdsc as descricao 
			    FROM 
					$this->stNomeTabela
				WHERE
					tgpstatus='A'
			    ORDER BY 
					descricao";
		$dados = $this->carregar($sql);

		return ($dados ? $dados : array());
	}
}
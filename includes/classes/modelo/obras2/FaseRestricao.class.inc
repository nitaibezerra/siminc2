<?php
	
class FaseRestricao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.faserestricao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "fsrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
                                        'fsrid' => null, 
                                        'fsrdsc' => null, 
                                        'fsrstatus' => null, 
                                      );

	public function listaCombo(){
		$sql = "SELECT
					fsrid AS codigo,
					fsrdsc AS descricao
				FROM
					obras2.faserestricao
				WHERE
					fsrstatus = 'A'
				ORDER BY
					fsrdsc";

		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}
									  
}
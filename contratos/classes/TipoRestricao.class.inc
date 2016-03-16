<?php
	
class TipoRestricao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "contratos.tiporestricao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tprid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tprid' => null, 
									  	'tprdsc' => null, 
									  	'tprdtinclusao' => null, 
									  	'tprstatus' => null, 
									  );

	public function listaCombo(){
		$sql = "SELECT
					tprid AS codigo,
					tprdsc AS descricao
				FROM
					contratos.tiporestricao
				WHERE
					tprstatus = 'A'
				ORDER BY
					tprdsc";

		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}
									  
}
<?php
	
class TipoEvento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.tipoevento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tpeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tpeid' => null, 
									  	'tpedsc' => null, 
									  );

	public function listaCombo(){
		
		$sql = "SELECT
					tpeid AS codigo,
					tpedsc AS descricao
				FROM
					agenda.tipoevento"; 
		
		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}
									  
}
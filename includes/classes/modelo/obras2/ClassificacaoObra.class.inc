<?php
	
class ClassificacaoObra extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.classificacaoobra";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "cloid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'cloid' => null, 
									  	'clodsc' => null, 
									  );

	public function listaCombo(){
		$sql = "SELECT
					cloid AS codigo,
					clodsc AS descricao
				FROM
					obras2.classificacaoobra";

		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}

    public function listaComboSql(){
        $sql = "SELECT
					cloid AS codigo,
					clodsc AS descricao
				FROM
					obras2.classificacaoobra";

        return $sql;
    }
}
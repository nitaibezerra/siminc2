<?php
	
class ModalidadeEnsino extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.modalidadeensino";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "moeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'moeid' => null, 
									  	'moedsc' => null, 
									  	'moestatus' => null, 
									  );

	public function listaCombo(){
		$sql = "SELECT
					moeid AS codigo,
					moedsc AS descricao
				FROM
					obras2.modalidadeensino
				WHERE
					moestatus = 'A'";

		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}

    public function listaComboSql(){
        $sql = "SELECT
					moeid AS codigo,
					moedsc AS descricao
				FROM
					obras2.modalidadeensino
				WHERE
					moestatus = 'A'";
        return $sql;
    }
}
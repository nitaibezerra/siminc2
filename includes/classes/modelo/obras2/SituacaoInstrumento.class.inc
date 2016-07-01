<?php
	
class SituacaoInstrumento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.situacaoinstrumento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "stiid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'stiid' => null,
									  	'stidesc' => null,
									  	'stidtinclusao' => null,
									  	'stistatus' => null,
									  );

    public function listaCombo(){
        $sql = "SELECT
			    	stiid AS codigo,
			    	stidesc AS descricao
		    	FROM
		    		obras2.situacaoinstrumento
		    	WHERE
		    		stistatus = 'A'
		    	ORDER BY
					stidesc ASC";

        $dados = $this->carregar($sql);

        return (is_array($dados) ? $dados : array());
    }
    public function listaComboSql(){
        $sql = "SELECT
			    	stiid AS codigo,
			    	stidesc AS descricao
		    	FROM
		    		obras2.situacaoinstrumento
		    	WHERE
		    		stistatus = 'A'
		    	ORDER BY
					stidesc ASC";



        return $sql;
    }

}
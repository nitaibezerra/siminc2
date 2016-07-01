<?php 
	
class RealizacaoSupervisao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.realizacaosupervisao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rsuid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'rsuid' => null, 
									  	'rsudsc' => null
									  );
    
    public function listaCombo(){
    	$sql = "SELECT
			    	rsuid AS codigo,
			    	rsudsc AS descricao
		    	FROM
		    		obras2.realizacaosupervisao";
    
    	$dados = $this->carregar($sql);
    
    	return (is_array($dados) ? $dados : array());
    }
    public function listaComboSql(){
        $sql = "SELECT
			    	rsuid AS codigo,
			    	rsudsc AS descricao
		    	FROM
		    		obras2.realizacaosupervisao";



        return $sql;
    }
}
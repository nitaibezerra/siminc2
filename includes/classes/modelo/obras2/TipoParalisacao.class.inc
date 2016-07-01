<?php
	
class TipoParalisacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tipoparalisacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tplid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tplid' => null, 
									  	'tpldsc' => null, 
									  	'tpldtinclusao' => null, 
									  	'tplstatus' => null, 
									  );

	public function listaCombo(){
		$sql = "SELECT
					tplid AS codigo,
					tpldsc AS descricao
				FROM
					obras2.tipoparalisacao
				WHERE
					tplstatus = 'A'";
        if(!obraMI($_SESSION['obras2']['obrid'])){
            $sql .= " AND tplid != 5";
        }
		$dados = $this->carregar($sql);
		return (is_array($dados) ? $dados : array());
	}
        
        public function getDesc($id)
        {
            $sql = "SELECT
					
					tpldsc
				FROM
					obras2.tipoparalisacao
				WHERE
					tplstatus = 'A' and tplid= ".$id;
            
           return $this->pegaUm($sql); 
        }
}
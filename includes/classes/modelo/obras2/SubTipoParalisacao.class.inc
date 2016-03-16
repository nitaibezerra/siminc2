<?php
	
class SubTipoParalisacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.subtipoparalisacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "stpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'stpid' => null, 
									  	'tplid' => null, 
									  	'stpdsc' => null, 
									  	'stpstatus' => null, 
									  );

	public function listaCombo( Array $param = array() ){
		$where = array();
		
		if (!empty($param['tplid'])){
			$where[] = "tplid = {$param['tplid']}";
		}
				
		$sql = "SELECT
					stpid AS codigo,
					stpdsc AS descricao
				FROM
					obras2.subtipoparalisacao
				WHERE
					stpstatus = 'A'" . 
				(count($where) ? " AND " . implode(" AND ", $where) : "");
				
		$dados = $this->carregar($sql);
		return (is_array($dados) ? $dados : array());
	}
}
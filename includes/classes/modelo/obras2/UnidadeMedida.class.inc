<?php
	
class UnidadeMedida extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.unidademedida";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "umdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'umdid' => null, 
									  	'umdeesc' => null, 
									  	'umdstatus' => null, 
									  	'umddtinclusao' => null, 
									  );

	public function listaCombo( Array $param = array() ){
		$where = array();
		
//		switch( true ){
//			case !empty($param['orgid']):
//				$where[] = "orgid = {$param['orgid']}";
//				break;
//		}
		
		$sql = "SELECT
					umdid AS codigo,
					umdeesc AS descricao
				FROM
					obras2.unidademedida
				WHERE
					umdstatus = 'A'" . 
				(count($where) ? " AND " . implode(" AND ", $where) : "");

		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}
									  
}
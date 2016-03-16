<?php
	
class ProgramaFonte extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.programafonte";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "prfid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'prfid' => null, 
									  	'orgid' => null, 
									  	'prfdesc' => null, 
									  	'sbaid' => null, 
									  );
	
    public function listaCombo( Array $param = array(), $query = false){
    	$join  = array();
    	$where = array();
    	
    	if (!empty($param['tpoid'])){
    		$join[]  = "INNER JOIN obras.programatipologia ptp ON prf.prfid = ptp.prfid";
    		$where[] = "tpoid = {$param['tpoid']}"; 
    	}

    	if (!empty($param['prfid'])){
    		$where[] = "prfid IN (" . implode(', ', $param['prfid']). ")";
    	}
    	
//    	if (key_exists('tpoid', $param) && empty($param['tpoid'])){
//    		return array();
//    	}
    	
    	if (!empty($param['orgid'])){
			$where[] = "orgid = {$param['orgid']}"; 
    	}
    	
    	$sql = "SELECT DISTINCT
					prf.prfid as codigo, 
					prf.prfdesc as descricao
				FROM 
					obras2.programafonte prf
				" . (count($join) ? implode(" ", $join) : "") . "
				" . (count($where) ? " WHERE " . implode(" AND ", $where) : "");

        if($query)
            return $sql;
        
    	$dados = $this->carregar($sql);
    	return (is_array($dados) ? $dados : array());
    }
}
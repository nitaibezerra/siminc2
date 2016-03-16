<?php
	
class Supervisao_Empenho extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.supervisao_empenho";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "semid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'semid' => null, 
									  	'sgeid' => null, 
									  	'semnumempenho' => null, 
									  	'semdataempenho' => null, 
									  	'semvalor' => null, 
									  	'semstatus' => null, 
									  );

	public function listaCombo( Array $param = array()){
		$arWhere = array();
		
		if ( !empty( $param['sgeid'] ) ){
			$arWhere[] = "sgeid = {$param['sgeid']}";
		}
		
		$where = (count($arWhere) ? " AND " . implode(' AND ',$arWhere) : "");
		$sql = <<<EOD
	 			SELECT 
			   		semid AS codigo, 
			   		semnumempenho AS descricao 
			    FROM 
					obras2.supervisao_empenho
				WHERE
					semstatus='A'
					$where
			    ORDER BY 
					semnumempenho
EOD;
		$dados = $this->carregar($sql);
		
		return ($dados ? $dados : array());
	}
        
	public function listaComboTodos( Array $param = array()){
		
		$where = (count($arWhere) ? " AND " . implode(' AND ',$arWhere) : "");
		$sql = <<<EOD
	 			SELECT 
			   		semid AS codigo, 
			   		semnumempenho AS descricao 
			    FROM 
					obras2.supervisao_empenho
				WHERE
					semstatus='A'
			    ORDER BY 
					semnumempenho
EOD;
		$dados = $this->carregar($sql);
		
		return ($dados ? $dados : array());
	}
        
	public function listaComboPorEmpresa( Array $param = array()){
		$arWhere = array();
		
		if ( !empty( $param['sgrid'] ) ){
			$arWhere[] = "sgr.sgrid = {$param['sgrid']}";
		}
		if ( !empty( $param['entid'] ) ){
			$arWhere[] = "ent.entid = {$param['entid']}";
		}
		
		$where = (count($arWhere) ? " AND " . implode(' AND ',$arWhere) : "");
		$sql = <<<EOD
	 			SELECT 
			   		sem.semid AS codigo, 
			   		sem.semnumempenho AS descricao 
			    FROM 
					obras2.supervisao_grupo sgr
                            JOIN obras2.supervisao_grupo_empresa sge ON sge.sgrid = sgr.sgrid
                            JOIN entidade.entidade ent ON ent.entid = sge.entid
                            JOIN obras2.supervisao_empenho sem ON sem.sgeid = sge.sgeid
				WHERE
					sem.semstatus='A'
					$where
			    ORDER BY 
					semnumempenho
EOD;
		$dados = $this->carregar($sql);
		
		return ($dados ? $dados : array());
	}
									  
}
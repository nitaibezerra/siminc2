<?php
	
class Grupolinha extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.grupolinha";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "grlid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'grlid' => null, 
									  	'tblid' => null, 
									  	'gru_grlid' => null, 
									  	'grldsc' => null, 
									  );
									  
	public function conta( $tabela, $where = null ){
		if( $where ){
			$cond = " AND grldsc <> '$nbsp;'";
		}
		$sql = "SELECT
					count(grlid)
				FROM
					{$this->stNomeTabela}
				WHERE
					tblid = ".$tabela."
					".$cond;
		return $this->pegaUm( $sql );
	}
	
	public function listaObjeto(Array $where = null){
		$where  = (array) $where;
		
		foreach ($where as $k => $item){
			if ( simec_get_class($item) == 'Tabela' ){
				$codicao[] = "tblid = " . $item->tblid;
				$codicao[] = "gru_grlid IS NULL ";
				continue;
			}elseif ( simec_get_class($item) == 'Grupolinha' ){
				$codicao[] = "gru_grlid = " . $item->grlid;
				continue;
			}	
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );
		
		return $this->carregarObjeto($sql, simec_get_class($this));
	}		
}
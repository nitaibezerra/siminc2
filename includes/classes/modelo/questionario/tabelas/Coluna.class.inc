<?php
	
class Coluna extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.coluna";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "colid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'colid' => null, 
									  	'itmid' => null, 
									  	'grcid' => null, 
									  	'collegenda' => null, 
									  	'coltipo' => null,
									  	'colmascara' => null, 
									  	'coltamanhomaximo' => null 
									  );
	public function pegaDados( $tabela ){
		$sql = "(SELECT
					c.colid as codigo,
					i.itmid,
					c.collegenda,
					i.itmdsc as descricao,
					c.coltipo,
					c.colmascara,
					coltamanhomaximo
				FROM
					{$this->stNomeTabela} c
				INNER JOIN questionario.item i ON c.itmid = i.itmid
				INNER JOIN questionario.grupocoluna gc USING (grcid)
				INNER JOIN questionario.tabela t USING (tblid)
				WHERE
					tblid = ".$tabela."
				
				) UNION ALL (
				
				
				SELECT
					c.colid as codigo,
					i.itmid,
					c.collegenda,
					i.itmdsc as descricao,
					c.coltipo,
					c.colmascara,
					coltamanhomaximo
				FROM
					{$this->stNomeTabela} c
				INNER JOIN questionario.item i ON c.itmid = i.itmid
				INNER JOIN questionario.grupocoluna gc USING (grcid)
				INNER JOIN questionario.grupocoluna gc2 ON gc2.grcid = gc.gru_grcid
				INNER JOIN questionario.tabela t ON t.tblid = gc2.tblid AND t.tblid = ".$tabela.") ORDER BY codigo";
		
		/*
		$sql = "SELECT
				c.colid as codigo,
				i.itmdsc as descricao
			FROM
				{$this->stNomeTabela} c
			INNER JOIN questionario.item i ON c.itmid = i.itmid
			INNER JOIN questionario.grupocoluna gc USING (grcid)
			INNER JOIN questionario.tabela t USING (tblid)
			WHERE
				tblid = ".$tabela;
		*/
		return $this->carregar( $sql );
	}
	
	public function conta( $arrGrcid ){
		
		$arrGrcid = (array) $arrGrcid;
		
		$sql = "SELECT
					count(colid)
				FROM
					{$this->stNomeTabela}
				WHERE
					grcid IN (".implode(",", $arrGrcid).")";

					
		return $this->pegaUm( $sql );
	}
	
	public function listaObjeto(Array $where = null){
		$where  = (array) $where;
		
		foreach ($where as $k => $item){
			if ( simec_get_class($item) == 'Grupocoluna' ){
				$codicao[] = "grcid = " . $item->grcid;
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
	
	public function pegaItem( $itmid ){
		
		$sql = "SELECT itmdsc FROM questionario.item WHERE itmid = ".$itmid;
		return $this->pegaUm($sql);
		
	}
}
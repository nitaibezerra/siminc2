<?php
	
class Linha extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.linha";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "linid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'linid' => null, 
									  	'itmid' => null, 
									  	'grlid' => null, 
									  	'lindsc' => null, 
									  );
									  
	public function pegaDados( $tabela ){
		$sql = "SELECT
					l.linid as codigo,
					i.itmdsc as descricao,
					t.tbldescricao as tabela,
					l.grlid as grupoid,
					gl.grldsc as grupolinha,
					gl.gru_grlid as grupopai,
					l.lindsc			
				FROM
					{$this->stNomeTabela} l
				INNER JOIN questionario.item i ON l.itmid = i.itmid
				INNER JOIN questionario.grupolinha gl USING (grlid)
				INNER JOIN questionario.tabela t USING (tblid)
				WHERE
					tblid = {$tabela}
				ORDER BY grupoid, codigo";
		return $this->carregar( $sql );
	}
	
	public function conta( $grupoid ){
		$sql = "SELECT
					count(l.linid)
				FROM
					{$this->stNomeTabela} l
				WHERE
					grlid = ".$grupoid;
		return $this->pegaUm( $sql );
	}
	
	public function listaObjeto(Array $where = null){
		$where  = (array) $where;
		
		foreach ($where as $k => $item){
			if ( simec_get_class($item) == 'Grupolinha' ){
				$codicao[] = "grlid = " . $item->grlid;
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
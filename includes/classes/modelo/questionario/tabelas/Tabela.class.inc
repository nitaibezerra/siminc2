<?php
	
class Tabela extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.tabela";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tblid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tblid' => null, 
									  	'perid' => null, 
									  	'tbldescricao' => null, 
									  	'tbllabelcolunas' => null, 
									  	'tbllabellinhas' => null, 
									  	'tbltipo' => null,
									  	'tblpercent' => null,
									  	'tbltotalizador' => null,
									  );

	public function pegaDados( $perid ){
		$sql = "SELECT 
					tbldescricao as descricao, 
					tbllabelcolunas as colunas, 
					tbllabellinhas as linhas, 
					tblid, 
					tbltipo, 
					tblpercent,
					tbltotalizador
				FROM 
					{$this->stNomeTabela} 
				WHERE 
					perid = ".$perid;
		$dados = $this->pegaLinha( $sql );
		
		if( $dados ){
			return $dados;
		} else {
			die('Tabela inexistente!');
		}
	}
	
	public function verificaTabela( $perid ){
		$sql = "SELECT 
					tbldescricao as descricao
				FROM 
					{$this->stNomeTabela} 
				WHERE 
					perid = ".$perid;
		$dados = $this->pegaLinha( $sql );
		
		if( $dados ){
			return true;
		} else {
			return false;
		}
	}
	
	public function listaObjeto(Array $where = null){
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( simec_get_class($item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
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
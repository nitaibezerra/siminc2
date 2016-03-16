<?php
	
class QQuestionario extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.questionario";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "queid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'queid' => null, 
									  	'sisid' => null, 
									  	'quetitulo' => null, 
									  	'quedescricao' => null, 
									  	'quedtinicio' => null, 
									  	'quedtfim' => null, 
									  	'queindativo' => null, 
									  );
									  
	public function lista(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		
		foreach ($where as $item){
			if (is_object($item) && simec_get_class($item) == 'Sistema'){
				$codicao[] = "sisid = " . $item->sisid;
			}
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );
		
		return $this->carregar($sql, simec_get_class($this));
	}
	
	public function listaObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		
		foreach ($where as $item){
			if (is_object($item) && simec_get_class($item) == 'Sistema'){
				$codicao[] = "sisid = " . $item->sisid;
			}elseif (is_object($item) && simec_get_class($item) == 'QQuestionario'){
				$codicao[] = "queid = " . $item->queid;
			}
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );
		
		return $this->carregarObjeto($sql, simec_get_class($this));
	}	
	
	public function pegaUmObjeto( Array $where , $coluna = null ){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		
		foreach ($where as $k => $item){
			if ( $k === 'queid'){
				$codicao[] = "queid = {$item}";
			}elseif (is_object($item) && simec_get_class($item) == 'Sistema'){
				$codicao[] = "sisid = " . $item->sisid;
			}
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );
		
		return parent::pegaUmObjeto($sql, simec_get_class($this));
	}	

	public function antesExcluir(){
		$grupo = new QGrupo();
		$pergunta = new QPergunta();
		$retornoG = $grupo->lista( array($this), "count(*) AS total" );
		$retornoP = $pergunta->lista( array($this), "count(*) AS total" );
		$this->commit();

		if ( $retornoG[0]['total'] > 0 || $retornoP[0]['total'] > 0 ){
			return false;
		}else{
			return true;
		}
	}
	
}
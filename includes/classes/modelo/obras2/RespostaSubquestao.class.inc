<?php 
	
class RespostaSubquestao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.respostasubquestao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rsqid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'rsqid' 	=> null, 
									  	'sqtid' 	=> null, 
									  	'rsqobs' 	=> null, 
									  	'rsqstatus' => null, 
									  	'qtsid' 	=> null, 
									  );
    
    public function pegaIdRestricao(){
    	
    	$sql = "SELECT
    				rstid
    			FROM
    				obras2.restricao
    			WHERE
    				rsqid = ".$this->rsqid;
    	return $this->pegaUm($sql);
    }

    public function apagaRespostaSubquestao( $qtsid, $sqtid ){
		$sql = "UPDATE obras2.respostasubquestao SET rsqstatus = 'I' WHERE qtsid = {$qtsid} AND sqtid = {$sqtid}";
		$this->executar( $sql );
	}
	
    public function apagaPorRsqid( $rsqid  ){
    	$rsqid = (array) ($rsqid ? $rsqid : 0);
		$sql = "UPDATE obras2.respostasubquestao SET rsqstatus = 'I' WHERE rsqid IN (" . implode(', ', $rsqid) . ")";
		$this->executar( $sql );
	}
    
	public function pegaRsqid( array $param = array() ){
		$arWhere = array("rsqstatus = 'A'");
		
		if ( !empty($param['qtsid']) ){
			$param['qtsid'] = (array) $param['qtsid'];
			$arWhere[] 		= "rq.qtsid IN(" . implode(", ", $param['qtsid']) . ")";
		}
		
		$sql = "SELECT
					rsqid
				FROM
					obras2.respostasubquestao rq	
				WHERE
					" . implode(" AND ", $arWhere);
		$dados = $this->carregarColuna( $sql );
		
		return ($dados ? $dados : array());
	}
}
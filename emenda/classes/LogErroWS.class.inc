<?php
	
class LogErroWS extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emenda.logerrows";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "logid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'logid' => null, 
									  	'ptrid' => null, 
									  	'exfid' => null, 
									  	'cocid' => null, 
									  	'logtipo' => null, 
									  	'logerro' => null, 
									  	'usucpf' => null, 
									  	'logdatainclusao' => null, 
									  	'logenvio' => null, 
									  	'logresposta' => null, 
									  );
									  
	public function montaTextoErro($texto){
		$this->logerro .= $texto."<br />-----------------------------------------------------------------------------------<br /><br />";
	}
	
	/*public function insereDadosErro( $dados ){
		$sql = "INSERT INTO emenda.logerrows(ptrid, exfid, cocid, logtipo, logenvio, logresposta, usucpf, logdatainclusao) 
				VALUES ({$dados['ptrid']}, {$dados['exfid']}, ".($dados['cocid'] ? $dados['cocid'] : 'null').", '{$dados['logtipo']}', '{$dados['logenvio']}', '{$dados['logresposta']}', '{$_SESSION['usucpf']}', now())";
		$this->executar($sql);
		$this->commit();
	}*/
}
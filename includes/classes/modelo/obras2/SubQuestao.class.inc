<?php
	
class SubQuestao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.subquestao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sqtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sqtid' 			=> null, 
									  	'qstid' 			=> null, 
									  	'mrqid' 		   	=> null, 
									  	'sqtdsc' 		   	=> null, 
									  	'sqtimg' 		   	=> null, 
									  	'sqtobs' 		  	=> null, 
									  	'sqtordem' 		  	=> null, 
									  	'sqtstatus' 	   	=> null, 
									  	'sqtbloqrestricao' 	=> null, 
									  );

	public function pegaSubQuestaoPorQstid( $qstid ){
		$sql = "SELECT
					*
				FROM
					obras2.subquestao	
				WHERE
					sqtstatus = 'A' AND
					qstid = {$qstid}
				ORDER BY
					sqtordem";
		$dados = $this->carregar( $sql );
		
		return ($dados ? $dados : array());
	}
	
	public function pegaResultadosSubQuestaoPorSqtidQtsid( $sqtid, $qtsid ){
		$sql = "SELECT
					*
				FROM
					obras2.respostasubquestao
				WHERE
					rsqstatus = 'A' AND
					qtsid = {$qtsid} AND
					sqtid = {$sqtid}";
		$dados = $this->carregar( $sql );
		
		return ($dados ? $dados : array());
	}
}
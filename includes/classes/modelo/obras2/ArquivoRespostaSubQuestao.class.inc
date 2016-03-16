<?php
	
class ArquivoRespostaSubQuestao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.arquivorespostasubquestao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "arsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'arsid' => null, 
									  	'rsqid' => null, 
									  	'arqid' => null, 
									  	'arsstatus' => null, 
									  );
    
    public function listaPorRespQuestao( $rsqid, $smiid = null ){
    	if ( $_SESSION['obras2']['sueid'] ){
    		$where = "sueid = {$_SESSION['obras2']['sueid']}";
    	}else{
    		$where = "smiid = {$smiid}";
    	}
    	
    	$sql = "SELECT
		    		*
		    	FROM
		    		obras2.arquivorespostasubquestao aqs
		    	INNER JOIN obras2.arquivosupervisao ars ON ars.arqid = aqs.arqid AND ars.aqsstatus = 'A'
		    	INNER JOIN public.arquivo a ON a.arqid = aqs.arqid AND a.arqstatus = 'A'
		    	WHERE
			    	arsstatus = 'A' AND
			    	rsqid = {$rsqid} AND
    				{$where}";
    
    	$dados = $this->carregar( $sql );
    	return ($dados ? $dados : array());
    }
}
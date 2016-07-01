<?php
	
class ArquivoQuestaoSupervisao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.arquivoquestaosupervisao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "aqsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'aqsid' => null, 
									  	'qtsid' => null, 
									  	'arqid' => null, 
									  	'aqsstatus' => null, 
									  );

	public function listaPorRespQuestao( $qtsid ){
		$sql = "SELECT
					aqs.aqsid as pk_questao_foto,
					*
				FROM
					obras2.arquivoquestaosupervisao aqs
		    	INNER JOIN obras2.arquivosupervisao ars ON ars.arqid = aqs.arqid AND ars.aqsstatus = 'A'
				INNER JOIN public.arquivo 			a ON a.arqid = aqs.arqid AND a.arqstatus = 'A'
				WHERE
					aqs.aqsstatus = 'A' AND
					qtsid = {$qtsid} ";		
		
		$dados = $this->carregar( $sql );
		return ($dados ? $dados : array());
	} 
	
	public function apagaPorQtsid( $qtsid ){
		$sql = "UPDATE obras2.arquivoquestaosupervisao SET
					aqsstatus = 'I'
				WHERE
					qtsid = {$qtsid}";		
		
		$this->executar( $sql );
	} 
}
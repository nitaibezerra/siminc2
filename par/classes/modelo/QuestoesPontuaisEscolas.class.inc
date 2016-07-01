<?php
	
class QuestoesPontuaisEscolas extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.questoespontuaisescolas";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "qpeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'qpeid' => null, 
									  	'qrpid' => null, 
									  	'perid' => null, 
									  	'entid' => null, 
									  );
									  
	public function quantidadeEscola( $qrpid, $perid ){
		$sql = "SELECT DISTINCT
						count(qpeid)
					FROM
						par.questoespontuaisescolas
					WHERE 
						perid = {$perid} AND
						qrpid = {$qrpid}";
		return $this->pegaUm( $sql );
	}
	
	public function nomeEscolasCadastradas($qrpid, $perid){
		$sql = "SELECT
					entnome
				FROM
					entidade.entidade
				WHERE
					entid IN		
						(SELECT DISTINCT
							entid
						FROM
							par.questoespontuaisescolas
						WHERE 
							perid = {$perid} AND
							qrpid = {$qrpid}
						)";
		return $this->carregarColuna( $sql );
		
	}
}
<?php
	
class QuestoesPontuaisAnexos extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.questoespontuaisanexos";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "qpaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'qpaid' => null, 
									  	'qrpid' => null, 
									  	'perid' => null, 
									  	'arqid' => null, 
									  );
									  
	public function verifica( $qrpid, $perid ){
		$sql = "SELECT qpaid FROM par.questoespontuaisanexos WHERE qrpid = {$qrpid} AND perid ={$perid}";
		$qpaid = $this->pegaUm( $sql );
		if( $qpaid ){
			$sql = "DELETE FROM par.questoespontuaisanexos WHERE qpaid = ".$qpaid;
			$this->executar($sql);
		}
		return true;
	}
	
	public function pegaResposta( $qrpid, $perid ){
		$sql = "SELECT
						*
					FROM
						par.questoespontuaisanexos a
					INNER JOIN
						public.arquivo ar on ar.arqid = a.arqid
					WHERE
						a.qrpid = {$qrpid} AND
						a.perid ={$perid}";
		return $this->pegaLinha( $sql );
	}
	
	public function deletaAnexo( $arqid ){
		$sql = "DELETE FROM
						par.questoespontuaisanexos a
					WHERE
						a.arqid = ".$arqid.";
				DELETE FROM
						public.arquivo a
					WHERE
						a.arqid = ".$arqid.";";
		
		$this->executar( $sql );
		$this->commit();
		return true;
	}
}
<?php 

class ProrrogacaoPrazoOS extends Modelo{
	/**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.prorrogacao_prazo_os";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "prposid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
    								'prposid' => null,
								  	'sosid' => null,
								  	'prazo_anterior' => null,
								  	'novo_prazo' => null,
								  	'justificativa' => null,
								  	'usucpf' => null,
								  	'data_prorrogacao' => null,
								  	'prposstatus' => null
								  );

    /**
     * Verifica existencia de prorrogacao para OS
     */
    public function verificaProrrogacao( $sosid )
    {
    	$sql = "
    		SELECT COUNT(*) as total 
    		FROM " . $this->stNomeTabela . " 
    		WHERE sosid = " . $sosid . " AND prposstatus = 'A' LIMIT 1 ";
    	$resultado = $this->carregar( $sql );
    	
    	if( $resultado[0]['total'] > 0 )
    		return true;
    	else 
    		return false;
    }

    /**
     * Lista todas asprorrogacoes criadas para determinada OS
     */
    public function listaProrrogacoesDeOS( $sosid )
    {
    	$sql = "
    		SELECT 
    			prp.prposid,
    			sos.sosnum,
    			prp.prazo_anterior,
    			prp.novo_prazo,
    			prp.justificativa,
    			sus.usunome,
    			prp.data_prorrogacao
    		FROM " . $this->stNomeTabela . " prp
    		INNER JOIN obras2.supervisao_os sos ON sos.sosid = prp.sosid
    		INNER JOIN seguranca.usuario sus ON sus.usucpf = prp.usucpf
    		WHERE prp.sosid  = " . $sosid . " AND prp.prposstatus = 'A' ";
    	return $this->carregar( $sql );
    }

    /**
     * Apaga prorrogacao e aplica a ultima antes dessa como o valor da data da os
     */
    public function apagaProrrogacao( $prposid )
    {
    	$sosid = $this->buscaSosid( $prposid );

    	$inativada = $this->inativaProrrogacao( $prposid );
    	if( $inativada ){

    		if( $this->aplicaUltimaProrrogacaoComoDataFinalDaOS( $sosid ) ){
    			$this->commit();
    			return true;
    		}else
    			return false;

    	}else
    		return false;
    }

    /**
     * Busca sosid
     */
    public function buscaSosid( $prposid )
    {
    	$sql = "
    		SELECT sosid
    		FROM ".$this->stNomeTabela."
    		WHERE prposid = ". $prposid ." ";
    	return $this->pegaUm( $sql );
    }

    /**
     * Inativa prorrogacao
     */
    public function inativaProrrogacao( $prposid )
    {
    	$sql = " 
    		UPDATE ".$this->stNomeTabela." 
    		SET prposstatus = 'I' 
    		WHERE prposid = ".$prposid." ";
    	$this->executar( $sql );
    	return $this->executar( $sql );
    }

    /**
     * Aplica ultima prorrogacao ativa como data final da OS
     */
    public function aplicaUltimaProrrogacaoComoDataFinalDaOS( $sosid )
    {
    	$sql = "
			UPDATE obras2.supervisao_os
			SET sosdttermino = ";

		$sql1 = "
			SELECT COUNT(*) as total
			FROM ".$this->stNomeTabela."
			WHERE sosid = '".$sosid."' AND prposstatus = 'A'
			LIMIT 1";
		$numeroDeProrrogacoesAtivas = $this->carregar($sql1);
		// ver($numeroDeProrrogacoesAtivas[0]['total']) . " - ";
		if( $numeroDeProrrogacoesAtivas[0]['total'] > 0 )
			$sql .= "(
				SELECT novo_prazo
    			FROM ".$this->stNomeTabela."
    			WHERE sosid = '".$sosid."' AND prposstatus = 'A'
    			ORDER BY prposid DESC 
    			LIMIT 1 )";
		else
			$sql .= "(
				SELECT prazo_anterior
				FROM ".$this->stNomeTabela."
				WHERE sosid = '".$sosid."'
    			ORDER BY prposid ASC 
    			LIMIT 1 )";
		
		$sql .= "WHERE sosid = ".$sosid." ";
		// exit($sql);
		return $this->executar( $sql );
	}
}
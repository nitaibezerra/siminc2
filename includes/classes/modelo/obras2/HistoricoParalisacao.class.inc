<?php
	
class HistoricoParalisacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.historicoparalisacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hprid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'hprid' => null, 
									  	'tplid' => null, 
									  	'supidparalisacao' => null, 
									  	'supidliberacao' => null, 
									  	'obrid' => null, 
									  	'arqid' => null, 
									  	'arqidboletim' => null, 
									  	'hprdtinclusao' => null, 
									  	'hprdtliberacao' => null, 
									  	'hprobs' => null, 
									  	'hprdtstatus' => null, 
									  	'arqidosparalisacao' => null,
                                        'arqiddocumentoembargo' => null,
                                        'stpid' => null
									  );

    public function getUltimoHistoricoParalisacao( $obrid ){
        $obrid = ($obrid ? $obrid : $_SESSION['obras2']['obrid']);

        $sql = "SELECT
                    *
                FROM
                    obras2.historicoparalisacao
                WHERE
                    obrid = {$obrid} AND
                    hprdtstatus = 'A' ORDER BY hprid DESC";
        return $this->pegaLinha( $sql );
    }

    public function getHistoricoParalisacao( $supid ){

        if(!$supid)
            return array();

        $sql = "SELECT
                    *
                FROM
                    obras2.historicoparalisacao
                WHERE
                    supidparalisacao = {$supid}
                ORDER BY hprid DESC";
        return $this->pegaLinha( $sql );
    }
									  
	public function pegaHpridParalisado( Array $param = array() ){
		$where = array();
		
		if (!empty($param['obrid'])){
			$where[] = "obrid = {$param['obrid']}";
		}
		
		$sql = "SELECT
        			hprid
                FROM
                	obras2.historicoparalisacao
                WHERE
                    supidparalisacao IS NOT NULL AND
                    supidliberacao IS NULL AND
                    hprdtstatus = 'A'" . 
				(count($where) ? " AND " . implode(" AND ", $where) : "");
				
		$dados = $this->pegaUm( $sql );
		return $dados;
	}
	
	public function carregaPorSupidParalisacao( $supidparalisacao ){
		$sql = "SELECT
					*
				FROM
                	obras2.historicoparalisacao
				WHERE
                	supidparalisacao = {$supidparalisacao}
                ORDER BY
                	hprid DESC
                LIMIT 1";                
                
		$dados = $this->pegaLinha( $sql );
		$dados = ($dados ? $dados : array());
		$this->popularDadosObjeto( $dados );			
		return $dados;
	}

	public function atualizaStatusPorObrid( $obrid ){
		$sql = "UPDATE 
					obras2.historicoparalisacao SET 
						hprdtstatus = 'I'
        		WHERE obrid = {$obrid}";
		$this->executar( $sql );
	}

	public function listaSql( array $param = array() ){
		$where = array();
//		$join  = array();
    			
		if ( $param['obrid'] ){
			$param['obrid'] = (array) $param['obrid'];
			$where[] 		= "hr.obrid IN(" . implode(", ", $param['obrid']) . ")";
		}
		
		$sql = "SELECT
					'<center>
						<img style=\"cursor:pointer;\" 
							 src=\"/imagens/consultar.gif\" 
							 border=\"0\" 
							 title=\"Visualizar Observação\" 
							 onclick=\"displayMessage(\'?modulo=principal/cadObra&acao=A&requisicao=obshistoricoparalisacao&hprid=' || hr.hprid || '\')\">
					</center>' as acao,
					tp.tpldsc as tipo,
					to_char(hprdtinclusao, 'DD/MM/YYYY') as dtparalisacao,
					CASE WHEN hprdtliberacao is null THEN 'Não Liberada'
						 ELSE to_char(hprdtliberacao, 'DD/MM/YYYY') END as dtliberacao,
					su.usunome as nome
				FROM
					obras2.historicoparalisacao hr
				INNER JOIN
					obras2.tipoparalisacao tp ON hr.tplid = tp.tplid
				LEFT JOIN
					obras2.supervisao s ON s.supid = hr.supidparalisacao
				LEFT JOIN
					obras2.supervisao s1 ON s1.supid = hr.supidliberacao
				LEFT JOIN
					seguranca.usuario su ON s.usucpf = su.usucpf
	
				WHERE
					--hr.hprdtstatus = 'A'
					" . (count($where) ? " " . implode(' AND ',$where) : "");
		
		return $sql;
	}
}

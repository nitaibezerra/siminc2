<?php
	
class EmpreendimentoSupervisao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.empreendimentosupervisao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "emsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'emsid' => null, 
									  	'usucpf' => null, 
									  	'entid' => null, 
									  	'empid' => null, 
									  	'staid' => null, 
									  	'emsdtcadastro' => null, 
									  	'emsdtvistoria' => null, 
									  	'emsdtconclusao' => null, 
									  	'emsstatus' => null, 
									  	'obrid_1' => null, 
									  );

	public function  listaDados( Array $param = array() ){
		$where = array();
		
		if ( $param['empid'] ){
			$where[] = "es.empid = {$param['empid']}";
		}
		
		$sql = "SELECT
					TO_CHAR(es.emsdtvistoria, 'dd/mm/yyyy') AS emsdtvistoria,
					TO_CHAR(es.emsdtconclusao, 'dd/mm/yyyy') AS emsdtconclusao,
					TO_CHAR(es.emsdtcadastro, 'dd/mm/yyyy') AS emsdtcadastro,
					sa.stadesc,
					e.entnome,
					e.entnumcpfcnpj AS entcpf,
					u.usunome,
					es.emsid,
					es.supid_1
				FROM
					obras2.empreendimentosupervisao es
				LEFT JOIN seguranca.usuario u ON u.usucpf = es.usucpf
				LEFT JOIN entidade.entidade e ON e.entid = es.entid
				LEFT JOIN obras2.situacaoatividade sa ON sa.staid = es.staid
				WHERE
					es.emsstatus = 'A'" .  
				(count($where) ? " AND " . implode(' AND ',$where) : "") . "
				ORDER BY
					emsid";
		$dados = $this->carregar( $sql );
		return ($dados ? $dados : array());
	}
	
	public function excluir( $emsid ){
		$sql = "UPDATE obras2.supervisao SET 
					supstatus='I'
				WHERE 
				 	emsid = {$emsid};";
		$this->executar( $sql );
		
		$sql = "UPDATE obras2.empreendimentosupervisao SET 
					emsstatus='I'
				WHERE 
				 	emsid = {$emsid};";
		$this->executar( $sql );
		
	}

	public function pegaUltEmsidByEmpreendimento( $empid ){
		$arJoin  = array();
		$arWhere = array();
		
//		if ( $param['not(emsid)'] ){
//			$arWhere[] = "s.emsid IS NULL";
//		}
//		
//		if ( $param['not(sueid)'] ){
//			$arWhere[] = "s.sueid IS NULL";
//		}
//		
//		if ( $param['empid'] && $param['is(sueid)'] ){
//			$arJoin[] = "JOIN obras2.supervisaoempresa se ON se.sueid = s.sueid";
//			$arWhere[] = "se.empid = {$param['empid']}";
//		}elseif ( $param['empid'] ){
//			$arJoin[] = "JOIN obras2.empreendimentosupervisao es ON es.emsid = s.emsid";
//			$arWhere[] = "es.empid = {$param['empid']}";
//		}
		
		$sql = "SELECT
                	MAX(es.emsid) AS total
                FROM
                	obras2.empreendimentosupervisao es
                WHERE
                	es.emsstatus = 'A' AND
                    es.empid = {$empid}";
		
		return $this->pegaUm( $sql );
	}
}
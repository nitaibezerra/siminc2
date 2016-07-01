<?php
	
class Evento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.evento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "eveid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'eveid' => null, 
									  	'tpeid' => null, 
									  	'tppid' => null, 
//									  	'docid' => null, 
									  	'etaid' => null, 
									  	'sltid' => null, 
									  	'usucpf' => null, 
									  	'agdid' => null, 
									  	'estuf' => null, 
									  	'muncod' => null, 
									  	'evedtinicio' => null, 
									  	'evedtfinal' => null, 
									  	'evehsinicio' => null, 
									  	'evehsfinal' => null, 
									  	'eveassunto' => null, 
									  	'evedtinclusao' => null, 
									  	'evestatus' => null, 
									  	'evebairro' => null, 
									  	'evelogradouro' => null, 
									  	'evenumero' => null, 
									  	'evecomplemento' => null 
									  );

	public function antesSalvar(){
		$this->evedtinicio = (strlen( $this->evedtinicio ) > 10 ? $this->evedtinicio : formata_data_sql( $this->evedtinicio ));		
		$this->evedtfinal  = (strlen( $this->evedtfinal ) > 10 ? $this->evedtfinal : formata_data_sql( $this->evedtfinal ));	

		return true;
	}									  
									  
	public function listaSQL( Array $where = array() ){
		
		$arWhere = array();
		if ( !empty($where['agdid']) ){
				$arWhere[] = "a.agdid = {$where['agdid']}"; 
		}
				
		if ( !empty($where['eveassunto']) ){
				$arWhere[] = "e.eveassunto ILIKE '%{$where['eveassunto']}%'"; 
		}
		
		if ( !empty($where['sltcargo']) ){
				$arWhere[] = "s.sltcargo ILIKE '%{$where['sltcargo']}%'"; 
		}
		
		if ( !empty($where['tpeid']) ){
				$arWhere[] = "e.tpeid = {$where['tpeid']}"; 
		}
		
		if ( !empty($where['tppid']) ){
				$arWhere[] = "e.tppid = {$where['tppid']}"; 
		}
		
		if ( !empty($where['estuf']) ){
				$arWhere[] = "e.estuf = '{$where['estuf']}'"; 
		}
		
		if ( !empty($where['muncod']) ){
				$arWhere[] = "e.muncod = '{$where['muncod']}'"; 
		}
		
		if ( !empty($where['sltid'][0]) ){
				$arWhere[] = "e.sltid IN (" . implode(", ", (array) $where['sltid']) . ")"; 
		}
		
		if ( !empty($where['etaid'][0]) ){
				$arWhere[] = "e.etaid IN (" . implode(", ", (array) $where['etaid']) . ")"; 
		}
		
		/*
		if ( !empty($where['prtid']) ){
				$arWhere[] = "prtid IN (" . implode(", ", (array) $where['prtid']) . ")"; 
		}
		*/
		if ( !empty($where['cevid'][0]) ){
				$arWhere[] = "ec.cevid IN (" . implode(", ", (array) $where['cevid']) . ")"; 
		}
		
		
		if ( !empty($where['evedtinicio']) ){
				$arWhere[] = "evedtinicio >= '" . formata_data_sql( $where['evedtinicio'] ) . "'"; 
		}
		
		if ( !empty($where['evedtfinal']) ){
				$arWhere[] = "evedtfinal <= '" . formata_data_sql( $where['evedtfinal'] ) . "'"; 
		}
		
		if ( !empty($where['evehsinicio']) ){
				$arWhere[] = "evehsinicio >= '" . $where['evehsinicio'] . "'"; 
		}
		
		if ( !empty($where['evehsfinal']) ){
				$arWhere[] = "evehsfinal <= '" . $where['evehsfinal'] . "'"; 
		}
		
		if ( empty($where['evedtinicio']) && empty($where['evedtfinal']) ){
			$arWhere[] = "e.evedtinicio >= CURRENT_DATE";
			$arWhere[] = "e.evedtinicio <= CURRENT_DATE + INTERVAL '30 DAY'";
		}
		
		$userResp = new UsuarioResponsabilidade();
		$arAgdid  = $userResp->pegaRespAgenda();
		
		if ( !verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR) ) && !in_array($where['agdid'], $arAgdid) ){
			$arAevid = $userResp->pegaRespArea();
			$arEveid = $this->pegaEventoPorArea( $arAevid, $where['agdid']);
			$arEveid = ( $arEveid ? $arEveid : array(0) );
			
			if ( $arEveid ){
				$arWhere[] = " e.eveid IN (" . implode(", ", $arEveid) . ") ";
			}
		}
		
		$acao = "'<center>
					<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/alterar.gif\"
	 					style=\"cursor: pointer\"
	 					onclick=\"javascript: alterarEvento(\'' || e.eveid || '\');\"
	 					title=\"Alterar Cadastro Evento\">";
		
		if ( verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR) ) || ( in_array($where['agdid'], $arAgdid) ) ){
			$acao .= "<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/excluir.gif\"
	 					style=\"cursor: pointer; margin-left: 3px;\"
	 					onclick=\"javascript: excluirEvento(\'' || e.eveid || '\');\"
	 					title=\"Excluir Cadastro Evento\">
	 				  <img
	 					align=\"absmiddle\"
	 					src=\"/imagens/print.png\"
	 					style=\"cursor: pointer\"
	 					onclick=\"javascript: abrirDespacho(\'' || e.eveid || '\');\"
	 					title=\"Imprimir Despacho Inicial\">";
		}
		
		$acao .= "</center>'";
		
		$sql = "SELECT 
					$acao AS acao,
					e.eveid || '/' || TO_CHAR(e.evedtinclusao, 'YYYY') AS codigo,
					e.eveassunto,
					TO_CHAR(e.evedtinicio, 'DD/MM/YYYY') || ' - ' || TO_CHAR(e.evehsinicio, 'HH24:MI') AS data_inicio,
					TO_CHAR(e.evedtfinal, 'DD/MM/YYYY') || ' - ' || TO_CHAR(e.evehsfinal, 'HH24:MI') AS data_fim,
					te.tpedsc,
					tp.tppdsc,
					ea.etadsc,
					s.sltdsc,
					u.usunome
				FROM
					agenda.evento e
				JOIN agenda.tipoevento te ON te.tpeid = e.tpeid
				JOIN agenda.tipoprioridade tp ON tp.tppid = e.tppid
				JOIN agenda.agenda a ON a.agdid = e.agdid AND
										a.agdstatus = 'A'
				JOIN agenda.solicitante s ON s.sltid = e.sltid AND
											 s.sltstatus = 'A'
				JOIN agenda.entidadeagenda ea ON ea.etaid = e.etaid AND
												 ea.etastatus = 'A'										
				JOIN seguranca.usuario u ON u.usucpf = e.usucpf	
				JOIN agenda.eventocontato ec on ec.eveid = e.eveid									
				WHERE
					evestatus = 'A'" . 
				(count( $arWhere ) ? " AND " . implode(" AND ", $arWhere) : "")
				. "
				group by 
					e.eveid,e.eveassunto,e.evedtinicio,e.evehsinicio,e.evedtfinal,e.evehsfinal,te.tpedsc,tp.tppdsc,ea.etadsc,s.sltdsc,u.usunome,e.evedtinclusao
				ORDER BY
					evedtinicio, evehsinicio"; 
				//ver($sql, d);
		return $sql;
	}
	
	public function pegaEventoPorArea( array $aevid, $agdid ){
		if (!count( $aevid ) || !$agdid ){
			return array();
		}
		
		$sql = "SELECT
					DISTINCT e.eveid
				FROM
					agenda.agenda a 
				JOIN agenda.evento e ON e.agdid = a.agdid
				JOIN agenda.eventoarea ea ON ea.eveid = e.eveid AND
											 ea.evastatus = 'A'	AND 
											 ea.aevid IN (" . implode(", ", $aevid) . ")
				WHERE
					a.agdid = {$agdid}";
		
		$dados = $this->carregarColuna( $sql );
		$dados = ( $dados ? $dados : array() );
		
		return $dados;
	}
	
	public function listaByAgenda( $agdid ){
		$sql = "SELECT
					eveid,
					eveassunto,
					-- DATA INÍCIO
					TO_CHAR(evedtinicio, 'YYYY') AS anoinicio,
					(TO_CHAR(evedtinicio, 'MM')::integer - 1) AS mesinicio,
					TO_CHAR(evedtinicio, 'DD')::integer AS diainicio,
					-- HORA INÍCIO
					TO_CHAR(evehsinicio, 'HH24') AS horainicio,
					TO_CHAR(evehsinicio, 'MI') AS mininicio,
					-- DATA FIM
					TO_CHAR(evedtfinal, 'YYYY') AS anofim,
					(TO_CHAR(evedtfinal, 'MM')::integer - 1) AS mesfim,
					TO_CHAR(evedtfinal, 'DD')::integer AS diafim,
					-- HORA FIM	
					TO_CHAR(evehsfinal, 'HH24') AS horafim,
					TO_CHAR(evehsfinal, 'MI') AS minfim,
					tppid AS priority,
					tpedsc AS tipoevento,
					sltdsc as solicitante
				FROM
					agenda.evento e 
				INNER JOIN agenda.tipoevento AS te ON te.tpeid = e.tpeid
				INNER JOIN agenda.solicitante AS sol ON sol.sltid = e.sltid
				WHERE
					agdid = {$agdid}";
		
		$dados = $this->carregar( $sql );
		
		return ($dados ? $dados : array());
	}

	public function carregaTodosDados( $eveid ){
		$sql = "SELECT
					e.*,
					TO_CHAR(e.evedtinclusao, 'DD/MM/YYYY') AS evedtinclusao,
					TO_CHAR(e.evedtinicio, 'DD/MM/YYYY') AS evedtinicio,
					TO_CHAR(e.evedtfinal, 'DD/MM/YYYY') AS evedtfinal,
					TO_CHAR(e.evehsinicio, 'HH24:MI') AS evehsinicio,
					TO_CHAR(e.evehsfinal, 'HH24:MI') AS evehsfinal,
					TO_CHAR(e.evedtinclusao, 'YYYY') AS anoevento,
					es.estuf,
					m.mundescricao,
					ea.etadsc,
					s.sltdsc,
					s.sltcargo,
					te.tpedsc,
					tp.tppdsc,
					u.usunome AS responsavel
				FROM
					agenda.evento e
				JOIN territorios.estado es ON es.estuf = e.estuf
				JOIN territorios.municipio m ON m.muncod = e.muncod
				JOIN agenda.entidadeagenda ea ON ea.etaid = e.etaid
				JOIN agenda.solicitante s ON s.sltid = e.sltid
				JOIN agenda.tipoevento te ON te.tpeid = e.tpeid
				JOIN agenda.tipoprioridade tp ON tp.tppid = e.tppid
				JOIN seguranca.usuario u ON u.usucpf = e.usucpf
				WHERE
					eveid = {$eveid}";
		
		$dados = $this->pegaLinha( $sql );
		
		return ($dados ? $dados : array()); 
	}

	public function extratoByAgenda( Array $where = array() ){
		
		$arWhere   = array();
		$arWhere[] = "e.agdid = {$where['agdid']}";
		
		if ( !empty($where['eveassunto']) ){
				$arWhere[] = "e.eveassunto ILIKE '%{$where['eveassunto']}%'"; 
		}
		
		if ( !empty($where['tpeid']) ){
			$arWhere[] = "e.tpeid = {$where['tpeid']}";
		}
		
		if ( !empty($where['tppid']) ){
				$arWhere[] = "e.tppid = {$where['tppid']}";
		}
				
		if ( !empty($where['sltcargo']) ){
				$arWhere[] = "s.sltcargo ILIKE '%{$where['sltcargo']}%'"; 
		}
		
		if ( !empty($where['estuf']) ){
				$arWhere[] = "e.estuf = '{$where['estuf']}'"; 
		}
		
		if ( !empty($where['muncod']) ){
				$arWhere[] = "e.muncod = '{$where['muncod']}'"; 
		}
		
		if ( !empty($where['sltid'][0]) ){
				$arWhere[] = "e.sltid IN (" . implode(", ", (array) $where['sltid']) . ")"; 
		}
		
		if ( !empty($where['etaid'][0]) ){
				$arWhere[] = "e.etaid IN (" . implode(", ", (array) $where['etaid']) . ")"; 
		}
		
		if ( !empty($where['cevid'][0]) ){
				$arWhere[] = "ec.cevid IN (" . implode(", ", (array) $where['cevid']) . ")"; 
		}
				
		if ( !empty($where['evedtinicio']) ){
				$arWhere[] = "e.evedtinicio >= '" . formata_data_sql( $where['evedtinicio'] ) . "'"; 
		}
		
		if ( !empty($where['evedtfinal']) ){
				$arWhere[] = "e.evedtfinal <= '" . formata_data_sql( $where['evedtfinal'] ) . "'"; 
		}

		if ( !empty($where['evehsinicio']) ){
			$arWhere[] = "evehsinicio >= '" . $where['evehsinicio'] . "'";
		}
		
		if ( !empty($where['evehsfinal']) ){
			$arWhere[] = "evehsfinal <= '" . $where['evehsfinal'] . "'";
		}		
		
		$sql = "SELECT distinct
					e.eveid,
					e.eveid || '/' || TO_CHAR(evedtinicio, 'YYYY') AS numero,
					TO_CHAR(evedtinicio, 'DD/MM/YYYY') || ' - ' || TO_CHAR(e.evehsinicio, 'HH24:MI') AS evedtinicio,
					eveassunto,
					te.tpedsc,
					tp.tppdsc,
					ea.etadsc,
					s.sltdsc,
					es.estuf,
					m.mundescricao
				FROM
					agenda.evento e
				JOIN agenda.tipoprioridade tp ON tp.tppid = e.tppid
				JOIN agenda.tipoevento te ON te.tpeid = e.tpeid
				JOIN agenda.solicitante s ON s.sltid = e.sltid AND s.sltstatus = 'A'
				JOIN agenda.entidadeagenda ea ON ea.etaid = e.etaid AND ea.etastatus = 'A'	
				JOIN territorios.estado es ON es.estuf = e.estuf
				JOIN territorios.municipio m ON m.muncod = e.muncod
				JOIN agenda.eventocontato ec on ec.eveid = e.eveid
				WHERE
					e.evestatus = 'A'" . (count( $arWhere ) ? " AND " . implode(" AND ", $arWhere) : "") . "
				order by 3";
		//dbg($sql,1);		
		$dados = $this->carregar( $sql );
		
		return ($dados ? $dados : array());
	}
}
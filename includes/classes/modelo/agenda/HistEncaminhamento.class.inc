<?php
	
class HistEncaminhamento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.histencaminhamento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hseid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'hseid' => null, 
									  	'esdid' => null, 
									  	'usucpfparecer' => null, 
									  	'evaid' => null, 
									  	'hsesequencia' => null, 
									  	'hseparecer' => null, 
									  	'hsedtinclusaoparecer' => null, 
									  	'hsestatus' => null, 
									  	'hseinfrespagenda' => null, 
									  );

	public function listaSQL( Array $where = array() ){
		
		$arWhere = array();
		switch (true){
			case isset($where['evaid']):
				$arWhere[] = "ea.evaid = {$where['evaid']}"; 
				break;
		}
		
		$agdid  = $where['agdid'];
		$agenda = new Agenda();
		
		if ( $agenda->possuiPermissaoTotalAgenda( $agdid ) ){
			$acao = "'<center>
						<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/alterar.gif\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: cadastrarParecer(\'' || ea.evaid || '\', \'' || he.hseid || '\');\"
		 					title=\"Editar o parecer\">
						<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/excluir.gif\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: excluirParecer(\'' || he.hseid || '\');\"
		 					title=\"Excluir o parecer\">
	 				  </center>'";
		}else{
			$acao = "CASE
						WHEN he.usucpfparecer = '" . $_SESSION['usucpf'] . "' THEN
							'<center>
								<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/alterar.gif\"
				 					style=\"cursor: pointer\"
				 					onclick=\"javascript: cadastrarParecer(\'' || ea.evaid || '\', \'' || he.hseid || '\');\"
				 					title=\"Editar o parecer\">
								<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/excluir.gif\"
				 					style=\"cursor: pointer\"
				 					onclick=\"javascript: excluirParecer(\'' || he.hseid || '\');\"
				 					title=\"Excluir o parecer\">
			 				</center>'
			 			ELSE
							''
					END";
		}		
		$sql = "SELECT
					$acao AS acao,
					ed.esddsc,
					-- ae.aevsigla || ' - ' || ae.aevdsc,
					he.hseparecer,
					he.hseinfrespagenda,
					TO_CHAR(he.hsedtinclusaoparecer, 'DD/MM/YYYY HH24:MI') AS data,
					u.usunome AS usuarioparecer
				FROM
					agenda.eventoarea ea
				--JOIN agenda.areaenvolvida ae ON ae.aevid = ea.aevid AND 
				--								ae.aevstatus = 'A'
				JOIN agenda.histencaminhamento he ON he.evaid = ea.evaid
				JOIN workflow.estadodocumento ed ON ed.esdid = he.esdid				
				JOIN seguranca.usuario u ON u.usucpf = he.usucpfparecer		
				WHERE
					he.hsestatus = 'A'
				" . 
				(count( $arWhere ) ? " AND " . implode(" AND ", $arWhere) : "")
				. "
				ORDER BY
					he.hsedtinclusaoparecer DESC"; 
//				dbg($sql, d);
		return $sql;
	}

	public function pegaTodosDados( $hseid ){
		$sql = "SELECT
					ea.*,
					he.*,
					aevdsc,
					ed.esdid,
					esddsc
				FROM
				agenda.histencaminhamento he
				JOIN agenda.eventoarea ea ON ea.evaid = he.evaid AND
											 ea.evastatus = 'A'
				JOIN agenda.areaenvolvida ae ON ae.aevid = ea.aevid AND 
												ae.aevstatus = 'A'
				JOIN workflow.estadodocumento ed ON ed.esdid = he.esdid 
				WHERE
					he.hsestatus = 'A' AND
					he.hseid = {$hseid}";
		
		$dados = $this->pegaLinha( $sql );
		return $dados;
	}
	
	public function existeParecer( $evaid ){
		$sql = "SELECT
					count(*) AS existe
				FROM
					agenda.histencaminhamento 
				WHERE
					evaid = {$evaid}";
		
		return $this->pegaUm( $sql );
	}
}
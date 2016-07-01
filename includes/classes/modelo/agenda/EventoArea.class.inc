<?php
	
class EventoArea extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.eventoarea";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "evaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'evaid' => null, 
									  	'eveid' => null, 
									  	'aevid' => null, 
									  	'docid' => null, 
									  	'evastatus' => null, 
									  );
									  
	public function listaSQL( Array $where = array(), $habilExc = 'S' ){
		
		$arWhere = array();
		switch (true){
			case isset($where['eveid']):
				$arWhere[] = "ea.eveid = {$where['eveid']}"; 
				break;
		}
		
		if ( $habilExc == 'S' ){
			$acao = "'<center>
						<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/alterar.gif\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: alterarVinculo(\'' || ea.evaid || '\');\"
		 					title=\"Alterar vínculo de área\">
		 				<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/excluir.gif\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: excluirVinculo(\'' || ea.evaid || '\');\"
		 					title=\"Excluir vínculo de área\">
	 				  </center>'";
		}else{
			$acao = "'<center>
						<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/alterar.gif\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: alterarVinculo(\'' || ea.evaid || '\');\"
		 					title=\"Alterar vínculo de área\">
	 				  </center>'";
					
		}
		$sql = "SELECT
					$acao AS acao,
					aevsigla || ' - ' || aevdsc,
					esddsc
				FROM
					agenda.eventoarea ea
				JOIN agenda.areaenvolvida ae ON ae.aevid = ea.aevid AND 
												ae.aevstatus = 'A'
				JOIN workflow.documento d ON d.docid = ea.docid				
				JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
				WHERE
					evastatus = 'A'				
				" . 
				(count( $arWhere ) ? " AND " . implode(" AND ", $arWhere) : "")
				. "
				ORDER BY
					evaid DESC"; 
				
		return $sql;
	}
	
	public function pegaTodosDados( $evaid ){
		$sql = "SELECT
					ea.*,
					aevdsc,
					ed.esdid,
					esddsc
				FROM
					agenda.eventoarea ea
				JOIN agenda.areaenvolvida ae ON ae.aevid = ea.aevid AND 
												ae.aevstatus = 'A'
				JOIN workflow.documento d ON d.docid = ea.docid 
				JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid 
				WHERE
					ea.evastatus = 'A' AND
					ea.evaid = {$evaid}";
		
		$dados = $this->pegaLinha( $sql );
		return $dados;
	}
									  									  
	public function listaDscAreaSituacaoByEveid( $eveid ){
		$sql = "SELECT
					ae.aevdsc,
					ed.esddsc
				FROM
					agenda.eventoarea ea
					JOIN agenda.areaenvolvida ae ON ae.aevid = ea.aevid AND
											  ae.aevstatus = 'A'
					JOIN workflow.documento d ON d.docid = ea.docid				
					JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
				WHERE
					ea.evastatus = 'A' AND
					ea.eveid = {$eveid}";
		
		$dados = $this->carregar( $sql );
		
		return ($dados ? $dados : array());
	}
}
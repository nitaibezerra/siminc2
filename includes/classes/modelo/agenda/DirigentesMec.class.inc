<?php
	
class DirigentesMec extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.dirigentesmec";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "drgid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'drgid' => null, 
									  	'aevid' => null, 
									  	'drgnome' => null, 
									  	'drgcargo' => null, 
									  	'drgcelular' => null, 
									  	'drgdddcelular' => null, 
									  	'drgemail' => null, 
									  	'drgdtinclusao' => null, 
									  	'drgstatus' => null, 
									  );
									  
	public function listaSQL( Array $where = array() ){
		
		$arWhere = array();
//		switch (true){
//			case isset($where['agdid']):
//				$arWhere[] = "a.agdid = {$where['agdid']}"; 
//				break;
//		}
		
		$acao = "'<center>
					<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/alterar.gif\"
	 					style=\"cursor: pointer\"
	 					onclick=\"javascript: alterarDirigente(\'' || d.drgid || '\');\"
	 					title=\"Alterar Cadastro Dirigente\">
	 				<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/excluir.gif\"
	 					style=\"cursor: pointer\"
	 					onclick=\"javascript: excluirDirigente(\'' || d.drgid || '\');\"
	 					title=\"Excluir Cadastro Dirigente\">
 				  </center>'";
		
		$sql = "SELECT
					$acao AS acao,
					drgnome,
					drgcargo,
					drgemail,
					aevdsc,
					CASE 
						WHEN drgdddcelular IS NOT NULL AND drgdddcelular != ''
							THEN '(' || COALESCE(drgdddcelular, '') || ') ' || drgcelular
						WHEN drgcelular IS NOT NULL
							THEN drgcelular
					END
				FROM
					agenda.dirigentesmec d
				JOIN agenda.areaenvolvida ae ON ae.aevid = d.aevid AND
												ae.aevstatus = 'A'								
				WHERE
					drgstatus = 'A'" . 
				(count( $arWhere ) ? " AND " . implode(" AND ", $arWhere) : "")
				. "
				ORDER BY
					aevdsc, drgnome"; 
				
		return $sql;
	}
	
	public function listaSimples( Array $where = array() ){
		
		$arWhere = array();
		switch (true){
			case isset($where['aevid']):
				$arWhere[] = "d.aevid = {$where['aevid']}"; 
				break;
		}
		
		$sql = "SELECT
					drgnome,
					drgcargo,
					drgemail,
					CASE 
						WHEN drgdddcelular IS NOT NULL AND drgdddcelular != ''
							THEN '(' || COALESCE(drgdddcelular, '') || ') ' || drgcelular
						WHEN drgcelular IS NOT NULL
							THEN drgcelular
					END
				FROM
					agenda.dirigentesmec d
				WHERE
					drgstatus = 'A'" . 
				(count( $arWhere ) ? " AND " . implode(" AND ", $arWhere) : "")
				. "
				ORDER BY
					drgnome"; 
				
		return $sql;
	}
									  
}
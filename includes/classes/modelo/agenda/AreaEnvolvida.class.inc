<?php
	
class AreaEnvolvida extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.areaenvolvida";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "aevid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'aevid' => null, 
									  	'aevdsc' => null, 
									  	'aevsigla' => null, 
									  	'aevdtinclusao' => null, 
									  	'aevstatus' => null, 
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
	 					onclick=\"javascript: alterarArea(\'' || aevid || '\');\"
	 					title=\"Alterar Cadastro Área\">
	 				<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/excluir.gif\"
	 					style=\"cursor: pointer\"
	 					onclick=\"javascript: excluirArea(\'' || aevid || '\');\"
	 					title=\"Excluir Cadastro Área\">
 				  </center>'";
		
		$sql = "SELECT
					$acao AS acao,
					aevdsc,
					aevsigla
				FROM
					agenda.areaenvolvida								
				WHERE
					aevstatus = 'A'" . 
				(count( $arWhere ) ? " AND " . implode(" AND ", $arWhere) : "")
				. "
				ORDER BY
					aevdsc"; 
				
		return $sql;
	}
	
	public function listaCombo( Array $param = array() ){
		$join  = array();
		$where = array();
		switch ( true ){
			case !empty( $param['notVinculoEvento'] ):
				$where[] = "ae.aevid NOT IN ( SELECT 
												aevid
											  FROM
												agenda.eventoarea
											  WHERE
												eveid = {$param['notVinculoEvento']})";
				break;
			
		} 
		$sql = "SELECT
					aevid AS codigo,
					aevsigla || ' - ' || aevdsc AS descricao
				FROM
					agenda.areaenvolvida ae
				WHERE
					aevstatus = 'A'" .
				(count( $where ) ? " AND " . implode(" AND ", $where) : ""); 
		
		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}
									  
}
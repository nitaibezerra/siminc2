<?php
	
class Restricao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "contratos.restricao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rstid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'rstid' => null, 
									  	'tprid' => null, 
									  	'fsrid' => null, 
									  	'ctrid' => null, 
									  	'usucpf' => null, 
									  	'usucpfsuperacao' => null, 
									  	'rstdsc' => null, 
									  	'rstdtprevisaoregularizacao' => null, 
									  	'rstdtsuperacao' => null, 
									  	'rstdscprovidencia' => null, 
									  	'rstsituacao' => null, 
									  	'rstdtinclusao' => null, 
									  	'rststatus' => null, 
									  );

	function listaSql( Array $param = array() ){
		$arWhere = array();
		
		if ( !empty($param['ctrid']) ){
			$param['ctrid'] = (array) $param['empid'];
			$arWhere[] 		= "r.ctrid IN(" . implode(", ", $param['ctrid']) . ")";
		}
		

				
		$acao = "'<center>
					<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/alterar.gif\"
	 					style=\"cursor: pointer\"
	 					onclick=\"javascript: alterarRest(\'' || r.rstid || '\');\"
	 					title=\"Alterar\">
	 				<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/excluir.gif\"
	 					style=\"cursor: pointer; margin-left: 3px;\"
	 					onclick=\"javascript: excluirRest(\'' || r.rstid || '\');\"
	 					title=\"Excluir\">
	 			  </center>'";
		
		
		$sql = "SELECT
					{$acao} AS acao,
					CASE WHEN r.fsrid IS NOT NULL THEN fr.fsrdsc ELSE 'Não Informada' END AS fase,
					tr.tprdsc,
					TO_CHAR(r.rstdtinclusao, 'DD/MM/YYYY') AS rstdtinclusao,
					r.rstdsc,
					r.rstdscprovidencia,
					TO_CHAR(r.rstdtprevisaoregularizacao, 'DD/MM/YYYY') AS rstdtprevisaoregularizacao,
					usu.usunome AS criadopor,
					CASE WHEN r.rstsituacao = TRUE THEN TO_CHAR(r.rstdtsuperacao, 'DD/MM/YYYY') ELSE 'Não' END AS rstdtsuperacao,
					sup.usunome as ususuperacao
				FROM
					contratos.restricao r
				JOIN contratos.tiporestricao tr ON tr.tprid = r.tprid AND
												tr.tprstatus = 'A'
				JOIN contratos.faserestricao fr ON fr.fsrid = r.fsrid AND
												fr.fsrstatus = 'A'
				JOIN seguranca.usuario usu ON usu.usucpf = r.usucpf
				LEFT JOIN seguranca.usuario sup ON sup.usucpf = r.usucpfsuperacao	 
				WHERE
					r.rststatus = 'A' AND " . 
					( implode(" AND ", $arWhere) ) . "
				ORDER BY
						r.ctrid";
		return $sql;		
	}
}
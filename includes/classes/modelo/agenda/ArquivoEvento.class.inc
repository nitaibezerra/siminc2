<?php
	
class ArquivoEvento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.arquivoevento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "taeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'taeid' 		=> null, 
									  	'arqid' 		=> null, 
									  	'eveid' 		=> null, 
									  	'taedsc' 		=> null,
									  	'taestatus' 	=> null,
									  	'taetinclusao'  => null
									  );

	public function listaSQL( Array $where = array() ){
		
		$arWhere = array();
		switch (true){
			case isset($where['eveid']):
				$arWhere[] = "ae.eveid = {$where['eveid']}"; 
				break;
		}
		
		$acao = "'<center>
	 				<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/excluir.gif\"
	 					style=\"cursor: pointer\"
	 					onclick=\"javascript: excluirArquivo(\'' || a.arqid || '\');\"
	 					title=\"Excluir Anexo\">
 				  </center>'";
		
		$sql = "SELECT
					$acao AS acao,
					taedsc,
					'<a href=\"javascript:downloadArquivo(' || a.arqid || ')\">' || a.arqnome || '.' || a.arqextensao || '</a>' AS arquivo,
					TO_CHAR(taetinclusao, 'DD/MM/YYYY') AS data_inclusao,
					u.usunome
				FROM
					agenda.arquivoevento ae
				JOIN public.arquivo a ON a.arqid = ae.arqid AND
										 a.arqstatus = 'A'
				JOIN seguranca.usuario u ON u.usucpf = a.usucpf
				WHERE
					taestatus = 'A'" . 
				(count( $arWhere ) ? " AND " . implode(" AND ", $arWhere) : "")
				. "
				ORDER BY
					taetinclusao, arquivo"; 
//				dbg($sql, d);
		return $sql;
	}
	
}
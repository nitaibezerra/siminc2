<?php
	
class ArquivoLicitacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.arquivolicitacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "aqlid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'aqlid' => null,
									  	'arqid' => null, 
									  	'flcid' => null, 
									  	'usucpf' => null, 
									  	'aqldtinclusao' => null, 
									  	'aqlstatus' => null, 
									  );

	public function listaSqlPorFaseLicitacao( $flcid, $habil = 'S' ){
		
		if ( empty($flcid) ) return '';
		
		if ( $habil == 'S' ){
			$caminho = '?modulo=principal/vincFaseLicitacao&acao=A&requisicao=excluir';
			$permissaoBotaoExcluir = "'<center>
										<a href=\"#\" onclick=\"javascript:ExcluirDocumento(\'" . $caminho . "\',' || arq.arqid || ',' || aql.aqlid || ');\">
											<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\">
										</a>
									   </center>' AS acao,";
		}else{
			$permissaoBotaoExcluir = "'' AS acao,";
		}
		
		$sql = "SELECT
						{$permissaoBotaoExcluir}
						to_char(aql.aqldtinclusao,'DD/MM/YYYY'),
						'<a style=\"cursor: pointer; color: blue;\" onclick=\"DownloadArquivo(' || arq.arqid || ');\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
						arq.arqtamanho || ' kbs' as tamanho ,
						arq.arqdescricao,								
						usu.usunome
					FROM
						public.arquivo arq 
						JOIN obras2.arquivolicitacao aql ON arq.arqid = aql.arqid AND
															aql.aqlstatus = 'A' AND	
															aql.flcid = {$flcid}	 
						JOIN seguranca.usuario usu ON usu.usucpf = aql.usucpf";

		return $sql;
	}
	
    public function listaHomologadoSql( Array $param = array() ){
    	$where 	 = array();
    	
		$where[] = "fl.tflid = " . FASE_LIC_HOMOLOGACAO;
    	
    	if ( $param['obrid'] ){
    		$where[] = "ol.obrid = {$param['obrid']}"; 
    	}
    	 
		$sql = "SELECT
					to_char(aql.aqldtinclusao,'DD/MM/YYYY'),
					'<a style=\"cursor: pointer; color: blue;\" onclick=\"DownloadArquivoFase(' || arq.arqid || ');\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
					arq.arqtamanho || ' kbs' as tamanho ,
					arq.arqdescricao,								
					usu.usunome
				FROM
					public.arquivo arq 
					JOIN obras2.arquivolicitacao aql ON arq.arqid = aql.arqid AND
														aql.aqlstatus = 'A'	 
					JOIN seguranca.usuario usu ON usu.usucpf = aql.usucpf
					JOIN obras2.faselicitacao fl ON fl.flcid = aql.flcid AND 
													fl.flcstatus = 'A'
					JOIN obras2.obralicitacao ol ON ol.licid = fl.licid AND
													ol.oblstatus = 'A'								
				WHERE " . 
					implode(' AND ',$where);    	
					
    	return $sql;
    }

    /**
     * Retorna os dados o Arquivo Licitação de acordo com o arqid
     *
     * @param $arqid
     * @return array|bool|void
     */
    public function buscaDadosPorArqid( $arqid ){
        $sql = "SELECT
                                *
                        FROM
                                obras2.arquivolicitacao
                        WHERE
                                aqlstatus = 'A' AND
                                arqid = {$arqid}";

        $dado = $this->pegaLinha( $sql );

        return ($dado ? $dado : array());
    }

    /**
     * Inativa um arquivo licitação
     *
     * @param null $aqlid
     */
    public function inativar($aqlid = null)
    {
        $sql = "UPDATE {$this->stNomeTabela} SET aqlstatus = 'I' WHERE aqlid = {$aqlid}";

        $this->executar($sql);
        $this->commit();
    }
	
	
}
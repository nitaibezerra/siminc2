<?php
	
class ObrasContrato extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.obrascontrato";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ocrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ocrid' => null, 
									  	'crtid' => null, 
									  	'obrid' => null, 
									  	'umdid' => null, 
									  	'arqidos' => null, 
									  	'arqidcusto' => null, 
									  	'ocrqtdconstrucao' => null, 
									  	'ocrdtordemservico' => null, 
									  	'ocrdtinicioexecucao' => null, 
									  	'ocrprazoexecucao' => null, 
									  	'ocrdtterminoexecucao' => null, 
									  	'ocrvalorexecucao' => null, 
									  	'ocrcustounitario' => null, 
									  	'ocrpercentualdbi' => null, 
									  	'ocraditivado' => null, 
									  	'ocrstatus' => null, 
									  );

	public function listaByContrato( $crtid, $obrid = null, Array $param = array() ){
		
		if ( empty($crtid) ){
			return array();
		}
		
		$join  = array();
		$where = array();
		
		if ( $obrid ){
			$where[] = "o.obrid={$obrid}";
		}
		
		if ( $param['IS(obraAditivo)'] ){
			$join[]  = "JOIN workflow.documento d ON d.docid = o.docid";
			$where[] = "d.esdid = ". ESDID_OBJ_ADITIVO;
		}
		
		$sql = "SELECT
					umdid, 
				  	ocrqtdconstrucao, 
				  	ocrdtordemservico, 
				  	ocrdtinicioexecucao, 
				  	ocrprazoexecucao, 
				  	ocrdtterminoexecucao, 
				  	ocrvalorexecucao, 
				  	ocrcustounitario, 
				  	ocrpercentualdbi, 
				  	o.obrid,
				  	o.obrnome,
				  	oc.ocraditivado,
				  	oc.arqidos,
				  	oc.arqidcusto
				FROM
					obras2.obrascontrato oc
				JOIN obras2.obras o ON o.obrid = oc.obrid AND
									   o.obrstatus = 'A'
				" . ( $join ? implode(' ', $join) : '' ) . "					   
				WHERE
					ocrstatus = 'A' AND 
					" . ($where ? implode(' AND ', $where) . " AND " : "") . "
					oc.crtid={$crtid}
				ORDER BY
					oc.ocrid";
		
		$dados = $this->carregar( $sql );
		
		return ( $dados ? $dados : array() );
	}

	public function apagaByContrato( $crtid ){
		$sql = "UPDATE obras2.obrascontrato
				SET ocrstatus='I'
				WHERE crtid = {$crtid};";
		
		$this->executar( $sql );
	}
	
	public function apagaPorLicitacao( $licid ){
		$sql = "UPDATE obras2.obrascontrato
				SET ocrstatus='I'
				WHERE crtid IN (SELECT
									crtid
								FROM
									obras2.contrato
								WHERE
									licid = {$licid});";
		
		$this->executar( $sql );
	}

	public function getDadosCabecalhoByObra( $obrid ){
		$sql = "SELECT
					TO_CHAR(ocrdtinicioexecucao, 'DD/MM/YYYY') AS ocrdtinicioexecucao,
					TO_CHAR(ocrdtterminoexecucao, 'DD/MM/YYYY') AS ocrdtterminoexecucao,
					ocrvalorexecucao,
					entnome
				FROM
					obras2.obrascontrato oc
				JOIN obras2.contrato c ON c.crtid = oc.crtid AND
										  c.crtstatus = 'A'
				JOIN entidade.entidade e ON e.entid = c.entidempresa AND
											e.entstatus = 'A'
				WHERE
					obrid = {$obrid} AND
					ocrstatus = 'A';";
		
		$dados = $this->pegaLinha( $sql );
		return ($dados ? $dados : array());
	}
	
	public function getValorContrato( $obrid ){
		$sql = "SELECT
					ocrvalorexecucao
				FROM
					obras2.obrascontrato oc
				JOIN obras2.contrato c ON c.crtid = oc.crtid AND
							  			  c.crtstatus = 'A'
				WHERE
					oc.obrid = {$obrid} AND
					oc.ocrstatus = 'A'
				LIMIT 1;";
		
		$dado = $this->pegaUm( $sql );
		return ($dado ? $dado : 0);
	}

	public function listaIdByContrato( $crtid ){
		$sql = "SELECT
					ocrid
				FROM
					obras2.obrascontrato
				WHERE
					ocrstatus = 'A' AND
					crtid = {$crtid}";
		$dados = $this->carregarColuna( $sql );
		
		return ($dados ? $dados : array() );
	}

	public function pegaObridHistoricoByContratoAndObraPai($crtid, $obridPai){
		$sql = "SELECT
					o.obrid
				FROM
					obras2.obrascontrato oc
				JOIN obras2.obras o ON o.obrid = oc.obrid AND 
									   o.obrstatus = 'A' AND
									   o.obridpai = {$obridPai}
				WHERE
					oc.ocrstatus = 'A' AND
					oc.crtid = {$crtid}";
		
		return $this->pegaUm( $sql );
	}

    public function listaArqidOsPorObra($obrid){
        $sql = "
            SELECT 
                to_char(arq.arqdata,'DD/MM/YYYY'),
                '<a style=\"cursor: pointer; color: blue;\" onclick=\"DownloadArquivoFase(' || arq.arqid || ');\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
                arq.arqtamanho || ' kbs' as tamanho ,
                arq.arqdescricao,								
                usu.usunome        
            FROM obras2.obrascontrato oc 
            JOIN public.arquivo arq on oc.arqidos = arq.arqid
            JOIN seguranca.usuario usu ON usu.usucpf = arq.usucpf
            WHERE oc.obrid = $obrid and ocrstatus = 'A'
            ";
        return $this->carregar($sql);
    }
    
	public function listaTodosArqidPorObra( $obrid ){
		$sql = "SELECT
					arqidcontrato,
					arqidos,
					arqidcusto
				FROM
					obras2.obrascontrato oc 
				JOIN obras2.contrato c ON c.crtid = oc.crtid AND
							  			  c.crtstatus = 'A'
				WHERE
					oc.ocrstatus = 'A' AND
					oc.obrid = {$obrid}";
		$dados = $this->pegaLinha( $sql );
		return ($dados ? $dados : array());
	}

	/*
	 * $crtidRemetente		=> Corresponde ao registro histórico de aditivo que vai ser retornado ao contrato vigente da obra;
	 * $crtidDestinatario	=> Corresponde ao ID do contrato vigente da obra.
	 */
	public function retrocedeObrasContrato( $crtidRemetente, $crtidDestinatario ){
		$sql = "UPDATE obras2.obrascontrato oc
				SET umdid=a.umdid, ocrqtdconstrucao=a.ocrqtdconstrucao, ocrdtordemservico=a.ocrdtordemservico, 
				    ocrdtinicioexecucao=a.ocrdtinicioexecucao, ocrprazoexecucao=a.ocrprazoexecucao, ocrdtterminoexecucao=a.ocrdtterminoexecucao, 
				    ocrvalorexecucao=a.ocrvalorexecucao, ocrcustounitario=a.ocrcustounitario, ocrpercentualdbi=a.ocrpercentualdbi,
				    ocraditivado=a.ocraditivado, arqidos=a.arqidos, arqidcusto=a.arqidcusto
				FROM(
					SELECT umdid, ocrqtdconstrucao, ocrdtordemservico, 
					       ocrdtinicioexecucao, ocrprazoexecucao, ocrdtterminoexecucao, 
					       ocrvalorexecucao, ocrcustounitario, ocrpercentualdbi, ocrstatus, 
					       ocraditivado, arqidos, arqidcusto, obridpai
					FROM obras2.obrascontrato oc
					JOIN obras2.obras o ON o.obrid = oc.obrid
					WHERE
						ocrstatus = 'A' AND
						crtid = {$crtidRemetente}
				) AS a
				WHERE 
					oc.ocrstatus = 'A' AND
					oc.crtid = {$crtidDestinatario} AND
					oc.obrid = a.obridpai;";
		$this->executar( $sql );	
	}

    public function valorContratoObraMi( $tpoid,$estuf ){
        $sql = "SELECT SUM(i.icmvalor)
                FROM obras2.cronogramapadraomi c
                JOIN obras2.itenscomposicaopadraomi i ON i.cpmid = c.cpmid AND i.relativoEdificacao = 'D'
                WHERE c.estuf like '{$estuf}'
                and c.tpoid = '{$tpoid}'
                and c.cpmstatus = 'A'";
        $dados = $this->pegaUm( $sql );
        return ($dados ? $dados : array());
    }

    /**
     * Retorna todos os dados da tabela obrascontrato e contrato
     * @param $obrid
     */
    public function pegaDadosContratoObra($obrid)
    {
        $sql = "
            SELECT
                *
            FROM obras2.obrascontrato oc
                JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
            WHERE oc.ocrstatus = 'A' AND oc.obrid = $obrid
        ";
        return $this->pegaLinha($sql);
    }
}
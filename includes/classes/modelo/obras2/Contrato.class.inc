<?php

class Contrato extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.contrato";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "crtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
                                        'crtid' => null, 
                                        'entidempresa' => null, 
                                        'licid' => null, 
                                        'orgid' => null, 
                                        'ttaid' => null, 
                                        'crtidpai' => null, 
                                        'arqid' => null, 
                                        'arqidcontrato' => null, 
                                        'crtdtassinatura' => null, 
                                        'crtdttermino' => null, 
                                        'crtprazovigencia' => null, 
                                        'crtvalorexecucao' => null, 
                                        'crtpercentualdbi' => null, 
                                        'crtdtassinaturaaditivo' => null, 
                                        'crtdenominacao' => null, 
                                        'crtjustificativa' => null, 
                                        'crtsupressao' => null, 
                                        'crtstatus' => null,
                                        'usucpf' => null ,
                                        'dt_cadastro' => null
                                      );

	public function listaSql( Array $filtro = array() ){
		$arWhere = array();
		
		if ( $filtro['licnumero'] ){
			$arWhere[] = "l.licnumero ILIKE '%{$filtro['licnumero']}%'";
		}
		
		if ( $filtro['molid'] ){
			$arWhere[] = "l.molid = '{$filtro['molid']}'";
		}
		
		if ( $filtro['crtaditivado'] ){
			$arWhere[] = "c.ttaid " . ($filtro['crtaditivado'] == 'S' ? 'IS NOT NULL' : 'IS NULL');
		}
		
		if ( $filtro['crtdtassinatura_ini'] ){
			$filtro['crtdtassinatura_ini'] = formata_data_sql( $filtro['crtdtassinatura_ini'] );
			$arWhere[] = "c.crtdtassinatura >= '{$filtro['crtdtassinatura_ini']}'";
		}
		
		if ( $filtro['crtdtassinatura_fim'] ){
			$filtro['crtdtassinatura_fim'] = formata_data_sql( $filtro['crtdtassinatura_fim'] );
			$arWhere[] = "c.crtdtassinatura <= '{$filtro['crtdtassinatura_fim']}'";
		}
		
		if ( $filtro['crtdttermino_ini'] ){
			$filtro['crtdttermino_ini'] = formata_data_sql( $filtro['crtdttermino_ini'] );
			$arWhere[] = "c.crtdttermino >= '{$filtro['crtdttermino_ini']}'";
		}
		
		if ( $filtro['crtdttermino_fim'] ){
			$filtro['crtdttermino_fim'] = formata_data_sql( $filtro['crtdttermino_fim'] );
			$arWhere[] = "c.crtdttermino <= '{$filtro['crtdttermino_fim']}'";
		}
		
		if ( $filtro['crtvalorexecucao_ini'] ){
			$filtro['crtvalorexecucao_ini'] = MoedaToBd( $filtro['crtvalorexecucao_ini'] );
			$arWhere[] = "c.crtvalorexecucao >= {$filtro['crtvalorexecucao_ini']}";
		}
		
		if ( $filtro['crtvalorexecucao_fim'] ){
			$filtro['crtvalorexecucao_fim'] = MoedaToBd( $filtro['crtvalorexecucao_fim'] );
			$arWhere[] = "c.crtvalorexecucao <= {$filtro['crtvalorexecucao_fim']}";
		}
		
		if ( $filtro['orgid'] ){
			$arWhere[] = "c.orgid = {$filtro['orgid']}";
		}
		
		if ( $filtro['crtsupressao'] ){
			$arWhere[] = "c.crtsupressao = '{$filtro['crtsupressao']}'";
		}elseif ( isset( $filtro['crtsupressao'] ) ){
			$arWhere[] = "c.crtsupressao IS NULL";
		}
		
		if ( $filtro['not(crtidpai)'] ){
			$arWhere[] = "c.crtidpai IS NULL";
		}
		
		$acao = "'<center>
					<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/alterar.gif\"
	 					style=\"cursor: pointer\"
	 					onclick=\"javascript: alterarCrt(\'' || c.crtid || '\');\"
	 					title=\"Alterar Contrato\">
	 				<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/excluir.gif\"
	 					style=\"cursor: pointer; margin-left: 3px;\"
	 					onclick=\"javascript: excluirCrt(\'' || c.crtid || '\');\"
	 					title=\"Excluir Contrato\">
				  </center>'";
		
		$sql = "SELECT
					DISTINCT
					{$acao} AS acao,
					l.licnumero || ' - ' || ml.moldsc,
					e.entnome,
					CASE WHEN c.ttaid IS NOT NULL THEN 'Sim' ELSE 'Não' END,
					TO_CHAR(c.crtdtassinatura, 'DD/MM/YYYY') AS dataassinatura,
					TO_CHAR(c.crtdttermino, 'DD/MM/YYYY') AS datatermino,
					crtvalorexecucao
				FROM
					obras2.contrato c
				JOIN entidade.entidade e ON e.entid=c.entidempresa
				JOIN obras2.licitacao l ON l.licid = c.licid
				JOIN obras2.modalidadelicitacao ml ON ml.molid = l.molid
				WHERE
					crtstatus = 'A'" .
					(count($arWhere) ? " AND " . implode(" AND ", $arWhere) : "");
					
		return $sql;
	}

	public function listaHistoricoByContratoPai( $crtidPai, array $param = array() ){
		$arWhere = array();
		
		if ( $param['maiorUltDataTramiteWf'] ){
			$arWhere[] = "dt_cadastro > '{$param['maiorUltDataTramiteWf']}'";
		}
		
		
		$sql = "SELECT 
					*
				FROM
					obras2.contrato
				WHERE
					crtstatus = 'A' AND
					crtidpai = {$crtidPai}" .
					($arWhere ? " AND " . implode(" AND ", $arWhere) : "")
					. "
				ORDER BY
					crtid";
					
		$dados = $this->carregar( $sql );
		return ($dados ? $dados : array());
	}

	public function apagaPorLicitacao( $licid ){
		$sql = "UPDATE obras2.contrato SET crtstatus='I' WHERE licid = {$licid};";
		$this->executar( $sql );	
	}
	
	public function apagaPorCrtid( $crtid ){
		$sql = "UPDATE obras2.contrato SET crtstatus='I' WHERE crtid = {$crtid};";
		$this->executar( $sql );	
	}
	
	public function pegaCrtidPorObrid( $obrid ){
		$sql = "SELECT
					c.crtid
				FROM
					obras2.contrato c 
				JOIN obras2.obrascontrato oc ON oc.crtid = c.crtid AND
												oc.ocrstatus = 'A' AND
												oc.obrid = {$obrid}
				WHERE
					c.crtstatus = 'A'";	
		
		return $this->pegaUm( $sql );			
	}
	
	public function pegaCrtidCancelamentoAditivo($crtid, $ultDataAditivo){
		$sql = "SELECT 
					MAX(crtid) AS crtid
				FROM (
						SELECT 
							MAX(crtid) AS crtid
						FROM
							obras2.contrato
						WHERE
							crtstatus = 'A' AND
							(crtidpai = {$crtid}) AND (dt_cadastro < '{$ultDataAditivo}' OR dt_cadastro IS NULL)
					UNION ALL
						SELECT 
							MIN(crtid) AS crtid
						FROM
							obras2.contrato
						WHERE
							crtstatus = 'A' AND
							(crtidpai = {$crtid}) AND (dt_cadastro > '{$ultDataAditivo}' OR dt_cadastro IS NULL)
				) AS a";
		$crtid = $this->pegaUm( $sql );
		
		return $crtid;
	}
	
	public function retrocedeContratoInteiro( $crtid, $obrid ){
		$obras 			= new Obras();
		$ultDataAditivo = $obras->pegaUltDataMudancaEstadoWf( $obrid );
		$crtidRetorno 	= $this->pegaCrtidCancelamentoAditivo($crtid, $ultDataAditivo);
		
		if ( $crtidRetorno ){
			// Retrocede Contrato
			$this->retrocedeContrato($crtidRetorno, $crtid);
	
			$obrasContrato = new ObrasContrato();
			// Retrocede ObrasContrato
			$obrasContrato->retrocedeObrasContrato($crtidRetorno, $crtid);
			
			// pega todos aditivos que execederem a data da última celebração de aditivo 
			$dadoHist = $this->listaHistoricoByContratoPai( $crtid, array('maiorUltDataTramiteWf' => $ultDataAditivo) );
			foreach ( $dadoHist as $arContrato ){
				//Apaga Contrato retrocedidos
				$this->apagaPorCrtid( $arContrato['crtid'] );
				
				// Apaga ObrasContrato retrocedidos
				$obrasContrato->apagaByContrato( $arContrato['crtid'] );
			}
		}
	}
	
	/*
	 * $crtidRemetente		=> Corresponde ao registro histórico de aditivo que vai ser retornado ao contrato vigente da obra;
	 * $crtidDestinatario	=> Corresponde ao ID do contrato vigente da obra.
	 */
	private function retrocedeContrato( $crtidRemetente, $crtidDestinatario ){
		$sql = "UPDATE obras2.contrato c
				   SET crtdtassinatura=a.crtdtassinatura, crtdttermino=a.crtdttermino, crtprazovigencia=a.crtprazovigencia, 
				       crtvalorexecucao=a.crtvalorexecucao, crtpercentualdbi=a.crtpercentualdbi, entidempresa=a.entidempresa, 
				       licid=a.licid, orgid=a.orgid, crtdenominacao=a.crtdenominacao, crtjustificativa=a.crtjustificativa, 
				       ttaid=a.ttaid, crtsupressao=a.crtsupressao, crtdtassinaturaaditivo=a.crtdtassinaturaaditivo, arqid=a.arqid, obrid_1=a.obrid_1, 
				       arqidcontrato=a.arqidcontrato
				FROM (
					SELECT crtid, crtdtassinatura, crtdttermino, crtprazovigencia, crtvalorexecucao, 
					       crtpercentualdbi, crtstatus, entidempresa, licid, orgid, 
					       crtdenominacao, crtjustificativa, ttaid, crtsupressao, crtdtassinaturaaditivo, 
					       arqid, obrid_1, arqidcontrato
					FROM obras2.contrato
					WHERE
						crtstatus = 'A' AND
						crtid = {$crtidRemetente}
				) AS a       
				WHERE 
					c.crtstatus = 'A' AND
					c.crtid = {$crtidDestinatario};";
		$this->executar( $sql );	
	}

	public function bloqueiaContrato($crtid){
		$sql = "SELECT
					COUNT(*) AS total
				FROM
					obras2.contrato c
				JOIN obras2.obrascontrato oc ON oc.crtid = c.crtid AND
								oc.ocrstatus = 'A'
				JOIN obras2.obras o ON o.obrid = oc.obrid AND
						       o.obrstatus = 'A'
				JOIN workflow.documento d ON d.docid = o.docid AND
							     d.esdid IN (" . ESDID_OBJ_EXECUCAO . ", 
							     			 " . ESDID_OBJ_ADITIVO . ", 
							     			 " . ESDID_OBJ_CONCLUIDO . ")
				WHERE
					c.crtid = {$crtid}";	
		
		$totRegistro = $this->pegaUm( $sql );
		
		return ($totRegistro ? true : false);
	}

	public function pegaDadosEmpresaPorObrid( $obrid ){
		
		$sql = "SELECT
					e.*,
					ed.*,
					m.mundescricao,
					nj.njudsc AS natureza	
				FROM
					obras2.contrato c
				JOIN obras2.obrascontrato oc ON oc.crtid = c.crtid AND
												oc.ocrstatus = 'A' AND
												oc.obrid = {$obrid}	
				JOIN entidade.entidade e ON e.entid = c.entidempresa	
				JOIN entidade.endereco ed ON ed.entid = e.entid
				JOIN territorios.municipio m ON m.muncod = ed.muncod	
				LEFT JOIN entidade.naturezajuridica nj ON nj.njuid = e.njuid
				WHERE
					c.crtstatus = 'A'";
		$dados = $this->pegaLinha( $sql );
		
		return ($dados ? $dados : array());
	}

	public function pegaDadosPorObrid( $obrid ){
		$sql = "SELECT
					c.*,
					oc.*,
					um.umdeesc
				FROM
					obras2.contrato c
				JOIN obras2.obrascontrato oc ON oc.crtid = c.crtid AND
												oc.ocrstatus = 'A' AND
												oc.obrid = {$obrid}
				JOIN obras2.unidademedida um ON um.umdid = oc.umdid AND
												um.umdstatus = 'A'												
				WHERE
					c.crtstatus = 'A'";
		$dados = $this->pegaLinha( $sql );
		
		return ($dados ? $dados : array());		
	}
}
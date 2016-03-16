<?php

class ProcessoObrasPar extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.processoobraspar";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("proid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'proid' => null,
        'pronumeroprocesso' => null,
        'muncod' => null,
        'resid' => null,
        'probanco' => null,
        'proagencia' => null,
        'prodatainclusao' => null,
        'usucpf' => null,
        'proseqconta' => null,
        'protipo' => null,
        'seq_conta_corrente' => null,
        'nu_conta_corrente' => null,
        'estuf' => null,
        'procnpj' => null,
        'inuid' => null,
        'sisid' => null,
        'progeraproc' => null,
        'arqidanexodoc' => null,
        'prostatus' => null,
        'prodataconsultasigef' => null,
        'profinalizado' => null,
    );

    public function buscarPagamentoObrasPar($arrPost = array(), $booSQL = FALSE) {
        #armazena as condicoes/join da consulta
        $arrWhere = array('v.saldo > 0 ');
        $arrJoin = array();

        #preid
        if (!empty($arrPost['preid'])) {
            $preIdObrIdClause = true;
            $preIdObrIdClauseValue = $arrPost['preid'];
            $arrWhere[] = "eo.preid = {$arrPost['preid']}";
        }
        
        #Join em Comum
        if (!empty($arrPost['homologacao']) || !empty($arrPost['obrid']) || !empty($arrPost['execucao25']) || !empty($arrPost['execucao50']) || !empty($arrPost['esdid'])) {
            $arrJoin[] = "INNER JOIN obras2.obras obr ON obr.preid = eo.preid";
        }

        #obrid
        if (!empty($arrPost['obrid'])) {
            $preIdObrIdClause = true;
            $preIdObrIdClauseValue = $arrPost['obrid'];
            $arrWhere[] = "obr.obrid = {$arrPost['obrid']}";
        }

        #processo
        if (!empty($arrPost['numeroprocesso'])) {
            $arrWhere[] = "p.pronumeroprocesso = '{$arrPost['numeroprocesso']}'";
        }

        #municipio
        if (!empty($arrPost['municipio'])) {
            $arrWhere[] = "removeacento(m.mundescricao) ILIKE removeacento('%" . $arrPost['municipio'] . "%')";
        }

        #UF
        if (!empty($arrPost['estuf'])) {
            $arrWhere[] = "(iu.estuf = '" . $arrPost['estuf'] . "' OR iu.mun_estuf = '" . $arrPost['estuf'] . "')";
        }

        #Esfera
        if (!empty($arrPost['esfera'])) {
            if ($arrPost['esfera'] == 'Municipal') {
                $arrWhere[] = "iu.estuf IS NULL";
            } else {
                $arrWhere[] = "iu.estuf IS NOT NULL";
            }
        }

        #Grupo de Municipio
        if ($arrPost['grupoMunicipio'][0]) {
            $arrWhere[] = "tm.tpmid IN (" . implode(",", $arrPost['grupoMunicipio']) . ")";
            $arrJoin[] = "INNER JOIN territorios.muntipomunicipio mt ON mt.muncod = m.muncod";
            $arrJoin[] = "INNER JOIN territorios.tipomunicipio tm ON tm.tpmid = mt.tpmid";
        }

        #Tipo Obra
        if ($arrPost['tipoobra'][0]) {
            $arrWhere[] = "p.protipo IN ('" . implode("','", $arrPost['tipoobra']) . "')";
        }
        
        # Adiciona join em comum
        if ($arrPost['termogerado'] == "1" || !empty($arrPost['dopusucpfvalidacaogestor']) || !empty($arrPost['dopusucpfvalidacaofnde'])) {
            $arrJoin[] = "INNER JOIN par.documentopar dp ON dp.proid = p.proid AND dopstatus = 'A'";
            $arrJoin[] = "INNER JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid";
        }

        #Termo Gerado
        if (!empty($arrPost['termogerado'])) {
            if ($arrPost['termogerado'] == "1") {
                if ($arrPost['tpdcod']) {
                    $arrWhere[] = "dp.dopid in (SELECT dopid FROM par.documentopar dp1 
            			INNER JOIN par.modelosdocumentos md ON md.mdoid = dp1.mdoid AND md.mdostatus = 'A'
            			WHERE md.tpdcod = {$arrPost['tpdcod']} ) ";
                }
                if ($arrPost['mdoid']) {
                    $arrWhere[] = "dp.mdoid = {$arrPost['mdoid']}";
                }
            } else {
                $arrWhere[] = "p.proid not in (SELECT DISTINCT proid FROM par.documentopar WHERE dopstatus = 'A' AND proid IS NOT NULL)";
            }
        }

        #Termo Validacao Gestor
        if (!empty($arrPost['dopusucpfvalidacaogestor'])) {
            $arrWhere[] = "dp.mdoid in (24,27,25,26)";
            $strValidacaoGestor = "SELECT DISTINCT 
                                        dp.dopid 
                                    FROM par.modelosdocumentos md
                                    INNER JOIN par.documentopar dp ON dp.mdoid = md.mdoid and dp.dopstatus = 'A'
                                    INNER JOIN par.documentoparvalidacao dv ON dv.dopid = dp.dopid and dv.dpvstatus = 'A'
                                    WHERE md.mdostatus = 'A'";
            if ($arrPost['dopusucpfvalidacaogestor'] == 't') {
                $arrWhere[] = "dp.dopid IN ($strValidacaoGestor)";
            } else {
                $arrWhere[] = "dp.dopid NOT IN ($strValidacaoGestor)";
            }
        }

        #Termo Validacao FNDE
        if (!empty($arrPost['dopusucpfvalidacaofnde'])) {
            $strValidacaoFnde = "SELECT DISTINCT 
                                    dp.dopid 
                                FROM par.modelosdocumentos md
                                INNER JOIN par.documentopar dp on dp.mdoid = md.mdoid and dp.dopstatus = 'A'
                                INNER JOIN par.documentoparvalidacao dv on dv.dopid = dp.dopid and dv.dpvstatus = 'A'
                                WHERE md.mdoqtdvalidacao > 1
                                AND md.mdostatus = 'A'";
            if ($arrPost['dopusucpfvalidacaofnde'] == 't') {
                $arrWhere[] = "dp.dopid in ($strValidacaoFnde)";
            } else {
                $arrWhere[] = "dp.dopid not in ($strValidacaoFnde)";
            }
        }

        #Pagamento
        if ($arrPost['pagamento'] != "0") {
            $strPagamento = "SELECT DISTINCT
                                e.empnumeroprocesso
                            FROM par.empenho e
                            INNER JOIN par.pagamento pg ON e.empid=pg.empid AND pg.pagstatus = 'A' [[replace]]                
                            INNER JOIN par.v_vrlempenhocancelado vve on vve.empid = e.empid
                            WHERE e.empstatus = 'A'
                                AND e.empcodigoespecie NOT IN ('03', '13', '02', '04')
                                AND e.empnumeroprocesso=p.pronumeroprocesso
                                AND vve.vrlempenhocancelado > 0";

            if ($arrPost['pagamento'] == "1") { #Pagamento Solicitado
                $arrWhere[] = "p.pronumeroprocesso IN (" . str_replace("[[replace]]", "AND pg.pagsituacaopagamento <> '2 - EFETIVADO'", $strPagamento) . ")";
            } else if ($arrPost['pagamento'] == "2") { #Pagamento Não Solicitado
                $arrWhere[] = "(" . str_replace("[[replace]]", "", $strPagamento) . ") IS NULL";
            } else { #Pagamento Efetivado
                $arrWhere[] = "p.pronumeroprocesso IN (" . str_replace("[[replace]]", "AND pg.pagsituacaopagamento = '2 - EFETIVADO'", $strPagamento) . ")";
            }
        }

        #Join em comum
        if (!empty($arrPost['homologacao']) || !empty($arrPost['execucao25']) || !empty($arrPost['execucao50'])) {
            $arrJoin[] = "INNER JOIN obras2.validacao val ON val.obrid = obr.obrid";
        }
        
        # Ordem de serviço inserida Homologacao
        if (!empty($arrPost['homologacao'])) {
            if ($arrPost['homologacao'] == 'S') {
                $arrWhere[] = " val.vldstatushomologacao = 'S' ";
            } elseif ($arrPost['homologacao'] == 'N') {
                $arrWhere[] = " (val.vldstatushomologacao = 'N') ";
            } else {
                $arrWhere[] = " (val.vldstatushomologacao is null) ";
            }
        }

        #Execução 25% Validada
        if (!empty($arrPost['execucao25'])) {
            if ($arrPost['execucao25'] == 'S') {
                $arrWhere[] = " val.vldstatus25exec = 'S' ";
            } elseif ($arrPost['execucao25'] == 'N') {
                $arrWhere[] = " val.vldstatus25exec = 'N' ";
            } else {
                $arrWhere[] = " val.vldstatus25exec IS NULL ";
            }
        }

        #execucao 50% Validada
        if (!empty($arrPost['execucao50'])) {
            if ($arrPost['execucao50'] == 'S') {
                $arrWhere[] = " val.vldstatus50exec = 'S' ";
            } elseif ($arrPost['execucao50'] == 'N') {
                $arrWhere[] = " val.vldstatus50exec = 'N' ";
            } else {
                $arrWhere[] = " val.vldstatus50exec IS NULL ";
            }
        }

        #Situacao da Obra
        if (!empty($arrPost['esdid'])) {
            $arrJoin[] = "INNER JOIN workflow.documento obrd ON obrd.docid = obr.docid";
            $arrWhere[] = " obrd.esdid = '{$arrPost['esdid']}' ";
        }

        #Obras nao paga ateh
        if (!empty($arrPost['perc_n_exec'])) {
            $arrJoin[] = "INNER JOIN par.processoobrasparcomposicao poc ON poc.proid = p.proid";
            $arrJoin[] = "INNER JOIN (	SELECT preid, SUM(poppercentualpag) as perc_pag 
            							FROM par.pagamentoobrapar pop
            							INNER JOIN par.pagamento pag3 ON pag3.pagid = pop.pagid AND pag3.pagstatus = 'A'
            							GROUP BY preid) as pag ON pag.preid = poc.preid";
            $arrWhere[] = " ( 100 - coalesce(pag.perc_pag,0) ) <= {$arrPost['perc_n_exec']}";
        }

        #Obras MUNICIPAIS com pendencias
        if (!empty($arrPost['pendenciaObras'])) {
            $arrWhere[] = " iu.itrid = '2' ";
            if ($arrPost['pendenciaObras'] == '1') { // com pendencias
                $arrWhere[] = " p.muncod in (select coalesce(muncod, '') from obras2.v_pendencia_obras where empesfera = 'M')  ";
            } else { // sem pendencias
                $arrWhere[] = " p.muncod not in (select coalesce(muncod, '') from obras2.v_pendencia_obras where empesfera = 'M')  ";
            }
        }

        #Obras ESTADUAIS com pendencias
        if (!empty($arrPost['pendenciaObrasUF'])) {
            $arrWhere[] = " iu.itrid = '1' ";
            if ($arrPost['pendenciaObrasUF'] == '1') { // com pendencias
                $arrWhere[] = " p.estuf in (select coalesce(estuf, '') from obras2.v_pendencia_obras where empesfera = 'E')  ";
            } else { // sem pendencias
                $arrWhere[] = " p.estuf not in (select coalesce(estuf, '') from obras2.v_pendencia_obras where empesfera = 'E')  ";
            }
        }

        #Possui erro na distribuicao do pagamento
        if ($this->testa_superuser() && $arrPost['pagamentoMaiorQueEmpenho'] == 'S') {
            $arrPost['tooid'] = 2;
            $query = sql_verificaPagamentoMaiorQueEmpenhoObra();
            $arrWhere[] = "(SELECT
                    COUNT(empid)
                FROM
                    (
                    " . str_replace(Array('%processo%', '%filtro%'), Array('p.pronumeroprocesso', ''), $query) . "					
                    ) as foo ) > 0";
        }

        #Coluna Habilita
        if ($arrPost['acaoBotao'] == 'exportar') {
            $colunaHabilita = 'iue.iuesituacaohabilita';
        } else {
            $colunaHabilita = "CASE WHEN iue.iuesituacaohabilita = 'Habilitado' THEN
                                    '<center><img src=../imagens/workflow/1.png title=\"'||iue.iuesituacaohabilita||'\" style=cursor:pointer;></center>' 
                                ELSE 
                                    '<center><img src=../imagens/workflow/2.png title=\"'||iue.iuesituacaohabilita||'\" style=cursor:pointer;></center>' 
                                END AS habilitado";
        }

        #botao de processo
        $btnProcesso = "(
                        CASE WHEN (select count(*) as qtd from painel.dadosfinanceirosconvenios dfi where dfi.dfiprocesso = p.pronumeroprocesso) > 0 THEN
                            '<span class=\"processoDetalhes processo_detalhe\" >'
                        ELSE
                            '<span class=\"processo_detalhe\" >'
                        END
                        ) ||  to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>',";
        $perfil = pegaPerfilGeral();
        if ($arrPost['acaoBotao'] == 'exportar') {
            $sqlAcao = "";
            $btnProcesso = "to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00'),";
        } elseif (( (in_array(PAR_PERFIL_PAGADOR, $perfil)) || (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)) || (in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)))) {
            $sqlAcao = "'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaEmpenhoPagamentoObrasPar(\''||p.proid||'\', this);\"></center>' as mais,";
            if ($preIdObrIdClause) {
                $sqlAcao.= "'<center><img src=../imagens/money.gif style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/solicitacaoPagamentoObraPar&acao=A&processo=' || p.pronumeroprocesso || '&proid='||p.proid||'&preidobrid={$preIdObrIdClauseValue}\' ,\'Empenho\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\"></center>' as acoes,";
            } else {
                $sqlAcao.= "'<center><img src=../imagens/money.gif style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/solicitacaoPagamentoObraPar&acao=A&processo=' || p.pronumeroprocesso || '&proid='||p.proid||'\',\'Empenho\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\"></center>' as acoes,";
            }
        } else {
            $sqlAcao = "'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaEmpenhoPagamentoObrasPar(\''||p.proid||'\', this);\"></center>' as mais,";
            $sqlAcao .= "'' as acoes, ";
        }
        
        if( $arrPost['colunas'][0] != '' ){
        	foreach( $arrPost['colunas'] as $coluna ){
        		switch( $coluna ){
        			case 'saldo_conta_corrente':
        				$colunaHabilita .= ", 
			                	par.retornasaldoprocesso(p.pronumeroprocesso) as saldobancario ";
        				break;
        		}
        	}
        }
        
	    if( !empty($arrPost['boSolicitacaoDesembolso']) ){
			$fil = '';
			if( $arrPost['boSolicitacaoDesembolso'] == 'S' ){
				$fil = 'p.proid in';
			} 
			if( $arrPost['boSolicitacaoDesembolso'] == 'N' ){
				$fil = 'p.proid not in';		
			}
			$arrWhere[] = " $fil (SELECT
												proid /*,
											    preid,
												sldpercpagamento,
											    perc,
											    perc_pago*/
											FROM(
											    SELECT 
											        pp.proid,
											        pp.preid,
											        sd.sldpercpagamento,
											        (SELECT obrpercentultvistoria FROM obras2.obras 
											            WHERE preid = pre.preid AND obrstatus = 'A' AND obridpai IS NULL) as perc,
											        ((SELECT SUM(p2.popvalorpagamento) 
        											FROM par.pagamentoobrapar p2
											            inner join par.pagamento pag2 ON pag2.pagid = p2.pagid
											        WHERE p2.preid = pre.preid
											            and pag2.pagstatus = 'A' 
											            and pag2.pagsituacaopagamento not ilike '%CANCELADO%' ) / pre.prevalorobra)*100 as perc_pago
											    FROM 
											        par.processoobrasparcomposicao pp 
											        inner join obras.preobra pre on pre.preid = pp.preid and pre.prestatus = 'A'
											        inner join obras2.solicitacao_desembolso sd on sd.obrid = pre.obrid and sd.sldstatus = 'A'
											        inner join workflow.documento d ON d.docid = sd.docid
											    WHERE
											        pp.pocstatus = 'A'
											        and d.esdid = 1576
											) foo
											WHERE (perc - perc_pago) >= sldpercpagamento )";
		}

        $sql = "SELECT DISTINCT 
                    {$sqlAcao}
                    {$btnProcesso}
                    m.mundescricao, 
                    CASE WHEN p.estuf IS NOT NULL THEN p.estuf ELSE m.estuf END as estuf, 
                    (SELECT 
						SUM(pre.prevalorobra) 
						FROM par.processoobrasparcomposicao pc 
						
						INNER JOIN obras.preobra pre ON pre.preid = pc.preid 
                        INNER JOIN par.empenho emp ON emp.empid=eo.empid and empstatus = 'A'
                        WHERE 
                        pc.proid = p.proid
                        AND eobstatus = 'A' 
                    ) as valorobras,
                    (SELECT sum(saldo) FROM par.v_saldo_obra_por_empenho WHERE processo = p.pronumeroprocesso ) as valorempenhado,
                    (SELECT
                        COALESCE(sum(pagvalorparcela),0.00)
                        FROM(
                                    SELECT DISTINCT pg.pagid, pg.pagvalorparcela FROM par.pagamento pg 
                        INNER JOIN par.empenho em ON em.empid=pg.empid and empstatus = 'A' 
                        INNER JOIN par.pagamentoobrapar po ON po.pagid = pg.pagid
                        WHERE em.empnumeroprocesso=p.pronumeroprocesso AND pg.pagstatus='A' AND pg.pagsituacaopagamento ILIKE '%EFETIVADO%') as foo
                    ) as valorpagamento,
                    (SELECT count(DISTINCT preid) FROM par.empenhoobrapar eo 
                        INNER JOIN par.empenho emp ON emp.empid=eo.empid and empstatus = 'A'
                        WHERE 
                        emp.empnumeroprocesso=p.pronumeroprocesso 
                        AND eobstatus = 'A' 
                        AND empcodigoespecie not in ('03', '13', '02', '04') 
                        AND eo.preid in (
                                SELECT 
                                        pc.preid
                                FROM par.processoobraspar p
                                INNER JOIN par.processoobrasparcomposicao pc ON pc.proid = p.proid
                                WHERE pc.pocstatus = 'A'
                        )
                    ) as qtdobrempenhadas,
                    CASE WHEN (p.muncod is null or p.muncod = '') THEN 'Estadual' ELSE 'Municipal' END as esfera,
                    CASE WHEN p.sisid = 14 THEN 'Brasil-pró' ELSE 'PAR' END as tiposistema ,
                    $colunaHabilita ,
                    CASE WHEN ed.esddsc IS NULL THEN 'Diagnóstico' ELSE ed.esddsc END as situacao
                FROM {$this->stNomeTabela} p 
                INNER JOIN par.instrumentounidade 			iu ON iu.inuid = p.inuid
                INNER JOIN par.instrumentounidadeentidade 	iue ON iue.inuid = iu.inuid and iue.iuestatus = 'A' and iue.iuedefault = true 
                INNER JOIN workflow.documento 				d   ON d.docid = iu.docid AND d.tpdid = 44
                INNER JOIN workflow.estadodocumento 		ed  ON ed.esdid = d.esdid
                LEFT  JOIN territorios.municipio 			m   ON m.muncod = p.muncod
                INNER JOIN par.v_saldo_empenho_do_processo 	v   ON v.processo = p.pronumeroprocesso
                INNER JOIN par.empenho 						e   ON e.empnumeroprocesso = p.pronumeroprocesso AND v.processo = e.empnumeroprocesso  AND e.empstatus = 'A' AND e.empcodigoespecie NOT IN ('03', '13', '02', '04') 
                INNER JOIN par.empenhoobrapar 				eo  ON eo.empid = e.empid AND eobstatus = 'A'
                ".implode(' ', $arrJoin)."
                WHERE " . implode(' AND ', $arrWhere);
                return ($booSQL) ? $sql : $this->carregar($sql);
    }

}

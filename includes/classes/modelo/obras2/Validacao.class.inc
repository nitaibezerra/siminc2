<?php
	
class Validacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.validacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "vldid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
                                        'vldid' => null,
                                        'usucpf_homo' => null,
                                        'usucpf_25' => null,
                                        'usucpf_50' => null,
                                        'obrid' => null,
                                        'vldstatushomologacao' => null,
                                        'vlddtinclusaosthomo' => null,
                                        'vldstatus25exec' => null,
                                        'vlddtinclusaost25exec' => null,
                                        'vldstatus50exec' => null,
                                        'vlddtinclusaost50exec' => null,
                                        'vldobshomologacao' => null,
                                        'vldobs25exec' => null,
                                        'vldobs50exec' => null,
                                        'vldid_1' => null
                                  );

    function pegaValidacaoObra($obrid)
    {
        $sql = "
            SELECT * FROM obras2.validacao v
            WHERE v.obrid = $obrid;
        ";

        $dado = $this->pegaLinha($sql);
        return ($dado ? $dado : array());
    }

	function pegaDadosComFaseLicitacao( Array $param = array() ){
		$where 	 = array();
//		$where[] = "fl.tflid = " . FASE_LIC_HOMOLOGACAO;


    	if ( $param['obrid'] ){
    		$where[] = "o.obrid = {$param['obrid']}";
    	}

//    	if( $param['tflid'] ){
//    		$where[] = "fl.tflid = '".$param['tflid']."' ";
//    	}

		$sql = "SELECT
					v.vldid, u1.usunome as usuhomo, u2.usunome as usu25, u3.usunome as usu50, v.obrid, v.usucpf_homo, v.usucpf_25,
	  				v.usucpf_50, v.vldstatushomologacao, v.vlddtinclusaosthomo, to_char(v.vlddtinclusaosthomo, 'DD/MM/YYYY HH24:MI:SS') as vlddtinclusaosthomo1,
					v.vldstatus25exec, to_char(v.vlddtinclusaost25exec, 'DD/MM/YYYY HH24:MI:SS') as vlddtinclusaost25exec1, v.vlddtinclusaost25exec,
					v.vldstatus50exec, to_char(v.vlddtinclusaost50exec, 'DD/MM/YYYY HH24:MI:SS') as vlddtinclusaost50exec1, v.vlddtinclusaost50exec, fl.flcid, tf.tfldesc, fl.tflid, fl.flchomlicdtprev,
					fl.flcdata, fl.flcmeiopublichomol, fl.flcobshomol, o.obrpercentultvistoria,
					v.vldobshomologacao, v.vldobs25exec, v.vldobs50exec
				FROM obras2.obras o
				  	left join obras2.validacao v on v.obrid = o.obrid
				  	left join obras2.obralicitacao ol on ol.obrid = o.obrid and
				  										 ol.oblstatus = 'A'
				  	left join obras2.faselicitacao fl on fl.licid = ol.licid and
				  										 fl.tflid = " . FASE_LIC_HOMOLOGACAO . " and
				  										 fl.flcstatus = 'A'
					left join obras2.tiposfaseslicitacao tf ON tf.tflid = fl.tflid
				  	left join seguranca.usuario u1 on u1.usucpf = v.usucpf_homo
					left join seguranca.usuario u2 on u2.usucpf = v.usucpf_25
					left join seguranca.usuario u3 on u3.usucpf = v.usucpf_50
				WHERE
				  	".implode(' AND ',$where);
		$dados = $this->pegaLinha( $sql );
                
//                ver($sql);
//                ver($dados);
//dbg($sql, d);
		return ($dados ? $dados : array());
	}

    public function listaRelatorioValidacao($params)
    {
        $where = array();
        extract ($params);
        if( $prfid[0] && $prfid_campo_flag ){
            if ( !$prfid_campo_excludente ){
                array_push($where, " ep.prfid  IN (" . implode( ',', $prfid ) . ") ");
            }else{
                array_push($where, " ( ep.prfid  NOT IN (" . implode( ',', $prfid ) . ") OR ep.prfid is null ) ");
            }

        }
// Fonte
        if( $tooid[0] && $tooid_campo_flag ){
            if ( !$tooid_campo_excludente ){
                array_push($where, " oi.tooid  IN (" . implode( ',', $tooid ) . ") ");
            }else{
                array_push($where, " ( oi.tooid  NOT IN (" . implode( ',', $tooid ) . ") OR oi.tooid IS NULL ) ");
            }

        }

        if (!empty($dthomologacao_de)) {array_push($where, "v.vlddtinclusaosthomo::date >= '" . ajusta_data($dthomologacao_de) ."'");}
        if (!empty($dthomologacao_ate)) {array_push($where, "v.vlddtinclusaosthomo::date <= '" . ajusta_data($dthomologacao_ate) ."'");}

        if (!empty($dtexecucao25_de)) {array_push($where, "v.vlddtinclusaost25exec::date >= '" . ajusta_data($dtexecucao25_de) ."'");}
        if (!empty($dtexecucao25_ate)) {array_push($where, "v.vlddtinclusaost25exec::date <= '" . ajusta_data($dtexecucao25_ate) ."'");}

        if (!empty($dtexecucao50_de)) {array_push($where, "v.vlddtinclusaost50exec::date >= '" . ajusta_data($dtexecucao50_de) ."'");}
        if (!empty($dtexecucao50_ate)) {array_push($where, "v.vlddtinclusaost50exec::date <= '" . ajusta_data($dtexecucao50_ate) ."'");}


        if( !empty( $entidunidade ) ){ array_push($where, "ep.entidunidade = {$entidunidade} ");}
        if( !empty( $esdid ) ) {array_push($where, "doc.esdid = {$esdid}");}
        if( !empty( $tpoid ) ) {array_push($where, "tpo.tpoid= {$tpoid}");}
        if( !empty( $stoid ) ) {array_push($where, "oi.stoid = {$stoid}");}
        if( !empty( $estuf ) ) {array_push($where, "edo.estuf = '{$estuf}'");}
        if( !empty( $obrid ) ) {array_push($where, "oi.obrid = $obrid");}
        if( !empty( $preid ) ) {array_push($where, "oi.preid = $preid");}
//if( !empty( $obrtextobusca ) ) array_push($where, "(oi.preid = ".(int)$obrtextobusca." or oi.obrid =".(int)$obrtextobusca.")");

        if( $homologacao == 'S' ) {array_push( $where, " v.vldstatushomologacao = 'S' " );}
        elseif( $homologacao == 'N' ) {array_push( $where, " (v.vldstatushomologacao = 'N') " );}
        elseif( $homologacao == 'P' ) {array_push( $where, " (v.vldstatushomologacao is null or v.vldstatushomologacao = ' ' ) " );}

        if( $execucao25 == 'S' ) {array_push( $where, " v.vldstatus25exec = 'S' " );}
        elseif( $execucao25 == 'N' ) {array_push( $where, " v.vldstatus25exec = 'N' " );}
        elseif( $execucao25 == 'P' ) {array_push( $where, " (v.vldstatus25exec IS NULL or v.vldstatus25exec = ' ' )" );}

        if( $execucao50 == 'S' ) {array_push( $where, " v.vldstatus50exec = 'S' " );}
        elseif( $execucao50 == 'N' ) {array_push( $where, " v.vldstatus50exec = 'N' " );}
        elseif( $execucao50 == 'P' ) {array_push( $where, " (v.vldstatus50exec IS NULL  or v.vldstatus50exec = ' ' )" );}

        if ($repasse == 'S') {
            array_push($where, " oi.obrid IN (SELECT DISTINCT o.obrid FROM obras2.obras o
                                        JOIN par.pagamentoobra po ON po.preid = o.preid
                                        JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) = '2 - EFETIVADO'::text
                                        WHERE o.obridpai IS NULL AND o.obrstatus = 'A') ");
        } elseif ($repasse == 'N') {
            array_push($where, " oi.obrid NOT IN (SELECT DISTINCT o.obrid FROM obras2.obras o
                                        JOIN par.pagamentoobra po ON po.preid = o.preid
                                        JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) = '2 - EFETIVADO'::text
                                        WHERE o.obridpai IS NULL AND o.obrstatus = 'A') ");
        }


        if( $arqhanexo == 'S' ){
            $innerArquivo = "inner join obras2.obralicitacao ol1 on ol1.obrid = oi.obrid
                     inner join obras2.licitacao lc1 on lc1.licid = ol1.licid
                     inner join obras2.faselicitacao fl1 on fl1.licid = lc1.licid and fl1.flcstatus = 'A' and fl1.tflid = 9
                     inner join obras2.arquivolicitacao aql1 on fl1.flcid = aql1.flcid";
        }
        elseif( $arqhanexo == 'N' ){
            array_push( $where, " oi.obrid not in ( SELECT DISTINCT ol.obrid
                                                FROM obras2.faselicitacao fl
                                                JOIN obras2.tiposfaseslicitacao tfl ON tfl.tflid = fl.tflid AND tfl.tflstatus = 'A'
                                                JOIN obras2.arquivolicitacao aql on fl.flcid = aql.flcid
                                                JOIN obras2.licitacao l ON l.licid = fl.licid
                                                LEFT JOIN obras2.modalidadelicitacao m ON m.molid = l.molid
                                                JOIN obras2.obralicitacao ol ON ol.licid = l.licid AND ol.oblstatus = 'A'
                                                WHERE ol.obrid = oi.obrid AND fl.flcstatus = 'A' AND tfl.tflid = 9
                                            ) " );
        }
        elseif( $arqhanexo == '' || !$arqhanexo ){
            $innerArquivo = "";
        }

        if( $arqanexo == 'S' ){
            $innerOsArquivo = "
                     join obras2.obrascontrato ocr on ocr.obrid = oi.obrid
                     join public.arquivo arq on ocr.arqidos = arq.arqid and ocrstatus = 'A'
                    ";
        }
        elseif( $arqanexo == 'N' ){
            array_push( $where, " oi.obrid not in (
                                            SELECT DISTINCT ocr.obrid from obras2.obrascontrato ocr
                                            join public.arquivo arq on ocr.arqidos = arq.arqid and ocrstatus = 'A' AND ocr.obrid = oi.obrid
                                        ) " );
        }
        elseif( $arqanexo == '' || !$arqanexo ){
            $innerOsArquivo = "";
        }



        if ( $_REQUEST['pesquisa'] == 'xls' ){
            $nome_obra = " '(' || oi.obrid || ') ' || oi.obrnome, ";
            $campos = "
                    mun.mundescricao as municipio,
                    edo.estuf as uf,
                    COALESCE(oi.obrvalorprevisto, 0.00) as obrvalorprevisto,
                        COALESCE(oi.obrpercentultvistoria, 0.00) as obrpercexec,
                        CASE WHEN oi.preid IS NOT NULL THEN
                                'Sim'
                        else
                                'Não'
                        END as pagamento,
                        pag.pagvalorparcela,
                        pag.probanco || ' ' AS probanco,
                        pag.proagencia || ' ' AS proagencia,
                        pag.nu_conta_corrente || ' ' AS nu_conta_corrente,
                        (SELECT
                                dfi.dfisaldoconta + dfi.dfisaldofundo + dfi.dfisaldopoupanca + dfi.dfisaldordbcdb
                            FROM painel.dadosfinanceirosconvenios dfi
                            WHERE dfiprocesso = p_conv.pronumeroprocesso
                            ORDER BY dfi.dfidatasaldo DESC LIMIT 1) as saldo_conta,

                        (SELECT
                                dfi.dfimesanosaldo
                            FROM painel.dadosfinanceirosconvenios dfi
                            WHERE dfiprocesso = p_conv.pronumeroprocesso
                            ORDER BY dfi.dfidatasaldo DESC LIMIT 1) as ref_mes,
                        v.vldstatushomologacao,
                    CASE WHEN (v.vldobshomologacao IS NULL or v.vldobshomologacao = '') THEN 'Sem observações' ELSE v.vldobshomologacao END as observacao,
                    v.vldstatus25exec,
                    CASE WHEN v.vldobs25exec IS NULL THEN 'Sem observações' ELSE v.vldobs25exec END,
                    v.vldstatus50exec,
                    CASE WHEN v.vldobs50exec IS NULL THEN 'Sem observações' ELSE v.vldobs50exec END,

                     CASE WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = oi.obrid) > 0 THEN 'VERMELHO'
                        WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = oi.obrid) > 0 THEN 'AMARELO'
                        WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = oi.obrid) > 0 THEN 'VERDE'
                        WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = oi.obrid) > 0 THEN 'AZUL'
                        ELSE 'CINZA' END as CA

                        ,

                        CASE WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = oi.obrid) > 0 THEN 'VERMELHO'
                        WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = oi.obrid) > 0 THEN 'AMARELO'
                        WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = oi.obrid) > 0 THEN 'VERDE'
                        WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = oi.obrid) > 0 THEN 'AZUL'
                        ELSE 'CINZA' END as CV

                        ,

                        CASE WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = oi.obrid) > 0 THEN 'VERMELHO'
                        WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = oi.obrid) > 0 THEN 'AMARELO'
                        WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = oi.obrid) > 0 THEN 'VERDE'
                        WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = oi.obrid) > 0 THEN 'AZUL'
                        ELSE 'CINZA' END as CT

                        ,

                        CASE WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = oi.obrid) > 0 THEN 'VERMELHO'
                        WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = oi.obrid) > 0 THEN 'AMARELO'
                        WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = oi.obrid) > 0 THEN 'VERDE'
                        WHEN (SELECT COUNT(*)
                            FROM obras2.restricao r
                            JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                            WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = oi.obrid) > 0 THEN 'AZUL'
                        ELSE 'CINZA' END as Outros,";
        }else{
            $nome_obra = " '<a onclick=\"obrIrParaCaminhoPopUp(\'' || oi.obrid || '\',\'cadastro\');\">' || '(' || oi.obrid || ') ' || upper(oi.obrnome) || '</a>' as obrdesc,";
            $campos = "
                    mun.mundescricao as municipio,
                    edo.estuf as uf,
                    COALESCE(oi.obrvalorprevisto, 0.00) as obrvalorprevisto,
                        COALESCE(oi.obrpercentultvistoria, 0.00) as obrpercexec,
                        CASE WHEN oi.preid IS NOT NULL THEN
                                '<a onmouseover=\"SuperTitleAjax(u+\'' || oi.preid || '\',this)\" onmouseout=\"SuperTitleOff(this);\"><center><img src=\"/imagens/money_g.gif\" style=\"cursor:pointer; width:15px;\"></center></a>'
                        END as pagamento,
                        pag.pagvalorparcela,
                        pag.probanco || ' ' AS probanco,
                        pag.proagencia || ' ' AS proagencia,
                        pag.nu_conta_corrente || ' ' AS nu_conta_corrente,

                        (SELECT
                                dfi.dfisaldoconta + dfi.dfisaldofundo + dfi.dfisaldopoupanca + dfi.dfisaldordbcdb
                            FROM painel.dadosfinanceirosconvenios dfi
                            WHERE dfiprocesso = p_conv.pronumeroprocesso
                            ORDER BY dfi.dfidatasaldo DESC LIMIT 1) as saldo_conta,

                        (SELECT
                                dfi.dfimesanosaldo
                            FROM painel.dadosfinanceirosconvenios dfi
                            WHERE dfiprocesso = p_conv.pronumeroprocesso
                            ORDER BY dfi.dfidatasaldo DESC LIMIT 1) as ref_mes,

                    '<center>'||v.vldstatushomologacao||'</center>' as vldstatushomologacao,
                    '<center>' || CASE WHEN (v.vldobshomologacao IS NULL or v.vldobshomologacao = '') THEN 'Sem observações' ELSE v.vldobshomologacao END || '</center>' as observacao,
                    '<center>' || v.vldstatus25exec ||'</center>' as vldstatus25exec,
                    '<center>' || CASE WHEN v.vldobs25exec IS NULL THEN 'Sem observações' ELSE v.vldobs25exec END ||'</center>' as vldobs25exec,
                    '<center>' || v.vldstatus50exec ||'</center>' as vldstatus50exec,
                    '<center>' || CASE WHEN v.vldobs50exec IS NULL THEN 'Sem observações' ELSE v.vldobs50exec END ||'</center>' as vldobs50exec,
                    CASE WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_inativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_alerta.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_ativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_concluido.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    ELSE '<img align=\"absmiddle\" src=\"/imagens/0_inexistente.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;' END as CA

                    ,

                    CASE WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_inativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_alerta.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_ativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_concluido.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    ELSE '<img align=\"absmiddle\" src=\"/imagens/0_inexistente.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;' END as CV

                    ,

                    CASE WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_inativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_alerta.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_ativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_concluido.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    ELSE '<img align=\"absmiddle\" src=\"/imagens/0_inexistente.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;' END as CT

                    ,

                    CASE WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_inativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_alerta.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_ativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    WHEN (SELECT COUNT(*)
                        FROM obras2.restricao r
                        JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                        WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = oi.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_concluido.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
                    ELSE '<img align=\"absmiddle\" src=\"/imagens/0_inexistente.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;' END as Outros
            ,
                    ";
        }

        $sql = "SELECT DISTINCT ON (oi.obrid)
		    oi.obrid,
		    oi.preid,
		    ent.entnome,
                    --'(' || oi.obrid || ') ' || oi.obrnome,
                    {$nome_obra}



            p_conv.termo_convenio as nr_convenio,
            p_conv.pronumeroprocesso AS proc,



            esd.esddsc,
		    {$campos}
		    too.toodescricao,
		    pf.prfdesc,
		    tpo.tpodsc,
		    u.usunome AS usunome_hom,
		    TO_CHAR(v.vlddtinclusaosthomo, 'DD-MM-YYYY') AS data_hom,
		    u1.usunome AS usunome_25,
		    TO_CHAR(v.vlddtinclusaost25exec, 'DD-MM-YYYY') AS data_25,
		    u2.usunome AS usunome_50,
		    TO_CHAR(v.vlddtinclusaost50exec, 'DD-MM-YYYY') AS data_50,

            to_char(pg.pagdatapagamento, 'DD/MM/YYYY') ,
            pg.pagsituacaopagamento,

            perc.perc_pag,
            t.\"Fim Vigência da Obra\" as vig_obra,
            CASE WHEN t.\"Fim Vigência Termo\" IS NULL AND oi.tooid = 2 THEN TO_CHAR(dc.dcodatafim,'DD/MM/YYYY')::text ELSE t.\"Fim Vigência Termo\" END as vig_termo,
            t.\"Início Vigência Termo\" as ini_termo

		FROM
			obras2.empreendimento ep

		INNER JOIN obras2.obras oi ON oi.empid = ep.empid
		LEFT JOIN painel.dadosconvenios dc on dc.dcoprocesso = Replace(Replace(Replace(oi.obrnumprocessoconv,'.',''),'/',''),'-','')

        LEFT JOIN obras2.vm_termo_obras t ON t.\"Preid\" = oi.preid
		LEFT JOIN (
			SELECT MAX(pag.pagid) pagid, po.preid
			FROM par.pagamento pag
			JOIN par.pagamentoobra  po ON po.pagid = pag.pagid
			WHERE pag.pagstatus = 'A' AND pag.pagsituacaopagamento <> 'CANCELADO'
			GROUP BY po.preid
			UNION
			SELECT MAX(pag.pagid) pagid, po.preid
			FROM par.pagamento pag
			JOIN par.pagamentoobrapar  po ON po.pagid = pag.pagid
			WHERE pag.pagstatus = 'A' AND pag.pagsituacaopagamento <> 'CANCELADO'
			GROUP BY po.preid
		) po ON po.preid = oi.preid
		LEFT JOIN par.pagamento pg ON pg.pagid = po.preid

		LEFT JOIN workflow.documento doc ON doc.docid = oi.docid

		LEFT JOIN (SELECT
					SUM(p.pagvalorparcela) AS pagvalorparcela,
					probanco,
					proagencia,
					nu_conta_corrente,
					emo.preid
				   FROM
					par.empenhoobra emo
					INNER JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A'
					INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
					INNER JOIN par.pagamento p ON p.empid = emo.empid AND
												 p.pagstatus='A'
				   GROUP BY
				   	emo.preid, pro.probanco, pro.proagencia, pro.nu_conta_corrente) pag ON pag.preid = oi.preid

        LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = oi.obrid

        LEFT JOIN entidade.entidade                ent ON ent.entid  = ep.entidunidade
        LEFT JOIN entidade.endereco                 ed ON ed.entid   = ent.entid
        LEFT JOIN entidade.endereco                edo ON edo.endid  = oi.endid
        LEFT JOIN territorios.municipio                 mun ON mun.muncod = ed.muncod
        LEFT JOIN obras2.validacao                   v ON v.obrid    = oi.obrid
        LEFT JOIN seguranca.usuario                  u ON u.usucpf   = v.usucpf_homo
        LEFT JOIN seguranca.usuario                 u1 ON u1.usucpf  = v.usucpf_25
        LEFT JOIN seguranca.usuario                 u2 ON u2.usucpf  = v.usucpf_50
        LEFT JOIN obras2.tipoorigemobra            too ON too.tooid  = oi.tooid
        LEFT JOIN obras2.programafonte              pf on pf.prfid   = ep.prfid
        LEFT JOIN workflow.documento                 d ON d.docid    = oi.docid
        LEFT JOIN workflow.estadodocumento         esd ON esd.esdid  = d.esdid
        LEFT JOIN obras2.tipologiaobra          tpo ON tpo.tpoid  = oi.tpoid
        LEFT JOIN (

                       SELECT
                            CASE WHEN pre.prevalorobra != 0 THEN ROUND((pag.popvalorpagamento / pre.prevalorobra) * 100) ELSE 0 END as perc_pag, pre.preid
                        FROM (

                        SELECT
                           sum(pao.popvalorpagamento) as popvalorpagamento, pao.preid
                        FROM par.pagamentoobrapar pao
                        INNER JOIN par.pagamento pag1 ON pag1.pagid = pao.pagid
                        WHERE pag1.pagstatus = 'A' AND pag1.pagsituacaopagamento <> '%CANCELADO%'
                        GROUP BY pao.preid

                        ) pag
                        INNER JOIN obras.preobra pre ON pre.preid = pag.preid

                        UNION


                        SELECT
                            CASE WHEN pre.prevalorobra != 0 THEN ROUND((pag.pobvalorpagamento / pre.prevalorobra) * 100) ELSE 0 END as perc_pag, pre.preid
                        FROM (

                        SELECT
                           sum(pao.pobvalorpagamento) as pobvalorpagamento, pao.preid
                        FROM par.pagamentoobra pao
                        INNER JOIN par.pagamento pag1 ON pag1.pagid = pao.pagid
                        WHERE pag1.pagstatus = 'A' AND pag1.pagsituacaopagamento <> '%CANCELADO%'
                        GROUP BY pao.preid

                        ) pag
                        INNER JOIN obras.preobra pre ON pre.preid = pag.preid


                   ) as perc ON perc.preid = oi.preid
		    $innerArquivo
            $innerOsArquivo
		WHERE
			oi.obrstatus = 'A' AND ep.empstatus = 'A' AND oi.obridpai IS NULL ".( !empty($where) ? ' AND ' . implode(' AND ', $where) : '' )." order by oi.obrid";

        return $sql;
    }
}
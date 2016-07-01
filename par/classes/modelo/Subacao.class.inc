<?php
	
class Subacao extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.subacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sbaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sbaid' => null, 
									  	'sbadsc' => null, 
									  	'sbaordem' => null,
    									'sbaobra' => null,
									    'sbaestrategiaimplementacao' => null,
    									'sbaptres' => null,
    									'sbanaturezadespesa' => null,
    									'sbamonitoratecnico' => null,
    									'docid' => null,
    									'frmid' => null,
									    'indid' => null,
    									'foaid' => null,
									    'undid' => null,
    									'ptsid' => null,
    									'ppsid' => null,
									    'aciid' => null,
									    'sbacronograma' => null,
									    'sbaextraordinaria' => null,
    									'prgid' => null,
    									'sbastatus' => null,
    									'sbaextraordinariatramita' => null
									  );

	public function deletaPorPtoid($ptoid)
	{
		if( $_SESSION['par']['boExcluiSubacao'] == 'S' ){
			$this->executar("DELETE FROM par.subacao WHERE aciid in(select aciid FROM par.acao WHERE ptoid = {$ptoid})");
		} else {
	 		$this->executar("UPDATE par.subacao SET sbastatus = 'I' WHERE aciid in(select aciid FROM par.acao WHERE acistatus = 'A' AND ptoid = {$ptoid})");
		}
	}
	
	public function listaSubAcaoPorAcao($aciid)
	{
		$sql = "select
					'<img src=\"../imagens/alterar.gif\" title=\"Listar Subacao\" style=\"cursor:pointer\" onclick=\"listarSubacao(\'' || sbaid || '\')\"  />' as acao,
					sbadsc,
					COALESCE(frmdsc,'N/A') as frmdesc,
					COALESCE(unddsc,'N/A') as unddsc
				from
					par.subacao sba
				left join
					par.formaexecucao frm on frm.frmid = sba.frmid
				left join
					par.unidademedida und on und.undid = sba.undid
				where
					aciid = $aciid AND
					sba.sbastatus = 'A'
				order by
					sbaordem,sbadsc";
		$cabecalho = array("Ação","Subação","Forma de Execução","Unidade de Medida");
		$this->monta_lista($sql,$cabecalho,100,10,"N","center");
	}
	
	public function carregaSubAcaoPorAcao($aciid)
	{
		$sql = "select
					sbaid,
					sbadsc
				from
					par.subacao
				where
					aciid = $aciid AND
					sba.sbastatus = 'A'
				order by
					sbaordem,sbadsc";
		return $this->carregar($sql);
	}
        
        public function listarSubacaoPorDopid($arrParam, $booSQL = FALSE) {
            $arrPost = $_SESSION['par']['validacao_execucao'];

            $arrWhere = array();
            $arrJoin = array();
            $arrJoinPrincipal = array();

            #Join em comum
            if ($arrPost['comnota'] != 'T' || $arrPost['comcontrato'] != 'T') {
                $arrJoin[] = "INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icoano = sd.sbdano AND sic.icostatus = 'A'";
                $arrJoinPrincipal[] = "INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icoano = sd.sbdano AND sic.icostatus = 'A'";
            }

            #Com Nota
            if ($arrPost['comnota'] != 'T') {
                if ($arrPost['comnota'] == 'S') {
                    $arrJoin[] = "INNER JOIN par.subacaoitenscomposicaonotasfiscais nf ON nf.icoid = sic.icoid AND nf.scnstatus = 'A'";
                    $arrJoinPrincipal[] = "INNER JOIN par.subacaoitenscomposicaonotasfiscais nf ON nf.icoid = sic.icoid AND nf.scnstatus = 'A'";
                    #Com notavalidada
                    if ($arrPost['comnotavalidada'] != 'T') {
                        if ($arrPost['comnotavalidada'] == 'S') {
                            $arrJoin[] = "INNER JOIN par.notasfiscais ntf ON ntf.ntfid = nf.ntfid AND ntfvalidado = 't' AND ntfstatus = 'A'";
                            $arrJoinPrincipal[] = "INNER JOIN par.notasfiscais ntf ON ntf.ntfid = nf.ntfid AND ntfvalidado = 't' AND ntfstatus = 'A'";
                        } else {
                            $arrJoin[] = "INNER JOIN par.notasfiscais ntf ON ntf.ntfid = nf.ntfid AND ntfstatus = 'A'";
                            $arrJoinPrincipal[] = "INNER JOIN par.notasfiscais ntf ON ntf.ntfid = nf.ntfid AND ntfstatus = 'A'";
                        }
                    }
                } else{
                    $arrJoin[] = "LEFT JOIN par.subacaoitenscomposicaonotasfiscais nf ON nf.icoid = sic.icoid AND nf.scnstatus = 'A'";
                    $arrJoinPrincipal[] = "LEFT JOIN par.subacaoitenscomposicaonotasfiscais nf ON nf.icoid = sic.icoid AND nf.scnstatus = 'A'";
                    $arrWhere[] = "(SELECT count(scnid) FROM par.subacaoitenscomposicaonotasfiscais WHERE icoid = sic.icoid) = 0";
                }
            }

            #Com Contrato
            if ($arrPost['comcontrato'] != 'T') {
                if ($arrPost['comcontrato'] == 'S') {
                    $arrJoin[] = "INNER JOIN par.subacaoitenscomposicaocontratos sif ON sif.icoid = sic.icoid AND sif.sccstatus = 'A'";
                    $arrJoinPrincipal[] = "INNER JOIN par.subacaoitenscomposicaocontratos sif ON sif.icoid = sic.icoid AND sif.sccstatus = 'A'";
                } else{
                    $arrJoin[] = "LEFT JOIN par.subacaoitenscomposicaocontratos sif ON sif.icoid = sic.icoid AND sif.sccstatus = 'A'";
                    $arrJoinPrincipal[] = "LEFT JOIN par.subacaoitenscomposicaocontratos sif ON sif.icoid = sic.icoid AND sif.sccstatus = 'A'";
                    $arrWhere[] = "(SELECT count(sccid) FROM par.subacaoitenscomposicaocontratos WHERE icoid = sic.icoid) = 0";
                }
            }

            #valor
            if ($arrPost['vlrliberado'] != 'T') {
                if ($arrPost['vlrliberado'] == 'S') {
                    $arrJoin[] = "INNER JOIN par.subacaodetalhe_liberacaopagamento sdlp ON sdlp.sbdid = sd1.sbdid AND sdlp.sdpstatus = 'A'";
                    $arrJoinPrincipal[] = "INNER JOIN par.subacaodetalhe_liberacaopagamento sdlp ON sdlp.sbdid = sd.sbdid AND sdlp.sdpstatus = 'A'";
                } else{
                    $arrJoin[] = "LEFT JOIN par.subacaodetalhe_liberacaopagamento sdlp ON sdlp.sbdid = sd1.sbdid AND sdlp.sdpstatus = 'A'";
                    $arrJoinPrincipal[] = "LEFT JOIN par.subacaodetalhe_liberacaopagamento sdlp ON sdlp.sbdid = sd.sbdid AND sdlp.sdpstatus = 'A'";
                    $arrWhere[] = "(SELECT count(sdpid) FROM par.subacaodetalhe_liberacaopagamento WHERE sbdid = sd.sbdid) = 0";
                }
            }

            #Pagamentos
            if ($arrPost['pagamento'] != 'T') {
                if ($arrPost['pagamento'] == 'sempagamento') {
                    $arrWhere[] = "(
                     SELECT
                        COALESCE(ps.pobvalorpagamento, 0) AS valor
                    FROM par.processopar p
                    INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = p.prpid
                    INNER JOIN par.subacaodetalhe sd1 ON sd1.sbdid = ppc.sbdid ". implode(' ',$arrJoin )."
                    LEFT JOIN par.empenho e
                        INNER JOIN par.pagamento pag ON pag.empid = e.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('EFETIVADO', '2 - EFETIVADO', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA')
                        INNER JOIN par.pagamentosubacao ps ON ps.pagid = pag.pagid
                    ON e.empnumeroprocesso = p.prpnumeroprocesso AND e.empstatus = 'A' AND ps.sbaid = sd.sbaid
                    WHERE sd1.sbdid = sd.sbdid AND (ps.pobvalorpagamento IS NULL OR ps.pobvalorpagamento = 0) LIMIT 1
                ) = 0";
                } else{
                    $arrWhere[] = "(
                     SELECT
                        COALESCE(ps.pobvalorpagamento, 0) AS valor
                    FROM par.processopar p
                    INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = p.prpid
                    INNER JOIN par.subacaodetalhe sd1 ON sd1.sbdid = ppc.sbdid ". implode(' ',$arrJoin )."
                    LEFT JOIN par.empenho e
                        INNER JOIN par.pagamento pag ON pag.empid = e.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('EFETIVADO', '2 - EFETIVADO', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA')
                        INNER JOIN par.pagamentosubacao ps ON ps.pagid = pag.pagid
                    ON e.empnumeroprocesso = p.prpnumeroprocesso AND e.empstatus = 'A' AND ps.sbaid = sd.sbaid
                    WHERE sd1.sbdid = sd.sbdid AND (ps.pobvalorpagamento IS NOT NULL OR ps.pobvalorpagamento > 0)  AND (ps.pobvalorpagamento < (SELECT saldo FROM par.v_saldo_empenho_por_subacao WHERE sbaid = sd.sbaid AND eobano = sd.sbdano LIMIT 1)) LIMIT 1
                ) > 0 ";
                }
            }else {
                $arrWhere[] = "(
                     SELECT
                            foo.valor_pago
                     FROM(
                             SELECT
                                COALESCE(ps.pobvalorpagamento, 0) AS valor_pago,
                                (SELECT SUM(saldo) FROM par.v_saldo_empenho_por_subacao WHERE sbaid = sd.sbaid AND eobano = sd.sbdano) as valor_empenho
                            FROM par.processopar p
                            INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = p.prpid
                            INNER JOIN par.subacaodetalhe sd1 ON sd1.sbdid = ppc.sbdid ". implode(' ',$arrJoin )."
                            LEFT JOIN par.empenho e
                            INNER JOIN par.pagamento pag ON pag.empid = e.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('EFETIVADO', '2 - EFETIVADO', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA')
                            INNER JOIN par.pagamentosubacao ps ON ps.pagid = pag.pagid
                            ON e.empnumeroprocesso = p.prpnumeroprocesso AND e.empstatus = 'A' AND ps.sbaid = sd1.sbaid
                            WHERE sd1.sbdid = sd.sbdid
                     ) AS foo WHERE (foo.valor_pago < foo.valor_empenho) LIMIT 1
            )Is NOT NULL ";
            }

            #TR
            $strDiv = "'</td></tr><tr id=\"tr_'|| sd.sbdid ||'\" style=\"display:none\"><td id=\"td_'|| sd.sbdid ||'\" colspan=\"11\" background=\"#CEE0EF\"></td></tr>'";

            $strAnd = (count($arrWhere) > 0) ? ' AND ' : '';

            $sql = "SELECT DISTINCT  
                        CASE WHEN s.sbaid  IS NOT NULL THEN -- quer dizer que ele perdeu itens.
                                '<center>
                                        <img style=\"cursor:pointer\" src=\"/imagens/consultar.gif\" onclick=\"listarSubacao(\''||s.sbaid||'\', \'' || sd.sbdano || '\', {$arrParam['inuid']})\" border=0 />
                                        <img style=\"cursor:pointer\" src=\"/imagens/mais.gif\" onclick=\"carregarSubacaoItensComposicao(this.id, '|| sd.sbdid ||', '|| dp.dopid ||')\" border=0 id='|| sd.sbdid ||' />
                                </center>'
                        END as acao,  
                        '<center>' || par.retornacodigosubacao(s.sbaid) || '</center>'/* || s.sbadsc*/,
                        sd.sbdvaloraprovado::numeric(20,2) as valor_subacao,
                        (
                             ARRAY_TO_STRING(array(
                                SELECT
                                   '<center><a onmouseover=\"SuperTitleAjax(\'par.php?modulo=principal/acompanhamento/validacaoExecucao&acao=A&requisicao=SuperTitleAjax&sbaid='||ve.sbaid||'&empid='||ve.empid||'&sbdano='||sd.sbdano||'\',this); removeTitle(jQuery(this))\" onmouseout=\"SuperTitleOff(this)\">'|| e.empnumero ||'</a></center>'
                                FROM par.v_saldo_empenho_por_subacao  ve
                                INNER JOIN par.empenho e ON e.empid = ve.empid
                                WHERE ve.sbaid = s.sbaid AND ve.eobano = sd.sbdano)
                                , ' '
                            )
                        ) as nota_empenho,
                        (SELECT sum(saldo) FROM par.v_saldo_empenho_por_subacao v WHERE v.sbaid = s.sbaid AND v.eobano = sd.sbdano)::numeric(20,2) as valor_empenho,
                        (SELECT par.retornavalorpagosubacao(s.sbaid, sd.sbdano))::numeric(20,2) as valor_pago,
                        (
                            SELECT
                                COALESCE(SUM(foo.valor)::numeric(20,2),0.00)
                            FROM (
                                SELECT DISTINCT
                                    icoquantidaderecebida * icovalor as valor
                                FROM par.subacaoitenscomposicaocontratos sicc
                                INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid=sicc.icoid AND sic.icostatus = 'A'
                                WHERE sic.sbaid = s.sbaid AND sicc.sccstatus = 'A'
                            ) AS foo
                        ) as vlr_contrato_item,
                        (
                            SELECT
                                COALESCE(SUM(foo.convlrcontrato)::numeric(20,2),0.00)
                            FROM (
                            SELECT
                                con.convlrcontrato
                            FROM par.subacaoitenscomposicaocontratos sicc
                            INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid=sicc.icoid AND sic.icostatus = 'A'
                            INNER JOIN par.contratos con ON con.conid=sicc.conid AND con.constatus = 'A'
                            WHERE sic.sbaid = s.sbaid AND sicc.sccstatus = 'A' GROUP BY con.conid
                            ) AS foo
                        ) as valor_contrato,
                        (
                            SELECT DISTINCT
                                COALESCE(SUM(foo.valor)::numeric(20,2),0.00)
                            FROM (
                                SELECT DISTINCT
                                    icoquantidaderecebida * icovalor as valor
                                FROM par.subacaoitenscomposicaonotasfiscais sicnf
                                INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid=sicnf.icoid AND sic.icostatus = 'A'
                                WHERE sic.sbaid = s.sbaid AND sicnf.scnstatus = 'A'
                            ) AS foo
                        ) as vlr_nota_item,
                        (
                            SELECT
                                COALESCE(SUM(foo.ntfvlrnota)::numeric(20,2),0.00)
                            FROM (
                                SELECT
                                    nf.ntfvlrnota
                                FROM par.subacaoitenscomposicaonotasfiscais sicnf
                                INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid=sicnf.icoid AND sic.icostatus = 'A'
                                INNER JOIN par.notasfiscais nf ON nf.ntfid=sicnf.ntfid AND nf.ntfstatus = 'A'
                                WHERE sic.sbaid = s.sbaid AND sicnf.scnstatus = 'A' GROUP BY nf.ntfid
                            ) AS foo
                        ) as valor_nota,
                        ((SELECT SUM(saldo) FROM par.v_saldo_empenho_por_subacao v WHERE v.sbaid = s.sbaid AND v.eobano = sd.sbdano) - (SELECT par.retornavalorpagosubacao(s.sbaid,sd.sbdano)))::numeric(20,2) as vlr_maximo_liberar,
                        (SELECT COALESCE(sdpvalortotal, 0.00) FROM par.subacaodetalhe_liberacaopagamento WHERE sbdid = sd.sbdid AND sdpstatus = 'A' LIMIT 1)::numeric(20,2) as ultimo_valor,
                        CASE WHEN ((SELECT par.retornavalorpagosubacao(s.sbaid, sd.sbdano)) >= (SELECT sum(saldo) as valor_emp FROM par.v_saldo_empenho_por_subacao v WHERE v.sbaid = s.sbaid AND v.eobano = sd.sbdano))
                            THEN '<center>100% Pago</center>'
                            ELSE '<input type=\"hidden\" value=\"'||((SELECT sum(saldo) FROM par.v_saldo_empenho_por_subacao v WHERE v.sbaid = s.sbaid AND v.eobano = sd.sbdano) - (SELECT par.retornavalorpagosubacao(s.sbaid,sd.sbdano)))||'\" name=\"vlr_dif_'||sd.sbdid||'\">
                            <input type=\"text\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'[.###],##\',this.value); \" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" sbdid=\"'|| sd.sbdid ||'\" class=\"'||dp.dopid||'\" />'
                        END as valor_liberar,
                       $strDiv as div
                FROM par.termocomposicao tc
                INNER JOIN par.documentopar dp ON dp.dopid=tc.dopid AND dp.dopstatus = 'A'
                INNER JOIN par.subacaodetalhe sd ON sd.sbdid = tc.sbdid
                INNER JOIN par.subacao s ON s.sbaid = sd.sbaid AND s.sbastatus = 'A' ". implode(' ',$arrJoinPrincipal )."
                WHERE tc.dopid = {$arrParam['dopid']}
                    AND ((SELECT par.retornavalorpagosubacao(s.sbaid, sd.sbdano)) >= (SELECT sum(saldo) as valor_emp FROM par.v_saldo_empenho_por_subacao v WHERE v.sbaid = s.sbaid AND v.eobano = sd.sbdano)) IS FALSE
                    {$strAnd} ". implode(' AND ', $arrWhere);
//ver(simec_htmlentities($sql), d);
            return ($booSQL) ? $sql : $this->carregar($sql);
        }
            
}									  
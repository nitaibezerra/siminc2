<?php

class ProcessoPar extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.processopar";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("prpid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'prpid' => null,
        'prpid' => null,
        'prpnumeroprocesso' => null,
        'muncod' => null,
        'prpbanco' => null,
        'prpagencia' => null,
        'prpdatainclusao' => null,
        'usucpf' => null,
        'prpseqconta' => null,
        'prptipo' => null,
        'seq_conta_corrente' => null,
        'nu_conta_corrente' => null,
        'inuid' => null,
        'prptipoexecucao' => null,
        'prpnumeroconveniofnde' => null,
        'prpanoconveniofnde' => null,
        'prpnumeroconveniosiafi' => null,
        'prpdocumenta' => null,
        'prpcnpj' => null,
        'sisid' => null,
        'prpstatus' => null,
        'dt_movimento' => null,
        'fase_solicitacao' => null,
        'co_situacao_conta' => null,
        'situacao_conta' => null,
        'nu_identificador' => null,
        'ds_razao_social' => null,
        'prpgeraproc' => null,
        'arqidanexodoc' => null,
        'prpstatusmotivo' => null,
        'prpdataconsultasigef' => null,
        'prpfinalizado' => null,
        'prpdatapagamentosigef' => null
    );

    public function relatorioProrrogacaoParGenerico($arrPost = array(), $booSQL = FALSE) {
        #Processos ativos
        $arrWhere = array("prp.prpstatus = 'A'");

        #processo
        if (!empty($arrPost['prpnumeroprocesso'])) {
            $arrWhere[] = "prp.prpnumeroprocesso = '{$arrPost['prpnumeroprocesso']}'";
        }

        #vigencia de
        if (!empty($arrPost['dtvigenciainicio'])) {
            $arrWhere[] = "to_char(to_timestamp('01/'||dop.dopdatafimvigencia, 'DD/MM/YYYY')::date, 'YYYYMM')::numeric >= to_char('{$arrPost['dtvigenciainicio']}'::date, 'YYYYMM')::numeric ";
        }

        #vigencia ate
        if (!empty($arrPost['dtvigenciafim'])) {
            $arrWhere[] = "to_char(to_timestamp('01/'||dop.dopdatafimvigencia, 'DD/MM/YYYY')::date, 'YYYYMM')::numeric <= to_char('{$arrPost['dtvigenciafim']}'::date, 'YYYYMM')::numeric ";
        }

        #municipio
        if ($arrPost['municipio'][0]) {
            $arrWhere[] = "AND inu.muncod IN ('" . implode("','", $arrPost['municipio']) . "')";
        }

        #UF
        if ($arrPost['estuf'][0]) {
            $arrWhere[] = "( inu.estuf IN ('" . implode("','", $arrPost['estuf']) . "') OR mun.estuf IN ('" . implode("','", $arrPost['estuf']) . "') )";
        }

        #Numero documento
        if (!empty($arrPost['dopnumerodocumento'])) {
            $arrWhere[] = "dop.dopnumerodocumento = {$arrPost['dopnumerodocumento']}";
        }

        #tratamento de regra para exibicao de tela
        if ($arrPost['acaoBotao'] == 'btnPesquisar') {
            $acoes = "'<img src=\"../imagens/icone_lupa.png\" style=\"cursor:pointer;\" title=\"Visualizar Termo\" onclick=\"carregaTermoMinuta('||dop.dopid||');\" border=\"0\">&nbsp;'||";
            $mais = "CASE WHEN prp.prpfinalizado IS NULL 
                            THEN '<img  style=\"width:18px;height: 18px;\" border=\"0\" title=\"Processo Vigente\" src=\"../imagens/par/cadeado-aberto.png\">' 
                            ELSE '<span title=\"Processo Finalizado\" style=\"font-size:15px; color:black;\" class=\"glyphicon glyphicon-lock\"></span>' 
                    END AS mais,";
        }
        
        $processo = "'<span class=\"processo_detalhe\" >' || to_char(prpnumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>'";
        if (isset($arrPost['btnExcel'])) {
            $processo = "(TO_CHAR(prpnumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00'))";
        }
        #sql principal
        $sql = "SELECT DISTINCT
                    $mais
                    $processo AS processo,
                    dop.dopnumerodocumento AS termo,
                    $acoes mdo.mdonome AS nomeDocumento,
                    coalesce(mun.estuf, inu.estuf) AS estado,
                    mun.mundescricao AS municipio,
                    CAST(dop.dopvalortermo as numeric(20,2)) AS dopvalortermo,
                    em.valor AS valorempenho,
                    pm.valor_pagamentosolicitado AS valorpagamentosolicitado,
                    pm.valor_pagamentoefetivado AS valorpagamento,
                    dop.dopdatafimvigencia AS vigencia,
                    COALESCE( par.retornasaldoprocesso(prp.prpnumeroprocesso),'Não informado') as saldo_bancario,
    (
    
                SELECT 
                    COUNT(icoid) || ' de ' || SUM(contrato)
                FROM (
                               SELECT DISTINCT 
                                   sic.icoid as icoid,
                                   CASE WHEN EXISTS ( SELECT icoid FROM par.subacaoitenscomposicaocontratos sicc WHERE sicc.icoid = sic.icoid) 
                                               THEN 1
                                               ELSE 0
                                   END as contrato 
                               FROM par.subacaoitenscomposicao sic
                               INNER JOIN par.subacao s ON s.sbaid = sic.sbaid AND s.sbastatus = 'A'
                               INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
                               INNER JOIN par.processoparcomposicao ppc ON ppc.sbdid = sd.sbdid AND ppc.ppcstatus = 'A'
                               INNER JOIN par.processopar pp ON pp.prpid = ppc.prpid AND pp.prpstatus = 'A'
                               WHERE pp.prpnumeroprocesso = prp.prpnumeroprocesso
                                   AND sic.icoano = sd.sbdano
                                   AND icostatus = 'A'
                ) as foo
    ) as contrato
                FROM par.processopar prp
                INNER JOIN par.documentopar dop ON dop.prpid = prp.prpid AND dop.dopstatus = 'A'
                INNER JOIN par.modelosdocumentos mdo ON mdo.mdoid = dop.mdoid AND mdo.tpdcod IN(21, 102)
                INNER JOIN par.instrumentounidade inu ON inu.inuid = prp.inuid
                LEFT  JOIN territorios.municipio mun ON mun.muncod = inu.muncod
                LEFT JOIN ( 
                    SELECT
                        dopid,
                        sum(valor) as valor
                    FROM (
                        SELECT DISTINCT
                            d.dopid, 
                            vve.saldo as valor
                        FROM par.documentopar d
                        INNER JOIN par.processopar prp ON prp.prpid = d.prpid and prp.prpstatus = 'A'
                        INNER JOIN par.empenho emp ON emp.empnumeroprocesso = prp.prpnumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
                        INNER JOIN par.vm_saldo_empenho_do_processo vve ON vve.processo = prp.prpnumeroprocesso
                    ) as foo
                    GROUP BY dopid
                ) em ON em.dopid = dop.dopid
                LEFT JOIN (
                    SELECT 
                        prp.prpid, 
                        CASE WHEN pag.pagsituacaopagamento  != '2 - EFETIVADO' THEN sum( ps.pobvalorpagamento ) END as valor_pagamentosolicitado,
                        CASE WHEN pag.pagsituacaopagamento = '2 - EFETIVADO' THEN sum( ps.pobvalorpagamento ) END as valor_pagamentoefetivado
                    FROM par.processopar prp
                    INNER JOIN par.empenho emp ON emp.empnumeroprocesso = prp.prpnumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
                    INNER JOIN par.pagamento pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND  pag.pagsituacaopagamento IN ('8 - SOLICITAÇÃO APROVADA', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', 'Enviado ao SIGEF', '2 - EFETIVADO') 
                    INNER JOIN par.pagamentosubacao ps ON ps.pagid = pag.pagid AND pobstatus = 'A'
                    GROUP BY prp.prpid, pagsituacaopagamento
                ) pm ON prp.prpid = pm.prpid
                WHERE " . implode(' AND ', $arrWhere) . " ORDER BY 3,4";
        
        return ($booSQL) ? $sql : $this->carregar($sql);
    }
    
    public function relatorioLiberacaoPagamento($arrPost = array(), $booSQL = FALSE) {

        $_SESSION['par']['validacao_execucao'] = $arrPost;

        #Processos ativos
        $arrWhere = array();
        $arrJoin = array();

        #esfera
        if ($arrPost['esfera'] != 'T') {
            if ($arrPost['esfera'] == 'municipal') {
                $arrWhere[] = "iu.itrid = 2";
            }else {
                $arrWhere[] = "iu.itrid = 1";
            }
        }
        
        #processo
        if (!empty($arrPost['prpnumeroprocesso'])) {
            $arrWhere[] = "prp.prpnumeroprocesso = '{$arrPost['prpnumeroprocesso']}'";
        }
        
        #dopnumerodocumento
        if (!empty($arrPost['dopnumerodocumento'])) {
            $arrWhere[] = "dop.dopnumerodocumento = '{$arrPost['dopnumerodocumento']}'";
        }

        #municipio
        if ($arrPost['listaMunicipio'][0]) {
            $arrWhere[] = "iu.muncod IN ('" . implode("','", $arrPost['listaMunicipio']) . "')";

        }

        #UF
        if ($arrPost['estuf'][0]) {
            $arrWhere[] = "(iu.estuf IN ('" . implode("','", $arrPost['estuf']) . "') OR iu.mun_estuf IN ('" . implode("','", $arrPost['estuf']) . "'))";
        }
        
        #Com TR
        if ($arrPost['comtr'] != 'T') {
            if ($arrPost['comtr'] == 'S') {
                $arrWhere[] = "(SELECT COUNT(atpid) FROM par.anexotorpar WHERE dopid = dop.dopid AND atpstatus = 'A') > 0";
            } else {
                $arrWhere[] = "(SELECT COUNT(atpid) FROM par.anexotorpar WHERE dopid = dop.dopid AND atpstatus = 'A') = 0";
            }
        }

        #Join em comum
        if ($arrPost['comnota'] != 'T' || $arrPost['comcontrato'] != 'T') {
            $arrJoin[] = "INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icoano = sd.sbdano AND sic.icostatus = 'A'";
        }

        #Com Nota
        if ($arrPost['comnota'] != 'T') {
            if ($arrPost['comnota'] == 'S') {
                $arrJoin[] = "INNER JOIN par.subacaoitenscomposicaonotasfiscais nf ON nf.icoid = sic.icoid AND nf.scnstatus = 'A'";
                #Com notavalidada
                if ($arrPost['comnotavalidada'] != 'T') {
                    #Com diligencia
                    $strDiligencia = '';
                    if ($arrPost['comnotadiligenciada'] != 'T') {
                        if ($arrPost['comnotadiligenciada'] == 'S') {
                            $strDiligencia = " AND ntf.ntfdiligencia IS NOT NULL";
                        } else {
                            $strDiligencia = " AND ntf.ntfdiligencia IS NULL";
                        }
                    }
                    
                    #Com notavalidada
                    if ($arrPost['comnotavalidada'] == 'S') {
                        $arrJoin[] = "INNER JOIN par.notasfiscais ntf ON ntf.ntfid = nf.ntfid AND ntfvalidado = 't' AND ntfstatus = 'A' $strDiligencia";
                    } else {
                        $arrJoin[] = "INNER JOIN par.notasfiscais ntf ON ntf.ntfid = nf.ntfid AND ntfstatus = 'A' $strDiligencia";
                    }
                } else {
                    #Com notavalidada
                    if ($arrPost['comnotadiligenciada'] != 'T') {
                        #Com diligencia
                        if ($arrPost['comnotadiligenciada'] == 'S') {
                            $arrJoin[] = "INNER JOIN par.notasfiscais ntf ON ntf.ntfid = nf.ntfid AND ntfstatus = 'A' AND ntf.ntfdiligencia IS NOT NULL";
                        } else {
                            $arrJoin[] = "INNER JOIN par.notasfiscais ntf ON ntf.ntfid = nf.ntfid AND ntfstatus = 'A' AND ntf.ntfdiligencia IS NULL";
                        }
                    }
                }
            } else{
                $arrJoin[] = "LEFT JOIN par.subacaoitenscomposicaonotasfiscais nf ON nf.icoid = sic.icoid AND nf.scnstatus = 'A'";
            }
        }

        #Com Contrato
        if ($arrPost['comcontrato'] != 'T') {
            if ($arrPost['comcontrato'] == 'S') {
                #Com diligencia
                $strDiligencia = '';
                if ($arrPost['comcontratodiligenciado'] != 'T') {
                    if ($arrPost['comcontratodiligenciado'] == 'S') {
                        $strDiligencia = " AND con.condiligencia IS NOT NULL";
                    } else {
                        $strDiligencia = " AND con.condiligencia IS NULL";
                    }
                }
                $arrJoin[] = "INNER JOIN par.subacaoitenscomposicaocontratos sif ON sif.icoid = sic.icoid AND sif.sccstatus = 'A' ";
                $arrJoin[] = "INNER JOIN par.contratos con ON con.conid = sif.conid AND sif.sccstatus = 'A' $strDiligencia";
            } else{
                $arrJoin[] = "LEFT JOIN par.subacaoitenscomposicaocontratos sif ON sif.icoid = sic.icoid AND sif.sccstatus = 'A' ";
            }
        }

        #valor
        if ($arrPost['vlrliberado'] != 'T') {
            if ($arrPost['vlrliberado'] == 'S') {
                $arrJoin[] = "INNER JOIN par.subacaodetalhe_liberacaopagamento sdlp ON sdlp.sbdid = sd.sbdid AND sdlp.sdpstatus = 'A'";
            } else{
                $arrJoin[] = "LEFT JOIN par.subacaodetalhe_liberacaopagamento sdlp ON sdlp.sbdid = sd.sbdid AND sdlp.sdpstatus = 'A'";
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
                    INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid ". implode(' ',$arrJoin )."
                    LEFT JOIN par.empenho e
                        INNER JOIN par.pagamento pag ON pag.empid = e.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('EFETIVADO', '2 - EFETIVADO', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA')
                        INNER JOIN par.pagamentosubacao ps ON ps.pagid = pag.pagid
                    ON e.empnumeroprocesso = p.prpnumeroprocesso AND e.empstatus = 'A' AND ps.sbaid = sd.sbaid
                    WHERE p.prpnumeroprocesso = prp.prpnumeroprocesso AND (ps.pobvalorpagamento IS NULL OR ps.pobvalorpagamento = 0) LIMIT 1
                ) = 0";
            } else{
                $arrWhere[] = "(
                     SELECT
                        COALESCE(ps.pobvalorpagamento, 0) AS valor
                    FROM par.processopar p
                    INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = p.prpid
                    INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid ". implode(' ',$arrJoin )."
                    LEFT JOIN par.empenho e
                        INNER JOIN par.pagamento pag ON pag.empid = e.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('EFETIVADO', '2 - EFETIVADO', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA')
                        INNER JOIN par.pagamentosubacao ps ON ps.pagid = pag.pagid
                    ON e.empnumeroprocesso = p.prpnumeroprocesso AND e.empstatus = 'A' AND ps.sbaid = sd.sbaid
                    WHERE p.prpnumeroprocesso = prp.prpnumeroprocesso AND (ps.pobvalorpagamento IS NOT NULL OR ps.pobvalorpagamento > 0)  AND (ps.pobvalorpagamento < (SELECT saldo FROM par.v_saldo_empenho_por_subacao WHERE sbaid = sd.sbaid AND eobano = sd.sbdano LIMIT 1)) LIMIT 1
                ) > 0 ";
            }
        } else {
            $arrWhere[] = "(
                     SELECT
                            foo.valor_pago
                     FROM(
                             SELECT
                                COALESCE(ps.pobvalorpagamento, 0) AS valor_pago,
                                (SELECT SUM(saldo) FROM par.v_saldo_empenho_por_subacao WHERE sbaid = sd.sbaid AND eobano = sd.sbdano) as valor_empenho
                            FROM par.processopar p
                            INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = p.prpid
                            INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid ". implode(' ',$arrJoin )."
                            LEFT JOIN par.empenho e
                                INNER JOIN par.pagamento pag ON pag.empid = e.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('EFETIVADO', '2 - EFETIVADO', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA')
                                INNER JOIN par.pagamentosubacao ps ON ps.pagid = pag.pagid
                            ON e.empnumeroprocesso = p.prpnumeroprocesso AND e.empstatus = 'A' AND ps.sbaid = sd.sbaid
                            WHERE p.prpnumeroprocesso = prp.prpnumeroprocesso
                     ) AS foo WHERE (foo.valor_pago < foo.valor_empenho) LIMIT 1
            )Is NOT NULL ";
        }
        
        #tratamento de regra para exibicao de tela
        $mais = "'<center>
                    <img style=\"cursor:pointer\" src=\"/imagens/consultar.gif\" onclick=\"visualizarDocumento('|| dop.dopid ||')\" border=0 />
                    <img id=\"'|| dop.dopid ||'\" border=\"0\" style=\"cursor:pointer\" src=\"/imagens/mais.gif\" onclick=\"carregarPainelInterno(this.id,'|| dop.dopid ||', '|| iu.inuid ||');\">
                 </center>'";
        
        #Processo
        $processo = "'<span class=\"processo_detalhe\" >' || to_char(prp.prpnumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>'";
        
        #TR
        $strDiv = "'</td></tr><tr id=\"tr_'|| dop.dopid ||'\" style=\"display:none\"><td id=\"td_'|| dop.dopid ||'\" colspan=\"9\" background=\"#CEE0EF\"></td></tr>'";

        $strAnd = (count($arrWhere)>0) ? ' AND ' : '';
        #sql principal
        $sql = "SELECT DISTINCT
                    $mais  as mais,
                    CASE WHEN iu.estuf IS NOT NULL THEN iu.estuf ELSE iu.mun_estuf END as uf,
                    iu.muncod as ibge,
                    mun.mundescricao as municipio,
                    $processo as processo,
                    dop.dopnumerodocumento as termo,
                    dop.mdonome as tipo_termo,
                    dop.dopvalortermo,
                    $strDiv as div
                FROM par.processopar prp
                INNER JOIN par.vm_documentopar_ativos dop ON dop.prpid = prp.prpid
                INNER JOIN par.instrumentounidade iu ON iu.inuid = prp.inuid
                LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
                WHERE prp.prpstatus = 'A'
                AND tpdcod IN (102,103,21)
                AND ( SELECT valorpago FROM par.vm_valor_pago_por_processo WHERE processo = prp.prpnumeroprocesso) <  (SELECT saldo FROM par.vm_saldo_empenho_do_processo WHERE processo = prp.prpnumeroprocesso)
                " . $strAnd .' '.implode(' AND ', $arrWhere) . "
                ORDER BY uf, municipio" ;

        ver(simec_htmlentities($sql), d);
        return ($booSQL) ? $sql : $this->carregar($sql);
    }

    public function relatorioGeralSubacao($arrPost = array(), $booSQL = FALSE) {
//        ver($arrPost, d);
        #Processos ativos
        $arrWhere = array();
        
        #Nivel
        if (!empty($arrPost['nivel'])) {
            $arrWhere[] = "prp.prpnumeroprocesso = '{$arrPost['prpnumeroprocesso']}'";
        }

        #UF
        if ($arrPost['estuf'][0]) {
            $arrWhere[] = "(iu.estuf IN ('" . implode("','", $arrPost['estuf']) . "') OR iu.mun_estuf IN ('" . implode("','", $arrPost['estuf']) . "'))";
        }
        
        #municipio
        if ($arrPost['muncod'][0]) {
            $arrWhere[] = "iu.muncod IN ('" . implode("','", $arrPost['muncod']) . "')";
        }
        
        #processo
        if (!empty($arrPost['prpnumeroprocesso'])) {
            $arrWhere[] = "prp.prpnumeroprocesso = '{$arrPost['prpnumeroprocesso']}'";
        }
        
        #dopnumerodocumento
        if (!empty($arrPost['dopnumerodocumento'])) {
            $arrWhere[] = "dop.dopnumerodocumento = '{$arrPost['dopnumerodocumento']}'";
        }
        
        #Esfera
        if ($arrPost['esfera'] != 'T') {
            if ($arrPost['esfera'] == 'Municipal') {
                $arrWhere[] = "iu.itrid = 2";
            } else {
                $arrWhere[] = "iu.itrid = 1";
            }
        }
        
        #Pendencia
        /*if ($arrPost['possui_pendencia'] != 'T') {
            if ($arrPost['possui_pendencia'] == 'municipal') {
                $arrWhere[] = "iu.itrid = 2";
            } else {
                $arrWhere[] = "iu.itrid = 1";
            }
        }*/
        
        #Plano Interno
        if ($arrPost['sbdplanointerno'][0]) {
            $arrWhere[] = "sd.sbdplanointerno IN ('" . implode("','", $arrPost['sbdplanointerno']) . "')";
        }
        
        #PTRES
        if ($arrPost['sbdptres'][0]) {
            $arrWhere[] = "sd.sbdptres IN ('" . implode("','", $arrPost['sbdptres']) . "')";
        }
        
        #Status Subacao
        if ($arrPost['ssuid'][0]) {
            $arrWhere[] = "ssu.ssuid IN ('" . implode("','", $arrPost['ssuid']) . "')";
        }
        
        #Subacao
        if ($arrPost['sbaid'][0]) {
            $arrWhere[] = "s.sbaid IN ('" . implode("','", $arrPost['sbaid']) . "')";
        }

        #Termo Validado
        if ($arrPost['termo_validado'] != 'T') {
            if ($arrPost['termo_validado'] == 'S') {
                $arrWhere[] = "dop.dopusucpfvalidacaogestor IS NOT NULL";
            } else {
                $arrWhere[] = "dop.dopusucpfvalidacaogestor IS NULL";
            }
        }
        
        $strAnd = (count($arrWhere)>0) ? ' AND ' : '';
        #sql principal
        $sql = "SELECT 
                    ".implode(',', $arrPost['destino'])."
                FROM (
                    SELECT DISTINCT
                        CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END AS uf,
                        CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.mundescricao || ' - ' || iu.mun_estuf END AS entidade,
                        TO_CHAR(prp.prpnumeroprocesso::bigint, 'FM00000\".\"000000\"\/\"0000\"-\"00') AS processo,
                        dop.dopnumerodocumento AS documento, 
                        CASE WHEN dpv.dpvid IS NOT NULL THEN 'Validado' ELSE 'Não validado' END AS validacao,
                        tpd.tpddsc AS tipo,
                        'acaoorcamentaria' as acaoorcamentaria,
                        con.connumerocontrato,
                        ntf.ntfnumeronotafiscal,
                        'Pagamento Solicitado' as pagvalorparcela,
                        'Valor Empenhado' as valorempenhado,
                        'Valor Pago' as valorpago,
                        ntf.ntfvlrnota,
                        con.convlrcontrato,
                        'Vinculação' as vpsvinculacao,
                        dop.dopvalortermo,
                        dop.dopdatafimvigencia,
                        ssudescricao,
                        par.retornasaldoprocesso(prpnumeroprocesso) AS saldobancario,
                        sd.sbdptres,
                        pag.pagid,
                        de.empnumero,
                        par.retornacodigosubacao(sd.sbaid) AS codigosubacao,
                        s.sbadsc AS subacao,
                        sd.sbdano AS ano,
                        par.recuperavalorvalidadossubacaoporano(sd.sbaid, sd.sbdano) AS valorsubacao,
                        de.saldo AS valorempenho,
                        CASE WHEN pag.pagid IS NOT NULL THEN sum( pobvalorpagamento ) END AS valorpagamento,
                        pag.pagsituacaopagamento AS situacaopagamento,
                        CASE WHEN iu.itrid = 1 THEN 'Estadual' ELSE 'Municipal' END AS esfera,
                        par.recuperaquantidadeitensplanejadossubacaoporano(sd.sbaid, sd.sbdano) AS itensplanejados,
                        par.recuperaquantidadeitensvalidadossubacaoporano(sd.sbaid, sd.sbdano) AS itensvalidados,
                        iue.iuesituacaohabilita habilita,
                        dop.dopdatafimvigencia AS vigencia,
                        CASE WHEN iu.itrid = 1 
                            THEN (SELECT CASE  WHEN COUNT (*) > 0 THEN 'Sim' ELSE 'Não' END FROM obras2.vm_pendencia_obras WHERE estuf = iu.estuf AND empesfera = 'E')
                            ELSE (SELECT CASE  WHEN COUNT (*) > 0 THEN 'Sim' ELSE 'Não' END FROM obras2.vm_pendencia_obras WHERE muncod = iu.muncod AND empesfera = 'M')
                        END AS pendencia_obra,
                        CASE WHEN (SELECT COUNT(s.sbaid) FROM par.subacao s INNER JOIN par.subacaodetalhe sd  ON sd.sbaid = s.sbaid INNER JOIN par.termocomposicao tc  ON tc.sbdid = sd.sbdid WHERE tc.dopid = dop.dopid) > 0 THEN 'Sim' ELSE 'Não' END AS contrato_inserido,
                        CASE WHEN (SELECT COUNT(sic.sbaid) FROM par.subacaoitenscomposicao sic INNER JOIN par.subacaoitenscomposicaonotasfiscais sicnf ON sicnf.icoid = sic.icoid AND sic.sbaid = s.sbaid ) > 0 THEN 'Sim' ELSE 'Não' END AS nota_fiscal_inserida,
                        (
                            SELECT 
                                count(DISTINCT ses.escid) 
                            FROM par.subacaoescolas ses
                            INNER JOIN par.escolas e ON ses.escid = e.escid 
                            INNER JOIN entidade.entidade ent ON ent.entid = e.entid
                            INNER JOIN par.subacao sba ON sba.sbaid = ses.sbaid
                            INNER JOIN par.acao a ON a.aciid  = sba.aciid
                            INNER JOIN par.pontuacao p ON p.ptoid  = a.ptoid
                            INNER JOIN par.instrumentounidade inu ON inu.inuid = p.inuid
                            WHERE ses.sbaid = sd.sbaid 
                                AND ses.sesano = sd.sbdano AND ( (inu.itrid = 1 AND ent.tpcid = 1) OR (inu.itrid = 2 AND ent.tpcid = 3) )
                                AND (ent.entescolanova = false OR ent.entescolanova IS NULL) AND ent.entstatus = 'A' AND ses.sesstatus = 'A'
                        ) AS qtd_escolas_atendidas
                    FROM  par.instrumentounidade iu
                    INNER JOIN par.processopar prp ON prp.inuid = iu.inuid
                    LEFT JOIN par.instrumentounidadeentidade iue ON iue.iuecnpj = prpcnpj
                    LEFT JOIN par.vm_documentopar_ativos dop 
                        INNER JOIN par.modelosdocumentos mdo ON mdo.mdoid = dop.mdoid
                        INNER JOIN public.tipodocumento tpd ON tpd.tpdcod = mdo.tpdcod
                        LEFT  JOIN par.documentoparvalidacao dpv ON dpv.dopid = dop.dopid 
                    ON dop.prpid = prp.prpid
                    LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
                    INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
                    INNER JOIN par.subacaodetalhe sd  ON sd.sbdid = ppc.sbdid
                    INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icostatus = 'A'
                    LEFT JOIN par.subacaoitenscomposicaocontratos sicc 
                        INNER JOIN par.contratos con ON con.conid = sicc.conid AND con.constatus = 'A' 
                    ON sicc.icoid = sic.icoid AND sicc.sccstatus = 'A'
                    LEFT JOIN par.subacaoitenscomposicaonotasfiscais sicntf 
                        INNER JOIN par.notasfiscais ntf ON ntf.ntfid = sicntf.ntfid AND ntf.ntfstatus = 'A'
                    ON sicntf.icoid = sic.icoid AND sicntf.scnstatus = 'A'
                    INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
                    INNER JOIN par.statussubacao ssu ON ssu.ssuid = sd.ssuid
                    LEFT JOIN par.v_dadosempenhosubacao de  ON de.sbdid = sd.sbdid AND de.processo = prp.prpnumeroprocesso
                    LEFT JOIN par.pagamentosubacao pas ON pas.sbaid = sd.sbaid AND pas.pobano = sd.sbdano and pobstatus = 'A'
                    LEFT JOIN par.pagamento pag ON pag.pagid = pas.pagid AND pag.pagsituacaopagamento NOT ILIKE '%CANCELADO%' AND pag.pagstatus = 'A' AND pag.empid = de.empid
                    WHERE 1=1 ". $strAnd .' '.implode(' AND ', $arrWhere) . "
                    GROUP BY
                            iu.itrid,
                            iu.estuf,
                            iu.mun_estuf,
                            mun.mundescricao,
                            prp.prpnumeroprocesso,
                            dop.dopnumerodocumento, 
                            dop.dopdatafimvigencia, 
                            dop.dopid,
                            dpv.dpvid,
                            tpd.tpddsc,
                            sd.sbaid, 
                            sd.sbdano,
                            s.sbadsc,
                            s.frmid,
                            s.sbaid,
                            pag.pagsituacaopagamento,
                            pag.pagid,
                            ssu.ssudescricao,
                            habilita,
                            pendencia_obra,
                            de.saldo,
                            de.empnumero,
                            sd.sbdptres,
                            dop.dopvalortermo,
                            dop.dopdatafimvigencia,
                            con.connumerocontrato,
                            con.convlrcontrato,
                            ntf.ntfnumeronotafiscal,
                            ntf.ntfvlrnota
                    ORDER BY
                            1,2,3,4
                ) as foo";
//        ver(simec_htmlentities($sql), d);
        return ($booSQL) ? $sql : $this->carregar($sql);
    }

}

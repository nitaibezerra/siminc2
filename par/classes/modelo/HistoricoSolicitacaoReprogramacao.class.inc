<?php

class HistoricoSolicitacaoReprogramacao extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.historicosolicitacaoreprogramacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("hsrid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'hsrid' => null
    );

    public function relatorioHistoricoSolicitacaoreprogramacao($arrPost = array(), $booSQL = FALSE) {

        #Processos ativos
        $arrWhere = array();
        $arrWhereInterno = array();

        #dopnumerodocumento
        if (!empty($arrPost['dopnumerodocumento'])) {
            $arrWhere[] = "foo.termo_compromisso = '{$arrPost['dopnumerodocumento']}'";
        }
        
        #hsrprotocolo
        if (!empty($arrPost['hsrprotocolo'])) {
            $arrWhere[] = "foo.hsrprotocolo = '{$arrPost['hsrprotocolo']}'";
        }

        #municipio
        if ($arrPost['listaMunicipio'][0]) {
            $arrWhere[] = "foo.entidade IN ('" . implode("','", $arrPost['listaMunicipio']) . "')";

        }

        #UF
        if ($arrPost['estuf'][0]) {
            $arrWhere[] = "(foo.uf IN ('" . implode("','", $arrPost['estuf']) . "'))";
        }
        
        #TIPO
        $strSituacao = "";
        #Aguardando Validação do FNDE
        $querySituacaoA = "SELECT DISTINCT
                                prp.prpid
                            FROM par.processopar prp
                            INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
                            INNER JOIN par.subacaodetalhe sd  ON sd.sbdid = ppc.sbdid
                            INNER JOIN par.subacao s ON s.sbaid = sd.sbaid                              
                            INNER JOIN par.documentopar dop ON dop.prpid = prp.prpid
                            INNER JOIN par.modelosdocumentos mdo ON mdo.mdoid = dop.mdoid
                            INNER JOIN par.documentoparreprogramacaosubacao dpr ON dpr.dopid = dop.dopid
                            INNER JOIN par.instrumentounidade inu ON inu.inuid = prp.inuid
                            WHERE dpr.dpsstatus = 'P' AND dopstatus = 'A'";
        
        #Aguardando Conclusão do Município / Estado
        $querySituacaoB = "SELECT DISTINCT
                                        prp.prpid
                                FROM par.processopar prp
                                INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
                                INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
                                INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
                                INNER JOIN par.documentopar dop ON dop.prpid = prp.prpid
                                INNER JOIN par.modelosdocumentos mdo ON mdo.mdoid = dop.mdoid
                                INNER JOIN par.documentoparreprogramacaosubacao dpr ON dpr.dopid = dop.dopid
                                INNER JOIN par.instrumentounidade inu ON inu.inuid = prp.inuid
                                WHERE dopreformulacao IS TRUE 
                                    AND (SELECT count(sd2.sbdid) 
                                    FROM par.processoparcomposicao ppc 
                                    LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (20, 23) 
                                    WHERE ppc.ppcstatus = 'A' AND dop.prpid = ppc.prpid) > 0 AND dpr.dpsstatus = 'A' AND dopstatus = 'A'";
        
        #Aguardando Aprovação do FNDE
        $querySituacaoC = "SELECT DISTINCT
                                prp.prpid
                            FROM par.processopar prp
                            INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
                            INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
                            INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
                            INNER JOIN par.documentopar dop ON dop.prpid = prp.prpid
                            INNER JOIN par.modelosdocumentos mdo ON mdo.mdoid = dop.mdoid
                            INNER JOIN par.documentoparreprogramacaosubacao dpr ON dpr.dopid = dop.dopid
                            INNER JOIN par.instrumentounidade inu ON inu.inuid = prp.inuid
                            WHERE dopreformulacao IS TRUE 
                                AND ((SELECT count(sd2.sbdid) FROM par.processoparcomposicao ppc LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (20, 23) WHERE ppc.ppcstatus = 'A' and dop.prpid = ppc.prpid) = 0 AND
                                (SELECT count(sd2.sbdid) FROM par.processoparcomposicao ppc LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (21) WHERE ppc.ppcstatus = 'A' and dop.prpid = ppc.prpid) > 0 ) AND dpr.dpsstatus = 'A' AND dopstatus = 'A'";
        
        #Aguardando Empenho
        $querySituacaoG = "SELECT DISTINCT
                                prp.prpid
                            FROM par.processopar prp
                            INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
                            INNER JOIN par.subacaodetalhe sd  ON sd.sbdid = ppc.sbdid
                            INNER JOIN par.subacao s ON s.sbaid = sd.sbaid                                              
                            INNER JOIN par.documentopar dop ON dop.prpid = prp.prpid
                            INNER JOIN par.modelosdocumentos mdo ON mdo.mdoid = dop.mdoid
                            INNER JOIN par.documentoparreprogramacaosubacao dpr ON dpr.dopid = dop.dopid
                            INNER JOIN par.instrumentounidade inu ON inu.inuid = prp.inuid
                            WHERE (
                                SELECT DISTINCT
                                       ((par.recuperavalorvalidadossubacaoporano(sbd.sbaid, sbd.sbdano)-(sbdrepassevlrcomplementaraprovado+sbdrepassevlrrafaprovado)) - sum(vve.vrlempenhocancelado)) > 0
                                FROM par.empenhosubacao ems
                                INNER join par.empenho emp on emp.empid = ems.empid and empcodigoespecie not in ('03', '13', '02', '04') and eobstatus = 'A' and empstatus = 'A'
                                inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
                                inner join par.processopar prp ON prp.prpnumeroprocesso = emp.empnumeroprocesso
                                left join par.documentopar dop ON dop.prpid = prp.prpid AND dop.dopstatus = 'A'
                                left join par.subacaodetalhe sbd  on ems.sbaid = sbd.sbaid and sbdano = sd.sbdano
                                left join (
                                       select ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as vrlreforco, ep.empcodigoespecie, sd1.sbdid 
                                       from par.empenho ep
                                       inner join par.empenhosubacao es on es.empid = ep.empid and empstatus = 'A' and eobstatus = 'A'
                                       inner join par.subacaodetalhe sd1 on sd1.sbaid = es.sbaid and sd1.sbdano = es.eobano
                                       where ep.empcodigoespecie in ('02')
                                       group by 
                                               ep.empnumeroprocesso,
                                               ep.empcodigoespecie,
                                               ep.empidpai,
                                               sd1.sbdid
                                ) as emr on emr.empidpai = emp.empid and emr.sbdid = sbd.sbdid
                                where ems.sbaid = s.sbaid  AND ems.eobano = sd.sbdano AND ems.eobstatus = 'A'
                                GROUP BY sbd.sbaid, sbd.sbdano, sbd.sbdrepassevlrcomplementaraprovado, sbd.sbdrepassevlrrafaprovado,emr.vrlreforco
                            ) 
                            AND dopreformulacao IS TRUE 
                            AND (SELECT count(sd2.sbdid) FROM par.processoparcomposicao ppc LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (20, 21, 23) WHERE ppc.ppcstatus = 'A' and dop.prpid = ppc.prpid) = 0 
                            AND dpr.dpsstatus = 'A' AND dopstatus = 'A'";
                
        #Aguardando Geração de Termo
        $querySituacaoD = "SELECT DISTINCT
                                prp.prpid
                           FROM par.processopar prp
                           INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
                           INNER JOIN par.subacaodetalhe sd  ON sd.sbdid = ppc.sbdid
                           INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
                           INNER JOIN par.documentopar dop ON dop.prpid = prp.prpid
                           INNER JOIN par.modelosdocumentos mdo ON mdo.mdoid = dop.mdoid
                           INNER JOIN par.documentoparreprogramacaosubacao dpr ON dpr.dopid = dop.dopid
                           INNER JOIN par.instrumentounidade inu ON inu.inuid = prp.inuid
                           WHERE (
                                SELECT DISTINCT
                                   ((par.recuperavalorvalidadossubacaoporano(sbd.sbaid, sbd.sbdano)-(sbdrepassevlrcomplementaraprovado+sbdrepassevlrrafaprovado))::numeric - sum(des.saldo) ) <= 0
                                FROM par.empenhosubacao ems
                                INNER JOIN par.v_dadosempenhosubacao des ON des.sbaid = ems.sbaid AND des.sbdano = ems.eobano
                                INNER JOIN par.subacaodetalhe sbd ON ems.sbaid = sbd.sbaid and sbd.sbdano = ems.eobano
                                WHERE ems.sbaid = s.sbaid  AND ems.eobano = sd.sbdano AND ems.eobstatus = 'A'
                                GROUP BY sbd.sbaid, sbd.sbdano, sbd.sbdrepassevlrcomplementaraprovado, sbd.sbdrepassevlrrafaprovado
                           ) 
                           AND dopreformulacao IS TRUE 
                           AND (SELECT count(sd2.sbdid) FROM par.processoparcomposicao ppc LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (20, 21, 23) WHERE ppc.ppcstatus = 'A' and dop.prpid = ppc.prpid) = 0 
                           AND dpr.dpsstatus = 'A' AND dopstatus = 'A'";
        
        #Reprogramações Finalizadas
        $querySituacaoE = "SELECT DISTINCT
                                prp.prpid                                                         
                            FROM par.processopar prp
                            INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
                            INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
                            INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
                            INNER JOIN par.documentopar dop ON dop.prpid = prp.prpid
                            INNER JOIN par.modelosdocumentos mdo ON mdo.mdoid = dop.mdoid
                            INNER JOIN par.documentoparreprogramacaosubacao dpr  ON dpr.dopid = dop.dopidpai
                            INNER JOIN par.reprogramacao rep2 ON rep2.dopidreprogramado = dop.dopid and rep2.repid = dpr.repid and rep2.repstatus = 'F'
                            INNER JOIN par.instrumentounidade inu ON inu.inuid = prp.inuid
                            WHERE dop.dopidpai IS NOT NULL 
                            AND rep2.repdtfim IS NOT NULL AND dpr.dpsstatus = 'A' AND dopstatus = 'A'";
                
        #Aguardando Finalização de Reprogramação
        $querySituacaoF = "SELECT DISTINCT
                                prp.prpid
                            FROM par.processopar prp
                            INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
                            INNER JOIN par.subacaodetalhe sd  ON sd.sbdid = ppc.sbdid
                            INNER JOIN par.subacao s   ON s.sbaid = sd.sbaid
                            INNER JOIN par.documentopar dop ON dop.prpid = prp.prpid
                            INNER JOIN par.modelosdocumentos mdo ON mdo.mdoid = dop.mdoid
                            INNER JOIN par.reprogramacao rep2 ON rep2.dopidreprogramado = dop.dopid AND rep2.repstatus = 'A'
                            INNER JOIN par.documentoparreprogramacaosubacao dpr ON rep2.repid = dpr.repid
                            INNER JOIN par.instrumentounidade inu ON inu.inuid = prp.inuid
                            WHERE rep2.repdtfim IS NULL 
                            AND dpr.dpsstatus = 'A' AND dopstatus = 'A'";
        
        if ($arrPost['hsrtipo'] != "T") {
            if ($arrPost['hsrtipo'] == "S") {
                switch ($arrPost['situacao']){
                    case 'B' :
                        $strSituacao = 'Aguardando Conclusão do Município / Estado';
                        $arrWhereInterno[] = "hsr.prpid IN ({$querySituacaoB})";
                        break;
                    case 'C' :
                        $strSituacao = 'Aguardando Aprovação do FNDE';
                        $arrWhereInterno[] = "hsr.prpid IN ({$querySituacaoC})";
                        break;
                    case 'D' :
                        $strSituacao = 'Aguardando Geração de Termo';
                        $arrWhereInterno[] = "hsr.prpid IN ({$querySituacaoD})";
                        break;
                    case 'E' :
                        $strSituacao = 'Reprogramações Finalizadas';
                        $arrWhereInterno[] = "hsr.prpid IN ({$querySituacaoE})";
                        break;
                    case 'F' :
                        $strSituacao = 'Aguardando Finalização de Reprogramação';
                        $arrWhereInterno[] = "hsr.prpid IN ($querySituacaoF)";
                        break;
                    case 'G' :
                        $strSituacao = 'Aguardando Empenho';
                        $arrWhereInterno[] = "hsr.prpid IN ({$querySituacaoG})";
                        break;
                    case 'A' :
                    default :
                        $strSituacao = 'Aguardando Validação do FNDE';
                        $arrWhereInterno[] = "hsr.prpid IN ({$querySituacaoA})";
                        break;
                }
            } else {
                switch ($arrPost['situacao']){
                    case 'B' :
                        $arrWhereInterno[] = "dopr.dprstatus = 'P'";
                        $arrWhereInterno[] = "dprdataprazoaprovado IS NOT NULL";
                        break;
                    case 'E' :
                        $arrWhereInterno[] = "dop.dopidpai IS NOT NULL";
                        $arrWhereInterno[] = "dopr.dprstatus = 'A' OR dopr.dprstatus = 'I'";
                        break;
                    case 'A' :
                    default :
                        $arrWhereInterno[] = "dopr.dprstatus = 'P'";
                        $arrWhereInterno[] = "dopr.dprdataprazoaprovado IS NULL";
                        break;
                }
            }
        }

        $strAnd = (count($arrWhere)>0) ? ' WHERE ' : '';
        $strAndInterno = (count($arrWhereInterno)>0) ? ' AND ' : '';
        #sql principal
        $sql = "SELECT
                        *
                FROM(";
                    if ($arrPost['hsrtipo'] == 'S' || $arrPost['hsrtipo'] == 'T') {
                    $sql .= "SELECT 
                                CASE WHEN m.muncod IS NOT NULL
                                    THEN m.mundescricao
                                    ELSE 'SEDUC'
                                END as entidade,
                                CASE WHEN iu.estuf IS NOT NULL
                                    THEN iu.estuf
                                    ELSE iu.mun_estuf
                                END as uf,
                                dop.dopnumerodocumento as termo_compromisso,
                                CASE WHEN hsrtipo = 'S' 
                                        THEN
                                                array_to_string( 
                                                        array(
                                                                SELECT 
                                                                        par.retornacodigosubacao(s.sbaid) || ' - ' ||  sbadsc  
                                                                FROM par.historicosolicitacaoreprogramacaosubacao hrs
                                                                INNER JOIN par.subacao s ON s.sbaid = hrs.sbaid where hsr.hsrid = hrs.hsrid )
                                                                , '</br>' 
                                                ) 
                                        ELSE '<center>-</center>'
                                END as subacoes,
                                'Subação' as tipo_solicitacao, 
                                TO_CHAR(hsrdata, 'DD/MM/YYYY') as dt_solicitacao,
                                usu.usunome as usuario_solicitacao,
                                hsr.hsrjustificativa,
                                hsrprotocolo,";
                        if (!empty($strSituacao)) {
                            $sql .= "'$strSituacao' as situacao,";
                        } else {
                            $sql .= "CASE
                                        WHEN hsr.prpid IN ($querySituacaoA) THEN 'Aguardando Validação do FNDE'
                                        WHEN hsr.prpid IN ($querySituacaoB) THEN 'Aguardando Conclusão do Município / Estado'
                                        WHEN hsr.prpid IN ($querySituacaoC) THEN 'Aguardando Aprovação do FNDE'
                                        WHEN hsr.prpid IN ($querySituacaoD) THEN 'Aguardando Geração de Termo'
                                        WHEN hsr.prpid IN ($querySituacaoE) THEN 'Reprogramações Finalizadas'
                                        WHEN hsr.prpid IN ($querySituacaoF) THEN 'Aguardando Finalização de Reprogramação'
                                        WHEN hsr.prpid IN ($querySituacaoG) THEN 'Aguardando Empenho'
                                        ELSE 'Aguardando Validação do FNDE'
                                    END as situacao,";
                        }
                    $sql .= "CASE WHEN doprs.dpstipoparecer = 'A' THEN 'Deferido'
                                     WHEN doprs.dpstipoparecer = 'R' THEN 'Indeferido'
                                    ELSE '<center>-</center>'
                                END as resultado,
                                CASE WHEN doprs.dpsparecer IS NOT NULL
                                    THEN doprs.dpsparecer 
                                    ELSE '<center>-</center>'
                                END as justificativa,
                                CASE WHEN doprs.dpsdataparecer IS NOT NULL
                                    THEN TO_CHAR(doprs.dpsdataparecer , 'DD/MM/YYYY')
                                    ELSE '<center>-</center>'
                                END as data,
                                CASE WHEN doprs.dpscpfparecer IS NOT NULL
                                    THEN usu2.usunome
                                    ELSE '<center>-</center>'
                                END as usuario_parecer
                            FROM par.historicosolicitacaoreprogramacao hsr 
                            INNER JOIN par.reprogramacao rep ON rep.repid = hsr.repid
                            LEFT JOIN par.historicosolicitacaoreprogramacaosubacao hsrs ON hsrs.hsrid = hsr.hsrid
                            LEFT JOIN par.documentoparreprogramacaosubacao doprs ON doprs.dopid = hsr.dopid
                            INNER JOIN par.subacaodetalhe sd ON doprs.sbdid = sd.sbdid
                            INNER JOIN par.processopar prp ON prp.prpid = hsr.prpid
                            INNER JOIN par.instrumentounidade iu ON iu.inuid = prp.inuid
                            LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
                            INNER JOIN seguranca.usuario usu ON hsr.hsrcpf = usu.usucpf
                            LEFT JOIN seguranca.usuario usu2 ON doprs.dpscpfparecer = usu2.usucpf
                            INNER JOIN par.documentopar dop ON dop.dopid = doprs.dopid
                            WHERE hsrtipo = 'S' " . $strAndInterno .' '.implode(' AND ', $arrWhereInterno);
                    }
                    
                    if ($arrPost['hsrtipo'] == 'T') {
                        $sql .= " UNION ALL ";
                    }
                        
                    if ($arrPost['hsrtipo'] == 'P' || $arrPost['hsrtipo'] == 'T') {
                        $sql .= "SELECT 
                            CASE WHEN m.muncod IS NOT NULL
                                THEN m.mundescricao
                                ELSE 'SEDUC'
                            END as entidade,
                            CASE WHEN iu.estuf IS NOT NULL
                                THEN iu.estuf
                                ELSE iu.mun_estuf
                            END as uf,
                            dop.dopnumerodocumento as termo_compromisso,
                            '-' as subacoes,
                            'Prazo' as tipo_solicitacao, 
                            TO_CHAR(hsrdata, 'DD/MM/YYYY') as dt_solicitacao,
                            usu.usunome as usuario_solicitacao,
                            hsr.hsrjustificativa,
                            hsrprotocolo,
                            CASE 
                                WHEN dopr.dprstatus = 'P' AND dopr.dprdataprazoaprovado IS NULL THEN 'Aguardando Validação do FNDE'
                                WHEN dopr.dprstatus = 'P' AND dopr.dprdataprazoaprovado IS NOT NULL THEN 'Aguardando Geração de Termo'
                                WHEN dop.dopidpai IS NOT NULL THEN 'Reprogramações Finalizadas'
                                ELSE 'Aguardando Validação do FNDE'
                            END as situacao,
                            CASE WHEN dopr.dprtipovalidacao = 'A' THEN 'Deferido'
                                 WHEN dopr.dprtipovalidacao = 'R' THEN 'Indeferido'
                                ELSE '<center>-</center>'
                            END as resultado,
                            CASE WHEN dopr.dprparecer IS NOT NULL
                                THEN dopr.dprparecer 
                                ELSE '<center>-</center>'
                            END as justificativa,
                            CASE WHEN dopr.dprdatavalidacao IS NOT NULL
                                THEN TO_CHAR(dopr.dprdatavalidacao , 'DD/MM/YYYY')
                                ELSE '<center>-</center>'
                            END as data,
                            CASE WHEN dopr.dprcpfvalidacao IS NOT NULL
                                THEN usu2.usunome
                                ELSE '<center>-</center>'
                            END as usuario_parecer
                        FROM par.historicosolicitacaoreprogramacao hsr
                        LEFT JOIN par.documentoparreprogramacao dopr ON dopr.dopid = hsr.dopid
                        INNER JOIN par.processopar prp ON prp.prpid = hsr.prpid
                        INNER JOIN par.instrumentounidade iu ON iu.inuid = prp.inuid
                        LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
                        INNER JOIN seguranca.usuario usu ON hsr.hsrcpf = usu.usucpf
                        LEFT JOIN seguranca.usuario usu2 ON dopr.dprcpfvalidacao = usu2.usucpf
                        INNER JOIN par.documentopar dop ON hsr.dopid = dop.dopid
                        WHERE hsrtipo = 'P' " . $strAndInterno .' '.implode(' AND ', $arrWhereInterno);
                    }
                    
                $sql .= ") AS foo 
                " . $strAnd .' '.implode(' AND ', $arrWhere);
//                ver(simec_htmlentities($sql), d);
        return ($booSQL) ? $sql : $this->carregar($sql);
    }

}

<?php

/**
 * Classe responsável por guardar os métodos que serão acessados pelo webservice referentes ao módulo de obras
 *
 */
class Obras2Ws extends ClasseWs{

    /**
     * Retorna uma lista com todas as obras do sistema
     * @param array $dados Dados passados pelo webservice (caso existe uma posição "filtro" utiliza o valor para filtrar a lista)
     * @return array
     */
    public function listarObras($dados){
        global $db;

        $where   = array();

        if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {

            $arrObr = array(
                PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
                PFLCOD_EMPRESA_VISTORIADORA_GESTOR
            );

            $arrUni = Array(PFLCOD_GESTOR_UNIDADE);

            $arrOrg = Array(PFLCOD_ADMINISTRADOR,
                PFLCOD_CADASTRADOR_INSTITUCIONAL,
                PFLCOD_CONSULTA_TIPO_DE_ENSINO,
                PFLCOD_SUPERVISOR_MEC,
                PFLCOD_GESTOR_MEC);

            $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);

            $resp = array();
            $arPflcod = array();
            $orWhere = array();

            if (possui_perfil($arrEst)) {
                $arPflcod = array_merge($arPflcod, $arrEst);
                $orWhere['estuf'] = "mun.estuf IN ( SELECT estuf FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.estuf = edr.estuf)";
            }

            if (possui_perfil($arrObr)) {
                $arPflcod = array_merge($arPflcod, $arrObr);
                $orWhere['empid'] = "e.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.empid = e.empid)";
            }

            if (possui_perfil($arrOrg)) {
                $arPflcod = array_merge($arPflcod, $arrOrg);
                $orWhere['orgid'] = "e.orgid IN ( SELECT urs.orgid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.orgid = e.orgid)";
            }

            if (possui_perfil($arrUni)) {
                $arPflcod = array_merge($arPflcod, $arrUni);
                $orWhere['entidunidade'] = "e.entidunidade IN ( SELECT urs.entid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.entid = e.entidunidade)";
            }

            if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
                $usuarioResp = new UsuarioResponsabilidade();
                $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
                $arEmpid = ($arEmpid ? $arEmpid : array(0));

                $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
                $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;

                $orWhere['sup'] = "e.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    /*urs.entid = e.entidunidade AND*/ urs.empid IN('" . implode("', '", $arEmpid) . "'))";
            }

        }

        if($dados['filtro']){
            $where['obrnome'] = "
                        ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%{$dados['filtro']}%') OR
                            o.obrid::CHARACTER VARYING ILIKE ('%{$dados['filtro']}%') ) ";
        }


        $sql = "
             SELECT * FROM (
                    SELECT DISTINCT ON (o.obrid)
                        o.obrid,
                        o.preid,
                        ''||p_conv.pronumeroprocesso||'' as pronumeroprocesso,
                        p_conv.termo_convenio,
                        p_conv.ano_termo_convenio,
                CASE WHEN e.prfid = 42 OR o.tooid = 4 THEN
                '(EP) '
                ELSE
                ''
                END || '(' || o.obrid || ') ' || o.obrnome || '' as obrnome,
                uni.entnome,
                mun.mundescricao,
                mun.estuf,
                TO_CHAR(oc.ocrdtinicioexecucao, 'dd/mm/YYYY') AS inicio,
                TO_CHAR(oc.ocrdtterminoexecucao, 'dd/mm/YYYY') AS termino,
                        CASE
                            WHEN str.strid = 6 THEN '' || str.strdsc || ''
                            ELSE str.strdsc
                        END AS situacao,
                        ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) || '%' as percentual_execucao,
                        to_char(obrdtultvistoria, 'DD/MM/YYYY') as ultima_atualizacao,
                        o.obrpercentultvistoria as obrpercentultvistoria,
                        to_char(e.empdtultvistoriaempresa,'DD/MM/YYYY') as empdtultvistoriaempresa,
                        e.emppercentultvistoriaempresa as emppercentultvistoriaempresa,
                        tpo.tpodsc,
                        ocrvalorexecucao
                    FROM obras2.obras o
                    LEFT JOIN obras2.empreendimento e                    ON e.empid = o.empid
                    LEFT JOIN entidade.endereco edr                      ON edr.endid = o.endid
                    LEFT JOIN territorios.municipio mun                  ON mun.muncod = edr.muncod
                    LEFT JOIN territorios.estado est                     ON mun.estuf = est.estuf
                    LEFT JOIN (SELECT MAX(oc.ocrid) AS ocrid, obrid FROM obras2.obrascontrato oc WHERE oc.ocrstatus = 'A' GROUP BY oc.obrid) ocr ON ocr.obrid = o.obrid
                    LEFT JOIN obras2.obrascontrato                    oc ON oc.ocrid = ocr.ocrid
                    LEFT JOIN obras2.contrato c                          ON c.crtid = oc.crtid AND c.crtstatus = 'A'
                    LEFT JOIN obras2.programafonte pf                    ON pf.prfid = e.prfid
                    LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
                    LEFT JOIN obras2.tipoorigemobra too                  ON too.tooid = o.tooid
                    LEFT JOIN entidade.entidade uni                      ON uni.entid = e.entidunidade
                    LEFT JOIN obras2.situacao_registro str               ON str.strid = o.strid
                    LEFT JOIN workflow.documento d                       ON d.docid = o.docid
                    LEFT JOIN workflow.estadodocumento ed                ON ed.esdid   = d.esdid

                    -- Dados do Processo e Termo
                    LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid

                    LEFT JOIN (
                        SELECT
                                max( inc_estado ) as inc_estado,
                                obrid
                            FROM
                            (
                                SELECT DISTINCT
                                    CASE
                                        WHEN rstdtsuperacao IS NOT NULL THEN 1
                                        WHEN rstdtprevisaoregularizacao > now() THEN 2
                                        WHEN rstdtprevisaoregularizacao < now() THEN 3
                                        ELSE 2
                                    END as inc_estado,
                                    obrid
                                FROM
                                    obras2.restricao
                                WHERE
                                    obrid IS NOT NULL AND
                    rstitem = 'I' AND
                    rststatus = 'A'
                                ) as  foo
                            GROUP BY
                                obrid
                            ) inc ON inc.obrid = o.obrid

                    WHERE
                        o.obridpai IS NULL AND
                        o.obrstatus = 'A'
                        " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "
                        " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "
                ) as f
                ORDER BY f.obrid ASC
        ";

        if($dados['limit']){
            $sql .= " LIMIT {$dados['limit']}";
        }

        if($dados['offset']){
            $sql .= " OFFSET {$dados['offset']}";
        }
        $dados = $db->carregar($sql);
        $dados = $dados ? $dados : array();

//        foreach ($dados as &$d){
//            $d['obrlatitude']  = null;
//            $d['obrlongitude'] = null;
//
//            if($d['obrgeometryjson']){
//                $geometry = json_decode($d['obrgeometryjson']);
//                list($d['obrlongitude'], $d['obrlatitude']) = $geometry->coordinates;
//            }
//
//            unset($d['obrgeometryjson']);
//        }

        return array("codigo" => '0', "mensagem" => 'Consulta realizada com sucesso', "dados" => $dados);
    }


    /**
     * Retorna as informações de uma obra através do id informado
     * @param array $dados Dados passados pelo webservice (necessário existir uma posição obrid com o id da obra)
     * @return array
     */
    public function getDadosObra($dados){
        global $db;

        if(!$dados['obrid']){
            return array();
        }else{
            $where   = array();
            $where[] = " o.obrid = '{$dados['obrid']}' ";
        }

        if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {

            $arrObr = array(
                PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
                PFLCOD_EMPRESA_VISTORIADORA_GESTOR
            );

            $arrUni = Array(PFLCOD_GESTOR_UNIDADE);

            $arrOrg = Array(PFLCOD_ADMINISTRADOR,
                PFLCOD_CADASTRADOR_INSTITUCIONAL,
                PFLCOD_CONSULTA_TIPO_DE_ENSINO,
                PFLCOD_SUPERVISOR_MEC,
                PFLCOD_GESTOR_MEC);

            $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);

            $resp = array();
            $arPflcod = array();
            $orWhere = array();

            if (possui_perfil($arrEst)) {
                $arPflcod = array_merge($arPflcod, $arrEst);
                $orWhere['estuf'] = "mun.estuf IN ( SELECT estuf FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.estuf = edr.estuf)";
            }

            if (possui_perfil($arrObr)) {
                $arPflcod = array_merge($arPflcod, $arrObr);
                $orWhere['empid'] = "e.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.empid = e.empid)";
            }

            if (possui_perfil($arrOrg)) {
                $arPflcod = array_merge($arPflcod, $arrOrg);
                $orWhere['orgid'] = "e.orgid IN ( SELECT urs.orgid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.orgid = e.orgid)";
            }

            if (possui_perfil($arrUni)) {
                $arPflcod = array_merge($arPflcod, $arrUni);
                $orWhere['entidunidade'] = "e.entidunidade IN ( SELECT urs.entid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.entid = e.entidunidade)";
            }

            if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
                $usuarioResp = new UsuarioResponsabilidade();
                $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
                $arEmpid = ($arEmpid ? $arEmpid : array(0));

                $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
                $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;

                $orWhere['sup'] = "e.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    /*urs.entid = e.entidunidade AND*/ urs.empid IN('" . implode("', '", $arEmpid) . "'))";
            }

        }

        $sql = "
             SELECT * FROM (
                    SELECT DISTINCT ON (o.obrid)
                        o.obrid,
                        o.preid,
                        ''||p_conv.pronumeroprocesso||'' as pronumeroprocesso,
                        p_conv.termo_convenio,
                        p_conv.ano_termo_convenio,
                CASE WHEN e.prfid = 42 OR o.tooid = 4 THEN
                '(EP) '
                ELSE
                ''
                END || '(' || o.obrid || ') ' || o.obrnome || '' as obrnome,
                uni.entnome,
                mun.mundescricao,
                mun.estuf,
                TO_CHAR(oc.ocrdtinicioexecucao, 'dd/mm/YYYY') AS inicio,
                TO_CHAR(oc.ocrdtterminoexecucao, 'dd/mm/YYYY') AS termino,
                        CASE
                            WHEN str.strid = 6 THEN '' || str.strdsc || ''
                            ELSE str.strdsc
                        END AS situacao,
                        ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) || '%' as percentual_execucao,
                        to_char(obrdtultvistoria, 'DD/MM/YYYY') as ultima_atualizacao,
                        o.obrpercentultvistoria as obrpercentultvistoria,
                        to_char(e.empdtultvistoriaempresa,'DD/MM/YYYY') as empdtultvistoriaempresa,
                        e.emppercentultvistoriaempresa as emppercentultvistoriaempresa,
                        tpo.tpodsc,
                        ocrvalorexecucao
                    FROM obras2.obras o
                    LEFT JOIN obras2.empreendimento e                    ON e.empid = o.empid
                    LEFT JOIN entidade.endereco edr                      ON edr.endid = o.endid
                    LEFT JOIN territorios.municipio mun                  ON mun.muncod = edr.muncod
                    LEFT JOIN territorios.estado est                     ON mun.estuf = est.estuf
                    LEFT JOIN (SELECT MAX(oc.ocrid) AS ocrid, obrid FROM obras2.obrascontrato oc WHERE oc.ocrstatus = 'A' GROUP BY oc.obrid) ocr ON ocr.obrid = o.obrid
                    LEFT JOIN obras2.obrascontrato                    oc ON oc.ocrid = ocr.ocrid
                    LEFT JOIN obras2.contrato c                          ON c.crtid = oc.crtid AND c.crtstatus = 'A'
                    LEFT JOIN obras2.programafonte pf                    ON pf.prfid = e.prfid
                    LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
                    LEFT JOIN obras2.tipoorigemobra too                  ON too.tooid = o.tooid
                    LEFT JOIN entidade.entidade uni                      ON uni.entid = e.entidunidade
                    LEFT JOIN obras2.situacao_registro str               ON str.strid = o.strid
                    LEFT JOIN workflow.documento d                       ON d.docid = o.docid
                    LEFT JOIN workflow.estadodocumento ed                ON ed.esdid   = d.esdid

                    -- Dados do Processo e Termo
                    LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid

                    LEFT JOIN (
                        SELECT
                                max( inc_estado ) as inc_estado,
                                obrid
                            FROM
                            (
                                SELECT DISTINCT
                                    CASE
                                        WHEN rstdtsuperacao IS NOT NULL THEN 1
                                        WHEN rstdtprevisaoregularizacao > now() THEN 2
                                        WHEN rstdtprevisaoregularizacao < now() THEN 3
                                        ELSE 2
                                    END as inc_estado,
                                    obrid
                                FROM
                                    obras2.restricao
                                WHERE
                                    obrid IS NOT NULL AND
                    rstitem = 'I' AND
                    rststatus = 'A'
                                ) as  foo
                            GROUP BY
                                obrid
                            ) inc ON inc.obrid = o.obrid

                    WHERE
                        o.obridpai IS NULL AND
                        o.obrstatus = 'A'
                        " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "
                        " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "
                ) as f
                ORDER BY f.obrid ASC
        ";

        $dados = $db->pegaLinha($sql);
        $dados = $dados ? $dados : array();

        return array("codigo" => '0', "mensagem" => 'Consulta realizada com sucesso', "dados" => $dados);
    }

    /**
     * Retorna uma lista com todas as obras do sistema de acordo com o painel de pendência
     * @param array $dados Dados passados pelo webservice (caso existe uma posição "filtro" utiliza o valor para filtrar a lista)
     * @return array
     */
    public function listaObrasPorPendencia($dados){
        global $db;

        $where   = array();

        if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {

            $arrObr = array(
                PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
                PFLCOD_EMPRESA_VISTORIADORA_GESTOR
            );

            $arrUni = Array(PFLCOD_GESTOR_UNIDADE);

            $arrOrg = Array(PFLCOD_ADMINISTRADOR,
                PFLCOD_CADASTRADOR_INSTITUCIONAL,
                PFLCOD_CONSULTA_TIPO_DE_ENSINO,
                PFLCOD_SUPERVISOR_MEC,
                PFLCOD_GESTOR_MEC);

            $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);

            $resp = array();
            $arPflcod = array();
            $orWhere = array();

            if (possui_perfil($arrEst)) {
                $arPflcod = array_merge($arPflcod, $arrEst);
                $orWhere['estuf'] = "mun.estuf IN ( SELECT estuf FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.estuf = edr.estuf)";
            }

            if (possui_perfil($arrObr)) {
                $arPflcod = array_merge($arPflcod, $arrObr);
                $orWhere['empid'] = "e.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.empid = e.empid)";
            }

            if (possui_perfil($arrOrg)) {
                $arPflcod = array_merge($arPflcod, $arrOrg);
                $orWhere['orgid'] = "e.orgid IN ( SELECT urs.orgid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.orgid = e.orgid)";
            }

            if (possui_perfil($arrUni)) {
                $arPflcod = array_merge($arPflcod, $arrUni);
                $orWhere['entidunidade'] = "e.entidunidade IN ( SELECT urs.entid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.entid = e.entidunidade)";
            }

            if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
                $usuarioResp = new UsuarioResponsabilidade();
                $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
                $arEmpid = ($arEmpid ? $arEmpid : array(0));

                $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
                $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;

                $orWhere['sup'] = "e.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    /*urs.entid = e.entidunidade AND*/ urs.empid IN('" . implode("', '", $arEmpid) . "'))";
            }

        }

        if($dados['filtro']){
            $where['obrnome'] = "
                        ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%{$dados['filtro']}%') OR
                            o.obrid::CHARACTER VARYING ILIKE ('%{$dados['filtro']}%') ) ";
        }

        if($dados['pendencia']){
            $where['pendencia'] = "
                        ( o.obrid IN (SELECT obrid FROM obras2.vm_total_pendencias WHERE pendencia = '{$dados['pendencia']}') ) ";
        } else {
            $where['pendencia'] = "
                        ( o.obrid IN (SELECT obrid FROM obras2.vm_total_pendencias) ) ";
        }

        $sql = "
             SELECT * FROM (
                    SELECT DISTINCT ON (o.obrid)
                        o.obrid,
                        o.preid,
                        ''||p_conv.pronumeroprocesso||'' as pronumeroprocesso,
                        p_conv.termo_convenio,
                        p_conv.ano_termo_convenio,
                CASE WHEN e.prfid = 42 OR o.tooid = 4 THEN
                '(EP) '
                ELSE
                ''
                END || '(' || o.obrid || ') ' || o.obrnome || '' as obrnome,
                uni.entnome,
                mun.mundescricao,
                mun.estuf,
                TO_CHAR(oc.ocrdtinicioexecucao, 'dd/mm/YYYY') AS inicio,
                TO_CHAR(oc.ocrdtterminoexecucao, 'dd/mm/YYYY') AS termino,
                        CASE
                            WHEN str.strid = 6 THEN '' || str.strdsc || ''
                            ELSE str.strdsc
                        END AS situacao,
                        ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) || '%' as percentual_execucao,
                        to_char(obrdtultvistoria, 'DD/MM/YYYY') as ultima_atualizacao,
                        o.obrpercentultvistoria as obrpercentultvistoria,
                        to_char(e.empdtultvistoriaempresa,'DD/MM/YYYY') as empdtultvistoriaempresa,
                        e.emppercentultvistoriaempresa as emppercentultvistoriaempresa,
                        tpo.tpodsc,
                        ocrvalorexecucao
                    FROM obras2.obras o
                    LEFT JOIN obras2.empreendimento e                    ON e.empid = o.empid
                    LEFT JOIN entidade.endereco edr                      ON edr.endid = o.endid
                    LEFT JOIN territorios.municipio mun                  ON mun.muncod = edr.muncod
                    LEFT JOIN territorios.estado est                     ON mun.estuf = est.estuf
                    LEFT JOIN (SELECT MAX(oc.ocrid) AS ocrid, obrid FROM obras2.obrascontrato oc WHERE oc.ocrstatus = 'A' GROUP BY oc.obrid) ocr ON ocr.obrid = o.obrid
                    LEFT JOIN obras2.obrascontrato                    oc ON oc.ocrid = ocr.ocrid
                    LEFT JOIN obras2.contrato c                          ON c.crtid = oc.crtid AND c.crtstatus = 'A'
                    LEFT JOIN obras2.programafonte pf                    ON pf.prfid = e.prfid
                    LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
                    LEFT JOIN obras2.tipoorigemobra too                  ON too.tooid = o.tooid
                    LEFT JOIN entidade.entidade uni                      ON uni.entid = e.entidunidade
                    LEFT JOIN obras2.situacao_registro str               ON str.strid = o.strid
                    LEFT JOIN workflow.documento d                       ON d.docid = o.docid
                    LEFT JOIN workflow.estadodocumento ed                ON ed.esdid   = d.esdid

                    -- Dados do Processo e Termo
                    LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid

                    LEFT JOIN (
                        SELECT
                                max( inc_estado ) as inc_estado,
                                obrid
                            FROM
                            (
                                SELECT DISTINCT
                                    CASE
                                        WHEN rstdtsuperacao IS NOT NULL THEN 1
                                        WHEN rstdtprevisaoregularizacao > now() THEN 2
                                        WHEN rstdtprevisaoregularizacao < now() THEN 3
                                        ELSE 2
                                    END as inc_estado,
                                    obrid
                                FROM
                                    obras2.restricao
                                WHERE
                                    obrid IS NOT NULL AND
                    rstitem = 'I' AND
                    rststatus = 'A'
                                ) as  foo
                            GROUP BY
                                obrid
                            ) inc ON inc.obrid = o.obrid

                    WHERE
                        o.obridpai IS NULL AND
                        o.obrstatus = 'A'
                        " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "
                        " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "
                ) as f
                ORDER BY f.obrid ASC
        ";

        if($dados['limit']){
            $sql .= " LIMIT {$dados['limit']}";
        }

        if($dados['offset']){
            $sql .= " OFFSET {$dados['offset']}";
        }
        $dados = $db->carregar($sql);
        $dados = $dados ? $dados : array();

        return array("codigo" => '0', "mensagem" => 'Consulta realizada com sucesso', "dados" => $dados);
    }

    /**
     * Retorna uma lista com todas as obras do sistema de acordo com o painel de pendência
     * @param array $dados Dados passados pelo webservice (caso existe uma posição "filtro" utiliza o valor para filtrar a lista)
     * @return array
     */
    public function totalObrasPorPendencia($dados){
        global $db;

        $where   = array();

        if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {

            $arrObr = array(
                PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
                PFLCOD_EMPRESA_VISTORIADORA_GESTOR
            );

            $arrUni = Array(PFLCOD_GESTOR_UNIDADE);

            $arrOrg = Array(PFLCOD_ADMINISTRADOR,
                PFLCOD_CADASTRADOR_INSTITUCIONAL,
                PFLCOD_CONSULTA_TIPO_DE_ENSINO,
                PFLCOD_SUPERVISOR_MEC,
                PFLCOD_GESTOR_MEC);

            $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);

            $resp = array();
            $arPflcod = array();
            $orWhere = array();

            if (possui_perfil($arrEst)) {
                $arPflcod = array_merge($arPflcod, $arrEst);
                $orWhere['estuf'] = "mun.estuf IN ( SELECT estuf FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.estuf = edr.estuf)";
            }

            if (possui_perfil($arrObr)) {
                $arPflcod = array_merge($arPflcod, $arrObr);
                $orWhere['empid'] = "e.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.empid = e.empid)";
            }

            if (possui_perfil($arrOrg)) {
                $arPflcod = array_merge($arPflcod, $arrOrg);
                $orWhere['orgid'] = "e.orgid IN ( SELECT urs.orgid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.orgid = e.orgid)";
            }

            if (possui_perfil($arrUni)) {
                $arPflcod = array_merge($arPflcod, $arrUni);
                $orWhere['entidunidade'] = "e.entidunidade IN ( SELECT urs.entid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.entid = e.entidunidade)";
            }

            if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
                $usuarioResp = new UsuarioResponsabilidade();
                $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
                $arEmpid = ($arEmpid ? $arEmpid : array(0));

                $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
                $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;

                $orWhere['sup'] = "e.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    /*urs.entid = e.entidunidade AND*/ urs.empid IN('" . implode("', '", $arEmpid) . "'))";
            }

        }

        if($dados['filtro']){
            $where['obrnome'] = "
                        ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%{$dados['filtro']}%') OR
                            o.obrid::CHARACTER VARYING ILIKE ('%{$dados['filtro']}%') ) ";
        }

        if($dados['pendencia']){
            $where['pendencia'] = "
                        ( o.obrid IN (SELECT obrid FROM obras2.vm_total_pendencias WHERE pendencia = '{$dados['pendencia']}') ) ";
        } else {
            $where['pendencia'] = "
                        ( o.obrid IN (SELECT obrid FROM obras2.vm_total_pendencias) ) ";
        }

        $sql = "
             SELECT COUNT(*) FROM (
                    SELECT DISTINCT ON (o.obrid)
                        o.obrid,
                        o.preid,
                        ''||p_conv.pronumeroprocesso||'' as pronumeroprocesso,
                        p_conv.termo_convenio,
                        p_conv.ano_termo_convenio,
                CASE WHEN e.prfid = 42 OR o.tooid = 4 THEN
                '(EP) '
                ELSE
                ''
                END || '(' || o.obrid || ') ' || o.obrnome || '' as obrnome,
                uni.entnome,
                mun.mundescricao,
                mun.estuf,
                TO_CHAR(oc.ocrdtinicioexecucao, 'dd/mm/YYYY') AS inicio,
                TO_CHAR(oc.ocrdtterminoexecucao, 'dd/mm/YYYY') AS termino,
                        CASE
                            WHEN str.strid = 6 THEN '' || str.strdsc || ''
                            ELSE str.strdsc
                        END AS situacao,
                        ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) || '%' as percentual_execucao,
                        to_char(obrdtultvistoria, 'DD/MM/YYYY') as ultima_atualizacao,
                        o.obrpercentultvistoria as obrpercentultvistoria,
                        to_char(e.empdtultvistoriaempresa,'DD/MM/YYYY') as empdtultvistoriaempresa,
                        e.emppercentultvistoriaempresa as emppercentultvistoriaempresa,
                        tpo.tpodsc,
                        ocrvalorexecucao
                    FROM obras2.obras o
                    LEFT JOIN obras2.empreendimento e                    ON e.empid = o.empid
                    LEFT JOIN entidade.endereco edr                      ON edr.endid = o.endid
                    LEFT JOIN territorios.municipio mun                  ON mun.muncod = edr.muncod
                    LEFT JOIN territorios.estado est                     ON mun.estuf = est.estuf
                    LEFT JOIN (SELECT MAX(oc.ocrid) AS ocrid, obrid FROM obras2.obrascontrato oc WHERE oc.ocrstatus = 'A' GROUP BY oc.obrid) ocr ON ocr.obrid = o.obrid
                    LEFT JOIN obras2.obrascontrato                    oc ON oc.ocrid = ocr.ocrid
                    LEFT JOIN obras2.contrato c                          ON c.crtid = oc.crtid AND c.crtstatus = 'A'
                    LEFT JOIN obras2.programafonte pf                    ON pf.prfid = e.prfid
                    LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
                    LEFT JOIN obras2.tipoorigemobra too                  ON too.tooid = o.tooid
                    LEFT JOIN entidade.entidade uni                      ON uni.entid = e.entidunidade
                    LEFT JOIN obras2.situacao_registro str               ON str.strid = o.strid
                    LEFT JOIN workflow.documento d                       ON d.docid = o.docid
                    LEFT JOIN workflow.estadodocumento ed                ON ed.esdid   = d.esdid

                    -- Dados do Processo e Termo
                    LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid

                    LEFT JOIN (
                        SELECT
                                max( inc_estado ) as inc_estado,
                                obrid
                            FROM
                            (
                                SELECT DISTINCT
                                    CASE
                                        WHEN rstdtsuperacao IS NOT NULL THEN 1
                                        WHEN rstdtprevisaoregularizacao > now() THEN 2
                                        WHEN rstdtprevisaoregularizacao < now() THEN 3
                                        ELSE 2
                                    END as inc_estado,
                                    obrid
                                FROM
                                    obras2.restricao
                                WHERE
                                    obrid IS NOT NULL AND
                    rstitem = 'I' AND
                    rststatus = 'A'
                                ) as  foo
                            GROUP BY
                                obrid
                            ) inc ON inc.obrid = o.obrid

                    WHERE
                        o.obridpai IS NULL AND
                        o.obrstatus = 'A'
                        " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "
                        " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "
                ) as f
        ";

        $dados = $db->pegaUm($sql);

        return array("codigo" => '0', "mensagem" => 'Consulta realizada com sucesso', "total" => $dados);
    }
}
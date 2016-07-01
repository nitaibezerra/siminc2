<?php
if ($_REQUEST['status'] != "") {
    $_REQUEST['colunas'][] = "obrdtexclusao";
    $_REQUEST['colunas'][] = "usucpfexclusao";
    $_REQUEST['colunas'][] = "obrobsexclusao";
}

function obras_monta_agp_relatorio2() {
    $agrupador = array('nomedaobra');
    $agp = array(
        "agrupador" => array(),
        "agrupadoColuna" => array()
    );

    foreach ($_REQUEST['colunas'] as $valor) {
        $agp['agrupadoColuna'][] = $valor;
    }
    foreach ($agrupador as $val) {
        switch ($val) {
            case "nomedaobra":
                array_push($agp['agrupador'], array(
                    "campo" => "nomedaobra",
                    "label" => "Nome da Obra")
                );
                break;
            case "nomedaobra2":
                array_push($agp['agrupador'], array(
                    "campo" => "nomedaobra2",
                    "label" => "Nome da Obra")
                );
                break;
            case "nomedaobraxls":
                array_push($agp['agrupador'], array(
                    "campo" => "nomedaobraxls",
                    "label" => "Nome da Obra")
                );
                break;
        }
    }
    return $agp;
}

/**
 * Funçao que monta o sql para trazer o relatório geral de obras
 *
 * @author Fernando A. Bagno da Silva
 * @since 20/02/2009
 * 
 * @author2 Rodrigo Pessoa
 * @since2 08/11/2013
 * @version obras2
 * @return string
 */
function obras_monta_sql_relatorio2() {

    $where = array();

    extract($_REQUEST);
    
    //ver($_REQUEST);

    $selectTerritorios = "territorios.municipio ";

    // tipo de ensino - ok
//    if ($orgid) {
//        array_push($where, " org.orgid in (" . implode(',', $orgid) . ") ");
//    }

    // região - ok
    if ($regiao[0] && $regiao_campo_flag) {
        array_push($where, " re.regcod " . (!$regiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $regiao) . "') ");
    }

    // Mesorregião - ok
    if ($mesoregiao[0] && $mesoregiao_campo_flag) {
        array_push($where, " me.mescod " . (!$mesoregiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $mesoregiao) . "') ");
    }

    // Microrregião - ok
    if ($microregiao[0] && $microregiao_campo_flag) {
        array_push($where, " mic.miccod " . (!$microregiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $microregiao) . "') ");
    }

    // UF - ok
    if ($uf[0] && $uf_campo_flag) {
        array_push($where, " ed.estuf " . (!$uf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $uf) . "') ");
    }

    // grupo municipio - ok
    if ($grupomun[0] && $grupomun_campo_flag) {

        $selectTerritorios = "( SELECT 
                                        tm.muncod, tm.mundescricao, gt.gtmid, gt.gtmdsc, tpm.tpmdsc
                                FROM
                                        territorios.municipio tm 
                                INNER JOIN
                                        territorios.muntipomunicipio mtm ON mtm.muncod = tm.muncod
                                INNER JOIN
                                        territorios.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid 
                                INNER JOIN
                                        territorios.grupotipomunicipio gt ON gt.gtmid = tpm.gtmid 
                                WHERE 
                                        tpm.gtmid = 5 AND gt.gtmid = 5 )";

//        $selectGrupoMun  = "CASE WHEN tm.gtmid is not null THEN tm.gtmdsc ELSE 'Outros' END as grupomun, ";
//        $dadosGrupoMun   = "grupomun, ";
//        $groupByGrupoMun = "tm.gtmdsc, ";
//        $groupByGtmid    = "tm.gtmid, ";
        
        array_push($where, " tm.gtmid " . (!$grupomun_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $grupomun) . "') ");
    }

    // tipo municipio - ok
    if ($tipomun[0] && $tipomun_campo_flag) {
        array_push($where, " tpm.tpmid " . (!$tipomun_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $tipomun) . "') ");
    }

    // municipio - ok
    if ($municipio[0] && $municipio_campo_flag) {
        array_push($where, " ed.muncod " . (!$municipio_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $municipio) . "') ");
    }

    // unidade - ok
    if ($unidade[0] && $unidade_campo_flag) {
        array_push($where, " oi.entidunidade " . (!$unidade_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode(',', $unidade) . ") ");
    }

    // entidcampus - ok
    if ($entidcampus[0] && $entidcampus_campo_flag) {
//        array_push($where, " oi.entidcampus " . (!$entidcampus_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode(',', $entidcampus) . ") ");
        array_push($where, " oi.entid " . (!$entidcampus_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode(',', $entidcampus) . ") ");
    }

    // programa - ???
    if ($prfid[0] && $prfid_campo_flag) {
        if (!$prfid_campo_excludente) {
            array_push($where, " pf.prfid  IN (" . implode(',', $prfid) . ") ");
        } else {
            array_push($where, " ( pf.prfid  NOT IN (" . implode(',', $prfid) . ") OR pf.prfid is null ) ");
        }
    }

    // metragem da obra - ???
    if ($metragem_inicio) {
        if ($metragem_operador == 'entre') {
            array_push($where, " oi.obrqtdconstruida >= '$metragem_inicio' AND oi.obrqtdconstruida <= '$metragem_fim' ");
        } else {
            array_push($where, " oi.obrqtdconstruida $metragem_operador '$metragem_inicio' ");
        }
    }

    // fonte - TIPOORIGEMOBRA - ok
    if ($tooid[0] && $tooid_campo_flag) {
        if (!$tooid_campo_excludente) {
            array_push($where, " oi.tooid  IN (" . implode(',', $tooid) . ") ");
        } else {
            array_push($where, " ( oi.tooid  NOT IN (" . implode(',', $tooid) . ") OR oi.tooid is null ) ");
        }
    }

    // tipologia da obra - ok
    if ($tpoid[0] && $tpoid_campo_flag) {
        array_push($where, " oi.tpoid " . (!$tpoid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode(',', $tpoid) . ") ");
    }

    // classificação da obra - ok
    if ($cloid[0] && $cloid_campo_flag) {
        array_push($where, " oi.cloid " . (!$cloid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode(',', $cloid) . ") ");
    }

    if ($stoid[0] && $stoid_campo_flag) {
        array_push($where, " esd.esdid " . (!$stoid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode(',', $stoid) . ") ");
    }

    // percentual da obra - ok ?
    if ($percentualinicial) {
        //array_push($where, " oi.obrpercexec BETWEEN {$percentualinicial} AND {$percentualfinal}");
        array_push($where, " oi.obrpercentultvistoria BETWEEN {$percentualinicial} AND {$percentualfinal}");
    }

    // localização da obra - ok
    if ($latitudeElongitude) {
        array_push($where, " (TRIM(ed.medlatitude) <> '' AND TRIM(ed.medlongitude) <> '')");
    }
    
    //ver($where);

    // possui foto - ok ?
    switch ($foto) {
        case 'sim' : 
//            $stFiltro .= " and (ao.obrid is not null and ao.aqostatus = 'A') ";
            $stFiltro .= " and oi.obrid IN (SELECT DISTINCT obrid FROM obras2.fotos) ";
            break;
        case 'nao' : 
//            $stFiltro .= " and ao.obrid is null  ";
            $stFiltro .= " and oi.obrid NOT IN (SELECT DISTINCT obrid FROM obras2.fotos) ";
            break;
    }

    // Filtro de vistoria - ok
    switch ($_REQUEST["vistoria"]) {
        case 'sim' : $stFiltro .= " and oi.obrdtvistoria is not null ";
            break;
        case 'nao' : $stFiltro .= " and oi.obrdtvistoria is null ";
            break;
    }

    $param['orgid'] = (isset($param['orgid']) ? $param['orgid'] : $_SESSION['obras2']['orgid']);

    switch ($_REQUEST['obrvinculada']) {
        case 'S':
            $stFiltro .= "and oi.obrid IN (SELECT v.obridvinculado FROM obras2.obras v WHERE v.obrstatus = 'P' AND v.obridvinculado IS NOT NULL) ";
            break;
        case 'N':
            $stFiltro .= "and oi.obrid NOT IN (SELECT v.obridvinculado FROM obras2.obras v WHERE v.obrstatus = 'P' AND v.obridvinculado IS NOT NULL) ";
            break;
    }


    // Filtro de responsável pela vistoria - ok
    switch ($_REQUEST["responsavel"]) {
        case '' : $stFiltro .= " ";
            break;
        case '0' : $stFiltro .= " and COALESCE(s.supid,0) = 0 ";
            break;
        case '1' : $stFiltro .= " and s.rsuid = 1 ";
            break;
        case '2' : $stFiltro .= " and s.rsuid = 2 ";
            break;
        case '3' : $stFiltro .= " and s.rsuid = 3 ";
            break;
        case '4' : $stFiltro .= " and s.rsuid = 4 ";
            break;
    }

    // Filtro de restricao - ok
    switch ($restricao) {
        case 'sim' : $stFiltro .= " and (r.obrid is not null and r.rststatus = 'A')";
            break;
        case 'nao' : $stFiltro .= " and r.obrid is null ";
            break;
    }

    // valor da obra - ???
    if ($_REQUEST["vlrmenor"] && $_REQUEST["vlrmaior"]) {
        $vlrmenor = str_replace(array(".", ","), array("", "."), $_REQUEST["vlrmenor"]);
        $vlrmaior = str_replace(array(".", ","), array("", "."), $_REQUEST["vlrmaior"]);
        
//        $stFiltro .= " AND oi.obrvlrrealobra BETWEEN " . $vlrmenor . " AND " . $vlrmaior . " ";
        // Descobrir de onde vem o valor atualizado
        $stFiltro .= " AND oi.obrvalorprevisto BETWEEN " . $vlrmenor . " AND " . $vlrmaior . " ";
    }

    // Valor Contratado da Obra (R$): - ???
    if ($obrcustocontrato_inicio != "" && $obrcustocontrato_fim != "") {
        $obrcustocontrato_inicio = str_replace(array(".", ","), array("", "."), $obrcustocontrato_inicio);
        $obrcustocontrato_fim    = str_replace(array(".", ","), array("", "."), $obrcustocontrato_fim);
        array_push($where, " oi.obrvalorprevisto between $obrcustocontrato_inicio and $obrcustocontrato_fim ");
    }

    // Está em funcionamento? - ???
    // Verificar de onde traz esse dado.
    switch ($_REQUEST["funundfuncionamento"]) {
        case 'sim' : 
//            $stFiltro .= " and funundfuncionamento is true ";
            break;
        case 'nao' : 
//            $stFiltro .= " and (funundfuncionamento is false )";
            break;
    }

    switch ($_REQUEST["medidasexcecao"]) {
        case 'sim' :
            $stFiltro .= " and oi.medidasexcecao is true ";
            break;
        case 'nao' :
            $stFiltro .= " and (oi.medidasexcecao is false )";
            break;
    }

    if($_REQUEST["stiid"]){
        $stFiltro .= " and (oi.stiid = {$_REQUEST["stiid"]} )";
    }

    if($_REQUEST["frpid"]){
        $stFiltro .= " and (oi.frpid = {$_REQUEST["frpid"]} )";
    }

    if ($_REQUEST["funundfuncionamento"] == 'sim' || $_REQUEST["funundfuncionamento"] == 'nao') {
        $innerFuncionamento = "
                INNER JOIN ( SELECT 
                        f1.obrid, 
                        funundfuncionamento
                FROM
                        obras2.funcunidade f1
                INNER JOIN 
                (
                SELECT obrid, max(funid) as funid FROM obras2.funcunidade GROUP BY obrid ORDER BY obrid
                ) f2 ON f1.funid = f2.funid ) fun ON oi.obrid = fun.obrid";
        //Modificar após descobrir de onde traz esse dado.
        $innerFuncionamento = '';
    }

    // Status da Obra - ok
    if ($_REQUEST["status"] == "inativo") {
        $stFiltro .= " AND oi.obrstatus = 'I' and usucpfexclusao is not null ";
    } elseif ($_REQUEST["status"] == "todas") {
        $stFiltro .= "  ";
    } else {
        $stFiltro .= " AND oi.obrstatus = 'A' ";
    }
    
    // Esfera - ok
    if (!empty($_REQUEST["obrtipoesfera"])) {
//        $stFiltro .= " AND oi.obrtipoesfera = '{$_REQUEST["obrtipoesfera"]}' ";
        $stFiltro .= " AND emp.empesfera = '{$_REQUEST["obrtipoesfera"]}' ";
    }

    //Regra = verifica se a obra possui supervisão - ok
    switch ($_REQUEST["supervisao"]) {

        case "S":
            $stFiltro .= " AND ( (SELECT 
                                         COUNT(sup.obrid)
                                  FROM   
                                         obras2.supervisao sup
                                  WHERE
                                         sup.obrid = oi.obrid
                                  AND    sup.supstatus = 'A'
                                  ) > 0  )";
            break;
        case "N":
            $stFiltro .= " AND ( (SELECT 
                                         COUNT(sup.obrid)
                                  FROM   
                                         obras2.supervisao sup
                                  WHERE
                                         sup.obrid = oi.obrid
                                  AND    sup.supstatus = 'A'
		                ) = 0  )";
            break;
    }

    // SQL passada pelo Mario dia 15/06/2012
    /*
    $sql_obrsupervisoes = "  SELECT DISTINCT count(obr22.obrid) as supervisoes--,
                                    --obr22.obrid
                             FROM
                                obras2.obrainfraestrutura obr22
                             INNER JOIN
                                obras2.repositorio ore22 ON ore22.obrid = obr22.obrid
                             INNER JOIN
                                obras2.itemgrupo ig22 ON ig22.repid = ore22.repid
                             INNER JOIN
                                obras2.grupodistribuicao gd22 ON gd22.gpdid = ig22.gpdid
                             INNER JOIN
                                workflow.documento wd22 ON wd22.docid = gd22.docid
                             INNER JOIN
                                workflow.estadodocumento we22 ON we22.esdid = wd22.esdid
                             WHERE 
                                     we22.esdid = 173
                                     AND obr22.obrid = oi.obrid
                             GROUP BY
                             obr22.obrid";
    */
    // Modificado em 12/11/2013
    $sql_obrsupervisoes = "  SELECT DISTINCT 
                                count(obr22.obrid) as supervisoes
                             FROM
                                obras2.obras obr22
                             INNER JOIN
                                obras2.supervisao sup2 ON sup2.obrid = obr22.obrid
                             INNER JOIN 
                                workflow.documento wd22 ON wd22.docid = obr22.docid
                             INNER JOIN
                                workflow.estadodocumento we22 ON we22.esdid = wd22.esdid
                             WHERE 
                                 we22.esdid = 173
                             AND obr22.obrid = oi.obrid
                             GROUP BY
                                obr22.obrid";
    
    //FIM SQL passada pelo Mario dia 15/06/2012
    
    $subSelectSupervisao = "  
                    SELECT 
                        rsuid,obrid,supid 
                     FROM
                        obras2.supervisao s
                     WHERE
                        supid = (SELECT supid
                                 FROM obras2.supervisao ss
                                 WHERE ss.obrid = s.obrid
                                 ORDER BY supdata desc, supid desc limit 1
                                )
            ";
    $subSelectFotos = " SELECT DISTINCT obrid FROM obras2.fotos ";
    
    $subSelectRestricao = " SELECT DISTINCT obrid, rststatus 
                            FROM obras2.restricao 
                            WHERE rststatus = 'A' 
                           ";
    
    $subSelectCampo_dtpublicacao = " 
                                    SELECT 
                                        date(fl.flcpubleditaldtprev)
                                    FROM 
                                        obras2.faselicitacao fl
                                    LEFT JOIN obras2.licitacao lic1          ON lic1.licid = fl.licid    -- Licitacao
                                    LEFT JOIN obras2.obralicitacao olic1     ON lic1.licid = olic1.licid -- Obra Licitacao
                                    LEFT JOIN obras2.tiposfaseslicitacao tfl ON tfl.tflid  = fl.tflid
                                    WHERE 
                                        --fl.obrid     = oi.obrid 
                                        olic1.obrid  = oi.obrid 
                                    AND fl.flcstatus = 'A' 
                                    AND tfl.tflordem = 1 
                                    ORDER BY flcid DESC 
                                    LIMIT 1
                                   ";
    
    $subSelectCampo_dthomologacao = " 
                                     SELECT 
                                            date(fl.flchomlicdtprev)
                                     FROM obras2.faselicitacao fl
                                     LEFT JOIN obras2.licitacao lic2          ON lic2.licid = fl.licid    -- Licitacao
                                     LEFT JOIN obras2.obralicitacao olic2     ON lic2.licid = olic2.licid -- Obra Licitacao
                                     LEFT JOIN obras2.tiposfaseslicitacao tfl ON tfl.tflid  = fl.tflid
                                     WHERE 
                                         --fl.obrid     = oi.obrid 
                                         olic2.obrid  = oi.obrid 
                                     AND fl.flcstatus = 'A' 
                                     AND tfl.tflordem = 4 
                                     ORDER BY flcid DESC 
                                     LIMIT 1
                                    ";
    
    $subSelectCampo_usucpfexclusao = " SELECT usunome FROM seguranca.usuario WHERE usucpf = usucpfexclusao ";
    $subSelectCampo_qtdvistorias   = " 
                                      SELECT COUNT(DISTINCT supv.supid) 
                                      FROM obras2.supervisao supv 
                                      WHERE supv.obrid = oi.obrid 
                                      AND supstatus = 'A'
                                     ";
    $subSelectCampo_supdata = " 
                               SELECT to_char (min(supdata),'dd/mm/yyyy') 
                               FROM obras2.supervisao supv 
                               WHERE supv.obrid = oi.obrid 
                               AND   supstatus  = 'A' and staid = 3
                              ";
    
    $subSelectCampo_dtParalisado = " SELECT to_char (min(supdata),'dd/mm/yyyy') 
                                    FROM obras2.supervisao supv 
                                    WHERE supv.obrid = oi.obrid 
                                    AND supstatus = 'A' and staid = ".STAID_PARALISADO." ";
    
    $subSelectSumItCronograma = "  SELECT
                                       sum(icovlritem) AS soma_cronograma, obrid
                                   FROM
                                       obras2.itenscomposicao ic
                                   JOIN obras2.itenscomposicaoobra ico ON ico.itcid = ic.itcid AND ico.icostatus = 'A'
                                   JOIN obras2.cronograma cro ON cro.croid = ico.croid AND cro.crostatus = 'A'
                                   WHERE
                                      ic.itcstatus = 'A' AND
                                      ico.relativoedificacao = 'D'
                                   GROUP BY 2";
    
    
    
    // Todos os JOINs
    $joins = " 
                LEFT JOIN obras2.situacaoinstrumento sti       ON oi.stiid         = sti.stiid AND sti.stistatus = 'A'    -- Situação Instrumento
                LEFT JOIN obras2.tipoformarepasserecursos frp  ON oi.frpid         = frp.frpid
                LEFT JOIN entidade.endereco ed                 ON oi.endid         = ed.endid     -- Endereco
                LEFT JOIN obras2.obralicitacao olic            ON oi.obrid         = olic.obrid   -- Obra Licitacao
                LEFT JOIN obras2.licitacao lic                 ON olic.licid       = lic.licid    -- Licitacao
                LEFT JOIN obras2.faselicitacao flic            ON olic.licid       = flic.licid   -- Faselicitacao
                LEFT JOIN obras2.tiposfaseslicitacao tflic     ON flic.tflid       = tflic.tflid  -- Tipos de Fase da licitacao
                LEFT JOIN obras2.modalidadelicitacao mol       ON mol.molid        = lic.molid    -- Modalidades da licitacao
                LEFT JOIN obras2.empreendimento emp            ON oi.empid         = emp.empid    -- Empreendimento
                LEFT JOIN obras2.situacaoatividade st          ON oi.staid         = st.staid     -- Situacao da obra
                LEFT JOIN obras2.tipoobra tpo                  ON oi.tobid         = tpo.tobid    -- Tipo de obra
                LEFT JOIN obras2.orgao org                     ON org.orgid        = emp.orgid    -- Orgao

                LEFT JOIN territorios.municipio mun            ON mun.muncod       = ed.muncod    -- Municipio
                LEFT JOIN territorios.estado et                ON mun.estuf         = et.estuf     -- Estado
                LEFT JOIN territorios.regiao re                ON re.regcod        = et.regcod    -- Regiao
                LEFT JOIN territorios.mesoregiao me            ON me.mescod        = mun.mescod   -- Mesoregiao
                LEFT JOIN territorios.microregiao mic          ON mic.miccod       = mun.miccod   -- Microregiao
                LEFT JOIN territorios.pais pa                  ON pa.paiid         = re.paiid     -- Pais

                LEFT JOIN entidade.entidade ee                 ON emp.entidunidade = ee.entid     -- Entidade -> Unidade


                LEFT JOIN (SELECT MAX(ed.endid) endid, e.entid FROM entidade.entidade e
                    JOIN entidade.endereco ed ON ed.entid = e.entid AND endstatus = 'A'
                    WHERE e.entid IN (SELECT entid FROM obras2.obras WHERE obrstatus = 'A' AND obridpai IS NULL AND entid IS NOT NULL GROUP BY entid)
                    GROUP BY e.entid
                ) as ed_aux ON ed_aux.entid = ee.entid
                LEFT JOIN entidade.endereco eed                ON eed.endid        = ed_aux.endid

                -- LEFT JOIN ".$selectTerritorios." AS tm         ON tm.muncod        = ed.muncod    -- Usado caso o grupo de municipios seja setado
                LEFT JOIN obras2.programafonte pf              ON emp.prfid        = pf.prfid     -- Programa Fonte
                LEFT JOIN obras2.tipoorigemobra too            ON oi.tooid         = too.tooid    -- Tipo de origem da obra
                LEFT JOIN obras2.tipologiaobra tp              ON oi.tpoid         = tp.tpoid     -- Tipologia da Obra
                LEFT JOIN (".$subSelectSupervisao.") AS s      ON s.obrid          = oi.obrid     -- Supervisao
                LEFT JOIN ( ".$subSelectFotos." )    AS ft     ON ft.obrid         = oi.obrid     -- Fotos
                LEFT JOIN ( ".$subSelectRestricao." ) AS r     ON r.obrid          = oi.obrid     -- Restricao
                LEFT JOIN obras2.obrascontrato  ocr ON ocr.obrid = oi.obrid AND ocrstatus = 'A'
                LEFT JOIN obras2.contrato con                  ON con.crtid = ocr.crtid AND con.crtstatus = 'A'

                LEFT JOIN entidade.entidade etc ON etc.entid = con.entidempresa

                LEFT JOIN workflow.documento doc               ON doc.docid = oi.docid
                LEFT JOIN workflow.estadodocumento esd         ON doc.esdid = esd.esdid

                -- Dados do Processo e Termo
                LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = oi.obrid

             ";
    
    // monta o sql 
    $sql = "SELECT DISTINCT ON (oi.obrid)
				oi.obrid || ' '                                        as obridid,
				'(' || oi.obrid || ') ' || oi.obrnome                  as nomedaobra,
                                oi.obrpercentultvistoria||'%'                          as porcexecucao,
				'('||ee.entnumdddcomercial||') '||ee.entnumcomercial   as unidade_numcomercial,
				esd.esddsc                                              as situacaoobra,
                                ( ".$subSelectCampo_dtParalisado." )                   as dataparalisado,
                                mun.muncod                                             as muncod1,
                                mun.mundescricao                                       as municipio,
				mun.estuf                                               as estado,
                                me.mesdsc                                              as mesoregiao,
				mic.micdsc                                             as microregiao,
                                ee.entnome                                             as unidade,
                                ee.entemail                                            as unidade_email,
                                eed.endlog                                             as unidade_endlog,
                                eed.endcom                                             as unidade_endcom, 
                                eed.endbai                                             as unidade_endbai, 
                                eed.endnum                                             as unidade_endnum, 
                                eed.endcomunidade                                      as unidade_endcomunidade,
                                eed.endcep                                             as unidade_endcep,
				tpo.tobdesc                                            as tipoobra,
				pf.prfdesc                                             as subacao,
				too.toodescricao                                       as fonte,
				org.orgdesc                                            as orgao,
				oi.preid,
                                ed.endlog, ed.endcom, ed.endbai, 
				ed.endnum, ed.endcomunidade,

                TO_CHAR(etc.entnumcpfcnpj::int8, '00\".\"000\".\"000\"/\"0000\"-\"00') as cnpjempresacontratada,
                etc.entnome as nomeempresacontratada,
                con.crtdtassinatura as obrdtassinaturacontrato,



               p_conv.pronumeroprocesso,
               p_conv.termo_convenio,
               p_conv.ano_termo_convenio,
               -- oi.obrnumprocessoconv,

                                ( ".$subSelectCampo_qtdvistorias." ) as qtdvistorias,
				( ".$subSelectCampo_supdata." )      as supdata,
                                

                                CASE WHEN doc.esdid IN (690, 691) AND con.crtdttermino IS NULL THEN
                                    (SELECT
                                        to_char(MAX(ico.icodterminoitem), 'DD/MM/YYYY')
                                    FROM obras2.itenscomposicaoobra ico
                                    JOIN obras2.itenscomposicao i ON i.itcid = ico.itcid AND i.itcstatus = 'A'
                                    JOIN obras2.cronograma c ON c.croid = ico.croid AND ico.obrid = c.obrid
                                    WHERE ico.obrid = oi.obrid AND ico.icostatus = 'A')

                                ELSE
                                    to_char(date(con.crtdttermino), 'DD/MM/YYYY')
                                END as dtterminocontrato,
                                to_char(date(oi.obrdtinicio), 'DD/MM/YYYY')                        as obrdtinicio,
                                to_char(date(oi.obrdtfim), 'DD/MM/YYYY')                           as obrdttermino,
                                to_char(oi.obrdtinicio, 'DD/MM/YYYY')                              as datainicio,
                                to_char(oi.obrdtfim, 'DD/MM/YYYY')                                 as datafim,

                                CASE WHEN esd.esdid IN (693) THEN
                                    (COALESCE( (SELECT
                                        TO_CHAR(s.supdata, 'DD/MM/YYYY') as supdata
                                        FROM obras2.supervisao s
                                        WHERE s.obrid = oi.obrid AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.staid = 3
                                        ORDER BY s.supdata DESC
                                        LIMIT 1
                                        )
                                    ,
                                       (SELECT TO_CHAR(h.htddata, 'DD/MM/YYYY') FROM workflow.historicodocumento h
                                        JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid AND a.esdiddestino IN (693)
                                        WHERE h.docid = oi.docid
                                        ORDER BY h.htddata DESC
                                        LIMIT 1)
                                    ))
                                ELSE
                                        ''
                                END as obrdtfimvistoria,

                                to_char((".$subSelectCampo_dtpublicacao."), 'DD/MM/YYYY')          as dtpublicacao, -- Data de publicacao do edital
				to_char((".$subSelectCampo_dthomologacao."), 'DD/MM/YYYY')         as dthomologacao,

                                SUBSTRING(ed.endcep, 1, 5) || '-' || SUBSTRING(ed.endcep, 6, 8) as cep,
                                
				CASE WHEN oi.obrvalorprevisto is null OR oi.obrvalorprevisto = 0
                                     THEN ' - '
                                     ELSE to_char(oi.obrvalorprevisto, '999G999G999G999D99')
				END as obrvalorprevisto,
				CASE WHEN con.crtvalorexecucao is null OR con.crtvalorexecucao = 0
				     THEN ' - '
				     ELSE to_char(con.crtvalorexecucao, '999G999G999G999D99')
				END as obrcustocontrato,
				CASE WHEN oi.obrdtultvistoria IS NOT NULL 
                                     THEN to_char(oi.obrdtultvistoria, 'DD/MM/YYYY HH24:MI')
                                     ELSE to_char(oi.obrdtinclusao, 'DD/MM/YYYY HH24:MI')
                                END as ultatualizacao,
				CASE WHEN ( SELECT COUNT(rstid) FROM obras2.restricao WHERE obrid = oi.obrid ) = 0 
				     THEN 'Não' 
				     ELSE 'Sim' 
				END as qtdrestricoes,
                                CASE WHEN oi.tpoid is not null 
                                     THEN tp.tpodsc 
                                     ELSE 'Não informado' 
                                END as tipologia,
                                CASE WHEN mol.moldsc is null 
                                     THEN 'Não informado' 
                                     ELSE moldsc 
                                END as moldsc,
				CASE WHEN lic.licnumero  is null 
                                     THEN 'Não informado' 
                                     ELSE lic.licnumero  
                                END as numlicitacao,
                                
                                to_char(dco.dcodatainicio, 'DD/MM/YYYY')                              as dt_inicio_conv,
                                to_char(dco.dcodatafim, 'DD/MM/YYYY')                               as dt_final_conv,

				CASE WHEN ed.medlatitude = '' and ed.medlongitude = ''
					THEN ''
					ELSE 'Lat.: ' || ed.medlatitude || '<br>Long.: ' || ed.medlongitude 
				END as coordenadas_geograficas,
                                -- Verificar se isso ainda e valido
                                --Alterações Pedidas pelo Mario 15/06/2012
				( ".$sql_obrsupervisoes." ) as obrsupervisoes,


                                CASE WHEN pf.prfid != 41 THEN
                                    cast ( (SELECT MAX(d.dopnumerodocumento)
                                        from par.documentopar  d
                                          inner join par.termocomposicao tc on tc.dopid = d.dopid
                                          inner join obras.preobra pre on pre.preid = tc.preid
                                          inner join obras2.obras o on o.preid = pre.preid
                                          WHERE pre.preid = oi.preid AND d.dopstatus = 'A'
                                          GROUP BY pre.preid) as varchar)
                                ELSE
                                          cast ( (SELECT MAX(ter.terid || '/' || TO_CHAR(ter.terdatainclusao, 'YYYY'))
                                                FROM par.termocompromissopac ter
                                                LEFT JOIN par.termoobra tob ON tob.terid = ter.terid
                                                WHERE tob.terid = oi.preid AND ter.terstatus='A') as varchar)
                                END as termocompromisso,


                                ((SELECT sup.percentual FROM obras2.supervisao s
                                    JOIN (
                                      SELECT distinct
                                        s.*,
                                        ( SELECT CASE WHEN SUM(icovlritem) > 0 THEN ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2) ELSE 0 END AS total FROM obras2.itenscomposicaoobra i JOIN obras2.cronograma cro ON cro.croid = s.croid AND cro.croid = i.croid AND cro.crostatus IN ('A', 'H') LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL WHERE i.icostatus = 'A' AND i.relativoedificacao = 'D' AND cro.obrid = oi.obrid AND i.obrid = cro.obrid) as percentual
                                      FROM
                                          obras2.supervisao s
                                      WHERE
                                        s.obrid = oi.obrid AND
                                        s.emsid IS NULL AND s.smiid IS NULL AND
                                        s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S'
                                      ORDER BY
                                      s.supdata DESC LIMIT 3
                                      ) as sup ON sup.supid = s.supid
                                    ORDER BY s.supdata DESC LIMIT 1)
                                    -

                                    CASE WHEN (
                                        SELECT COUNT(*) FROM obras2.supervisao s
                                        WHERE
                                          s.obrid = oi.obrid AND
                                          s.emsid IS NULL AND s.smiid IS NULL AND
                                          s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S' AND
                                          s.usucpf IS NOT NULL AND s.rsuid = 1
                                      ) = 1 THEN 0
                                    ELSE
                                        (SELECT sup.percentual FROM obras2.supervisao s
                                        JOIN (
                                        SELECT distinct
                                        s.*,
                                        ( SELECT CASE WHEN SUM(icovlritem) > 0 THEN ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2) ELSE 0 END AS total FROM obras2.itenscomposicaoobra i JOIN obras2.cronograma cro ON cro.croid = s.croid AND cro.croid = i.croid AND cro.crostatus IN ('A','H') LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL WHERE i.icostatus = 'A' AND i.relativoedificacao = 'D' AND cro.obrid = oi.obrid AND i.obrid = cro.obrid) as percentual
                                        FROM
                                        obras2.supervisao s
                                        WHERE
                                        s.obrid = oi.obrid AND
                                        s.emsid IS NULL AND s.smiid IS NULL AND
                                        s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S' AND
                                        s.usucpf IS NOT NULL AND s.rsuid = 1
                                        ORDER BY
                                        s.supdata DESC LIMIT 3
                                        ) as sup ON sup.supid = s.supid
                                        ORDER BY s.supdata ASC LIMIT 1)
                                        END
                                    ) as avanco,
                                    TO_CHAR(oi.obrdtfim,'DD/MM/YYYY') as obrdtfim,

                                    (SELECT
                                          TO_CHAR(NOW() - s.supdata, 'DD')
                                        FROM
                                          obras2.supervisao s
                                        WHERE
                                          s.obrid = oi.obrid AND
                                          s.emsid IS NULL AND s.smiid IS NULL AND
                                          s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S' AND
                                          ( SELECT CASE WHEN SUM(icovlritem) > 0 THEN ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2) ELSE 0 END AS total FROM obras2.itenscomposicaoobra i JOIN obras2.cronograma cro ON cro.croid = s.croid AND cro.croid = i.croid AND cro.crostatus IN ('A', 'H') LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL WHERE i.icostatus = 'A' AND i.relativoedificacao = 'D' AND cro.obrid = oi.obrid AND i.obrid = cro.obrid) >= ".$_REQUEST["dqtdatingido"]."
                                        ORDER BY s.supdata LIMIT 1) as dqtdatingido,
                                    CASE WHEN oi.medidasexcecao IS TRUE THEN 'Sim' ELSE 'Não' END medidasexcecao,
                                    sti.stidesc as stiid,
                                    frp.frpdesc as frpid, 
                                    
                                    (SELECT 
                                     COUNT(*) total
                                    FROM
                                       obras2.empreendimento e
                                    LEFT JOIN obras2.obras                            o ON o.empid    = e.empid AND o.obrstatus = 'A' AND o.obridpai IS NULL
                                    JOIN obras2.supervisao_os_obra              oo ON oo.empid   = e.empid  AND oo.soostatus = 'A'
                                    JOIN obras2.supervisao_os                   os ON os.sosid   = oo.sosid AND os.sosstatus = 'A'
                                    LEFT JOIN obras2.supervisaoempresa          se ON se.sosid   = os.sosid AND se.suestatus = 'A' AND se.empid = e.empid
                                    LEFT JOIN workflow.documento                d1 ON d1.docid   = se.docid	
                                    LEFT JOIN obras2.supervisao                 su ON su.sueid   = se.sueid AND su.supstatus = 'A'
                                    WHERE d1.esdid <> 1188 AND o.obrid = oi.obrid 
                                    GROUP BY o.obrid) as qtdsupervisaoempresa
                                    
				
			FROM
				obras2.obras oi
                                
			".$innerFuncionamento." 
                            
		        LEFT JOIN painel.dadosconvenios dco ON dco.dcoprocesso = Replace(Replace(Replace(oi.obrnumprocessoconv,'.',''),'/',''),'-','')
                        ".$joins."
                            
			WHERE 
				oi.obridpai is null


                                ". ( is_array($where) && !empty($where) ? ' AND' . implode(' AND ', $where) : '' )
                                . $stFiltro . "
                                ";
    if($percentualevolucaoinicial != 0 || $percentualevolucaofinal != 100 )
        $sql = "SELECT t.* FROM ( $sql ) as t WHERE t.avanco BETWEEN {$percentualevolucaoinicial} AND {$percentualevolucaofinal}";

    return $sql;
}

ini_set("memory_limit", "4024M");

// salva os POST na tabela
if ($_REQUEST['salvar'] == 1) {
    $existe_rel = 0;
    $sql = sprintf(
            "select prtid from public.parametros_tela where prtdsc = '%s'", $_REQUEST['titulo']
    );
    $existe_rel = $db->pegaUm($sql);
    if ($existe_rel > 0) {
        $sql = sprintf(
                "UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d", $_REQUEST['titulo'], addslashes(addslashes(serialize($_REQUEST))), $_SESSION['usucpf'], $_SESSION['mnuid'], $existe_rel
        );
        $db->executar($sql);
        $db->commit();
    } else {
        $sql = sprintf(
                "INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )", $_REQUEST['titulo'], addslashes(addslashes(serialize($_REQUEST))), 'FALSE', $_SESSION['usucpf'], $_SESSION['mnuid']
        );
        $db->executar($sql);
        $db->commit();
    }
    ?>
    <script type="text/javascript">
        alert('Operação realizada com sucesso!');
        location.href = '/obras2/obras2.php?modulo=relatorio/relatorio_geral&acao=A';
    </script>
    <?php
    die;
}

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "4024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// Inclui componente de relatórios
include APPRAIZ . 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = obras_monta_sql_relatorio2();
$agrupador = obras_monta_agp_relatorio2();
$coluna    = obras_monta_coluna_relatorio2();
$dados     = $db->carregar($sql);

$rel->setAgrupador($agrupador, $dados);
$rel->setColuna($coluna);
$rel->setTolizadorLinha(false);

// Gera o XLS do relatório
if ($_REQUEST['pesquisa'] == '2') {
    ob_clean();
    $nomeDoArquivoXls = 'relatorio';
    echo $rel->getRelatorioXls();
    die;
}
?>
<!doctype>
<html>
    <head>
        <title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    </head>
    <body>
        <center>
            <!--  Cabeçalho Brasão -->
            <?php echo monta_cabecalho_relatorio('95'); ?>
        </center>
        <!--  Monta o Relatório -->
        <?php echo $rel->getRelatorio(); ?>
    </body>
</html>
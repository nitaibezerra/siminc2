<?PHP

function mostrar_profissional($filtroSQL = false) {
    global $db, $destino;

    $sql = "
        SELECT  DISTINCT edc.estuf as estuf,
                mun.mundescricao,
                mun.muncod,
                ent.entid,
                ent.entnome as entnome,
                uor.entsig,
                ex.exidsc,

                CASE
                    WHEN cam.cmpsituacao = 'F' THEN 'Funcionando'
                    WHEN cam.cmpsituacao = 'N' THEN 'Não Funcionando'
                END as cmpsituacao,

                CASE
                    WHEN cam.cmpinstalacao = 'P' THEN 'Instalações Provisórias'
                    WHEN cam.cmpinstalacao = 'D' THEN 'Instalações Definitivas'
                    WHEN cam.cmpinstalacao = 'S' THEN 'Sem Instalações'
                END as cmpinstalacao,

                CASE
                    WHEN cam.cmpsituacaoobra = 'L' THEN 'Licitação de Obras'
                    WHEN cam.cmpsituacaoobra = 'A' THEN 'Obras em Andamento'
                    WHEN cam.cmpsituacaoobra = 'C' THEN 'Obras Concluídas'
                    WHEN cam.cmpsituacaoobra = 'E' THEN 'Elaboração de Projetos'
                    WHEN cam.cmpsituacaoobra = 'D' THEN 'Dominialidade de Imóvel'
                END as cmpsituacaoobra,

                ano_05.ano2005,
                ano_06.ano2006,
                ano_07.ano2007,
                ano_08.ano2008,
                ano_09.ano2009,
                ano_10.ano2010,
                ano_11.ano2011,
                ano_12.ano2012,
                ano_13.ano2013,
                ano_14.ano2014,

                to_char(cam.cmpdatainauguracao::date,'DD/MM/YYYY') as cmpdatainauguracao,

                CASE WHEN cam.cmpdataimplantacao != ''
                        THEN substr(cam.cmpdataimplantacao, 5, 2) || '/' || substr(cam.cmpdataimplantacao, 1, 4)
                        ELSE ''
                END as cmpdataimplantacao,

                pre.insprevisto,
                rea.insrealizado,
                rea05_10.insrealizado2005_2010,
                pre11_14.insprevisto2011_2014,
                rea11.insrealizadomaior2011,

                (SELECT CASE WHEN count(*) >= 1 THEN 'Sim' ELSE 'Não' END FROM obras.obrainfraestrutura oi WHERE stoid = 1 AND obsstatus = 'A' AND entidcampus = cam.entid) AS obrascampus,

                CASE WHEN cam.datacriacao != ''
                        THEN substr(cam.datacriacao, 5, 2) || '/' || substr(cam.datacriacao, 1, 4)
                        ELSE ''
                END as datacriacao,

                ct.cptdsc,
                fun.dirigentecampus,
                itl.interlocutorcampus,
                uor.entunicod,
                uor.entnome as unidadeorc,

                COALESCE(edc.endcep, '') as cepcampus,
                COALESCE(edc.endbai, '') as bairrocampus,
                COALESCE(edc.endlog, '') as enderecocampus,
                COALESCE(edc.endnum, '') as numerocampus,
                COALESCE(endo.endcep, '') as cepuniversidade,
                COALESCE(endo.endbai, '') as bairrouniversidade,
                COALESCE(endo.endlog, '') as enderecouniversidade,
                COALESCE(endo.endnum, '') as numerouniversidade,

                replace(ent.entemail, ';' ,' / ' ) as emailcampus,
                '(' || COALESCE(ent.entnumdddcomercial, '') || ')' || COALESCE(ent.entnumcomercial, '') as telefonecampus,

                edc.medlatitude as latitude,
                edc.medlongitude as longitude

        FROM academico.campus cam

        LEFT JOIN academico.existencia ex ON ex.exiid = cam.exiid
        LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid
        LEFT JOIN entidade.endereco edc ON edc.entid = ent.entid
        LEFT JOIN territorios.municipio mun ON mun.muncod = edc.muncod
        LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
        LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
        LEFT JOIN entidade.entidade uor ON uor.entid = fea.entid
        LEFT JOIN entidade.endereco endo ON endo.entid = uor.entid
        LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = uor.entid
        LEFT JOIN academico.orgaouo tuo ON tuo.funid = fen2.funid
        LEFT JOIN academico.campustipo ct ON ct.cptid = cam.cptid

        LEFT JOIN(
            SELECT  entid,
                    sum(dsh.dshqtde::integer) as ano2005
            FROM painel.seriehistorica sh
            JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2005'
            GROUP BY entid
        ) AS ano_05 ON ano_05.entid = cam.entid

        LEFT JOIN(
            SELECT  entid,
                    sum(dsh.dshqtde::integer) as ano2006
            FROM painel.seriehistorica sh
            JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2006'
            GROUP BY entid
        ) AS ano_06 ON ano_06.entid = cam.entid

        LEFT JOIN(
            SELECT  entid,
                    sum(dsh.dshqtde::integer) as ano2007
            FROM painel.seriehistorica sh
            JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2007'
            GROUP BY entid
        ) AS ano_07 ON ano_07.entid = cam.entid

        LEFT JOIN(
            SELECT  entid,
                    sum(dsh.dshqtde::integer) as ano2008
            FROM painel.seriehistorica sh
            JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2008'
            GROUP BY entid
        ) AS ano_08 ON ano_08.entid = cam.entid

        LEFT JOIN(
            SELECT  entid,
                    sum(dsh.dshqtde::integer) as ano2009
            FROM painel.seriehistorica sh
            JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2009'
            GROUP BY entid
        ) AS ano_09 ON ano_09.entid = cam.entid

        LEFT JOIN(
            SELECT  entid,
                    sum(dsh.dshqtde::integer) as ano2010
            FROM painel.seriehistorica sh
            JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2010'
            GROUP BY entid
        ) AS ano_10 ON ano_10.entid = cam.entid

        LEFT JOIN(
            SELECT 	entid,
                    sum(dsh.dshqtde::integer) as ano2011
            FROM painel.seriehistorica sh
            JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2011'
            GROUP BY entid
        ) AS ano_11 ON ano_11.entid = cam.entid

        LEFT JOIN(
            SELECT  entid,
                    sum(dsh.dshqtde::integer) as ano2012
            FROM painel.seriehistorica sh
            JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2012'
            GROUP BY entid
        ) AS ano_12 ON ano_12.entid = cam.entid

        LEFT JOIN(
            SELECT  entid,
                    sum(dsh.dshqtde::integer) as ano2013
            FROM painel.seriehistorica sh
            JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2013'
            GROUP BY entid
        ) AS ano_13 ON ano_13.entid = cam.entid

        LEFT JOIN(
            SELECT  entid,
                    sum(dsh.dshqtde::integer) as ano2014
            FROM painel.seriehistorica sh
            JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2014'
            GROUP BY entid
        ) AS ano_14 ON ano_14.entid = cam.entid

        LEFT JOIN(
            SELECT  DISTINCT entnome AS dirigentecampus,
                    fea.entid
            FROM entidade.entidade etn
            JOIN entidade.funcaoentidade fnt ON etn.entid = fnt.entid AND fnt.funid = 24 AND fuestatus = 'A'
            LEFT JOIN academico.membroconselho AS mb ON mb.entid = fnt.entid AND mb.mcssituacao = 't'
            JOIN entidade.funentassoc fea ON fea.fueid = fnt.fueid AND fea.feaid = mb.feaid
            WHERE etn.entnome IS NOT NULL AND mb.mcsvigenciainicial IS NOT NULL 
        ) AS fun ON fun.entid = ent.entid

        LEFT JOIN(
            SELECT  DISTINCT entnome AS interlocutorcampus,
                    fea.entid
            FROM entidade.entidade etn
            JOIN entidade.funcaoentidade fnt ON etn.entid = fnt.entid AND fnt.funid = 40 AND fuestatus = 'A'
            LEFT JOIN academico.membroconselho AS mb ON mb.entid = fnt.entid AND mb.mcssituacao = 't'
            JOIN entidade.funentassoc fea ON fea.fueid = fnt.fueid  AND fea.feaid = mb.feaid
            WHERE etn.entnome IS NOT NULL AND mb.mcsvigenciainicial IS NOT NULL
        ) AS itl ON itl.entid = ent.entid

        LEFT JOIN(
            SELECT  cmi.cmpid,
                    trim(to_char(SUM(cpivalor), '999g999g999g999d99')) AS insprevisto
            FROM academico.campusitem cmi
            WHERE cmi.itmid = " . ITM_INVS_PREVISTO_PROF . " AND cmi.cpitabnum = 1
            GROUP BY cmi.cmpid
        ) AS pre ON pre.cmpid = cam.cmpid

        LEFT JOIN(
            SELECT  cmi.cmpid,
                    trim(to_char(SUM(cpivalor), '999g999g999g999d99')) AS insprevisto2011_2014
            FROM academico.campusitem cmi
            WHERE cmi.itmid = " . ITM_INVS_PREVISTO_PROF . " AND cmi.cpiano BETWEEN '2011' AND '2014' AND cmi.cpitabnum = 1
            GROUP BY cmi.cmpid
        ) AS pre11_14 ON pre11_14.cmpid = cam.cmpid

        LEFT JOIN(
            SELECT  cmi.cmpid,
                    trim(to_char(SUM(cpivalor), '999g999g999g999d99')) AS insrealizado
            FROM academico.campusitem cmi
            WHERE cmi.itmid = " . ITM_INVS_REALIZADO_PROF . " AND cmi.cpitabnum = 1
            GROUP BY cmi.cmpid
        ) AS rea ON rea.cmpid = cam.cmpid

        LEFT JOIN(
            SELECT  cmi.cmpid,
                    trim(to_char(SUM(cpivalor), '999g999g999g999d99')) AS insrealizado2005_2010
            FROM academico.campusitem cmi
            WHERE cmi.itmid = " . ITM_INVS_REALIZADO_PROF . " AND cmi.cpiano BETWEEN '2005' AND '2010' AND cmi.cpitabnum = 1
            GROUP BY cmi.cmpid
        ) AS rea05_10 ON rea05_10.cmpid = cam.cmpid

        LEFT JOIN(
            SELECT  cmi.cmpid,
                    trim(to_char(SUM(cpivalor), '999g999g999g999d99')) AS insrealizadomaior2011
            FROM academico.campusitem cmi
            WHERE cmi.itmid = " . ITM_INVS_REALIZADO_PROF . " AND cmi.cpiano >= '2011' AND cmi.cpitabnum = 1
            GROUP BY cmi.cmpid
        ) AS rea11 On rea11.cmpid = cam.cmpid

        WHERE ent.entstatus = 'A' " . ( ($filtroSQL) ? "AND " . implode(" AND ", $filtroSQL) : "" ) . "

        ORDER BY edc.estuf, uor.entsig, ent.entnome
    ";


//	$sql =  "
//            SELECT  edc.estuf as estuf,
//                    mun.mundescricao,
//                    mun.muncod,
//                    ent.entid,
//                    ent.entnome as entnome,
//                    uor.entsig,
//                    ex.exidsc,
//
//                    CASE
//                        WHEN cam.cmpsituacao = 'F' THEN 'Funcionando'
//                        WHEN cam.cmpsituacao = 'N' THEN 'Não Funcionando'
//                    END as cmpsituacao,
//
//                    CASE
//                        WHEN cam.cmpinstalacao = 'P' THEN 'Instalações Provisórias'
//                        WHEN cam.cmpinstalacao = 'D' THEN 'Instalações Definitivas'
//                        WHEN cam.cmpinstalacao = 'S' THEN 'Sem Instalações'
//                    END as cmpinstalacao,
//
//                    CASE
//                        WHEN cam.cmpsituacaoobra = 'L' THEN 'Licitação de Obras'
//                        WHEN cam.cmpsituacaoobra = 'A' THEN 'Obras em Andamento'
//                        WHEN cam.cmpsituacaoobra = 'C' THEN 'Obras Concluídas'
//                        WHEN cam.cmpsituacaoobra = 'E' THEN 'Elaboração de Projetos'
//                        WHEN cam.cmpsituacaoobra = 'D' THEN 'Dominialidade de Imóvel'
//                    END as cmpsituacaoobra,
//
//                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
//                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2005' AND entid = cam.entid) as ano2005,
//
//                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
//                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2006' AND entid = cam.entid) as ano2006,
//
//                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
//                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2007' AND entid = cam.entid) as ano2007,
//
//                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
//                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2008' AND entid = cam.entid) as ano2008,
//
//                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
//                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2009' AND entid = cam.entid) as ano2009,
//
//                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
//                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2010' AND entid = cam.entid) as ano2010,
//
//                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
//                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2011' AND entid = cam.entid) as ano2011,
//
//                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
//                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2012' AND entid = cam.entid) as ano2012,
//
//                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
//                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2013' AND entid = cam.entid) as ano2013,
//
//                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
//                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2014' AND entid = cam.entid) as ano2014,
//
//                    to_char(cam.cmpdatainauguracao::date,'DD/MM/YYYY') as cmpdatainauguracao,
//
//                    CASE WHEN cam.cmpdataimplantacao != ''
//                        THEN substr(cam.cmpdataimplantacao, 5, 2) || '/' || substr(cam.cmpdataimplantacao, 1, 4)
//                        ELSE ''
//                    END as cmpdataimplantacao,
//
//                    (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVS_PREVISTO_PROF."' AND cmi.cpitabnum=1) AS insprevisto,
//
//                    (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVS_REALIZADO_PROF."' AND cmi.cpitabnum=1) AS insrealizado,
//
//                    (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVS_REALIZADO_PROF."' AND cmi.cpiano BETWEEN '2005' AND '2010' AND cmi.cpitabnum=1) AS insrealizado2005_2010,
//
//                    (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVS_PREVISTO_PROF."' AND cmi.cpiano BETWEEN '2011' AND '2014' AND cmi.cpitabnum=1) AS insprevisto2011_2014,
//
//                    (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVS_REALIZADO_PROF."' AND cmi.cpiano >= '2011' AND cmi.cpitabnum=1) AS insrealizadomaior2011,
//
//                    (SELECT CASE WHEN count(*) >= 1 THEN 'Sim' ELSE 'Não' END FROM obras.obrainfraestrutura oi WHERE stoid = 1 AND obsstatus = 'A' AND entidcampus = cam.entid) AS obrascampus,
//
//                    CASE WHEN cam.datacriacao != ''
//                        THEN substr(cam.datacriacao, 5, 2) || '/' || substr(cam.datacriacao, 1, 4)
//                        ELSE ''
//                    END as datacriacao,
//
//                    ct.cptdsc,
//
//                    --(SELECT DISTINCT entnome FROM entidade.entidade etn JOIN entidade.funcaoentidade fnt ON etn.entid = fnt.entid AND fnt.funid = 24 AND fuestatus = 'A' JOIN entidade.funentassoc fea ON fea.fueid = fnt.fueid WHERE fea.entid = ent.entid AND entstatus = 'A' ) as dirigentecampus,
//
//                    uor.entunicod,
//                    uor.entnome as unidadeorc,
//                    COALESCE(edc.endcep, '') as cepcampus,
//                    COALESCE(edc.endbai, '') as bairrocampus,
//                    COALESCE(edc.endlog, '') as enderecocampus,
//                    COALESCE(edc.endnum, '') as numerocampus,
//                    COALESCE(endo.endcep, '') as cepuniversidade,
//                    COALESCE(endo.endbai, '') as bairrouniversidade,
//                    COALESCE(endo.endlog, '') as enderecouniversidade,
//                    COALESCE(endo.endnum, '') as numerouniversidade,
//                    replace(ent.entemail, ';' ,' / ' ) as emailcampus,
//                    '(' || COALESCE(ent.entnumdddcomercial, '') || ')' || COALESCE(ent.entnumcomercial, '') as telefonecampus,
//                    edc.medlatitude as latitude,
//                    edc.medlongitude as longitude
//
//            FROM academico.campus cam
//
//            LEFT JOIN academico.existencia ex ON ex.exiid = cam.exiid
//            LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid
//            LEFT JOIN entidade.endereco edc ON edc.entid = ent.entid
//            LEFT JOIN territorios.municipio mun ON mun.muncod = edc.muncod
//            LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
//            LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
//            LEFT JOIN entidade.entidade uor ON uor.entid = fea.entid
//            LEFT JOIN entidade.endereco endo ON endo.entid = uor.entid
//            LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = uor.entid
//            LEFT JOIN academico.orgaouo tuo ON tuo.funid = fen2.funid
//            LEFT JOIN academico.campustipo ct ON ct.cptid = cam.cptid
//
//            WHERE ent.entstatus = 'A' " . ( ($filtroSQL) ? "AND " . implode(" AND ",$filtroSQL) : "" ) . "
//
//            ORDER BY edc.estuf, uor.entsig, ent.entnome
//        ";
        //ver($sql, d);
    $dadoscsv = $db->carregar($sql);

    if ($dadoscsv[0]){
        $colspan = false;
        $rowspan = false;
        
        if ($_REQUEST['agrupador_' . $_REQUEST['orgid']]) {
            foreach ($_REQUEST['agrupador_' . $_REQUEST['orgid']] as $col) {
                if ($destino[$_REQUEST['orgid']][$col]['codigo'] == "matriculas") {
                    $qntRegistros = count($_REQUEST['agrupador_2']) + 9;
                    $colspan = count($destino[$_REQUEST['orgid']]) + 2;
                    $rowspan = 2;
                } else {
                    if (!$colspan)
                        $colspan = count($destino[$_REQUEST['orgid']]);
                    if (!$qntRegistros)
                        $qntRegistros = count($_REQUEST['agrupador_2']);
                    if (!$rowspan)
                        $rowspan = 1;
                }
            }
        }
        
        #GERANDO CABEÇALHO EXCEL, CSV E HTML
        $cc = 0;
        $xls = new GeraExcel();

        $html = "
            <script language=\"JavaScript\" src=\"../../includes/funcoes.js\"></script>
            <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
            <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>
            <table class='tabela'>
                <tr bgcolor=\"#C0C0C0\">
                    <td align='center'> 
                        MINISTÉRIO DA EDUCAÇÃO<br> Responsável pela informação: Secretaria de Educação Profissional e Tecnológica (SETEC)<br> <br> Expansão da Educação Profissional e Tecnológica
                    </td>
                </tr>
            </table>
            <table class='tabela'>
        ";

        //ver($_REQUEST['agrupador_'.$_REQUEST['orgid']],d);
        if ($_REQUEST['agrupador_' . $_REQUEST['orgid']]){
            $html .= "<tr>";

            foreach ($_REQUEST['agrupador_' . $_REQUEST['orgid']] as $col) {
                if ($destino[$_REQUEST['orgid']][$col]['codigo'] == "matriculas"){
                    if (!$colspan)
                        $colspan = count($destino[$_REQUEST['orgid']]) + 2;

                    $html .= "<td colspan='10' class='SubTituloCentro'>MATRÍCULAS</td>";
                    $htmlaux = "
                        <tr>
                            <td class='SubTituloCentro'>2005</td>
                            <td class='SubTituloCentro'>2006</td>
                            <td class='SubTituloCentro'>2007</td>
                            <td class='SubTituloCentro'>2008</td>
                            <td class='SubTituloCentro'>2009</td>
                            <td class='SubTituloCentro'>2010</td>
                            <td class='SubTituloCentro'>2011</td>
                            <td class='SubTituloCentro'>2012</td>
                            <td class='SubTituloCentro'>2013</td>
                            <td class='SubTituloCentro'>2014</td>
                        </tr>
                    ";

                    if ($_REQUEST['orgid'] == 2) {
                        $cabecalhoTituloEspecifico = 'atendidas';
                    } else {
                        $cabecalhoTituloEspecifico = 'ofertadas';
                    }

                    $xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2013");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2014");
                    $cc++;

                    $csv .= "Matrículas ofertadas - Ano 2005;Matrículas ofertadas - Ano 2006;Matrículas ofertadas - Ano 2007;Matrículas ofertadas - Ano 2008;Matrículas ofertadas - Ano 2009;Matrículas ofertadas - Ano 2010;Matrículas ofertadas - Ano 2011;Matrículas ofertadas - Ano 2012;Matrículas ofertadas - Ano 2013;Matrículas ofertadas - Ano 2014" . (($cc == $qntRegistros) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecocampus") {
                    if (!$colspan)
                        $colspan = count($destino[$_REQUEST['orgid']]) + 2;
                    
                    $qntRegistros = $qntRegistros + 3;
                    
                    $html .= "<td rowspan='" . $rowspan . "' class='SubTituloCentro'>CEP do Campus</td><td rowspan='" . $rowspan . "' class='SubTituloCentro'>Bairro do Campus</td><td rowspan='" . $rowspan . "' class='SubTituloCentro'>Endereço do Campus</td><td rowspan='" . $rowspan . "' class='SubTituloCentro'>Número do Campus</td>";

                    $xls->MontaConteudoString(0, $cc, "CEP do Campus");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Bairro do Campus");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Endereço do Campus");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Número do Campus");
                    $cc++;

                    $csv .= "CEP do Campus;Bairro do Campus;Endereço do Campus;Número do Campus" . (($cc == $qntRegistros) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecounidadeorc") {
                    $qntRegistros = $qntRegistros + 3;

                    $html .= "<td rowspan='" . $rowspan . "' class='SubTituloCentro'>CEP do Instituto</td><td rowspan='" . $rowspan . "' class='SubTituloCentro'>Bairro do Instituto</td><td rowspan='" . $rowspan . "' class='SubTituloCentro'>Endereço do Instituto</td><td rowspan='" . $rowspan . "' class='SubTituloCentro'>Número do Instituto</td>";

                    $xls->MontaConteudoString(0, $cc, "CEP do Instituto");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Bairro do Instituto");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Endereço do Instituto");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Número do Instituto");
                    $cc++;

                    $csv .= "CEP do Instituto;Bairro do Instituto;Endereço do Instituto;Número do Instituto" . (($cc == $qntRegistros) ? "\n" : ";");
                } else {
                    $html .= "<td rowspan='" . $rowspan . "' class='SubTituloCentro'>" . $destino[$_REQUEST['orgid']][$col]['descricao'] . "</td>";

                    $xls->MontaConteudoString(0, $cc, $destino[$_REQUEST['orgid']][$col]['descricao']);
                    $cc++;
                    $csv .= $destino[$_REQUEST['orgid']][$col]['descricao'] . (($cc == $qntRegistros) ? " \n" : ";");
                }
            }
            $html .= "</tr>";

            if ($htmlaux) {
                $html .= $htmlaux;
            }
        }
        #FIM - GERANDO CABEÇALHO EXCEL, CSV E HTML

        $i = 1;
        foreach ($dadoscsv as $registro){
            $cc = 0;
            
            if (fmod($i, 2) == 0)
                $marcado = '';
            else
                $marcado = '#F7F7F7';

            if ($_REQUEST['agrupador_' . $_REQUEST['orgid']]){
                $html .= "<tr bgcolor='" . $marcado . "'>";

                foreach ($_REQUEST['agrupador_' . $_REQUEST['orgid']] as $col) {
                    if ($destino[$_REQUEST['orgid']][$col]['codigo'] == "matriculas") {

                        $html .= "
                            <td>" . $registro['ano2005'] . "</td>
                            <td>" . $registro['ano2006'] . "</td>
                            <td>" . $registro['ano2007'] . "</td>
                            <td>" . $registro['ano2008'] . "</td>
                            <td>" . $registro['ano2009'] . "</td>
                            <td>" . $registro['ano2010'] . "</td>
                            <td>" . $registro['ano2011'] . "</td>
                            <td>" . $registro['ano2012'] . "</td>
                            <td>" . $registro['ano2013'] . "</td>
                            <td>" . $registro['ano2014'] . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, $registro['ano2005']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['ano2006']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['ano2007']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['ano2008']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['ano2009']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['ano2010']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['ano2011']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['ano2012']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['ano2013']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['ano2014']);
                        $cc++;

                        $csv .= $registro['ano2005'] . ";" . $registro['ano2006'] . ";" . $registro['ano2007'] . ";" . $registro['ano2008'] . ";" . $registro['ano2009'] . ";" . $registro['ano2010'] . ";" . $registro['ano2011'] . ";" . $registro['ano2012'] . ";" . $registro['ano2013'] . ";" . $registro['ano2014'] . (($cc == $qntRegistros) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecocampus") {

                        $tam = strlen($registro['cepcampus']);
                        $cep = substr($registro['cepcampus'], 0, 5) . '-' . substr($registro['cepcampus'], 5, $tam);

                        $html .= "
                            <td>" . $cep . "</td>
                            <td>" . $registro['bairrocampus'] . "</td>
                            <td>" . $registro['enderecocampus'] . "</td>
                            <td>" . $registro['numerocampus'] . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, $cep);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['bairrocampus']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['enderecocampus']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['numerocampus']);
                        $cc++;

                        $csv .= $cep . ";" . $registro['bairrocampus'] . ";" . $registro['enderecocampus'] . ";" . $registro['numerocampus'] . (($cc == $qntRegistros) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecounidadeorc") {

                        $tam = strlen($registro['cepuniversidade']);
                        $cep = substr($registro['cepuniversidade'], 0, 5) . '-' . substr($registro['cepuniversidade'], 5, $tam);

                        $html .= "
                            <td>" . $cep . "</td>
                            <td>" . $registro['bairrouniversidade'] . "</td>
                            <td>" . $registro['enderecouniversidade'] . "</td>
                            <td>" . $registro['numerouniversidade'] . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, $cep);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['bairrouniversidade']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['enderecouniversidade']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['numerouniversidade']);
                        $cc++;

                        $csv .= $cep . ";" . $registro['bairrouniversidade'] . ";" . $registro['enderecouniversidade'] . ";" . $registro['numerouniversidade'] . (($cc == $qntRegistros) ? "\n" : ";");
                    } else {
                        $html .= "<td>" . $registro[$col] . "</td>";

                        $xls->MontaConteudoString($i, $cc, $registro[$col]);
                        $cc++;

                        $csv .= $registro[$col] . (($cc == $qntRegistros) ? "\n" : ";");
                    }
                }
                $html .= "</tr>";
            }
            $i++;
        }

        $html .= "</table>";
        $msg = $_POST['obs_consulta'];
        //Roda pé em HTML: Observação feita pelo usuário na mantagem do relatório.
        $html .= "
            <table class='tabela'>
                <tr bgcolor=\"#C0C0C0\">
                    <td colspan='27' align='center'>\n
                        OBSERVAÇÃO: " . $msg . "
                    </td>
                </tr>
            </table>
        ";

        //Roda pé em XLS: Observação feita pelo usuário na mantagem do relatório.
        $xls->MontaConteudoString($i + 1, 0, 'OBSERVAÇÃO:');
        $xls->MontaConteudoString($i + 1, 1, $msg);

        //Roda pé em CSV: Observação feita pelo usuário na mantagem do relatório.
        $csv .= "\n\n" . "OBSERVAÇÃO: " . ";" . $msg;
    } else {
        $html = "
            <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
            <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>
            <table class='tabela'><tr bgcolor=\"#C0C0C0\">
                <tr>
                    <td align='center'>MINISTÉRIO DA EDUCAÇÃO<br>Responsável pela informação: Secretaria de Educação Profissional e Tecnológica (SETEC)<br><br>Expansão da Educação Profissional e Tecnológica</td>
                </tr>
                <tr>
                    <td style='color:#ff0000; text-align:center'>Não foram encontrados registros.</td>
                </tr>
            </table>
        ";
    }
    
    $nome_arquivo = "relatorio_nota_tecnica_" . date('d-m-Y_H_i') . ".csv";
    switch (substr($_POST['exibir'], 10, 3)) {
        case 'csv':
            header("Content-type: application/octet-stream");
            header("Content-Disposition: attachment; filename=\"$nome_arquivo\"");
            echo $csv;
            break;
        case 'xls':
            ob_clean();
            $xls->GeraArquivo();
            break;
        case 'htm':
            echo $html;
            break;
    }
    exit;
}

function mostrar_superior($filtroSQL = false) {
    global $db, $destino;

    $sql_campos = "";

    if (in_array('docenteprevisto', $_POST['agrupador_1'])) {
        $sql_campos .= "
            -- Docente (Previsto)
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_PREV_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS docenteprevano2005,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_PREV_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS docenteprevano2006,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_PREV_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS docenteprevano2007,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_PREV_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS docenteprevano2008,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_PREV_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS docenteprevano2009,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_PREV_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS docenteprevano2010,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_PREV_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS docenteprevano2011,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_PREV_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS docenteprevano2012,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_PREV_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS docenteprevano2013,
        ";
    }

    if (in_array('tecnicoprevisto', $_POST['agrupador_1'])) {
        $sql_campos .= "
            -- Técnico (Previsto)
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_PREV_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS tecnicoprevano2005,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_PREV_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS tecnicoprevano2006,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_PREV_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS tecnicoprevano2007,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_PREV_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS tecnicoprevano2008,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_PREV_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS tecnicoprevano2009,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_PREV_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS tecnicoprevano2010,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_PREV_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS tecnicoprevano2011,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_PREV_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS tecnicoprevano2012,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_PREV_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS tecnicoprevano2013,
        ";
    }
    if (in_array('matprevisto', $_POST['agrupador_1'])) {
        $sql_campos .= "
            -- Mat (Previsto)
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_PREV_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS matprevano2005,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_PREV_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS matprevano2006,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_PREV_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS matprevano2007,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_PREV_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS matprevano2008,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_PREV_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS matprevano2009,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_PREV_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS matprevano2010,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_PREV_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS matprevano2011,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_PREV_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS matprevano2012,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_PREV_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS matprevano2013,
        ";
    }
    if (in_array('vagasprevisto', $_POST['agrupador_1'])) {
        $sql_campos .= "
            -- Vagas (Previsto)
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_PREV_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS vagasprevano2005,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_PREV_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS vagasprevano2006,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_PREV_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS vagasprevano2007,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_PREV_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS vagasprevano2008,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_PREV_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS vagasprevano2009,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_PREV_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS vagasprevano2010,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_PREV_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS vagasprevano2011,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_PREV_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS vagasprevano2012,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_PREV_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS vagasprevano2013,
        ";
    }
    if (in_array('cursoprevisto', $_POST['agrupador_1'])) {
        $sql_campos .= "
            -- Cursos (Previsto)
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_PREV_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS cursoprevano2005,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_PREV_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS cursoprevano2006,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_PREV_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS cursoprevano2007,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_PREV_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS cursoprevano2008,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_PREV_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS cursoprevano2009,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_PREV_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS cursoprevano2010,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_PREV_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS cursoprevano2011,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_PREV_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS cursoprevano2012,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_PREV_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS cursoprevano2013,
        ";
    }
    if (in_array('investimentoprevisto', $_POST['agrupador_1'])) {
        $sql_campos .= "
            -- Investimentos (Previsto)
                (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_PREV_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS investimentoprevano2005,
                (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_PREV_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS investimentoprevano2006,
                (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_PREV_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS investimentoprevano2007,
                (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_PREV_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS investimentoprevano2008,
                (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_PREV_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS investimentoprevano2009,
                (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_PREV_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS investimentoprevano2010,
                (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_PREV_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS investimentoprevano2011,
                (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_PREV_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS investimentoprevano2012,
                (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_PREV_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS investimentoprevano2013,
            ";
    }
    if (in_array('bolsasmestradoprevisto', $_POST['agrupador_1'])) {
        $sql_campos .= "
            -- BOLSAS DE MESTRADO (Previsto)
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_PREV_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2005,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_PREV_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2006,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_PREV_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2007,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_PREV_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2008,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_PREV_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2009,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_PREV_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2010,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_PREV_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2011,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_PREV_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2012,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_PREV_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2013,
        ";
    }
    if (in_array('bolsasdoutoradoprevisto', $_POST['agrupador_1'])) {
        $sql_campos .= "
            -- BOLSAS DE DOUTORADO (Previsto)
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2005,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2006,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2007,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2008,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2009,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2010,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2011,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2012,
            (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2013,
	";
    }
    if (in_array('bolsasposdoutoradoprevisto', $_POST['agrupador_1'])) {
        $sql_campos .= "-- BOLSAS DE PÓS-DOUTORADO (Previsto)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_PREV_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2013,
				 ";
    }
    if (in_array('docenterealizado', $_POST['agrupador_1'])) {
        $sql_campos .= "-- Docente (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_REALIZ_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS docenterealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_REALIZ_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS docenterealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_REALIZ_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS docenterealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_REALIZ_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS docenterealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_REALIZ_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS docenterealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_REALIZ_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS docenterealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_REALIZ_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS docenterealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_REALIZ_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS docenterealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_DOCENTE_REALIZ_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS docenterealizano2013,
				 ";
    }
    if (in_array('tecnicorealizado', $_POST['agrupador_1'])) {
        $sql_campos .= "-- Técnico (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_REALIZ_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS tecnicorealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_REALIZ_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS tecnicorealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_REALIZ_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS tecnicorealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_REALIZ_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS tecnicorealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_REALIZ_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS tecnicorealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_REALIZ_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS tecnicorealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_REALIZ_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS tecnicorealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_REALIZ_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS tecnicorealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_TECNICO_REALIZ_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS tecnicorealizano2013,
				 ";
    }
    if (in_array('matrealizado', $_POST['agrupador_1'])) {
        $sql_campos .= "-- Mat (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_REALIZ_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS matrealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_REALIZ_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS matrealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_REALIZ_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS matrealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_REALIZ_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS matrealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_REALIZ_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS matrealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_REALIZ_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS matrealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_REALIZ_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS matrealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_REALIZ_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS matrealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MAT_REALIZ_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS matrealizano2013,
				 ";
    }
    if (in_array('vagasrealizado', $_POST['agrupador_1'])) {
        $sql_campos .= "-- Vagas (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_REALIZ_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS vagasrealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_REALIZ_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS vagasrealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_REALIZ_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS vagasrealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_REALIZ_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS vagasrealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_REALIZ_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS vagasrealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_REALIZ_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS vagasrealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_REALIZ_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS vagasrealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_REALIZ_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS vagasrealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_VAGAS_REALIZ_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS vagasrealizano2013,
				 ";
    }
    if (in_array('cursorealizado', $_POST['agrupador_1'])) {
        $sql_campos .= "-- Cursos (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_REALIZ_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS cursorealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_REALIZ_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS cursorealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_REALIZ_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS cursorealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_REALIZ_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS cursorealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_REALIZ_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS cursorealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_REALIZ_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS cursorealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_REALIZ_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS cursorealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_REALIZ_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS cursorealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_CURSO_REALIZ_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS cursorealizano2013,
				 ";
    }
    if (in_array('investimentorealizado', $_POST['agrupador_1'])) {
        $sql_campos .= "-- Investimentos (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_REALIZ_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS investimentorealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_REALIZ_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS investimentorealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_REALIZ_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS investimentorealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_REALIZ_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS investimentorealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_REALIZ_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS investimentorealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_REALIZ_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS investimentorealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_REALIZ_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS investimentorealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_REALIZ_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS investimentorealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_REALIZ_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS investimentorealizano2013,
				 ";
    }
    if (in_array('bolsasmestradorealizado', $_POST['agrupador_1'])) {
        $sql_campos .= "-- BOLSAS DE MESTRADO (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_REALIZ_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_REALIZ_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_REALIZ_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_REALIZ_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_REALIZ_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_REALIZ_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_REALIZ_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_REALIZ_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_MESTRADO_REALIZ_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2013,
				 ";
    }
    if (in_array('bolsasdoutoradorealizado', $_POST['agrupador_1'])) {
        $sql_campos .= "-- BOLSAS DE DOUTORADO (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_DOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2013,
				 ";
    }
    if (in_array('bolsasposdoutoradorealizado', $_POST['agrupador_1'])) {
        $sql_campos .= "-- BOLSAS DE PÓS-DOUTORADO (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2013,
				 ";
    }
    if (in_array('matriculasgradpresen', $_POST['agrupador_1'])) {
        $sql_campos .= "-- Matrículas (Graduação Presencial)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP . "' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS matriculasgradpresenano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP . "' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS matriculasgradpresenano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP . "' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS matriculasgradpresenano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP . "' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS matriculasgradpresenano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP . "' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS matriculasgradpresenano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP . "' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS matriculasgradpresenano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP . "' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS matriculasgradpresenano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP . "' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS matriculasgradpresenano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP . "' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS matriculasgradpresenano2013,
				 ";
    }

    $sql = "
        SELECT  edc.estuf,
                mun.mundescricao,
                mun.muncod,
                ent.entid,
                ent.entnome,
                uor.entsig,
                ex.exidsc,
                CASE
                    WHEN cam.cmpsituacao = 'F' THEN 'Funcionando'
                    WHEN cam.cmpsituacao = 'N' THEN 'Não Funcionando'
                END as cmpsituacao,
                CASE
                    WHEN cam.cmpinstalacao = 'P' THEN 'Instalações Provisórias'
                    WHEN cam.cmpinstalacao = 'D' THEN 'Instalações Definitivas'
                    WHEN cam.cmpinstalacao = 'S' THEN 'Sem Instalações'
                END as cmpinstalacao,
                CASE
                    WHEN cam.cmpsituacaoobra = 'L' THEN 'Licitação de Obras'
                    WHEN cam.cmpsituacaoobra = 'A' THEN 'Obras em Andamento'
                    WHEN cam.cmpsituacaoobra = 'C' THEN 'Obras Concluídas'
                END as cmpsituacaoobra,
                
                $sql_campos
                    
                to_char(cam.cmpdatainauguracao::date,'DD/MM/YYYY') as cmpdatainauguracao,
                
                CASE WHEN cam.cmpdataimplantacao != '' 
                    THEN substr(cam.cmpdataimplantacao, 5, 2) || '/' || substr(cam.cmpdataimplantacao, 1, 4) 
                    ELSE '' 
                END as cmpdataimplantacao,
                
                (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_PREV_SUP . "' AND cmi.cpitabnum=1) AS insvtot,
                (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_REALIZ_SUP . "' AND cmi.cpiano BETWEEN '2005' AND '2010' AND cmi.cpitabnum=1) AS insrealizado2005_2010,
                (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_PREV_SUP . "' AND cmi.cpiano BETWEEN '2011' AND '2014' AND cmi.cpitabnum=1) AS insprevisto2011_2014,
                (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '" . ITM_INVESTIMENTO_REALIZ_SUP . "' AND cmi.cpiano >= '2011' AND cmi.cpitabnum=1) AS insrealizadomaior2011,
                (SELECT CASE WHEN count(*) >= 1 THEN 'Sim' ELSE 'Não' END FROM obras.obrainfraestrutura oi WHERE stoid = 1 AND obsstatus = 'A' AND entidcampus = cam.entid) AS obrascampus,
                
                CASE WHEN cam.datacriacao != '' 
                    THEN substr(cam.datacriacao, 5, 2) || '/' || substr(cam.datacriacao, 1, 4)
                    ELSE '' 
                END as datacriacao,
                
                ct.cptdsc,
                
                fun.dirigentecampus,
                itl.interlocutorcampus,
                --(SELECT entnome FROM entidade.entidade etn JOIN entidade.funcaoentidade fnt ON etn.entid = fnt.entid AND fnt.funid=24 JOIN entidade.funentassoc fea ON fea.fueid = fnt.fueid WHERE fea.entid=ent.entid) as dirigentecampus,
                
                 uor.entunicod,
                 uor.entnome as unidadeorc,
                 COALESCE(edc.endcep, '') as cepcampus,
                 COALESCE(edc.endbai, '') as bairrocampus,
                 COALESCE(edc.endlog, '') as enderecocampus,
                 COALESCE(edc.endnum, '') as numerocampus,
                 COALESCE(endo.endcep, '') as cepuniversidade,
                 COALESCE(endo.endbai, '') as bairrouniversidade,
                 COALESCE(endo.endlog, '') as enderecouniversidade,
                 COALESCE(endo.endnum, '') as numerouniversidade,
                 ent.entemail as emailcampus,
                 
                 '(' || COALESCE(ent.entnumdddcomercial, '') || ')' || COALESCE(ent.entnumcomercial, '') as telefonecampus
        FROM academico.campus cam
        
        LEFT JOIN academico.existencia ex ON ex.exiid = cam.exiid
        LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid
        LEFT JOIN entidade.endereco edc ON edc.entid = ent.entid
        LEFT JOIN territorios.municipio mun ON mun.muncod = edc.muncod
        LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
        LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
        LEFT JOIN entidade.entidade uor ON uor.entid = fea.entid
        LEFT JOIN entidade.endereco endo ON endo.entid = uor.entid
        LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = uor.entid
        LEFT JOIN academico.orgaouo tuo ON tuo.funid = fen2.funid
        LEFT JOIN academico.campustipo ct ON ct.cptid = cam.cptid
        
        LEFT JOIN(
            SELECT  DISTINCT entnome AS dirigentecampus,
                    fea.entid
            FROM entidade.entidade etn
            JOIN entidade.funcaoentidade fnt ON etn.entid = fnt.entid AND fnt.funid = 24 AND fuestatus = 'A'
            LEFT JOIN academico.membroconselho AS mb ON mb.entid = fnt.entid AND mb.mcssituacao = 't'
            JOIN entidade.funentassoc fea ON fea.fueid = fnt.fueid AND fea.feaid = mb.feaid
            WHERE etn.entnome IS NOT NULL AND mb.mcsvigenciainicial IS NOT NULL
        ) AS fun ON fun.entid = ent.entid

        LEFT JOIN(
            SELECT  DISTINCT entnome AS interlocutorcampus,
                    fea.entid
            FROM entidade.entidade etn
            JOIN entidade.funcaoentidade fnt ON etn.entid = fnt.entid AND fnt.funid = 40 AND fuestatus = 'A'
            LEFT JOIN academico.membroconselho AS mb ON mb.entid = fnt.entid AND mb.mcssituacao = 't'
            JOIN entidade.funentassoc fea ON fea.fueid = fnt.fueid AND fea.feaid = mb.feaid
            WHERE etn.entnome IS NOT NULL AND mb.mcsvigenciainicial IS NOT NULL
        ) AS itl ON itl.entid = ent.entid
        
        WHERE ent.entstatus = 'A' " . ( ($filtroSQL) ? "AND " . implode(" AND ", $filtroSQL) : "" ) . " AND uor.entid NOT IN (388754) --Retirando a BAHIA
            
        ORDER BY edc.estuf, uor.entsig, ent.entnome
    ";
    $dadoscsv = $db->carregar($sql);

    if ($dadoscsv[0]) {
        $colspan = false;
        $rowspan = false;
        
        if ($_REQUEST['agrupador_' . $_REQUEST['orgid']]) {
            foreach ($_REQUEST['agrupador_' . $_REQUEST['orgid']] as $col) {
                if ($destino[$_REQUEST['orgid']][$col]['codigo'] == "matriculas") {
                    $colspan = count($destino[$_REQUEST['orgid']]) + 3;
                    $rowspan = 2;
                } else {
                    if (!$colspan)
                        $colspan = count($destino[$_REQUEST['orgid']]);
                    if (!$rowspan)
                        $rowspan = 1;
                }
            }
        }

        #GERANDO CABEÇALHO EXCEL, CSV E HTML
        $cc = 0;
        $xls = new GeraExcel();

        $html = "
            <script language=\"JavaScript\" src=\"../../includes/funcoes.js\"></script>
                <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
                <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>
                <table class='tabela'>
                    <tr bgcolor=\"#C0C0C0\">
                        <td align='center'> 
                            MINISTÉRIO DA EDUCAÇÃO <br> Responsável pela informação: Secretaria de Educação Profissional e Tecnológica (SETEC) <br> <br> Expansão da Educação Profissional e Tecnológica
                        </td>
                    </tr>
                </table>
                <table class='tabela'>
        ";

        if ($_REQUEST['agrupador_' . $_REQUEST['orgid']]) {
            $html .= "<tr>";
            //$htmlaux .= "<tr>";
            foreach ($_REQUEST['agrupador_' . $_REQUEST['orgid']] as $col) {
                if ($destino[$_REQUEST['orgid']][$col]['codigo'] == "docenteprevisto") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>DOCENTE (PREVISTO)</td>";
                    $htmlaux = "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2013");
                    $cc++;

                    $csv .= "DOCENTE (PREVISTO) - 2005;DOCENTE (PREVISTO) - 2006;DOCENTE (PREVISTO) - 2007;DOCENTE (PREVISTO) - 2008;DOCENTE (PREVISTO) - 2009;DOCENTE (PREVISTO) - 2010;DOCENTE (PREVISTO) - 2011;DOCENTE (PREVISTO) - 2012;DOCENTE (PREVISTO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "tecnicoprevisto") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>TÉCNICO (PREVISTO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2013");
                    $cc++;

                    $csv .= "TÉCNICO (PREVISTO) - 2005;TÉCNICO (PREVISTO) - 2006;TÉCNICO (PREVISTO) - 2007;TÉCNICO (PREVISTO) - 2008;TÉCNICO (PREVISTO) - 2009;TÉCNICO (PREVISTO) - 2010;TÉCNICO (PREVISTO) - 2011;TÉCNICO (PREVISTO) - 2012;TÉCNICO (PREVISTO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "matprevisto") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>MAT (PREVISTO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2013");
                    $cc++;

                    $csv .= "MAT (PREVISTO) - 2005;MAT (PREVISTO) - 2006;MAT (PREVISTO) - 2007;MAT (PREVISTO) - 2008;MAT (PREVISTO) - 2009;MAT (PREVISTO) - 2010;MAT (PREVISTO) - 2011;MAT (PREVISTO) - 2012;MAT (PREVISTO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "vagasprevisto") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>VAGAS (PREVISTO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2013</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2013");
                    $cc++;

                    $csv .= "VAGAS (PREVISTO) - 2005;VAGAS (PREVISTO) - 2006;VAGAS (PREVISTO) - 2007;VAGAS (PREVISTO) - 2008;VAGAS (PREVISTO) - 2009;VAGAS (PREVISTO) - 2010;VAGAS (PREVISTO) - 2011;VAGAS (PREVISTO) - 2012;VAGAS (PREVISTO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "cursoprevisto") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>CURSO (PREVISTO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2013");
                    $cc++;

                    $csv .= "CURSO (PREVISTO) - 2005;CURSO (PREVISTO) - 2006;CURSO (PREVISTO) - 2007;CURSO (PREVISTO) - 2008;CURSO (PREVISTO) - 2009;CURSO (PREVISTO) - 2010;CURSO (PREVISTO) - 2011;CURSO (PREVISTO) - 2012;CURSO (PREVISTO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "investimentoprevisto") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>INVESTIMENTO (PREVISTO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2013");
                    $cc++;

                    $csv .= "INVESTIMENTO (PREVISTO) - 2005;INVESTIMENTO (PREVISTO) - 2006;INVESTIMENTO (PREVISTO) - 2007;INVESTIMENTO (PREVISTO) - 2008;INVESTIMENTO (PREVISTO) - 2009;INVESTIMENTO (PREVISTO) - 2010;INVESTIMENTO (PREVISTO) - 2011;INVESTIMENTO (PREVISTO) - 2012;INVESTIMENTO (PREVISTO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasmestradoprevisto") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>BOLSAS DE MESTRADO (PREVISTO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2013");
                    $cc++;

                    $csv .= "BOLSAS DE MESTRADO (PREVISTO) - 2005;BOLSAS DE MESTRADO (PREVISTO) - 2006;BOLSAS DE MESTRADO (PREVISTO) - 2007;BOLSAS DE MESTRADO (PREVISTO) - 2008;BOLSAS DE MESTRADO (PREVISTO) - 2009;BOLSAS DE MESTRADO (PREVISTO) - 2010;BOLSAS DE MESTRADO (PREVISTO) - 2011;BOLSAS DE MESTRADO (PREVISTO) - 2012;BOLSAS DE MESTRADO (PREVISTO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasdoutoradoprevisto") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>BOLSAS DE DOUTORADO (PREVISTO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2013");
                    $cc++;

                    $csv .= "BOLSAS DE DOUTORADO (PREVISTO) - 2005;BOLSAS DE DOUTORADO (PREVISTO) - 2006;BOLSAS DE DOUTORADO (PREVISTO) - 2007;BOLSAS DE DOUTORADO (PREVISTO) - 2008;BOLSAS DE DOUTORADO (PREVISTO) - 2009;BOLSAS DE DOUTORADO (PREVISTO) - 2010;BOLSAS DE DOUTORADO (PREVISTO) - 2011;BOLSAS DE DOUTORADO (PREVISTO) - 2012;BOLSAS DE DOUTORADO (PREVISTO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasposdoutoradoprevisto") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>BOLSAS DE PÓS-DOUTORADO (PREVISTO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2013");
                    $cc++;

                    $csv .= "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2005;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2006;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2007;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2008;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2009;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2010;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2011;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2012;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "docenterealizado") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>DOCENTE (REALIZADO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2013");
                    $cc++;

                    $csv .= "DOCENTE (REALIZADO) - 2005;DOCENTE (REALIZADO) - 2006;DOCENTE (REALIZADO) - 2007;DOCENTE (REALIZADO) - 2008;DOCENTE (REALIZADO) - 2009;DOCENTE (REALIZADO) - 2010;DOCENTE (REALIZADO) - 2011;DOCENTE (REALIZADO) - 2012;DOCENTE (REALIZADO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "tecnicorealizado") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>TÉCNICO (REALIZADO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2013");
                    $cc++;

                    $csv .= "TÉCNICO (REALIZADO) - 2005;TÉCNICO (REALIZADO) - 2006;TÉCNICO (REALIZADO) - 2007;TÉCNICO (REALIZADO) - 2008;TÉCNICO (REALIZADO) - 2009;TÉCNICO (REALIZADO) - 2010;TÉCNICO (REALIZADO) - 2011;TÉCNICO (REALIZADO) - 2012;TÉCNICO (REALIZADO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "matrealizado") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>MAT (REALIZADO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2013");
                    $cc++;

                    $csv .= "MAT (REALIZADO) - 2005;MAT (REALIZADO) - 2006;MAT (REALIZADO) - 2007;MAT (REALIZADO) - 2008;MAT (REALIZADO) - 2009;MAT (REALIZADO) - 2010;MAT (REALIZADO) - 2011;MAT (REALIZADO) - 2012;MAT (REALIZADO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "vagasrealizado") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>VAGAS (REALIZADO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2013");
                    $cc++;

                    $csv .= "VAGAS (REALIZADO) - 2005;VAGAS (REALIZADO) - 2006;VAGAS (REALIZADO) - 2007;VAGAS (REALIZADO) - 2008;VAGAS (REALIZADO) - 2009;VAGAS (REALIZADO) - 2010;VAGAS (REALIZADO) - 2011;VAGAS (REALIZADO) - 2012;VAGAS (REALIZADO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "cursorealizado") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>CURSO (REALIZADO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2013");
                    $cc++;

                    $csv .= "CURSO (REALIZADO) - 2005;CURSO (REALIZADO) - 2006;CURSO (REALIZADO) - 2007;CURSO (REALIZADO) - 2008;CURSO (REALIZADO) - 2009;CURSO (REALIZADO) - 2010;CURSO (REALIZADO) - 2011;CURSO (REALIZADO) - 2012;CURSO (REALIZADO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "investimentorealizado") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>INVESTIMENTO (REALIZADO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2013");
                    $cc++;

                    $csv .= "INVESTIMENTO (REALIZADO) - 2005;INVESTIMENTO (REALIZADO) - 2006;INVESTIMENTO (REALIZADO) - 2007;INVESTIMENTO (REALIZADO) - 2008;INVESTIMENTO (REALIZADO) - 2009;INVESTIMENTO (REALIZADO) - 2010;INVESTIMENTO (REALIZADO) - 2011;INVESTIMENTO (REALIZADO) - 2012;INVESTIMENTO (REALIZADO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasmestradorealizado") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>BOLSAS DE MESTRADO (REALIZADO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2013");
                    $cc++;

                    $csv .= "BOLSAS DE MESTRADO (REALIZADO) - 2005;BOLSAS DE MESTRADO (REALIZADO) - 2006;BOLSAS DE MESTRADO (REALIZADO) - 2007;BOLSAS DE MESTRADO (REALIZADO) - 2008;BOLSAS DE MESTRADO (REALIZADO) - 2009;BOLSAS DE MESTRADO (REALIZADO) - 2010;BOLSAS DE MESTRADO (REALIZADO) - 2011;BOLSAS DE MESTRADO (REALIZADO) - 2012;BOLSAS DE MESTRADO (REALIZADO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasdoutoradorealizado") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>BOLSAS DE DOUTORADO (REALIZADO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2013");
                    $cc++;

                    $csv .= "BOLSAS DE DOUTORADO (REALIZADO) - 2005;BOLSAS DE DOUTORADO (REALIZADO) - 2006;BOLSAS DE DOUTORADO (REALIZADO) - 2007;BOLSAS DE DOUTORADO (REALIZADO) - 2008;BOLSAS DE DOUTORADO (REALIZADO) - 2009;BOLSAS DE DOUTORADO (REALIZADO) - 2010;BOLSAS DE DOUTORADO (REALIZADO) - 2011;BOLSAS DE DOUTORADO (REALIZADO) - 2012;BOLSAS DE DOUTORADO (REALIZADO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasposdoutoradorealizado") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>BOLSAS DE PÓS-DOUTORADO (REALIZADO)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2013");
                    $cc++;

                    $csv .= "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2005;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2006;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2007;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2008;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2009;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2010;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2011;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2012;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "matriculasgradpresen") {

                    $html .= "<td colspan='9' class='SubTituloCentro'>MATRÍCULAS (GRADUAÇÃO PRESENCIAL)</td>";
                    $htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

                    $xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2005");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2006");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2007");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2008");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2009");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2010");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2011");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2012");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2013");
                    $cc++;

                    $csv .= "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2005;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2006;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2007;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2008;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2009;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2010;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2011;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2012;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2013" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecocampus") {
                    $rowspan = 2;
                    $html .= "<td rowspan='" . $rowspan . "' class='SubTituloCentro'>CEP do Campus</td><td rowspan='" . $rowspan . "' class='SubTituloCentro'>Bairro do Campus</td><td rowspan='" . $rowspan . "' class='SubTituloCentro'>Endereço do Campus</td><td rowspan='" . $rowspan . "' class='SubTituloCentro'>Número do Campus</td>";

                    $xls->MontaConteudoString(0, $cc, "CEP do Campus");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Bairro do Campus");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Endereço do Campus");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Número do Campus");
                    $cc++;

                    $csv .= "CEP do Campus;Bairro do Campus;Endereço do Campus;Número do Campus" . (($cc == $colspan) ? "\n" : ";");
                } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecounidadeorc") {

                    $rowspan = 2;
                    $html .= "<td rowspan='" . $rowspan . "' class='SubTituloCentro'>CEP da Universidade</td><td rowspan='" . $rowspan . "' class='SubTituloCentro'>Bairro da Universidade</td><td rowspan='" . $rowspan . "' class='SubTituloCentro'>Endereço da Universidade</td><td rowspan='" . $rowspan . "' class='SubTituloCentro'>Número da Universidade</td>";

                    $xls->MontaConteudoString(0, $cc, "CEP da Universidade");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Bairro da Universidade");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Endereço da Universidade");
                    $cc++;
                    $xls->MontaConteudoString(0, $cc, "Número da Universidade");
                    $cc++;

                    $csv .= "CEP da Universidade;Bairro da Universidade;Endereço da Universidade;Número da Universidade" . (($cc == $colspan) ? "\n" : ";");
                } else {

                    $rowspan = 2;
                    $html .= "<td rowspan='" . $rowspan . "' class='SubTituloCentro'>" . $destino[$_REQUEST['orgid']][$col]['descricao'] . "</td>";

                    $xls->MontaConteudoString(0, $cc, $destino[$_REQUEST['orgid']][$col]['descricao']);
                    $cc++;

                    $csv .= $destino[$_REQUEST['orgid']][$col]['descricao'] . (($cc == $colspan) ? "\n" : ";");
                }
            }

            $html .= "</tr>";

            if ($htmlaux) {
                $html .= $htmlaux;
            }
        }
        #FIM - GERANDO CABEÇALHO EXCEL, CSV E HTML

        $i = 1;
        foreach($dadoscsv as $registro){
            $cc = 0;
            if (fmod($i, 2) == 0)
                $marcado = '';
            else
                $marcado = '#F7F7F7';

            if ($_REQUEST['agrupador_' . $_REQUEST['orgid']]){
                $html .= "<tr bgcolor='" . $marcado . "'>";

                foreach ($_REQUEST['agrupador_' . $_REQUEST['orgid']] as $col) {
                    if ($destino[$_REQUEST['orgid']][$col]['codigo'] == "docenteprevisto") {
                        $html .= "
                            <td>" . number_format($registro['docenteprevano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenteprevano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenteprevano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenteprevano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenteprevano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenteprevano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenteprevano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenteprevano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenteprevano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['docenteprevano2005'], 0, ',', '.') . ";" . number_format($registro['docenteprevano2006'], 0, ',', '.') . ";" . number_format($registro['docenteprevano2007'], 0, ',', '.') . ";" . number_format($registro['docenteprevano2008'], 0, ',', '.') . ";" . number_format($registro['docenteprevano2009'], 0, ',', '.') . ";" . number_format($registro['docenteprevano2010'], 0, ',', '.') . ";" . number_format($registro['docenteprevano2011'], 0, ',', '.') . ";" . number_format($registro['docenteprevano2012'], 0, ',', '.') . ";" . number_format($registro['docenteprevano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "tecnicoprevisto") {
                        $html .= "
                            <td>" . number_format($registro['tecnicoprevano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicoprevano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicoprevano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicoprevano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicoprevano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicoprevano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicoprevano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicoprevano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicoprevano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['tecnicoprevano2005'], 0, ',', '.') . ";" . number_format($registro['tecnicoprevano2006'], 0, ',', '.') . ";" . number_format($registro['tecnicoprevano2007'], 0, ',', '.') . ";" . number_format($registro['tecnicoprevano2008'], 0, ',', '.') . ";" . number_format($registro['tecnicoprevano2009'], 0, ',', '.') . ";" . number_format($registro['tecnicoprevano2010'], 0, ',', '.') . ";" . number_format($registro['tecnicoprevano2011'], 0, ',', '.') . ";" . number_format($registro['tecnicoprevano2012'], 0, ',', '.') . ";" . number_format($registro['tecnicoprevano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "matprevisto") {
                        $html .= "
                            <td>" . number_format($registro['matprevano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matprevano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matprevano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matprevano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matprevano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matprevano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matprevano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matprevano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matprevano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['matprevano2005'], 0, ',', '.') . ";" . number_format($registro['matprevano2006'], 0, ',', '.') . ";" . number_format($registro['matprevano2007'], 0, ',', '.') . ";" . number_format($registro['matprevano2008'], 0, ',', '.') . ";" . number_format($registro['matprevano2009'], 0, ',', '.') . ";" . number_format($registro['matprevano2010'], 0, ',', '.') . ";" . number_format($registro['matprevano2011'], 0, ',', '.') . ";" . number_format($registro['matprevano2012'], 0, ',', '.') . ";" . number_format($registro['matprevano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "vagasprevisto") {

                        $html .= "
                            <td>" . number_format($registro['vagasprevano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasprevano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasprevano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasprevano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasprevano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasprevano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasprevano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasprevano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasprevano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['vagasprevano2005'], 0, ',', '.') . ";" . number_format($registro['vagasprevano2006'], 0, ',', '.') . ";" . number_format($registro['vagasprevano2007'], 0, ',', '.') . ";" . number_format($registro['vagasprevano2008'], 0, ',', '.') . ";" . number_format($registro['vagasprevano2009'], 0, ',', '.') . ";" . number_format($registro['vagasprevano2010'], 0, ',', '.') . ";" . number_format($registro['vagasprevano2011'], 0, ',', '.') . ";" . number_format($registro['vagasprevano2012'], 0, ',', '.') . ";" . number_format($registro['vagasprevano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "cursoprevisto") {
                        $html .= "
                            <td>" . number_format($registro['cursoprevano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursoprevano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursoprevano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursoprevano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursoprevano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursoprevano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursoprevano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursoprevano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursoprevano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['cursoprevano2005'], 0, ',', '.') . ";" . number_format($registro['cursoprevano2006'], 0, ',', '.') . ";" . number_format($registro['cursoprevano2007'], 0, ',', '.') . ";" . number_format($registro['cursoprevano2008'], 0, ',', '.') . ";" . number_format($registro['cursoprevano2009'], 0, ',', '.') . ";" . number_format($registro['cursoprevano2010'], 0, ',', '.') . ";" . number_format($registro['cursoprevano2011'], 0, ',', '.') . ";" . number_format($registro['cursoprevano2012'], 0, ',', '.') . ";" . number_format($registro['cursoprevano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "investimentoprevisto") {
                        $html .= "
                            <td>" . number_format($registro['investimentoprevano2005'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentoprevano2006'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentoprevano2007'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentoprevano2008'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentoprevano2009'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentoprevano2010'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentoprevano2011'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentoprevano2012'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentoprevano2013'], 2, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2005'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2006'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2007'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2008'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2009'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2010'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2011'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2012'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2013'], 2, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['investimentoprevano2005'], 2, ',', '.') . ";" . number_format($registro['investimentoprevano2006'], 2, ',', '.') . ";" . number_format($registro['investimentoprevano2007'], 2, ',', '.') . ";" . number_format($registro['investimentoprevano2008'], 2, ',', '.') . ";" . number_format($registro['investimentoprevano2009'], 2, ',', '.') . ";" . number_format($registro['investimentoprevano2010'], 2, ',', '.') . ";" . number_format($registro['investimentoprevano2011'], 2, ',', '.') . ";" . number_format($registro['investimentoprevano2012'], 2, ',', '.') . ";" . number_format($registro['investimentoprevano2013'], 2, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasmestradoprevisto") {
                        $html .= "
                            <td>" . number_format($registro['bolsasmestradoprevano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradoprevano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradoprevano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradoprevano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradoprevano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradoprevano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradoprevano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradoprevano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradoprevano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['bolsasmestradoprevano2005'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradoprevano2006'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradoprevano2007'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradoprevano2008'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradoprevano2009'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradoprevano2010'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradoprevano2011'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradoprevano2012'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradoprevano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasdoutoradoprevisto") {
                        $html .= "
                            <td>" . number_format($registro['bolsasdoutoradoprevano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradoprevano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradoprevano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradoprevano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradoprevano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradoprevano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradoprevano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradoprevano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradoprevano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['bolsasdoutoradoprevano2005'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradoprevano2006'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradoprevano2007'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradoprevano2008'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradoprevano2009'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradoprevano2010'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradoprevano2011'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradoprevano2012'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradoprevano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasposdoutoradoprevisto") {
                        $html .= "
                            <td>" . number_format($registro['bolsasposdoutoradoprevano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradoprevano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradoprevano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradoprevano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradoprevano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradoprevano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradoprevano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradoprevano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradoprevano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['bolsasposdoutoradoprevano2005'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradoprevano2006'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradoprevano2007'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradoprevano2008'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradoprevano2009'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradoprevano2010'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradoprevano2011'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradoprevano2012'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradoprevano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "docenterealizado") {
                        $html .= "
                            <td>" . number_format($registro['docenterealizano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenterealizano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenterealizano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenterealizano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenterealizano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenterealizano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenterealizano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenterealizano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['docenterealizano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['docenterealizano2005'], 0, ',', '.') . ";" . number_format($registro['docenterealizano2006'], 0, ',', '.') . ";" . number_format($registro['docenterealizano2007'], 0, ',', '.') . ";" . number_format($registro['docenterealizano2008'], 0, ',', '.') . ";" . number_format($registro['docenterealizano2009'], 0, ',', '.') . ";" . number_format($registro['docenterealizano2010'], 0, ',', '.') . ";" . number_format($registro['docenterealizano2011'], 0, ',', '.') . ";" . number_format($registro['docenterealizano2012'], 0, ',', '.') . ";" . number_format($registro['docenterealizano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "tecnicorealizado") {
                        $html .= "
                            <td>" . number_format($registro['tecnicorealizano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicorealizano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicorealizano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicorealizano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicorealizano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicorealizano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicorealizano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicorealizano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['tecnicorealizano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['tecnicorealizano2005'], 0, ',', '.') . ";" . number_format($registro['tecnicorealizano2006'], 0, ',', '.') . ";" . number_format($registro['tecnicorealizano2007'], 0, ',', '.') . ";" . number_format($registro['tecnicorealizano2008'], 0, ',', '.') . ";" . number_format($registro['tecnicorealizano2009'], 0, ',', '.') . ";" . number_format($registro['tecnicorealizano2010'], 0, ',', '.') . ";" . number_format($registro['tecnicorealizano2011'], 0, ',', '.') . ";" . number_format($registro['tecnicorealizano2012'], 0, ',', '.') . ";" . number_format($registro['tecnicorealizano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "matrealizado") {
                        $html .= "
                            <td>" . number_format($registro['matrealizano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matrealizano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matrealizano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matrealizano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matrealizano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matrealizano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matrealizano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matrealizano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matrealizano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['matrealizano2005'], 0, ',', '.') . ";" . number_format($registro['matrealizano2006'], 0, ',', '.') . ";" . number_format($registro['matrealizano2007'], 0, ',', '.') . ";" . number_format($registro['matrealizano2008'], 0, ',', '.') . ";" . number_format($registro['matrealizano2009'], 0, ',', '.') . ";" . number_format($registro['matrealizano2010'], 0, ',', '.') . ";" . number_format($registro['matrealizano2011'], 0, ',', '.') . ";" . number_format($registro['matrealizano2012'], 0, ',', '.') . ";" . number_format($registro['matrealizano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "vagasrealizado") {
                        $html .= "
                            <td>" . number_format($registro['vagasrealizano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasrealizano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasrealizano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasrealizano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasrealizano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasrealizano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasrealizano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasrealizano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['vagasrealizano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['vagasrealizano2005'], 0, ',', '.') . ";" . number_format($registro['vagasrealizano2006'], 0, ',', '.') . ";" . number_format($registro['vagasrealizano2007'], 0, ',', '.') . ";" . number_format($registro['vagasrealizano2008'], 0, ',', '.') . ";" . number_format($registro['vagasrealizano2009'], 0, ',', '.') . ";" . number_format($registro['vagasrealizano2010'], 0, ',', '.') . ";" . number_format($registro['vagasrealizano2011'], 0, ',', '.') . ";" . number_format($registro['vagasrealizano2012'], 0, ',', '.') . ";" . number_format($registro['vagasrealizano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "cursorealizado") {
                        $html .= "
                            <td>" . number_format($registro['cursorealizano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursorealizano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursorealizano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursorealizano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursorealizano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursorealizano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursorealizano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursorealizano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['cursorealizano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['cursorealizano2005'], 0, ',', '.') . ";" . number_format($registro['cursorealizano2006'], 0, ',', '.') . ";" . number_format($registro['cursorealizano2007'], 0, ',', '.') . ";" . number_format($registro['cursorealizano2008'], 0, ',', '.') . ";" . number_format($registro['cursorealizano2009'], 0, ',', '.') . ";" . number_format($registro['cursorealizano2010'], 0, ',', '.') . ";" . number_format($registro['cursorealizano2011'], 0, ',', '.') . ";" . number_format($registro['cursorealizano2012'], 0, ',', '.') . ";" . number_format($registro['cursorealizano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "investimentorealizado") {
                        $html .= "
                            <td>" . number_format($registro['investimentorealizano2005'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentorealizano2006'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentorealizano2007'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentorealizano2008'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentorealizano2009'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentorealizano2010'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentorealizano2011'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentorealizano2012'], 2, ',', '.') . "</td>
                            <td>" . number_format($registro['investimentorealizano2013'], 2, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2005'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2006'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2007'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2008'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2009'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2010'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2011'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2012'], 2, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2013'], 2, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['investimentorealizano2005'], 2, ',', '.') . ";" . number_format($registro['investimentorealizano2006'], 2, ',', '.') . ";" . number_format($registro['investimentorealizano2007'], 2, ',', '.') . ";" . number_format($registro['investimentorealizano2008'], 2, ',', '.') . ";" . number_format($registro['investimentorealizano2009'], 2, ',', '.') . ";" . number_format($registro['investimentorealizano2010'], 2, ',', '.') . ";" . number_format($registro['investimentorealizano2011'], 2, ',', '.') . ";" . number_format($registro['investimentorealizano2012'], 2, ',', '.') . ";" . number_format($registro['investimentorealizano2013'], 2, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasmestradorealizado") {
                        $html .= "
                            <td>" . number_format($registro['bolsasmestradorealizano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradorealizano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradorealizano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradorealizano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradorealizano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradorealizano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradorealizano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradorealizano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasmestradorealizano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['bolsasmestradorealizano2005'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradorealizano2006'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradorealizano2007'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradorealizano2008'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradorealizano2009'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradorealizano2010'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradorealizano2011'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradorealizano2012'], 0, ',', '.') . ";" . number_format($registro['bolsasmestradorealizano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasdoutoradorealizado") {
                        $html .= "
                            <td>" . number_format($registro['bolsasdoutoradorealizano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradorealizano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradorealizano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradorealizano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradorealizano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradorealizano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradorealizano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradorealizano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasdoutoradorealizano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['bolsasdoutoradorealizano2005'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradorealizano2006'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradorealizano2007'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradorealizano2008'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradorealizano2009'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradorealizano2010'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradorealizano2011'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradorealizano2012'], 0, ',', '.') . ";" . number_format($registro['bolsasdoutoradorealizano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasposdoutoradorealizado") {
                        $html .= "
                            <td>" . number_format($registro['bolsasposdoutoradorealizano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradorealizano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradorealizano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradorealizano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradorealizano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradorealizano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradorealizano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradorealizano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['bolsasposdoutoradorealizano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['bolsasposdoutoradorealizano2005'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradorealizano2006'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradorealizano2007'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradorealizano2008'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradorealizano2009'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradorealizano2010'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradorealizano2011'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradorealizano2012'], 0, ',', '.') . ";" . number_format($registro['bolsasposdoutoradorealizano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "matriculasgradpresen") {
                        $html .= "
                            <td>" . number_format($registro['matriculasgradpresenano2005'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matriculasgradpresenano2006'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matriculasgradpresenano2007'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matriculasgradpresenano2008'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matriculasgradpresenano2009'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matriculasgradpresenano2010'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matriculasgradpresenano2011'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matriculasgradpresenano2012'], 0, ',', '.') . "</td>
                            <td>" . number_format($registro['matriculasgradpresenano2013'], 0, ',', '.') . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2005'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2006'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2007'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2008'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2009'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2010'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2011'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2012'], 0, ',', '.'));
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2013'], 0, ',', '.'));
                        $cc++;

                        $csv .= number_format($registro['matriculasgradpresenano2005'], 0, ',', '.') . ";" . number_format($registro['matriculasgradpresenano2006'], 0, ',', '.') . ";" . number_format($registro['matriculasgradpresenano2007'], 0, ',', '.') . ";" . number_format($registro['matriculasgradpresenano2008'], 0, ',', '.') . ";" . number_format($registro['matriculasgradpresenano2009'], 0, ',', '.') . ";" . number_format($registro['matriculasgradpresenano2010'], 0, ',', '.') . ";" . number_format($registro['matriculasgradpresenano2011'], 0, ',', '.') . ";" . number_format($registro['matriculasgradpresenano2012'], 0, ',', '.') . ";" . number_format($registro['matriculasgradpresenano2013'], 0, ',', '.') . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecocampus") {

                        $tam = strlen($registro['cepcampus']);
                        $cep = substr($registro['cepcampus'], 0, 5) . '-' . substr($registro['cepcampus'], 5, $tam);

                        $html .= "
                            <td>" . $cep . "</td>
                            <td>" . $registro['bairrocampus'] . "</td>
                            <td>" . $registro['enderecocampus'] . "</td>
                            <td>" . $registro['numerocampus'] . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, $cep);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['bairrocampus']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['enderecocampus']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['numerocampus']);
                        $cc++;

                        $csv .= $cep . ";" . $registro['bairrocampus'] . ";" . $registro['enderecocampus'] . ";" . $registro['numerocampus'] . (($cc == $colspan) ? "\n" : ";");
                    } elseif ($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecounidadeorc") {

                        $tam = strlen($registro['cepuniversidade']);
                        $cep = substr($registro['cepuniversidade'], 0, 5) . '-' . substr($registro['cepuniversidade'], 5, $tam);

                        $html .= "
                            <td>" . $cep . "</td>
                            <td>" . $registro['bairrouniversidade'] . "</td>
                            <td>" . $registro['enderecouniversidade'] . "</td>
                            <td>" . $registro['numerouniversidade'] . "</td>
                        ";

                        $xls->MontaConteudoString($i, $cc, $cep);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['bairrouniversidade']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['enderecouniversidade']);
                        $cc++;
                        $xls->MontaConteudoString($i, $cc, $registro['numerouniversidade']);
                        $cc++;

                        $csv .= $cep . ";" . $registro['bairrouniversidade'] . ";" . $registro['enderecouniversidade'] . ";" . $registro['numerouniversidade'] . (($cc == $colspan) ? "\n" : ";");
                    } else {
                        $html .= "<td>" . $registro[$col] . "</td>";

                        $xls->MontaConteudoString($i, $cc, $registro[$col]);
                        $cc++;

                        $csv .= $registro[$col] . (($cc == $colspan) ? "\n" : ";");
                    }
                }
                $html .= "</tr>";
            }
            $i++;
        }
        $html .= "</table>";

        $msg = $_POST['obs_consulta'];
        //Roda pé em HTML: Observação feita pelo usuário na mantagem do relatório.
        $html .= "
            <table class='tabela'>
                <tr bgcolor=\"#C0C0C0\">
                    <td colspan='27' align='center'>\n OBSERVAÇÃO: " . $msg . " </td>
                </tr>
            </table>
        ";

        //Roda pé em XLS: Observação feita pelo usuário na mantagem do relatório.
        $xls->MontaConteudoString($i + 1, 0, 'OBSERVAÇÃO:');
        $xls->MontaConteudoString($i + 1, 1, $msg);

        //Roda pé em CSV: Observação feita pelo usuário na mantagem do relatório.
        $csv .= "\n\n" . "OBSERVAÇÃO: " . ";" . $msg;
    } else {
        $html = "
            <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
            <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>
            <table class='tabela'><tr bgcolor=\"#C0C0C0\"><td colspan='27' align='center'>MINISTÉRIO DA EDUCAÇÃO<br>Responsável pela informação: Secretaria de Educação Profissional e Tecnológica (SETEC)<br><br>Expansão da Educação Profissional e Tecnológica</td></tr><tr><td style='color:#ff0000; text-align:center'>Não foram encontrados registros.</td></tr></table>
        ";
    }

    $nome_arquivo = "relatorio_nota_tecnica_" . date('d-m-Y_H_i');
    switch (substr($_POST['exibir'], 10, 3)) {
        case 'csv':
            header("Content-type: application/octet-stream");
            header("Content-Disposition: attachment; filename=\"$nome_arquivo\"");
            echo $csv;
            break;
        case 'xls':
            ob_clean();
            $xls->GeraArquivo();
            break;
        case 'htm':
            echo $html;
            break;
    }
    exit;
}

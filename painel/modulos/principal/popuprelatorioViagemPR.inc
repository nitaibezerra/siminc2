<?php
set_time_limit(0);
$exportarWord =isset($_REQUEST['exportarWord'])?$_REQUEST['exportarWord']:NULL;
if(!empty($exportarWord)){
	header("Content-type: application/vnd.ms-word");
	header("Content-type: application/force-download");
	header("Content-Disposition: attachment; filename=RelatorioViagemPR.doc");
	header("Pragma: no-cache");
}
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<style>
    @media print{
        .no-print, .no-print *{
            display: none !important;
        }
    }
</style>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script>
    function exportarWord(){
        window.location.href+='&exportarWord=1';
    }
</script>
<?php
$regcod = $_REQUEST['regcod'];
$estuf = $_REQUEST['estuf'];
$muncod = $_REQUEST['muncod'];

if ($_GET['ministro'] == 'true'){
    $ministro = "SIM";
}else{
    $ministro = "";
}

if($regcod){
	$where = "WHERE regcod = '{$regcod}' ";
}elseif($estuf){
	$where = "WHERE estuf = '{$estuf}' ";
}elseif($muncod){
	$where = "WHERE muncod = '{$muncod}' ";
}

if($ministro){
    //PERIODICIDADES DOS INDICADORES
    $sql = "SELECT tipo, valor
            FROM (
                SELECT seh.indid AS tipo, dpe.dpeanoref AS valor
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    WHERE seh.sehstatus <> 'I'
                    AND seh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                    AND seh.indid IN (771,2792,271,3106,2845,690,2367)
            UNION ALL
                SELECT tipo, SUM(valor1) || ' - ' || SUM(valor2) AS valor
                FROM (
                    SELECT seh.indid AS tipo, MIN(dpe.dpeanoref)::INTEGER AS valor1, 0 AS valor2
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid IN (1562,778,2787,2577,1865)
                    GROUP BY tipo
                    UNION ALL
                    SELECT seh.indid AS tipo, 0 AS valor1, MAX(dpe.dpeanoref)::INTEGER AS valor2
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid IN (1562,778,2787,2577,1865)
                    GROUP BY tipo
                ) AS FOO1
                GROUP BY tipo
            ) AS FOO2";
    $arrDados = $db->carregar($sql);
    if($arrDados){
        foreach($arrDados as $dado){
            $arrPeriodicidade[$dado['tipo']]['valor'] = $dado['valor'];
        }
    }
}

if($ministro){
    $union = "SELECT 1 AS ordem, 'Creches - MC' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                    AND seh.indid = 2511
                    AND dsh.tidid2 IN (6912) --MC
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 1 AS ordem, 'Creches - MI' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                    AND seh.indid = 2511
                    AND dsh.tidid2 IN (6913) --MI
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 1 AS ordem, 'Creches - ' || tid1.tiddsc || ' - MI' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                    AND seh.indid = 2511
                    AND dsh.tidid2 IN (6913) --MI
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 1 AS ordem, 'Creches - ' || tid1.tiddsc || ' - MC' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                    AND seh.indid = 2511
                    AND dsh.tidid2 IN (6912) --MC
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 1 AS ordem, 'Creches - Investimentos' AS tipo, SUM(dsh.dshvalor) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                    AND seh.indid = 2511
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 1 AS ordem, 'Creches - Municipios' AS tipo, COUNT(DISTINCT dsh.dshcodmunicipio) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                    AND seh.indid = 2511
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 2 AS ordem, 'BrasilPro - ' || tid2.tiddsc AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                    AND seh.indid = 771
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 2 AS ordem, 'BrasilPro' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                    AND seh.indid = 771
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 2 AS ordem, 'BrasilPro - municipios' AS tipo, COUNT(DISTINCT dsh.dshcodmunicipio) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                    AND seh.indid = 771
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 5 AS ordem, 'Fortalecimento do Ensino Médio' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 3106
                    AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 5 AS ordem, 'ENEM - TotalRaca' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 271
                    AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 5 AS ordem, 'ENEM - ' || tid1.tiddsc AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 271
                    AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 6 AS ordem, 'ônibus - ' || dpe.dpeanoref AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 1865
                    AND dsh.tidid2 = 4301 --Recursos FNDE
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 9 AS ordem, 'lanchas - ' || dpe.dpeanoref AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2826
                    AND dsh.tidid1 IN (7609,7606) --Recursos FNDE e Doação
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 10 AS ordem, 'bicicletas - ' || dpe.dpeanoref AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2888
                    AND dsh.tidid1 = 7873 --Recursos FNDE
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 3 AS ordem, 'Creches - 2015 - Aprovadas' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2367
                    AND seh.dpeid = (SELECT MAX(dpe.dpeid) FROM painel.seriehistorica s INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = s.dpeid where s.indid = seh.indid AND s.sehstatus <> 'I' AND dpe.dpeanoref = '2015')
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 3 AS ordem, 'Creches - 2014 - Aprovadas' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2367
                    AND seh.dpeid = (SELECT MAX(dpe.dpeid) FROM painel.seriehistorica s INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = s.dpeid where s.indid = seh.indid AND s.sehstatus <> 'I' AND dpe.dpeanoref = '2014')
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 3 AS ordem, 'Creches - 2015 - ' || tid1.tiddsc AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2367
                    AND seh.dpeid = (SELECT MAX(dpe.dpeid) FROM painel.seriehistorica s INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = s.dpeid where s.indid = seh.indid AND s.sehstatus <> 'I' AND dpe.dpeanoref = '2015')
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 3 AS ordem, 'Creches - 2014 - ' || tid1.tiddsc AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2367
                    AND seh.dpeid = (SELECT MAX(dpe.dpeid) FROM painel.seriehistorica s INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = s.dpeid where s.indid = seh.indid AND s.sehstatus <> 'I' AND dpe.dpeanoref = '2014')
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 4 AS ordem, 'Tempo Integral - 2015' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 690
                    AND seh.dpeid = (SELECT MAX(dpe.dpeid) FROM painel.seriehistorica s INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = s.dpeid where s.indid = seh.indid AND s.sehstatus <> 'I' AND dpe.dpeanoref = '2015')
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 5 AS ordem, 'Alfabetização na idade certa - 2015' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2845
                    AND seh.dpeid = (SELECT MAX(dpe.dpeid) FROM painel.seriehistorica s INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = s.dpeid where s.indid = seh.indid AND s.sehstatus <> 'I' AND dpe.dpeanoref = '2015')
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 26 AS ordem, 'CIENCIA - 2015' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 1562
                    AND dpe.dpeanoref = '2015'
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 38 AS ordem, 'Mais Médicos - vagas - 2015' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid IN (2678, 2823, 2889)
                    AND dpe.dpeanoref = '2015'
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 39 AS ordem, 'Mais Médicos - publica - 2015' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2678
                    AND dpe.dpeanoref = '2015'
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 39 AS ordem, 'Mais Médicos - publica - existentes - 2015' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2678
                    AND dsh.tidid1 = 7263
                    AND dpe.dpeanoref = '2015'
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 39 AS ordem, 'Mais Médicos - publica - novos - 2015' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2678
                    AND dsh.tidid1 = 7262
                    AND dpe.dpeanoref = '2015'
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 40 AS ordem, 'Mais Médicos - privada - 2015' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2823
                    AND dpe.dpeanoref = '2015'
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 40 AS ordem, 'Mais Médicos - privada - existentes - 2015' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2823
                    AND dsh.tidid1 = 7588
                    AND dpe.dpeanoref = '2015'
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 40 AS ordem, 'Mais Médicos - privada - novos - 2015' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2823
                    AND dsh.tidid1 = 7589
                    AND dpe.dpeanoref = '2015'
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 41 AS ordem, 'Mais Médicos - privada - reativada - 2015' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2889
                    AND dpe.dpeanoref = '2015'
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 11 AS ordem, 'Pronatec - ' || tid2.tiddsc || ' - 2015' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2577
                    AND dpe.dpeanoref = '2015'
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 16 AS ordem, 'PROUNI - 2015' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2787
                    AND dpe.dpeanoref = '2015'
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 21 AS ordem, 'FIES - 2015' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 778
                    AND dpe.dpeanoref = '2015'
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 21 AS ordem, 'Institutos - 2015' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2575
                    AND dpe.dpeanoref = '2015'
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 21 AS ordem, 'FormacaoProf - 1513' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 1513
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 21 AS ordem, 'FormacaoProf - 2963' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 2963
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 21 AS ordem, 'FormacaoProf - 3198' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 3198
                    AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    SELECT 21 AS ordem, 'FormacaoProf - 1510' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                    FROM painel.seriehistorica seh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                    WHERE seh.sehstatus <> 'I'
                    AND seh.indid = 1510
                    AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                    GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                UNION ALL
                    ";
}

$sql = "SELECT tipo, SUM(total) AS valor
		FROM (
		$union
			SELECT 1 AS ordem, 'Creches' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
			AND seh.indid = 2367
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 1 AS ordem, 'Creches - PAC' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
			AND seh.indid = 2367
			AND dsh.tidid2 IN (5200)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 1 AS ordem, 'Creches - PREPAC' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
			AND seh.indid = 2367
			AND dsh.tidid2 IN (5199)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 1 AS ordem, 'Creches - OUTROS' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
			AND seh.indid = 2367
			AND dsh.tidid2 NOT IN (5200, 5199)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 3 AS ordem, 'Creches - ' || tid1.tiddsc AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
			AND seh.indid = 2367
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 3 AS ordem, 'Creches - ' || tid1.tiddsc || ' - PAC' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
			AND seh.indid = 2367
			AND dsh.tidid2 IN (5200)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 3 AS ordem, 'Creches - ' || tid1.tiddsc || ' - PREPAC' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
			AND seh.indid = 2367
			AND dsh.tidid2 IN (5199)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 3 AS ordem, 'Creches - ' || tid1.tiddsc || ' - OUTROS' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
			AND seh.indid = 2367
			AND dsh.tidid2 NOT IN (5199, 5200)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 4 AS ordem, 'Tempo Integral' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
			--AND seh.dpeid = 1224 --(2013 DEFINIDO A PEDIDO LOYANE DIA 09/06/2014) (RETIRADO A PEDIDO DA MARINA DIA 21/08/2014)
			AND seh.indid = 690
			--AND dsh.tidid2 = 3258 --Adesão efetivada (FILTRO RETIRADO A PEDIDO DA LOYANE DIA 08/05/2014)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 4 AS ordem, 'Tempo Integral - ' || tid1.tiddsc AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
			AND seh.indid = 690
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 5 AS ordem, 'Alfabetização na idade certa' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2845
			AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 6 AS ordem, 'Caminho da Escola - ônibus' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 1865
			AND dsh.tidid2 = 4301 --Recursos FNDE
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 7 AS ordem, 'Caminho da Escola - urbano' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 1865
			AND dsh.tidid1 = 4300
			AND dsh.tidid2 = 4301 --Recursos FNDE
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 8 AS ordem, 'Caminho da Escola - rural' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 1865
			AND dsh.tidid1 = 4299
			AND dsh.tidid2 = 4301 --Recursos FNDE
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 9 AS ordem, 'Lanchas' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2826
			AND dsh.tidid1 IN (7609,7606) --Recursos FNDE e Doação
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 10 AS ordem, 'Bicicletas' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2888
			AND dsh.tidid1 = 7873 --Recursos FNDE
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 11 AS ordem, 'Pronatec' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2577
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 11 AS ordem, 'Pronatec - FIC' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2577
			AND dsh.tidid2 IN (7598)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 11 AS ordem, 'Pronatec - TEC' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2577
			AND dsh.tidid2 IN (7599)
            GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 12 AS ordem, 'Pronatec - Municípios' AS tipo, COUNT(DISTINCT dsh.dshcodmunicipio) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2577 --Alterado a pedido da Loyanne do 2401 para o 2577
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 13 AS ordem, 'Pronatec - Mulheres' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND ((seh.indid = 2761 AND dsh.tidid1 = 7447) OR (seh.indid = 2744 AND dsh.tidid1 = 7438))
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 13 AS ordem, 'Pronatec - TotalGenero' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid IN (2761, 2744)
			AND dsh.tidid1 NOT IN (7448, 7442) --Não Informado
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 14 AS ordem, 'Pronatec - Negros' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2784
			AND dsh.tidid2 IN (7499, 7500)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 14 AS ordem, 'Pronatec - TotalRaca' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2784
			AND dsh.tidid2 NOT IN (7501) --Não Informado
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 15 AS ordem, 'Pronatec - Jovens' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2576
			AND dsh.tidid2 IN (7007, 7008, 7009)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 15 AS ordem, 'Pronatec - TotalFaixa' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2576
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 16 AS ordem, 'PROUNI' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2787
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 16 AS ordem, 'PROUNI - 2011' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2787
			AND dpe.dpeanoref >= '2011'
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 17 AS ordem, 'PROUNI - Bolsas Ativas' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2892
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 18 AS ordem, 'PROUNI - Mulheres' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2894
			AND dsh.tidid1 IN (7822)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 18 AS ordem, 'PROUNI - TotalGenero' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2894
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 19 AS ordem, 'PROUNI - Negros' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2893
			AND dsh.tidid1 IN (7810, 7811)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 19 AS ordem, 'PROUNI - TotalRaca' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2893
			AND dsh.tidid1 NOT IN (7812) --Não Informado
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 20 AS ordem, 'PROUNI - Jovens' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2787
			AND dsh.tidid2 IN (7825, 7826, 7827)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 20 AS ordem, 'PROUNI - TotalFaixa' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2787
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 21 AS ordem, 'FIES' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 778
			AND dpe.dpeanoref >= '2011'
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 22 AS ordem, 'FIES - estoque' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 778
			AND dpe.dpeanoref >= '2010'
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 23 AS ordem, 'FIES - Mulheres' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2818
			AND dsh.tidid1 IN (7591)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 23 AS ordem, 'FIES - TotalGenero' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2818
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 24 AS ordem, 'FIES - Negros' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 778
			AND dpe.dpeanoref >= '2011'
			AND dsh.tidid2 IN (3026, 3027)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 24 AS ordem, 'FIES - TotalRaca' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 778
			AND dpe.dpeanoref >= '2011'
			AND dsh.tidid2 NOT IN (3028) --Não Informado
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 25 AS ordem, 'FIES - Jovens' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2818
			AND dsh.tidid2 IN (7592, 7593, 7594)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 25 AS ordem, 'FIES - TotalFaixa' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2818
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 26 AS ordem, 'CIENCIA' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 1562
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 27 AS ordem, 'CIENCIA - Mulheres' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 1562
			AND dsh.tidid1 = 3764
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 28 AS ordem, 'CIENCIA - TotalGenero' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 1562
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 29 AS ordem, 'CIENCIA - Negros' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
            FROM painel.seriehistorica seh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
            INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
            WHERE seh.sehstatus <> 'I'
            AND seh.indid = 2886
            AND dsh.tidid2 IN (7784, 7783)
            GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 30 AS ordem, 'CIENCIA - TotalRaca' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
            FROM painel.seriehistorica seh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
            INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
            WHERE seh.sehstatus <> 'I'
            AND seh.indid = 2886
            AND dsh.tidid2 NOT IN (7781) --Não Informado
            GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 31 AS ordem, 'CIENCIA - Jovens' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
            FROM painel.seriehistorica seh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
            INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
            WHERE seh.sehstatus <> 'I'
            AND seh.indid = 2887
            AND dsh.tidid2 IN (7791, 7792, 7793)
            GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 32 AS ordem, 'CIENCIA - TotalFaixa' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
            FROM painel.seriehistorica seh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
            INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
            WHERE seh.sehstatus <> 'I'
            AND seh.indid = 2887
            GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 33 AS ordem, 'Mais Médicos' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
			AND seh.indid = 2792
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 34 AS ordem, 'Mais Médicos - municipios' AS tipo, COUNT(DISTINCT dsh.dshcodmunicipio) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
			AND seh.indid = 2792
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
            SELECT 35 AS ordem, 'Mais Médicos - instituições' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
            FROM painel.seriehistorica seh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
            INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
            WHERE seh.sehstatus <> 'I'
            AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
            AND seh.indid = 2291
            GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 36 AS ordem, 'Mais Médicos - tutores' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
			AND seh.indid = 2292
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 37 AS ordem, 'Mais Médicos - supervisores' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
			AND seh.indid = 2293
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 38 AS ordem, 'Mais Médicos - vagas' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid IN (2678, 2823, 2889)
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 39 AS ordem, 'Mais Médicos - publica' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2678
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 39 AS ordem, 'Mais Médicos - publica - existentes' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2678
			AND dsh.tidid1 = 7263
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 39 AS ordem, 'Mais Médicos - publica - novos' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2678
			AND dsh.tidid1 = 7262
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 40 AS ordem, 'Mais Médicos - privada' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2823
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 40 AS ordem, 'Mais Médicos - privada - existentes' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2823
			AND dsh.tidid1 = 7588
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 40 AS ordem, 'Mais Médicos - privada - novos' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2823
			AND dsh.tidid1 = 7589
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
			SELECT 41 AS ordem, 'Mais Médicos - privada - reativada' AS tipo, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
			FROM painel.seriehistorica seh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
			WHERE seh.sehstatus <> 'I'
			AND seh.indid = 2889
			GROUP BY ordem, tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
		UNION ALL
            SELECT
                42 AS ordem,
                CASE
                    WHEN dsh.tidid1 IN (2005) THEN 'UniSuperior - Existentes até 2002'
                    WHEN dsh.tidid1 IN (2004) THEN 'UniSuperior - Expansão 2003-2010'
                    WHEN dsh.tidid1 IN (4560) THEN 'UniSuperior - Expansão 2011-2014'
                END AS tipo,
                SUM(dsh.dshqtde) AS total,
                dsh.dshuf AS estuf,
                dsh.dshcodmunicipio AS muncod,
                est.regcod
            FROM painel.seriehistorica seh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
            INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
            INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
            WHERE seh.sehstatus <> 'I'
            AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
            AND seh.indid = 577
            AND dsh.tidid1 IN (2005, 2004, 4560)
            GROUP BY tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
        UNION ALL
            SELECT
                42 AS ordem,
                CASE
                    WHEN dsh.tidid2 IN (1476) THEN 'CampSuperior - Existentes até 2002'
                    WHEN dsh.tidid2 IN (2258) THEN 'CampSuperior - Expansão 2003-2010'
                    WHEN dsh.tidid2 IN (2587, 2588) THEN 'CampSuperior - Expansão 2011-2014'
                END AS tipo,
                SUM(dsh.dshqtde) AS total,
                dsh.dshuf AS estuf,
                dsh.dshcodmunicipio AS muncod,
                est.regcod
            FROM painel.seriehistorica seh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
            WHERE seh.sehstatus <> 'I'
            AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
            AND seh.indid = 382
            AND dsh.tidid2 IN (1476, 2258, 2587, 2588)
            GROUP BY tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
        UNION ALL
            SELECT
                42 AS ordem,
                CASE
                    WHEN dsh.tidid2 IN (1476) THEN 'CampSuperior - Existentes até 2002 - Municipios'
                    WHEN dsh.tidid2 IN (2258) THEN 'CampSuperior - Expansão 2003-2010 - Municipios'
                    WHEN dsh.tidid2 IN (2587, 2588) THEN 'CampSuperior - Expansão 2011-2014 - Municipios'
                END AS tipo,
                COUNT(DISTINCT dsh.dshcodmunicipio) AS total,
                dsh.dshuf AS estuf,
                dsh.dshcodmunicipio AS muncod,
                est.regcod
            FROM painel.seriehistorica seh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
            WHERE seh.sehstatus <> 'I'
            AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
            AND seh.indid = 382
            AND dsh.tidid2 IN (1476, 2258, 2587, 2588)
            GROUP BY tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
        UNION ALL
            SELECT
                43 AS ordem,
                CASE
                    WHEN dsh.tidid1 IN (1736) THEN 'CampProfissional - Existentes até 2002'
                    WHEN dsh.tidid1 IN (2257) THEN 'CampProfissional - Expansão 2003-2010'
                    WHEN dsh.tidid1 IN (2596, 2597) THEN 'CampProfissional - Expansão 2011-2014'
                    WHEN dsh.tidid1 IN (7107) THEN 'CampProfissional - Novas expansões'
                END AS tipo,
                SUM(dsh.dshqtde) AS total,
                dsh.dshuf AS estuf,
                dsh.dshcodmunicipio AS muncod,
                est.regcod
            FROM painel.seriehistorica seh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
            INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
            WHERE seh.sehstatus <> 'I'
            AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
            AND seh.indid = 525
            AND dsh.tidid1 IN (1736, 2257, 2596, 2597, 7107)
            GROUP BY tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
        UNION ALL
            SELECT
                43 AS ordem,
                CASE
                    WHEN dsh.tidid1 IN (1736) THEN 'CampProfissional - Existentes até 2002 - Municipios'
                    WHEN dsh.tidid1 IN (2257) THEN 'CampProfissional - Expansão 2003-2010 - Municipios'
                    WHEN dsh.tidid1 IN (2596, 2597) THEN 'CampProfissional - Expansão 2011-2014 - Municipios'
                    WHEN dsh.tidid1 IN (7107) THEN 'CampProfissional - Novas expansões - Municipios'
                END AS tipo,
                COUNT(DISTINCT dsh.dshcodmunicipio) AS total,
                dsh.dshuf AS estuf,
                dsh.dshcodmunicipio AS muncod,
                est.regcod
            FROM painel.seriehistorica seh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
            INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
            WHERE seh.sehstatus <> 'I'
            AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
            AND seh.indid = 525
            AND dsh.tidid1 IN (1736, 2257, 2596, 2597, 7107)
            GROUP BY tipo, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
        ) AS foo
        $where
        GROUP BY ordem, tipo
        ORDER BY ordem";
//ver($sql,d);
$arrDados = $db->carregar($sql);
if($arrDados){
	foreach($arrDados as $dado){
		$arrTipo[$dado['tipo']]['valor'][] = $dado['valor'];
	}
}

if($regcod){
	$sql = "SELECT 'Região ' || regdescricao AS titulo FROM territorios.regiao WHERE regcod = '{$regcod}' ";
}elseif($estuf){
	if($estuf == 'AL' || $estuf == 'MG' || $estuf == 'PE' || $estuf == 'RO' || $estuf == 'RR' || $estuf == 'SC' || $estuf == 'SE' || $estuf == 'SP'){
		$preposicao = " de";
	}elseif($estuf == 'BA' || $estuf == 'PB'){
		$preposicao = " da";
	}else{
		$preposicao = " do";
	}
    if($ministro){
        $preposicao = ":";
    }
	$sql = "SELECT 'Estado{$preposicao} ' || estdescricao AS titulo FROM territorios.estado WHERE estuf = '{$estuf}' ";
}elseif($muncod){
    if($ministro){
        $preposicao = ":";
    }else{
        $preposicao = " de";
    }
	$sql = "SELECT 'Município{$preposicao} ' || mundescricao AS titulo FROM territorios.municipio WHERE muncod = '{$muncod}' ";
}else{
	$sql = "SELECT 'Nacional' AS titulo  ";
}
$titulo = $db->pegaUm($sql);
?>
<html>
	<body>
		<?php if(empty($exportarWord)): ?>
			<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen"  style="border-bottom: 1px solid;" >
				<tr bgcolor="#ffffff">
					<td>
						<img src="../imagens/brasao.gif" width="50" height="50" border="0">
					</td>
					<td height="20" nowrap>
						<?php echo NOME_SISTEMA; ?><br>
						Ministério da Educação / SE - Secretaria Executiva<br>
						DTI - Diretoria de Tecnologia da Informação<br>
					</td>
					<td height="20" align="right">
						Impresso por: <strong><?= $_SESSION['usunome']; ?></strong><br>
						Órgão: <?= $_SESSION['usuorgao']; ?><br>
						Hora da Impressão: <?= date("d/m/Y - H:i:s") ?>
					</td>
				</tr>
				<tr bgcolor="#ffffff">
					<td colspan="2">&nbsp;</td>
				</tr>
			</table>
			<div style="text-align: right;" class="no-print">
				<input type="button" onclick="window.print();" value="Imprimir" name="btnImprimir" class="botao">
				<input type="button" onclick="exportarWord();" value="Exportar para Word" name="btnExportarWord" class="botao">
			</div>
		<?php endif; ?>
		<p>
		<center><big><b><?=$titulo?><br>(<?=date('m/Y')?>)</b></big></center>
		</p>
        <?php
        if($ministro){
            $width=800;
            $height=200;
            if($regcod){
                $parametro = "";
            }elseif($estuf){
                $sql = "SELECT estcod FROM territorios.estado WHERE estuf = '{$estuf}' ";
                $estcod = $db->pegaUm($sql);
                $parametro = ".php?tipo=estado&cod={$estcod}";
            }elseif($muncod){
                $parametro = ".php?tipo=municipio&cod={$muncod}";
            }else{
                $parametro = ".php";
                $width=800;
                $height=750;
            }

            if($parametro){
            ?>
                >>> <b>IDEB</b><br>
                <iframe src="http://sistemasideb.inep.gov.br:7000/ContentManager2009<?=$parametro?>" width="<?=$width?>" height="<?=$height?>" scrolling="no" frameborder="0" align="center"></iframe>
            <?php
            }
        }
        ?>
        <?php if($ministro){ ?>
            <p><span title="Indicador: 2367 e 2511">
        <?php }else{?>
            <p><span title="Indicador: 2367">
        <?php }?>
		>>> <b>Creches<?php if($ministro){ echo ' ('.$arrPeriodicidade['2367']['valor'].')';}?></b> – <?=number_format((is_array($arrTipo['Creches']['valor'])?array_sum($arrTipo['Creches']['valor']):0),0,",",".")?> aprovadas (<?=number_format((is_array($arrTipo['Creches - PAC']['valor'])?array_sum($arrTipo['Creches - PAC']['valor']):0),0,",",".")?> PAC2 + <?=number_format((is_array($arrTipo['Creches - PREPAC']['valor'])?array_sum($arrTipo['Creches - PREPAC']['valor']):0),0,",",".")?> PRÉ-PAC + <?=number_format((is_array($arrTipo['Creches - OUTROS']['valor'])?array_sum($arrTipo['Creches - OUTROS']['valor']):0),0,",",".")?> outros <?=($ministro ? (' / '.number_format((is_array($arrTipo['Creches - MC']['valor'])?array_sum($arrTipo['Creches - MC']['valor']):0),0,",",".")).' Metodologia Convencional + '.number_format((is_array($arrTipo['Creches - MI']['valor'])?array_sum($arrTipo['Creches - MI']['valor']):0),0,",",".").' Metodologia Inovadora' : '')?>)<br>
		<ul>
			<li><?=number_format((is_array($arrTipo['Creches - Em obras']['valor'])?array_sum($arrTipo['Creches - Em obras']['valor']):0),0,",",".")?> em obras (<?=number_format((is_array($arrTipo['Creches - Em obras - PAC']['valor'])?array_sum($arrTipo['Creches - Em obras - PAC']['valor']):0),0,",",".")?> PAC2 + <?=number_format((is_array($arrTipo['Creches - Em obras - PREPAC']['valor'])?array_sum($arrTipo['Creches - Em obras - PREPAC']['valor']):0),0,",",".")?> PRÉ-PAC + <?=number_format((is_array($arrTipo['Creches - Em obras - OUTROS']['valor'])?array_sum($arrTipo['Creches - Em obras - OUTROS']['valor']):0),0,",",".")?> outros <?=($ministro ? (' / '.number_format((is_array($arrTipo['Creches - Em obras - MC']['valor'])?array_sum($arrTipo['Creches - Em obras - MC']['valor']):0),0,",",".")).' Metodologia Convencional + '.number_format((is_array($arrTipo['Creches - Em obras - MI']['valor'])?array_sum($arrTipo['Creches - Em obras - MI']['valor']):0),0,",",".").' Metodologia Inovadora' : '')?>)</li>
			<li><?=number_format((is_array($arrTipo['Creches - Concluída']['valor'])?array_sum($arrTipo['Creches - Concluída']['valor']):0),0,",",".")?> concluídas (<?=number_format((is_array($arrTipo['Creches - Concluída - PAC']['valor'])?array_sum($arrTipo['Creches - Concluída - PAC']['valor']):0),0,",",".")?> PAC2 + <?=number_format((is_array($arrTipo['Creches - Concluída - PREPAC']['valor'])?array_sum($arrTipo['Creches - Concluída - PREPAC']['valor']):0),0,",",".")?> PRÉ-PAC + <?=number_format((is_array($arrTipo['Creches - Concluída - OUTROS']['valor'])?array_sum($arrTipo['Creches - Concluída - OUTROS']['valor']):0),0,",",".")?> outros <?=($ministro ? (' / '.number_format((is_array($arrTipo['Creches - Concluída - MC']['valor'])?array_sum($arrTipo['Creches - Concluída - MC']['valor']):0),0,",",".")).' Metodologia Convencional + '.number_format((is_array($arrTipo['Creches - Concluída - MI']['valor'])?array_sum($arrTipo['Creches - Concluída - MI']['valor']):0),0,",",".").' Metodologia Inovadora' : '')?>)</li>
            <?php if($ministro){ ?>
                <li><?=number_format((is_array($arrTipo['Creches - Ação preparatória']['valor'])?array_sum($arrTipo['Creches - Ação preparatória']['valor']):0),0,",",".")?> ação preparatória (<?=number_format((is_array($arrTipo['Creches - Ação preparatória - PAC']['valor'])?array_sum($arrTipo['Creches - Ação preparatória - PAC']['valor']):0),0,",",".")?> PAC2 + <?=number_format((is_array($arrTipo['Creches - Ação preparatória - PREPAC']['valor'])?array_sum($arrTipo['Creches - Ação preparatória - PREPAC']['valor']):0),0,",",".")?> PRÉ-PAC + <?=number_format((is_array($arrTipo['Creches - Ação preparatória - OUTROS']['valor'])?array_sum($arrTipo['Creches - Ação preparatória - OUTROS']['valor']):0),0,",",".")?> outros / <?=number_format((is_array($arrTipo['Creches - Ação preparatória - MC']['valor'])?array_sum($arrTipo['Creches - Ação preparatória - MC']['valor']):0),0,",",".")?> Metodologia Convencional + <?=number_format((is_array($arrTipo['Creches - Ação preparatória - MI']['valor'])?array_sum($arrTipo['Creches - Ação preparatória - MI']['valor']):0),0,",",".")?> Metodologia Inovadora)</li>
                <li><?=number_format((is_array($arrTipo['Creches - Obra cancelada']['valor'])?array_sum($arrTipo['Creches - Obra cancelada']['valor']):0),0,",",".")?> canceladas (<?=number_format((is_array($arrTipo['Creches - Obra cancelada - PAC']['valor'])?array_sum($arrTipo['Creches - Obra cancelada - PAC']['valor']):0),0,",",".")?> PAC2 + <?=number_format((is_array($arrTipo['Creches - Obra cancelada - PREPAC']['valor'])?array_sum($arrTipo['Creches - Obra cancelada - PREPAC']['valor']):0),0,",",".")?> PRÉ-PAC + <?=number_format((is_array($arrTipo['Creches - Obra cancelada - OUTROS']['valor'])?array_sum($arrTipo['Creches - Obra cancelada - OUTROS']['valor']):0),0,",",".")?> outros  / <?=number_format((is_array($arrTipo['Creches - Obra cancelada - MC']['valor'])?array_sum($arrTipo['Creches - Obra cancelada - MC']['valor']):0),0,",",".")?> Metodologia Convencional + <?=number_format((is_array($arrTipo['Creches - Obra cancelada - MI']['valor'])?array_sum($arrTipo['Creches - Obra cancelada - MI']['valor']):0),0,",",".")?> Metodologia Inovadora)</li>
                <li><?=number_format((is_array($arrTipo['Creches - Paralisada']['valor'])?array_sum($arrTipo['Creches - Paralisada']['valor']):0),0,",",".")?> paralisadas (<?=number_format((is_array($arrTipo['Creches - Paralisada - PAC']['valor'])?array_sum($arrTipo['Creches - Paralisada - PAC']['valor']):0),0,",",".")?> PAC2 + <?=number_format((is_array($arrTipo['Creches - Paralisada - PREPAC']['valor'])?array_sum($arrTipo['Creches - Paralisada - PREPAC']['valor']):0),0,",",".")?> PRÉ-PAC + <?=number_format((is_array($arrTipo['Creches - Paralisada - OUTROS']['valor'])?array_sum($arrTipo['Creches - Paralisada - OUTROS']['valor']):0),0,",",".")?> outros / <?=number_format((is_array($arrTipo['Creches - Paralisada - MC']['valor'])?array_sum($arrTipo['Creches - Paralisada - MC']['valor']):0),0,",",".")?> Metodologia Convencional + <?=number_format((is_array($arrTipo['Creches - Paralisada - MI']['valor'])?array_sum($arrTipo['Creches - Paralisada - MI']['valor']):0),0,",",".")?> Metodologia Inovadora)</li>
                <li>Total de investimento aprovado: R$ <?=number_format((is_array($arrTipo['Creches - Investimentos']['valor'])?array_sum($arrTipo['Creches - Investimentos']['valor']):0),2,",",".")?></li>
                <li>Municípios atendidos: <?=number_format((is_array($arrTipo['Creches - Municipios']['valor'])?array_sum($arrTipo['Creches - Municipios']['valor']):0),0,",",".")?></li>
            <?php } else { ?>
                <li><?=number_format((is_array($arrTipo['Creches - Obra cancelada']['valor'])?array_sum($arrTipo['Creches - Obra cancelada']['valor']):0),0,",",".")?> canceladas (<?=number_format((is_array($arrTipo['Creches - Obra cancelada - PAC']['valor'])?array_sum($arrTipo['Creches - Obra cancelada - PAC']['valor']):0),0,",",".")?> PAC2 + <?=number_format((is_array($arrTipo['Creches - Obra cancelada - PREPAC']['valor'])?array_sum($arrTipo['Creches - Obra cancelada - PREPAC']['valor']):0),0,",",".")?> PRÉ-PAC + <?=number_format((is_array($arrTipo['Creches - Obra cancelada - OUTROS']['valor'])?array_sum($arrTipo['Creches - Obra cancelada - OUTROS']['valor']):0),0,",",".")?> outros <?=($ministro ? (' / '.number_format((is_array($arrTipo['Creches - Obra cancelada - MC']['valor'])?array_sum($arrTipo['Creches - Obra cancelada - MC']['valor']):0),0,",",".")).' Metodologia Convencional + '.number_format((is_array($arrTipo['Creches - Obra cancelada - MI']['valor'])?array_sum($arrTipo['Creches - Obra cancelada - MI']['valor']):0),0,",",".").' Metodologia Inovadora' : '')?>)</li>
            <?php
            }?>
		</ul>
		</p>
		<p><span title="Indicador: 690">
		>>> <b>Tempo Integral<?php if($ministro){ echo ' ('.$arrPeriodicidade['690']['valor'].')';}?></b> – <?=number_format((is_array($arrTipo['Tempo Integral']['valor'])?array_sum($arrTipo['Tempo Integral']['valor']):0),0,",",".")?> escolas (<?=number_format((is_array($arrTipo['Tempo Integral - Estadual']['valor'])?array_sum($arrTipo['Tempo Integral - Estadual']['valor']):0),0,",",".")?> escolas estaduais e <?=number_format((is_array($arrTipo['Tempo Integral - Municipal']['valor'])?array_sum($arrTipo['Tempo Integral - Municipal']['valor']):0),0,",",".")?> escolas municipais)
		</p>
		<p><span title="Indicador: 2845">
		>>> <b>Alfabetização na idade certa<?php if($ministro){ echo ' ('.$arrPeriodicidade['2845']['valor'].')';}?></b> – <?=number_format((is_array($arrTipo['Alfabetização na idade certa']['valor'])?array_sum($arrTipo['Alfabetização na idade certa']['valor']):0),0,",",".")?> professores alfabetizadores receberam formação
		</p>
        <?php if($ministro){ ?>
            <p><span title="Indicador: 3106">
            >>> <b>Fortalecimento do Ensino Médio<?php if($ministro){ echo ' ('.$arrPeriodicidade['3106']['valor'].')';}?></b> – <?=number_format((is_array($arrTipo['Fortalecimento do Ensino Médio']['valor'])?array_sum($arrTipo['Fortalecimento do Ensino Médio']['valor']):0),0,",",".")?> professores do ensino médio receberam formação
            </p>
            <p><span title="Indicador: 271">
            >>> <b>ENEM<?php if($ministro){ echo ' ('.$arrPeriodicidade['271']['valor'].')';}?></b> – <?=number_format((is_array($arrTipo['ENEM - TotalRaca']['valor'])?array_sum($arrTipo['ENEM - TotalRaca']['valor']):0),0,",",".")?> inscritos<br>
            <ul>
                <li>Amarela: <?=number_format((is_array($arrTipo['ENEM - Amarela']['valor'])?(array_sum($arrTipo['ENEM - Amarela']['valor'])/array_sum($arrTipo['ENEM - TotalRaca']['valor']))*100:0),2,",",".")?>%</li>
                <li>Branca: <?=number_format((is_array($arrTipo['ENEM - Branca']['valor'])?(array_sum($arrTipo['ENEM - Branca']['valor'])/array_sum($arrTipo['ENEM - TotalRaca']['valor']))*100:0),2,",",".")?>%</li>
                <li>Indígena: <?=number_format((is_array($arrTipo['ENEM - Indígena']['valor'])?(array_sum($arrTipo['ENEM - Indígena']['valor'])/array_sum($arrTipo['ENEM - TotalRaca']['valor']))*100:0),2,",",".")?>%</li>
                <li>Não Declarada: <?=number_format((is_array($arrTipo['ENEM - Não Declarada']['valor'])?(array_sum($arrTipo['ENEM - Não Declarada']['valor'])/array_sum($arrTipo['ENEM - TotalRaca']['valor']))*100:0),2,",",".")?>%</li>
                <li>Parda: <?=number_format((is_array($arrTipo['ENEM - Parda']['valor'])?(array_sum($arrTipo['ENEM - Parda']['valor'])/array_sum($arrTipo['ENEM - TotalRaca']['valor']))*100:0),2,",",".")?>%</li>
                <li>Preta: <?=number_format((is_array($arrTipo['ENEM - Preta']['valor'])?(array_sum($arrTipo['ENEM - Preta']['valor'])/array_sum($arrTipo['ENEM - TotalRaca']['valor']))*100:0),2,",",".")?>%</li>
            </ul>
            </p>
        <?php } ?>
		<p>
		>>> <b>Caminho da Escola<?php if($ministro){ echo ' ('.$arrPeriodicidade['1865']['valor'].')';}?></b><br>
		<ul>
            <span title="Indicador: 1865 (Recursos FNDE)">
			<li><b>Ônibus:</b> <?=number_format((is_array($arrTipo['Caminho da Escola - ônibus']['valor'])?array_sum($arrTipo['Caminho da Escola - ônibus']['valor']):0),0,",",".")?> ônibus adquiridos com recursos do FNDE</li>
			<ul>
				<li><b>Urbanos:</b> <?=number_format((is_array($arrTipo['Caminho da Escola - urbano']['valor'])?array_sum($arrTipo['Caminho da Escola - urbano']['valor']):0),0,",",".")?></li>
				<li><b>Rurais:</b> <?=number_format((is_array($arrTipo['Caminho da Escola - rural']['valor'])?array_sum($arrTipo['Caminho da Escola - rural']['valor']):0),0,",",".")?></li>
			</ul>
			<span title="Indicador: 2826 (Recursos FNDE)">
			<li><b>Lanchas:</b> <?=number_format((is_array($arrTipo['Lanchas']['valor'])?array_sum($arrTipo['Lanchas']['valor']):0),0,",",".")?> adquiridos com recursos do FNDE</li>
			<span title="Indicador: 2888 (Recursos FNDE)">
			<li><b>Bicicletas:</b> <?=number_format((is_array($arrTipo['Bicicletas']['valor'])?array_sum($arrTipo['Bicicletas']['valor']):0),0,",",".")?> adquiridos com recursos do FNDE</li>
            <?php if($ministro){ ?>
                <span title="Indicadores: 1865, 2826 e 2888 (Recursos FNDE)">
			    <li><b>Estoque desde 2011:</b> <?=number_format((is_array($arrTipo['ônibus - 2011']['valor'])?array_sum($arrTipo['ônibus - 2011']['valor']):0),0,",",".")?> ônibus, <?=number_format((is_array($arrTipo['lanchas - 2011']['valor'])?array_sum($arrTipo['lanchas - 2011']['valor']):0),0,",",".")?> lanchas e <?=number_format((is_array($arrTipo['bicicletas - 2011']['valor'])?array_sum($arrTipo['bicicletas - 2011']['valor']):0),0,",",".")?> bicicletas, adquiridos com recursos do FNDE</li>
            <?php } ?>
		</ul>
		</p>
		<p><span title="Indicador: 2577">
		>>> <b>Pronatec<?php if($ministro){ echo ' ('.$arrPeriodicidade['2577']['valor'].')';}?></b> – <?=number_format((is_array($arrTipo['Pronatec']['valor'])?array_sum($arrTipo['Pronatec']['valor']):0),0,",",".")?> matrículas (<?=number_format((is_array($arrTipo['Pronatec - FIC']['valor'])?array_sum($arrTipo['Pronatec - FIC']['valor']):0),0,",",".")?> em cursos de qualificação profissional e <?=number_format((is_array($arrTipo['Pronatec - TEC']['valor'])?array_sum($arrTipo['Pronatec - TEC']['valor']):0),0,",",".")?> em cursos técnicos)<br>
		<ul>
			<span title="Indicador: 2577">
			<li>Municípios: <?=number_format((is_array($arrTipo['Pronatec - Municípios']['valor'])?array_sum($arrTipo['Pronatec - Municípios']['valor']):0),0,",",".")?></li>
			<span title="Indicadores: 2761 e 2744">
			<li>Mulheres: <?=number_format((is_array($arrTipo['Pronatec - Mulheres']['valor'])?(array_sum($arrTipo['Pronatec - Mulheres']['valor'])/array_sum($arrTipo['Pronatec - TotalGenero']['valor']))*100:0),2,",",".")?>%</li>
			<span title="Indicador: 2784">
			<li>Negros: <?=number_format((is_array($arrTipo['Pronatec - Negros']['valor'])?(array_sum($arrTipo['Pronatec - Negros']['valor'])/array_sum($arrTipo['Pronatec - TotalRaca']['valor']))*100:0),2,",",".")?>%</li>
			<span title="Indicador: 2576">
			<li>Jovens: <?=number_format((is_array($arrTipo['Pronatec - Jovens']['valor'])?(array_sum($arrTipo['Pronatec - Jovens']['valor'])/array_sum($arrTipo['Pronatec - TotalFaixa']['valor']))*100:0),2,",",".")?>%</li>
		</ul>
		</p>
		<p><span title="Indicador: 2787">
		>>> <b>PROUNI<?php if($ministro){ echo ' ('.$arrPeriodicidade['2787']['valor'].')';}else{echo ' (desde 2005)';}?></b> – <?=number_format((is_array($arrTipo['PROUNI']['valor'])?array_sum($arrTipo['PROUNI']['valor']):0),0,",",".")?> bolsas ocupadas<br>
		<ul>
            <span title="Indicador: 2787">
			<li>Estoque desde 2011: <?=number_format((is_array($arrTipo['PROUNI - 2011']['valor'])?array_sum($arrTipo['PROUNI - 2011']['valor']):0),0,",",".")?></li>
			<span title="Indicador: 2892">
			<li>Bolsas ativas: <?=number_format((is_array($arrTipo['PROUNI - Bolsas Ativas']['valor'])?array_sum($arrTipo['PROUNI - Bolsas Ativas']['valor']):0),0,",",".")?></li>
			<span title="Indicador: 2894">
			<li>Mulheres: <?=number_format((is_array($arrTipo['PROUNI - Mulheres']['valor'])?(array_sum($arrTipo['PROUNI - Mulheres']['valor'])/array_sum($arrTipo['PROUNI - TotalGenero']['valor']))*100:0),2,",",".")?>%</li>
			<span title="Indicador: 2893">
			<li>Negros: <?=number_format((is_array($arrTipo['PROUNI - Negros']['valor'])?(array_sum($arrTipo['PROUNI - Negros']['valor'])/array_sum($arrTipo['PROUNI - TotalRaca']['valor']))*100:0),2,",",".")?>%</li>
            <span title="Indicador: 2787 (14 a 29 anos)">
			<li>Jovens: <?=number_format((is_array($arrTipo['PROUNI - Jovens']['valor'])?(array_sum($arrTipo['PROUNI - Jovens']['valor'])/array_sum($arrTipo['PROUNI - TotalFaixa']['valor']))*100:0),2,",",".")?>%</li>
		</ul>
		</p>
		<p><span title="Indicador: 778">
		>>> <b>FIES (desde 2011)</b> – <?=number_format((is_array($arrTipo['FIES']['valor'])?array_sum($arrTipo['FIES']['valor']):0),0,",",".")?> contratos<br>
		<ul>
            <span title="Indicador: 778">
			<li>Estoque desde 2010: <?=number_format((is_array($arrTipo['FIES - estoque']['valor'])?array_sum($arrTipo['FIES - estoque']['valor']):0),0,",",".")?></li>
            <span title="Indicador: 2818">
			<li>Mulheres: <?=number_format((is_array($arrTipo['FIES - Mulheres']['valor'])?(array_sum($arrTipo['FIES - Mulheres']['valor'])/array_sum($arrTipo['FIES - TotalGenero']['valor']))*100:0),2,",",".")?>%</li>
            <span title="Indicador: 778">
			<li>Negros: <?=number_format((is_array($arrTipo['FIES - Negros']['valor'])?(array_sum($arrTipo['FIES - Negros']['valor'])/array_sum($arrTipo['FIES - TotalRaca']['valor']))*100:0),2,",",".")?>%</li>
            <span title="Indicador: 2818">
			<li>Jovens: <?=number_format((is_array($arrTipo['FIES - Jovens']['valor'])?(array_sum($arrTipo['FIES - Jovens']['valor'])/array_sum($arrTipo['FIES - TotalFaixa']['valor']))*100:0),2,",",".")?>%</li>
		</ul>
		</p>
		<p><span title="Indicador: 1562">
		>>> <b>Ciência sem Fronteiras<?php if($ministro){ echo ' ('.$arrPeriodicidade['1562']['valor'].')';}?></b> – <?=number_format((is_array($arrTipo['CIENCIA']['valor'])?array_sum($arrTipo['CIENCIA']['valor']):0),0,",",".")?> bolsas concedidas<br>
		<ul>
            <span title="Indicador: 1562">
			<li>Mulheres: <?=number_format((is_array($arrTipo['CIENCIA - Mulheres']['valor'])?(array_sum($arrTipo['CIENCIA - Mulheres']['valor'])/array_sum($arrTipo['CIENCIA - TotalGenero']['valor']))*100:0),2,",",".")?>%</li>
            <span title="Indicador: 2886">
			<li>Negros: <?=number_format((is_array($arrTipo['CIENCIA - Negros']['valor'])?(array_sum($arrTipo['CIENCIA - Negros']['valor'])/array_sum($arrTipo['CIENCIA - TotalRaca']['valor']))*100:0),2,",",".")?>%</li>
            <span title="Indicador: 2887">
			<li>Jovens: <?=number_format((is_array($arrTipo['CIENCIA - Jovens']['valor'])?(array_sum($arrTipo['CIENCIA - Jovens']['valor'])/array_sum($arrTipo['CIENCIA - TotalFaixa']['valor']))*100:0),2,",",".")?>%</li>
		</ul>
		</p>
		<p><span title="Indicador: 2792">
		>>> <b>Mais Médicos<?php if($ministro){ echo ' ('.$arrPeriodicidade['2792']['valor'].')';}?></b> – <?=number_format((is_array($arrTipo['Mais Médicos']['valor'])?array_sum($arrTipo['Mais Médicos']['valor']):0),0,",",".")?> médicos em atuação<br>
		<ul>
            <?php
            if(!$muncod){
            ?>
			    <span title="Indicador: 2792">
			    <li>Municípios: <?=number_format((is_array($arrTipo['Mais Médicos - municipios']['valor'])?array_sum($arrTipo['Mais Médicos - municipios']['valor']):0),0,",",".")?></li>
            <?php
            }
            $sql = "SELECT DISTINCT UPPER(uni.uninome) AS instituicao
                    FROM maismedicos.universidade uni
                    inner join maismedicos.universidademunicipio unm  ON uni.uniid = unm.uniid
                    inner join maismedicos.medico mdc ON mdc.muncod = unm.muncod
                    inner join territorios.municipio mun ON mun.muncod = mdc.muncod
                    INNER JOIN territorios.estado est ON est.estuf = mun.estuf
                    WHERE unistatus = 'A'
                    ".($regcod ? "AND regcod = '{$regcod}'" : ($estuf ? "AND est.estuf = '{$estuf}'" : ($muncod ? "AND mdc.muncod = '{$muncod}'" : '')))."
                    ORDER BY instituicao";
            $arrDadosInstituicoes = $db->carregar($sql);
            if($arrDadosInstituicoes){
                $totalInstituicoes = count($arrDadosInstituicoes);
            }
            ?>
			<span title="Busca diretamente do sistema Mais Médicos. Retorna as instituições no território do médico">
			<li>Instituições Supervisoras: <?=number_format($totalInstituicoes,0,",",".")?></li>
            <?php
            if($arrDadosInstituicoes){
                echo "<ul>";
                foreach($arrDadosInstituicoes as $dado){
                    echo "<li>" . $dado['instituicao'] . "</li>";
                }
                echo "</ul>";
            }
            ?>
			<span title="Indicador: 2292">
			<li>Tutores: <?=number_format((is_array($arrTipo['Mais Médicos - tutores']['valor'])?array_sum($arrTipo['Mais Médicos - tutores']['valor']):0),0,",",".")?></li>
			<span title="Indicador: 2293">
			<li>Supervisores: <?=number_format((is_array($arrTipo['Mais Médicos - supervisores']['valor'])?array_sum($arrTipo['Mais Médicos - supervisores']['valor']):0),0,",",".")?></li>
			<span title="Indicadores: 2678, 2823 e 2889">
			<li>Novas vagas de graduação de medicina: <?=number_format((is_array($arrTipo['Mais Médicos - vagas']['valor'])?array_sum($arrTipo['Mais Médicos - vagas']['valor']):0),0,",",".")?> novas vagas</li>
			<ul>
				<span title="Indicador: 2678">
				<li>Rede pública: <?=number_format((is_array($arrTipo['Mais Médicos - publica']['valor'])?array_sum($arrTipo['Mais Médicos - publica']['valor']):0),0,",",".")?> vagas criadas (<?=number_format((is_array($arrTipo['Mais Médicos - publica - existentes']['valor'])?array_sum($arrTipo['Mais Médicos - publica - existentes']['valor']):0),0,",",".")?> vagas em cursos existentes e <?=number_format((is_array($arrTipo['Mais Médicos - publica - novos']['valor'])?array_sum($arrTipo['Mais Médicos - publica - novos']['valor']):0),0,",",".")?> vagas em novos cursos)</li>
                    <?php if($ministro){
                        $sql = "SELECT ano, SUM(total) AS valor
                            FROM (
                                SELECT dpe.dpeanoref AS ano, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                                FROM painel.seriehistorica seh
                                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                                INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                                WHERE seh.sehstatus <> 'I'
                                AND seh.indid = 2678
                                GROUP BY ano, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                            ) AS FOO
                            $where
                            GROUP BY ano
                            ORDER BY ano";
                        $arrDados = $db->carregar($sql);
                        if($arrDados){
                            echo "<ul><li>No anos:</li><ul>";
                            foreach($arrDados as $dado){
                                echo "    <li>".$dado['ano'].": ".number_format($dado['valor'],0,",",".")." vagas criadas</li>";
                            }
                            echo "</ul></ul>";
                        }

                        if(!$muncod){
                            $sql = "SELECT estuf, municipio, SUM(total) AS valor
                                FROM (
                                    SELECT mun.mundescricao AS municipio, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                                    FROM painel.seriehistorica seh
                                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                                    INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio
                                    WHERE seh.sehstatus <> 'I'
                                    AND seh.indid = 2678
                                    GROUP BY municipio, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                                ) AS FOO
                                $where
                                GROUP BY estuf, municipio
                                ORDER BY estuf, municipio";
                            $arrDados = $db->carregar($sql);
                            if($arrDados){
                                echo "<ul><li>Nos municípios:</li><ul>";
                                foreach($arrDados as $dado){
                                    echo "    <li>".$dado['estuf'] . " - " . $dado['municipio'].": ".number_format($dado['valor'],0,",",".")." vagas criadas</li>";
                                }
                                echo "</ul></ul>";
                            }
                        }
                    } ?>
				<span title="Indicador: 2823">
				<li>Rede privada: <?=number_format((is_array($arrTipo['Mais Médicos - privada']['valor'])?array_sum($arrTipo['Mais Médicos - privada']['valor']):0),0,",",".")?> vagas criadas (<?=number_format((is_array($arrTipo['Mais Médicos - privada - existentes']['valor'])?array_sum($arrTipo['Mais Médicos - privada - existentes']['valor']):0),0,",",".")?> vagas em cursos existentes e <?=number_format((is_array($arrTipo['Mais Médicos - privada - novos']['valor'])?array_sum($arrTipo['Mais Médicos - privada - novos']['valor']):0),0,",",".")?> vagas em novos cursos)</li>
                <?php if($ministro){
                    $sql = "SELECT ano, SUM(total) AS valor
                            FROM (
                                SELECT dpe.dpeanoref AS ano, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                                FROM painel.seriehistorica seh
                                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                                INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                                WHERE seh.sehstatus <> 'I'
                                AND seh.indid = 2823
                                GROUP BY ano, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                            ) AS FOO
                            $where
                            GROUP BY ano
                            ORDER BY ano";
                    $arrDados = $db->carregar($sql);
                    if($arrDados){
                        echo "<ul><li>No anos:</li><ul>";
                        foreach($arrDados as $dado){
                            echo "    <li>".$dado['ano'].": ".number_format($dado['valor'],0,",",".")." vagas criadas</li>";
                        }
                        echo "</ul></ul>";
                    }

                    if(!$muncod){
                        $sql = "SELECT estuf, municipio, SUM(total) AS valor
                                FROM (
                                    SELECT mun.mundescricao AS municipio, SUM(dsh.dshqtde) AS total, dsh.dshuf AS estuf, dsh.dshcodmunicipio AS muncod, est.regcod
                                    FROM painel.seriehistorica seh
                                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = seh.dpeid
                                    INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                                    INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio
                                    WHERE seh.sehstatus <> 'I'
                                    AND seh.indid = 2823
                                    GROUP BY municipio, dsh.dshuf, dsh.dshcodmunicipio, est.regcod
                                ) AS FOO
                                $where
                                GROUP BY estuf, municipio
                                ORDER BY estuf, municipio";
                        $arrDados = $db->carregar($sql);
                        if($arrDados){
                            echo "<ul><li>Nos municípios:</li><ul>";
                            foreach($arrDados as $dado){
                                echo "    <li>".$dado['estuf'] . " - " . $dado['municipio'].": ".number_format($dado['valor'],0,",",".")." vagas criadas</li>";
                            }
                            echo "</ul></ul>";
                        }
                    }
                } ?>
                <span title="Indicador: 2889">
				<li>Rede privada: <?=number_format((is_array($arrTipo['Mais Médicos - privada - reativada']['valor'])?array_sum($arrTipo['Mais Médicos - privada - reativada']['valor']):0),0,",",".")?> vagas reativadas</li>
                <?php if($ministro){ ?>
                    <li>Previsão Nacional de expansão rede pública: 3.615 novas vagas até 2017</li>
                    <li>Previsão Nacional de expansão rede privada: 7.832 vagas até 2017</li>
                <?php } ?>
			</ul>
		</ul>
		</p>
        <?php if($ministro){ ?>
            <p><span title="Indicador: 771">
            >>> <b>Brasil Profissionalizado (<?=$arrPeriodicidade['771']['valor']?>)</b> – <?=number_format((is_array($arrTipo['BrasilPro']['valor'])?array_sum($arrTipo['BrasilPro']['valor']):0),0,",",".")?> obras (Reforma/Ampliação/Construção)<br>
            <ul>
                <li><?=number_format((is_array($arrTipo['BrasilPro - Em Planejamento pelo Proponente']['valor'])?array_sum($arrTipo['BrasilPro - Em Planejamento pelo Proponente']['valor']):0),0,",",".")?> Em Planejamento pelo Proponente</li>
                <li><?=number_format((is_array($arrTipo['BrasilPro - Em Licitação']['valor'])?array_sum($arrTipo['BrasilPro - Em Licitação']['valor']):0),0,",",".")?> Em licitacao</li>
                <li><?=number_format((is_array($arrTipo['BrasilPro - Contratação']['valor'])?array_sum($arrTipo['BrasilPro - Contratação']['valor']):0),0,",",".")?> Em contratação</li>
                <li><?=number_format((is_array($arrTipo['BrasilPro - Em Execução']['valor'])?array_sum($arrTipo['BrasilPro - Em Execução']['valor']):0),0,",",".")?> Em execução</li>
                <li><?=number_format((is_array($arrTipo['BrasilPro - Paralisada']['valor'])?array_sum($arrTipo['BrasilPro - Paralisada']['valor']):0),0,",",".")?> Paralisadas</li>
                <li><?=number_format((is_array($arrTipo['BrasilPro - Concluída']['valor'])?array_sum($arrTipo['BrasilPro - Concluída']['valor']):0),0,",",".")?> Concluídas</li>
                <li><?=number_format((is_array($arrTipo['BrasilPro - Obra Cancelada']['valor'])?array_sum($arrTipo['BrasilPro - Obra Cancelada']['valor']):0),0,",",".")?> Canceladas</li>
                <li><?=number_format((is_array($arrTipo['BrasilPro - Inacabada']['valor'])?array_sum($arrTipo['BrasilPro - Inacabada']['valor']):0),0,",",".")?> Inacabadas</li>
                <li>Municípios: <?=number_format((is_array($arrTipo['BrasilPro - municipios']['valor'])?array_sum($arrTipo['BrasilPro - municipios']['valor']):0),0,",",".")?></li>
            </ul>
            </p>
            <p>
            >>> <b>Formação de Professores</b><br>
            <ul>
                <span title="Indicador: 1513">
                <li><?=number_format((is_array($arrTipo['FormacaoProf - 1513']['valor'])?array_sum($arrTipo['FormacaoProf - 1513']['valor']):0),0,",",".")?> professores matriculados no Plano Nacional de Formação de Professores da Educação Básica (PARFOR)</li>
                <span title="Indicador: 2963">
                <li><?=number_format((is_array($arrTipo['FormacaoProf - 2963']['valor'])?array_sum($arrTipo['FormacaoProf - 2963']['valor']):0),0,",",".")?> bolsas do Programa Institucional de Bolsa de Iniciação à Docência (PIBID)</li>
                <span title="Indicador: 3198">
                <li><?=number_format((is_array($arrTipo['FormacaoProf - 3198']['valor'])?array_sum($arrTipo['FormacaoProf - 3198']['valor']):0),0,",",".")?> matrículas na Universidade Aberta do Brasil (UAB)</li>
                <!--<span title="Indicador: 1510">
                <li><?=number_format((is_array($arrTipo['FormacaoProf - 1510']['valor'])?array_sum($arrTipo['FormacaoProf - 1510']['valor']):0),0,",",".")?> matrículas no Programa de Mestrado Profissional em Matemática (PROFMAT)</li>
                -->
            </ul>
            </p>
        <?php
        }
        ?>
        <p><span title="Indicadores: 557 e 382">
            >>> <b>Universidades Federais</b><br>
            <ul>
                <li>Existentes até 2002: <?=number_format((is_array($arrTipo['UniSuperior - Existentes até 2002']['valor'])?array_sum($arrTipo['UniSuperior - Existentes até 2002']['valor']):0),0,",",".")?> universidades e <?=number_format((is_array($arrTipo['CampSuperior - Existentes até 2002']['valor'])?array_sum($arrTipo['CampSuperior - Existentes até 2002']['valor']):0),0,",",".")?> câmpus em <?=number_format((is_array($arrTipo['CampSuperior - Existentes até 2002 - Municipios']['valor'])?array_sum($arrTipo['CampSuperior - Existentes até 2002 - Municipios']['valor']):0),0,",",".")?> municípios</li>
                <li>Expansão 2003-2010</li>
                <ul>
                    <li>Universidades criadas: <?=number_format((is_array($arrTipo['UniSuperior - Expansão 2003-2010']['valor'])?array_sum($arrTipo['UniSuperior - Expansão 2003-2010']['valor']):0),0,",",".")?></li>
                    <?php
                    $sql = "SELECT universidade, municipio, estuf
                            FROM (
                                SELECT
                                    entinst.entnome AS universidade,
                                    dsh.dshuf AS estuf,
                                    dsh.dshcodmunicipio AS muncod,
                                    est.regcod,
                                    mun.mundescricao AS municipio
                                FROM painel.seriehistorica seh
                                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                                INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                                INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio
                                INNER JOIN entidade.entidade entinst ON entinst.entunicod = dsh.unicod
                                INNER JOIN entidade.funcaoentidade fn2 ON entinst.entid = fn2.entid AND fn2.funid = 12 AND entinst.entstatus = 'A' AND fn2.fuestatus = 'A'
                                WHERE seh.sehstatus <> 'I'
                                AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                                AND seh.indid = 577
                                AND dsh.tidid1 IN (2004) --Expansão 2003-2010
                            ) AS FOO
                            $where
                            ORDER BY universidade";
                    $arrDados = $db->carregar($sql);
                    if($arrDados){
                        echo "<ul>";
                        foreach($arrDados as $dado){
                            echo "    <li>".$dado['universidade']." - ".$dado['municipio']."/".$dado['estuf']."</li>";
                        }
                        echo "</ul>";
                    }
                    ?>
                    <li>Câmpus criados: <?=number_format((is_array($arrTipo['CampSuperior - Expansão 2003-2010']['valor'])?array_sum($arrTipo['CampSuperior - Expansão 2003-2010']['valor']):0),0,",",".")?></li>
                    <?php
                    $sql = "SELECT universidade, campus
                            FROM (
                                SELECT
                                    entinst.entnome AS universidade,
                                    entcampus.entnome AS campus,
                                    dsh.dshuf AS estuf,
                                    dsh.dshcodmunicipio AS muncod,
                                    est.regcod
                                FROM painel.seriehistorica seh
                                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                                INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                                INNER JOIN entidade.entidade entcampus ON entcampus.entid = dsh.entid
                                INNER JOIN entidade.funcaoentidade fn ON entcampus.entid = fn.entid AND fn.funid = 18 AND entcampus.entstatus = 'A' AND fn.fuestatus = 'A'
                                INNER JOIN entidade.funentassoc a ON a.fueid = fn.fueid
                                INNER JOIN entidade.entidade entinst ON entinst.entid = a.entid
                                INNER JOIN entidade.funcaoentidade fn2 ON entinst.entid = fn2.entid AND fn2.funid = 12 AND entinst.entstatus = 'A' AND fn2.fuestatus = 'A'
                                WHERE seh.sehstatus <> 'I'
                                AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                                AND seh.indid = 382
                                AND dsh.tidid2 IN (2258) --Expansão 2003-2010
                            ) AS FOO
                            $where
                            ORDER BY universidade, campus";
                    $arrDados = $db->carregar($sql);
                    if($arrDados){
                        $contador = 0;
                        $campusUniversidade = "";
                        $arrUniversidade = array();
                        foreach($arrDados as $dado){
                            if($contador==0) $universidade = $dado['universidade'];
                            if($universidade==$dado['universidade']){
                                if($campusUniversidade){
                                    $campusUniversidade = $campusUniversidade.", ".$dado['campus'];
                                }else{
                                    $campusUniversidade = $dado['campus'];
                                }
                            }else{
                                $arrUniversidade[] = array(
                                    'universidade'=>$universidade,
                                    'campus'=>$campusUniversidade,
                                );
                                $campusUniversidade = $dado['campus'];
                            }
                            $universidade = $dado['universidade'];
                            $contador++;
                        }
                        $arrUniversidade[] = array(
                            'universidade'=>$universidade,
                            'campus'=>$campusUniversidade,
                        );
                        echo "<ul>";
                        foreach($arrUniversidade as $dado){
                            echo "    <li>".$dado['campus']." (".$dado['universidade'].")</li>";
                        }
                        echo "</ul>";
                    }
                    ?>
                </ul>
                <li>Expansão 2011-2014</li>
                <ul>
                    <li>Universidades criadas: <?=number_format((is_array($arrTipo['UniSuperior - Expansão 2011-2014']['valor'])?array_sum($arrTipo['UniSuperior - Expansão 2011-2014']['valor']):0),0,",",".")?></li>
                    <?php
                    $sql = "SELECT universidade, municipio, estuf
                            FROM (
                                SELECT
                                    entinst.entnome AS universidade,
                                    dsh.dshuf AS estuf,
                                    dsh.dshcodmunicipio AS muncod,
                                    est.regcod,
                                    mun.mundescricao AS municipio
                                FROM painel.seriehistorica seh
                                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                                INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                                INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio
                                INNER JOIN entidade.entidade entinst ON entinst.entunicod = dsh.unicod
                                INNER JOIN entidade.funcaoentidade fn2 ON entinst.entid = fn2.entid AND fn2.funid = 12 AND entinst.entstatus = 'A' AND fn2.fuestatus = 'A'
                                WHERE seh.sehstatus <> 'I'
                                AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                                AND seh.indid = 577
                                AND dsh.tidid1 IN (4560) --Expansão 2011-2014
                            ) AS FOO
                            $where
                            ORDER BY universidade";
                    $arrDados = $db->carregar($sql);
                    if($arrDados){
                        echo "<ul>";
                        foreach($arrDados as $dado){
                            echo "    <li>".$dado['universidade']." - ".$dado['municipio']."/".$dado['estuf']."</li>";
                        }
                        echo "</ul>";
                    }
                    ?>
                    <li>Novos câmpus: <?=number_format((is_array($arrTipo['CampSuperior - Expansão 2011-2014']['valor'])?array_sum($arrTipo['CampSuperior - Expansão 2011-2014']['valor']):0),0,",",".")?></li>
                    <?php
                    $sql = "SELECT universidade, campus, situacao, matriculas, vagas, 1 AS valor
                            FROM (
                                SELECT
                                entinst.entnome AS universidade,
                                entcampus.entnome AS campus,
                                dsh.dshuf AS estuf,
                                dsh.dshcodmunicipio AS muncod,
                                est.regcod,
                                CASE
                                    WHEN dsh.tidid1 IN (365) THEN 'Funcionando'
                                    WHEN dsh.tidid1 IN (366) THEN 'Em implantação'
                                END AS situacao,
                                matriculas,
                                vagas
                                FROM painel.seriehistorica seh
                                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                                INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                                INNER JOIN entidade.entidade entcampus ON entcampus.entid = dsh.entid
                                INNER JOIN entidade.funcaoentidade fn ON entcampus.entid = fn.entid AND fn.funid = 18 AND entcampus.entstatus = 'A' AND fn.fuestatus = 'A'
                                INNER JOIN entidade.funentassoc a ON a.fueid = fn.fueid
                                INNER JOIN entidade.entidade entinst ON entinst.entid = a.entid
                                INNER JOIN entidade.funcaoentidade fn2 ON entinst.entid = fn2.entid AND fn2.funid = 12 AND entinst.entstatus = 'A' AND fn2.fuestatus = 'A'
                                LEFT JOIN academico.campus cmp ON cmp.entid = entcampus.entid
                                --LEFT JOIN (SELECT cmpid, SUM(cpivalor) AS matriculas FROM academico.campusitem WHERE itmid IN (45) AND cpiano = '2012' AND cpitabnum = 1 GROUP BY cmpid ) AS cpi1 ON cpi1.cmpid = cmp.cmpid
                                LEFT JOIN (SELECT SUM(dshqtde) AS matriculas, entid  FROM painel.seriehistorica seh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid WHERE seh.sehstatus <> 'I' AND seh.dpeid = 1224 AND seh.indid = 3094 GROUP BY entid) AS cpi1 ON cpi1.entid = entcampus.entid
                                LEFT JOIN (SELECT cmpid, SUM(cpivalor) AS vagas FROM academico.campusitem WHERE itmid IN (28) AND cpiano = '2014' AND cpitabnum = 1 GROUP BY cmpid ) AS cpi2 ON cpi2.cmpid = cmp.cmpid
                                WHERE seh.sehstatus <> 'I'
                                AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                                AND seh.indid = 382
                                AND dsh.tidid2 IN (2587, 2588) --Expansão 2011-2014
                            ) AS FOO
                            $where
                            ORDER BY situacao DESC, universidade, campus";
                    $arrDados = $db->carregar($sql);
                    $arrSituacao = array();
                    if($arrDados){
                        foreach($arrDados as $dado){
                            $arrSituacao[$dado['situacao']]['valor'][] = $dado['valor'];
                        }
                        $situacao = "";
                        $labelSituacao = "";
                        echo "<ul>";
                        foreach($arrDados as $dado){
                            $labelSituacao = $dado['situacao'];
                            if($labelSituacao != $situacao){
                                echo "<li>".$labelSituacao.": ".($labelSituacao=='Funcionando' ? number_format((is_array($arrSituacao['Funcionando']['valor'])?array_sum($arrSituacao['Funcionando']['valor']):0),0,",",".") : number_format((is_array($arrSituacao['Em implantação']['valor'])?array_sum($arrSituacao['Em implantação']['valor']):0),0,",","."))."</li>";
                                $situacao = $labelSituacao;
                            }
                            echo "<ul>";
                            echo "    <li>".$dado['campus']." (".$dado['universidade'].") - ".($labelSituacao=='Funcionando' ? number_format($dado['matriculas'],0,",",".")." matrículas em 2013" : number_format($dado['vagas'],0,",",".")." vagas em 2014")."</li>";
                            echo "</ul>";
                        }
                        echo "</ul>";
                    }
                    ?>
                </ul>
            </ul>
        </p>

        <p><span title="Indicador: 525">
            >>> <b>Institutos Federais de Educação Tecnológica</b><br>
            <ul>
                <li>Existentes até 2002: <?=number_format((is_array($arrTipo['CampProfissional - Existentes até 2002']['valor'])?array_sum($arrTipo['CampProfissional - Existentes até 2002']['valor']):0),0,",",".")?> câmpus em <?=number_format((is_array($arrTipo['CampProfissional - Existentes até 2002 - Municipios']['valor'])?array_sum($arrTipo['CampProfissional - Existentes até 2002 - Municipios']['valor']):0),0,",",".")?> municípios</li>
                <li>Expansão 2003-2010</li>
                <ul>
                    <li>Câmpus criados: <?=number_format((is_array($arrTipo['CampProfissional - Expansão 2003-2010']['valor'])?array_sum($arrTipo['CampProfissional - Expansão 2003-2010']['valor']):0),0,",",".")?></li>
                    <?php
                    $sql = "SELECT universidade, campus
                            FROM (
                                SELECT
                                    entinst.entnome AS universidade,
                                    entcampus.entnome AS campus,
                                    dsh.dshuf AS estuf,
                                    dsh.dshcodmunicipio AS muncod,
                                    est.regcod
                                FROM painel.seriehistorica seh
                                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                                INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                                INNER JOIN entidade.entidade entcampus ON entcampus.entid = dsh.entid
                                INNER JOIN entidade.funcaoentidade fn ON entcampus.entid = fn.entid AND fn.funid = 17 AND entcampus.entstatus = 'A' AND fn.fuestatus = 'A'
                                INNER JOIN entidade.funentassoc a ON a.fueid = fn.fueid
                                INNER JOIN entidade.entidade entinst ON entinst.entid = a.entid
                                INNER JOIN entidade.funcaoentidade fn2 ON entinst.entid = fn2.entid AND fn2.funid = 11 AND entinst.entstatus = 'A' AND fn2.fuestatus = 'A'
                                WHERE seh.sehstatus <> 'I'
                                AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                                AND seh.indid = 525
                                AND dsh.tidid1 IN (2257) --Expansão 2003-2010
                            ) AS FOO
                            $where
                            ORDER BY universidade, campus";
                    $arrDados = $db->carregar($sql);
                    if($arrDados){
                        $contador = 0;
                        $campusUniversidade = "";
                        $arrUniversidade = array();
                        foreach($arrDados as $dado){
                            if($contador==0) $universidade = $dado['universidade'];
                            if($universidade==$dado['universidade']){
                                if($campusUniversidade){
                                    $campusUniversidade = $campusUniversidade.", ".$dado['campus'];
                                }else{
                                    $campusUniversidade = $dado['campus'];
                                }
                            }else{
                                $arrUniversidade[] = array(
                                    'universidade'=>$universidade,
                                    'campus'=>$campusUniversidade,
                                );
                                $campusUniversidade = $dado['campus'];
                            }
                            $universidade = $dado['universidade'];
                            $contador++;
                        }
                        $arrUniversidade[] = array(
                            'universidade'=>$universidade,
                            'campus'=>$campusUniversidade,
                        );
                        echo "<ul>";
                        foreach($arrUniversidade as $dado){
                            echo "    <li>".$dado['campus']." (".$dado['universidade'].")</li>";
                        }
                        echo "</ul>";
                    }
                    ?>
                </ul>
                <li>Expansão 2011-2014</li>
                <ul>
                    <li>Novos câmpus: <?=number_format((is_array($arrTipo['CampProfissional - Expansão 2011-2014']['valor'])?array_sum($arrTipo['CampProfissional - Expansão 2011-2014']['valor']):0),0,",",".")?></li>
                    <?php
                    $sql = "SELECT universidade, campus, situacao, matriculas, vagas, 1 AS valor
                            FROM (
                                SELECT
                                entinst.entnome AS universidade,
                                entcampus.entnome AS campus,
                                dsh.dshuf AS estuf,
                                dsh.dshcodmunicipio AS muncod,
                                est.regcod,
                                CASE
                                    WHEN dsh.tidid2 IN (1738) THEN 'Funcionando'
                                    WHEN dsh.tidid2 IN (1739) THEN 'Em implantação'
                                END AS situacao,
                                matriculas,
                                vagas
                                FROM painel.seriehistorica seh
                                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                                INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                                INNER JOIN entidade.entidade entcampus ON entcampus.entid = dsh.entid
                                INNER JOIN entidade.funcaoentidade fn ON entcampus.entid = fn.entid AND fn.funid = 17 AND entcampus.entstatus = 'A' AND fn.fuestatus = 'A'
                                INNER JOIN entidade.funentassoc a ON a.fueid = fn.fueid
                                INNER JOIN entidade.entidade entinst ON entinst.entid = a.entid
                                INNER JOIN entidade.funcaoentidade fn2 ON entinst.entid = fn2.entid AND fn2.funid = 11 AND entinst.entstatus = 'A' AND fn2.fuestatus = 'A'
                                LEFT JOIN academico.campus cmp ON cmp.entid = entcampus.entid
                                LEFT JOIN
                                    (SELECT
                                        entid,
                                        sum(dsh.dshqtde::integer) as matriculas
                                    FROM painel.seriehistorica sh
                                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                                    WHERE sh.sehstatus <> 'I'
                                    AND dpe.dpeanoref = '2014'
                                    AND (
                                            (sh.indid IN (2575))
                                            OR
                                            (sh.indid IN (2905))
                                            OR
                                            (sh.indid IN (2903))
                                        )
                                    GROUP BY entid) AS cpi1 ON cpi1.entid = entcampus.entid
                                LEFT JOIN
                                    (SELECT cmpid, SUM(cpivalor) AS vagas FROM academico.campusitem WHERE itmid IN (35) AND cpiano = '2014' AND cpitabnum = 1 GROUP BY cmpid ) AS cpi2 ON cpi2.cmpid = cmp.cmpid
                                WHERE seh.sehstatus <> 'I'
                                AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                                AND seh.indid = 525
                                AND dsh.tidid1 IN (2596, 2597) --Expansão 2011-2014
                            ) AS FOO
                            $where
                            ORDER BY situacao DESC, universidade, campus";
                    $arrDados = $db->carregar($sql);
                    $arrSituacao = array();
                    if($arrDados){
                        foreach($arrDados as $dado){
                            $arrSituacao[$dado['situacao']]['valor'][] = $dado['valor'];
                        }
                        $situacao = "";
                        $labelSituacao = "";
                        echo "<ul>";
                        foreach($arrDados as $dado){
                            $labelSituacao = $dado['situacao'];
                            if($labelSituacao != $situacao){
                                echo "<li>".$labelSituacao.": ".($labelSituacao=='Funcionando' ? number_format((is_array($arrSituacao['Funcionando']['valor'])?array_sum($arrSituacao['Funcionando']['valor']):0),0,",",".") : number_format((is_array($arrSituacao['Em implantação']['valor'])?array_sum($arrSituacao['Em implantação']['valor']):0),0,",","."))."</li>";
                                $situacao = $labelSituacao;
                            }
                            echo "<ul>";
                            echo "    <li>".$dado['campus']." (".$dado['universidade'].") - ".($labelSituacao=='Funcionando' ? number_format($dado['matriculas'],0,",",".")." matrículas em 2014" : number_format($dado['vagas'],0,",",".")." vagas em 2014")."</li>";
                            echo "</ul>";
                        }
                        echo "</ul>";
                    }
                    ?>
                </ul>
                <li>A implantar 2015</li>
                <ul>
                    <li>Câmpus: <?=number_format((is_array($arrTipo['CampProfissional - Novas expansões']['valor'])?array_sum($arrTipo['CampProfissional - Novas expansões']['valor']):0),0,",",".")?></li>
                    <?php
                    $sql = "SELECT universidade, campus, situacao, matriculas, vagas, 1 AS valor
                            FROM (
                                SELECT
                                entinst.entnome AS universidade,
                                entcampus.entnome AS campus,
                                dsh.dshuf AS estuf,
                                dsh.dshcodmunicipio AS muncod,
                                est.regcod,
                                CASE
                                    WHEN dsh.tidid2 IN (1738) THEN 'Funcionando'
                                    WHEN dsh.tidid2 IN (1739) THEN 'Em implantação'
                                END AS situacao,
                                matriculas,
                                vagas
                                FROM painel.seriehistorica seh
                                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                                INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                                INNER JOIN entidade.entidade entcampus ON entcampus.entid = dsh.entid
                                INNER JOIN entidade.funcaoentidade fn ON entcampus.entid = fn.entid AND fn.funid = 17 AND entcampus.entstatus = 'A' AND fn.fuestatus = 'A'
                                INNER JOIN entidade.funentassoc a ON a.fueid = fn.fueid
                                INNER JOIN entidade.entidade entinst ON entinst.entid = a.entid
                                INNER JOIN entidade.funcaoentidade fn2 ON entinst.entid = fn2.entid AND fn2.funid = 11 AND entinst.entstatus = 'A' AND fn2.fuestatus = 'A'
                                LEFT JOIN academico.campus cmp ON cmp.entid = entcampus.entid
                                LEFT JOIN
                                    (SELECT
                                        entid,
                                        sum(dsh.dshqtde::integer) as matriculas
                                    FROM painel.seriehistorica sh
                                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                                    WHERE sh.sehstatus <> 'I'
                                    AND dpe.dpeanoref = '2014'
                                    AND (
                                            (sh.indid IN (2575))
                                            OR
                                            (sh.indid IN (2905))
                                            OR
                                            (sh.indid IN (2903))
                                        )
                                    GROUP BY entid) AS cpi1 ON cpi1.entid = entcampus.entid
                                LEFT JOIN
                                    (SELECT cmpid, SUM(cpivalor) AS vagas FROM academico.campusitem WHERE itmid IN (35) AND cpiano = '2014' AND cpitabnum = 1 GROUP BY cmpid ) AS cpi2 ON cpi2.cmpid = cmp.cmpid
                                WHERE seh.sehstatus <> 'I'
                                AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                                AND seh.indid = 525
                                AND dsh.tidid1 IN (7107) --Novas Expansões
                            ) AS FOO
                            $where
                            ORDER BY situacao DESC, universidade, campus";
                    $arrDados = $db->carregar($sql);
                    $arrSituacao = array();
                    if($arrDados){
                        foreach($arrDados as $dado){
                            $arrSituacao[$dado['situacao']]['valor'][] = $dado['valor'];
                        }
                        $situacao = "";
                        $labelSituacao = "";
                        echo "<ul>";
                        foreach($arrDados as $dado){
                            $labelSituacao = $dado['situacao'];
                            if($labelSituacao != $situacao){
                                echo "<li>".$labelSituacao.": ".($labelSituacao=='Funcionando' ? number_format((is_array($arrSituacao['Funcionando']['valor'])?array_sum($arrSituacao['Funcionando']['valor']):0),0,",",".") : number_format((is_array($arrSituacao['Em implantação']['valor'])?array_sum($arrSituacao['Em implantação']['valor']):0),0,",","."))."</li>";
                                $situacao = $labelSituacao;
                            }
                            echo "<ul>";
                            echo "    <li>".$dado['campus']." (".$dado['universidade'].")</li>";
                            echo "</ul>";
                        }
                        echo "</ul>";
                    }
                    ?>
                </ul>
            </ul>
        </p>
        <?php
/*
        if($ministro){
            $sql = "SELECT setor, tipo
                    FROM(
                        SELECT
                            CASE
                                WHEN dsh.tidid2 IN (7925,7922) THEN 'Em implantação'
                                WHEN dsh.tidid2 IN (7924,7921) THEN 'Em planejamento'
                                WHEN dsh.tidid2 IN (7926,7923) THEN 'Inaugurado'
                            END AS setor,
                            CASE
                                WHEN dsh.tidid2 IN (7926,7925,7924) THEN 'IST ' || mun.mundescricao || ' - ' || tid1.tiddsc
                                WHEN dsh.tidid2 IN (7923,7922,7921) THEN 'ISI ' || mun.mundescricao || ' - ' || tid1.tiddsc
                            END AS tipo,
                            est.regcod,
                            est.estuf,
                            mun.muncod
                        FROM painel.seriehistorica seh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                        INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
                        INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio
                        WHERE seh.sehstatus <> 'I'
                        AND seh.dpeid = ( SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid AND s.sehstatus <> 'I')
                        AND seh.indid = 2948
                    ) AS FOO
                    $where
                    ORDER BY setor, tipo";
            $arrDados = $db->carregar($sql);
            if($arrDados){
                echo "<p>";
                echo "<span title='Indicador: 2948'>";
                echo ">>> <b>Institutos SENAI</b> – ".count($arrDados)." novas unidades";
                echo "<ul>";
                $setor = "";
                foreach($arrDados as $dado){
                    $labelSetor = $dado['setor'];
                    if($labelSetor != $setor){
                        echo "<li>".$labelSetor.":</li>";
                        $setor = $labelSetor;
                    }
                    echo "	<ul>";
                    echo "		<li>".$dado['tipo'] . "</li>";
                    echo "	</ul>";
                }
                echo "</ul>";
                echo "</p>";
            }
        }
*/
		?>
        <?php if($ministro){
            $totalAprovadas2015 = (is_array($arrTipo['Creches - 2015 - Aprovadas']['valor']) ? array_sum($arrTipo['Creches - 2015 - Aprovadas']['valor']):0);
            $totalAprovadas2014 = (is_array($arrTipo['Creches - 2014 - Aprovadas']['valor']) ? array_sum($arrTipo['Creches - 2014 - Aprovadas']['valor']):0);
            $totalConcluidas2015 = (is_array($arrTipo['Creches - 2015 - Concluída']['valor']) ? array_sum($arrTipo['Creches - 2015 - Concluída']['valor']):0);
            $totalConcluidas2014 = (is_array($arrTipo['Creches - 2014 - Concluída']['valor']) ? array_sum($arrTipo['Creches - 2014 - Concluída']['valor']):0);
            $totalAprovadas = $totalAprovadas2015 - $totalAprovadas2014;
            $totalConcluidas = $totalConcluidas2015 - $totalConcluidas2014;

            $totalPronatecFIC = (is_array($arrTipo['Pronatec - FIC - 2015']['valor']) ? array_sum($arrTipo['Pronatec - FIC - 2015']['valor']):0);
            $totalPronatecTecnico = (is_array($arrTipo['Pronatec - Técnico - 2015']['valor']) ? array_sum($arrTipo['Pronatec - Técnico - 2015']['valor']):0);
            $totalPronatec = $totalPronatecFIC + $totalPronatecTecnico;
            ?>
            >>> <b>Em 2015</b><br>
            <ul>
                <span title="Indicador: 2367">
                <li><?=number_format($totalAprovadas,0,",",".")?> creches aprovadas e <?=number_format($totalConcluidas,0,",",".")?> creches concluídas</li>
                <span title="Indicador: 690">
                <li><?=number_format((is_array($arrTipo['Tempo Integral - 2015']['valor'])?array_sum($arrTipo['Tempo Integral - 2015']['valor']):0),0,",",".")?> escolas tempo integral</li>
                <span title="Indicador: 2845">
                <li><?=number_format((is_array($arrTipo['Alfabetização na idade certa - 2015']['valor'])?array_sum($arrTipo['Alfabetização na idade certa - 2015']['valor']):0),0,",",".")?> professores alfabetizadores em formação</li>
                <span title="Indicadores: 1865, 2826 e 2888 (Recursos FNDE)">
			    <li><?=number_format((is_array($arrTipo['ônibus - 2015']['valor'])?array_sum($arrTipo['ônibus - 2015']['valor']):0),0,",",".")?> ônibus, <?=number_format((is_array($arrTipo['lanchas - 2015']['valor'])?array_sum($arrTipo['lanchas - 2015']['valor']):0),0,",",".")?> lanchas e <?=number_format((is_array($arrTipo['bicicletas - 2015']['valor'])?array_sum($arrTipo['bicicletas - 2015']['valor']):0),0,",",".")?> bicicletas, adquiridos com recursos do FNDE</li>
                <span title="Indicador: 1562">
                <li><?=number_format((is_array($arrTipo['CIENCIA - 2015']['valor'])?array_sum($arrTipo['CIENCIA - 2015']['valor']):0),0,",",".")?> bolsas concedidas no ciências sem fronteiras</li>
                <li>Mais Médicos:</li>
                <ul>
                    <li>Novas vagas de graduação de medicina: <?=number_format((is_array($arrTipo['Mais Médicos - vagas - 2015']['valor'])?array_sum($arrTipo['Mais Médicos - vagas - 2015']['valor']):0),0,",",".")?> novas vagas</li>
                    <ul>
                        <span title="Indicador: 2678">
                        <li>Rede pública: <?=number_format((is_array($arrTipo['Mais Médicos - publica - 2015']['valor'])?array_sum($arrTipo['Mais Médicos - publica - 2015']['valor']):0),0,",",".")?> vagas criadas (<?=number_format((is_array($arrTipo['Mais Médicos - publica - existentes - 2015']['valor'])?array_sum($arrTipo['Mais Médicos - publica - existentes - 2015']['valor']):0),0,",",".")?> vagas em cursos existentes e <?=number_format((is_array($arrTipo['Mais Médicos - publica - novos - 2015']['valor'])?array_sum($arrTipo['Mais Médicos - publica - novos - 2015']['valor']):0),0,",",".")?> vagas em novos cursos)</li>
                        <span title="Indicador: 2823">
                        <li>Rede privada: <?=number_format((is_array($arrTipo['Mais Médicos - privada - 2015']['valor'])?array_sum($arrTipo['Mais Médicos - privada - 2015']['valor']):0),0,",",".")?> vagas criadas (<?=number_format((is_array($arrTipo['Mais Médicos - privada - existentes - 2015']['valor'])?array_sum($arrTipo['Mais Médicos - privada - existentes - 2015']['valor']):0),0,",",".")?> vagas em cursos existentes e <?=number_format((is_array($arrTipo['Mais Médicos - privada - novos - 2015']['valor'])?array_sum($arrTipo['Mais Médicos - privada - novos - 2015']['valor']):0),0,",",".")?> vagas em novos cursos)</li>
                        <span title="Indicador: 2889">
                        <li>Rede privada: <?=number_format((is_array($arrTipo['Mais Médicos - privada - reativada - 2015']['valor'])?array_sum($arrTipo['Mais Médicos - privada - reativada - 2015']['valor']):0),0,",",".")?> vagas reativadas</li>
                        <li>Previsão Nacional de expansão rede pública: 400 novas vagas</li>
                        <ul><li>220 em autorizações e 180 em aumento de vagas em cursos existentes</li></ul>
                        <li>Previsão Nacional de expansão rede privada: 1.540 vagas</li>
                        <ul><li>1.120 em autorizações e 420 em aumento de vagas em cursos existentes</li></ul>
                    </ul>
                </ul>
                <span title="Indicador: 2577">
                <li><?=number_format($totalPronatec,0,",",".")?> matrículas no Pronatec, <?=number_format($totalPronatecFIC,0,",",".")?> em cursos de qualificação e <?=number_format($totalPronatecTecnico,0,",",".")?> em cursos técnicos</li>
                <span title="Indicador: 2787">
                <li><?=number_format((is_array($arrTipo['PROUNI - 2015']['valor'])?array_sum($arrTipo['PROUNI - 2015']['valor']):0),0,",",".")?> bolsas ocupadas no PROUNI</li>
                <span title="Indicador: 778">
                <li><?=number_format((is_array($arrTipo['FIES - 2015']['valor'])?array_sum($arrTipo['FIES - 2015']['valor']):0),0,",",".")?> contratos firmados pelo FIES</li>
                <span title="Indicador: 2575">
                <li><?=number_format((is_array($arrTipo['Institutos - 2015']['valor'])?array_sum($arrTipo['Institutos - 2015']['valor']):0),0,",",".")?> matrículas em Institutos Federais</li>
            </ul>
            </p>
        <?php } ?>
	</body>
</html>
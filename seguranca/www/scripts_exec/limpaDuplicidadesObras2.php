<?php 

ini_set("memory_limit", "3024M");
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '147';


$db = new cls_banco();


include_once APPRAIZ . 'www/obras2/_constantes.php';
include_once APPRAIZ . 'www/obras2/_funcoes.php';
include_once APPRAIZ . 'www/obras2/_componentes.php';
include_once APPRAIZ . "www/autoload.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";

if($_REQUEST['pagamento']){

    $sql = "

        SELECT COUNT(pse.sueid), pse.pagid, pse.sueid
        from obras2.pagamento_supervisao_empresa pse
        inner join obras2.supervisaoempresa sue on sue.sueid = pse.sueid AND sue.suestatus = 'A'
        inner join obras2.empreendimento emp on emp.empid = sue.empid AND emp.empidpai IS NULL
        inner join obras2.obras obr on emp.empid = obr.empid AND obr.obridpai IS NULL AND obr.obrstatus = 'A'
        WHERE suestatus = 'A' AND pse.psestatus = 'A'
        GROUP BY pse.pagid, pse.sueid
        HAVING COUNT(pse.sueid) > 1


    ";
    $pagamentos = $db->carregar($sql);

    foreach ($pagamentos as $pg){
        $subSql = "SELECT * FROM obras2.pagamento_supervisao_empresa WHERE pagid = {$pg['pagid']} AND sueid = {$pg['sueid']} AND psestatus = 'A'";
        $pse = $db->carregar($subSql);

        foreach($pse as $key => $p){
            if (count($pse) > 1){
                $db->executar("UPDATE obras2.pagamento_supervisao_empresa SET psestatus = 'I' WHERE pseid = {$p['pseid']}");
                unset($pse[$key]);
            }
        }
        ver('mantido:', $pse);
    }

    $db->commit();
    exit;
}

// Corrige o valor de todas AS OS
if($_REQUEST['OS_VALOR']){
    include_once APPRAIZ . "includes/classes/modelo/obras2/Supervisao_Os.class.inc";
    include_once APPRAIZ . "includes/classes/modelo/obras2/Supervisao_Grupo_Empresa.class.inc";

    $sql = "SELECT DISTINCT sue.sosid FROM obras2.supervisaoempresa sue
            JOIN workflow.documento d ON d.docid = sue.docid
            WHERE d.esdid = 1188 AND sue.suestatus = 'A'";
    $soss = $db->carregarColuna($sql);
    foreach ($soss as $sosid){
        $os = new Supervisao_Os($sosid);
        $os->recalculaValorOs();
    }
    ver($soss, d);
    exit;
}

// Corrige as OS que foram cadastradas com empreendimento de bkp
if($_REQUEST['OS_EMP']){
    $sql = "SELECT os.sosid, e.empid as empiderro,o.obrid as obriderro, e.empidpai as empidcorreto, o1.obrid as obridcorreto , s.supid, sue.sueid
                    FROM obras2.supervisao_os_obra os
                    JOIN obras2.empreendimento e ON e.empid = os.empid
                    JOIN obras2.empreendimento e1 ON e1.empid = e.empidpai
                    JOIN obras2.obras o ON o.empid = e.empid
                    JOIN obras2.obras o1 ON o1.empid = e1.empid
                    LEFT JOIN obras2.supervisaoempresa sue ON os.sosid = sue.sosid AND sue.empid = e.empid
                    LEFT JOIN obras2.supervisao s ON s.sueid = sue.sueid
                    WHERE e.empidpai IS NOT NULL AND soostatus = 'A'";

    $dados = $db->carregar($sql);

    foreach ($dados as $dado){
        $sql1 = $sql2 = $sql3 = '';
        // Corrige a supervisao empresa
        if($dado['sueid']){
            $sql1 = "UPDATE obras2.supervisaoempresa SET empid = {$dado['empidcorreto']} WHERE sueid = {$dado['sueid']}";
            $db->executar($sql1);
        }
        // Corrige a supervisao
        if($dado['supid']){
            $sql2 = "UPDATE obras2.supervisao SET obrid = {$dado['obridcorreto']} WHERE supid = {$dado['supid']}";
            $db->executar($sql2);
        }
        // Corrige a OS
        $sql3 = "UPDATE obras2.supervisao_os_obra SET empid = {$dado['empidcorreto']} WHERE sosid = {$dado['sosid']} AND empid = {$dado['empiderro']}";
        echo $sql1 . '<br />' .$sql2. '<br />' .$sql3;
        echo '<br /> -------------- <br />';
        $db->executar($sql3);
    }
    $db->commit();
    ver($dados, d);
}

if($_REQUEST['OS']){
    include_once APPRAIZ . "includes/classes/modelo/obras2/Obras.class.inc";
    include_once APPRAIZ . "includes/classes/modelo/obras2/Email.class.inc";
    include_once APPRAIZ . "includes/classes/modelo/obras2/OrdemServicoMI.class.inc";
    include_once APPRAIZ . "includes/classes/modelo/obras2/DestinatarioEmail.class.inc";
    require_once APPRAIZ . 'includes/workflow.php';

    $sql = "SELECT
              o.obrid,
              ed.esddsc,
              (SELECT ed.esddsc FROM obras2.ordemservicomi sm
                LEFT JOIN workflow.documento        d ON d.docid  = sm.docid
                LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                WHERE tomid = 1 AND sm.osmstatus = 'A' AND o.obrid = sm.obrid ORDER BY sm.osmid DESC LIMIT 1) dsc,
              (SELECT ed.esdid FROM obras2.ordemservicomi sm
                LEFT JOIN workflow.documento        d ON d.docid  = sm.docid
                LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
              WHERE tomid = 1 AND sm.osmstatus = 'A' AND o.obrid = sm.obrid ORDER BY sm.osmid DESC LIMIT 1) esdid,
              (SELECT sm.osmid FROM obras2.ordemservicomi sm
                LEFT JOIN workflow.documento        d ON d.docid  = sm.docid
                LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
              WHERE tomid = 1 AND sm.osmstatus = 'A' AND o.obrid = sm.obrid ORDER BY sm.osmid DESC LIMIT 1) osmid,
              (SELECT sm.docid FROM obras2.ordemservicomi sm
                LEFT JOIN workflow.documento        d ON d.docid  = sm.docid
                LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
              WHERE tomid = 1 AND sm.osmstatus = 'A' AND o.obrid = sm.obrid ORDER BY sm.osmid DESC LIMIT 1) docid

            FROM obras2.obras o
              LEFT JOIN workflow.documento        d ON d.docid  = o.docid
              LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
              LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid
              LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf

            WHERE
              o.obridpai IS NULL AND
              o.obrstatus = 'A' AND
              o.tpoid IN (104,105) AND
              d.esdid = 690 AND
              o.obrid NOT IN (SELECT sm.obrid FROM obras2.ordemservicomi sm
                LEFT JOIN workflow.documento        d ON d.docid  = sm.docid
                LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
              WHERE tomid = 1 AND sm.osmstatus = 'A' AND d.esdid IN (905,907));";
    $oss = $db->carregar($sql);
    
    $situaçaoOS = array();
    foreach($oss as $os){
        // Tramita de volta para execução
        if($os['esdid'] == 908){
            wf_alterarEstado($os['docid'], 2087, 'OS retornada para execução pois a obra ainda se encontra em execução.', array('osmid' => $os['osmid'] ));
            $situaçaoOS['908'][] = $os;
        } else if($os['esdid'] == 906){
            $situaçaoOS['906'][] = $os;
            wf_alterarEstado($os['docid'], 2713, 'OS retornada para execução pois a obra ainda se encontra em execução.', array('osmid' => $os['osmid'] ));
        } else {
            $situaçaoOS['notramit'][] = $os;
        }
    }
    ver($situaçaoOS, d);
    $db->commit();
    exit;
}

if($_REQUEST['contratacao']){
    $obras_contrato_duplicado ="SELECT
                                  count(obrid),
                                  obrid
                                FROM obras2.obrascontrato
                                WHERE ocrstatus = 'A'
                                GROUP BY obrid
                                HAVING count(obrid) > 1";

    $itens = $db->carregar($obras_contrato_duplicado);

    if(!$itens)
        die('Nenhum item duplicado.');

    foreach($itens as $item){

        $contratosExclusao = "
          SELECT
            ocrid
          FROM obras2.obrascontrato
          WHERE obrid = {$item['obrid']} AND ocrstatus = 'A' AND
                ocrid NOT IN (SELECT
                                MIN(ocrid)
                              FROM obras2.obrascontrato
                              WHERE obrid = {$item['obrid']} AND ocrstatus = 'A'
                              GROUP BY obrid)
          ORDER BY ocrid DESC
        ";

        $contratosInativados = $db->carregar($contratosExclusao);
        echo 'Obra : ' . $item['obrid'] . '<br />';
        foreach($contratosInativados as $ctr){
            $sql = "UPDATE obras2.obrascontrato
                    SET ocrstatus = 'I'
                    WHERE ocrid = {$ctr['ocrid']}";
            echo '&nbsp;&nbsp;Inativado : ' . $ctr['ocrid'] . '<br />';
            echo '&nbsp;&nbsp;' . $sql . '<br />';
            $db->executar($sql);
        }
        echo "------------------------------ <br />";

    }
    $db->commit();
    exit;
}



if($_REQUEST['supervisao']){
    $sql_itens_duplicados = "SELECT o.obrid, s.qtd, s.sueid from obras2.supervisaoempresa su
      JOIN (
             SELECT
               count(*) as qtd, s.sueid
             FROM obras2.supervisao s
             WHERE s.supstatus = 'A' AND s.sueid is not null
             group by s.sueid
             HAVING count(*)>1) as s ON s.sueid = su.sueid
      JOIN obras2.obras o ON o.empid = su.empid AND o.obridpai is null AND o.obrstatus = 'A'
    --WHERE o.obrid NOT IN (30933,29727, 25691, 24859, 23020,22604, 20065, 19825, 19555, 19409, 18712, 18431, 18428, 18167, 11875, 11157, 8702)
    ORDER BY o.obrid DESC";



    $itens = $db->carregar($sql_itens_duplicados);

    foreach($itens as $item){
        $sql_itens_exclusao = "SELECT
                                  s.supdtinclusao,(select sum(si.spivlritemsobreobraexecanterior) from obras2.supervisaoitem si where si.supid = s.supid) as percent,*
                                FROM obras2.supervisao s
                                WHERE s.supstatus = 'A' AND s.sueid is not null AND s.obrid = {$item['obrid']} AND supid NOT IN (
                                  SELECT
                                    MAX(s.supid) as supid
                                  FROM obras2.supervisao s
                                  WHERE s.supstatus = 'A' AND s.sueid is not null AND s.obrid = {$item['obrid']} AND sueid = {$item['sueid']}
                                  GROUP BY s.sueid
                                )
                                ORDER BY s.supdtinclusao DESC";

        $itensExclusao = $db->carregar($sql_itens_exclusao);

        $sql_item_mantido = "SELECT
                                    MAX(s.supid) as supid
                                  FROM obras2.supervisao s
                                  WHERE s.supstatus = 'A' AND s.sueid is not null AND sueid = {$item['sueid']}
                                  GROUP BY s.sueid";

        $itemMantido = $db->pegaLinha($sql_item_mantido);

        echo '<b>Supervisão ' . $item['sueid'] . ' da obra ' .$item['obrid']. '</b><br />';

        if(!$itensExclusao){
            echo ' Supervisão duplicada pela vinculação ' . $itemEx['supid'] . '<br />';
        } else {
            foreach($itensExclusao as $itemEx){

                $sql_inativacao = "UPDATE obras2.supervisao SET supstatus = 'I' WHERE supid = {$itemEx['supid']}";
                $db->executar($sql_inativacao);

                echo ' Exclusao da supervisao: ' . $itemEx['supid'] . '<br />';
            }
            echo ' Mantida ' . $itemMantido['supid'] . '<br />';
        }

        echo '<b>------------------------------------</b><br />';

        echo '<br />';

    }

    $db->commit();



    exit;
}




$sqlValidação = "select distinct count(sueid),sueid from obras2.questaosupervisao
where qtsstatus = 'A' --and sueid = 1798
group by qstid,sueid
having count(sueid) > 1;";

ver($db->carregar($sqlValidação));

$sql = "
update obras2.questaosupervisao
set qtsstatus = 'I'
where qtsid not in (
select qs2.qtsid from obras2.questaosupervisao  qs2
inner join
(select max(qs1.qtsid) qtsid,qs1.sueid,qs1.qstid from obras2.questaosupervisao  qs1
where qs1.sueid in
(
select distinct qs.sueid from obras2.questaosupervisao  qs
inner join (select count(sueid),sueid from obras2.questaosupervisao
where qtsstatus = 'A'
group by qstid,sueid
having count(sueid) > 1 ) x on x.sueid = qs.sueid)
group by qs1.sueid,qs1.qstid
order by qs1.sueid,qs1.qstid) y on y.qtsid = qs2.qtsid)
and sueid in
(
select distinct qs2.sueid from obras2.questaosupervisao  qs2
inner join
(select max(qs1.qtsid) qtsid,qs1.sueid,qs1.qstid from obras2.questaosupervisao  qs1
where qs1.sueid in
(
select distinct qs.sueid from obras2.questaosupervisao  qs
inner join (select count(sueid),sueid from obras2.questaosupervisao
where qtsstatus = 'A'
group by qstid,sueid
having count(sueid) > 1 ) x on x.sueid = qs.sueid)
group by qs1.sueid,qs1.qstid
order by qs1.sueid,qs1.qstid) y on y.qtsid = qs2.qtsid);
";
$db->executar($sql);
$db->commit();

ver($db->carregar($sqlValidação));

?>

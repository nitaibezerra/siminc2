<?php

ini_set("memory_limit", "3024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = "simec_espelho_producao"; //simec_desenvolvimento
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
include_once APPRAIZ . "includes/classes/modelo/obras2/ItensComposicaoObras.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/ObrasContrato.class.inc";

/**
 * Script para atualizar o percentual de todas as obras de acordo com o percentual da última vistoria
 */

$sqlObras = "SELECT
              o.obrid,
              o.obrpercentultvistoria,
              d.esdid,
              TO_CHAR(o.obrdtultvistoria, 'YYYY-mm-dd') as obrdtultvistoria,
              (SELECT
                 ( SELECT CASE WHEN SUM(icovlritem) > 0 THEN ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2) ELSE 0 END AS total FROM obras2.itenscomposicaoobra i INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL WHERE i.icostatus = 'A' AND i.relativoedificacao = 'D' AND cro.obrid = o.obrid AND i.obrid = cro.obrid ) as percentual
               FROM
                 obras2.supervisao s
               WHERE
                 s.obrid = o.obrid AND
                 s.emsid IS NULL AND s.smiid IS NULL AND
                 s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S'
                 AND s.rsuid = 1
               ORDER BY
                 s.supdata DESC, s.supdtinclusao DESC, s.supid DESC LIMIT 1) as percentual,
               (SELECT
                 s.supid
               FROM
                 obras2.supervisao s
               WHERE
                 s.obrid = o.obrid AND
                 s.emsid IS NULL AND s.smiid IS NULL AND
                 s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S'
                 AND s.rsuid = 1
               ORDER BY
                 s.supdata DESC, s.supdtinclusao DESC, s.supid DESC LIMIT 1) as supid,
               (SELECT
                 TO_CHAR(s.supdata, 'YYYY-mm-dd') as supdata
               FROM
                 obras2.supervisao s
               WHERE
                 s.obrid = o.obrid AND
                 s.emsid IS NULL AND s.smiid IS NULL AND
                 s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S'
                 AND s.rsuid = 1
               ORDER BY
                 s.supdata DESC, s.supdtinclusao DESC, s.supid DESC LIMIT 1) as supdata
            FROM obras2.obras o
            JOIN workflow.documento d ON d.docid = o.docid
            WHERE o.obrstatus = 'A' AND obridpai IS NULL AND o.obridvinculado IS NULL AND
            (o.obrid IN (  SELECT s.obrid FROM
                            obras2.supervisao s
                          WHERE
                            s.emsid IS NULL AND s.smiid IS NULL AND
                            s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S'
                            AND s.rsuid = 1
                          GROUP BY s.obrid) OR (o.obrpercentultvistoria > 0 AND o.obrid NOT IN (SELECT obrid FROM obras.obrainfraestrutura) ) ) -- AND o.obrid = 14679
--8938";

function testaCronogramaContrato($obrid)
{
    global $db;
    // Verifica se existe itens duplicados
    $sql = "SELECT
              COUNT(ico.itcid)
            FROM obras2.itenscomposicaoobra ico
            INNER JOIN obras2.cronograma cro ON cro.croid = ico.croid AND cro.crostatus = 'A'
            WHERE cro.obrid = {$obrid} AND ico.icostatus = 'A' AND ico.obrid = cro.obrid
            GROUP BY ico.itcid
            HAVING COUNT(ico.itcid) > 1";
    $d = $db->pegaUm($sql);
    if ($d)
        return false;

    // Veritica se a soma dos itens e igual ao cotrato
    $obraContrato = new ObrasContrato();
    $itensComposicao = new ItensComposicaoObras();
    $somaItens = (float)$itensComposicao->getValorTotalItens($obrid);
    $ocrvalorexecucao = (float)$obraContrato->getValorContrato($obrid);

    if (abs(round($somaItens, 2) - round($ocrvalorexecucao, 2)) > 0)
        return false;

    return true;
}

function testaSupervisao($obrid, $supid)
{
    global $db;

    if(!$supid)
        return true;

    $sql = "SELECT
              COUNT(i.itcid)
            FROM obras2.itenscomposicaoobra i
            INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus = 'A'
              LEFT JOIN obras2.supervisaoitem sic
                ON sic.icoid = i.icoid AND sic.supid = {$supid} AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
            WHERE i.icostatus = 'A' AND i.relativoedificacao = 'D' AND cro.obrid = {$obrid} AND i.obrid = cro.obrid
            GROUP BY i.itcid
            HAVING COUNT(i.itcid) > 1";
    $d = $db->pegaUm($sql);
    if ($d)
        return false;
    return true;
}


$obras = $db->carregar($sqlObras);
$obrasProblemaSupervisao = array();
$obrasProblemaContrato = array();
$obrasParaAtualizar = array();
$obrasConcluidas = array();
$obrasDtVistoria = array();

$c = 1;
foreach ($obras as $obra) {
    if ($obra['supdata'] != $obra['obrdtultvistoria'])
        $obrasDtVistoria[] = $obra;

    $diff = abs($obra['obrpercentultvistoria'] - $obra['percentual']);
    if ($diff != 0) {

        if ($obra['esdid'] == 693 && $obra['obrpercentultvistoria'] == 100) {
            $obrasConcluidas[] = $obra;
            continue;
        }

        $s = testaSupervisao($obra['obrid'], $obra['supid']);
        $c = testaCronogramaContrato($obra['obrid']);

        if (!$s) {
            $obrasProblemaSupervisao[] = $obra;
            continue;
        } else if (!$c) {
            $obrasProblemaContrato[] = $obra;
            continue;
        }

        $obrasParaAtualizar[] = $obra;
    }
}

foreach ($obrasDtVistoria as $obra) {
    $obra['supdata'] = ($obra['supdata']) ? "'{$obra['supdata']}'" : "NULL";
    $sql = "UPDATE obras2.obras SET obrdtultvistoria = {$obra['supdata']} WHERE obrid = {$obra['obrid']}";
    $db->executar($sql);

    if ($_REQUEST['commit'])
        $db->commit();
    else
        $db->rollback();
}

foreach ($obrasParaAtualizar as $obra) {
    $obra['percentual'] = (!$obra['percentual']) ? 0 : $obra['percentual'];
    $sql = "UPDATE obras2.obras SET obrpercentultvistoria = {$obra['percentual']} WHERE obrid = {$obra['obrid']}";
    $db->executar($sql);

    if ($_REQUEST['commit'])
        $db->commit();
    else
        $db->rollback();
}

?>
<style>
    td {
        border: 1px solid #FF0000;
    }
</style>
<h2>Obras atualizadas</h2>
<table>
    <tr>
        <td>ID</td>
        <td>obrpercentultvistoria</td>
        <td>percentual</td>
    </tr>
    <? $c = 0 ?>
    <? foreach ($obrasParaAtualizar as $obr): ?>
        <tr>
            <td><?= $obr['obrid'] ?></td>
            <td><?= $obr['obrpercentultvistoria'] ?></td>
            <td><?= $obr['percentual'] ?></td>
        </tr>
        <? $c++ ?>
    <? endforeach; ?>
    <tr>
        <td colspan="3">TOTAL: <?= $c ?></td>
    </tr>
</table>

<h2>Obras com data de vistoria errada</h2>
<table>
    <tr>
        <td>ID</td>
        <td>obrdtultvistoria</td>
        <td>supdata</td>
    </tr>
    <? $c = 0 ?>
    <? foreach ($obrasDtVistoria as $obr): ?>
        <tr>
            <td><?= $obr['obrid'] ?></td>
            <td><?= $obr['obrdtultvistoria'] ?></td>
            <td><?= $obr['supdata'] ?></td>
        </tr>
        <? $c++ ?>
    <? endforeach; ?>
    <tr>
        <td colspan="3">TOTAL: <?= $c ?></td>
    </tr>
</table>

<h2>Obras com problema na supervisão</h2>
<table>
    <tr>
        <td>ID</td>
        <td>obrpercentultvistoria</td>
        <td>percentual</td>
    </tr>
    <? $c = 0 ?>
    <? foreach ($obrasProblemaSupervisao as $obr): ?>
        <tr>
            <td><?= $obr['obrid'] ?></td>
            <td><?= $obr['obrpercentultvistoria'] ?></td>
            <td><?= $obr['percentual'] ?></td>
        </tr>
        <? $c++ ?>
    <? endforeach; ?>
    <tr>
        <td colspan="3">TOTAL: <?= $c ?></td>
    </tr>
</table>

<h2>Obras com problemas no contrato</h2>
<table>
    <tr>
        <td>ID</td>
        <td>obrpercentultvistoria</td>
        <td>percentual</td>
    </tr>
    <? $c = 0 ?>
    <? foreach ($obrasProblemaContrato as $obr): ?>
        <tr>
            <td><?= $obr['obrid'] ?></td>
            <td><?= $obr['obrpercentultvistoria'] ?></td>
            <td><?= $obr['percentual'] ?></td>
        </tr>
        <? $c++ ?>
    <? endforeach; ?>
    <tr>
        <td colspan="3">TOTAL: <?= $c ?></td>
    </tr>
</table>

<h2>Obras concluidas com 100%</h2>
<table>
    <tr>
        <td>ID</td>
        <td>obrpercentultvistoria</td>
        <td>percentual</td>
    </tr>
    <? $c = 0 ?>
    <? foreach ($obrasConcluidas as $obr): ?>
        <tr>
            <td><?= $obr['obrid'] ?></td>
            <td><?= $obr['obrpercentultvistoria'] ?></td>
            <td><?= $obr['percentual'] ?></td>
        </tr>
        <? $c++ ?>
    <? endforeach; ?>
    <tr>
        <td colspan="3">TOTAL: <?= $c ?></td>
    </tr>
</table>


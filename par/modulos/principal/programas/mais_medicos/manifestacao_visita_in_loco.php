<?php

include_once APPRAIZ . 'par/modulos/principal/programas/mais_medicos/ManifestacaoVisitaInLoco.php';
include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$manifestacaoVisitaInLoco = new ManifestacaoVisitaInLoco();

if ($_POST) {
    if ($_POST['salvar_manifestacao_visita_in_loco']) {
        $manifestacaoVisitaInLoco->salvarManifestacaoNotaTecnica($_POST, $db);
    }

    if ($_POST['salvar_manifestacao_resposta_nota_tecnica']) {
        $manifestacaoVisitaInLoco->salvarRespostaNotaTecnica($_POST, $db);
    }
}

if ($_GET['recurso'] === '1' and isset($_GET['id'])) {
    $manifestacaoVisitaInLoco->excluirManifestacaoNotaTecnica((int) $_GET['id'], (int) $_GET['arqid'], $db);
}

if ($_GET['recurso'] === '2' and isset($_GET['id'])) {
    $manifestacaoVisitaInLoco->excluirManifestacaoRespostaNotaTecnica((int) $_GET['id'], (int) $_GET['arqid'], $db);
}

if ($_GET['download'] === '1' and isset($_GET['arqid'])) {
    $manifestacaoVisitaInLoco->getArquivo((int) $_GET['arqid']);
}

$sql = $manifestacaoVisitaInLoco->getSqlManifestacaoNotaTecnica();
$dadosManifestacaoMunArray = $db->carregar($sql);
if ($dadosManifestacaoMunArray) {
    $dadosManifestacaoMun = $dadosManifestacaoMunArray[0];
}else{
    $dadosManifestacaoMun = array();
}


if ($dadosManifestacaoMun['mntid']) {
    $sql = $manifestacaoVisitaInLoco->getSqlManifestacaoRespostaNotaTecnica($dadosManifestacaoMun['mntid']);
    $dadosManifestacaoRespMunArray = $db->carregar($sql);
    // $dadosManifestacaoRespMun = $dadosManifestacaoRespMunArray[0];
    $dadosManifestacaoRespMun = $dadosManifestacaoRespMunArray;
}

<?php

if ($_REQUEST['add']) {

    // carrega as funções gerais
    include_once "config.inc";
    include_once APPRAIZ . "includes/funcoes.inc";
    include_once APPRAIZ . "includes/classes_simec.inc";
    include_once APPRAIZ . "includes/workflow.php";

    // carrega as funções do módulo
    include_once '../_constantes.php';
    include_once '../_funcoes.php';
    include_once '../_componentes.php';

    $db = new cls_banco();


    $sql = "select * from workflow.estadodocumento
            where tpdid = " . WF_TPDID_DEMANDASFIES_DEMANDA . "
            and esdid != " . ESD_DEMANDA_EM_INTERVENCAO;
    $dados = $db->carregar($sql);
    $sql = "select pflcod, pfldsc from seguranca.perfil
            where sisid = 198
            and pflcod != 1286
            and pflstatus = 'A'";
    $perfis = $db->carregar($sql);

    foreach ($dados as $dado) {
        $sql = "SELECT aedid FROM workflow.acaoestadodoc
                WHERE esdiddestino = " . ESD_DEMANDA_EM_INTERVENCAO . "
                AND esdidorigem = {$dado['esdid']}";

        $aedid = $db->pegaUm($sql);
        if(!$aedid){
            $sql = "INSERT INTO workflow.acaoestadodoc
                            (esdidorigem, esdiddestino, aeddscrealizar,
                            aedstatus, aeddscrealizada, esdsncomentario,
                            aedvisivel, aedcodicaonegativa)
                      VALUES(
                            {$dado['esdid']}, " . ESD_DEMANDA_EM_INTERVENCAO . ", 'Solicitar Intervenção',
                            'A', 'Intervenção Solicitada', true,
                            false, false)
                      RETURNING
                            aedid";
            $aedid = $db->pegaUm($sql);

            foreach ($perfis as $perfil) {
                $sql = "INSERT INTO workflow.estadodocumentoperfil (aedid, pflcod)
                          VALUES($aedid, {$perfil['pflcod']})";
                $db->executar($sql);
            }
            $db->commit();
        }


        $sql = "SELECT aedid FROM workflow.acaoestadodoc
                WHERE esdiddestino = {$dado['esdid']}
                AND esdidorigem = " . ESD_DEMANDA_EM_INTERVENCAO;

        $aedid = $db->pegaUm($sql);

        if(!$aedid){
            $sql = "INSERT INTO workflow.acaoestadodoc
                            (esdidorigem, esdiddestino, aeddscrealizar,
                            aedstatus, aeddscrealizada, esdsncomentario,
                            aedvisivel, aedcodicaonegativa)
                      VALUES(
                            " . ESD_DEMANDA_EM_INTERVENCAO . ", {$dado['esdid']}, 'Retornar ao demandante',
                            'A', 'Demanda retornada', true,
                            false, false)
                      RETURNING
                            aedid";
            $aedid = $db->pegaUm($sql);

            foreach ($perfis as $perfil) {
                $sql = "INSERT INTO workflow.estadodocumentoperfil (aedid, pflcod)
                          VALUES($aedid, {$perfil['pflcod']})";
                $db->executar($sql);
            }
            $db->commit();
        }
    }
    echo 'FIM';
}

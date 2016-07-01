<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
$uoEquipeTecnica = in_array(PFL_UO_EQUIPE_TECNICA, pegaPerfilGeral($_SESSION['usucpf'])) ? 1 : 0;
$where = '';
if(isset($_GET['unicod']) && $_GET['unicod'] != ''){
    $where = "AND uni.unicod = '". $_GET['unicod']."'";
}
if(isset($_GET['esdid']) && $_GET['esdid'] != ''){
    $where .= "AND esd.esdid = '". $_GET['esdid']."'";
}
$query = <<<DML
    SELECT
        pp.prpid,
        uni.unicod|| ' - '|| uni.unidsc AS unidade,
        SPLIT_PART(usu2.usunome, ' ', 1)||' '|| CASE WHEN LENGTH(SPLIT_PART(usu2.usunome, ' ', 2)) < 3 THEN SPLIT_PART(usu2.usunome, ' ', 2)||' '|| SPLIT_PART(usu2.usunome, ' ', 3) ELSE SPLIT_PART(usu2.usunome, ' ', 2) END AS usuario,
        SPLIT_PART(usu.usunome, ' ', 1)||' '|| CASE WHEN LENGTH(SPLIT_PART(usu.usunome, ' ', 2)) < 3 THEN SPLIT_PART(usu.usunome, ' ', 2)||' '|| SPLIT_PART(usu.usunome, ' ', 3) ELSE SPLIT_PART(usu.usunome, ' ', 2) END AS usuario2,
        TO_CHAR(pp.dataultimaatualizacao, 'dd/mm/yyyy') AS data,
        COALESCE(esd.esddsc,'Não Iniciado'),
        pp.prpid AS arquivo,
        usu.usucpf,
        {$uoEquipeTecnica} AS uo_equipe_tecnica
    FROM proporc.prelimites_pessoal pp
    INNER JOIN public.unidade uni ON (pp.unicod = uni.unicod)
    LEFT JOIN workflow.documento doc ON (pp.docid = doc.docid)
    LEFT JOIN workflow.estadodocumento esd ON (doc.esdid = esd.esdid)
    LEFT JOIN seguranca.usuario usu ON (pp.usucpfresponsavel = usu.usucpf)
    LEFT JOIN seguranca.usuario usu2 ON (pp.usucpf = usu2.usucpf)
    WHERE uni.unistatus = 'A'
        AND (uni.orgcod = '26000' OR uni.unicod IN('74902', '73107'))
        $where
        $whereUO
    order by uni.unicod
;
DML;
$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
$listagem->setCabecalho(array('Unidade', 'Usuário' , 'Responsável','Última Alteração','Status', 'Modelo Preenchido'));
$listagem->addCallbackDeCampo(array('unidade', 'usuario'), 'alinhaParaEsquerda');
$listagem->setQuery($query);
$listagem->esconderColunas(array('usucpf','uo_equipe_tecnica'));
$listagem->addAcao('edit', 'acessarPrelimite');
$listagem->addAcao('user', array('func' => 'modalAltUsuario','titulo' => 'Alterar Usuário Responsável','extra-params' => array('usucpf')));
$listagem->setAcaoComoCondicional('user', array(array('campo' => 'uo_equipe_tecnica','valor' => 0, 'op' => 'igual')));
$listagem->addCallbackDeCampo('arquivo', 'arquivoPreenchido');
$listagem->turnOnPesquisator();
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
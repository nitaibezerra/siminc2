<?php

if($prfid){
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    if($tipo == 'S'){
        $dados = $modeloAcompanhamento->listarSubacao($prfid);
        $cabecalhoTabela = array(
            'Subação',
            'Progresso'
        );
        $listagem->setDados($dados);
        $listagem->addAcao('edit2', 'acessarSubacao');
        $listagem->esconderColunas(array('esddsc','coordenador','situacao','prfid','usucpf'));
        $listagem->addCallbackDeCampo('progresso', 'callbackProgressBar');
    }else if($tipo == 'A'){
        $sql = <<<DML
            SELECT qa.qsaid, q.qstnome, pr.prftitulo, qa.unicod, qa.acacod
            FROM acomporc.questionarioacao qa
            JOIN acomporc.periodoreferencia pr ON pr.prfid = qa.prfid
            JOIN acomporc.questionario q ON q.qstid = qa.qstid
            WHERE pr.prsano = '{$_SESSION['exercicio']}'
DML;
        $cabecalhoTabela = array(
        'Questinário',
        'Período de Referência',
        'Unidade',
        'Ação'
        );
    }else if($tipo == 'T'){
        $dados = $modeloAcompanhamento->listarRelatorioTCU($prfid);
        $listagem->setDados($dados);
        $listagem->esconderColunas('esddsc');
        $listagem->addAcao('edit2', array('func' => 'acessarRelatorio','titulo' => 'Acessar Relatório TCU'));
        $listagem->addCallbackDeCampo('acatipo', 'formatarAcaTipo');
        $cabecalhoTabela = array(
            'Unidade',
            'Ação',
            'Tipo'
        );
    }

    $listagem->setCabecalho($cabecalhoTabela);
    $listagem->turnOnPesquisator();

    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}
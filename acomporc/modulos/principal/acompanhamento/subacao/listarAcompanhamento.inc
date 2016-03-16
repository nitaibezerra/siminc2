<?php
$perfis = pegaPerfilGeral();
if($prfid){
    $listagem = new Simec_Listagem();
    if($tipo == 'S'){
        $dados = $modeloAcompanhamento->listarSubacao($prfid);
        $cabecalhoTabela = array(
            'Subação',
            'Coordenador',
            'Estado do Documento',
            'Progresso'
        );
        $listagem->setDados($dados);
        $listagem->addCallbackDeCampo('progresso', 'callbackProgressBar');
        $listagem->addCallbackDeCampo('coordenador', 'apresentaUsuarioCPF');
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
        $sql = <<<DML
            SELECT qr.qrtid, q.qstnome, pr.prftitulo, qr.acacod
            FROM acomporc.questionariorelatoriotcu qr
            JOIN acomporc.periodoreferencia pr ON pr.prfid = qr.prfid
            JOIN acomporc.questionario q ON q.qstid = qr.qstid
            WHERE pr.prsano = '{$_SESSION['exercicio']}'
DML;
        $cabecalhoTabela = array(
        'Questinário',
        'Período de Referência',
        'Ação'
        );
    }

    $listagem->setCabecalho($cabecalhoTabela);
    $listagem->turnOnPesquisator();
    $listagem->setTitulo('Subações');
    $listagem->esconderColunas('prfid');
    $listagem->esconderColunas('usucpf');
    $listagem->setLarguraColuna(array('progresso' => '40%', 'coordenador' => '25%', 'sbacod' => '8%'));
    $listagem->addAcao('edit2', 'acessarSubacao');
    
     if (in_array(PFL_SUPERUSUARIO, $perfis) || in_array(PFL_CGP_GESTAO, $perfis)) {
        $listagem->addAcao('delete', 'apagarSubacao');
    }
    $listagem->addAcao('user', array('func' => 'alterarResponsavel','titulo' => 'Alterar Coordenador da Subação','extra-params'=>array('sbacod','prfid','usucpf')));
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}
?>
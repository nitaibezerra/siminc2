<?php
if($prfid){
    $listagem = new Simec_Listagem();
    global $db;
    if($tipo == 'S'){
        $sql = <<<DML
            SELECT qs.qstid, q.qstnome, pr.prftitulo, array_to_string(array_agg('<span class="label label-info">'||qs.sbacod||'</span>'),' ') as sbacod, pr.prfid
            FROM acomporc.questionariosubacao qs 
            JOIN acomporc.periodoreferencia pr ON pr.prfid = qs.prfid  
                JOIN acomporc.questionario q ON q.qstid = qs.qstid
            WHERE pr.prfid = '{$prfid}'
            GROUP BY qs.qstid, q.qstnome, pr.prftitulo, pr.prfid
DML;
        $dados = $db->carregar($sql);
        if(!$dados){
            $dados = array();
        }else{
            foreach($dados as $dado){
                $dado['sbacod'] = preg_replace('/[{}]/','',$dado['sbacod']);
            }
        }
        $listagem->setDados($dados);
        $listagem->esconderColunas('prfid');
        $cabecalhoTabela = array(
        'Questionário',
        'Período de Referência',
        'Subações'
        );

    }else if($tipo == 'A'){
        $sql = <<<DML
            SELECT qa.qstid, q.qstnome, pr.prftitulo, array_to_string(array_agg('<span class="label label-info">'||qa.unicod||' / '|| qa.acacod||'</span>'),' ') as acacod, pr.prfid
            FROM acomporc.questionarioacao qa
            JOIN acomporc.periodoreferencia pr ON pr.prfid = qa.prfid  
            JOIN acomporc.questionario q ON q.qstid = qa.qstid
            WHERE pr.prfid = '{$prfid}'
            GROUP BY qa.qstid, q.qstnome, pr.prftitulo, pr.prfid
DML;
        $dados = $db->carregar($sql);
        if(!$dados){
            $dados = array();
        }else{
            foreach($dados as $dado){
                $dado['acacod'] = preg_replace('/[{}]/','',$dado['acacod']);
            }
        }
        $listagem->setDados($dados);
        $listagem->esconderColunas('prfid');
        $cabecalhoTabela = array(
        'Questinário',
        'Período de Referência',
        'Unidades / Ações'
        );
    }else if($tipo == 'T'){
        $sql = <<<DML
            SELECT qr.qstid, q.qstnome, pr.prftitulo, array_to_string(array_agg('<span class="label label-info">'||qr.acacod||'</span>'),' ') as acacod, pr.prfid
            FROM acomporc.questionariorelatoriotcu qr
            JOIN acomporc.periodoreferencia pr ON pr.prfid = qr.prfid  
            JOIN acomporc.questionario q ON q.qstid = qr.qstid
            WHERE pr.prsano = '{$_SESSION['exercicio']}'
            GROUP BY qr.qstid, q.qstnome, pr.prftitulo, pr.prfid
DML;
        $dados = $db->carregar($sql);
        if(!$dados){
            $dados = array();
        }else{
            foreach($dados as $dado){
                $dado['acacod'] = preg_replace('/[{}]/','',$dado['acacod']);
            }
        }
        $listagem->setDados($dados);
        $listagem->esconderColunas('prfid');
        $cabecalhoTabela = array(
        'Questinário',
        'Período de Referência',
        'Ações'
        );
    }

    
    $listagem->setCabecalho($cabecalhoTabela);
    #$listagem->setQuery($sql);
    $listagem->turnOnPesquisator();
    #$listagem->addCallbackDeCampo('prftitulo','alinhaParaEsquerda');
    $listagem->addAcao('edit', array('func' => 'alterarAssociacao','extra-params' => array('prfid')));
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}else{
    
}
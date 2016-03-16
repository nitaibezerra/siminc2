<?php
$sql = <<<DML
    SELECT 
        p.prfid, 
        p.prftitulo,
        p.prfdescricao,
        TO_CHAR(p.prfinicio, 'DD/MM/YYYY') as prfinicio,
        TO_CHAR(p.prffim, 'DD/MM/YYYY') as prffim,
        TO_CHAR(p.prfpreenchimentoinicio, 'DD/MM/YYYY') as prfpreenchimentoinicio,
        TO_CHAR(p.prfpreenchimentofim, 'DD/MM/YYYY') as prfpreenchimentofim,
        to_char(prfcriacao, 'DD/MM/YYYY HH12:MI:SS') as prfcriacao
    FROM acomporc.periodoreferencia p
    WHERE p.prftipo = '{$tipo}'
    AND prsano = '{$_SESSION['exercicio']}'
    ORDER BY p.prfid DESC
DML;

$cabecalhoTabela = array(
    'Título',
    'Descrição',
    'Início da Vigência',
    'Fim da Vigência',
    'Início do Preenchimento',
    'Fim do Preenchimento',
    'Data de Criação'
);

$listagem = new Simec_Listagem();
$listagem->setCabecalho($cabecalhoTabela);
$listagem->setQuery($sql);
$listagem->turnOnPesquisator();
$listagem->addCallbackDeCampo('prfdescricao', 'alinhaParaEsquerda');
$listagem->addAcao('edit', array('func' => 'alterarPeriodo', 'titulo' => 'Editar Questionário'));
$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
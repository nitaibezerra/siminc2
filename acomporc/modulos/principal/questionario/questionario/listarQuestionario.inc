<?php
$cabecalhoTabela = array(
    'Nome',
    'Período de Referência'
);

$listagem = new Simec_Listagem();
$listagem->setCabecalho($cabecalhoTabela);
$listagem->setQuery($questionarioModelo->findAll(true));
$listagem->turnOnPesquisator();
$listagem->addCallbackDeCampo('qstnome', 'alinhaParaEsquerda');
$listagem->addAcao('edit', array('func' => 'alterarQuestionario', 'titulo' => 'Editar Questionário'));
$listagem->addAcao('edit2', array('func' => 'acessarPerguntas', 'titulo' => 'Acessar perguntas do questionário'));
$listagem->addAcao('delete', array('func' => 'deletarQuestionario', 'titulo' => 'Remover Questionário'));
$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
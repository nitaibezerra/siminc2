<?php
$sql = <<<DML
    SELECT 
        qp.qprid, 
        qp.qprpergunta,
        CASE qp.qprrespostapadrao::text WHEN '' THEN 'N/A' ELSE qp.qprrespostapadrao END as qprrespostapadrao,
        CASE qp.qprobrigatorio WHEN 't' THEN '<span class="label label-success">SIM</span>' ELSE '<span class="label label-danger">NÃO</span>' END as qprobrigatorio
    FROM acomporc.questionarioperguntas qp
    JOIN acomporc.questionario q ON q.qstid = qp.qstid
    WHERE q.qstid = {$questionario}
    ORDER BY qp.qprordem, qp.qprpergunta
DML;

$cabecalhoTabela = array(
    'Pergunta',
    'Resposta Padrão',
    'Obrigatório'
);

$listagem = new Simec_Listagem();
$listagem->setCabecalho($cabecalhoTabela);
$listagem->setQuery($sql);
$listagem->turnOnPesquisator();
$listagem->addCallbackDeCampo('qprpergunta','alinhaParaEsquerda');
$listagem->addCallbackDeCampo('qprrespostapadrao','alinhaParaEsquerda');
$listagem->addAcao('edit', array('func' => 'alterarQuestao','titulo' => 'Editar Pergunta'));
$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
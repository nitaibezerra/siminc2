<?php
/**
 * Formulário de cadastro e alteração de períodos de referência.
 *
 * $Id: formPeriodoreferencia.inc 97009 2015-05-07 20:07:34Z maykelbraz $
 */
if ($oPerRef) {
    $dados = $oPerRef->getDados();
}

$form = new Simec_View_Form('periodoreferencia');
$form->addHidden('prfid', $dados['prfid'])
    ->addInputTexto('prftitulo', $dados['prftitulo'], 'prftitulo', 250, false, array('flabel' => 'Título'))
    ->addInputTextarea('prfdescricao', $dados['prfdescricao'], 'prfdescricao', 3000, ['flabel' => 'Descrição'])
    ->addInputData(
        array('prfinicio', 'prffim'),
        array($dados['prfinicio'], $dados['prffim']),
        array('prfinicio', 'prffim'),
        array('flabel' => 'Período de validade')
    )->addInputData(
        array('prfpreenchimentoinicio', 'prfpreenchimentofim'),
        array($dados['prfpreenchimentoinicio'], $dados['prfpreenchimentofim']),
        array('prfpreenchimentoinicio', 'prfpreenchimentofim'),
        array('flabel' => 'Período de preenchimento')
    )->addBotoes(array('limpar', 'salvar'))
    ->setRequisicao('salvar')
    ->render();

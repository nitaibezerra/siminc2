<?php
/**
 * Layout de gestão de Fontes, GND e UOs.
 *
 * $Id: fgu.inc 98338 2015-06-08 20:47:48Z maykelbraz $
 */

$dspid = (int)$_REQUEST['dspid'];
$catDespesa = new Proporc_Model_Despesa();
list($dadosdespesa) = $catDespesa->recuperarTodos(
    't1.dspnome, t2.gdpnome',
    array("t1.dspid = {$dspid}"),
    '',
    array('join' => 'gdpid')
);

// -- painel
painelDetalheCategoriaDespesa($dadosdespesa);

$form = new Simec_View_Form('fgu');
$form->addHidden('dspid', $dspid);

$configInputFontes = array('multiple' => true, 'flabel' => 'Fonte', 'titulo' => 'Selecione as Fontes');
$sqlFontes = Proporc_Model_Despesafonterecurso::queryTodasAsFontes();
$sqlFontesSelecionadas = Proporc_Model_Despesafonterecurso::queryFontesSelecionadas($dspid);
$form->addInputCombo('foncod', $sqlFontes, $db->carregarColuna($sqlFontesSelecionadas), 'foncod', $configInputFontes);

$sqlGnd = Proporc_Model_Despesagnd::queryTodosOsGnds();
$sqlGndSelecionados = Proporc_Model_Despesagnd::queryGndsSelecionados($dspid);
$configInputGnd = array('multiple' => true, 'flabel' => 'GND', 'titulo' => 'Selecione os GNDs');
$form->addInputCombo('gndcod', $sqlGnd, $db->carregarColuna($sqlGndSelecionados), 'gndcod', $configInputGnd);

$sqlUnidades = Proporc_Model_Despesaunidadeorcamentaria::queryTodasAsUnidades();
$sqlUnidadesSelecionadas = Proporc_Model_Despesaunidadeorcamentaria::queryUnidadesSelecionadas($dspid);
$configInputUnidade = array(
    'multiple' => true,
    'flabel' => 'Unidade Orçamentária',
    'titulo' => 'Selecione as Unidades Orçamentárias'
);
$form->addInputCombo('unicod', $sqlUnidades, $db->carregarcoluna($sqlUnidadesSelecionadas), 'unicod', $configInputUnidade);
$form->addBotoes(array('salvar'))
    ->setRequisicao('salvarDespesaFGU');
$form->render();

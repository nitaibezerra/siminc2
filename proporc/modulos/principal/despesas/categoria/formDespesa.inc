<?php
/**
 * Formulário de cadastro de categorias de despesa.
 *
 * $Id: formDespesa.inc 101084 2015-08-10 21:41:06Z maykelbraz $
 */
global $grpDespesa;

$formGd = new Simec_View_Form('categoriadespesa');
$formGd->addHidden('dspid', null)
    ->setRequisicao('salvarCategoriadespesa');
$opcoes = array('join' => 'prfid', 'query' => true);
$where = array("t2.prsano = '{$_SESSION['exercicio']}'");
$query = $grpDespesa->recuperarTodosFormatoInput('gdpnome', $where, null, $opcoes);

$formGd->addCombo('Grupo de despesa', 'gdpid', $query);
unset($opcoes, $where, $query);

$opcoes['multiple'] = true;
$formGd->addTexto('Nome', 'dspnome', 255)
    ->addTexto('Sigla', 'dspsigla', 25)
    ->addMoeda('Montante', 'vlrmontante', array('valor' => '0,00'))
    ->addSeparador()
    ->addCombo('Fonte', 'foncod', Proporc_Model_Despesafonterecurso::queryTodasAsFontes(), $opcoes)
    ->addCombo('GND', 'gndcod', Proporc_Model_Despesagnd::queryTodosOsGnds(), $opcoes)
    ->addSeparador()
    ->addInputTextarea('dspobservacao', null, 'categoriadespesa_dspobservacao', 300, array('flabel' => 'Observações'))
    ->render();
unset($opcoes);

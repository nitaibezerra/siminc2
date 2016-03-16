<?php
/**
 * Formulário de filtros de categorias de despesa.
 *
 * $Id: formFiltros.inc 99791 2015-07-07 13:31:18Z maykelbraz $
 */
$gdpid = isset($filtrocategorias['gdpid'])?$filtrocategorias['gdpid']:$_REQUEST['gdpid'];
$form = new Simec_View_Form('filtrocategorias');
$form->addInputTexto('dspnome', $filtrocategorias['dspnome'], 'dspnome-filtro', 255, false, array('flabel' => 'Nome'))
    ->addInputTexto('dspsigla', $filtrocategorias['dspsigla'], 'dspsigla-filtro', 25, false, array('flabel' => 'Sigla'))
    ->addInputCombo(
        'gdpid',
        $grpDespesa->recuperarTodosFormatoInput('gdpnome', array("prfid = {$prfref->prfid}")),
        $gdpid,
        'gdpid-filtro',
        array('flabel' => 'Grupo de despesa')
    )->addBotoes(array('limpar', 'buscar', 'novo' => array('label' => 'Nova coluna')))
    ->setRequisicao('filtrarCategorias')
    ->render();

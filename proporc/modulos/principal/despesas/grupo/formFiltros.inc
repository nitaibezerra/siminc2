<?php
/**
 * Formulário de filtros de grupos de despesa.
 *
 * $Id: formFiltros.inc 98655 2015-06-16 13:52:40Z maykelbraz $
 */

$form = new Simec_View_Form('filtrogrupos');
$form->addInputTexto('gdpnome', $filtrogrupos['gdpnome'], 'gdpnome-filtro', 255, false, array('flabel' => 'Nome'))
    ->addBotoes(array('limpar', 'buscar', 'novo' => array('label' => 'Novo grupo')))
    ->setRequisicao('filtrarGrupos')
    ->render();

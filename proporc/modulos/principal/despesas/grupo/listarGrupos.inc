<?php
/**
 * Listagem de grupos de despesas cadastrados.
 *
 * $Id: listarGrupos.inc 99791 2015-07-07 13:31:18Z maykelbraz $
 */

$list = new Simec_Listagem();

$filtro = array('prfid' => $prfref->prfid);
$filtro = $filtrogrupos?array_merge($filtro, $filtrogrupos):$filtro;

$list->setQuery($grupoDespesa->carregarGruposDespesa($filtro, true))
    ->setCabecalho(array('Ordem', 'Nome'))
    ->turnOnPesquisator()
    ->addAcao('edit', 'editarGrupo')
    ->addAcao('list', 'listarDespesasDoGrupo')
    ->addAcao('delete', 'apagarGrupo')
    ->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

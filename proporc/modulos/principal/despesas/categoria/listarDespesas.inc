<?php
/**
 * Listagem de categorias de despesas cadastradas.
 *
 * $Id: listarDespesas.inc 99836 2015-07-07 19:13:14Z maykelbraz $
 */

$filtro = array("prfid = {$prfref->prfid}");
foreach ($filtrocategorias as $campo => $valor) {
    switch ($campo) {
        case 'dspsigla':
        case 'dspnome':
            $valor = "%{$valor}%";
            $filtro[] = sprintf("$campo ILIKE '%s'", $valor);
            break;
    }
}

if (!empty($gdpid)) {
    $filtro[] = sprintf("t2.gdpid = '%s'", $gdpid);
}
$list = new Simec_Listagem();
$config = array('query' => true, 'join' => 'gdpid');
$colunas = 't1.dspid, t2.gdpnome, t1.dspnome, t1.vlrmontante, t1.dspsigla';
$query = $catDespesa->recuperarTodos($colunas, $filtro, null, $config);
$list->setQuery($query)
    ->setCabecalho(array('Grupo', 'Coluna', 'Montante (R$)', 'Sigla'))
    ->turnOnPesquisator()
    ->addAcao('edit', 'editarCategoria')
    ->addAcao('check', array('func' => 'editarFgu', 'titulo' => 'Configurar Fontes, UOs, GNDs, Ações, Subações e POs.'))
    ->addAcao('delete', 'apagarCategoria')
    ->addCallbackDeCampo('vlrmontante', 'mascaraMoeda')
    ->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

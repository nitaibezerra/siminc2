<?php
if(!empty($mes)){
	$and = "AND cfp.cfpmes::integer = {$mes}";
}
$sql = <<<DML
	SELECT
            cfp.cfpano,
            cfp.cfpmes,
            cfp.cfptipobeneficio,
            cfp.cfptipofolha,
            cfp.cfporgao,
            cfp.cfpupag,
            cfp.cfpsgregime,
            cfp.cfpsituacao,
            cfp.cfpctrl,
            cfp.cfpcodigodespesa,
            cfp.cfprubrica,
            cfp.cfpvalor
        FROM
            progfin.cargacusteiofolhapagamento cfp
        WHERE cfpano::integer =  {$_SESSION['exercicio']}
           {$and}
        ORDER BY
            cfp.cfpano,
            cfp.cfpmes,
            cfp.cfptipobeneficio,
            cfp.cfptipofolha,
            cfp.cfporgao,
            cfp.cfpupag,
            cfp.cfpsgregime,
            cfp.cfpsituacao,
            cfp.cfpctrl,
            cfp.cfpcodigodespesa,
            cfp.cfprubrica,
            cfp.cfpvalor
DML;

$cabecalhoTabela = array(
    'Ano',
    'Mês',
    'Tipo Beneficio',
    'Tipo Folha',
    'Órgão',
    'Unidade Pagadora',
    'Regime',
    'Situação',
    'Controle',
    'Elemento Despesa',
    'Rubrica',
    'Valor'
);
echo "<br/>";
$listagem = new Simec_Listagem();
$listagem->setCabecalho($cabecalhoTabela);
$listagem->setQuery($sql);
$listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA);
//$listagem->totalizarColuna('cfpvalor');
$listagem->turnOnPesquisator();
$listagem->addCallbackDeCampo('cfpvalor', 'formataDinheiro');

if (false === $listagem->render()) {
    echo "Nenhum registro encontrado.";
}
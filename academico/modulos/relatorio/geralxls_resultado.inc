<?php

// Inclui componente de relatórios
include APPRAIZ . 'includes/classes/relatorio.class.inc';

include "funcoes_relatorio_infra.inc";
$chave = array_search('nomedaobra', $_REQUEST['agrupador']);
if ($chave) $_REQUEST['agrupador'][$chave] = "nomedaobraxls";
$chave = array_search('metragem', $_REQUEST['agrupador']);
if ($chave) $_REQUEST['agrupador'][$chave] = "metragemxls";
$chave = array_search('nivelpreenchimento', $_REQUEST['agrupador']);
if ($chave) $_REQUEST['agrupador'][$chave] = "nivelpreenchimentoxls";

$sql = query_relatorio_infra();
$agrupador = monta_agp_relatorio_infra();
$coluna = monta_coluna_relatorio_infra();

$dados = $db->carregar($sql);

$rel = new montaRelatorio();
$rel->setAgrupador($agrupador, $dados);
$rel->setColuna($coluna);
$rel->setTotNivel(true);

$nomeDoArquivoXls = "SIMEC_Relat" . date("YmdHis");

// Gera o XLS do relatório

ob_clean();

echo $rel->getRelatorioXls();
die;



?>

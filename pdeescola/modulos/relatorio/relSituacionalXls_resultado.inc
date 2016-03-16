<?php

ini_set("memory_limit", "1024M");

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql       = pdeescola_situacional_monta_sql_relatorio();
$agrupador = pdeescola_situacional_monta_agp_relatorio();
$coluna    = pdeescola_situacional_monta_coluna_relatorio();

$dados = $db->carregar( $sql );

$rel = new montaRelatorio();
$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTotNivel(true);

$nomeDoArquivoXls = "SIMEC_Relat".date("YmdHis");
echo $rel->getRelatorioXls();
 
?>

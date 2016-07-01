<?php

ini_set("memory_limit", "1024M");

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

/* Adaptando para retirar código HTML, pois o agrupador 
 * nomedaobra contem código HTML (<a ...>)
 * */
$chave = array_search('nomedaescola', $_REQUEST['agrupador']);
$_REQUEST['agrupador'][$chave]="nomedaescolaxls";
/* FIM Adaptando para retirar código HTML, pois o agrupador 
 * nomedaobra contem código HTML (<a ...>)
 * */

$sql       = pdeescola_monta_sql_relatorio();
$agrupador = pdeescola_monta_agp_relatorio();
$coluna    = pdeescola_monta_coluna_relatorio();

$dados = $db->carregar( $sql );

$rel = new montaRelatorio();
$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTotNivel(true);

$nomeDoArquivoXls = "SIMEC_Relat".date("YmdHis");
echo $rel->getRelatorioXls();
 
?>

<?php
ini_set("memory_limit", "1024M");
set_time_limit(30000);

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql = monta_sql();
$dados = $db->carregar($sql);

$arLabels = retornaColunasELabels();
$arColunas = array_keys($arLabels);
$arCabecalho = array('Descrição');
$colXls = array('descricao');

// -- Montando o cabeçalho do XLS - adicionando agrupadores ao cabeçalho
foreach ($_POST['agrupador'] as $agp) {
    if (in_array($agp, $arColunas)) {
        array_push($arCabecalho, $arLabels[$agp]);
        array_push($colXls, $agp);
    }
}
// -- Montando o cabeçalho do XLS - adicionando as colunas escolhidas ao cabeçalho
foreach (monta_coluna() as $cabecalho) {
    array_push($arCabecalho, $cabecalho['label']);
    array_push($colXls, $cabecalho['campo']);
}

header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
header("Pragma: no-cache");
header("Content-type: application/xls; name=SIMEC_ProgOrcamentaria_RelatorioGeral_".date("Ymdhis").".xls");
header("Content-Disposition: attachment; filename=SIMEC_ProgOrcamentaria_RelatorioGeral_".date("Ymdhis").".xls");
header("Content-Description: MID Gera excel");

$db->monta_lista_tabulado($dados,$arCabecalho,100000,5,'N','100%','');
die;

?>

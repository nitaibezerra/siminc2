<?php 

ini_set("memory_limit", "3024M");
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '147';


$db = new cls_banco();

/**
 * Arquivo executado 3 vezes ao dia: 07h, 13h, 18h
 */


// Obras 2.0
$db->executar("REFRESH MATERIALIZED VIEW obras2.vm_obras_situacao_estadual;");
$db->executar("REFRESH MATERIALIZED VIEW obras2.vm_obras_situacao_municipal;");
$db->executar("REFRESH MATERIALIZED VIEW obras2.vm_termo_obras;");
$db->executar("REFRESH MATERIALIZED VIEW obras2.vm_financeiro_obras;");
$db->executar("REFRESH MATERIALIZED VIEW obras2.vm_fisico_financeiro_processo;");
$db->executar("REFRESH MATERIALIZED VIEW obras2.vm_pendencia_obras;");
$db->executar("REFRESH MATERIALIZED VIEW obras2.vm_termo_convenio_obras;");
$db->executar("REFRESH MATERIALIZED VIEW obras2.vm_total_pendencias;");
$db->executar("REFRESH MATERIALIZED VIEW obras2.vm_vigencia_obra;");

$db->commit();

echo "Executado.";

?>
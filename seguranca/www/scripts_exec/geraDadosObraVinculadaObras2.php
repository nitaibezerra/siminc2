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


include_once APPRAIZ . 'www/obras2/_constantes.php';
include_once APPRAIZ . 'www/obras2/_funcoes.php';
include_once APPRAIZ . 'www/obras2/_componentes.php';
include_once APPRAIZ . "www/autoload.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";

include_once APPRAIZ . "includes/classes/modelo/obras2/RegistroAtividade.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Restricao.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/FilaRestricao.class.inc";


$sql = "SELECT * from obras2.obras where obrid IN(SELECT v.obridvinculado FROM obras2.obras v WHERE v.obrstatus = 'P' AND v.obridvinculado IS NOT NULL)";
$obrasComVinculacao = $db->carregar($sql);
if($obrasComVinculacao){
    foreach($obrasComVinculacao as $obr){
        echo 'Pega da obra: ' . $obr['obrid'] . ' atualiza pegando do vinculo: ' . $obr['obridvinculado'] . '<br />';
        migraDadosObraVinculada($obr['obrid'], $obr['obridvinculado']);
    }
}
exit;

?>

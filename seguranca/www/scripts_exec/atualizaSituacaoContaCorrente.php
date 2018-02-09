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


include_once APPRAIZ . "www/autoload.php";

include_once APPRAIZ . "execucaofinanceira/classes/Processo.class.inc";
include_once APPRAIZ . "execucaofinanceira/classes/ContaCorrente.class.inc";


$wsusuario	= 'USAP_WS_SIGARP';
$wssenha	= '03422625';

$cc = new ContaCorrente();


$sql = "
            SELECT proid, pronumeroprocesso, tipo FROM (

                SELECT proid, proseqconta, pronumeroprocesso, 'par' as tipo FROM par.processoobraspar
                UNION
                SELECT proid, proseqconta, pronumeroprocesso, 'pac' as tipo FROM par.processoobra

            ) as p
";


$processos = $db->carregar($sql);


$update = '';
foreach ($processos as $processo){
    $situacao = $cc->consultarSituacaoContaCorrentePorProcesso( $wsusuario, $wssenha, $processo['pronumeroprocesso']);

    if(is_int($situacao)){
        $tabela = ($processo['tipo'] == 'par') ? 'processoobraspar' : 'processoobra';

        $update .= "UPDATE par.$tabela SET prosituacaoconta = $situacao WHERE proid = {$processo['proid']};";
    }

    echo 'Processo: ' . $processo['pronumeroprocesso'] . ' - Situação: ' . $situacao . '<br />';
}

$db->executar($update);
$db->commit();

echo 'Atualização completa';




?>

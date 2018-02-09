<?php

ini_set("memory_limit", "3024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = "simec_espelho_producao"; //simec_desenvolvimento
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
include_once APPRAIZ . "includes/classes/modelo/obras2/Restricao.class.inc";
require_once APPRAIZ . "includes/classes/dateTime.inc";
require_once APPRAIZ . "includes/classes/modelo/obras2/Email.class.inc";
require_once APPRAIZ . "includes/classes/modelo/obras2/DestinatarioEmail.class.inc";



if($_REQUEST['atualizar'] == 'true'){
    acionaComando();
    die();
}

function acionaComando()
{
    $obj  = new Restricao();
    
    $tempo_inicio = time();
    
    $resp = $obj->atualizarRestricaoInconformidadeRemovendoTagsHtmlDosCampos(true);
    
    $countErro = $resp['qtd_erros'];
    $countOk   = $resp['qtd_ok'] ;
    
    echo '<pre>';
    echo '<h1>Restrições/Inconformidades Ativas e Inativas</h1>';
    var_dump($resp);
    echo '<h1>TOTAL de Restrições/Inconformidades alteradas</h1>'. (count($resp)-2).'<br><br><br>';
//    var_dump($resp);
    if($countErro > 0){
        echo '<h1>QTD de Restrições/Inconformidades que seriam atualizadas</h1>'. $countOk;
        echo '<h1>QTD de Restrições/Inconformidades que não seriam atualizadas</h1>'. $countErro;
        echo '<h1>As Restrições/Inconformidades não foram  atualizadas pois ocorreu um erro.</h1>';
    }else{
        echo '<h1>QTD de Restrições/Inconformidades que foram atualizadas</h1>'. $countOk;
    }
    echo '</pre>';
    
    $tempo_fim    = time();
    
    $duracaoS = $tempo_fim - $tempo_inicio;
    $duracaoM = ($duracaoS/60);
    
    echo '<h2>TEMPO DE EXECUÇÃO: </h2> Segundos: '.$duracaoS.'<br> Minutos: '.$duracaoM;
    
    
}

?>
<style>
    td {
        border: 1px solid #FF0000;
    }
</style>
<h2>Total de Restrições/Inconformidades</h2>
<?php

$obj    = new Restricao();
$totais = $obj->atualizarRestricaoInconformidadeRemovendoTagsHtmlDosCampos(false);
echo '<pre>';
var_dump($totais);
echo '</pre>';

///www/scripts_exec/obras2_atualizaDocidRestricaoInconformidade.php

/**
 * RESULTADO:
 * 
 * Restrições/Inconformidades Ativas e Inativas

array(3804) {
TOTAL de Restrições/Inconformidades alteradas

3802


QTD de Restrições/Inconformidades que foram atualizadas

3802
TEMPO DE EXECUÇÃO:

Segundos: 5
Minutos: 0.0833333333333
 * 
 * 
 * 
 */



?>

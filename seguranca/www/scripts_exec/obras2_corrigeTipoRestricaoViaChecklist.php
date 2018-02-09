<?php

//http://simec-local/seguranca/scripts_exec/obras2_corrigeTipoRestricaoViaChecklist.php?atualizar=true

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

function getArrayDescs(){
    $arrDados[1]['descricao']    = 'O contrato encontra-se vencido.';
    $arrDados[2]['descricao']    = 'Falta inserir aditivo de prazo para que o contrato permaneça vigente.';
    $arrDados[3]['descricao']    = 'Falta inserir Ordem de Serviço';
    $arrDados[4]['descricao']    = ' As informações digitadas na aba contratação estão em desacordo com os dados da Ordem de serviço anexada.';
    $arrDados[5]['descricao']    = 'Falta inserir planilha contratada (assinada) na aba contratação, com valor total igual ao contratado.';
    $arrDados[6]['descricao']    = 'O valor total da planilha está em desacordo com o valor digitado na aba contratação.';
    $arrDados[7]['descricao']    = 'Falta ART/RRT de execução.';
    $arrDados[8]['descricao']    = 'Os dados constantes na ART/RRT de execução estão em desacordo com as informações da empresa cadastrada na aba contratação';
    $arrDados[9]['descricao']    = 'Falta ART/RRT de fiscalização.';
    $arrDados[10]['descricao']   = 'Os dados constantes na ART/RRT de fiscalização estão em desacordo com os dados do fiscal (engenheiro/arquiteto) vinculado à obra.';
    $arrDados[11]['descricao']   = 'Falta dados de pagamentos e medições na aba execução orçamentária.';
    $arrDados[12]['descricao']   = 'Os dados das notas fiscais estão em desacordo com as informações da empresa contratada declarados na aba contratação. ';
    $arrDados[13]['descricao']   = 'Os lançamentos de pagamento estão desatualizados.';
    $arrDados[14]['descricao']   = 'Falta inserir Boletins de Medição.';
    $arrDados[15]['descricao']   = 'O contrato vencerá nos próximos 30 dias.';
    return $arrDados;
}

function getIdsAtualizar(){
    global $db;
    
    $arrDados = getArrayDescs();
    $arrIds = array();
    
    foreach ($arrDados as $value) {
        
        $sql   = " SELECT rstid FROM obras2.restricao WHERE rstdsc like '%".$value['descricao']."%' AND tprid  = 18; ";
        $lista = $db->carregar($sql);
        
        if(is_array($lista)){
            foreach ($lista as $v) {
                $arrIds[] = $v['rstid'];
            }
        }
        
    }
    return $arrIds;
}




function acionaComando(){
    global $db;
    $tprid  = 19;
    
    $arrIds = getIdsAtualizar();
    $erros = array();
    $c = 0;
    foreach ($arrIds as $value) {
        $sql = "UPDATE obras2.restricao SET tprid = ".$tprid."  WHERE rstid = ".$value;
        try{
            $db->executar($sql);
            $c++;
        } catch (Exception $ex) {
           $erros[] = '<p>Erros em rstid = '.$value.'<br>'.$ex->getMessage().'</p>';  
        }
    }
    
    if(empty($erros)){
        $db->commit();
        echo '<h2>Foram atualizados '.$c.' registros com os seguintes IDS:</h2>';
        echo '<pre>';
        var_dump($arrIds);
        echo '</pre>';
        
    }else{
        echo '<pre>';
        var_dump($erros);
        echo '</pre>';
    }
}

?>
<style>
    td {
        border: 1px solid #FF0000;
    }
</style>
<h2>Mensagens Restrições/Inconformidades</h2>
<pre>
<?php var_dump(getArrayDescs())?>
</pre>


<h2>Ids de Restrições/Inconformidades a atualizar</h2>
<pre>
<?php var_dump(getIdsAtualizar())?>
</pre>
<?php




/**
 * Dados Modificados
 * Foram atualizados 104 registros com os seguintes IDS:

array(104) {
  [0]=>
  string(5) "40052"
  [1]=>
  string(5) "40057"
  [2]=>
  string(5) "40062"
  [3]=>
  string(5) "40419"
  [4]=>
  string(5) "40420"
  [5]=>
  string(5) "40421"
  [6]=>
  string(5) "40422"
  [7]=>
  string(5) "40066"
  [8]=>
  string(5) "40074"
  [9]=>
  string(5) "40080"
  [10]=>
  string(5) "40086"
  [11]=>
  string(5) "40088"
  [12]=>
  string(5) "40094"
  [13]=>
  string(5) "40095"
  [14]=>
  string(5) "40103"
  [15]=>
  string(5) "40105"
  [16]=>
  string(5) "40108"
  [17]=>
  string(5) "39984"
  [18]=>
  string(5) "40196"
  [19]=>
  string(5) "40200"
  [20]=>
  string(5) "40205"
  [21]=>
  string(5) "40209"
  [22]=>
  string(5) "40213"
  [23]=>
  string(5) "40217"
  [24]=>
  string(5) "40222"
  [25]=>
  string(5) "40230"
  [26]=>
  string(5) "40311"
  [27]=>
  string(5) "40586"
  [28]=>
  string(5) "40587"
  [29]=>
  string(5) "40061"
  [30]=>
  string(5) "40065"
  [31]=>
  string(5) "40073"
  [32]=>
  string(5) "40079"
  [33]=>
  string(5) "40085"
  [34]=>
  string(5) "40093"
  [35]=>
  string(5) "40102"
  [36]=>
  string(5) "40107"
  [37]=>
  string(5) "40195"
  [38]=>
  string(5) "40199"
  [39]=>
  string(5) "40204"
  [40]=>
  string(5) "40216"
  [41]=>
  string(5) "40221"
  [42]=>
  string(5) "40229"
  [43]=>
  string(5) "40056"
  [44]=>
  string(5) "40228"
  [45]=>
  string(5) "40186"
  [46]=>
  string(5) "40185"
  [47]=>
  string(5) "40227"
  [48]=>
  string(5) "40072"
  [49]=>
  string(5) "40084"
  [50]=>
  string(5) "40049"
  [51]=>
  string(5) "40064"
  [52]=>
  string(5) "40068"
  [53]=>
  string(5) "40078"
  [54]=>
  string(5) "40092"
  [55]=>
  string(5) "40184"
  [56]=>
  string(5) "40192"
  [57]=>
  string(5) "40203"
  [58]=>
  string(5) "40226"
  [59]=>
  string(5) "40055"
  [60]=>
  string(5) "40048"
  [61]=>
  string(5) "40063"
  [62]=>
  string(5) "40071"
  [63]=>
  string(5) "40091"
  [64]=>
  string(5) "40183"
  [65]=>
  string(5) "40188"
  [66]=>
  string(5) "40191"
  [67]=>
  string(5) "40202"
  [68]=>
  string(5) "40220"
  [69]=>
  string(5) "40225"
  [70]=>
  string(5) "40060"
  [71]=>
  string(5) "40083"
  [72]=>
  string(5) "40047"
  [73]=>
  string(5) "40082"
  [74]=>
  string(5) "40182"
  [75]=>
  string(5) "40190"
  [76]=>
  string(5) "40219"
  [77]=>
  string(5) "40224"
  [78]=>
  string(5) "40234"
  [79]=>
  string(5) "40168"
  [80]=>
  string(5) "40067"
  [81]=>
  string(5) "40070"
  [82]=>
  string(5) "40075"
  [83]=>
  string(5) "40077"
  [84]=>
  string(5) "40090"
  [85]=>
  string(5) "40194"
  [86]=>
  string(5) "40198"
  [87]=>
  string(5) "40201"
  [88]=>
  string(5) "40206"
  [89]=>
  string(5) "40046"
  [90]=>
  string(5) "40081"
  [91]=>
  string(5) "40089"
  [92]=>
  string(5) "40181"
  [93]=>
  string(5) "40187"
  [94]=>
  string(5) "40189"
  [95]=>
  string(5) "40218"
  [96]=>
  string(5) "40223"
  [97]=>
  string(5) "40231"
  [98]=>
  string(5) "40233"
  [99]=>
  string(5) "40069"
  [100]=>
  string(5) "40076"
  [101]=>
  string(5) "40193"
  [102]=>
  string(5) "40207"
  [103]=>
  string(5) "40215"
}

 */

?>

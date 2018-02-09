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
include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Obras.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Cronograma_PadraoMi.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/OrdemServicoMI.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Itens_Composicao_PadraoMi.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/QtdItensComposicaoObraMi.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/ItensComposicaoObras.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Cronograma.class.inc";

try{
    if(!empty($_GET['obrid'])){

        $ids = explode(',', $_GET['obrid']);

        foreach($ids as $obrid)
        {
            $obra = new Obras($obrid);
            $obra->exportarCronogramaPadraoParaObra();
            echo '<h1>Executado ID: '.$obrid.'</h1>';
        }

    } else {
        echo "Obrid não foi passado";
    }
} catch (Exception $ex) {
    echo '<h1>Não foi possível atualizar o cronograma da obra. Erro: </h1>';
    echo '<pre>';
    var_dump($ex);
    echo '</pre>';
}


?>
<?php 
ini_set('memory_limit', '3024M');
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../'));

$_REQUEST['baselogin']  = 'simec_espelho_producao';//simec_desenvolvimento
// carrega as funções gerais
require_once BASE_PATH_SIMEC . '/global/config.inc';
require_once APPRAIZ . 'includes/classes_simec.inc';
require_once APPRAIZ . 'includes/funcoes.inc';
include_once APPRAIZ . 'includes/human_gateway_client_api/HumanClientMain.php';

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '147';

$db = new cls_banco();

include_once APPRAIZ . 'www/obras2/_constantes.php';
include_once APPRAIZ . 'www/obras2/_funcoes.php';
include_once APPRAIZ . 'www/obras2/_componentes.php';
include_once APPRAIZ . 'www/autoload.php';
include_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . 'includes/classes/modelo/obras2/Obras.class.inc';
include_once APPRAIZ . 'includes/classes/modelo/obras2/Cronograma_PadraoMi.class.inc';
include_once APPRAIZ . 'includes/classes/modelo/obras2/OrdemServicoMI.class.inc';
include_once APPRAIZ . 'includes/classes/modelo/obras2/Itens_Composicao_PadraoMi.class.inc';
include_once APPRAIZ . 'includes/classes/modelo/obras2/QtdItensComposicaoObraMi.class.inc';
include_once APPRAIZ . 'includes/classes/modelo/obras2/ItensComposicaoObras.class.inc';

$strSQL = "SELECT DISTINCT 
        o.obrid, po.preid, p.pagid, p.pagdatapagamento, d.esdid
    FROM obras2.obras  o
        JOIN workflow.documento d ON d.docid = o.docid
        JOIN par.pagamentoobra po ON po.preid = o.preid
        JOIN par.pagamento p ON p.pagid = po.pagid AND pagstatus = 'A' AND p.pagsituacaopagamento = '2 - EFETIVADO'
    WHERE d.esdid = 762";

$rs = $db->carregar($strSQL);

if ($rs) {
    foreach ($rs as $row) {
        $obra = new Obras($row['obrid']);
        $estado = wf_pegarEstadoAtual($obra->docid);
        if ($estado['esdid'] == 762 && wf_alterarEstado($obra->docid, 1782, '', array('obrid' => $obra->obrid))) {
            //echo 'Ação executada obra: ' . $obra->obrid . '<br />';
        } else {
            //echo 'Erro no Workflow: ' . $obra->obrid . ' <br />';
        }
    }
}
$db->commit();
//echo 'SUCESSO! <br />';
//exit;
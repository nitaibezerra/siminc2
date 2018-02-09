<?php
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$obras = array();

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
// $_REQUEST['baselogin']  = "simec_desenvolvimento";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
// require_once "../../global/config.inc";

require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/RegistroAtividade.class.inc";
include_once APPRAIZ . "includes/classes/Sms.class.inc";

//eduardo - envio SMS pendecias de obras - PAR
//http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 98;

/****************************************
*				PREFEITO				*
****************************************/

$conteudo = "Prezado Diretor, sua escola já enviou texto para a Olimpíada de Língua Portuguesa? O prazo é 15/08! Acesse www.escrevendoofuturo.org.br";

$contador = 3;
$limit = 5000;
do{
    $offset = $limit * $contador;
    $dados = getDados($limit, $offset);
    $contador++;
    if(is_array($dados)){
        enviarNotificacoes($dados, $conteudo);
    }
} while (is_array($dados));

echo "FIM";
die;


function getDados($limit, $offset)
{
	$db = new cls_banco();

    $sql = "select celular
            from seguranca.sms_envio_unico
            limit $limit
            offset $offset
            ";

 	return $db->carregar($sql);
}

function enviarNotificacoes($dados, $conteudo)
{
	$db = new cls_banco();

    $aCelularEnvio = array();
		$aCelularEnvio = array(
			'556191434894', // Daniel
 			'556181054537', // Orion
		);
	foreach ($dados as $contato) {
		$aCelularEnvio[] = 55 . str_replace(array(' ', '-'), array('', ''), $contato['celular']);
	}
	
	$aCelularEnvio = array_unique($aCelularEnvio);

    $data = date('Y-m-d') . ' 08:00:00';

    $sms = new Sms();
    $sms->enviarSms($aCelularEnvio, $conteudo, $data);
}

?>
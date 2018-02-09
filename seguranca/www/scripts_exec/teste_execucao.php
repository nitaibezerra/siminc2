<?php
ini_set( 'display_errors', 1 );
ini_set("memory_limit", "4024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = "simec_espelho_producao"; //simec_desenvolvimento
// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '23';


$db = new cls_banco();

$sql = "select distinct
			emp.empid
		from
			par.processoobra pro
			inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
		where
			pro.prostatus = 'A'
			and emp.empstatus = 'A'
		order by emp.empid";

$arrEmp = $db->carregarColuna($sql);
$arrEmp = $arrEmp ? $arrEmp :array();

$qtdEmp = 1000;
$contador = $qtdEmp;
$strEmpenho = '';
$limitEmp = array();

foreach ($arrEmp as $key => $empid) {
	if( empty($strEmpenho) ){
		$strEmpenho = $empid;
	} else {
		$strEmpenho = $strEmpenho . ', ' . $empid;
	}
	
	if( ((int)$contador - 1) == $key ){
		array_push($limitEmp, $strEmpenho);
		$strEmpenho = '';
		$contador = $contador + $qtdEmp;
	}
	if( $key == sizeof($arrEmp)-1 && !empty($strEmpenho) ){
		array_push($limitEmp, $strEmpenho);
		$strEmpenho = '';
		$contador = 0;
	}
}

$totalUrl = sizeof($limitEmp);

//ver($limitEmp);
$db->executar("delete from par.empenho_temp where sistema = 'PAC'");
$db->commit();

foreach ($limitEmp as $key => $empid) {
	$sql = "insert into par.empenho_temp(codigo, empenho, quantidade, sistema)
			values($key, '$empid', ".sizeof(explode(', ', $empid)).", 'PAC')";
	$db->executar($sql);
}
$db->commit();

$arUrls = array();

$strUrl = '';
for ($i=0; $i<$totalUrl; $i++){
	$urls = $_SERVER['SERVER_NAME'].':'.$_SERVER['SERVER_PORT'].'/seguranca/scripts_exec/par_atualiza_empenho_sigef.php?sistema=PAC&cont='.$i;
	array_push($arUrls, $urls);
	$strUrl = $strUrl .'<br>'.$urls; 
}

$strAssunto = "Teste Wesley";
$strMensagem = $strUrl;
$remetente = array("nome"=>SIGLA_SISTEMA. " - VIEW_PAR", "email"=>"noreply@mec.gov.br");
$strEmailTo = array($_SESSION['email_sistema']);
$retorno =  enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem, '', '', array() );

$db->close();
//ver($msg, 'FIM', d);


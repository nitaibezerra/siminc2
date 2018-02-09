<?php
set_time_limit(100000);
ini_set("memory_limit", "10000M");

// carrega as funções gerais
define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );
require_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

include_once APPRAIZ . 'includes/classes/Modelo.class.inc';

include_once APPRAIZ . 'maismedicos/classes/Ws_Tutor.class.inc';
include_once APPRAIZ . 'maismedicos/classes/Ws_Planotrabalho.class.inc';
include_once APPRAIZ . 'maismedicos/classes/Ws_Planotrabalho_Itens.class.inc';

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '00000000191';
	
// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$ws_tutor = new Ws_Tutor();
$msg = $ws_tutor->atualizaPlanoTrabalhoMaisMedicos();

/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "WS Mais Médicos";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress("wescley.lima@ebserh.gov.br", "Wescley Lima");
$mensagem->AddAddress("henrique.couto@ebserh.gov.br", "Henrique Couto");
$mensagem->Subject = "Atualização da Vinculação de Instituições/Município do Mais Médicos";
$corpoemail = 'Atualização dos Planos de Trabalho do Mais Médicos pelo WS.<br/></br>'.$msg;

$mensagem->Body = $corpoemail;
$mensagem->IsHTML( true );
$mensagem->Send();
// dbg($msg);
print $msg;
/*
 * FIM
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
?>
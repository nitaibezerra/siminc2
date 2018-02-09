<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "/var/www/simec/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . 'includes/classes/entidadeFNDE.class.inc';

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '00000000191';
	
// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "SELECT eb.enbid, eb.enbcnpj, eb.enbano
		FROM emenda.entidadebeneficiada eb 
		where eb.enbcnpj is not null
		order by eb.enbcnpj";
$arEntidade = $db->carregar( $sql );

$obEntidade = new entidadeFNDE();
$obEntidade = $obEntidade ? $obEntidade : array();

foreach ($arEntidade as $v) {
	$obEntidadeDados  = $obEntidade->buscaEntidadeBaseFNDE_WS( $v['enbcnpj'] );	
	if( is_object($obEntidadeDados) ){
		$obEntidadeDados->enbid = $v['enbid'];
		$obEntidade->atualizaEntidadeBeneficiada( $obEntidadeDados );
	}
}

/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "Atualiza Entidades Emenda";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress($_SESSION['email_sistema']);
$mensagem->Subject = "Atualiza Entidades Emenda";
$corpoemail = 'Script Entidade Emenda Atualizado com Sucesso.';

$mensagem->Body = $corpoemail;
$mensagem->IsHTML( true );
$mensagem->Send();
/*
 * FIM
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */

$db->close();

?>
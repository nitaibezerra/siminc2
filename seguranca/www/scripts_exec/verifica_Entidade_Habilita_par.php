
<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once "/var/www/simec/global/config.inc";
//include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "/includes/classes/Fnde_Webservice_Client.class.inc";
include_once APPRAIZ . "par/classes/Habilita.class.inc";

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);}


$db = new cls_banco();

session_start();

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

$Tinicio = getmicrotime();

$wsusuario 	= 'mecromeu';
$wssenha	= '16101974';

$sql = "SELECT count(iuecnpj) / 100  FROM par.instrumentounidadeentidade WHERE iuestatus = 'A' and iuecnpj is not null";
$totalRegistro = $db->pegaUm($sql);

$obHabilita = new Habilita();

$sql = "UPDATE par.instrumentounidadeentidade SET iueenviado = false WHERE iuestatus = 'A' and iuecnpj is not null";
$db->executar($sql);
$db->commit();
$count = 0;

for($i=0; $i<=$totalRegistro; $i++) {
	
	$sql = "SELECT DISTINCT iuecnpj, iuesituacaohabilita FROM par.instrumentounidadeentidade WHERE iuestatus = 'A' and iuecnpj is not null and iueenviado = false limit 100";
	$arrEntidade = $db->carregar($sql);
	$arrEntidade = $arrEntidade ? $arrEntidade : array();
	
	foreach ($arrEntidade as $entidade) {
		$count++;
		$habilitado = $obHabilita->consultaHabilitaEntidade($entidade['iuecnpj'], true);
		$habilitado = json_decode($habilitado);
		$habilitado = utf8_decode($habilitado->descricao);
		
		$sql = "UPDATE par.instrumentounidadeentidade SET
	  				iuesituacaohabilita = '{$habilitado}',
	  				iueenviado = true
	  			WHERE
	  				iuecnpj = '{$entidade['iuecnpj']}'";
	  	
	  	$db->executar($sql);
	  	$db->commit();
	}
	  	
	/*foreach ($arrEntidade as $entidade) {

		$habilitado = $obHabilita->consultaHabilitaEntidade($entidade['iuecnpj'], true);
		$habilitado = json_decode($habilitado);
		$habilitado = utf8_decode($habilitado->descricao);
		$totalRegistro++;
		 //if( $habilitado != $entidade['iuesituacaohabilita'] ){
		$sql = "UPDATE par.instrumentounidadeentidade SET
	  				iuesituacaohabilita = '{$habilitado}',
	  				iueenviado = true
	  			WHERE
	  				iuecnpj = '{$entidade['iuecnpj']}'";
	  	
	  	$db->executar($sql);
	  	$db->commit();
		//}	
	}*/
}
//$Tfinal = date("d/m/Y H:i:s");
$Tfinal = getmicrotime() - $Tinicio;

$html = '<table class="listagem" cellspacing="0" cellpadding="2" border="0" align="center" width="95%">
			<tbody>
			<tr bgcolor="#ffffff">
				<td>A verificação das entidades no sistema de habilita foram realizados com sucesso! '.date("d/m/Y h:i:s").'</td>
			</tr>
			<tr bgcolor="#ffffff">
				<td>O tempo de execução das atualizações foi de '.$Tfinal.' segundos</td>
			</tr>
			<tr bgcolor="#ffffff">
				<td><b>Total de Registros Processado: '.$count.'</b></td>
			</tr>
			</tbody>
		  </table>';

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "SCRIPT AUTOMATICO";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA );
$mensagem->Subject = "Atualização do PAR - Habilita Entidade";
//$mensagem->Body = $html;
$mensagem->Body = "<p>A verificação das entidades no sistema de habilita foram realizados com sucesso! ".date("d/m/Y h:i:s")."</p>
				   <p>O tempo de execução das atualizações foi de ".$Tfinal." segundos</p>
				   <p>Total de Registros Processado: ".$count."</p>";

$mensagem->IsHTML( true );
$mensagem->Send();
?>
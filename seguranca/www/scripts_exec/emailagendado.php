<?php
/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações */

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . 'www/painel/_constantes.php';
include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';

$e = new EmailAgendado();
$e->setTitle("SCDP - ALERTA - SOLICITAÇAO DE DIÁRIAS E PASSAGENS");

$html = "<span style=\"font-weight:bold\" >Prezados(as) Senhores(as),</span><br /><br />
<center>
<div style=\"font-size:22px;font-weight:bold\" >
Hoje é o último dia para solicitar a emissão de passagens e diárias para o dia ".date("d/m",mktime(0, 0, 0, date("m")  , date("d")+10, date("Y"))).".<br />
Fora deste prazo, somente será autorizada a solicitação mediante justificativa escrita encaminhada por meio de Memorando ou Ofício a <br />
Chefia de Gabinete da Secretaria Executiva.<br />
Informamos também que não será autorizada a emissão com pendências de prestação de contas.<br /><br />
</div>
<span style=\"font-weight:bold\" >
Secretaria Executiva<br />
MEC<br />
</span></center>";
echo $html;
$e->setText($html);
$e->setEmailOrigem("no-reply@mec.gov.br");
//$e->setEmailToReply($_SESSION['email_sistema']);
$e->setName("SCDP");
//$e->setEmailsDestinoPorArquivo(APPRAIZ . 'www/painel/email.txt');
//$e->setEmailsDestino(array($_SESSION['email_sistema']));
$e->setEmailsDestino(array($_SESSION['email_sistema']));
$e->enviarEmails();
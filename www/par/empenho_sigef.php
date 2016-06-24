<?
set_time_limit(30000);
ini_set("memory_limit", "3000M");

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);}
 
 session_start();
 
$Tinicio = getmicrotime();

$dataIni = date("d/m/Y h:i:s");

if(!$_SESSION['usucpf']) $_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$arrProcesso = "'23400009625201271', '23400010786201216', '23400011498201271', '23400011847201254', '23400001632201225', '23400001637201258'";
$arrProcesso = "'23400001637201258'";

$sql = "select
			processo,
		    cnpj,
		    programa,
		    sistema
		from(
		  SELECT distinct
		      p.prpnumeroprocesso as processo,
		      p.prpcnpj as cnpj,
		      'CM' as programa,
		      'PAR' as sistema
		  FROM 
		      par.processopar p
		  WHERE 
		      p.prpcnpj is not null
		      and p.prpstatus = 'A'
		  
		  union all
		  
		  SELECT distinct
		      p.pronumeroprocesso as processo,
		      p.procnpj as cnpj,
		      'CM' as programa,
		      'ObrasPAR' as sistema
		  FROM 
		      par.processoobraspar p
		  WHERE 
		      p.procnpj is not null
		      and p.prostatus = 'A'
		  union all
		  
		  SELECT distinct
		      p.pronumeroprocesso as processo,
		      p.procnpj as cnpj,
		      'CN' as programa,
		      'PAC' as sistema
		  FROM 
		      par.processoobra p
		  WHERE 
		      p.procnpj is not null
		      and p.prostatus = 'A' 
		) as foo		 
		where processo in ('23400000854201221')";

$arrProcesso = $db->carregar($sql);

$wsusuario 	= 'juliov';
$wssenha	= 'jamudei1';

/*$wsusuario 	= 'MECTIAGOT';
$wssenha	= 'M3135689';*/
$data_created = date("c");

//$db->executar("delete from par.empenhodivergentesigef");

foreach ($arrProcesso as $v) {
	$co_cnpj = $v['cnpj'];
	$nu_processo = $v['processo'];
	$co_programa_fnde = $v['programa'];	

$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$wsusuario</usuario>
			<senha>$wssenha</senha>
		</auth>
		<params>
			<co_cnpj>$co_cnpj</co_cnpj>
			<nu_processo>$nu_processo</nu_processo>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<efetivados>S</efetivados>
			<rownum>0</rownum>
			<numero_de_linhas>300</numero_de_linhas>
		</params>
	</body>
</request>
XML;

				
	/*if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
		$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
	} else {*/
		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
	//}
	//$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';

	$xml = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWS)
			->setParams( array('xml' => $arqXml, 'method' => 'consultarAndamentoNE') )
			->execute();

	$xmlRetorno = $xml;

    $xml = simplexml_load_string( stripslashes($xml));
	ver($xml,d);
	if( (int)$xml->status->result == 1 && (int)$xml->status->message->code == 1 ){
		$arrXML = $xml->body->children();
		
		foreach ($arrXML as $xml) {
			$nu_seq_ne 						= trim((string)$xml->nu_seq_ne);
			$nu_empenho_original 			= trim((string)$xml->nu_empenho_original);
			$nu_seq_doc_siafi_original 		= trim((string)$xml->nu_seq_doc_siafi_original);
			$an_exercicio_original 			= trim((string)$xml->an_exercicio_original);
			$situacao_documento 			= trim((string)$xml->situacao_documento);
			$data_documento 				= trim((string)$xml->data_documento);			
			$valor_ne 						= trim((string)$xml->valor_ne);
			$processo 						= trim((string)$xml->processo);
			$nu_cnpj 						= trim((string)$xml->nu_cnpj);
			$numero_documento 				= trim((string)$xml->numero_documento);
			$ds_problema 					= trim((string)$xml->ds_problema);
			$co_especie_empenho 			= trim((string)$xml->co_especie_empenho);
			$valor_total_empenhado 			= trim((string)$xml->valor_total_empenhado);
			$valor_saldo_pagamento 			= trim((string)$xml->valor_saldo_pagamento);			
			$co_programa_fnde 				= trim((string)$xml->co_programa_fnde);
			$ano_convenio_original 			= trim((string)$xml->ano_convenio_original);			
			$nu_convenio_original 			= trim((string)$xml->nu_convenio_original);
			$unidade_gestora_responsavel	= trim((string)$xml->unidade_gestora_responsavel);
			$co_diretoria 					= trim((string)$xml->co_diretoria);
			
			/*$valor_ne = str_replace(".","", $valor_ne);
			$valor_ne = str_replace(",",".", $valor_ne);*/
			/*$arrPrograma = explode('-', $co_programa_fnde);
			
			$empnumerooriginal 	= substr($numero_documento, 6);
			$empanooriginal 	= substr($numero_documento, 0, 4);
			$data_documento 	= ($data_documento ? "'".formata_data_sql($data_documento)."'" : 'null');
			
			$empnumerooriginal 		= ($empnumerooriginal ? "'".$empnumerooriginal."'" : 'null');
			$empanooriginal 		= ($empanooriginal ? "'".$empanooriginal."'" : 'null');
			$nu_convenio_original 	= ($nu_convenio_original ? "'".$nu_convenio_original."'" : 'null');
			$ano_convenio_original 	= ($ano_convenio_original ? "'".$ano_convenio_original."'" : 'null');
			$situacao_documento 	= ($situacao_documento ? "'".$situacao_documento."'" : 'null');
			$valor_total_empenhado 	= ($valor_total_empenhado ? "'".$valor_total_empenhado."'" : 'null');
			$valor_saldo_pagamento 	= ($valor_saldo_pagamento ? "'".$valor_saldo_pagamento."'" : 'null');
			$nu_empenho_original 	= ($nu_empenho_original ? "'".$nu_empenho_original."'" : 'null');*/
			
			$sql = "INSERT INTO par.empenhodivergentesigef(nu_seq_ne, nu_empenho_original, nu_seq_doc_siafi_original, an_exercicio_original, situacao_documento, data_documento,
  							valor_ne, processo, nu_cnpj, numero_documento, ds_problema, co_especie_empenho, valor_total_empenhado, valor_saldo_pagamento, co_programa_fnde,
  							ano_convenio_original, nu_convenio_original, unidade_gestora_responsavel, co_diretoria) 
						VALUES ('{$nu_seq_ne}', '{$nu_empenho_original}', '{$nu_seq_doc_siafi_original}', '{$an_exercicio_original}', '{$situacao_documento}', '{$data_documento}',
  							'{$valor_ne}', '{$processo}', '{$nu_cnpj}', '{$numero_documento}', '{$ds_problema}', '{$co_especie_empenho}', '{$valor_total_empenhado}', '{$valor_saldo_pagamento}', '{$co_programa_fnde}',
  							'{$ano_convenio_original}', '{$nu_convenio_original}', '{$unidade_gestora_responsavel}', '{$co_diretoria}')";
  			
			$db->executar($sql);
		}
	}
		$db->commit();
	//}
}
//die('foi');
$Tfinal= getmicrotime() - $Tinicio;

die("<p>A atualização dos Empenhos foram realizados com sucesso! Data Inicio: ".$dataIni.", Data Fim: ".date("d/m/Y h:i:s")."</p>
				   <p>O tempo de execução das atualizações foi de ".$Tfinal." segundos</p>");

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "SCRIPT AUTOMATICO";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress( $_SESSION['email_sistema'], "SIMEC" );
$mensagem->Subject = "Atualização do PAR - Empenho do SIGEF";
$mensagem->Body = "<p>A atualização dos Empenhos foram realizados com sucesso! ".date("d/m/Y h:i:s")."</p>
				   <p>O tempo de execução das atualizações foi de ".$Tfinal." segundos</p>";
$mensagem->IsHTML( true );
$mensagem->Send();

?>
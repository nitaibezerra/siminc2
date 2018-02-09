<?php

//$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once "/var/www/simec/global/config.inc";
//include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "/includes/classes/Fnde_Webservice_Client.class.inc";

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);}


$db = new cls_banco();

function consultarContaCorrente($dados) {
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		$senha   = $dados['wssenha'];

        $proseqconta = $db->pegaUm("SELECT proseqconta FROM par.processoobraspar WHERE prostatus = 'A' and proid='".$_SESSION['par_var']['proid']."'");

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
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <seq_solic_cr>$proseqconta</seq_solic_cr>
		</params>
	</body>
</request>
XML;
		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';

		if($proseqconta) {

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
					->execute();

			$xmlRetorno = $xml;

		    $xml = simplexml_load_string( stripslashes($xml));

		    if($xml->body->row->seq_conta) {
		    	$db->executar("UPDATE par.processoobraspar SET 
		    						nu_conta_corrente='".$xml->body->row->nu_conta_corrente."', 
		    						seq_conta_corrente='".$xml->body->row->seq_conta."' 
		    					WHERE proseqconta='".$proseqconta."'");
		    	$db->commit();
		    }

		    $result = (integer) $xml->status->result;

		    if($result) {
		    	return false;
		    } else {
		    	return true;
		    }

		} else {
			return true;
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";

	}
}

function consultarEmpenho($dados) {
	global $db;

	try {
		$data_created 			= date("c");
		$usuario 				= $dados['wsusuario'];
		$senha   				= $dados['wssenha'];
        $nu_seq_ne 				= $dados['empprotocolo'];
        $proid	 				= $dados['proid'];
        $empid	 				= $dados['empid'];
        $empsituacao			= $dados['empsituacao'];
        $empcodigoespecie		= $dados['empcodigoespecie'];
        $empvalorempenho		= $dados['empvalorempenho'];
        $empnumero				= $dados['empnumero'];
        $empanooriginal			= $dados['empanooriginal'];
        $empnumerooriginal		= $dados['empnumerooriginal'];
        $empnumeroprocesso		= $dados['empnumeroprocesso'];
        $empcnpj				= $dados['empcnpj'];
        $valor_saldo_pagamento	= $dados['valor_saldo_pagamento'];
        $valor_total_empenhado	= $dados['valor_total_empenhado'];

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
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_seq_ne>$nu_seq_ne</nu_seq_ne>
		</params>
	</body>
</request>
XML;

		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));
		
		$result = (integer) $xml->status->result;
		$status = utf8_decode((string)$xml->body->row->status);
		$co_status			= substr( $status, 0, 1 );
		
		$aParametros = array();
		
	    if( $result ){
	    	if( (int)$co_status == 1 ){
				$arrXML = $xml->body->children();
				$arrXML = $arrXML ? $arrXML : array();
				
				$db->executar("update par.empenho set empstatus = 'A' where empid = $empid");
				
				foreach($arrXML as $chaves => $dados ){
					foreach($dados as $key => $dado){
						if($key == 'data_documento' ){
							if( $dados->{$key} ){
								$dados->{$key} = formata_data_sql(trim((string) $dados->{$key}));
							}
						}
						$aParametros[$key] = trim( utf8_decode( (string) $dados->{$key} ) );
					}
				}
				
				if( $aParametros['situacao_documento'] != trim($empsituacao) || $aParametros['valor_ne'] != $empvalorempenho || 
					$aParametros['numero_documento'] != $empnumero || $aParametros['co_especie_empenho'] != $empcodigoespecie ){
					
					$set = array();
					if( $aParametros['nu_cnpj'] ) 			 $set[] = "empcnpj = '".$aParametros['nu_cnpj']."'";
					if( $aParametros['processo'] ) 			 $set[] = "empnumeroprocesso = '".$aParametros['processo']."'";
					if( $aParametros['nu_seq_ne'] ) 		 $set[] = "empprotocolo = '".$aParametros['nu_seq_ne']."'";
					if( $aParametros['co_especie_empenho'] ) $set[] = "empcodigoespecie = '".$aParametros['co_especie_empenho']."'";
					if( $aParametros['situacao_documento'] ) $set[] = "empsituacao = '".$aParametros['situacao_documento']."'";
						
					if( !empty($aParametros['numero_documento']) ){
						$empnumerooriginal 	= substr($aParametros['numero_documento'], 6);
						$empanooriginal 	= substr($aParametros['numero_documento'], 0, 4);
					
						$set[] = "empnumero = '".$aParametros['numero_documento']."'";
						$set[] = "empnumerooriginal = '".$empnumerooriginal."'";
						$set[] = "empanooriginal = '".$empanooriginal."'";
					
					}
					if( $aParametros['valor_ne'] ) 						$set[] = "empvalorempenho = '".$aParametros['valor_ne']."'";
					if( $aParametros['ds_problema'] ) 					$set[] = "ds_problema = '".$aParametros['ds_problema']."'";
					if( $aParametros['valor_total_empenhado'] )			$set[] = "valor_total_empenhado = '".$aParametros['valor_total_empenhado']."'";
					if( $aParametros['valor_saldo_pagamento'] )			$set[] = "valor_saldo_pagamento = '".$aParametros['valor_saldo_pagamento']."'";
					if( $aParametros['data_documento'] )				$set[] = "empdata = '".$aParametros['data_documento']."'";
					//if( $aParametros['unidade_gestora_responsavel'] )	$set[] = "empunidgestoraeminente = '".$aParametros['unidade_gestora_responsavel']."'";
					if( $aParametros['tp_especializacao'] )				$set[] = "tp_especializacao = '".$aParametros['tp_especializacao']."'";
					if( $aParametros['co_diretoria'] )					$set[] = "co_diretoria = '".$aParametros['co_diretoria']."'";
					
					if($set) {
						$sql = "UPDATE par.empenho SET 
									".implode(",",$set)."
								WHERE empid='".$empid."'";
						$db->executar($sql);
					}
					
					$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, empsituacao, ds_problema, co_especie_empenho, valor_total_empenhado, valor_saldo_pagamento)
				    			VALUES ('".$_SESSION['usucpf']."',
				    					'".$empid."',
				    					NOW(),
				    					'".$aParametros['situacao_documento']."',
				    					'".$aParametros['ds_problema']."',
				    					'".$aParametros['co_especie_empenho']."',
				    					".((strlen($aParametros['valor_total_empenhado']))?"'".$aParametros['valor_total_empenhado']."'":"NULL").",
				    					".((strlen($aParametros['valor_saldo_pagamento']))?"'".$aParametros['valor_saldo_pagamento']."'":"NULL").");";
					
					$db->executar($sql);
					
					$sql = "INSERT INTO par.historicowsprocessoobrapar(
					    	proid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$proid."',
					    		'consultarEmpenhoRotina - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
					
					$db->executar($sql);
					$db->commit();
				}
			} else {
				$db->executar("update par.empenho set empstatus = 'I' where empid = $empid");
				$db->commit();
			}
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Consulta empenho encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar empenho no SIGEF: $erroMSG";
	}
}

function consultarPagamento($dados) {
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

	    $dadospag = $db->pegaLinha("SELECT * FROM par.pagamento WHERE pagid='".$dados['pagid']."'");

	    if($dadospag) {
	    	$nu_seq_ob = $dadospag['parnumseqob'];
	    }

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
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_seq_ob>$nu_seq_ob</nu_seq_ob>
		</params>
	</body>
</request>
XML;

		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;

		if( $result ) {			
			$situacao_documento = utf8_decode($xml->body->row->situacao_documento);

			//$db->executar("UPDATE par.pagamento SET pagstatus = 'A' WHERE pagid = {$dados['pagid']}");
				
			$atualizar = true;
			if($dados['atualizacao_lote']) {
				$sql = "SELECT * FROM par.pagamento WHERE pagid='".$dados['pagid']."'";
				$atualizacao_pagamento = $db->pegaLinha($sql);

				if(trim($atualizacao_pagamento['pagsituacaopagamento']) == trim(iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)) &&
				   trim($atualizacao_pagamento['pagdatapagamentosiafi']) == formata_data_sql(iconv("UTF-8", "ISO-8859-1", $xml->body->row->data_documento)) &&
				   trim($atualizacao_pagamento['pagnumeroob']) == trim($xml->body->row->numero_documento)
				   ) {
				   	$atualizar = false;
				}

			}

			$data = (string)$xml->body->row->data_documento ? "'".formata_data_sql(iconv("UTF-8", "ISO-8859-1", $xml->body->row->data_documento))."'" : 'NULL';
			
			if($atualizar) {
				if((string)$xml->body->row->data_documento){
				
					$pagnumeroob = ((strlen($xml->body->row->numero_documento))?$xml->body->row->numero_documento:"-");
					
					$sql = "UPDATE par.pagamento 
							SET 
								pagsituacaopagamento='".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."',
								pagdatapagamentosiafi=".$data.",
								pagnumeroob = '$pagnumeroob'
						   	WHERE 
								pagid = '".$dadospag['pagid']."'";
					
					$db->executar($sql);
				} else {
					$db->executar("UPDATE par.pagamento SET pagsituacaopagamento='".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."'
								   WHERE pagid='".$dadospag['pagid']."'");
				}
				
				if( trim($dadospag['pagsituacaopagamento']) !=  utf8_decode($situacao_documento) ){
					$db->executar("INSERT INTO par.historicopagamento(
			           			   pagid, hpgdata, usucpf, hpgparcela, hpgvalorparcela, hpgsituacaopagamento)
			   					   VALUES ('".$dadospag['pagid']."', NOW(), '".$_SESSION['usucpf']."',
			   					   		   '".$dadospag['pagparcela']."', '".$dadospag['pagvalorparcela']."', '".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."');");
				}
				
				$sql = "INSERT INTO par.historicowsprocessoobrapar(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."',
				    		'consultarPagamento - Sucesso - Rotina Automática',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";
				
				$db->executar($sql);
				$db->commit();
			}

			return true;

		}


	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Consulta pagamento encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar Pagamento no SIGEF: $erroMSG";


	}
}


session_start();

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

$Tinicio = getmicrotime();

$wsusuario 	= 'USAP_WS_SIGARP';
$wssenha	= '03422625';

 // atualizando conta corrente
$sql = "SELECT * FROM par.processoobraspar WHERE prostatus = 'A' and nu_conta_corrente IS NULL AND seq_conta_corrente IS NOT NULL";

$contas = $db->carregar($sql);
//ver($contas,d);
if($contas[0]) {
	foreach($contas as $conta) {

		$_SESSION['par_var']['proid'] = $conta['proid'];

		$parametros = array('wsusuario' => $wsusuario,
							'wssenha'	=> $wssenha);

		echo "Atualiza CC #".$conta['proid']." : ".consultarContaCorrente($parametros)."<br>";

	}
}

// atualizando empenho
 $sql = "SELECT distinct pro.proid, emp.empprotocolo, emp.empsituacao, emp.empid, emp.empcodigoespecie,
			emp.empvalorempenho, emp.empnumero, emp.empanooriginal, emp.empnumerooriginal, emp.empnumeroprocesso,
    		emp.empcnpj, emp.valor_saldo_pagamento, emp.valor_total_empenhado  
		FROM par.empenho emp
			INNER JOIN par.processoobraspar pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'";

$empenhos = $db->carregar($sql);

if($empenhos[0]) {
	foreach($empenhos as $key => $empenho) {
		$empenho['wsusuario'] = $wsusuario;
		$empenho['wssenha'] = $wssenha;
		echo "Atualiza empenho #".($key+1)." : ".consultarEmpenho($empenho)."<br>";
	}
} 
// atualizando pagamento
$sql = "SELECT * FROM par.pagamento pag
		INNER JOIN par.empenho emp ON emp.empid = pag.empid and empstatus = 'A'
		INNER JOIN par.processoobraspar pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'";

$pagamentos = $db->carregar($sql);

if($pagamentos[0]) {
	foreach($pagamentos as $pagamento) {

		$_SESSION['par_var']['proid'] = $pagamento['proid'];

		$parametros = array('pagid'     	   => $pagamento['pagid'],
							'wsusuario' 	   => $wsusuario,
							'wssenha'		   => $wssenha,
							'atualizacao_lote' => true);

		echo "Atualiza pagamento #".$pagamento['pagid']." : ".consultarPagamento($parametros)."<br>";

	}
}

$db->commit();

$Tfinal= getmicrotime() - $Tinicio;

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "SCRIPT AUTOMATICO";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress( $_SESSION['email_sistema'], SIGLA_SISTEMA );
$mensagem->Subject = "Atualização do OBRAS PAR - Conta Corrente, Empenho ou Pagamento";
$mensagem->Body = "<p>A atualização das Contas Correntes, Empenhos e Pagamentos foram realizados com sucesso! ".date("d/m/Y h:i:s")."</p>
				   <p>O tempo de execução das atualizações foi de ".$Tfinal." segundos</p>";

$mensagem->IsHTML( true );
$mensagem->Send();

?>
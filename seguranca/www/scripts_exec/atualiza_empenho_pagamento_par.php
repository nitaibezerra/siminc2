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

        $proseqconta = $db->pegaUm("SELECT prpseqconta FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."' and prpstatus = 'A' ");

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
		    	$db->executar("UPDATE par.processopar SET nu_conta_corrente='".$xml->body->row->nu_conta_corrente."', seq_conta_corrente='".$xml->body->row->seq_conta."' WHERE prpseqconta='".$proseqconta."'");
		    	$db->commit();
		    }

			/* echo "------ CONSULTA DE CONTA CORRENTE ------\n\n";
			echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
			echo "*** Detalhes da consulta ***\n\n";
			echo "* Data movimento:".(($xml->body->row->dt_movimento)?$xml->body->row->dt_movimento:'-')."\n";
			echo "* Fase solicitação:".(($xml->body->row->fase_solicitacao)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->fase_solicitacao):'-')."\n";
			echo "* Entidade:".(($xml->body->row->ds_razao_social)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->ds_razao_social):'-')."(".(($xml->body->row->nu_identificador)?$xml->body->row->nu_identificador:'-').")\n\n";

			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'consultarContaCorrente',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit(); */

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

		$data_created = date("c");

		$usuario = $dados['wsusuario'];
		$senha   = $dados['wssenha'];

	    $dadosemp = $db->pegaLinha("SELECT * FROM par.empenho WHERE empid='".$dados['empid']."'");

        if($dadosemp) {
        	$nu_seq_ne = $dadosemp['empprotocolo'];
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
	     
	    if( $result ){
	    	if( (int)$co_status == 1 ){
		    	$db->executar("update par.empenho set empstatus = 'A' where empid = {$dados['empid']}");
		    	
				$situacaoEmpenho = iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento);
						
				$atualizar = true;
				if($dados['atualizacao_lote']) {
					$sql = "SELECT * FROM par.empenho WHERE empid='".$dados['empid']."'";
					$atualizacao_empenho = $db->pegaLinha($sql);
					
					if($atualizacao_empenho['empnumero']             == $xml->body->row->numero_documento &&
					   $atualizacao_empenho['ds_problema']           == $xml->body->row->ds_problema &&
					   $atualizacao_empenho['valor_total_empenhado'] == $xml->body->row->valor_total_empenhado &&
					   $atualizacao_empenho['valor_saldo_pagamento'] == $xml->body->row->valor_saldo_pagamento &&
					   trim($atualizacao_empenho['empsituacao'])     == trim($situacaoEmpenho) ) {
					   	
					   	$atualizar = false;
						
					}
					
				}
		
				if($atualizar) {
							
					$set = array();
					if( $xml->body->row->nu_cnpj ) 						$set[] = "empcnpj = '".$xml->body->row->nu_cnpj."'";
					if( $xml->body->row->processo ) 					$set[] = "empnumeroprocesso = '".$xml->body->row->processo."'";
					if( $xml->body->row->nu_seq_ne ) 					$set[] = "empprotocolo = '".$xml->body->row->nu_seq_ne."'";
					if( $xml->body->row->co_especie_empenho ) 			$set[] = "empcodigoespecie = '".$xml->body->row->co_especie_empenho."'";
					if( $xml->body->row->situacao_documento ) 			$set[] = "empsituacao = '".$situacaoEmpenho."'";
					
					$numero_documento = trim((string)$xml->body->row->numero_documento);
					
					if( !empty($numero_documento) ){
						$empnumerooriginal 	= substr($numero_documento, 6);
						$empanooriginal 	= substr($numero_documento, 0, 4);
						
						$set[] = "empnumero = '".$numero_documento."'";
						$set[] = "empnumerooriginal = '".$empnumerooriginal."'";
						$set[] = "empanooriginal = '".$empanooriginal."'";
						
					}
					if( $xml->body->row->valor_ne ) 					$set[] = "empvalorempenho = '".$xml->body->row->valor_ne."'";
					if( $xml->body->row->ds_problema ) 					$set[] = "ds_problema = '".$xml->body->row->ds_problema."'";
					if( $xml->body->row->valor_total_empenhado )		$set[] = "valor_total_empenhado = '".$xml->body->row->valor_total_empenhado."'";
					if( $xml->body->row->valor_saldo_pagamento )		$set[] = "valor_saldo_pagamento = '".$xml->body->row->valor_saldo_pagamento."'";
					if( $xml->body->row->data_documento )				$set[] = "empdata = '".$xml->body->row->data_documento."'";
					//if( $xml->body->row->unidade_gestora_responsavel )	$set[] = "empunidgestoraeminente = '".$xml->body->row->unidade_gestora_responsavel."'";
					if( $xml->body->row->tp_especializacao )			$set[] = "tp_especializacao = '".$xml->body->row->tp_especializacao."'";
					if( $xml->body->row->co_diretoria )					$set[] = "co_diretoria = '".$xml->body->row->co_diretoria."'";
					
					
					if($set) {
						
						$db->executar("UPDATE par.empenho SET ".implode(",",$set)."
									   WHERE empid='".$dados['empid']."'");
						
						if( trim($dadosemp['empsituacao']) != (string)$situacaoEmpenho ){
							$sql = "INSERT INTO par.historicoempenho(
				           		usucpf, empid, hepdata, empsituacao, co_especie_empenho, ds_problema, valor_total_empenhado,
				            	valor_saldo_pagamento)
				    			VALUES ('".$_SESSION['usucpf']."',
				    					'".$dados['empid']."',
				    					NOW(),
				    					'".$situacaoEmpenho."',
				    					'".$xml->body->row->co_especie_empenho."',
				    					'".$xml->body->row->ds_problema."',
				    					".((strlen($xml->body->row->valor_total_empenhado))?"'".$xml->body->row->valor_total_empenhado."'":"NULL").",
				    					".((strlen($xml->body->row->valor_saldo_pagamento))?"'".$xml->body->row->valor_saldo_pagamento."'":"NULL").");";
				
							$db->executar($sql);
						}
						$db->commit();
					}
					
					$sql = "INSERT INTO par.historicowsprocessopar(
						    	prpid,
						    	hwpwebservice,
						    	hwpxmlenvio,
						    	hwpxmlretorno,
						    	hwpdataenvio,
						        usucpf)
						    VALUES ('".$_SESSION['par_var']['prpid']."',
						    		'consultarEmpenhoRotina - Sucesso',
						    		'".addslashes($arqXml)."',
						    		'".addslashes($xmlRetorno)."',
						    		NOW(),
						            '".$_SESSION['usucpf']."');";
					
					$db->executar($sql);			
					$db->commit();				
				}
			}else{
				$db->executar("update par.empenho set empstatus = 'I' where empid = {$dados['empid']}");
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
	    
		if($result) {
			
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid, 
				    	hwpwebservice, 
				    	hwpxmlenvio, 
				    	hwpxmlretorno, 
				    	hwpdataenvio, 
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."', 
				    		'consultarPagamento - Sucesso', 
				    		'".addslashes($arqXml)."', 
				    		'".addslashes($xmlRetorno)."', 
				    		NOW(), 
				            '".$_SESSION['usucpf']."');";
			
			$db->executar($sql);
			$db->commit();
	    	
			$situacao_documento = trim($xml->body->row->situacao_documento);
			
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
	
			if($atualizar) {
				
				$pagnumeroob = ((strlen($xml->body->row->numero_documento))?$xml->body->row->numero_documento:"-");
				
				$sql = "UPDATE par.pagamento 
						SET 
							pagsituacaopagamento='".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."'".((trim($xml->body->row->data_documento))?", 
							pagdatapagamentosiafi='".formata_data_sql(iconv("UTF-8", "ISO-8859-1", trim($xml->body->row->data_documento)))."'":"").",
							pagnumeroob = '$pagnumeroob'
					   	WHERE 
							pagid = '".$dadospag['pagid']."'";
				
				$db->executar($sql);
				
				if( trim($dadospag['pagsituacaopagamento']) !=  utf8_decode($situacao_documento) ){
			
					$db->executar("INSERT INTO par.historicopagamento(
			           			   pagid, hpgdata, usucpf, hpgparcela, hpgvalorparcela, hpgsituacaopagamento)
			   					   VALUES ('".$dadospag['pagid']."', NOW(), '".$_SESSION['usucpf']."', 
			   					   		   '".$dadospag['pagparcela']."', '".$dadospag['pagvalorparcela']."', '".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."');");
					
				}
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

// atualizando conta corrente

if(!$_REQUEST['prpid']) {

	$sql = "SELECT * FROM par.processopar WHERE nu_conta_corrente IS NULL AND seq_conta_corrente IS NOT NULL and prpstatus = 'A'";
	
	$contas = $db->carregar($sql);
	
	if($contas[0]) {
		foreach($contas as $conta) {
			
			$_SESSION['par_var']['prpid'] = $conta['prpid'];
			
			$parametros = array('wsusuario' => 'USAP_WS_SIGARP',
								'wssenha'	=> '03422625');
			
			echo "Atualiza CC #".$conta['prpid']." : ".consultarContaCorrente($parametros)."<br>";
	
		}
	}

}

if(!$_REQUEST['prpid']) {

	/* if(date("w")!=6) {
		$wh_empenho   = " WHERE to_char(empdata,'YYYY')='".date("Y")."'";
		$wh_pagamento = " AND to_char(pagdatapagamento,'YYYY')='".date("Y")."'";
	} */
	
	
	// atualizando empenho
	$sql = "SELECT * FROM par.empenho emp 
			INNER JOIN par.processopar pro ON pro.prpnumeroprocesso = emp.empnumeroprocesso and pro.prpstatus = 'A'".$wh_empenho;
	
	$empenhos = $db->carregar($sql);
	
	if($empenhos[0]) {
		foreach($empenhos as $empenho) {
			
			$_SESSION['par_var']['prpid'] = $empenho['prpid'];
			
			$parametros = array('empid'     	   => $empenho['empid'],
								'wsusuario' 	   => 'USAP_WS_SIGARP',
								'wssenha'		   => '03422625',
								'atualizacao_lote' => true);
			
			echo "Atualiza empenho #".$empenho['empid']." : ".consultarEmpenho($parametros)."<br>";
	
		}
	}

} else {
	if(is_numeric($_REQUEST['prpid'])) {
		$wh_pagamento = " AND pro.prpid='".$_REQUEST['prpid']."'";
	}
}

// atualizando pagamento
$sql = "SELECT * FROM par.pagamento pag
		INNER JOIN par.empenho emp ON emp.empid = pag.empid and empstatus = 'A' 
		INNER JOIN par.processopar pro ON pro.prpnumeroprocesso = emp.empnumeroprocesso and pro.prpstatus = 'A'
		WHERE 1=1".$wh_pagamento;

$pagamentos = $db->carregar($sql);

if($pagamentos[0]) {
	foreach($pagamentos as $pagamento) {
		
		$_SESSION['par_var']['prpid'] = $pagamento['prpid'];
		
		$parametros = array('pagid'     	   => $pagamento['pagid'],
							'wsusuario' 	   => 'USAP_WS_SIGARP',
							'wssenha'		   => '03422625',
							'atualizacao_lote' => ((is_numeric($_REQUEST['prpid']))?false:true));
		
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
$mensagem->Subject = "Atualização do PAR - Conta Corrente, Empenho ou Pagamento";
$mensagem->Body = "<p>A atualização das Contas Correntes, Empenhos e Pagamentos foram realizados com sucesso! ".date("d/m/Y h:i:s")."</p>
				   <p>O tempo de execução das atualizações foi de ".$Tfinal." segundos</p>";

$mensagem->IsHTML( true );
$mensagem->Send();

?>
<?php

header( 'Content-Type: text/html; charset=ISO-8859-1' );
//header( 'Content-Type: text/html; charset=UTF-8' );

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

set_time_limit( 0 );
error_reporting( E_ALL ^ E_NOTICE );

ini_set( 'soap.wsdl_cache_enabled', '0' );
ini_set( 'soap.wsdl_cache_ttl', 0 );
ini_set( 'default_socket_timeout', '99999999' );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funções gerais
require_once APPRAIZ . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/projovemcampo/_funcoes.php";
require_once APPRAIZ . "www/projovemcampo/_constantes.php";
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
function consultaPagamentoCampo(){
	$opcoes = Array(
	                'exceptions'	=> 0,
	                'trace'			=> true,
	                //'encoding'		=> 'UTF-8',
	                'encoding'		=> 'ISO-8859-1',
	                'cache_wsdl'    => WSDL_CACHE_NONE
	);
	
	$soapClient = new SoapClient( WSDL_CAMINHO, $opcoes );
	
	libxml_use_internal_errors( true );
	    
	// CPF do administrador de sistemas
	if(!$_SESSION['usucpf']) {
		$_SESSION['usucpforigem'] = '';
		$_SESSION['usucpf'] = '';
	}
	
	function getmicrotime2() {list($usec, $sec) = explode(" ", microtime()); return ((float)$usec + (float)$sec);}
	
	$microtime = getmicrotime2();
	    
	ini_set("memory_limit", "2048M");
	
	// abre conexção com o servidor de banco de dados
	$db = new cls_banco();
	
	$sql = "SELECT DISTINCT
				rem.*
			FROM
				projovemcampo.remessapagamento rem
			INNER JOIN projovemcampo.lancamentodiario lnd ON lnd.remid = rem.remid
			INNER JOIN projovemcampo.diario dia ON dia.diaid = lnd.diaid
			WHERE
				remprocessada=FALSE
			
				";
	;
	
	$remessapagamento = $db->carregar( $sql );

	if($remessapagamento[0]) {
		
		foreach($remessapagamento as $rem) {
			
			$arxml['reciboEnvio']['autenticacao']    = array('sistema' => SISTEMA_SGB, 'login' => USUARIO_SGB,'senha' => SENHA_SGB);
			$arxml['reciboEnvio']['remessa-id']  	 = $rem['remid'];
			$arxml['reciboEnvio']['rastreador'] 	 = $rem['remrastreador'];
	
			$consultarRemessaDePagamentos_obj = $soapClient->consultarRemessaDePagamentos( $arxml );

			$logerro_consultarRemessaDePagamentos = (($consultarRemessaDePagamentos_obj->remessa->mensagem->codigo=='10001' || $consultarRemessaDePagamentos_obj->remessa->situacao->codigo=='PROCESSADA' || $consultarRemessaDePagamentos_obj->remessa->situacao->codigo=='RECEBIDA')?'FALSE':'TRUE');
 			
			inserirDadosLog(array('logerro'=>$logerro_consultarRemessaDePagamentos,'logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'remid'=>$rem['remid'],'logservico'=>'consultarRemessaDePagamentos'));

			if(count($consultarRemessaDePagamentos_obj->remessa->pagamentos->pagamento)>1) {
				foreach($consultarRemessaDePagamentos_obj->remessa->pagamentos->pagamento as $pag) {
					processarPagamentoBolsistaSGB($pag);
				}
			} elseif(count($consultarRemessaDePagamentos_obj->remessa->pagamentos->pagamento)==1) {
				processarPagamentoBolsistaSGB($consultarRemessaDePagamentos_obj->remessa->pagamentos->pagamento);
			}
	
			if($consultarRemessaDePagamentos_obj->remessa->situacao->codigo=='PROCESSADA') {
					
				echo "Remessa processada : {$rem['remrastreador']}<br>";
					
				$sql = "UPDATE projovemcampo.remessapagamento SET remprocessada=true, remdtprocessamento='".formata_data_sql($consultarRemessaDePagamentos_obj->remessa->situacao->data)."' WHERE remid='".$consultarRemessaDePagamentos_obj->remessa->id."'";
				$db->executar($sql);
				
				$db->commit();
						
			} elseif($logerro_consultarRemessaDePagamentos=='TRUE') {
						
			echo "Remessa com problemas : {$rem['remrastreador']}<br>";
				
				$sql = "UPDATE projovemcampo.historicopagamento SET hiprejeitado='t' WHERE remid='".$rem['remid']."'";
				$db->executar($sql);
				
				$sql = "UPDATE projovemcampo.remessapagamento SET remprocessada=true, remdtprocessamento=NOW() WHERE remid='".$rem['remid']."'";
				$db->executar($sql);
				
				$db->commit();
			
			}
	
		}
	}
	
// 	$sql = "SELECT lnd.lndid FROM projovemcampo.estudante est
// 		INNER JOIN projovemcampo.lancamentodiario lnd ON lnd.estid = est.estid
// 		INNER JOIN workflow.documento d ON d.docid = lnd.docid
// 		INNER JOIN projovemcampo.historicopagamento hip ON hip.lndid = lnd.lndid
// 		INNER JOIN projovemcampo.remessapagamento rem ON rem.remid = hip.remid
// 		WHERE d.esdid = '".ESD_PAGAMENTO_AUTORIZADO."' AND rem.remprocessada=true AND hip.hiprejeitado = 't'--AND rem.remid = 19";
	
// 	$reenvios = $db->carregarColuna($sql);

// 	if($reenvios) {
// 		foreach($reenvios as $lndid) {
// 			$db->executar("UPDATE projovemcampo.lancamentodiario SET remid=NULL WHERE lndid='".$lndid."'");
// 			$db->commit();
// 		}
// 	}

	if(!$_REQUEST['numeroDiasProcessamento']) $numeroDiasProcessamento = 30;
	else $numeroDiasProcessamento = $_REQUEST['numeroDiasProcessamento'];
	
	for($i=1;$i<=$numeroDiasProcessamento;$i++) {
		$datasel[] = "to_char(NOW(),'YYYY-mm-dd')::date - interval '".$i." day' as data".$i;
	}
	
	$datas = $db->pegaLinha("select ".implode(",",$datasel));
	
	for($i=$numeroDiasProcessamento;$i>=1;$i--) {
		
		$arxml['situacoes']['autenticacao'] 		= array('sistema' => SISTEMA_SGB, 'login' => USUARIO_SGB,'senha' => SENHA_SGB);
		$arxml['situacoes']['programa'] 			= PROGRAMA_SGB;
		$arxml['situacoes']['dataDasAlteracoes'] 	= formata_data($datas['data'.$i]);
		
		$consultarSituacaoDePagamentos_obj = $soapClient->consultarSituacaoDePagamentos( $arxml );
		
		if($consultarSituacaoDePagamentos_obj->situacoes->pagamentos->pagamento) {
			foreach($consultarSituacaoDePagamentos_obj->situacoes->pagamentos->pagamento as $pgs) {
				
				$lndid = $pgs->id;
				
				if(count($pgs->situacoes->situacao)>1) {
					$pg = end($pgs->situacoes->situacao);
				} else {
					$pg = $pgs->situacoes->situacao;
				}
				
// 				if($pg->codigo==SGB_AUTORIZADA || $pg->codigo==SGB_HOMOLOGADA || $pg->codigo==SGB_PREAPROVADA || $pg->codigo==SGB_ENVIADOAOSIGEF) {
// 					$docid = $db->pegaUm("SELECT p.docid FROM projovemcampo.lancamentodiario p 
// 										  INNER JOIN workflow.documento d ON d.docid = p.docid 
// 										  WHERE lndid='".$lndid."' AND d.esdid='".ESD_PAGAMENTO_AG_AUTORIZACAO_SGB."'");
// 					if($docid) {
// 						echo "Pagamento #".$lndid." (".$pg->data.") NÃO foi enviado para Aguardando pagamento<br>";
// 					}
// 				}
				
// 				if($pg->codigo==SGB_ENVIADOBANCO) {
// 					$docid = $db->pegaUm("SELECT p.docid FROM projovemcampo.lancamentodiario p 
// 										  INNER JOIN workflow.documento d ON d.docid = p.docid 
// 										  WHERE lndid='".$lndid."' ANDd.esdid='".ESD_PAGAMENTO_ENVIADO."'");
// 					if(!$docid) {
// 						echo "Pagamento #".$lndid." (".$pg->data.") Aguardando pagamento, não foi enviado para Enviado ao Banco<br>";
// 					}
// 				}
				
					
				if($pg->codigo==SGB_CREDITADA || $pg->codigo==SGB_SACADA || $pg->codigo==SGB_RESTITUIDO) {
					
					$lancamentodiario = $db->pegaLinha("SELECT d.docid, d.esdid FROM projovemcampo.lancamentodiario p 
													  INNER JOIN workflow.documento d ON d.docid = p.docid 
													  WHERE lndid='".$lndid."'");
					
					$docid 		  = $lancamentodiario['docid'];
					$esdid_origem = $lancamentodiario['esdid'];
					
					if($esdid_origem) {
						$sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem='".$esdid_origem."' and esdiddestino='".ESD_PAGAMENTO_PAGO."'";
						$aedid = $db->pegaUm($sql);
					}
					
					
					if($docid && $aedid) {
						echo "Pagamento #".$lndid." (".$pg->data.") foi enviado para Pagamento Efetivado<br>";
						$result = wf_alterarEstado( $docid, $aedid, $cmddsc = '', array());
					}
	
				}
			}
		}

		inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logservico'=>'consultarSituacaoDePagamentos'));
		
	}
	
// 	$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='projovemcampo_consultar_pagamentos.php'";
// 	$db->executar($sql);
// 	$db->commit();
	
	
	$db->close();
		
	if($_SESSION['usucpf'] == '') {
		
		unset($_SESSION['usucpf']);
		unset($_SESSION['usucpforigem']);
		
	}


	echo "fim";
// 	echo "
//             <script>
//                 alert('Fim.');
//                 window.location.href = window.location.href;
//             </script>";
}

?>
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
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/sisindigena2/_funcoes.php";
require_once APPRAIZ . "www/sisindigena2/_constantes.php";
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


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
	$_SESSION['usucpforigem'] = '00000000191';
	$_SESSION['usucpf'] = '00000000191';
}

function getmicrotime() {list($usec, $sec) = explode(" ", microtime()); return ((float)$usec + (float)$sec);}

$microtime = getmicrotime();
    
ini_set("memory_limit", "2048M");

// abre conexção com o servidor de banco de dados
$db = new cls_banco();

$sql = "select i.iuscpf, lpad(f.fpbmesreferencia::text,2,'0') as fpbmesreferencia, fpbanoreferencia, p.pboid from sisindigena2.pagamentobolsista p 
		inner join sisindigena2.identificacaousuario i on i.iusd = p.iusd
		inner join workflow.documento d on d.docid = p.docid 
		inner join workflow.estadodocumento e on e.esdid = d.esdid 
		inner join workflow.historicodocumento h on h.hstid = d.hstid 
		inner join sisindigena2.folhapagamento f on f.fpbid = p.fpbid 
		where d.esdid in(".ESD_PAGAMENTO_AGUARDANDO_PAGAMENTO.",".ESD_PAGAMENTO_ENVIADOBANCO.",".ESD_PAGAMENTO_AG_AUTORIZACAO_SGB.") and h.htddata <= (now()- interval '10 days')";

$bolsassematualizacao = $db->carregar($sql);

if($bolsassematualizacao[0]) {
	foreach($bolsassematualizacao as $bolsas) {
		$_bolsasanalise[$bolsas['iuscpf']][$bolsas['fpbanoreferencia']][$bolsas['fpbmesreferencia']] = $bolsas['pboid'];
	}
	
}

if($_bolsasanalise) {
	foreach($_bolsasanalise as $iuscpf => $arr) {
		$arxml['bolsista']['autenticacao'] 		= array('sistema' => SISTEMA_SGB, 'login' => USUARIO_SGB,'senha' => SENHA_SGB);
		$arxml['bolsista']['cpf'] 				= $iuscpf;
		
		$consultarHistoricoAutorizacaoPagamento_obj = $soapClient->consultarHistoricoAutorizacaoPagamento( $arxml );

		inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcpf'=>$iuscpf,'logservico'=>'consultarHistoricoAutorizacaoPagamento'));
		
		$varrer = $consultarHistoricoAutorizacaoPagamento_obj->historicoAutorizacaoPagamento->autorizacoesPagamentos->autorizacaoPagamento;
		
		if($varrer) {
			foreach($varrer as $an) {
				if($_bolsasanalise[$iuscpf][$an->referencia->ano][$an->referencia->mes]) {
					
					if($an->situacaoPagamento->codigo=='C' || $an->situacaoPagamento->codigo=='X') {
					
						$pagamentobolsista = $db->pegaLinha("SELECT d.docid, d.esdid FROM sisindigena2.pagamentobolsista p
												  INNER JOIN workflow.documento d ON d.docid = p.docid
												  WHERE pboid='".$_bolsasanalise[$iuscpf][$an->referencia->ano][$an->referencia->mes]."'");
					
						$docid 		  = $pagamentobolsista['docid'];
						$esdid_origem = $pagamentobolsista['esdid'];
					
						if($esdid_origem) {
							$sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem='".$esdid_origem."' and esdiddestino='".ESD_PAGAMENTO_EFETIVADO."'";
							$aedid = $db->pegaUm($sql);
						}
					
					
						if($docid && $aedid) {
							echo "Pagamento #".$b['pboid']." foi enviado para Pagamento Efetivado<br>";
							$result = wf_alterarEstado( $docid, $aedid, $cmddsc = '', array());
						}
					
					}
					
					if($an->situacaoPagamento->codigo=='B') {
							
						$pagamentobolsista = $db->pegaLinha("SELECT d.docid, d.esdid FROM sisindigena2.pagamentobolsista p
												  INNER JOIN workflow.documento d ON d.docid = p.docid
												  WHERE pboid='".$_bolsasanalise[$iuscpf][$an->referencia->ano][$an->referencia->mes]."'");
							
						$docid 		  = $pagamentobolsista['docid'];
						$esdid_origem = $pagamentobolsista['esdid'];
							
						if($esdid_origem) {
							$sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem='".$esdid_origem."' and esdiddestino='".ESD_PAGAMENTO_ENVIADOBANCO."'";
							$aedid = $db->pegaUm($sql);
						}
							
							
						if($docid && $aedid) {
							echo "Pagamento #".$b['pboid']." foi enviado para banco<br>";
							$result = wf_alterarEstado( $docid, $aedid, $cmddsc = '', array());
						}
							
					}
						
					
				}
			}
		}
		
	}
}

$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='sisindigena2_consultar_pagamentos_e.php'";
$db->executar($sql);
$db->commit();


$db->close();
	
if($_SESSION['usucpf'] == '00000000191') {
	
	unset($_SESSION['usucpf']);
	unset($_SESSION['usucpforigem']);
	
}


echo "fim";


?>
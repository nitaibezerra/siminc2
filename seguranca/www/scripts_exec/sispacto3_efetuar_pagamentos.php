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
require_once APPRAIZ . "www/sispacto3/_funcoes.php";
require_once APPRAIZ . "www/sispacto3/_constantes.php";
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

$sql = "SELECT * FROM sispacto3.remessapagamento WHERE remprocessada=FALSE";
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
			
			$sql = "UPDATE sispacto3.remessapagamento SET remprocessada=true, remdtprocessamento='".formata_data_sql($consultarRemessaDePagamentos_obj->remessa->situacao->data)."' WHERE remid='".$consultarRemessaDePagamentos_obj->remessa->id."'";
			$db->executar($sql);
			$db->commit();
		} elseif($logerro_consultarRemessaDePagamentos=='TRUE') {
			
			echo "Remessa com problemas : {$rem['remrastreador']}<br>";
			
			$sql = "UPDATE sispacto3.pagamentobolsista SET remid=null WHERE remid='".$rem['remid']."'";
			$db->executar($sql);
			$sql = "UPDATE sispacto3.remessapagamento SET remprocessada=true, remdtprocessamento=NOW() WHERE remid='".$rem['remid']."'";
			$db->executar($sql);
			$db->commit();
		}
		
	}
}

$sql = "SELECT p.pboid FROM sispacto3.identificacaousuario i 
		INNER JOIN sispacto3.pagamentobolsista p ON p.iusd = i.iusd 
		INNER JOIN workflow.documento d ON d.docid = p.docid 
		INNER JOIN sispacto3.remessapagamento r ON r.remid = p.remid 
		WHERE d.esdid='".ESD_PAGAMENTO_AUTORIZADO."' AND i.iustermocompromisso=true AND r.remprocessada=true";

$reenvios = $db->carregarColuna($sql);

if($reenvios) {
	foreach($reenvios as $pboid) {
		$db->executar("UPDATE sispacto3.pagamentobolsista SET remid=NULL WHERE pboid='".$pboid."'");
		$db->commit();
	}
}

$sql = "SELECT i.iusd, p.pboid, p.pboparcela, i.iuscpf, i.nacid, i.iusnome, i.iusdatanascimento, i.iusnomemae, i.iussexo, m.muncod as co_municipio_ibge_nascimento, m.estuf as sg_uf_nascimento, 
			   i.eciid, lpad(i.iusagenciasugerida,4,'0') as iusagenciasugerida, m2.muncod as co_municipio_ibge, m2.estuf as sg_uf, ie.ienlogradouro, ie.iencomplemento, 
			   ie.iennumero, ie.iencep, ie.ienbairro, it.itdufdoc, it.tdoid, it.itdnumdoc, it.itddataexp, it.itdnoorgaoexp, i.iusemailprincipal, u.unicnpj, u.uninome, u.muncod as co_municipio_entidade, u.uniuf, 
			   ff.fpbanoreferencia, ff.fpbmesreferencia, fpu.rfuparcela, p.pbovlrpagamento, pp.plpcodfuncaosgb, d.docid 
		FROM sispacto3.identificacaousuario i 
		LEFT JOIN territorios.municipio m ON m.muncod = i.muncod 
		INNER JOIN sispacto3.pagamentobolsista p ON p.iusd = i.iusd 
		INNER JOIN sispacto3.pagamentoperfil pp ON pp.pflcod = p.pflcod 
		INNER JOIN sispacto3.folhapagamento ff ON ff.fpbid = p.fpbid
		INNER JOIN sispacto3.universidade u ON u.uniid = p.uniid 
		INNER JOIN sispacto3.universidadecadastro un ON un.uniid = u.uniid  
		INNER JOIN sispacto3.folhapagamentouniversidade fpu ON fpu.uncid = un.uncid AND fpu.fpbid = ff.fpbid AND pp.pflcod = fpu.pflcod  
		INNER JOIN workflow.documento d ON d.docid = p.docid 
		LEFT JOIN sispacto3.identificaoendereco ie ON ie.iusd = i.iusd 
		LEFT JOIN territorios.municipio m2 ON m2.muncod = ie.muncod 
		LEFT JOIN sispacto3.identusutipodocumento it ON it.iusd = i.iusd 
		WHERE d.esdid='".ESD_PAGAMENTO_AUTORIZADO."' and i.iustermocompromisso=true and p.remid IS NULL LIMIT 10000";

$listapagamentos = $db->carregar( $sql );

if($listapagamentos[0]) {
	foreach($listapagamentos as $pg) {
		$_listapagamento[$pg['fpbanoreferencia']."-".$pg['fpbmesreferencia']][] = $pg;
	}
}

if($_listapagamento) {
	foreach($_listapagamento as $per => $pagamentos) {
		
		$sql = "INSERT INTO sispacto3.remessapagamento(remdata) VALUES (NOW()) RETURNING remid;";
		$remid = $db->pegaUm($sql);
		
		$pers = explode("-",$per);
		$arxml['remessa']['autenticacao']    = array('sistema' => SISTEMA_SGB, 'login' => USUARIO_SGB,'senha' => SENHA_SGB);
		$arxml['remessa']['id']    			 = $remid;
		$arxml['remessa']['programa'] 	     = PROGRAMA_SGB;
		$arxml['remessa']['vigencia']        = array('mes' => $pers[1], 'ano' => $pers[0]);
		$arxml['remessa']['lote']            = 'P';
		
		foreach($pagamentos as $pg) {
			$arxmlpg['bolsista']      = array('cpf' => $pg['iuscpf'],'codigoDaFuncao' => $pg['plpcodfuncaosgb']);
			$arxmlpg['entidade']      = array('cnpj' => $pg['unicnpj'],'uf' => $pg['uniuf'],'codigoIbgeMunicipio' => $pg['co_municipio_entidade']);
			$arxmlpg['valor']         = $pg['pbovlrpagamento'];
			$arxmlpg['parcela'] 	  = (($pg['pboparcela'])?$pg['pboparcela']:$pg['rfuparcela']);
			$arxmlpg['id'] 			  = $pg['pboid'];
			$arxml['remessa']['pagamentos'][] = $arxmlpg;
			
			$sql = "UPDATE sispacto3.pagamentobolsista SET remid='".$remid."' WHERE iusd='".$pg['iusd']."'";
			$db->executar($sql);
		}
		
   		$enviarRemessaDePagamentos_obj = $soapClient->enviarRemessaDePagamentos( $arxml );
   		
   		unset($arxml);
   		
		$logerro_enviarRemessaDePagamentos = (($enviarRemessaDePagamentos_obj->reciboEnvio->mensagem->codigo=='10001')?'FALSE':'TRUE');
    	
    	if($logerro_enviarRemessaDePagamentos=='FALSE') {
    		
    		echo "Remessa criada com sucesso : ".$enviarRemessaDePagamentos_obj->reciboEnvio->rastreador." (".count($pagamentos)." registros)<br>";
    		
			$sql = "UPDATE sispacto3.remessapagamento SET remano='".$pers[0]."', remmes='".$pers[1]."', remrastreador='".$enviarRemessaDePagamentos_obj->reciboEnvio->rastreador."' WHERE remid='".$remid."'";
			$db->executar($sql);
			$db->commit();
    	} else {
    		
    		echo "Remessa criada (#".$remid.") foi cancelada<br>";
    		
    		$db->rollback();
    	}
		
		inserirDadosLog(array('logerro'=>$logerro_enviarRemessaDePagamentos,'logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'remid'=>$remid,'logservico'=>'enviarRemessaDePagamentos'));
    	
	}
}

$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='sispacto3_efetuar_pagamentos.php'";
$db->executar($sql);
$db->commit();

if($_SESSION['usucpf'] == '00000000191') {
	
	unset($_SESSION['usucpf']);
	unset($_SESSION['usucpforigem']);
	
}


$db->close();

echo "fim";


?>
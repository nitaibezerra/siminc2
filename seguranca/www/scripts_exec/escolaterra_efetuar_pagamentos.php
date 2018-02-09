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
require_once APPRAIZ . "www/escolaterra/_funcoes.php";
require_once APPRAIZ . "www/escolaterra/_constantes.php";
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
    
ini_set("memory_limit", "2048M");

// abre conexção com o servidor de banco de dados
$db = new cls_banco();

$sql = "SELECT * FROM escolaterra.remessapagamento WHERE remprocessada=FALSE";
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
			
			$sql = "UPDATE escolaterra.remessapagamento SET remprocessada=true, remdtprocessamento='".formata_data_sql($consultarRemessaDePagamentos_obj->remessa->situacao->data)."' WHERE remid='".$consultarRemessaDePagamentos_obj->remessa->id."'";
			$db->executar($sql);
			$db->commit();
		} elseif($logerro_consultarRemessaDePagamentos=='TRUE') {
			
			echo "Remessa com problemas : {$rem['remrastreador']}<br>";
			
			$sql = "UPDATE escolaterra.pagamentobolsista SET remid=null WHERE remid='".$rem['remid']."'";
			$db->executar($sql);
			$sql = "UPDATE escolaterra.remessapagamento SET remprocessada=true, remdtprocessamento=NOW() WHERE remid='".$rem['remid']."'";
			$db->executar($sql);
			$db->commit();
		}
		
	}
}

$sql = "SELECT p.pboid FROM escolaterra.identificacaousuario i 
		INNER JOIN escolaterra.pagamentobolsista p ON p.iusid = i.iusid 
		INNER JOIN workflow.documento d ON d.docid = p.docid 
		INNER JOIN escolaterra.remessapagamento r ON r.remid = p.remid 
		WHERE d.esdid='".ESD_PAGAMENTO_AUTORIZADO."' AND i.iustermocompromisso=true AND r.remprocessada=true";

$reenvios = $db->carregarColuna($sql);

if($reenvios) {
	foreach($reenvios as $pboid) {
		$db->executar("UPDATE escolaterra.pagamentobolsista SET remid=NULL WHERE pboid='".$pboid."'");
		$db->commit();
	}
}

$sql = "SELECT i.iusid, p.pboid, p.pboparcela, i.iuscpf, i.nacid, i.iusnome, i.iusdatanascimento, i.iusnomemae, i.iussexo, m.muncod as co_municipio_ibge_nascimento, m.estuf as sg_uf_nascimento, 
			   i.eciid, lpad(i.iusagenciasugerida,4,'0') as iusagenciasugerida, m2.muncod as co_municipio_ibge, m2.estuf as sg_uf, ie.ienlogradouro, ie.iencomplemento, 
			   ie.iennumero, ie.iencep, ie.ienbairro, it.itdufdoc, it.tdoid, it.itdnumdoc, it.itddataexp, it.itdnoorgaoexp, i.iusemailprincipal,  
			   ff.fpbanoreferencia, ff.fpbmesreferencia, fpu.rfuparcela, p.pbovlrpagamento, pp.plpcodfuncaosgb, d.docid,  
			   CASE WHEN i.iusrede='E' THEN un.ufpcnpj 
					WHEN i.iusrede='M' THEN ent.entnumcpfcnpj 
					ELSE '' END as unicnpj,
			   CASE WHEN i.iusrede='E' THEN un.estuf 
					WHEN i.iusrede='M' THEN mm.estuf 
					ELSE '' END as uniuf, 
			   CASE WHEN i.iusrede='E' THEN un.muncod 
					WHEN i.iusrede='M' THEN mm.muncod 
					ELSE '' END as co_municipio_entidade
		
		FROM escolaterra.identificacaousuario i 
		LEFT JOIN territorios.municipio m ON m.muncod = i.muncod 
		INNER JOIN escolaterra.pagamentobolsista p ON p.iusid = i.iusid 
		INNER JOIN escolaterra.pagamentoperfil pp ON pp.pflcod = p.pflcod 
		INNER JOIN escolaterra.periodoreferencia ff ON ff.fpbid = p.fpbid
		INNER JOIN escolaterra.ufparticipantes un ON un.ufpid = i.ufpid  
		INNER JOIN escolaterra.periodoreferenciauf fpu ON fpu.ufpid = un.ufpid AND fpu.fpbid = ff.fpbid 
		INNER JOIN workflow.documento d ON d.docid = p.docid 
		INNER JOIN par.entidade ent ON ent.muncod = i.muncodatuacao AND ent.dutid=6
		INNER JOIN escolaterra.entidadecadastro enc ON enc.entid = ent.entid AND enc.cadastradosgb=true
		INNER JOIN territorios.municipio mm ON mm.muncod = ent.muncod 
		LEFT JOIN escolaterra.identificaoendereco ie ON ie.iusid = i.iusid 
		LEFT JOIN territorios.municipio m2 ON m2.muncod = ie.muncod 
		LEFT JOIN escolaterra.identusutipodocumento it ON it.iusid = i.iusid 
		WHERE d.esdid='".ESD_PAGAMENTO_AUTORIZADO."' and i.iustermocompromisso=true and p.remid IS NULL and i.iusrede IS NOT NULL LIMIT 10000";

$listapagamentos = $db->carregar( $sql );

if($listapagamentos[0]) {
	foreach($listapagamentos as $pg) {
		$_listapagamento[$pg['fpbanoreferencia']."-".$pg['fpbmesreferencia']][] = $pg;
	}
}

if($_listapagamento) {
	foreach($_listapagamento as $per => $pagamentos) {
		
		$sql = "INSERT INTO escolaterra.remessapagamento(remdata) VALUES (NOW()) RETURNING remid;";
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
			//$arxmlpg['entidade']      = array('cnpj' => '','uf' => '','codigoIbgeMunicipio' => '');
			$arxmlpg['valor']         = $pg['pbovlrpagamento'];
			$arxmlpg['parcela'] 	  = (($pg['pboparcela'])?$pg['pboparcela']:$pg['rfuparcela']);
			$arxmlpg['id'] 			  = $pg['pboid'];
			$arxml['remessa']['pagamentos'][] = $arxmlpg;
			
			$sql = "UPDATE escolaterra.pagamentobolsista SET remid='".$remid."' WHERE iusid='".$pg['iusid']."'";
			$db->executar($sql);
		}
		
   		$enviarRemessaDePagamentos_obj = $soapClient->enviarRemessaDePagamentos( $arxml );
   		
   		unset($arxml);
   		
		$logerro_enviarRemessaDePagamentos = (($enviarRemessaDePagamentos_obj->reciboEnvio->mensagem->codigo=='10001')?'FALSE':'TRUE');
    	
    	if($logerro_enviarRemessaDePagamentos=='FALSE') {
    		
    		echo "Remessa criada com sucesso : ".$enviarRemessaDePagamentos_obj->reciboEnvio->rastreador." (".count($pagamentos)." registros)<br>";
    		
			$sql = "UPDATE escolaterra.remessapagamento SET remano='".$pers[0]."', remmes='".$pers[1]."', remrastreador='".$enviarRemessaDePagamentos_obj->reciboEnvio->rastreador."' WHERE remid='".$remid."'";
			$db->executar($sql);
			$db->commit();
    	} else {
    		
    		echo "Remessa criada (#".$remid.") foi cancelada<br>";
    		
    		$db->rollback();
    	}
		
		inserirDadosLog(array('logerro'=>$logerro_enviarRemessaDePagamentos,'logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'remid'=>$remid,'logservico'=>'enviarRemessaDePagamentos'));
    	
	}
}


if($_SESSION['usucpf'] == '00000000191') {
	
	unset($_SESSION['usucpf']);
	unset($_SESSION['usucpforigem']);
	
}

echo "fim";


?>
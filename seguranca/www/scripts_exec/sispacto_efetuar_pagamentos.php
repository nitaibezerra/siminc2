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
require_once APPRAIZ . "www/sispacto/_funcoes.php";
require_once APPRAIZ . "www/sispacto/_constantes.php";
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

$sql = "SELECT * FROM sispacto.remessapagamento WHERE remprocessada=FALSE";
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
			
			$sql = "UPDATE sispacto.remessapagamento SET remprocessada=true, remdtprocessamento='".formata_data_sql($consultarRemessaDePagamentos_obj->remessa->situacao->data)."' WHERE remid='".$consultarRemessaDePagamentos_obj->remessa->id."'";
			$db->executar($sql);
			$db->commit();
		} elseif($logerro_consultarRemessaDePagamentos=='TRUE') {
			
			echo "Remessa com problemas : {$rem['remrastreador']}<br>";
			
			$sql = "UPDATE sispacto.pagamentobolsista SET remid=null WHERE remid='".$rem['remid']."'";
			$db->executar($sql);
			$sql = "UPDATE sispacto.remessapagamento SET remprocessada=true, remdtprocessamento=NOW() WHERE remid='".$rem['remid']."'";
			$db->executar($sql);
			$db->commit();
		}
		
	}
}

$sql = "SELECT p.pboid FROM sispacto.identificacaousuario i 
		INNER JOIN sispacto.pagamentobolsista p ON p.iusd = i.iusd 
		INNER JOIN workflow.documento d ON d.docid = p.docid 
		INNER JOIN sispacto.remessapagamento r ON r.remid = p.remid 
		WHERE d.esdid='".ESD_PAGAMENTO_AUTORIZADO."' AND i.iustermocompromisso=true AND r.remprocessada=true";

$reenvios = $db->carregarColuna($sql);

if($reenvios) {
	foreach($reenvios as $pboid) {
		$db->executar("UPDATE sispacto.pagamentobolsista SET remid=NULL WHERE pboid='".$pboid."'");
		$db->commit();
	}
}

$sql = "SELECT i.iusd, p.pboid, p.pboparcela, i.iuscpf, i.nacid, i.iusnome, i.iusdatanascimento, i.iusnomemae, i.iussexo, m.muncod as co_municipio_ibge_nascimento, m.estuf as sg_uf_nascimento, 
			   i.eciid, lpad(i.iusagenciasugerida,4,'0') as iusagenciasugerida, m2.muncod as co_municipio_ibge, m2.estuf as sg_uf, ie.ienlogradouro, ie.iencomplemento, 
			   ie.iennumero, ie.iencep, ie.ienbairro, it.itdufdoc, it.tdoid, it.itdnumdoc, it.itddataexp, it.itdnoorgaoexp, i.iusemailprincipal, u.unicnpj, u.uninome, u.muncod as co_municipio_entidade, u.uniuf, 
			   ff.fpbanoreferencia, ff.fpbmesreferencia, fpu.rfuparcela, p.pbovlrpagamento, pp.plpcodfuncaosgb, d.docid 
		FROM sispacto.identificacaousuario i 
		LEFT JOIN territorios.municipio m ON m.muncod = i.muncod 
		INNER JOIN sispacto.pagamentobolsista p ON p.iusd = i.iusd 
		INNER JOIN sispacto.pagamentoperfil pp ON pp.pflcod = p.pflcod 
		INNER JOIN sispacto.folhapagamento ff ON ff.fpbid = p.fpbid
		INNER JOIN sispacto.universidade u ON u.uniid = p.uniid 
		INNER JOIN sispacto.universidadecadastro un ON un.uniid = u.uniid  
		INNER JOIN sispacto.folhapagamentouniversidade fpu ON fpu.uncid = un.uncid AND fpu.fpbid = ff.fpbid 
		INNER JOIN workflow.documento d ON d.docid = p.docid 
		LEFT JOIN sispacto.identificaoendereco ie ON ie.iusd = i.iusd 
		LEFT JOIN territorios.municipio m2 ON m2.muncod = ie.muncod 
		LEFT JOIN sispacto.identusutipodocumento it ON it.iusd = i.iusd 
		WHERE d.esdid='".ESD_PAGAMENTO_AUTORIZADO."' and i.iustermocompromisso=true and p.remid IS NULL LIMIT 10000";

$listapagamentos = $db->carregar( $sql );

if($listapagamentos[0]) {
	foreach($listapagamentos as $pg) {
		$_listapagamento[$pg['fpbanoreferencia']."-".$pg['fpbmesreferencia']][] = $pg;
	}
}

if($_listapagamento) {
	foreach($_listapagamento as $per => $pagamentos) {
		
		$sql = "INSERT INTO sispacto.remessapagamento(remdata) VALUES (NOW()) RETURNING remid;";
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
			
			$sql = "UPDATE sispacto.pagamentobolsista SET remid='".$remid."' WHERE iusd='".$pg['iusd']."'";
			$db->executar($sql);
		}
		
   		$enviarRemessaDePagamentos_obj = $soapClient->enviarRemessaDePagamentos( $arxml );
   		
   		unset($arxml);
   		
		$logerro_enviarRemessaDePagamentos = (($enviarRemessaDePagamentos_obj->reciboEnvio->mensagem->codigo=='10001')?'FALSE':'TRUE');
    	
    	if($logerro_enviarRemessaDePagamentos=='FALSE') {
    		
    		echo "Remessa criada com sucesso : ".$enviarRemessaDePagamentos_obj->reciboEnvio->rastreador." (".count($pagamentos)." registros)<br>";
    		
			$sql = "UPDATE sispacto.remessapagamento SET remano='".$pers[0]."', remmes='".$pers[1]."', remrastreador='".$enviarRemessaDePagamentos_obj->reciboEnvio->rastreador."' WHERE remid='".$remid."'";
			$db->executar($sql);
			$db->commit();
    	} else {
    		
    		echo "Remessa criada (#".$remid.") foi cancelada<br>";
    		
    		$db->rollback();
    	}
		
		inserirDadosLog(array('logerro'=>$logerro_enviarRemessaDePagamentos,'logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'remid'=>$remid,'logservico'=>'enviarRemessaDePagamentos'));
    	
	}
}

if(!$_REQUEST['naoProcessar']) {

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
				
				$pboid = $pgs->id;
				
				if(count($pgs->situacoes->situacao)>1) {
					$pg = end($pgs->situacoes->situacao);
				} else {
					$pg = $pgs->situacoes->situacao;
				}
				
				if($pg->codigo==SGB_AUTORIZADA || $pg->codigo==SGB_HOMOLOGADA || $pg->codigo==SGB_PREAPROVADA || $pg->codigo==SGB_ENVIADOAOSIGEF) {
					$docid = $db->pegaUm("SELECT p.docid FROM sispacto.pagamentobolsista p 
										  INNER JOIN workflow.documento d ON d.docid = p.docid 
										  WHERE pboid='".$pboid."' AND d.esdid='".ESD_PAGAMENTO_AG_AUTORIZACAO_SGB."'");
					if($docid) {
						echo "Pagamento #".$pboid." (".$pg->data.") foi enviado para Aguardando pagamento<br>";
						$result = wf_alterarEstado( $docid, AED_AUTORIZARSGB_PAGAMENTO, $cmddsc = '', array());
					} else {
						echo "Pagamento #".$pboid." (".$pg->data.") NÃO foi enviado para Aguardando pagamento<br>";
					}
				}
				
				if($pg->codigo==SGB_ENVIADOBANCO) {
					$docid = $db->pegaUm("SELECT p.docid FROM sispacto.pagamentobolsista p 
										  INNER JOIN workflow.documento d ON d.docid = p.docid 
										  WHERE pboid='".$pboid."' AND d.esdid='".ESD_PAGAMENTO_AGUARDANDO_PAGAMENTO."'");
					if($docid) {
						echo "Pagamento #".$pboid." (".$pg->data.") foi enviado para Enviado ao Banco<br>";
						$result = wf_alterarEstado( $docid, AED_ENVIARBANCO_PAGAMENTO, $cmddsc = '', array());
					} else {
						echo "Pagamento #".$pboid." (".$pg->data.") NÃO foi enviado para Enviado ao Banco<br>";
					}
				}
				
					
				if($pg->codigo==SGB_CREDITADA || $pg->codigo==SGB_SACADA || $pg->codigo==SGB_RESTITUIDO) {
					
					$pagamentobolsista = $db->pegaLinha("SELECT d.docid, d.esdid FROM sispacto.pagamentobolsista p 
													  INNER JOIN workflow.documento d ON d.docid = p.docid 
													  WHERE pboid='".$pboid."'");
					
					$docid 		  = $pagamentobolsista['docid'];
					$esdid_origem = $pagamentobolsista['esdid'];
					
					$sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem='".$esdid_origem."' and esdiddestino='".ESD_PAGAMENTO_EFETIVADO."'";
					$aedid = $db->pegaUm($sql);
					
					
					if($docid && $aedid) {
						echo "Pagamento #".$pboid." (".$pg->data.") foi enviado para Pagamento Efetivado<br>";
						$result = wf_alterarEstado( $docid, $aedid, $cmddsc = '', array());
					}
	
				}
			}
		}
		
		inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logservico'=>'consultarSituacaoDePagamentos'));
		
	}
	
	$sql = "UPDATE sispacto.mensarioavaliacoes ma SET mavtotal=foo.total FROM (
			SELECT * FROM (
			SELECT 
			mavid,
			mavfrequencia,
			mavatividadesrealizadas,
			mavmonitoramento,
			mavtotal,
			(COALESCE((mavfrequencia*fatfrequencia),0) + COALESCE((mavatividadesrealizadas*fatatividadesrealizadas),0) + COALESCE(mavmonitoramento,0)) as total
			FROM sispacto.mensarioavaliacoes ma 
			INNER JOIN sispacto.mensario m ON m.menid = ma.menid 
			INNER JOIN sispacto.identificacaousuario u ON u.iusd = m.iusd 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = u.iusd 
			INNER JOIN sispacto.fatoresdeavaliacao f ON f.fatpflcodavaliado = t.pflcod 
			) fee
			WHERE fee.mavtotal != total
			) foo 
			WHERE ma.mavid = foo.mavid";
	
	$db->executar($sql);
	$db->commit();
	
	
	$sql = "update sispacto.mensarioavaliacoes x set mavmonitoramento=foo.fatmonitoramento from (
	
			select mm.*, f.*, d.esdid from sispacto.mensario m 
			inner join sispacto.tipoperfil t on t.iusd = m.iusd and t.pflcod!=849 
			INNER JOIN sispacto.fatoresdeavaliacao f ON f.fatpflcodavaliado = t.pflcod
			inner join workflow.documento d on d.docid = m.docid and d.esdid in(657,601)
			inner join sispacto.mensarioavaliacoes mm on mm.menid = m.menid 
			where mavmonitoramento=0
			
			) foo where foo.mavid = x.mavid";
	
	$db->executar($sql);
	$db->commit();
	
	
	if(date("dd")=='14') {
		$sql = "UPDATE sispacto.identificacaousuario x set iustipoorientador=foo3.tipo from (
					SELECT * FROM (
					select CASE WHEN trim(foo.proletramento) IS NOT NULL THEN 'tutoresproletramento' 
								       WHEN trim(foo.semproletramento) IS NOT NULL THEN 'tutoresredesemproletramento' 
								       ELSE 'profissionaismagisterio' END as tipo, iusd, iustipoorientador, iuscpf
					from (
					select i.iuscpf, 
					       i.iusd, 
					       i.iustipoorientador, 
					       (select cpf from sispacto.tutoresproletramento where cpf=i.iuscpf limit 1) as proletramento, 
					       (select cpf from sispacto.tutoressemproletramento where cpf=i.iuscpf limit 1) as semproletramento 
					from sispacto.identificacaousuario i 
					inner join sispacto.tipoperfil t on t.iusd = i.iusd 
					where t.pflcod=".PFL_ORIENTADORESTUDO.") foo) foo2 WHERE foo2.tipo!=foo2.iustipoorientador
					) foo3 WHERE x.iusd=foo3.iusd";
		
		$db->executar($sql);
		$db->commit();
	}

}


if($_SESSION['usucpf'] == '00000000191') {
	
	unset($_SESSION['usucpf']);
	unset($_SESSION['usucpforigem']);
	
}

echo "fim";


?>
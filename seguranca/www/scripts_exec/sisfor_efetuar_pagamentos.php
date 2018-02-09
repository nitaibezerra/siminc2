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
require_once APPRAIZ . "www/sisfor/_constantes.php";
require_once APPRAIZ . "www/sisfor/_funcoes.php";
require_once APPRAIZ . "www/sisfor/_funcoes_pagamento.php";
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
    
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

$opcoes = Array(
		'exceptions'	=> 0,
		'trace'			=> true,
		//'encoding'		=> 'UTF-8',
		'encoding'		=> 'ISO-8859-1',
		'cache_wsdl'    => WSDL_CACHE_NONE
);

$_programas['FCSEC'] = "FCSEC";
$_programas['FCSEB'] = "FCSEB";

$soapClient = new SoapClient( WSDL_CAMINHO, $opcoes );

$sql = "SELECT sieid FROM sisfor.sisfories WHERE (siecadastrosgb=false OR siecadastrosgb IS NULL)";
$sieids = $db->carregarColuna($sql);

libxml_use_internal_errors( true );

if($sieids) {
	foreach($sieids as $sieid) {
		sincronizarDadosEntidadeSGB(array("sieid" => $sieid));
	}
}

echo "sincronização entidades SGB (".count($sieids)." registros) finalizadas<br>";

$sql = "SELECT DISTINCT i.iusd as iusd FROM sisfor.identificacaousuario i 
		INNER JOIN sisfor.tipoperfil t ON t.iusd = i.iusd 
		INNER JOIN sisfor.pagamentobolsista p ON p.iusd = i.iusd 
		WHERE i.iusstatus='A' AND i.iustermocompromisso=true and cadastradosgb=false";

$iusds = $db->carregarColuna($sql);

if($iusds) {
	foreach($iusds as $iusd) {
		sincronizarDadosUsuarioSGB(array('iusd'=>$iusd));
	}
}

echo "sincronização bolsistas SGB (".count($iusds)." registros) finalizadas<br>";

$sql = "SELECT p.docid FROM sisfor.pagamentobolsista p 
		INNER JOIN workflow.documento d ON d.docid = p.docid 
		INNER JOIN sisfor.identificacaousuario i ON i.iusd = p.iusd 
		WHERE d.esdid='".ESD_PG_AGUARDANDO_AUTORIZACAO."' AND cadastradosgb=true";

$docids = $db->carregarColuna($sql);

if($docids) {
	foreach($docids as $docid) {
		wf_alterarEstado( $docid, AED_PG_AUTORIZAR, '', array());
	}
}

echo "autorização automática das bolsas (".count($docids)." registros) finalizadas<br>";


$sql = "SELECT * FROM sisfor.remessapagamento WHERE remprocessada=FALSE";
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
			
			$sql = "UPDATE sisfor.remessapagamento SET remprocessada=true, remdtprocessamento='".formata_data_sql($consultarRemessaDePagamentos_obj->remessa->situacao->data)."' WHERE remid='".$consultarRemessaDePagamentos_obj->remessa->id."'";
			$db->executar($sql);
			$db->commit();
		} elseif($logerro_consultarRemessaDePagamentos=='TRUE') {
			
			echo "Remessa com problemas : {$rem['remrastreador']}<br>";
			
			$sql = "UPDATE sisfor.pagamentobolsista SET remid=null WHERE remid='".$rem['remid']."'";
			$db->executar($sql);
			$sql = "UPDATE sisfor.remessapagamento SET remprocessada=true, remdtprocessamento=NOW() WHERE remid='".$rem['remid']."'";
			$db->executar($sql);
			$db->commit();
		}
		
	}
}


echo "remessas pendentes (".count($remessapagamento)." registros) atualizadas<br>";

$sql = "SELECT p.pboid FROM sisfor.identificacaousuario i 
		INNER JOIN sisfor.pagamentobolsista p ON p.iusd = i.iusd 
		INNER JOIN workflow.documento d ON d.docid = p.docid 
		INNER JOIN sisfor.remessapagamento r ON r.remid = p.remid 
		WHERE d.esdid='".ESD_PAGAMENTO_AUTORIZADO."' AND i.iustermocompromisso=true AND r.remprocessada=true";

$reenvios = $db->carregarColuna($sql);

if($reenvios) {
	foreach($reenvios as $pboid) {
		$db->executar("UPDATE sisfor.pagamentobolsista SET remid=NULL WHERE pboid='".$pboid."'");
		$db->commit();
	}
}

echo "pagamentos que tiveram problemas no envio (".count($reenvios)." registros) atualizadao<br>";

$sql = "(
		
		SELECT i.iusd, p.pboid, p.pboparcela, i.iuscpf, i.nacid, i.iusnome, i.iusdatanascimento, i.iusnomemae, i.iussexo, m.muncod as co_municipio_ibge_nascimento, m.estuf as sg_uf_nascimento, 
			   i.eciid, lpad(i.iusagenciasugerida,4,'0') as iusagenciasugerida, m2.muncod as co_municipio_ibge, m2.estuf as sg_uf, ie.ienlogradouro, ie.iencomplemento, 
			   ie.iennumero, ie.iencep, ie.ienbairro, it.itdufdoc, it.tdoid, it.itdnumdoc, it.itddataexp, it.itdnoorgaoexp, i.iusemailprincipal, 
			   se.siecnpj as unicnpj, 
			   pl.unidsc as uninome, 
			   se.muncodies as co_municipio_entidade, 
			   me.estuf as uniuf,
			   ff.fpbanoreferencia, ff.fpbmesreferencia, fpu.rfuparcela, p.pbovlrpagamento, pp.plpcodfuncaosgb_fcseb, pp.plpcodfuncaosgb_fcsec, d.docid, si.sifprogramasgb 
		FROM sisfor.identificacaousuario i 
		LEFT JOIN territorios.municipio m ON m.muncod = i.muncod 
		INNER JOIN sisfor.pagamentobolsista p ON p.iusd = i.iusd 
		INNER JOIN sisfor.pagamentoperfil pp ON pp.pflcod = p.pflcod 
		INNER JOIN sisfor.folhapagamento ff ON ff.fpbid = p.fpbid
		INNER JOIN sisfor.tipoperfil tp ON tp.tpeid = p.tpeid and tpebolsa=true 
		INNER JOIN sisfor.sisfor si ON si.sifid = tp.sifid 
		INNER JOIN sisfor.sisfories se ON se.unicod = si.unicod 
		INNER JOIN public.unidade pl ON pl.unicod = se.unicod 
		LEFT JOIN territorios.municipio me ON me.muncod = se.muncodies
		INNER JOIN sisfor.folhapagamentoprojeto fpu ON fpu.sifid = tp.sifid AND fpu.fpbid = ff.fpbid  
		INNER JOIN workflow.documento d ON d.docid = p.docid 
		LEFT JOIN sisfor.identificaoendereco ie ON ie.iusd = i.iusd 
		LEFT JOIN territorios.municipio m2 ON m2.muncod = ie.muncod 
		LEFT JOIN sisfor.identusutipodocumento it ON it.iusd = i.iusd 
		WHERE d.esdid='".ESD_PAGAMENTO_AUTORIZADO."' and i.iustermocompromisso=true and siecnpj IS NOT NULL and p.remid IS NULL LIMIT 10000
				
		) UNION ALL (
				
		SELECT i.iusd, p.pboid, p.pboparcela, i.iuscpf, i.nacid, i.iusnome, i.iusdatanascimento, i.iusnomemae, i.iussexo, m.muncod as co_municipio_ibge_nascimento, m.estuf as sg_uf_nascimento, 
			   i.eciid, lpad(i.iusagenciasugerida,4,'0') as iusagenciasugerida, m2.muncod as co_municipio_ibge, m2.estuf as sg_uf, ie.ienlogradouro, ie.iencomplemento, 
			   ie.iennumero, ie.iencep, ie.ienbairro, it.itdufdoc, it.tdoid, it.itdnumdoc, it.itddataexp, it.itdnoorgaoexp, i.iusemailprincipal, 
			   se.siecnpj as unicnpj, 
			   pl.unidsc as uninome, 
			   se.muncodies as co_municipio_entidade, 
			   me.estuf as uniuf,
			   ff.fpbanoreferencia, ff.fpbmesreferencia, rm.remparcela as rfuparcela, p.pbovlrpagamento, pp.plpcodfuncaosgb_fcseb, pp.plpcodfuncaosgb_fcsec, d.docid, 'FCSEB' as sifprogramasgb 
		FROM sisfor.identificacaousuario i 
		LEFT JOIN territorios.municipio m ON m.muncod = i.muncod 
		INNER JOIN sisfor.pagamentobolsista p ON p.iusd = i.iusd  and p.pflcod=".PFL_COORDENADOR_INST."
		INNER JOIN sisfor.pagamentoperfil pp ON pp.pflcod = p.pflcod
		INNER JOIN sisfor.folhapagamento ff ON ff.fpbid = p.fpbid
		INNER JOIN sisfor.tipoperfil tp ON tp.tpeid = p.tpeid and tpebolsa=true 
		INNER JOIN sisfor.sisfories se ON se.usucpf = i.iuscpf 
		INNER JOIN public.unidade pl ON pl.unicod = se.unicod 
		LEFT JOIN territorios.municipio me ON me.muncod = se.muncodies
		INNER JOIN sisfor.relatoriomensal rm ON rm.iusd = i.iusd AND rm.fpbid = p.fpbid and rm.remparcela is not null  
		INNER JOIN workflow.documento d ON d.docid = p.docid 
		LEFT JOIN sisfor.identificaoendereco ie ON ie.iusd = i.iusd 
		LEFT JOIN territorios.municipio m2 ON m2.muncod = ie.muncod 
		LEFT JOIN sisfor.identusutipodocumento it ON it.iusd = i.iusd 
		WHERE d.esdid='".ESD_PAGAMENTO_AUTORIZADO."' and i.iustermocompromisso=true and siecnpj IS NOT NULL and p.remid IS NULL
		
		)";

$listapagamentos = $db->carregar( $sql );

if($listapagamentos[0]) {
	foreach($listapagamentos as $pg) {
		$_listapagamento[$pg['fpbanoreferencia']."-".$pg['fpbmesreferencia']][$pg['sifprogramasgb']][] = $pg;
	}
}

if($_listapagamento) {
	foreach($_listapagamento as $per => $pagamentos1) {
		
		foreach($pagamentos1 as $programa => $pagamentos) {
		
			$sql = "INSERT INTO sisfor.remessapagamento(remdata) VALUES (NOW()) RETURNING remid;";
			$remid = $db->pegaUm($sql);
			
			$pers = explode("-",$per);
			$arxml['remessa']['autenticacao']    = array('sistema' => SISTEMA_SGB, 'login' => USUARIO_SGB,'senha' => SENHA_SGB);
			$arxml['remessa']['id']    			 = $remid;
			$arxml['remessa']['programa'] 	     = $programa;
			$arxml['remessa']['vigencia']        = array('mes' => $pers[1], 'ano' => $pers[0]);
			$arxml['remessa']['lote']            = 'P';
			
			foreach($pagamentos as $pg) {
				$arxmlpg['bolsista']      = array('cpf' => $pg['iuscpf'],'codigoDaFuncao' => $pg['plpcodfuncaosgb_'.strtolower($programa)]);
				$arxmlpg['entidade']      = array('cnpj' => $pg['unicnpj'],'uf' => $pg['uniuf'],'codigoIbgeMunicipio' => $pg['co_municipio_entidade']);
				$arxmlpg['valor']         = $pg['pbovlrpagamento'];
				$arxmlpg['parcela'] 	  = (($pg['pboparcela'])?$pg['pboparcela']:$pg['rfuparcela']);
				$arxmlpg['id'] 			  = $pg['pboid'];
				$arxml['remessa']['pagamentos'][] = $arxmlpg;
				
				$sql = "UPDATE sisfor.pagamentobolsista SET remid='".$remid."' WHERE iusd='".$pg['iusd']."'";
				$db->executar($sql);
			}
			
	   		$enviarRemessaDePagamentos_obj = $soapClient->enviarRemessaDePagamentos( $arxml );
	   		
	   		unset($arxml);
	   		
			$logerro_enviarRemessaDePagamentos = (($enviarRemessaDePagamentos_obj->reciboEnvio->mensagem->codigo=='10001')?'FALSE':'TRUE');
	    	
	    	if($logerro_enviarRemessaDePagamentos=='FALSE') {
	    		
	    		echo "Remessa criada com sucesso : ".$enviarRemessaDePagamentos_obj->reciboEnvio->rastreador." (".count($pagamentos)." registros)<br>";
	    		
				$sql = "UPDATE sisfor.remessapagamento SET remano='".$pers[0]."', remmes='".$pers[1]."', remrastreador='".$enviarRemessaDePagamentos_obj->reciboEnvio->rastreador."' WHERE remid='".$remid."'";
				$db->executar($sql);
				$db->commit();
	    	} else {
	    		
	    		echo "Remessa criada (#".$remid.") foi cancelada<br>";
	    		
	    		$db->rollback();
	    	}
			
			inserirDadosLog(array('logerro'=>$logerro_enviarRemessaDePagamentos,'logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'remid'=>$remid,'logservico'=>'enviarRemessaDePagamentos'));
		
		}
    	
	}
}

echo "remessas foram criadas com sucesso<br>";

if(!$_REQUEST['naoProcessar']) {

	if(!$_REQUEST['numeroDiasProcessamento']) $numeroDiasProcessamento = 30;
	else $numeroDiasProcessamento = $_REQUEST['numeroDiasProcessamento'];
	
	for($i=1;$i<=$numeroDiasProcessamento;$i++) {
		$datasel[] = "to_char(NOW(),'YYYY-mm-dd')::date - interval '".$i." day' as data".$i;
	}
	
	$datas = $db->pegaLinha("select ".implode(",",$datasel));
	
	if($_programas) {
		
		echo "<pre>";
		echo "Programas cadastrados : ";
		print_r($_programas);
		
		foreach($_programas as $prg) {
			
			for($i=$numeroDiasProcessamento;$i>=1;$i--) {
				$arxml['situacoes']['autenticacao'] 		= array('sistema' => SISTEMA_SGB, 'login' => USUARIO_SGB,'senha' => SENHA_SGB);
				$arxml['situacoes']['programa'] 			= $prg;
				$arxml['situacoes']['dataDasAlteracoes'] 	= formata_data($datas['data'.$i]);
				
				echo "consultando... ".formata_data($datas['data'.$i])."<br>";
				
				$consultarSituacaoDePagamentos_obj = $soapClient->consultarSituacaoDePagamentos( $arxml );
				
				if($consultarSituacaoDePagamentos_obj->situacoes->pagamentos->pagamento) {
					foreach($consultarSituacaoDePagamentos_obj->situacoes->pagamentos->pagamento as $pgs) {
						
						echo "<pre>";
						echo "Pagamentos : ";
						print_r($pgs);
						
						
						$pboid = $pgs->id;
						
						if(count($pgs->situacoes->situacao)>1) {
							$pg = end($pgs->situacoes->situacao);
						} else {
							$pg = $pgs->situacoes->situacao;
						}
						
						if($pg->codigo==SGB_AUTORIZADA || $pg->codigo==SGB_HOMOLOGADA || $pg->codigo==SGB_PREAPROVADA || $pg->codigo==SGB_ENVIADOAOSIGEF) {
							$docid = $db->pegaUm("SELECT p.docid FROM sisfor.pagamentobolsista p 
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
							$docid = $db->pegaUm("SELECT p.docid FROM sisfor.pagamentobolsista p 
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
							
							$pagamentobolsista = $db->pegaLinha("SELECT d.docid, d.esdid FROM sisfor.pagamentobolsista p 
															  INNER JOIN workflow.documento d ON d.docid = p.docid 
															  WHERE pboid='".$pboid."'");
							
							$docid 		  = $pagamentobolsista['docid'];
							$esdid_origem = $pagamentobolsista['esdid'];
							
							if($esdid_origem) {
								$sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem='".$esdid_origem."' and esdiddestino='".ESD_PAGAMENTO_EFETIVADO."'";
								$aedid = $db->pegaUm($sql);
							}
							
							
							if($docid && $aedid) {
								echo "Pagamento #".$pboid." (".$pg->data.") foi enviado para Pagamento Efetivado<br>";
								$result = wf_alterarEstado( $docid, $aedid, $cmddsc = '', array());
							}
			
						}
					}
				}
				
				inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logservico'=>'consultarSituacaoDePagamentos'));
				
			}
		
		}
	
	}
	


}

$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='sisfor_efetuar_pagamentos.php'";
$db->executar($sql);
$db->commit();

$db->close();


if($_SESSION['usucpf'] == '00000000191') {
	
	unset($_SESSION['usucpf']);
	unset($_SESSION['usucpforigem']);
	
}



echo "fim";


?>
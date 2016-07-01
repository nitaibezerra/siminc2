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
require_once APPRAIZ . "global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/projovemcampo/_constantes.php";
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

function enviaPagamentoCampo($dados){

	if($dados['perid']!=''){
		
		
		$perid = $dados['perid'];
		
		$opcoes = Array(
		                'exceptions'	=> 0,
		                'trace'			=> true,
		                //'encoding'		=> 'UTF-8',
		                'encoding'		=> 'ISO-8859-1',
		                'cache_wsdl'    => WSDL_CACHE_NONE
		);
		
		$soapClient = new SoapClient( WSDL_CAMINHO, $opcoes );
// 		ver(d);
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
		
		$sql = "
				with
					tmp_parcela as (
							SELECT 
								MAX(hip.parcela) as parcela, estid 
							FROM 
								projovemcampo.historicopagamento hip 
							INNER JOIN projovemcampo.lancamentodiario lnd ON lnd.lndid = hip.lndid 
							WHERE 
								 hip.hiprejeitado = 'f'
							GROUP By
								estid)
				SELECT 
					est.estid, lnd.lndid, est.estcpf, est.estdatanascimento, est.estnome,
					est.estnomemae, est.estsexo, m.muncod as co_municipio_ibge_nascimento, 
					m.estuf as sg_uf_nascimento, lpad(abe.agbcod::char(4),4,'0') as iusagenciasugerida,
					m2.muncod as co_municipio_ibge, m2.estuf as sg_uf, est.estendlogradouro, est.estendcomplemento, est.estendnumero, est.estendcep, 
					est.estendbairro, est.estestufemissao, est.estnumrg, 
					est.estdataemissaorg, estorgaoexpedidorg, est.estemail, sec.secacnpj,
					secamuncod as co_municipio_entidade, m2.estuf as secuf, apr.apranoreferencia, 
					extract(month from rap.datafim)as mesref, 
					case	
						when par.parcela is null
						then 1
						else  par.parcela + 1
					end as parcela,
					'100' as valorpagamento, '52' as codfucaosgb, d.docid
				FROM
					projovemcampo.estudante est 
				INNER JOIN projovemcampo.lancamentodiario lnd ON lnd.estid = est.estid
				LEFT JOIN tmp_parcela par on par.estid = est.estid 
				INNER JOIN workflow.documento d ON d.docid = lnd.docid 
				INNER JOIN projovemcampo.diario dia ON dia.diaid = lnd.diaid AND (diatempoescola is not null AND diatempocomunidade is not null) AND (diatempoescola != 0 AND diatempocomunidade != 0)
				INNER JOIN projovemcampo.historico_diario hid ON hid.hidid = dia.hidid AND stdid = 8
				INNER JOIN projovemcampo.rangeperiodo rap ON rap.rapid = dia.rapid
				LEFT JOIN territorios.municipio m ON m.muncod = est.estmuncodnasc
				INNER JOIN projovemcampo.turma tur ON tur.turid = est.turid
				INNER JOIN projovemcampo.secretaria sec ON sec.secaid = tur.secaid
				INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid
				INNER JOIN projovemcampo.anoprograma apr ON apr.aprid = apc.aprid
				INNER JOIN projovemcampo.agenciabancariaescola abe ON abe.entid = tur.entid AND agbcod is not NULL 
				INNER JOIN entidade.endereco ende ON ende.entid = abe.entid
				LEFT JOIN territorios.municipio m2 ON m2.muncod = ende.muncod
				WHERE
					(((lndhorasescola + lndhorascomunidade)*100)/(diatempoescola + diatempocomunidade))>= 75			
				AND lnd.remid IS NULL 
				AND dia.perid in(".implode(",",$perid).")
				AND d.esdid = '".ESD_PAGAMENTO_AUTORIZADO."'
				AND	est.cadastradosgb='t'
				LIMIT 10000";
		
		$listapagamentos = $db->carregar( $sql );
// 		ver($listapagamentos,d);
		unset($perid);
		if($listapagamentos[0]) {
			foreach($listapagamentos as $pg) {
				$_listapagamento[$pg['apranoreferencia']."-".$pg['mesref']][] = $pg;
			}
		}
		
		if($_listapagamento) {
			foreach($_listapagamento as $per => $pagamentos) {
				
				$sql = "INSERT INTO projovemcampo.remessapagamento(remdata) VALUES (NOW()) RETURNING remid;";
				$remid = $db->pegaUm($sql);
				
				$pers = explode("-",$per);
				$arxml['remessa']['autenticacao']    = array('sistema' => SISTEMA_SGB, 'login' => USUARIO_SGB,'senha' => SENHA_SGB);
				$arxml['remessa']['id']    			 = $remid;
				$arxml['remessa']['programa'] 	     = PROGRAMA_SGB;
				$arxml['remessa']['vigencia']        = array('mes' => $pers[1], 'ano' => $pers[0]);
				$arxml['remessa']['lote']            = 'P';
				
				foreach($pagamentos as $pg) {
					$arxmlpg['bolsista']      = array('cpf' => $pg['estcpf'],'codigoDaFuncao' => $pg['codfucaosgb']);
					$arxmlpg['entidade']      = array('cnpj' => $pg['secacnpj'],'uf' => $pg['secuf'],'codigoIbgeMunicipio' => $pg['co_municipio_entidade']);
					$arxmlpg['valor']         = $pg['valorpagamento'];
					$arxmlpg['parcela'] 	  = ($pg['parcela']);
					$arxmlpg['id'] 			  = $pg['lndid'];
					$arxml['remessa']['pagamentos'][] = $arxmlpg;
					
					$sql = "INSERT INTO projovemcampo.historicopagamento(lndid,remid,usucpfacao,hipdataenvio,hiprejeitado,parcela) values('".$pg['lndid']."','".$remid."','" . $_SESSION['usucpf'] . "', now(),'f','".$pg['parcela']."' )";
					$db->executar($sql);
					
				}
// 				
		   		$enviarRemessaDePagamentos_obj = $soapClient->enviarRemessaDePagamentos( $arxml );
// 		   		
		   		unset($arxml);
		   		
				$logerro_enviarRemessaDePagamentos = (($enviarRemessaDePagamentos_obj->reciboEnvio->mensagem->codigo=='10001')?'FALSE':'TRUE');
				
		    	if($logerro_enviarRemessaDePagamentos=='FALSE') {
		    		
		    		echo "Remessa criada com sucesso : ".$enviarRemessaDePagamentos_obj->reciboEnvio->rastreador." (".count($pagamentos)." registros)<br>";
		    		
		    		foreach($pagamentos as $pg) {
		    			wf_alterarEstado( $pg['docid'], AED_PAGAMENTO_ENVIAR, $cmddsc = '', array());
		    		}
// 		    		ver(d);
					$sql = "UPDATE projovemcampo.remessapagamento SET remano='".$pers[0]."', remmes='".$pers[1]."', remrastreador='".$enviarRemessaDePagamentos_obj->reciboEnvio->rastreador."' WHERE remid='".$remid."'";
					$db->executar($sql);
					$db->commit();
		    	} else {
		    		
		    		echo "Remessa criada (#".$remid.") foi cancelada<br>";
		    		
		    		$db->rollback();
		    	}
				inserirDadosLog(array('logerro'=>$logerro_enviarRemessaDePagamentos,'logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'remid'=>$remid,'logservico'=>'enviarRemessaDePagamentos'));
		    	
			}
		}
		
// 		$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='projovemcampo_efetuar_pagamentos.php'";
// 		$db->executar($sql);
// 		$db->commit();
		
		if($_SESSION['usucpf'] == '') {
			
			unset($_SESSION['usucpf']);
			unset($_SESSION['usucpforigem']);
			
		}
		
		
		$db->close();
// 		echo "
//             <script>
//                 alert('Fim.');
//                 window.location.href = window.location.href;
//             </script>"; 
	}
}
?>
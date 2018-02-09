<?php
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$db = new cls_banco();
/**
 * Classe de acesso ao WS alterações orçamentárias.
 * @see WSAlteracoesOrcamentarias
 */
require(APPRAIZ . 'altorc/classes/WSAlteracoesOrcamentarias.php');


$ws = new WSAlteracoesOrcamentarias();
$retorno = $ws->obterEmendasAprovadas();

if( $_REQUEST['mostraRetorno'] == 'S' ){
	echo '<pre>';
	print_r($retorno);
	echo '</pre>';
	die;
}


if( (int)$retorno->return->sucesso == 1 ){
	$arrRetorno = $retorno->return->emendasAprovadas->emendaAprovada;
	cargaEmendaBase( $arrRetorno, $_REQUEST );
	echo '<pre>Foi</pre>';
} else {
	ver($retorno->return->mensagensErro, d);
}

function cargaEmendaBase( $arrDados = array() ){
	global $db;
		
	foreach ($arrDados as $v) {
		
		$codigo = $db->pegaUm("select cesid from emenda.cargaemendasiop 
								where numeroemenda = '{$v->numeroEmenda}' 
									and emendaano = '".date('Y')."' 
									and naturezadespesa = '".$v->naturezaDespesa."' 
									and fonte = '".$v->fonte."' 
									and codigoparlamentar = '{$v->codigoParlamentar}'");
		
		if( strlen($v->numeroEmenda) == 2 ){
			$codigoemenda = str_pad($v->codigoParlamentar, 6, 0, STR_PAD_RIGHT).(string)$v->numeroEmenda;
		} else {
			$codigoemenda = str_pad($v->codigoParlamentar, 7, 0, STR_PAD_RIGHT).(string)$v->numeroEmenda;
		}
		
		if( !empty($codigo) ){
			$sql = "UPDATE emenda.cargaemendasiop
					   SET identificadorunicolocalizador = '".$v->identificadorUnicoLocalizador."', esfera = '".$v->esfera."', codigouo = '".$v->codigoUO."', 
					       codigoprograma = '".$v->codigoPrograma."', codigofuncao = '".$v->codigoFuncao."', codigosubfuncao = '".$v->codigoSubFuncao."', codigoacao = '".$v->codigoAcao."', 
					       codigolocalizador = '".$v->codigoLocalizador."', naturezadespesa = '".$v->naturezaDespesa."', resultadoprimario = '".$v->resultadoPrimario."', 
					       fonte = '".$v->fonte."', iduso = '".$v->idUso."', planoorcamentario = '".$v->planoOrcamentario."', codigoparlamentar = '".$v->codigoParlamentar."', nomeparlamentar = '".$v->nomeParlamentar."', numeroemenda = '".$v->numeroEmenda."', 
					       codigoemenda = '".$codigoemenda."', codigopartido = '".$v->codigoPartido."', siglapartido = '".$v->siglaPartido."', 
					       ufparlamentar = '".$v->ufParlamentar."', valoratual = ".($v->valorAtual ? $v->valorAtual : 'null')."
					 WHERE cesid = $codigo";
			$db->executar($sql);
			$cesid = $codigo;
		} else {			
			$sql = "INSERT INTO emenda.cargaemendasiop(identificadorunicolocalizador, esfera, codigouo, codigoprograma, codigofuncao, codigosubfuncao, codigoacao, codigolocalizador,
  						naturezadespesa, resultadoprimario, fonte, iduso, planoorcamentario, codigoparlamentar, nomeparlamentar, codigoemenda, numeroemenda, emendaano, codigopartido, siglapartido, ufparlamentar, valoratual) 
					VALUES ('".$v->identificadorUnicoLocalizador."', 
						'".$v->esfera."', 
						'".$v->codigoUO."', 
						'".$v->codigoPrograma."', 
						'".$v->codigoFuncao."', 
						'".$v->codigoSubFuncao."', 
						'".$v->codigoAcao."', 
						'".$v->codigoLocalizador."',
  						'".$v->naturezaDespesa."', 
						'".$v->resultadoPrimario."', 
						'".$v->fonte."', 
						'".$v->idUso."', 
						'".$v->planoOrcamentario."', 
						'".$v->codigoParlamentar."', 
						'".$v->nomeParlamentar."', 
						'".$codigoemenda."', 
						'".$v->numeroEmenda."', 
						'".date('Y')."', 
						'".$v->codigoPartido."', 
						'".$v->siglaPartido."', 
						'".$v->ufParlamentar."', 
						".($v->valorAtual ? $v->valorAtual : 'null').") returning cesid";
			$cesid = $db->pegaUm($sql);
		}
		
		$gnd = substr($v->naturezaDespesa, 0, 1);
		
		//$v->beneficiariosEmenda->beneficiarioEmenda = (array) $v->beneficiariosEmenda->beneficiarioEmenda; 
		
		if( !empty($v->beneficiariosEmenda->beneficiarioEmenda) && $cesid ){
			
			$v->beneficiariosEmenda->beneficiarioEmenda = is_array($v->beneficiariosEmenda->beneficiarioEmenda) ? $v->beneficiariosEmenda->beneficiarioEmenda : array($v->beneficiariosEmenda->beneficiarioEmenda);
			$boEnviado = false;
			
			$db->executar("DELETE FROM emenda.beneficiarioemenda WHERE cesid = $cesid");
			$db->executar("DELETE FROM emenda.objetosbeneficiarioemenda WHERE cesid = $cesid");
			
			foreach ($v->beneficiariosEmenda->beneficiarioEmenda as $benef) {
				
				$sql = "INSERT INTO emenda.beneficiarioemenda(cesid, cnpjbeneficiario, nomebeneficiario, valorapuradorcl, valorrevisadobeneficiario, codigomomento, snatual)
			    		VALUES ($cesid, '".$benef->CNPJBeneficiario."', 
			    						'".$benef->nomeBeneficiario."', 
			    						".($benef->valorApuradoRCL ? $benef->valorApuradoRCL : 'null').", 
			    						".($benef->valorRevisadoBeneficiario ? $benef->valorRevisadoBeneficiario : 'null').",
			    						'".$benef->codigoMomento."', 
			    						'".$benef->snAtual."');";
				$db->executar($sql);					
				$db->commit(); 
				
				$retorno = false;
				if( $_REQUEST['gravaEmenda'] == 'S' ){
					if( !in_array($codigoemenda, array('26080003', '27230020', '11590005') ) ){
						$retorno = insereBeneficiarioEmendas($benef, $edeid, $codigoemenda, $v->nomeParlamentar, $gnd, $v->fonte);
					}
				}
				
				if( !empty($benef->objetosBeneficiarioEmenda->objetoBeneficiarioEmenda) ){
		
					foreach ($benef->objetosBeneficiarioEmenda->objetoBeneficiarioEmenda as $obj) {
						$sql = "INSERT INTO emenda.objetosbeneficiarioemenda(cesid, cnpjbeneficiario, descricaoobjeto, valorobjeto)
								VALUES ($cesid, '".$benef->CNPJBeneficiario."', '".$obj->descricaoObjeto."', ".($obj->valorObjeto ? $obj->valorObjeto : 'null').")";
						$db->executar($sql);
						$db->commit();
					}
				}
				
				if( $retorno == true && $boEnviado == false ){
					$conteudo = '<p><b>Senhor(a) parlamentar,</b></p>
						a indicação da emenda '.$codigoemenda.'/'.date(Y).' foi validada no SIOP.<br>
						O próximo passo é o preenchimento, até 07/08/'.date(Y).' no SIMEC/Emendas da iniciativa, dos dados do responsável pela elaboração do PTA e, quando se tratar de prefeitura e secretaria estadual, da vinculação da subação.<br>
						Qualquer dúvida, tratar com a ASPAR do MEC (2022-7899/7896/7894)';
						
					$remetente = array('nome' => SIGLA_SISTEMA. ' - MÓDULO EMENDAS', 'email' => 'noreply@simec.gov.br');
					
					if($_SESSION['baselogin'] != "simec_desenvolvimento" && $_SESSION['baselogin'] != "simec_espelho_producao" ){
						$email = $db->pegaUm("select a.autemail from emenda.autor a where a.autcod = '{$v->codigoParlamentar}'");
					} else {
						$email = $_SESSION['email_sistema'];
					}
					if( !empty($email) ){
						$retorno = enviar_email($remetente, array($email), SIGLA_SISTEMA. ' - EMENDAS', $conteudo, $cc, null);
						$boEnviado = true;
					}
				}
			}
		}
	}
	$db->commit();
}

function insereBeneficiarioEmendas($dados, $edeid, $emecod, $autor, $gnd, $fonte ){
	global $db;
	
	$enbid = $db->pegaUm("SELECT enbid FROM emenda.entidadebeneficiada WHERE enbcnpj='".$dados->CNPJBeneficiario."' and enbano = '".date('Y')."'");

	if( empty($enbid) ){				
		$sql = "INSERT INTO emenda.entidadebeneficiada(enbstatus, enbano, enbdataalteracao, enbnome, enbcnpj, muncod, estuf)
	    				VALUES ('A',
	    						'".date('Y')."',
	    						NOW(),
	    						'".$dados->nomeBeneficiario."',
	    						'".$dados->CNPJBeneficiario."',
	    						'null',
	    						'null') RETURNING enbid";
		$enbid = $db->pegaUm($sql);
	}
	
	$edevalor = ($dados->valorApuradoRCL ? $dados->valorApuradoRCL : 'null');
	$edevalordisponivel = ($dados->valorRevisadoBeneficiario ? $dados->valorRevisadoBeneficiario : 'null');
	
	$sql = "select 
			    ed.emdid
			from emenda.emenda e
				inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
			where
				e.emecod = '{$emecod}'
				and ed.gndcod = '{$gnd}' 
				and ed.foncod = '{$fonte}'
			    and e.emeano = '".date('Y')."'";
	$emdid = $db->pegaUm($sql);
	
	if( !empty($emdid) ){
		
		$edeid = $db->pegaUm("select edeid from emenda.emendadetalheentidade where emdid = $emdid and edestatus = 'A' and enbid = $enbid and edestatus = 'A'");
				
		if( empty($edeid) ){ 
			$sql = "INSERT INTO emenda.emendadetalheentidade ( emdid, enbid, edevalor, edevalordisponivel, usucpfalteracao, ededataalteracao, edestatus )
					VALUES ( {$emdid}, {$enbid}, {$edevalor}, {$edevalordisponivel}, '{$_SESSION['usucpf']}', 'now()', 'A' ) RETURNING edeid";
			
			$edeid = $db->pegaUm( $sql );
			$retorno = true;
		} else {		
			$sql = "UPDATE emenda.emendadetalheentidade SET enbid = {$enbid}, edevalor = {$edevalor}, edevalordisponivel = {$edevalordisponivel}, edestatus = 'A' WHERE edeid = {$edeid}";
			$db->executar($sql);
			$retorno = false;
		}
		$db->commit();
	}
	return $retorno;
}
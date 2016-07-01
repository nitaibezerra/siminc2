<?php
class WSAlteracoesOrcamentariasEmendas{
	public $usuario, $senha, $urlWsdl, $anoEmenda;
	
	public function __construct( $arrParam = array() ){
		
		$this->usuario		= $arrParam['usuario']; 
		$this->senha		= $arrParam['senha'];
		//$this->urlWsdl	= 'https://webservice.siop.gov.br/services/WSAlteracoesOrcamentarias?wsdl';
		//$this->urlWsdl	= 'https://www.siop.gov.br/services/WSAlteracoesOrcamentarias?wsdl';
		$this->urlWsdl		= 'https://testews.siop.gov.br/services/WSAlteracoesOrcamentarias?wsdl';
		$this->exercicio 	= $arrParam['exercicio'];
	}
	/*36800001 e 36800003*/
	
	public function obterEmendasAprovadas(){
		
		$caminho_certificado	=  APPRAIZ."planacomorc/modulos/sistema/comunica/WS_SISMEC_2.pem";
		$senha_certificado		= "sismec";
		
		/* $caminho_certificado	=  APPRAIZ."spo/certificados/sof_siop.pem";
		$senha_certificado		= "simec"; */
		
		$context = stream_context_create(array(
										'ssl' => array(
												'verify_peer' => false,
												'allow_self_signed' => true,
												'cafile' => $caminho_certificado,
												'local_cert' => $caminho_certificado,
												'passphrase ' 	=> $senha_certificado
												)
										)
									); 
		
		$obEnvio = $this->getObterEmendasAprovadas();
		//$obEnvio = $this->getBeneficiariosEmenda();
		//ver($obEnvio,d);
		$opts = array(
				'ssl'   => array(
						'verify_peer'          => false,
						'ciphers'=>"SHA1"
				),
				'https' => array(
						'curl_verify_ssl_peer'  => false,
						'curl_verify_ssl_host'  => false
				)
		);
		$wsProposta = new SimecWsObterEmendasAprovadas($this->urlWsdl, array(
																			'exceptions'	=> true,
																			'trace'			=> true,
																			'encoding'		=> 'ISO-8859-1',
																			'local_cert'        => file_get_contents($caminho_certificado),
																			'passphrase'        => $senha_certificado,
																			'stream_context' => $opts
																			)
													);
		//ver($wsProposta,d);
		//$wsProposta->__setUsernameToken($this->usuario, md5($this->senha) );
		//ver($wsProposta,d);
		$retorno = $wsProposta->obterEmendasAprovadas( $obEnvio );
		
		if( (int)$retorno->return->sucesso == 1 ){
			$arrRetorno = $retorno->return->emendasAprovadas->emendaAprovada;
			$this->cargaEmendaBase( $arrRetorno );
		} else {
			ver($retorno->return->mensagensErro, d);
		}
		//return (int)$retorno->return->sucesso;
		return (int)$retorno->return->sucesso;
	}
	
	public function getObterEmendasAprovadas(){	
	
		$ObjetoWS = new obterEmendasAprovadas();
		$ObjetoWS->CredencialDTO			= $this->getCredencialDTO(true);
		$ObjetoWS->FiltroEmendaAprovadaDTO	= $this->getFiltroEmendaAprovadaDTO();
	
		return $ObjetoWS;
	}
	
	public function getFiltroEmendaAprovadaDTO(){
		$ObjetoWS = new filtroEmendaAprovadaDTO();
		$ObjetoWS->exercicio 			= date('Y');
		//$ObjetoWS->codigosUO 			= $this->getCodigosUO();
		//$ObjetoWS->codigosParlamentares = $this->getCodigosParlamentares();
		
		return $ObjetoWS;
	}
	
	public function getCredencialDTO($hash = false){
	
		$ObjetoWS = new credencialDTO();
		$ObjetoWS->perfil 		= 32;
		$ObjetoWS->usuario 	= $this->usuario;
		if (!$hash)
			$ObjetoWS->senha 	= $this->senha;
		else
			$ObjetoWS->senha 	= md5($this->senha);
	
		return $ObjetoWS;
	}
	
	public function getCodigosUO(){
		$ObjetoWS = new codigosUO();
		$ObjetoWS->codigoUO = '';
		
		return $ObjetoWS;
	}
	
	public function getCodigosParlamentares(){
		$ObjetoWS = new codigosParlamentares();
		$ObjetoWS->codigoParlamentar = '';
		
		return $ObjetoWS;
	}
	
	public function getBeneficiariosEmenda() {
		$ObjetoWS = new beneficiariosEmenda();
		
		$ObjetoWS->beneficiarioEmenda = $this->getBeneficiarioDTO();
		
		return $ObjetoWS;
	}
	
	public function getBeneficiarioDTO() {
		$ObjetoWS = new beneficiarioDTO();
		
		$ObjetoWS->CNPJBeneficiario 			= ''; // string
		$ObjetoWS->nomeBeneficiario 			= ''; // string
		$ObjetoWS->valorRevisadoBeneficiario	= ''; // long
		$ObjetoWS->objetosBeneficiarioEmenda	= $this->getObjetosBeneficiarioEmenda(); // objetosBeneficiarioEmenda
		
		return $ObjetoWS;
	}
	
	public function getObjetosBeneficiarioEmenda() {
		$ObjetoWS = new objetosBeneficiarioEmenda();
		
		$ObjetoWS->objetoBeneficiarioEmenda = $this->getObjetoBeneficiarioDTO(); // objetoBeneficiarioDTO
		
		return $ObjetoWS;
	}
	
	public function getObjetoBeneficiarioDTO() {
		$ObjetoWS = new objetoBeneficiarioDTO();
		
		$ObjetoWS->descricaoObjeto	= ''; // string
		$ObjetoWS->valorObjeto		= ''; // long
		
		return $ObjetoWS;
	}
	
	public function cargaEmendaBase( $arrDados = array() ){
		global $db;
		
		foreach ($arrDados as $v) {
			
			$codigo = $db->pegaUm("select cesid from emenda.cargaemendasiop where numeroemenda = '{$v->numeroEmenda}' and emendaano = '".date('Y')."' and codigoparlamentar = '{$v->codigoParlamentar}'");
			
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
						       fonte = '".$v->fonte."', iduso = '".$v->idUso."', codigoparlamentar = '".$v->codigoParlamentar."', nomeparlamentar = '".$v->nomeParlamentar."', numeroemenda = '".$v->numeroEmenda."', 
						       codigoemenda = '".$codigoemenda."', codigopartido = '".$v->codigoPartido."', siglapartido = '".$v->siglaPartido."', 
						       ufparlamentar = '".$v->ufParlamentar."', valoratual = '".$v->valorAtual."'
						 WHERE cesid = $codigo";
				$db->executar($sql);
				$cesid = $codigo;
			} else {			
				$sql = "INSERT INTO emenda.cargaemendasiop(identificadorunicolocalizador, esfera, codigouo, codigoprograma, codigofuncao, codigosubfuncao, codigoacao, codigolocalizador,
	  						naturezadespesa, resultadoprimario, fonte, iduso, codigoparlamentar, nomeparlamentar, codigoemenda, numeroemenda, emendaano, codigopartido, siglapartido, ufparlamentar, valoratual) 
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
							'".$v->codigoParlamentar."', 
							'".$v->nomeParlamentar."', 
							'".$codigoemenda."', 
							'".$v->numeroEmenda."', 
							'".date('Y')."', 
							'".$v->codigoPartido."', 
							'".$v->siglaPartido."', 
							'".$v->ufParlamentar."', 
							'".($v->valorAtual ? $v->valorAtual : 'null')."') returning cesid";
				$cesid = $db->pegaUm($sql);
			}
			
			$gnd = substr($v->naturezaDespesa, 0, 1);
			
			if( !empty($v->beneficiariosEmenda->beneficiarioEmenda) && is_array($v->beneficiariosEmenda->beneficiarioEmenda) && $cesid ){
				
				$boEnviado = false;
				foreach ($v->beneficiariosEmenda->beneficiarioEmenda as $benef) {
					
					$db->executar("DELETE FROM emenda.beneficiarioemenda WHERE cesid = $cesid");
					$db->executar("DELETE FROM emenda.objetosbeneficiarioemenda WHERE cesid = $cesid");
					
					$sql = "INSERT INTO emenda.beneficiarioemenda(cesid, cnpjbeneficiario, nomebeneficiario, valorrevisadobeneficiario)
				    		VALUES ($cesid, '".$benef->CNPJBeneficiario."', '".$benef->nomeBeneficiario."', ".($benef->valorRevisadoBeneficiario ? $benef->valorRevisadoBeneficiario : 'null').");";
					$db->executar($sql);
					echo '<pre>';					
					echo($sql);
					echo '</pre>';					
					
					$retorno = $this->insereBeneficiarioEmendas($benef, $edeid, $codigoemenda, $v->nomeParlamentar, $gnd, $v->fonte);
					
					if( is_array($benef->objetosBeneficiarioEmenda->objetoBeneficiarioEmenda) ){
			
						foreach ($benef->objetosBeneficiarioEmenda->objetoBeneficiarioEmenda as $obj) {
							$sql = "INSERT INTO emenda.objetosbeneficiarioemenda(cesid, cnpjbeneficiario, descricaoobjeto, valorobjeto)
									VALUES ($cesid, '".$benef->CNPJBeneficiario."', '".$obj->descricaoObjeto."', ".($obj->valorObjeto ? $obj->valorObjeto : 'null').")";
							$db->executar($sql);
						}
					}
					
					if( $retorno == true && $boEnviado == false ){
						$conteudo = '<p><b>Senhor(a) parlamentar,</b></p>
							a indicação da emenda '.$codigoemenda.'/'.date(Y).' foi validada no SIOP.<br>
							O próximo passo é o preenchimento, até 03/07/'.date(Y).' no SIMEC/Emendas da iniciativa, dos dados do responsável pela elaboração do PTA e, quando se tratar de prefeitura e secretaria estadual, da vinculação da subação.<br>
							Qualquer dúvida, tratar com a ASPAR do MEC (2022-7899/7896/7894)';
							
						$remetente = array('nome' => 'SIMEC - MÓDULO EMENDAS', 'email' => 'noreply@simec.gov.br');
						
						if($_SESSION['baselogin'] != "simec_desenvolvimento" && $_SESSION['baselogin'] != "simec_espelho_producao" ){
							$email = $db->pegaUm("select a.autemail from emenda.autor a where a.autcod = '{$v->codigoParlamentar}'");
						} else {
							$email = $_SESSION['email_sistema'];
						}
						if( !empty($email) ){
							$retorno = enviar_email($remetente, array($email), 'SIMEC - EMENDAS', $conteudo, $cc, null);
							$boEnviado = true;
						}
					}
				}
			}
		}
		$db->commit();
	}
	
	public function insereBeneficiarioEmendas($dados, $edeid, $emecod, $autor, $gnd, $fonte ){
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
		
		$edevalor = ($benef->valorRevisadoBeneficiario ? $benef->valorRevisadoBeneficiario : 'null');
		
		$sql = "select 
				    ed.emdid
				from emenda.emenda e
					inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				where
					e.emecod = '{$emecod}'
					and ed.gndcod = '{$gnd}' 
					and ed.foncod = '{$fonte}'
				    and e.emeano = '".date('Y')."'";
		echo '<pre>';
		echo($sql);
		echo '</pre>';
		$emdid = $db->pegaUm($sql);
		
		if( !empty($emdid) ){
			$edeid = $db->pegaUm("select edeid from emenda.emendadetalheentidade where emdid = $emdid and edestatus = 'A' and enbid = $enbid and edestatus = 'A'");
					
			if( empty($edeid) ){ 
				$sql = "INSERT INTO emenda.emendadetalheentidade ( emdid, enbid, edevalor, usucpfalteracao, ededataalteracao, edestatus )
						VALUES ( {$emdid}, {$enbid}, {$edevalor}, '{$_SESSION['usucpf']}', 'now()', 'A' ) RETURNING edeid";
				
				$edeid = $db->pegaUm( $sql );
				echo '<pre>';
				echo($sql);
				echo '</pre>';
				$retorno = true;
			} else {		
				$sql = "UPDATE emenda.emendadetalheentidade SET enbid = {$enbid}, edevalor = {$edevalor}, edestatus = 'A' WHERE edeid = {$edeid}";
				$db->executar($sql);
				echo '<pre>';
				echo($sql);
				echo '</pre>';
				$retorno = false;
			}
			$db->commit();
		}
		return $retorno;
	}
}

class obterEmendasAprovadas {
	public $CredencialDTO; // credencialDTO
	public $FiltroEmendaAprovadaDTO; // filtroEmendaAprovadaDTO
}

class credencialDTO {
	public $perfil; // int
	public $senha; // string
	public $usuario; // string
}

class filtroEmendaAprovadaDTO {
	public $exercicio; // int
	public $codigosUO; // codigosUO
	public $codigosParlamentares; // codigosParlamentares
}

class codigosUO {
	public $codigoUO; // string
}

class codigosParlamentares {
	public $codigoParlamentar; // int
}

class obterEmendasAprovadasResponse {
	public $return; // retornoEmendaAprovadaDTO
}

class retornoEmendaAprovadaDTO {
	public $emendasAprovadas; // emendasAprovadas
}

class emendasAprovadas {
	public $emendaAprovada; // emendaAprovadaDTO
}

class emendaAprovadaDTO {
	public $identificadorUnicoLocalizador; // int
	public $esfera; // string
	public $codigoUO; // string
	public $codigoPrograma; // string
	public $codigoFuncao; // string
	public $codigoSubFuncao; // string
	public $codigoAcao; // string
	public $codigoLocalizador; // string
	public $naturezaDespesa; // string
	public $resultadoPrimario; // string
	public $fonte; // string
	public $idUso; // string
	public $codigoParlamentar; // int
	public $nomeParlamentar; // string
	public $numeroEmenda; // int
	public $codigoPartido; // string
	public $siglaPartido; // string
	public $ufParlamentar; // string
	public $valorAtual; // long
	public $beneficiariosEmenda; // beneficiariosEmenda
}

class beneficiariosEmenda {
	public $beneficiarioEmenda; // beneficiarioDTO
}

class beneficiarioDTO {
	public $CNPJBeneficiario; // string
	public $nomeBeneficiario; // string
	public $valorRevisadoBeneficiario; // long
	public $objetosBeneficiarioEmenda; // objetosBeneficiarioEmenda
}

class objetosBeneficiarioEmenda {
	public $objetoBeneficiarioEmenda; // objetoBeneficiarioDTO
}

class objetoBeneficiarioDTO {
	public $descricaoObjeto; // string
	public $valorObjeto; // long
}


/**
 * SimecWsObterEmendasAprovadas class
 * 
 *  
 * 
 * @author    {author}
 * @copyright {copyright}
 * @package   {package}
 */
class SimecWsObterEmendasAprovadas extends SoapClient {
	protected $soapClient;
	
	private $username;
	private $password;

  private static $classmap = array(
                                    
                                    'credencialDTO' => 'credencialDTO',
                                    'filtroAnaliseEmendaDTO' => 'filtroAnaliseEmendaDTO',
                                    'codigosUO' => 'codigosUO',                                    
                                    'obterEmendasAprovadas' => 'obterEmendasAprovadas',
                                    'filtroEmendaAprovadaDTO' => 'filtroEmendaAprovadaDTO',
                                    'codigosParlamentares' => 'codigosParlamentares',
                                    'obterEmendasAprovadasResponse' => 'obterEmendasAprovadasResponse',
                                    'retornoEmendaAprovadaDTO' => 'retornoEmendaAprovadaDTO',
							  		'emendasAprovadas' => 'emendasAprovadas',
							  		'emendaAprovadaDTO' => 'emendaAprovadaDTO',
							  		'beneficiariosEmenda' => 'beneficiariosEmenda',
							  		'beneficiarioDTO' => 'beneficiarioDTO',
							  		'objetosBeneficiarioEmenda' => 'objetosBeneficiarioEmenda',
							  		'objetoBeneficiarioDTO' => 'objetoBeneficiarioDTO',
                                   );

  public function SimecWsObterEmendasAprovadas($wsdl = "https://testews.siop.gov.br/services/WSAlteracoesOrcamentarias?wsdl", $options = array()) {
    foreach(self::$classmap as $key => $value) {
      if(!isset($options['classmap'][$key])) {
        $options['classmap'][$key] = $value;
      }
    }
    $this->soapClient = $client;
    
    parent::__construct($wsdl, $options);
  }

  /**
   *  
   *
   * @param obterEmendasAprovadas $obterEmendasAprovadas
   * @return obterEmendasAprovadasResponse
   */
	public function obterEmendasAprovadas(obterEmendasAprovadas $obterEmendasAprovadas) {
    	return $this->__soapCall('obterEmendasAprovadas', array($obterEmendasAprovadas),       array(
        					    'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            					'soapaction' => ''
           								)
      							);
  	}
  	
	/*Generates de WSSecurity header*/
	private function wssecurity_header() {
	
		/* The timestamp. The computer must be on time or the server you are
		 * connecting may reject the password digest for security.
		*/
		$timestamp = gmdate('Y-m-d\TH:i:s\Z');
		/* A random word. The use of rand() may repeat the word if the server is
		 * very loaded.
		*/
		$nonce = mt_rand();
		/* This is the right way to create the password digest. Using the
		 * password directly may work also, but it's not secure to transmit it
		* without encryption. And anyway, at least with axis+wss4j, the nonce
		* and timestamp are mandatory anyway.
		*/
		$passdigest = base64_encode(
				pack('H*',
						sha1(
								pack('H*', $nonce) . pack('a*',$timestamp).
								pack('a*',$this->password))));
	
		$auth = '
<wsse:Security xmlns:wsse="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd" xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd">
	<wsse:UsernameToken wsu:Id="UsernameToken-4">
		<wsse:Username>'.$this->username.'</wsse:Username>
		<wsse:Password Type="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest">'.$passdigest.'</wsse:Password>
		<wsse:Nonce EncodingType="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary">'.base64_encode(pack('H*', $nonce)).'</wsse:Nonce>
		<wsu:Created>'.$timestamp.'</wsu:Created>
	</wsse:UsernameToken>
</wsse:Security>';
	
		/* XSD_ANYXML (or 147) is the code to add xml directly into a SoapVar.
		 * Using other codes such as SOAP_ENC, it's really difficult to set the
		* correct namespace for the variables, so the axis server rejects the
		* xml.
		*/
		$authvalues = new SoapVar($auth,XSD_ANYXML);
		$header = new SoapHeader("http://docs.oasis-open.org/wss/2004/01/oasis-".
				"200401-wss-wssecurity-secext-1.0.xsd", "Security", $authvalues,
				true);
	
		return $header;
	}
	
	/* It's necessary to call it if you want to set a different user and
	 * password
	*/
	public function __setUsernameToken($username, $password) {
		$this->username = $username;
		$this->password = $password;
	}
	
	function __doRequest($request, $location, $action, $version, $one_way = 0){
		$this->log($request, $location, $action, $version, 'request');
	
		$response = SoapClient::__doRequest($request, $location, $action, $version, $one_way);
	
		$this->log($response, $location, $action, $version, 'response');
		 
		return $response;
	}
	
	public function log($request, $location, $action, $version, $tipo){
		//echo '<pre>';
		//print_r( simec_htmlentities($request) );
	}
	
			// route all other method calls directly to soapClient
	public function __call($method, $args){
		// you could also add method_exists check here
		return call_user_func_array(array($this->soapClient, $method), $args);
	}
}

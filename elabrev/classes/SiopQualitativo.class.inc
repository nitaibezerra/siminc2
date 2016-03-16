<?php
class SiopQualitativo{
	
	private $urlWsdl;
	private $wsusuario;
	private $wssenha;
	private $certificado;
	private $senha_certificado;
	private $db;
	private $post;
	private $acaid;
	private $acacod;
	private $unicod;
	private $valorfisico;
	private $codigomomento;
	
	public function __construct( $arrParam = array() ){
		global $db;
		
		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWsdl	 				= "https://homologacao.siop.planejamento.gov.br/services/WSQualitativo?wsdl";
			/*$this->certificado 			=  APPRAIZ. "elabrev/modulos/sistema/comunica/mec.pem";
			$this->senha_certificado 	= "mec2011";*/
			$this->certificado 			=  APPRAIZ. "elabrev/modulos/sistema/comunica/simec.pem";
			$this->senha_certificado 	= "siMEC2013";
		} else {
			$urlWsdl					= "https://www.siop.planejamento.gov.br/services/WSQualitativo?wsdl";
			/*$this->certificado 			=  "/etc/ssl/cert_simec_2012.pem";
			$this->senha_certificado 	= "12345678";*/
			$this->certificado 			=  APPRAIZ. "elabrev/modulos/sistema/comunica/simec.pem";
			$this->senha_certificado 	= "siMEC2013";
		}

		if($db){
			$this->db = $db;
		} else {
			$this->db = new cls_banco();
		}
		
		$this->wsusuario 	= $arrParam['usuario'];
		$this->wssenha 		= $arrParam['senha'];
		$this->urlWsdl 		= $urlWsdl;
		$this->post			= $arrParam['post'];
		$this->codigomomento= $arrParam['post']['codigomomento'];
		$this->acaid		= '';
		$this->unicod		= '';
		$this->acacod		= '';
		$this->valorfisico	= '';
	}
	
	public function obterProgramacaoCompleta(){
		
		$obEnvio = $this->getObterProgramacaoCompleta();
		//ver($obEnvio,d);
		$wsQualitativo = new WSQualitativo($this->urlWsdl, array(
															'local_cert'	=> $this->certificado, 
															'passphrase ' 	=> $this->senha_certificado,
															'exceptions'	=> true,
													        'trace'			=> true,
															'encoding'		=> 'ISO-8859-1' )
											);
		$retorno = $wsQualitativo->obterProgramacaoCompleta( $obEnvio );
		return $retorno;
	}
	
	public function obterTabelasApoio(){
		
		$obEnvio = $this->getObterTabelasApoio();
		//ver($obEnvio,d);
		$wsQualitativo = new WSQualitativo($this->urlWsdl, array(
															'local_cert'	=> $this->certificado, 
															'passphrase ' 	=> $this->senha_certificado,
															'exceptions'	=> true,
													        'trace'			=> true,
															'encoding'		=> 'ISO-8859-1' )
											);
		$retorno = $wsQualitativo->obterTabelasApoio( $obEnvio );
		return $retorno;
	}
	
	private function getCredencial(){
		
		$ObjetoWS = new credencialDTO();
		$ObjetoWS->perfil 	= 32;
		$ObjetoWS->senha  	= md5($this->wssenha);
		$ObjetoWS->usuario	= $this->wsusuario;
		
		return $ObjetoWS;
	}
	
	private function getObterProgramacaoCompleta(){
		
		$ObjetoWS = new obterProgramacaoCompleta();
		$ObjetoWS->credencial 								= $this->getCredencial(); // credencialDTO
		$ObjetoWS->exercicio								= $_SESSION['exercicio']; // int
		$ObjetoWS->codigoMomento							= $this->codigomomento; // int
		$ObjetoWS->retornarOrgaos 							= (boolean)$this->post['retornarOrgaos'];
		$ObjetoWS->retornarProgramas 						= (boolean)$this->post['retornarProgramas'];
		$ObjetoWS->retornarIndicadores 						= (boolean)$this->post['retornarIndicadores'];
		$ObjetoWS->retornarObjetivos 						= (boolean)$this->post['retornarObjetivos'];
		$ObjetoWS->retornarIniciativas 						= (boolean)$this->post['retornarIniciativas'];
		$ObjetoWS->retornarAcoes 							= (boolean)$this->post['retornarAcoes'];
		$ObjetoWS->retornarLocalizadores 					= (boolean)$this->post['retornarLocalizadores'];
		$ObjetoWS->retornarMetas 							= (boolean)$this->post['retornarMetas'];
		$ObjetoWS->retornarRegionalizacoes 					= (boolean)$this->post['retornarRegionalizacoes'];
		$ObjetoWS->retornarPlanosOrcamentarios 				= (boolean)$this->post['retornarPlanosOrcamentarios'];
		$ObjetoWS->retornarAgendaSam 						= (boolean)$this->post['retornarAgendaSam'];
		$ObjetoWS->retornarMedidasInstitucionaisNormativas 	= (boolean)$this->post['retornarMedidasInstitucionaisNormativas'];
		$ObjetoWS->dataHoraReferencia 						= ''; // dateTime
		
		return $ObjetoWS;
	}
	
	private function getObterTabelasApoio(){
		
		$ObjetoWS = new obterTabelasApoio();
		$ObjetoWS->credencial 						= $this->getCredencial(); // credencialDTO
		$ObjetoWS->exercicio						= $_SESSION['exercicio']; // int
		$ObjetoWS->retornarMomentos 				= false; // boolean
		$ObjetoWS->retornarEsferas 					= false; // boolean
		$ObjetoWS->retornarTiposInclusao 			= false; // boolean
		$ObjetoWS->retonarFuncoes 					= false; // boolean
		$ObjetoWS->retornarSubFuncoes 				= false; // boolean
		$ObjetoWS->retornarTiposAcao 				= false; // boolean
		$ObjetoWS->retornarProdutos 				= (boolean)$this->post['retornarProdutos']; // boolean
		$ObjetoWS->retornarUnidadesMedida 			= (boolean)$this->post['retornarUnidadesMedida']; // boolean
		$ObjetoWS->retornarRegioes 					= false; // boolean
		$ObjetoWS->retornarPerfis 					= false; // boolean
		$ObjetoWS->retornarTiposPrograma 			= false; // boolean
		$ObjetoWS->retornarMacroDesafios 			= false; // boolean
		$ObjetoWS->retornarUnidadesMedidaIndicador	= false; // boolean
		$ObjetoWS->retornarPeriodicidades 			= false; // boolean
		$ObjetoWS->retornarBasesGeograficas 		= false; // boolean
		$ObjetoWS->dataHoraReferencia 				= false; // dateTime
		
		return $ObjetoWS;
	}
	
}
?>
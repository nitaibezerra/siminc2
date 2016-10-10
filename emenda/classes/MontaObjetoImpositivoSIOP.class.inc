<?php
class MontaObjetoImpositivoSIOP{
	
	private $urlWsdl;
	private $wsusuario; 
	private $wssenha;
	private $certificado;
	private $senha_certificado;
	private $post;
	private $emdid;
	private $emecod;
	private $perfil;
	private $momento;
	
	public function __construct( $arrParam = array() ){

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			//$urlWsdl	 				= "https://homologacao.siop.planejamento.gov.br/services/WSAlteracoesOrcamentarias?wsdl";
			$urlWsdl	 				= "https://testews.siop.gov.br/services/WSAlteracoesOrcamentarias?wsdl";
			$this->certificado 			=  APPRAIZ . "planacomorc/modulos/sistema/comunica/simec.pem";
			$this->senha_certificado 	= "siMEC2013";
		} else {
			$urlWsdl					= "https://webservice.siop.gov.br/services/WSAlteracoesOrcamentarias?wsdl";
			$this->certificado 			=  APPRAIZ . "planacomorc/modulos/sistema/comunica/simec.pem";
//			$this->certificado 			=  "/etc/ssl/cert_simec_2012.pem";
//			$this->senha_certificado 	= "12345678";
			$this->senha_certificado 	= "siMEC2013";
		}
		
		$this->wsusuario 	= $arrParam['usuario'];
		$this->wssenha 		= $arrParam['senha'];
		$this->urlWsdl 		= $urlWsdl;
		$this->post			= $arrParam['post'];
		$this->perfil		= $arrParam['post']['wsperfil'];
		$this->momento		= $arrParam['post']['wsmomento'];
	}
	
	public function obterAnalisesEmendas(){
		
		$arrRetorno = array();
		if( is_array($this->post['chk']) ){
			
			foreach ( $this->post['chk'] as $count => $emdid ) {
				$this->emdid = $emdid;
				$obEnvio = $this->getObterAnalisesEmendas();
				//ver($obEnvio, d); 
				$arrRetorno = array();
				$wsImpositivo = new WSEmendaImpositivoSIOP($this->urlWsdl, array(
																				'local_cert'	=> $this->certificado, 
																				'passphrase ' 	=> $this->senha_certificado,
																				'exceptions'	=> true,
																		        'trace'			=> true,
																				'encoding'		=> 'ISO-8859-1' )
																);
				$wsImpositivo->emdid = $this->emdid;
				$wsImpositivo->emecod = $this->emecod;
				
				$retorno = $wsImpositivo->obterAnalisesEmendas( $obEnvio );
				
				$this->trataRetorno($retorno);
				array_push($arrRetorno, $retorno);
			}
		}
				
		return $retorno;
	}
	
	public function cadastrarAnalisesEmendas(){
		
		$arrRetorno = array();
		if( is_array($this->post['chk']) ){
			
			foreach ( $this->post['chk'] as $emdid ) {
				$this->emdid = $emdid;
				$obEnvio = $this->getCadastrarAnalisesEmendas();
				//ver($obEnvio,d);
				
				$wsImpositivo = new WSEmendaImpositivoSIOP($this->urlWsdl, array(
																				'local_cert'	=> $this->certificado, 
																				'passphrase ' 	=> $this->senha_certificado,
																				'exceptions'	=> true,
																		        'trace'			=> true,
																				'encoding'		=> 'ISO-8859-1' )
																);
				$wsImpositivo->emdid = $this->emdid;
				$wsImpositivo->emecod = $this->emecod;
				
				$retorno = $wsImpositivo->cadastrarAnalisesEmendas( $obEnvio );
				
				$this->trataRetorno($retorno);
				
				array_push($arrRetorno, $retorno);
			}
		}
				
		return $retorno;
	}
	
	private function trataRetorno( $retorno = array() ){
		global $db;
		
		$analiseEmendaDTO = $retorno->return->analiseEmendaDTO;
		$pendencias = $retorno->return->pendencias->pendencia;
		$sucesso = $retorno->return->sucesso;
		$mensagensErro = $retorno->return->mensagensErro;
		
		$msgRetorno = '';
		if( $sucesso ){		
			$msgRetorno = (empty($pendencias) ? 'Operação Relaizada com Sucesso' : $pendencias);
			if( $this->post['wssolicitacao'] == 'cadastrarAnalisesEmendas' ){
				$sql = "UPDATE emenda.emendadetalhe SET emddtultimoenviosiop = now() WHERE emdid = ".$this->emdid;
				$db->executar($sql);
			}
		} else {
			if( $pendencias ){
				$msgRetorno = $pendencias;
			} else {
				$msgRetorno = $mensagensErro;
			}
		}
		
		$sql = "UPDATE emenda.logerroswsdl SET logretorno = '".$msgRetorno."' WHERE logid = ".$_SESSION['siop']['logid'];
		$db->executar($sql);
		
		$db->commit();
		unset($_SESSION['siop']['logid']);
	}
	
	private function getCredencial(){
		
		$ObjetoWS = new credencialDTO();
		$ObjetoWS->perfil 	= $this->perfil;
		$ObjetoWS->senha  	= md5($this->wssenha);
		$ObjetoWS->usuario	= $this->wsusuario;
		
		return $ObjetoWS;
	}
	
	private function getObterAnalisesEmendas(){
				
		$ObjetoWS = new obterAnalisesEmendas();
		$ObjetoWS->CredencialDTO			= $this->getCredencial();
		$ObjetoWS->FiltroAnaliseEmendaDTO	= $this->getFiltroAnaliseEmendaDTO();
  		
  		return $ObjetoWS;
	}
	
	private function getCadastrarAnalisesEmendas(){
				
		$ObjetoWS = new cadastrarAnalisesEmendas();
		$ObjetoWS->CredencialDTO	= $this->getCredencial();
		$ObjetoWS->Integer			= $_SESSION['exercicio'];
		$ObjetoWS->AnaliseEmendaDTO = $this->getAnaliseEmendaDTO();
  		
  		return $ObjetoWS;
	}
	
	private function getFiltroAnaliseEmendaDTO(){
		global $db;
		
		$arrEmendas = $this->carregarDetalheEmenda();
		
		$ObjetoWS = new filtroAnaliseEmendaDTO();
		$ObjetoWS->exercicio			= $_SESSION['exercicio'];
		$ObjetoWS->codigoOrgao			= CODIGO_ORGAO_SISTEMA;
		$ObjetoWS->codigosUO			=  $this->getCodigosUO( $arrEmendas['unicod'] ); /*$db->pegaUm("select ungcod from public.unidadegestora where unicod = '{$arrEmendas['unicod']}'");*/
		$ObjetoWS->codigosParlamentar	= $this->getCodigosParlamentar($arrEmendas['emeid']);
		$ObjetoWS->codigoMomento		= $this->momento;					#1000 – Unidade Orçamentária; 2000 – Órgão Central; 2500 – SRI; 3000 – SOF; 5000 – Formalização
		$ObjetoWS->indicadorImpedimento	= $arrEmendas['impedimento']; 		#'N' – Não há impedimento; 'P' – Impedimento Parcial; e 'T' – Impedimento Total
		$ObjetoWS->snAtual				= true;
		//$ObjetoWS->snValidado			= false;
		
  		return $ObjetoWS;
	}
	
	private function getAnaliseEmendaDTO(){
		global $db;
		
		$arrEmendas = $this->carregarDetalheEmenda();		
		$arrOrca = $this->carregarDadosOrcamentario();
		
		$emecod = substr($arrEmendas['emecod'], -2);
				
		$ObjetoWS = new analiseEmendaDTO();
		$ObjetoWS->identificadorUnicoLocalizador 	= $arrOrca['acaidentificadorunicosiop'];
		$ObjetoWS->esfera 					= $arrEmendas['esfcod']; 		#'10' – Fiscal, '20' – Seguridade Social e '30' – Investimento
		$ObjetoWS->codigoUO 				= $arrEmendas['unicod'];
		$ObjetoWS->codigoPrograma 			= $arrEmendas['prgcod'];
		$ObjetoWS->codigoFuncao 			= $arrEmendas['funcod'];
		$ObjetoWS->codigoSubFuncao 			= $arrEmendas['sfucod'];
		$ObjetoWS->codigoAcao 				= $arrEmendas['acacod'];
		$ObjetoWS->codigoLocalizador 		= $arrEmendas['loccod'];
		$ObjetoWS->naturezaDespesa 			= $arrOrca['natcod'];
		$ObjetoWS->resultadoPrimario 		= $arrOrca['rpcod'];
		$ObjetoWS->fonte 					= $arrEmendas['foncod'];
		$ObjetoWS->idUso 					= $arrOrca['idusocod'];
		$ObjetoWS->codigoParlamentar 		= $arrEmendas['autcod'];
		$ObjetoWS->numeroEmenda 			= (string)$emecod;
		$ObjetoWS->siglaPartido 			= $arrEmendas['parsigla'];
		$ObjetoWS->ufParlamentar 			= $arrEmendas['estuf'];
		$ObjetoWS->valorAtual 				= $arrEmendas['emdvalor'];
		$ObjetoWS->codigoMomento 			= $this->momento; 				#1000 – Unidade Orçamentária; 2000 – Órgão Central; 2500 – SRI; 3000 – SOF; 5000 – Formalização
		$ObjetoWS->indicadorImpedimento 	= $arrEmendas['impedimento']; 	#'N' – Não há impedimento; 'P' – Impedimento Parcial; e 'T' – Impedimento Total
		$ObjetoWS->snValidado 				= true;
		$ObjetoWS->snAtual 					= true;
		$ObjetoWS->valorImpedimento 		= $arrEmendas['edivalor'];
		$ObjetoWS->codigosImpedimento		= $this->getCodigosImpedimento( $arrEmendas['ediid'] ); // codigosImpedimento
		$ObjetoWS->justificativa 			= $arrEmendas['edidescricao'];
		
		return $ObjetoWS;
	}
	
	private function getCodigosUO( $unicod ){
		global $db;
		
		/*$arrGestora = $db->carregarColuna("select ungcod from public.unidadegestora where unicod = '$unicod'");
		$arrGestora = $arrGestora ? $arrGestora : array();*/
		
		/*foreach ($arrGestora as $key => $ungcod) {
			$$key = new codigosUO();
			$$key->codigoUO = $ungcod;
			
			$ObjetoWS[$key] = $$key;
		}*/	
		
		$ObjetoWS = new codigosUO();
		$ObjetoWS->codigoUO = $unicod; //$arrGestora[0];
		
		return $ObjetoWS;
	}
	
	private function getCodigosImpedimento( $ediid ){
		global $db;
		
		$arrImped = $db->carregarColuna("select ei.emiid from emenda.emendaimpositivo_tiposjust eit
												inner join emenda.emendaimpositivo ei on ei.emiid = eit.emiid
											where
												eit.ediid = $ediid");
		$arrImped = $arrImped ? $arrImped : array();
		if( $arrImped ){
			/*foreach ($arrImped as $key => $emiid) {
				$$key = new codigosImpedimento();
				$$key->codigoImpedimento = $emiid;
				
				$ObjetoWS[$key] = $$key;
			}*/
			$ObjetoWS = new codigosImpedimento();
			$ObjetoWS->codigoImpedimento = $arrImped[0];
			return $ObjetoWS;
		} else {
			$ObjetoWS = new codigosImpedimento();
			$ObjetoWS->codigoImpedimento = '';
			return array($ObjetoWS);
		}
	}
	
	private function getCodigosParlamentar( $emeid ){
		global $db;
		
		$arrAutor = $db->carregarColuna("select autcod from emenda.autor a 
											inner join emenda.emenda e on e.autid = a.autid 
										where e.emeid = '$emeid' and autstatus = 'A'");
		$arrAutor = $arrAutor ? $arrAutor : array();
		
		/*foreach ($arrAutor as $key => $autor) {
			$$key = new codigosParlamentar();
			$$key->codigoParlamentar = $autor;
			
			$ObjetoWS[$key] = $$key;
		}*/
		
		$ObjetoWS = new codigosParlamentar();
		$ObjetoWS->codigoParlamentar = $arrAutor[0];
		
		return $ObjetoWS;
	}
	
	private function objectToArray ($object) {
		if ( count($object) > 1 ) {
			$arr = array();
			for ( $i = 0; $i < count($object); $i++ ) {
				$arr[] = get_object_vars($object[$i]);
			}
	 
	    	return $arr;
	 
		} else {
			return get_object_vars($object);
		}
	}
	
	private function arr2xml($array){
		if(is_array($array) || is_object($array) ){
			$array = (array)$array;
			function create_tag($XML,$array){
				foreach($array as $tag_name => $value){
					$tag_name=is_numeric($tag_name)?"_".$tag_name:$tag_name;
					$tag_name = (strpos( $tag_name, '_' ) == '' ? $tag_name : substr( $tag_name, 0, strlen( $tag_name ) -2 ));
					if(is_array($value) || is_object($value) ){
						$value = (array)$value;
						$XML->startElement($tag_name);
						create_tag($XML,$value);
					}else{
						$XML->writeElement($tag_name,$value);
					}
				}
				$XML->endElement();
			}
			$XML= new XMLWriter();
			$XML->openMemory();
			$XML->startDocument('1.0','UTF-8');
			create_tag($XML,$array);
			$XML->endDocument();
			return utf8_encode($XML->outputMemory());
		}else{
			return false;
		}
	}
		
	private function carregarDetalheEmenda(){
		global $db;
		
		if( $_SESSION['exercicio'] > 2014 ){
			$filtroImp = ' and edi.edeid = ede.edeid';
			$joinImp = " inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A' ";
		} 
		
		$sql = "select 
				    vf.unicod,
				    e.emeid,
				    case when edi.ediimpositivo = 'NH' then 'N'
				        when edi.ediimpositivo = 'PA' then 'P'
				        when edi.ediimpositivo = 'TO' then 'T'
				       else '' end as impedimento,
				    vf.esfcod,
				    vf.prgcod,
				    vf.funcod,
				    vf.sfucod,
				    vf.acacod,
				    vf.loccod,
				    ed.foncod,
				    a.autcod,
				    e.emecod,
				    p.parsigla,
				    a.estuf,
				    ed.emdvalor,
				    edi.edivalor,
				    edi.ediid,
				    edi.edidescricao
				from
				    emenda.emenda e
				    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				    $joinImp
				    inner join emenda.autor a on a.autid = e.autid
				    inner join emenda.partido p on p.parid = a.parid
				    inner join emenda.v_funcionalprogramatica vf on vf.acaid = e.acaid
				    inner join emenda.emendadetalheimpositivo edi on edi.emdid = ed.emdid $filtroImp
				where 
				    e.emeano = '{$_SESSION['exercicio']}'
				    and ed.emdid = {$this->emdid}";
		$arrEmendas = $db->pegaLinha($sql);
		
		$this->emecod = $arrEmendas['emecod'];
		return $arrEmendas;
	}
	
	private function carregarDadosOrcamentario(){
		global $db;
		
		$sql = "select distinct
				  	spa.acaidentificadorunicosiop,
				    spo.natcod,
				    spo.rpcod,
				    spo.idusocod 
				from altorc.snapshotacao spa 
				inner join altorc.snapshotplanoorcamentario spo on spo.snaid = spa.snaid
				inner join emenda.v_funcionalprogramatica vf on 
				spa.unicod = vf.unicod and spa.snaexercicio = '2014'
				    and spa.esfcod = cast(vf.esfcod as varchar)
				    and spa.funcod = vf.funcod
				    and spa.sfucod = vf.sfucod
				    and spa.acacod = vf.acacod 
				    and spa.loccod = vf.loccod 
				    and spa.prgcod = vf.prgcod
				inner join emenda.emenda e on e.acaid = vf.acaid and e.emeano = '2014'
				inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				inner join public.naturezadespesa nd on nd.ndpcod = spo.natcod and nd.gndcod = ed.gndcod
				where
					spo.rpcod = '6'
				    and ed.emdid = {$this->emdid}";
		$arDados = $db->pegaLinha($sql);
		return $arDados;
	}
}
?>
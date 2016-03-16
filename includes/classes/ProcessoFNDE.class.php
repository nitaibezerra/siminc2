<?php
include_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

class ProcessoFNDE{
	private $wsusuario, $wssenha;
	
	public function __construct( $wsusuario = '', $wssenha = '' ){		
		$this->wsusuario 		= $wsusuario; 
		$this->wssenha 			= $wssenha;
	}
	
	public function gerarProcessoFNDE( $arrParam = array() ){
		$nu_cpf 		= $arrParam['nu_cpf'];
		$data_created	= date("c");
		$ds_resumo 		= $arrParam['ds_resumo'];
		$co_assunto 	= $arrParam['co_assunto'];
		$nu_cnpj 		= $arrParam['nu_cnpj'];
		
		try{	
			$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>string</app>
        <version>string</version>
        <created>$data_created</created>
    </header>
    <body>
        <auth>
            <usuario>{$this->wsusuario}</usuario>
            <senha>{$this->wssenha}</senha>
        </auth>
        <params>
            <ds_resumo>$ds_resumo</ds_resumo>
			<co_assunto>$co_assunto</co_assunto>
			<nu_cpf>$nu_cpf</nu_cpf>
            <nu_cnpj>$nu_cnpj</nu_cnpj>
        </params>
    </body>
</request>
XML;

			if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'https://hmg.fnde.gov.br/webservices/wsgeraproc/ws/processo';
			} else {
				$urlWS = 'https://www.fnde.gov.br/webservices/wsgeraproc/ws/processo';
			}

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'gera') )
					->execute();

			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml));

			$result = (integer) $xml->status->result;
			if($result){
				$processo	= (string)$xml->status->message->code;
				$banco 		= (string)$xml->body->banco;
				$agencia	= (string)$xml->body->agencia;

				$this->WSHistoricoProcesso( $arqXml, $xmlRetorno, 'GERAPROC - GerarProcesso - Sucesso' );

				$arrRetorno = array('processo' => $processo, 'banco' => $banco, 'agencia' => $agencia, 'arqxml' => $arqXml, 'xmlretorno' => $xmlRetorno);
				return $arrRetorno;

			} else {
				$erros = $xml->status->error->message;
				if(count($erros)>0) {
					foreach($erros as $err) {
						$erroMSG .= ' '.str_replace(array(chr(13),chr(10)), ' ',utf8_decode($err->text)).' ';
						$erroMSG = str_replace( "'", '"', $erroMSG );
					}
				}
				$this->WSHistoricoProcesso( $arqXml, $xmlRetorno, 'GERAPROC - GerarProcesso - Erro' );
				echo $this->mensagem('Gerar Número do Processo no SIGEF', $erroMSG);
				die;
			}
		} catch (Exception $e){

			# Erro 404 página not found
			if($e->getCode() == 404){
				$erro = "Erro-Serviço Gerar Processo encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
				echo $this->mensagem('Gerar Número do Processo no SIGEF', $erro);
				die;
			}

			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			$this->WSHistoricoProcesso( $arqXml, $xmlRetorno, 'GERAPROC - GerarProcesso - Erro Exception' );
			echo $this->mensagem('Gerar Número do Processo no SIGEF', $erroMSG);

            //Envia e-mail ao desevolvimento.
            //enviar_email_dev('gerarProcessoFNDE - Linha:'. $e->getLine(), $e->getMessage());

			die;
		}
	}

	public function solicitarProcesso( $arrParam = array() ) {
		global $db;

		try{
			$an_processo 		= date("Y");
			$nu_processo 		= $arrParam['nu_processo'];
			$tp_processo 		= $arrParam['tp_processo'];
			$co_programa_fnde 	= $arrParam['co_programa_fnde'];
			$nu_sistema			= $arrParam['nu_sistema'];
			$usuario 			= $this->wsusuario;
			$senha   			= $this->wssenha;
			$nu_cnpj_favorecido = $arrParam['nu_cnpj_favorecido'];
			$data_created 		= date("c");

		$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<params>
	      	<nu_cnpj>$nu_cnpj_favorecido</nu_cnpj>
	      	<nu_processo>$nu_processo</nu_processo>
	      	<tp_processo>$tp_processo</tp_processo>
	      	<an_processo>$an_processo</an_processo>
	      	<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		</params>
	</body>
</request>
XML;

			if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://172.20.200.116/webservices/corp/integracao/web/dev.php/processo/solicitar';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/corp/index.php/processo/solicitar';
			}

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
					->execute();

			$xmlRetorno = $xml;
		    $xml = simplexml_load_string( stripslashes($xml));
			$result = (integer) $xml->status->result;

			if(!$result) {
				$erros = $xml->status->error->message;
				if(count($erros)>0) {
					foreach($erros as $err) {
				 		$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $err->text);
					}
				}
				echo $this->mensagem( 'ERRO AO SOLICITAR PROCESSO NO SISTEMA DOCUMENTA:', $mensagem );
				$this->WSHistoricoProcesso($arqXml, $xmlRetorno, 'GERAPROC - solicitarProcesso - Erro');
			    return false;
			} else {
				$this->WSHistoricoProcesso($arqXml, $xmlRetorno, 'GERAPROC - solicitarProcesso - Sucesso');
				return true;
			}
		} catch (Exception $e){

			# Erro 404 página not found
			if($e->getCode() == 404){
				$erro = "Erro-Serviço Solicitar Processo encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
				echo $this->mensagem('Gerar Solicita Processo no SIGEF', $erro);
				die;
			}

			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			$this->WSHistoricoProcesso( $arqXml, $xmlRetorno, 'GERAPROC - solicitarProcesso - Erro Exception' );
			echo $this->mensagem( 'ERRO AO SOLICITAR PROCESSO NO SISTEMA DOCUMENTA:', $erroMSG );
			die;
		}
	}

	public function solicitacaoCancelarProcesso($post){

		try{

			$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$date}</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<nu_identificador>$nu_identificador</nu_identificador>
			<nu_processo>$ptrnumprocessoempenho</nu_processo>
			<somente_conta_ativa>$somente_conta_ativa</somente_conta_ativa>
			<numero_de_linhas>$numero_de_linhas</numero_de_linhas>
		</params>
	</body>
</request>
XML;
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';

			$xml = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWS)
			->setParams( array('xml' => $arqXml, 'method' => 'consultarAndamentoCC') )
			->execute();

			$xmlRetorno = $xml;

			$xml = simplexml_load_string( stripslashes($xml));

			if ( (int) $xml->status->result ){
				$obContaCorrenteWS  = $xml->body->row->children();
				$status 			= (string)	$obContaCorrenteWS->status;
				$co_status			= substr( $status, 0, 1 );

				if( trim($co_status) != 0 ){
					$this->WSHistoricoProcesso( $arqXml, $xmlRetorno, 'GERAPROC - AtualizarContaCorrenteFNDE - Sucesso' );
					return $obContaCorrenteWS;
				} else {
					$this->WSHistoricoProcesso( $arqXml, $xmlRetorno, 'GERAPROC - AtualizarContaCorrenteFNDE - Erro' );
					//echo $this->mensagem('Consultar Conta Corrente no SIGEF', $status);
					return false;
					die;
				}
			} else {
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',utf8_decode($xml->status->error->message->text));
				$erroMSG = str_replace( "'", '"', $erroMSG );
				$this->WSHistoricoProcesso( $arqXml, $xml, 'GERAPROC - AtualizarContaCorrenteFNDE - Erro' );
				echo $this->mensagem('Consultar Conta Corrente no SIGEF', $erroMSG);
				die;
			}
		} catch (Exception $e){
			# Erro 404 página not found
			if($e->getCode() == 404){
				$erro = "Erro-Serviço Cancelar Processo encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
				echo $this->mensagem('Cancelar Processo no SIGEF', $erro);
				die;
			} else {
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
				$erroMSG = str_replace( "'", '"', $erroMSG );
				echo $this->mensagem('Cancelar Processo no SIGEF', $erroMSG);
				die;
			}
		}
	}

	public function cancelarProcesso( $arrParam = array() ) {
		global $db;

		try{
			$an_processo 		= date("Y");
			$nu_processo 		= $arrParam['nu_processo'];
			$tp_processo 		= $arrParam['tp_processo'];
			$co_programa_fnde 	= $arrParam['co_programa_fnde'];
			$nu_sistema			= $arrParam['nu_sistema'];
			$ds_resumo			= $arrParam['ds_resumo'];
			$usuario 			= $this->wsusuario;
			$senha   			= $this->wssenha;
			$nu_cnpj_favorecido = $arrParam['nu_cnpj_favorecido'];
			$data_created 		= date("c");

		$arqXml = <<<XML
<?xml version="1.0" encoding="UTF-8"?>
	<request>
		<header>
			<app>DOCUMENTA</app>
			<version>1</version>
			<created>$data_created</created>
		</header>
		<body>
			<auth>
				<usuario>$usuario</usuario>
				<senha>$senha</senha>
			</auth>
			<params>
				<nu_processo>$nu_processo</nu_processo>
				<ds_justificativa>$ds_resumo</ds_justificativa>
			</params>
		</body>
	</request>
XML;

			if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://hmg.fnde.gov.br/webservices/wsgeraproc/index.php/ws/processo';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wsgeraproc/ws/processo';
			}

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'cancela') )
					->execute();

	ver($xml,d);
			$xmlRetorno = $xml;
			//$xml = simplexml_load_string( stripslashes($xml));
			$result = (integer) $xml->status->result;
			if($result){
				$processo	= (string)$xml->status->message->code;
				$banco 		= (string)$xml->body->banco;
				$agencia	= (string)$xml->body->agencia;
	
				$this->WSHistoricoProcesso( $arqXml, $xmlRetorno, 'GERAPROC - GerarProcesso - Sucesso' );
	
				$arrRetorno = array('processo' => $processo, 'banco' => $banco, 'agencia' => $agencia, 'arqxml' => $arqXml, 'xmlretorno' => $xmlRetorno);
				return $arrRetorno;
	
			} else {
				$erros = $xml->status->error->message;
				if(count($erros)>0) {
					foreach($erros as $err) {
						$erroMSG .= ' '.str_replace(array(chr(13),chr(10)), ' ',utf8_decode($err->text)).' ';
						$erroMSG = str_replace( "'", '"', $erroMSG );
					}
				}
				$this->WSHistoricoProcesso( $arqXml, $xmlRetorno, 'GERAPROC - GerarProcesso - Erro' );
				echo $this->mensagem('Gerar Número do Processo no SIGEF', $erroMSG);
				die;
			}
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				$erro = "Erro-Serviço Solicitar Processo encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
				echo $this->mensagem('Gerar Solicita Processo no SIGEF', $erro);
				die;
			}
				
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			$this->WSHistoricoProcesso( $arqXml, $xmlRetorno, 'GERAPROC - solicitarProcesso - Erro Exception' );
			echo $this->mensagem( 'ERRO AO SOLICITAR PROCESSO NO SISTEMA DOCUMENTA:', $erroMSG );
			die;
		}
	}
	
	public function WSHistoricoProcesso($arqXml, $xmlRetorno, $msg = ''){
		global $db;
		
		$sql = "INSERT INTO seguranca.historicowsprocessofnde(sisid, hwpwebservice, hwpxmlenvio, hwpxmlretorno, hwpdataenvio, usucpf) 
				VALUES ({$_SESSION['sisid']}, '".$msg."', '".addslashes($arqXml)."', '".addslashes($xmlRetorno)."', now(), '".$_SESSION['usucpf']."')";
	
		$db->executar($sql);
		$db->commit();
	}
	
	public function mensagem( $title, $msg ){
		$html = '<div style=" border: 1px solid #B7B7B7; font-size: 11px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
					'.$title.'
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >'.$msg.'</div>
				</div>
			 	<br>';
		return $html;
	}
}
?>

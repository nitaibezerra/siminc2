<?php
include(APPRAIZ."monitora/classes/WSQuantitativo.class.inc");
class ImportacaoSiop extends cls_banco {

	private $diretorio, $urlwsdl, $certificado, $senha_certificado;
	
	public function __construct($wsdl, $certificado, $senha_certificado){
		$this->urlwsdl = $wsdl;
		$this->certificado = $certificado;
		$this->senha_certificado = $senha_certificado;
	}
	
	public function __destruct(){
		
		if( isset($_SESSION['transacao'] ) ){
			pg_query( $this->link, 'rollback; ');
			unset( $_SESSION['transacao'] );
		}
		
	}
	
	public function conectarWS($dados){
		$obWS = new WSQuantitativo($this->urlwsdl, array(
								'local_cert'	=> $this->certificado, 
								'passphrase ' 	=> $this->senha_certificado,
								'exceptions'	=> true,
						        'trace'			=> true,
								'encoding'		=> 'ISO-8859-1' ));
					
		$obterProgramacaoCompletaQuantitativo = new obterProgramacaoCompletaQuantitativo();
		//monta a credencial
		$credencialDTO = new credencialDTO();		
		$credencialDTO->perfil = $this->perfil;
		$credencialDTO->usuario = $dados['wsusuario'];
		$credencialDTO->senha = md5($dados['wssenha']);
		$obterProgramacaoCompletaQuantitativo->credencialDTO = $credencialDTO;
		
		//atribui parâmetros
		$obterProgramacaoCompletaQuantitativo->codigoMomento = 9000;
		$obterProgramacaoCompletaQuantitativo->exercicio = 2012;
		
		//return $obterProgramacaoCompletaQuantitativo;
		$obterProgramacaoCompletaQuantitativoResponse = $obWS->obterProgramacaoCompletaQuantitativo($obterProgramacaoCompletaQuantitativo);
		//ver($obterProgramacaoCompletaQuantitativoResponse,d);
		//return $obterProgramacaoCompletaQuantitativoResponse;
		
		$request = $obWS->__getLastRequest();
		$response = $obWS->__getLastResponse();
		return $response;
		//ver($obterProgramacaoCompletaQuantitativoResponse, htmlentities(utf8_decode($response)));
	}
	
	public function obterProgramacaoCompleta($dados){
		
		$obWS = new WSQuantitativo($this->urlwsdl, array(
								'local_cert'	=> $this->certificado, 
								'passphrase ' 	=> $this->senha_certificado,
								'exceptions'	=> true,
						        'trace'			=> true,
								'encoding'		=> 'ISO-8859-1' ));
					
		$obterProgramacaoCompletaQuantitativo = new obterProgramacaoCompletaQuantitativo();
		//monta a credencial
		$credencialDTO = new credencialDTO();		
		$credencialDTO->perfil = $this->perfil;
		$credencialDTO->usuario = $dados['wsusuario'];
		$credencialDTO->senha = md5($dados['wssenha']);
		$obterProgramacaoCompletaQuantitativo->credencialDTO = $credencialDTO;
		
		//atribui parâmetros
		$obterProgramacaoCompletaQuantitativo->codigoMomento = 9000;
		$obterProgramacaoCompletaQuantitativo->exercicio = 2012;
		
		$obterProgramacaoCompletaQuantitativoResponse = $obWS->obterProgramacaoCompletaQuantitativo($obterProgramacaoCompletaQuantitativo);
		
		$request = $obWS->__getLastRequest();
		$response = $obWS->__getLastResponse();
		//ver($obterProgramacaoCompletaQuantitativoResponse, htmlentities(utf8_decode($response)),d);
		return $obterProgramacaoCompletaQuantitativoResponse;
		
	}

	
	public function gerarDocumentoXML($dados){
		$arrDocumento = (array) $dados["documento"];
		
		$sql = $this->retornoSQL($arrDocumento);
		
		$this->gravarDocumentoXML($sql, $arquivo);
	}
	
	function gravarDocumentoXML( $arrParams, $arquivo ){
		$arquivo = $this->diretorio . 'obterPedidoAlteracao'.date( '-d-m-y' ) . ".xml";
		$objeto = new stdClass();
		$objeto->dados = file_get_contents( $arquivo );
		
		ver( htmlentities($objeto->dados),d);
		
		$documento = $this->ArrayToXML( $arrParams, $arquivo );
		//$documento = $this->criarDocumentoXML( $arrParams, $arquivo );
		
		ver( htmlentities($documento),d);
		//ver( htmlentities($documento->saveXML()),d);
		
		$caminho = $this->diretorio . $arquivo . date( '-d-m-y' ) . ".xml";
		
		if ( $documento instanceof DOMDocument ) {
			return $documento->save( $caminho ) > 0;
		} else {
			file_put_contents( $caminho, null );
		}
	}
	public function ArrayToXML($arrParams, $arquivo){
		$xml = '<?xml version="1.0" encoding="utf-8" ?> 
<soapenv xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:ser="http://servicoweb.siop.sof.planejamento.gov.br/"><obterPedidoAlteracao>';
		
		foreach($arrParams as $k=>$valor){
			if(is_array($valor)){
				foreach ($valor as $indice => $v) {
					$tag = ereg_replace('^[0-9]{1,}','data',$indice); // replace numeric key in array to 'data'
					$xml .= "<$tag>$v</$tag>";
				}
			}else{
				$tag = ereg_replace('^[0-9]{1,}','data',$k); // replace numeric key in array to 'data'
				$xml .= "<$tag>$valor</$tag>";
			}
		}
		$xml .= '</obterPedidoAlteracao></soapenv>';
		return $xml;
	}
	public function criarDocumentoXML( $arrParams, $arquivo ){
		
		# cria documento
		$documento = new DOMDocument( "1.0", "utf-8" );
		$registros = $documento->appendChild( new DOMElement( "soapenv" ) ); #soapenv:Envelope
		$registros->setAttribute( "xmlns:soapenv", "http://schemas.xmlsoap.org/soap/envelope/" );
		$registros->setAttribute( "xmlns:ser", "http://servicoweb.siop.sof.planejamento.gov.br/" );
		
		if( $arrParams ){
			$acao = $registros->appendChild( new DOMElement( $arquivo ) );
					
			foreach ($arrParams as $atributo => $valor) {
				$credencial = '';
				if( is_array($valor) ){
					foreach ($valor as $indice => $v) {
						$tag = ereg_replace('^[0-9]{1,}','data',$indice); // replace numeric key in array to 'data'
						$credencial .= "<$tag>$v</$tag>";
					}
				}
				
				if( $credencial ) $valor = $credencial;
				$valor = str_replace( "'", "&apos;", utf8_encode( htmlspecialchars( $valor ) ) );
				$acao->appendChild( new DOMElement( $atributo, $valor ) );
			}
		} else {
			throw new Exception( "Não há dados para gravar no arquivo {$nome}." );
		}
		return $documento;
	}
	
	public function getSQLFuncao( $dados = array() ){
		$arrDocumento = (array) $dados["documento"];
		
		try {
			# CRIA SQL DE Pedito Alteração
			if ( in_array( "obterPedidoAlteracao", $arrDocumento ) ) {				
				
				$sql = "";
				
				$credencialDTO = array("perfil" => 32,
						   "senha" => $dados['wssenha'],
						   "usuario" => $dados['wsusuario']);
				
				$arrParams = array(
								'credencial' => $credencialDTO,
								'exercicio' => 2011,
								'identificadorUnicoPedido' => 11733,
								'codigoMomento' => '0'
								);
					
				$documento = $this->gravarDocumentoXML($arrParams, 'obterPedidoAlteracao');
			}
		}catch ( Exception $erro ) {
			$db->rollback();
			?>
			<html>
				<head>
					<script type="text/javascript">
					alert( '<?= $erro->getMessage(); ?>' );
					location.href = "?modulo=<?= $_REQUEST["modulo"] ?>&acao=<?= $_REQUEST["acao"] ?>";
					</script>
				</head>
				<body/>
			</html>
			<?php
			exit();
		}
	}
	
	public function enviaArquivoSIOP($dados){
		try {
			$arquivo = 'obterPedidoAlteracao'.date( '-d-m-y' );
			
			$servicos = array(
				"obterPedidoAlteracao" => "obterPedidoAlteracao"
				//"obterPedidoAlteracao" => "recebePrograma"
			);
			
			# verifica se o arquivo existe
			$arquivo = $this->diretorio . basename( $arquivo ) . ".xml";
			
			if( !file_exists( $arquivo ) ) {
				throw new Exception( "O arquivo indicado não existe." );
			}
			
			# identifica o serviço
			preg_match( "/[a-zA-Z]{1,}/", 'obterPedidoAlteracao', $match );
			$servico = $servicos[$match[0]];
			
			# carrega os serviços do sigplan
			//$siopwsdl = new SoapClient($this->urlwsdl, $this->configuracao );
			$siopwsdl = new SoapClient($this->urlwsdl);
			
			# verifica se o método existe
			$verificacao = false;
			$padrao = "/[a-zA-Z ]({$servico})\(/";
			foreach( $siopwsdl->__getFunctions() as $assinatura ) {
				preg_match( $padrao, $assinatura, $match );
				if ( $servico == $match[1] ) {
					# identifica a estrutura de retorno
					$estrutura = substr( $assinatura, 0, strpos( $assinatura, " " ));
					$estrutura = str_replace( "Response", "Result", $estrutura );
					$verificacao = true;
					break;
				}
			}
			if ( !$verificacao ) {
				throw new Exception( "O serviço solicitado não existe." );
			}
			$usuario = WEB_SERVICE_SIOP_USUARIO;
			$senha = WEB_SERVICE_SIOP_SENHA;
			
			# monta parâmetros
			$objeto = new stdClass();
			$objeto->usuario = $usuario;
			$objeto->senha = $senha;
			$objeto->dados = file_get_contents( $arquivo );
			
			# realiza a requisição
			$retorno = $siopwsdl->$servico( $objeto )->$estrutura;
			
			if ( $retorno->numerocarga == -1 ) {
				throw new Exception( "{$retorno->mensagem} {$retorno->descricao}" );
			}
			ver($retorno,d);
			/*$db->commit();
			$_REQUEST["acao"] = "E";
			$db->sucesso( $_REQUEST["modulo"] );*/
			
		} catch ( Exception $erro ) {
			//$db->rollback();
			dump( $erro, true );
			?>
			<html>
				<head>
					<script type="text/javascript">
					alert( '<?= $erro->getMessage() ?>' );
					//location.href = "?modulo=<?= $_REQUEST["modulo"] ?>&acao=E";
					</script>
				</head>
				<body>&nbsp;</body>
			</html>
			<?
			exit();
		}
	}
}
?>
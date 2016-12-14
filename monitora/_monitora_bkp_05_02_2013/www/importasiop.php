<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

include(APPRAIZ."monitora/classes/AlteracaoOrcamentariaDAO.class.inc");
	include(APPRAIZ."monitora/classes/WSAlteracoesOrcamentaria.class.inc");
	
	//Endereço do wsdl do serviço
	$wsdl = WEB_SERVICE_SIOP_URL. "WSAlteracoesOrcamentarias?wsdl";
	$certificado = WEB_SERVICE_SIOP_CERTIFICADO;
	//Senha do certificado
	$senha_certificado = WEB_SERVICE_SIOP_SENHA;
	$documento = $_POST['documento'];
	
	$obAlteracaoDAO = new AlteracaoOrcamentariaDAO();
	
	$wsAlteracao = new WSAlteracoesOrcamentarias($wsdl, array(
			'local_cert'	=> $certificado, 
			'passphrase ' 	=> $senha_certificado,
			'exceptions'	=> true,
	        'trace'			=> true,
			'encoding'		=> 'ISO-8859-1' ));
	
	//monta a credencial
	$credencial = new credencialDTO();
	$credencial->perfil = 32;
	$credencial->usuario = WEB_SERVICE_SIOP_USUARIO;
	$credencial->senha = WEB_SERVICE_SIOP_SENHA;
	
	$arrPedido = $obAlteracaoDAO->carregarPedidoAlteracao();
	//ver($arrPedido,d);
	foreach ($arrPedido as $pedido) {
		$pedido['codigotipoinclusaolocalizador'] = $obAlteracaoDAO->getTipoInclusaoLocalizador($pedido);
		if( $pedido['codigotipofonterecurso'] == '2' || $pedido['codigotipofonterecurso'] == '1' ){
			
			if( $pedido['codigotipofonterecurso'] == '1' ){
				$pedido['valorcancelamento'] = $pedido['valordespesa'];
				$pedido['valorsuplementacao'] = '0';
			}else{
				$pedido['valorcancelamento'] = '0';
				$pedido['valorsuplementacao'] = $pedido['valordespesa'];
			}				
			$pedido['codigotipofonterecurso'] = '1';
			
		}
		if( $pedido['codigotipofonterecurso'] == '3' ){
			$pedido['codigotipofonterecurso'] = '2';
			$pedido['valorcancelamento'] = '0';
			$pedido['valorsuplementacao'] = $pedido['valordespesa'];
		}
		if( $pedido['codigotipofonterecurso'] == '4' ){
			$pedido['codigotipofonterecurso'] = '3';
			$pedido['valorcancelamento'] = '0';
			$pedido['valorsuplementacao'] = $pedido['valordespesa'];
		}
		
		//$pedido['codigotipoalteracao'] 	= '4';
		$pedido['quantidadeacrescimo'] 	= '1';
		$pedido['quantidadereducao'] 	= '0';
		$pedido['codigofonte'] 			= '200';
		
		$arrResposta = array('codigopergunta' => '114', 'resposta' => 'A universidade precisa comprar equipamentos para prédios recém-inaugurados, e as verbas orçamentárias disponíveis não são suficientes para comprar todos os equipamentos necessários.' );
		$obfinanceiroPedidoAlteracaoDTO = new financeiroPedidoAlteracaoDTO( $pedido );
		$obfisicoPedidoAlteracaoDTO = new fisicoPedidoAlteracaoDTO( $pedido, $obfinanceiroPedidoAlteracaoDTO );
		$obrespostaJustificativaDTO[] = new respostaJustificativaDTO( $arrResposta );

		$obPedidoAlteracaoDTO 			= new pedidoAlteracaoDTO( $pedido, $obfisicoPedidoAlteracaoDTO, $obrespostaJustificativaDTO );
		//ver($obPedidoAlteracaoDTO);
		
		$obCadastrarPedidoAlteracao = new cadastrarPedidoAlteracao();
		$obCadastrarPedidoAlteracao->credencial = $credencial;
		$obCadastrarPedidoAlteracao->pedidoAlteracaoDTO = $obPedidoAlteracaoDTO;
		//ver($obCadastrarPedidoAlteracao,d);
		
		$cadastrarPedidoAlteracaoResponse = $wsAlteracao->cadastrarPedidoAlteracao( $obCadastrarPedidoAlteracao );
		ver($cadastrarPedidoAlteracaoResponse,d);
		$retorno = $obAlteracaoDAO->manterCadastrarPedidoAlteracaoResponse( $cadastrarPedidoAlteracaoResponse );
		ver($retorno,d);
	}
?>
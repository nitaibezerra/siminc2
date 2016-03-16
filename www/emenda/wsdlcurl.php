<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

if(!$_SESSION['usucpf']) $_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$caminho_certificado	=  "planacomorc/modulos/sistema/comunica/WS_SISMEC_2.pem";
$senha_certificado		= "sismec";
$wsdl = 'https://testews.siop.gov.br/services/WSAlteracoesOrcamentarias?wsdl';

?>
<html>
<head>
<title>SIMEC - Web Services</title>
<meta http-equiv='Content-Type' content='text/html; charset=ISO-8895-1'>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
</head>
<body>
<form action="" method="post" id="formulario" name="formulario">
<input type="hidden" name="requisicao" id="requisicao" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=6 align="center" style="width: 100%">
	<tr>
		<th colspan="4" style="text-align: center;">Cliente WS</th>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="25%">Diretório Certificado:</td>
		<td colspan="3"><?php
			$caminho = ($_POST['caminho'] ? $_POST['caminho'] : $caminho_certificado);
			echo campo_texto('caminho', 'N', 'S', '', 150, 200, '', '');
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Senha Certificado:</td>
		<td colspan="3">
		<?php 
			$senha = ( $_POST['senha'] ? $_POST['senha'] : $senha_certificado);
			echo campo_texto( 'senha', 'N', 'S', '', 50, 200, '', ''); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >URL:</td>
		<td colspan="3">
		<?php
		$wsdl = ( $_POST['wsdl'] ? $_POST['wsdl'] : $wsdl);
		echo campo_texto( 'wsdl', 'N', 'S', '', 150, 200, '', '');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan="4" style="text-align: center;">
			<input class="botao" type="button" name="enviar" id= "enviar" value="Enviar" onclick="submeteFormulario('1');">
			<input class="botao" type="button" name="visualizar" id= "visualizar" value="Visualizar XML" onclick="submeteFormulario('2');">
		</td>
	</tr>
</table>
</form>
</body>
<script type="text/javascript">
function submeteFormulario( tipo ){
	if( tipo == 2 ) $('#requisicao').val('visualizar');
	else $('#requisicao').val('enviar');
	$('#formulario').submit();
}
</script>
</html>
<?php 

if( $_REQUEST['requisicao'] == 'enviar' || $_REQUEST['requisicao'] == 'visualizar' ){

$wsdl 					= $_REQUEST['wsdl'];
$caminho_certificado 	= APPRAIZ.$_REQUEST['caminho'];
$senha_certificado 		= $_REQUEST['senha'];

$str = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<definitions name='WSAlteracoesOrcamentarias' targetNamespace='http://servicoweb.siop.sof.planejamento.gov.br/' xmlns='http://schemas.xmlsoap.org/wsdl/' xmlns:soap='http://schemas.xmlsoap.org/wsdl/soap/' xmlns:tns='http://servicoweb.siop.sof.planejamento.gov.br/' xmlns:xsd='http://www.w3.org/2001/XMLSchema'>
 <types>
  <xs:schema targetNamespace='http://servicoweb.siop.sof.planejamento.gov.br/' version='1.0' xmlns:tns='http://servicoweb.siop.sof.planejamento.gov.br/' xmlns:xs='http://www.w3.org/2001/XMLSchema'>
   <xs:element name='cadastrarAnalisesEmendas' type='tns:cadastrarAnalisesEmendas'/>
   <xs:element name='cadastrarAnalisesEmendasResponse' type='tns:cadastrarAnalisesEmendasResponse'/>
   <xs:element name='cadastrarPedidoAlteracao' type='tns:cadastrarPedidoAlteracao'/>
   <xs:element name='cadastrarPedidoAlteracaoRemanejamentoEmendas' type='tns:cadastrarPedidoAlteracaoRemanejamentoEmendas'/>
   <xs:element name='cadastrarPedidoAlteracaoRemanejamentoEmendasResponse' type='tns:cadastrarPedidoAlteracaoRemanejamentoEmendasResponse'/>
   <xs:element name='cadastrarPedidoAlteracaoResponse' type='tns:cadastrarPedidoAlteracaoResponse'/>
   <xs:element name='cadastrarPedidoPAC' type='tns:cadastrarPedidoPAC'/>
   <xs:element name='cadastrarPedidoPACResponse' type='tns:cadastrarPedidoPACResponse'/>
   <xs:element name='consultarSituacaoTransmissaoSiafi' type='tns:consultarSituacaoTransmissaoSiafi'/>
   <xs:element name='consultarSituacaoTransmissaoSiafiResponse' type='tns:consultarSituacaoTransmissaoSiafiResponse'/>
   <xs:element name='enviarPedidoAlteracao' type='tns:enviarPedidoAlteracao'/>
   <xs:element name='enviarPedidoAlteracaoResponse' type='tns:enviarPedidoAlteracaoResponse'/>
   <xs:element name='excluirPedidoAlteracao' type='tns:excluirPedidoAlteracao'/>
   <xs:element name='excluirPedidoAlteracaoResponse' type='tns:excluirPedidoAlteracaoResponse'/>
   <xs:element name='obterAnalisesEmendas' type='tns:obterAnalisesEmendas'/>
   <xs:element name='obterAnalisesEmendasResponse' type='tns:obterAnalisesEmendasResponse'/>
   <xs:element name='obterEmendasAprovadas' type='tns:obterEmendasAprovadas'/>
   <xs:element name='obterEmendasAprovadasResponse' type='tns:obterEmendasAprovadasResponse'/>
   <xs:element name='obterPedidoAlteracao' type='tns:obterPedidoAlteracao'/>
   <xs:element name='obterPedidoAlteracaoResponse' type='tns:obterPedidoAlteracaoResponse'/>
   <xs:element name='obterPedidosAlteracao' type='tns:obterPedidosAlteracao'/>
   <xs:element name='obterPedidosAlteracaoPorDescricao' type='tns:obterPedidosAlteracaoPorDescricao'/>
   <xs:element name='obterPedidosAlteracaoPorDescricaoResponse' type='tns:obterPedidosAlteracaoPorDescricaoResponse'/>
   <xs:element name='obterPedidosAlteracaoResponse' type='tns:obterPedidosAlteracaoResponse'/>
   <xs:element name='obterPerguntaJustificativa' type='tns:obterPerguntaJustificativa'/>
   <xs:element name='obterPerguntaJustificativaResponse' type='tns:obterPerguntaJustificativaResponse'/>
   <xs:element name='obterPerguntasJustificativa' type='tns:obterPerguntasJustificativa'/>
   <xs:element name='obterPerguntasJustificativaResponse' type='tns:obterPerguntasJustificativaResponse'/>
   <xs:element name='obterSaldosAcoesPAC' type='tns:obterSaldosAcoesPAC'/>
   <xs:element name='obterSaldosAcoesPACResponse' type='tns:obterSaldosAcoesPACResponse'/>
   <xs:element name='obterTabelasApoioAlteracoesOrcamentarias' type='tns:obterTabelasApoioAlteracoesOrcamentarias'/>
   <xs:element name='obterTabelasApoioAlteracoesOrcamentariasResponse' type='tns:obterTabelasApoioAlteracoesOrcamentariasResponse'/>
   <xs:element name='verificarPedidoAlteracao' type='tns:verificarPedidoAlteracao'/>
   <xs:element name='verificarPedidoAlteracaoResponse' type='tns:verificarPedidoAlteracaoResponse'/>
   <xs:complexType name='obterPedidoAlteracao'>
    <xs:sequence>
     <xs:element minOccurs='0' name='credencial' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='exercicio' type='xs:int'/>
     <xs:element minOccurs='0' name='identificadorUnicoPedido' type='xs:int'/>
     <xs:element minOccurs='0' name='codigoMomento' type='xs:int'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='credencialDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='perfil' type='xs:int'/>
       <xs:element name='senha' type='xs:string'/>
       <xs:element name='usuario' type='xs:string'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType abstract='true' name='baseDTO'>
    <xs:sequence/>
   </xs:complexType>
   <xs:complexType name='obterPedidoAlteracaoResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoPedidoAlteracaoDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='retornoPedidoAlteracaoDTO'>
    <xs:complexContent>
     <xs:extension base='tns:retornoDTO'>
      <xs:sequence>
       <xs:element maxOccurs='unbounded' minOccurs='0' name='registros' nillable='true' type='tns:pedidoAlteracaoDTO'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='retornoDTO'>
    <xs:sequence>
     <xs:element maxOccurs='unbounded' minOccurs='0' name='mensagensErro' nillable='true' type='xs:string'/>
     <xs:element name='sucesso' type='xs:boolean'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='pedidoAlteracaoDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='codigoClassificacaoAlteracao' type='xs:int'/>
       <xs:element minOccurs='0' name='codigoInstrumentoLegal' type='xs:int'/>
       <xs:element minOccurs='0' name='codigoMomento' type='xs:int'/>
       <xs:element minOccurs='0' name='codigoOrgao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoSituacaoPedidoAlteracao' type='xs:int'/>
       <xs:element minOccurs='0' name='codigoTipoAlteracao' type='xs:string'/>
       <xs:element minOccurs='0' name='dataCriacao' type='xs:dateTime'/>
       <xs:element minOccurs='0' name='dataEfetivacao' type='xs:dateTime'/>
       <xs:element minOccurs='0' name='dataEnvio' type='xs:dateTime'/>
       <xs:element minOccurs='0' name='descricao' type='xs:string'/>
       <xs:element minOccurs='0' name='exercicio' type='xs:int'/>
       <xs:element maxOccurs='unbounded' minOccurs='0' name='fisicosPedidoAlteracao' nillable='true' type='tns:fisicoPedidoAlteracaoDTO'/>
       <xs:element minOccurs='0' name='identificadorUnico' type='xs:int'/>
       <xs:element minOccurs='0' name='identificadorUnicoPedidoAgregador' type='xs:int'/>
       <xs:element minOccurs='0' name='identificadorUnicoPedidoOrigem' type='xs:int'/>
       <xs:element minOccurs='0' name='loginUsuarioCriacao' type='xs:string'/>
       <xs:element minOccurs='0' name='loginUsuarioEfetivacao' type='xs:string'/>
       <xs:element minOccurs='0' name='loginUsuarioEnvio' type='xs:string'/>
       <xs:element minOccurs='0' name='nomeUsuarioCriacao' type='xs:string'/>
       <xs:element minOccurs='0' name='nomeUsuarioEfetivacao' type='xs:string'/>
       <xs:element minOccurs='0' name='nomeUsuarioEnvio' type='xs:string'/>
       <xs:element maxOccurs='unbounded' minOccurs='0' name='respostasJustificativa' nillable='true' type='tns:respostaJustificativaDTO'/>
       <xs:element minOccurs='0' name='snAgregadora' type='xs:boolean'/>
       <xs:element minOccurs='0' name='snAtual' type='xs:boolean'/>
       <xs:element minOccurs='0' name='snEmValidacaoExterna' type='xs:boolean'/>
       <xs:element minOccurs='0' name='snEnviadoCongressoNacional' type='xs:boolean'/>
       <xs:element minOccurs='0' name='snExclusaoLogica' type='xs:boolean'/>
       <xs:element minOccurs='0' name='snIntegracao' type='xs:boolean'/>
       <xs:element name='snOrcamentoInvestimento' type='xs:boolean'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='fisicoPedidoAlteracaoDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='codigoAcao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoEsfera' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoFuncao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoLocalizador' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoPrograma' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoSubFuncao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoTipoInclusaoAcao' type='xs:int'/>
       <xs:element minOccurs='0' name='codigoTipoInclusaoLocalizador' type='xs:int'/>
       <xs:element minOccurs='0' name='codigoUO' type='xs:string'/>
       <xs:element minOccurs='0' name='exercicio' type='xs:int'/>
       <xs:element maxOccurs='unbounded' minOccurs='0' name='listaFinanceiroPedidoAlteracaoDTO' nillable='true' type='tns:financeiroPedidoAlteracaoDTO'/>
       <xs:element minOccurs='0' name='quantidadeAcrescimo' type='xs:long'/>
       <xs:element minOccurs='0' name='quantidadeReducao' type='xs:long'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='financeiroPedidoAlteracaoDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='codigoFonte' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoIdOC' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoIdUso' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoNatureza' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoRP' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoRPLei' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoTipoFonteRecurso' type='xs:int'/>
       <xs:element maxOccurs='unbounded' minOccurs='0' name='fisicoFinanceiroEmendaOrigemPedidoAlteracaoDTO' nillable='true' type='tns:fisicoFinanceiroEmendaOrigemPedidoAlteracaoDTO'/>
       <xs:element minOccurs='0' name='planoOrcamentario' type='xs:string'/>
       <xs:element minOccurs='0' name='valorCancelamento' type='xs:long'/>
       <xs:element minOccurs='0' name='valorSuplementacao' type='xs:long'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='fisicoFinanceiroEmendaOrigemPedidoAlteracaoDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='codigoAcao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoEmendaOrigem' type='xs:int'/>
       <xs:element minOccurs='0' name='codigoEsfera' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoFuncao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoLocalizador' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoPrograma' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoSubFuncao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoTipoInclusaoLocalizador' type='xs:int'/>
       <xs:element minOccurs='0' name='codigoUO' type='xs:string'/>
       <xs:element minOccurs='0' name='financeiroPedidoAlteracaoDTO' type='tns:financeiroPedidoAlteracaoDTO'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='respostaJustificativaDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='codigoPergunta' type='xs:int'/>
       <xs:element minOccurs='0' name='resposta' type='xs:string'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='excluirPedidoAlteracao'>
    <xs:sequence>
     <xs:element minOccurs='0' name='credencial' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='exercicio' type='xs:int'/>
     <xs:element minOccurs='0' name='identificadorUnico' type='xs:int'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='excluirPedidoAlteracaoResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoPedidoAlteracaoDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='obterPerguntaJustificativa'>
    <xs:sequence>
     <xs:element minOccurs='0' name='credencial' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='codigoPergunta' type='xs:int'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='obterPerguntaJustificativaResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoPerguntaJustificativaDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='retornoPerguntaJustificativaDTO'>
    <xs:complexContent>
     <xs:extension base='tns:retornoDTO'>
      <xs:sequence>
       <xs:element maxOccurs='unbounded' minOccurs='0' name='registros' nillable='true' type='tns:perguntaJustificativaDTO'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='perguntaJustificativaDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='codigoPergunta' type='xs:int'/>
       <xs:element minOccurs='0' name='pergunta' type='xs:string'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='obterPerguntasJustificativa'>
    <xs:sequence>
     <xs:element minOccurs='0' name='credencial' type='tns:credencialDTO'/>
     <xs:element name='orcamentoInvestimento' type='xs:boolean'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='obterPerguntasJustificativaResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoPerguntaJustificativaDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='obterTabelasApoioAlteracoesOrcamentarias'>
    <xs:sequence>
     <xs:element minOccurs='0' name='credencial' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='exercicio' type='xs:int'/>
     <xs:element minOccurs='0' name='retornarClassificacoesAlteracao' type='xs:boolean'/>
     <xs:element minOccurs='0' name='retornarTiposAlteracao' type='xs:boolean'/>
     <xs:element minOccurs='0' name='retornarSituacoesPedidoAlteracao' type='xs:boolean'/>
     <xs:element minOccurs='0' name='retornarTiposInstrumentoLegal' type='xs:boolean'/>
     <xs:element minOccurs='0' name='retornarTiposFonteRecurso' type='xs:boolean'/>
     <xs:element minOccurs='0' name='dataHoraReferencia' type='xs:dateTime'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='obterTabelasApoioAlteracoesOrcamentariasResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoApoioAlteracoesOrcamentariasDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='retornoApoioAlteracoesOrcamentariasDTO'>
    <xs:complexContent>
     <xs:extension base='tns:retornoDTO'>
      <xs:sequence>
       <xs:element maxOccurs='unbounded' minOccurs='0' name='classificacoesAlteracaoDTO' nillable='true' type='tns:classificacaoAlteracaoDTO'/>
       <xs:element maxOccurs='unbounded' minOccurs='0' name='situacoesPedidoAlteracaoDTO' nillable='true' type='tns:situacaoPedidoAlteracaoDTO'/>
       <xs:element maxOccurs='unbounded' minOccurs='0' name='tiposAlteracaoDTO' nillable='true' type='tns:tipoAlteracaoDTO'/>
       <xs:element maxOccurs='unbounded' minOccurs='0' name='tiposFonteRecursoDTO' nillable='true' type='tns:tipoFonteRecursoDTO'/>
       <xs:element maxOccurs='unbounded' minOccurs='0' name='tiposInstrumentoLegalDTO' nillable='true' type='tns:tipoInstrumentoLegalDTO'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='classificacaoAlteracaoDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='codigoClassificacaoAlteracao' type='xs:int'/>
       <xs:element minOccurs='0' name='descricao' type='xs:string'/>
       <xs:element minOccurs='0' name='snAtivo' type='xs:boolean'/>
       <xs:element minOccurs='0' name='snTipoCredito' type='xs:boolean'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='situacaoPedidoAlteracaoDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='codigoSituacaoPedidoAlteracao' type='xs:int'/>
       <xs:element minOccurs='0' name='descricao' type='xs:string'/>
       <xs:element minOccurs='0' name='snAtivo' type='xs:boolean'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='tipoAlteracaoDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='baseLegal' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoClassificacaoAlteracao' type='xs:int'/>
       <xs:element minOccurs='0' name='codigoTipoAlteracao' type='xs:string'/>
       <xs:element minOccurs='0' name='descricao' type='xs:string'/>
       <xs:element minOccurs='0' name='exercicio' type='xs:int'/>
       <xs:element minOccurs='0' name='snOrcamentoInvestimento' type='xs:boolean'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='tipoFonteRecursoDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='codigoTipoFonteRecurso' type='xs:int'/>
       <xs:element minOccurs='0' name='descricao' type='xs:string'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='tipoInstrumentoLegalDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='codigoTipoInstrumentoLegal' type='xs:int'/>
       <xs:element minOccurs='0' name='descricao' type='xs:string'/>
       <xs:element minOccurs='0' name='snAtivo' type='xs:boolean'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='consultarSituacaoTransmissaoSiafi'>
    <xs:sequence>
     <xs:element minOccurs='0' name='credencial' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='exercicio' type='xs:int'/>
     <xs:element minOccurs='0' name='identificadorUnico' type='xs:int'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='consultarSituacaoTransmissaoSiafiResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoSituacaoTransmissaoSiafiDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='retornoSituacaoTransmissaoSiafiDTO'>
    <xs:complexContent>
     <xs:extension base='tns:retornoDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='codigoSituacao' type='xs:int'/>
       <xs:element minOccurs='0' name='descricaoSituacao' type='xs:string'/>
       <xs:element minOccurs='0' name='ESB' type='xs:string'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='obterSaldosAcoesPAC'>
    <xs:sequence>
     <xs:element minOccurs='0' name='credencial' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='exercicio' type='xs:int'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='obterSaldosAcoesPACResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoSaldosBloqueioPAC'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='retornoSaldosBloqueioPAC'>
    <xs:complexContent>
     <xs:extension base='tns:retornoDTO'>
      <xs:sequence>
       <xs:element maxOccurs='unbounded' minOccurs='0' name='saldoBloqueioDotacao' nillable='true' type='tns:saldoBloqueioDotacaoDTO'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='saldoBloqueioDotacaoDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='anoExercicio' type='xs:int'/>
       <xs:element minOccurs='0' name='anoReferencia' type='xs:int'/>
       <xs:element minOccurs='0' name='bloqueioAtual' type='xs:decimal'/>
       <xs:element minOccurs='0' name='categoriaEconomica' type='xs:string'/>
       <xs:element minOccurs='0' name='celula' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoAcao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoEsfera' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoFonte' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoFuncao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoIdOC' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoIdUso' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoLocalizador' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoPrograma' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoRP' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoRPLei' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoSubFuncao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoUnidadeOrcamentaria' type='xs:string'/>
       <xs:element minOccurs='0' name='dataGeracao' type='xs:dateTime'/>
       <xs:element minOccurs='0' name='dotacaoAtual' type='xs:decimal'/>
       <xs:element minOccurs='0' name='grupoNaturezaDespesa' type='xs:string'/>
       <xs:element name='indicadorFuncionalPac' type='xs:boolean'/>
       <xs:element name='indicadorRap' type='xs:boolean'/>
       <xs:element minOccurs='0' name='modalidadeDeAplicacao' type='xs:string'/>
       <xs:element minOccurs='0' name='planoOrcamentario' type='xs:string'/>
       <xs:element minOccurs='0' name='saldo' type='xs:decimal'/>
       <xs:element minOccurs='0' name='tipoCredito' type='xs:string'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='obterAnalisesEmendas'>
    <xs:sequence>
     <xs:element minOccurs='0' name='CredencialDTO' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='FiltroAnaliseEmendaDTO' type='tns:filtroAnaliseEmendaDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='filtroAnaliseEmendaDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='exercicio' type='xs:int'/>
       <xs:element minOccurs='0' name='codigoOrgao' type='xs:string'/>
       <xs:element name='codigosUO'>
        <xs:complexType>
         <xs:sequence>
          <xs:element maxOccurs='unbounded' minOccurs='0' name='codigoUO' type='xs:string'/>
         </xs:sequence>
        </xs:complexType>
       </xs:element>
       <xs:element name='codigosParlamentar'>
        <xs:complexType>
         <xs:sequence>
          <xs:element maxOccurs='unbounded' minOccurs='0' name='codigoParlamentar' type='xs:string'/>
         </xs:sequence>
        </xs:complexType>
       </xs:element>
       <xs:element name='codigoMomento' type='xs:int'/>
       <xs:element minOccurs='0' name='indicadorImpedimento' type='xs:string'/>
       <xs:element minOccurs='0' name='snAtual' type='xs:boolean'/>
       <xs:element minOccurs='0' name='snValidado' type='xs:boolean'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='obterAnalisesEmendasResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoAnaliseEmendaDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='retornoAnaliseEmendaDTO'>
    <xs:complexContent>
     <xs:extension base='tns:retornoDTO'>
      <xs:sequence>
       <xs:element name='analisesEmenda'>
        <xs:complexType>
         <xs:sequence>
          <xs:element maxOccurs='unbounded' minOccurs='0' name='analiseEmenda' type='tns:analiseEmendaDTO'/>
         </xs:sequence>
        </xs:complexType>
       </xs:element>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='analiseEmendaDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='identificadorUnicoLocalizador' type='xs:int'/>
       <xs:element minOccurs='0' name='esfera' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoUO' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoPrograma' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoFuncao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoSubFuncao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoAcao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoLocalizador' type='xs:string'/>
       <xs:element minOccurs='0' name='naturezaDespesa' type='xs:string'/>
       <xs:element minOccurs='0' name='resultadoPrimario' type='xs:string'/>
       <xs:element minOccurs='0' name='fonte' type='xs:string'/>
       <xs:element minOccurs='0' name='idUso' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoParlamentar' type='xs:int'/>
       <xs:element minOccurs='0' name='numeroEmenda' type='xs:int'/>
       <xs:element minOccurs='0' name='siglaPartido' type='xs:string'/>
       <xs:element minOccurs='0' name='ufParlamentar' type='xs:string'/>
       <xs:element minOccurs='0' name='valorAtual' type='xs:long'/>
       <xs:element minOccurs='0' name='codigoMomento' type='xs:int'/>
       <xs:element minOccurs='0' name='indicadorImpedimento' type='xs:string'/>
       <xs:element minOccurs='0' name='snValidado' type='xs:boolean'/>
       <xs:element minOccurs='0' name='snAtual' type='xs:boolean'/>
       <xs:element minOccurs='0' name='valorImpedimento' type='xs:long'/>
       <xs:element name='codigosImpedimento'>
        <xs:complexType>
         <xs:sequence>
          <xs:element maxOccurs='unbounded' minOccurs='0' name='codigoImpedimento' type='xs:int'/>
         </xs:sequence>
        </xs:complexType>
       </xs:element>
       <xs:element minOccurs='0' name='justificativa' type='xs:string'/>
       <xs:element minOccurs='0' name='planilhaDetalhe' type='tns:arquivoAnexoDTO'/>
       <xs:element maxOccurs='unbounded' minOccurs='0' name='outrosArquivos' nillable='true' type='tns:arquivoAnexoDTO'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='arquivoAnexoDTO'>
    <xs:sequence>
     <xs:element minOccurs='0' name='arquivo' type='xs:base64Binary'/>
     <xs:element minOccurs='0' name='descricao' type='xs:string'/>
     <xs:element minOccurs='0' name='nome' type='xs:string'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='cadastrarAnalisesEmendas'>
    <xs:sequence>
     <xs:element minOccurs='0' name='CredencialDTO' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='Integer' type='xs:int'/>
     <xs:element minOccurs='0' name='AnaliseEmendaDTO' type='tns:analiseEmendaDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='cadastrarAnalisesEmendasResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoCadastrarAnaliseEmendaDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='retornoCadastrarAnaliseEmendaDTO'>
    <xs:complexContent>
     <xs:extension base='tns:retornoDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='analiseEmendaDTO' type='tns:analiseEmendaDTO'/>
       <xs:element minOccurs='0' name='pendencias'>
        <xs:complexType>
         <xs:sequence>
          <xs:element maxOccurs='unbounded' minOccurs='0' name='pendencia' type='xs:string'/>
         </xs:sequence>
        </xs:complexType>
       </xs:element>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='obterPedidosAlteracao'>
    <xs:sequence>
     <xs:element minOccurs='0' name='credencial' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='exercicio' type='xs:int'/>
     <xs:element minOccurs='0' name='codigoMomento' type='xs:int'/>
     <xs:element minOccurs='0' name='filtroFuncionalProgramatica' type='tns:filtroFuncionalProgramaticaDTO'/>
     <xs:element minOccurs='0' name='dataHoraUltimaConsulta' type='xs:dateTime'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='filtroFuncionalProgramaticaDTO'>
    <xs:sequence>
     <xs:element minOccurs='0' name='codigoAcao' type='xs:string'/>
     <xs:element minOccurs='0' name='codigoEsfera' type='xs:string'/>
     <xs:element minOccurs='0' name='codigoFuncao' type='xs:string'/>
     <xs:element minOccurs='0' name='codigoLocalizador' type='xs:string'/>
     <xs:element minOccurs='0' name='codigoPrograma' type='xs:string'/>
     <xs:element minOccurs='0' name='codigoSubFuncao' type='xs:string'/>
     <xs:element minOccurs='0' name='codigoTipoInclusaoAcao' type='xs:int'/>
     <xs:element minOccurs='0' name='codigoTipoInclusaoLocalizador' type='xs:int'/>
     <xs:element minOccurs='0' name='codigoUO' type='xs:string'/>
     <xs:element minOccurs='0' name='exercicio' type='xs:int'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='obterPedidosAlteracaoResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoPedidoAlteracaoDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='obterPedidosAlteracaoPorDescricao'>
    <xs:sequence>
     <xs:element minOccurs='0' name='credencial' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='exercicio' type='xs:int'/>
     <xs:element minOccurs='0' name='codigoMomento' type='xs:int'/>
     <xs:element minOccurs='0' name='descricao' type='xs:string'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='obterPedidosAlteracaoPorDescricaoResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoPedidoAlteracaoDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='cadastrarPedidoAlteracao'>
    <xs:sequence>
     <xs:element minOccurs='0' name='credencial' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='pedidoAlteracao' type='tns:pedidoAlteracaoDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='cadastrarPedidoAlteracaoResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoPedidoAlteracaoDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='cadastrarPedidoAlteracaoRemanejamentoEmendas'>
    <xs:sequence>
     <xs:element minOccurs='0' name='credencial' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='tipoCredito' type='xs:string'/>
     <xs:element minOccurs='0' name='restricao' type='xs:string'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='cadastrarPedidoAlteracaoRemanejamentoEmendasResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoPedidoAlteracaoDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='cadastrarPedidoPAC'>
    <xs:sequence>
     <xs:element minOccurs='0' name='credencial' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='pedidoAlteracao' type='tns:pedidoAlteracaoDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='cadastrarPedidoPACResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoPedidoPACDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='retornoPedidoPACDTO'>
    <xs:complexContent>
     <xs:extension base='tns:retornoDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='ESB' type='xs:string'/>
       <xs:element name='fitaGerada' type='xs:boolean'/>
       <xs:element minOccurs='0' name='identificadorUnico' type='xs:int'/>
       <xs:element name='pedidoEfetivado' type='xs:boolean'/>
       <xs:element name='pedidoSalvo' type='xs:boolean'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='verificarPedidoAlteracao'>
    <xs:sequence>
     <xs:element minOccurs='0' name='credencial' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='exercicio' type='xs:int'/>
     <xs:element minOccurs='0' name='identificadorUnico' type='xs:int'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='verificarPedidoAlteracaoResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoVerificacaoPedidoAlteracaoDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='retornoVerificacaoPedidoAlteracaoDTO'>
    <xs:complexContent>
     <xs:extension base='tns:retornoDTO'>
      <xs:sequence>
       <xs:element name='verificacoes'>
        <xs:complexType>
         <xs:sequence>
          <xs:element maxOccurs='unbounded' minOccurs='0' name='verificacao' type='tns:verificacaoPedidoAlteracaoDTO'/>
         </xs:sequence>
        </xs:complexType>
       </xs:element>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='verificacaoPedidoAlteracaoDTO'>
    <xs:sequence>
     <xs:element minOccurs='0' name='regra' type='xs:string'/>
     <xs:element name='passou' type='xs:boolean'/>
     <xs:element name='snInformativa' type='xs:boolean'/>
     <xs:element name='snConfirmacaoEnvio' type='xs:boolean'/>
     <xs:element name='detalhes'>
      <xs:complexType>
       <xs:sequence>
        <xs:element maxOccurs='unbounded' minOccurs='0' name='detalhe' type='xs:string'/>
       </xs:sequence>
      </xs:complexType>
     </xs:element>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='enviarPedidoAlteracao'>
    <xs:sequence>
     <xs:element minOccurs='0' name='credencial' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='exercicio' type='xs:int'/>
     <xs:element minOccurs='0' name='identificadorUnico' type='xs:int'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='enviarPedidoAlteracaoResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoVerificacaoPedidoAlteracaoDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='obterEmendasAprovadas'>
    <xs:sequence>
     <xs:element minOccurs='0' name='CredencialDTO' type='tns:credencialDTO'/>
     <xs:element minOccurs='0' name='FiltroEmendaAprovadaDTO' type='tns:filtroEmendaAprovadaDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='filtroEmendaAprovadaDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='exercicio' type='xs:int'/>
       <xs:element name='codigosUO'>
        <xs:complexType>
         <xs:sequence>
          <xs:element maxOccurs='unbounded' minOccurs='0' name='codigoUO' type='xs:string'/>
         </xs:sequence>
        </xs:complexType>
       </xs:element>
       <xs:element name='codigosParlamentares'>
        <xs:complexType>
         <xs:sequence>
          <xs:element maxOccurs='unbounded' minOccurs='0' name='codigoParlamentar' type='xs:int'/>
         </xs:sequence>
        </xs:complexType>
       </xs:element>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='obterEmendasAprovadasResponse'>
    <xs:sequence>
     <xs:element minOccurs='0' name='return' type='tns:retornoEmendaAprovadaDTO'/>
    </xs:sequence>
   </xs:complexType>
   <xs:complexType name='retornoEmendaAprovadaDTO'>
    <xs:complexContent>
     <xs:extension base='tns:retornoDTO'>
      <xs:sequence>
       <xs:element name='emendasAprovadas'>
        <xs:complexType>
         <xs:sequence>
          <xs:element maxOccurs='unbounded' minOccurs='0' name='emendaAprovada' type='tns:emendaAprovadaDTO'/>
         </xs:sequence>
        </xs:complexType>
       </xs:element>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='emendaAprovadaDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='identificadorUnicoLocalizador' type='xs:int'/>
       <xs:element minOccurs='0' name='esfera' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoUO' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoPrograma' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoFuncao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoSubFuncao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoAcao' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoLocalizador' type='xs:string'/>
       <xs:element minOccurs='0' name='naturezaDespesa' type='xs:string'/>
       <xs:element minOccurs='0' name='resultadoPrimario' type='xs:string'/>
       <xs:element minOccurs='0' name='fonte' type='xs:string'/>
       <xs:element minOccurs='0' name='idUso' type='xs:string'/>
       <xs:element minOccurs='0' name='codigoParlamentar' type='xs:int'/>
       <xs:element minOccurs='0' name='nomeParlamentar' type='xs:string'/>
       <xs:element minOccurs='0' name='numeroEmenda' type='xs:int'/>
       <xs:element minOccurs='0' name='codigoPartido' type='xs:string'/>
       <xs:element minOccurs='0' name='siglaPartido' type='xs:string'/>
       <xs:element minOccurs='0' name='ufParlamentar' type='xs:string'/>
       <xs:element minOccurs='0' name='valorAtual' type='xs:long'/>
       <xs:element name='beneficiariosEmenda'>
        <xs:complexType>
         <xs:sequence>
          <xs:element maxOccurs='unbounded' minOccurs='0' name='beneficiarioEmenda' type='tns:beneficiarioDTO'/>
         </xs:sequence>
        </xs:complexType>
       </xs:element>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='beneficiarioDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='CNPJBeneficiario' type='xs:string'/>
       <xs:element minOccurs='0' name='nomeBeneficiario' type='xs:string'/>
       <xs:element minOccurs='0' name='valorRevisadoBeneficiario' type='xs:long'/>
       <xs:element name='objetosBeneficiarioEmenda'>
        <xs:complexType>
         <xs:sequence>
          <xs:element maxOccurs='unbounded' minOccurs='0' name='objetoBeneficiarioEmenda' type='tns:objetoBeneficiarioDTO'/>
         </xs:sequence>
        </xs:complexType>
       </xs:element>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
   <xs:complexType name='objetoBeneficiarioDTO'>
    <xs:complexContent>
     <xs:extension base='tns:baseDTO'>
      <xs:sequence>
       <xs:element minOccurs='0' name='descricaoObjeto' type='xs:string'/>
       <xs:element minOccurs='0' name='valorObjeto' type='xs:long'/>
      </xs:sequence>
     </xs:extension>
    </xs:complexContent>
   </xs:complexType>
  </xs:schema>
 </types>
 <message name='WSAlteracoesOrcamentarias_verificarPedidoAlteracaoResponse'>
  <part element='tns:verificarPedidoAlteracaoResponse' name='verificarPedidoAlteracaoResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterAnalisesEmendasResponse'>
  <part element='tns:obterAnalisesEmendasResponse' name='obterAnalisesEmendasResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_cadastrarPedidoAlteracaoResponse'>
  <part element='tns:cadastrarPedidoAlteracaoResponse' name='cadastrarPedidoAlteracaoResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_consultarSituacaoTransmissaoSiafiResponse'>
  <part element='tns:consultarSituacaoTransmissaoSiafiResponse' name='consultarSituacaoTransmissaoSiafiResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_cadastrarAnalisesEmendas'>
  <part element='tns:cadastrarAnalisesEmendas' name='cadastrarAnalisesEmendas'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterTabelasApoioAlteracoesOrcamentariasResponse'>
  <part element='tns:obterTabelasApoioAlteracoesOrcamentariasResponse' name='obterTabelasApoioAlteracoesOrcamentariasResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterPerguntasJustificativa'>
  <part element='tns:obterPerguntasJustificativa' name='obterPerguntasJustificativa'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_enviarPedidoAlteracao'>
  <part element='tns:enviarPedidoAlteracao' name='enviarPedidoAlteracao'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterPedidosAlteracaoPorDescricaoResponse'>
  <part element='tns:obterPedidosAlteracaoPorDescricaoResponse' name='obterPedidosAlteracaoPorDescricaoResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_cadastrarAnalisesEmendasResponse'>
  <part element='tns:cadastrarAnalisesEmendasResponse' name='cadastrarAnalisesEmendasResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_excluirPedidoAlteracao'>
  <part element='tns:excluirPedidoAlteracao' name='excluirPedidoAlteracao'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterEmendasAprovadas'>
  <part element='tns:obterEmendasAprovadas' name='obterEmendasAprovadas'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterPedidoAlteracaoResponse'>
  <part element='tns:obterPedidoAlteracaoResponse' name='obterPedidoAlteracaoResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterPerguntaJustificativa'>
  <part element='tns:obterPerguntaJustificativa' name='obterPerguntaJustificativa'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_cadastrarPedidoPACResponse'>
  <part element='tns:cadastrarPedidoPACResponse' name='cadastrarPedidoPACResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_cadastrarPedidoAlteracaoRemanejamentoEmendas'>
  <part element='tns:cadastrarPedidoAlteracaoRemanejamentoEmendas' name='cadastrarPedidoAlteracaoRemanejamentoEmendas'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterSaldosAcoesPAC'>
  <part element='tns:obterSaldosAcoesPAC' name='obterSaldosAcoesPAC'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_enviarPedidoAlteracaoResponse'>
  <part element='tns:enviarPedidoAlteracaoResponse' name='enviarPedidoAlteracaoResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_consultarSituacaoTransmissaoSiafi'>
  <part element='tns:consultarSituacaoTransmissaoSiafi' name='consultarSituacaoTransmissaoSiafi'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterEmendasAprovadasResponse'>
  <part element='tns:obterEmendasAprovadasResponse' name='obterEmendasAprovadasResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterPedidosAlteracaoPorDescricao'>
  <part element='tns:obterPedidosAlteracaoPorDescricao' name='obterPedidosAlteracaoPorDescricao'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterPerguntaJustificativaResponse'>
  <part element='tns:obterPerguntaJustificativaResponse' name='obterPerguntaJustificativaResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_excluirPedidoAlteracaoResponse'>
  <part element='tns:excluirPedidoAlteracaoResponse' name='excluirPedidoAlteracaoResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterPedidosAlteracao'>
  <part element='tns:obterPedidosAlteracao' name='obterPedidosAlteracao'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterPerguntasJustificativaResponse'>
  <part element='tns:obterPerguntasJustificativaResponse' name='obterPerguntasJustificativaResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterPedidoAlteracao'>
  <part element='tns:obterPedidoAlteracao' name='obterPedidoAlteracao'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_cadastrarPedidoAlteracaoRemanejamentoEmendasResponse'>
  <part element='tns:cadastrarPedidoAlteracaoRemanejamentoEmendasResponse' name='cadastrarPedidoAlteracaoRemanejamentoEmendasResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterPedidosAlteracaoResponse'>
  <part element='tns:obterPedidosAlteracaoResponse' name='obterPedidosAlteracaoResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterTabelasApoioAlteracoesOrcamentarias'>
  <part element='tns:obterTabelasApoioAlteracoesOrcamentarias' name='obterTabelasApoioAlteracoesOrcamentarias'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterSaldosAcoesPACResponse'>
  <part element='tns:obterSaldosAcoesPACResponse' name='obterSaldosAcoesPACResponse'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_cadastrarPedidoPAC'>
  <part element='tns:cadastrarPedidoPAC' name='cadastrarPedidoPAC'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_cadastrarPedidoAlteracao'>
  <part element='tns:cadastrarPedidoAlteracao' name='cadastrarPedidoAlteracao'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_obterAnalisesEmendas'>
  <part element='tns:obterAnalisesEmendas' name='obterAnalisesEmendas'></part>
 </message>
 <message name='WSAlteracoesOrcamentarias_verificarPedidoAlteracao'>
  <part element='tns:verificarPedidoAlteracao' name='verificarPedidoAlteracao'></part>
 </message>
 <portType name='WSAlteracoesOrcamentarias'>
  <operation name='cadastrarAnalisesEmendas' parameterOrder='cadastrarAnalisesEmendas'>
   <input message='tns:WSAlteracoesOrcamentarias_cadastrarAnalisesEmendas'></input>
   <output message='tns:WSAlteracoesOrcamentarias_cadastrarAnalisesEmendasResponse'></output>
  </operation>
  <operation name='cadastrarPedidoAlteracao' parameterOrder='cadastrarPedidoAlteracao'>
   <input message='tns:WSAlteracoesOrcamentarias_cadastrarPedidoAlteracao'></input>
   <output message='tns:WSAlteracoesOrcamentarias_cadastrarPedidoAlteracaoResponse'></output>
  </operation>
  <operation name='cadastrarPedidoAlteracaoRemanejamentoEmendas' parameterOrder='cadastrarPedidoAlteracaoRemanejamentoEmendas'>
   <input message='tns:WSAlteracoesOrcamentarias_cadastrarPedidoAlteracaoRemanejamentoEmendas'></input>
   <output message='tns:WSAlteracoesOrcamentarias_cadastrarPedidoAlteracaoRemanejamentoEmendasResponse'></output>
  </operation>
  <operation name='cadastrarPedidoPAC' parameterOrder='cadastrarPedidoPAC'>
   <input message='tns:WSAlteracoesOrcamentarias_cadastrarPedidoPAC'></input>
   <output message='tns:WSAlteracoesOrcamentarias_cadastrarPedidoPACResponse'></output>
  </operation>
  <operation name='consultarSituacaoTransmissaoSiafi' parameterOrder='consultarSituacaoTransmissaoSiafi'>
   <input message='tns:WSAlteracoesOrcamentarias_consultarSituacaoTransmissaoSiafi'></input>
   <output message='tns:WSAlteracoesOrcamentarias_consultarSituacaoTransmissaoSiafiResponse'></output>
  </operation>
  <operation name='enviarPedidoAlteracao' parameterOrder='enviarPedidoAlteracao'>
   <input message='tns:WSAlteracoesOrcamentarias_enviarPedidoAlteracao'></input>
   <output message='tns:WSAlteracoesOrcamentarias_enviarPedidoAlteracaoResponse'></output>
  </operation>
  <operation name='excluirPedidoAlteracao' parameterOrder='excluirPedidoAlteracao'>
   <input message='tns:WSAlteracoesOrcamentarias_excluirPedidoAlteracao'></input>
   <output message='tns:WSAlteracoesOrcamentarias_excluirPedidoAlteracaoResponse'></output>
  </operation>
  <operation name='obterAnalisesEmendas' parameterOrder='obterAnalisesEmendas'>
   <input message='tns:WSAlteracoesOrcamentarias_obterAnalisesEmendas'></input>
   <output message='tns:WSAlteracoesOrcamentarias_obterAnalisesEmendasResponse'></output>
  </operation>
  <operation name='obterEmendasAprovadas' parameterOrder='obterEmendasAprovadas'>
   <input message='tns:WSAlteracoesOrcamentarias_obterEmendasAprovadas'></input>
   <output message='tns:WSAlteracoesOrcamentarias_obterEmendasAprovadasResponse'></output>
  </operation>
  <operation name='obterPedidoAlteracao' parameterOrder='obterPedidoAlteracao'>
   <input message='tns:WSAlteracoesOrcamentarias_obterPedidoAlteracao'></input>
   <output message='tns:WSAlteracoesOrcamentarias_obterPedidoAlteracaoResponse'></output>
  </operation>
  <operation name='obterPedidosAlteracao' parameterOrder='obterPedidosAlteracao'>
   <input message='tns:WSAlteracoesOrcamentarias_obterPedidosAlteracao'></input>
   <output message='tns:WSAlteracoesOrcamentarias_obterPedidosAlteracaoResponse'></output>
  </operation>
  <operation name='obterPedidosAlteracaoPorDescricao' parameterOrder='obterPedidosAlteracaoPorDescricao'>
   <input message='tns:WSAlteracoesOrcamentarias_obterPedidosAlteracaoPorDescricao'></input>
   <output message='tns:WSAlteracoesOrcamentarias_obterPedidosAlteracaoPorDescricaoResponse'></output>
  </operation>
  <operation name='obterPerguntaJustificativa' parameterOrder='obterPerguntaJustificativa'>
   <input message='tns:WSAlteracoesOrcamentarias_obterPerguntaJustificativa'></input>
   <output message='tns:WSAlteracoesOrcamentarias_obterPerguntaJustificativaResponse'></output>
  </operation>
  <operation name='obterPerguntasJustificativa' parameterOrder='obterPerguntasJustificativa'>
   <input message='tns:WSAlteracoesOrcamentarias_obterPerguntasJustificativa'></input>
   <output message='tns:WSAlteracoesOrcamentarias_obterPerguntasJustificativaResponse'></output>
  </operation>
  <operation name='obterSaldosAcoesPAC' parameterOrder='obterSaldosAcoesPAC'>
   <input message='tns:WSAlteracoesOrcamentarias_obterSaldosAcoesPAC'></input>
   <output message='tns:WSAlteracoesOrcamentarias_obterSaldosAcoesPACResponse'></output>
  </operation>
  <operation name='obterTabelasApoioAlteracoesOrcamentarias' parameterOrder='obterTabelasApoioAlteracoesOrcamentarias'>
   <input message='tns:WSAlteracoesOrcamentarias_obterTabelasApoioAlteracoesOrcamentarias'></input>
   <output message='tns:WSAlteracoesOrcamentarias_obterTabelasApoioAlteracoesOrcamentariasResponse'></output>
  </operation>
  <operation name='verificarPedidoAlteracao' parameterOrder='verificarPedidoAlteracao'>
   <input message='tns:WSAlteracoesOrcamentarias_verificarPedidoAlteracao'></input>
   <output message='tns:WSAlteracoesOrcamentarias_verificarPedidoAlteracaoResponse'></output>
  </operation>
 </portType>
 <binding name='WSAlteracoesOrcamentariasBinding' type='tns:WSAlteracoesOrcamentarias'>
  <soap:binding style='document' transport='http://schemas.xmlsoap.org/soap/http'/>
  <operation name='cadastrarAnalisesEmendas'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='cadastrarPedidoAlteracao'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='cadastrarPedidoAlteracaoRemanejamentoEmendas'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='cadastrarPedidoPAC'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='consultarSituacaoTransmissaoSiafi'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='enviarPedidoAlteracao'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='excluirPedidoAlteracao'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='obterAnalisesEmendas'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='obterEmendasAprovadas'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='obterPedidoAlteracao'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='obterPedidosAlteracao'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='obterPedidosAlteracaoPorDescricao'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='obterPerguntaJustificativa'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='obterPerguntasJustificativa'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='obterSaldosAcoesPAC'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='obterTabelasApoioAlteracoesOrcamentarias'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
  <operation name='verificarPedidoAlteracao'>
   <soap:operation soapAction=''/>
   <input>
    <soap:body use='literal'/>
   </input>
   <output>
    <soap:body use='literal'/>
   </output>
  </operation>
 </binding>
 <service name='WSAlteracoesOrcamentarias'>
  <port binding='tns:WSAlteracoesOrcamentariasBinding' name='WSAlteracoesOrcamentariasPort'>
   <soap:address location='https://testews.siop.gov.br:443/services/WSAlteracoesOrcamentarias'/>
  </port>
 </service>
</definitions>
XML;

//ini_set('soap.wsdl_cache_enabled', 0);


$xmlDom = $str; //execute($wsdl, $caminho_certificado, $senha_certificado);

if( $_REQUEST['requisicao'] == 'visualizar' ){
	ver( simec_htmlentities( $xmlDom ),d);
}
ini_set('soap.wsdl_cache_enabled', 0);

print "Analyzing WSDL";

try {
	if( $caminho_certificado && $senha_certificado ){	
		  $context = stream_context_create(array(
											    'ssl' => array(
											    'verify_peer' => false,
											    'allow_self_signed' => true,
											    'cafile' => $caminho_certificado,
											    'local_cert' => $caminho_certificado,
											    'passphrase ' 	=> $senha_certificado	
											    )
											)); 
	}

	$client = new SoapClient($wsdl, array(
									'exceptions'	=> true,
							        'trace'			=> true,
									'encoding'		=> 'ISO-8859-1',
									'stream_context' => $context
									)
	);
  //$operations = $client->__getFunctions();
  //ver($operations,d);
} catch(SoapFault $e) {
	die($e);
}

print ".";
//ver(simec_htmlentities($xmlDom));
$dom = DOMDocument::loadXML($xmlDom);

print ".";
// get documentation
$nodes = $dom->getElementsByTagName('documentation');
$doc = array('service' => '',
	     'operations' => array());
foreach($nodes as $node) {
  if( $node->parentNode->localName == 'service' ) {
    $doc['service'] = trim($node->parentNode->nodeValue);
  } else if( $node->parentNode->localName == 'operation' ) {
    $operation = $node->parentNode->getAttribute('name');
    //$parameterOrder = $node->parentNode->getAttribute('parameterOrder');
    $doc['operations'][$operation] = trim($node->nodeValue);
  }
}
print ".";

// get targetNamespace
$targetNamespace = '';
$nodes = $dom->getElementsByTagName('definitions');
foreach($nodes as $node) {
  $targetNamespace = $node->getAttribute('targetNamespace');
}
print ".";

// declare service
$service = array('class' => 'SimecWsFacadeService',
		 'wsdl' => $wsdl,
		 'doc' => $doc['service'],
		 'functions' => array());
print ".";

// PHP keywords - can not be used as constants, class names or function names!
$reserved_keywords = array('and', 'or', 'xor', 'as', 'break', 'case', 'cfunction', 'class', 'continue', 'declare', 'const', 'default', 'do', 'else', 'elseif', 'enddeclare', 'endfor', 'endforeach', 'endif', 'endswitch', 'endwhile', 'eval', 'extends', 'for', 'foreach', 'function', 'global', 'if', 'new', 'old_function', 'static', 'switch', 'use', 'var', 'while', 'array', 'die', 'echo', 'empty', 'exit', 'include', 'include_once', 'isset', 'list', 'print', 'require', 'require_once', 'return', 'unset', '__file__', '__line__', '__function__', '__class__', 'abstract', 'private', 'public', 'protected', 'throw', 'try');

// ensure legal class name (I don't think using . and whitespaces is allowed in terms of the SOAP standard, should check this out and may throw and exception instead...)
$service['class'] = str_replace(' ', '_', $service['class']);
$service['class'] = str_replace('.', '_', $service['class']);
$service['class'] = str_replace('-', '_', $service['class']);

if(in_array(strtolower($service['class']), $reserved_keywords)) {
  $service['class'] .= 'Service';
}

// verify that the name of the service is named as a defined class
if(class_exists($service['class'])) {
  throw new Exception("Class '".$service['class']."' already exists");
}

/*if(function_exists($service['class'])) {
  throw new Exception("Class '".$service['class']."' can't be used, a function with that name already exists");
}*/

// get operations
$operations = $client->__getFunctions();
foreach($operations as $operation) {

  /*
   This is broken, need to handle
   GetAllByBGName_Response_t GetAllByBGName(string $Name)
   list(int $pcode, string $city, string $area, string $adm_center) GetByBGName(string $Name)

   finding the last '(' should be ok
   */
  //list($call, $params) = explode('(', $operation); // broken
  
  //if($call == 'list') { // a list is returned
  //}
  
  /*$call = array();
  preg_match('/^(list\(.*\)) (.*)\((.*)\)$/', $operation, $call);
  if(sizeof($call) == 3) { // found list()
    
  } else {
    preg_match('/^(.*) (.*)\((.*)\)$/', $operation, $call);
    if(sizeof($call) == 3) {
      
    }
  }*/

  $matches = array();
  if(preg_match('/^(\w[\w\d_]*) (\w[\w\d_]*)\(([\w\$\d,_ ]*)\)$/', $operation, $matches)) {
    $returns = $matches[1];
    $call = $matches[2];
    $params = $matches[3];
  } else if(preg_match('/^(list\([\w\$\d,_ ]*\)) (\w[\w\d_]*)\(([\w\$\d,_ ]*)\)$/', $operation, $matches)) {
    $returns = $matches[1];
    $call = $matches[2];
    $params = $matches[3];
  } else { // invalid function call
    throw new Exception('Invalid function call: '.$function);
  }

  $params = explode(', ', $params);

  $paramsArr = array();
  foreach($params as $param) {
    $paramsArr[] = explode(' ', $param);
  }
  //  $call = explode(' ', $call);
  $function = array('name' => $call,
		    'method' => $call,
		    'return' => $returns,
		    'doc' => isset($doc['operations'][$call])?$doc['operations'][$call]:'',
		    'params' => $paramsArr);

  // ensure legal function name
  if(in_array(strtolower($function['method']), $reserved_keywords)) {
    $function['name'] = '_'.$function['method'];
  }

  // ensure that the method we are adding has not the same name as the constructor
  if(strtolower($service['class']) == strtolower($function['method'])) {
    $function['name'] = '_'.$function['method'];
  }

  // ensure that there's no method that already exists with this name
  // this is most likely a Soap vs HttpGet vs HttpPost problem in WSDL
  // I assume for now that Soap is the one listed first and just skip the rest
  // this should be improved by actually verifying that it's a Soap operation that's in the WSDL file
  // QUICK FIX: just skip function if it already exists
  $add = true;
  foreach($service['functions'] as $func) {
    if($func['name'] == $function['name']) {
      $add = false;
    }
  }
  if($add) {
    $service['functions'][] = $function;
  }
  print ".";
}

$types = $client->__getTypes();

$primitive_types = array('string', 'int', 'long', 'float', 'boolean', 'dateTime', 'double', 'short', 'UNKNOWN', 'base64Binary', 'decimal', 'ArrayOfInt', 'ArrayOfFloat', 'ArrayOfString', 'decimal', 'hexBinary'); // TODO: dateTime is special, maybe use PEAR::Date or similar
$service['types'] = array();
foreach($types as $type) {
  $parts = explode("\n", $type);
  $class = explode(" ", $parts[0]);
  $class = $class[1];
  
  if( substr($class, -2, 2) == '[]' ) { // array skipping
    continue;
  }

  if( substr($class, 0, 7) == 'ArrayOf' ) { // skip 'ArrayOf*' types (from MS.NET, Axis etc.)
    continue;
  }


  $members = array();
  for($i=1; $i<count($parts)-1; $i++) {
    $parts[$i] = trim($parts[$i]);
    list($type, $member) = explode(" ", substr($parts[$i], 0, strlen($parts[$i])-1) );

    // check syntax
    if(preg_match('/^$\w[\w\d_]*$/', $member)) {
      throw new Exception('illegal syntax for member variable: '.$member);
      continue;
    }

    // IMPORTANT: Need to filter out namespace on member if presented
    if(strpos($member, ':')) { // keep the last part
      list($tmp, $member) = explode(':', $member);
    }

    // OBS: Skip member if already presented (this shouldn't happen, but I've actually seen it in a WSDL-file)
    // "It's better to be safe than sorry" (ref Morten Harket) 
    $add = true;
    foreach($members as $mem) {
      if($mem['member'] == $member) {
	$add = false;
      }
    }
    if($add) {
      $members[] = array('member' => $member, 'type' => $type);
    }
  }

  // gather enumeration values
  $values = array();
  if(count($members) == 0) {
    $values = checkForEnum($dom, $class);
  }

  $service['types'][] = array('class' => $class, 'members' => $members, 'values' => $values);
  print ".";
}
print "done\n";

print "Generating code...";
$code = "";

// add types
foreach($service['types'] as $type) {
  //  $code .= "/**\n";
  //  $code .= " * ".(isset($type['doc'])?$type['doc']:'')."\n";
  //  $code .= " * \n";
  //  $code .= " * @package\n";
  //  $code .= " * @copyright\n";
  //  $code .= " */\n";

  // add enumeration values
  $code .= "class ".$type['class']." {\n";
  foreach($type['values'] as $value) {
    $code .= "  const ".generatePHPSymbol($value)." = '$value';\n";
  }
  
  // add member variables
  foreach($type['members'] as $member) {
    //$code .= "  /* ".$member['type']." */\n";
    $code .= "  public \$".$member['member']."; // ".$member['type']."\n";
  }
  $code .= "}\n\n";

  /*  print "Writing ".$type['class'].".php...";
  $filename = $type['class'].".php";
  $fp = fopen($filename, 'w');
  fwrite($fp, "<?php\n".$code."?>\n");
  fclose($fp);
  print "ok\n";*/
}

// add service

// page level docblock
//$code .= "/**\n";
//$code .= " * ".$service['class']." class file\n";
//$code .= " * \n";
//$code .= " * @author    {author}\n";
//$code .= " * @copyright {copyright}\n";
//$code .= " * @package   {package}\n";
//$code .= " */\n\n";


// require types
//foreach($service['types'] as $type) {
//  $code .= "/**\n";
//  $code .= " * ".$type['class']." class\n";
//  $code .= " */\n";
//  $code .= "require_once '".$type['class'].".php';\n";
//}

$code .= "\n";

// class level docblock
$code .= "/**\n";
$code .= " * ".$service['class']." class\n";
$code .= " * \n";
$code .= parse_doc(" * ", $service['doc']);
$code .= " * \n";
$code .= " * @author    {author}\n";
$code .= " * @copyright {copyright}\n";
$code .= " * @package   {package}\n";
$code .= " */\n";
$code .= "class ".$service['class']." extends SoapClient {\n\n";

// add classmap
$code .= "  private static \$classmap = array(\n";
foreach($service['types'] as $type) {
  $code .= "                                    '".$type['class']."' => '".$type['class']."',\n";
}
$code .= "                                   );\n\n";
$code .= "  public function ".$service['class']."(\$wsdl = \"".$service['wsdl']."\", \$options = array()) {\n";

// initialize classmap (merge)
$code .= "    foreach(self::\$classmap as \$key => \$value) {\n";
$code .= "      if(!isset(\$options['classmap'][\$key])) {\n";
$code .= "        \$options['classmap'][\$key] = \$value;\n";
$code .= "      }\n";
$code .= "    }\n";
$code .= "    parent::__construct(\$wsdl, \$options);\n";
$code .= "  }\n\n";

foreach($service['functions'] as $function) {
  $code .= "  /**\n";
  $code .= parse_doc("   * ", $function['doc']);
  $code .= "   *\n";

  $signature = array(); // used for function signature
  $para = array(); // just variable names
  if(count($function['params']) > 0) {
    foreach($function['params'] as $param) {
      $code .= "   * @param ".(isset($param[0])?$param[0]:'')." ".(isset($param[1])?$param[1]:'')."\n";
      /*$typehint = false;
      foreach($service['types'] as $type) {
	if($type['class'] == $param[0]) {
	  $typehint = true;
	}
      }
      $signature[] = ($typehint) ? implode(' ', $param) : $param[1];*/
      $signature[] = (in_array($param[0], $primitive_types) or substr($param[0], 0, 7) == 'ArrayOf') ? $param[1] : implode(' ', $param);
      $para[] = $param[1];
    }
  }
  $code .= "   * @return ".$function['return']."\n";
  $code .= "   */\n";
  $code .= "  public function ".$function['name']."(".implode(', ', $signature).") {\n";
  //  $code .= "    return \$this->client->".$function['name']."(".implode(', ', $para).");\n";
  $code .= "    return \$this->__soapCall('".$function['method']."', array(";
  $params = array();
  if(count($signature) > 0) { // add arguments
    foreach($signature as $param) {
      if(strpos($param, ' ')) { // slice 
	$param = array_pop(explode(' ', $param));
      }
      $params[] = $param;
    }
    //$code .= "\n      ";
    $code .= implode(", ", $params);
    //$code .= "\n      ),\n";
  }
  $code .= "), ";
  //$code .= implode(', ', $signature)."),\n";
  $code .= "      array(\n";
  $code .= "            'uri' => '".$targetNamespace."',\n";
  $code .= "            'soapaction' => ''\n";
  $code .= "           )\n";
  $code .= "      );\n";
  $code .= "  }\n\n";
}
$code .= "}\n\n";
print "done\n";

//print "Writing ".$service['class'].".php...";
//$fp = fopen(APPRAIZ."/".$service['class'].".php", 'w');
//fwrite($fp, "<?php\n".$code."\n");
//fclose($fp);
echo "<pre>";
echo $code;
print "done\n";
}

function parse_doc($prefix, $doc) {
  $code = "";
  $words = @split(' ', $doc);
  $line = $prefix;
  foreach($words as $word) {
    $line .= $word.' ';
    if( strlen($line) > 90 ) { // new line
      $code .= $line."\n";
      $line = $prefix;
    }
  }
  $code .= $line."\n";
  return $code;
}

/**
 * Look for enumeration
 * 
 * @param DOM $dom
 * @param string $class
 * @return array
 */
function checkForEnum(&$dom, $class) {
  $values = array();
  
  $node = findType($dom, $class);
  if(!$node) {
    return $values;
  }
  
  $value_list = $node->getElementsByTagName('enumeration');
  if($value_list->length == 0) {
    return $values;
  }

  for($i=0; $i<$value_list->length; $i++) {
    $values[] = $value_list->item($i)->attributes->getNamedItem('value')->nodeValue;
  }
  return $values;
}

/**
 * Look for a type
 * 
 * @param DOM $dom
 * @param string $class
 * @return DOMNode
 */
function findType(&$dom, $class) {
  $types_node  = $dom->getElementsByTagName('types')->item(0);
  $schema_list = $types_node->getElementsByTagName('schema');
  
  for ($i=0; $i<$schema_list->length; $i++) {
    $children = $schema_list->item($i)->childNodes;
    for ($j=0; $j<$children->length; $j++) {
      $node = $children->item($j);
      if ($node instanceof DOMElement &&
	  $node->hasAttributes() &&
	  $node->attributes->getNamedItem('name')->nodeValue == $class) {
	return $node;
      }
    }
  }
  return null;
}

function generatePHPSymbol($s) {
  global $reserved_keywords;
  
  if(!preg_match('/^[A-Za-z_]/', $s)) {
    $s = 'value_'.$s;
  }
  if(in_array(strtolower($s), $reserved_keywords)) {
    $s = '_'.$s;
  }
  return preg_replace('/[-.\s]/', '_', $s);
}

function execute($url, $caminho_certificado, $senha_certificado) {
	echo $url;

	// Initialize session and set URL.
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL, $url);

	// Desabilita a verificação da CA do servidor
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);

	// Set so curl_exec returns the result instead of outputting it.
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	//curl_setopt($ch, CURLOPT_CERTINFO, false);

	//seta o caminho do arquivo do chave privada
	//		curl_setopt($ch, CURLOPT_SSLKEY, $caminho_certificado);
	//		curl_setopt($ch, CURLOPT_SSH_PRIVATE_KEYFILE, $caminho_certificado);
	curl_setopt($ch, CURLOPT_SSLCERT, $caminho_certificado);
	//		curl_setopt($ch, CURLOPT_KEYPASSWD, $senha_certificado);
	//		curl_setopt($ch, CURLOPT_SSLKEYPASSWD, $senha_certificado);
	curl_setopt($ch, CURLOPT_SSLCERTPASSWD, $senha_certificado);

	// Get the response and close the channel.
	$response = curl_exec($ch);
	//ver(simec_htmlentities($response),d);
	curl_close($ch);

	return $response;

}

?>
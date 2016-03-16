<?php

function imprimir($var, $exit = false) {
	echo "<pre>";
	print_r($var);
	echo "</pre>";
	if($exit) exit();
}
if($_POST['requisicao'] == 'enviasolicitacao') {
	
	
	
	
	//Caminho do arquivo .pem
	//Comando linux para extrair a informação pem do .p12:
	//		openssl pkcs12 -in mec.p12 -out mec.pem -nodes -clcerts
	//$certificado = dirname(__FILE__)."\mec.pem";
	
	
	
	include(APPRAIZ."monitora/classes/ImportacaoSiop.class.inc");
	//include(APPRAIZ."monitora/classes/WSQuantitativo.class.inc");
	
	//Endereço do wsdl do serviço
	$wsdl = "https://homologacao.siop.planejamento.gov.br/services/WSQuantitativo?wsdl";
	$certificado =  APPRAIZ. "monitora/modulos/sistema/comunica/mec.pem";
	//Senha do certificado
	$senha_certificado = "mec2011";
	
	$obSiop = new ImportacaoSiop($wsdl, $certificado, $senha_certificado);
	$teste = $obSiop->conectarWS( $_POST );
	
	ver($teste,d);
	
	$wsQuantitativo = new WSQuantitativo($wsdl, array(
			'local_cert'	=> $certificado, 
			'passphrase ' 	=> $senha_certificado,
			'exceptions'	=> true,
	        'trace'			=> true,
			'encoding'		=> 'ISO-8859-1' ));
	
	if($_POST['documento'] == 'obterPedidoAlteracao') {
		
		
	} else if($_POST['documento'] == 'obterProgramacaoCompleta') {
		
		$obterProgramacaoCompletaQuantitativo = new obterProgramacaoCompletaQuantitativo();

		//monta a credencial
		$credencialDTO = new credencialDTO();		
		$credencialDTO->perfil = 32;
		$credencialDTO->usuario = $_POST['wsusuario'];
		$credencialDTO->senha = md5($_POST['wssenha']);
		$obterProgramacaoCompletaQuantitativo->credencialDTO = $credencialDTO;
		
		//atribui parâmetros
		$obterProgramacaoCompletaQuantitativo->codigoMomento = 9000;
		$obterProgramacaoCompletaQuantitativo->exercicio = 2012;
		
		$obterProgramacaoCompletaQuantitativoResponse = $wsQuantitativo->obterProgramacaoCompletaQuantitativo($obterProgramacaoCompletaQuantitativo);
		
		$request = $wsQuantitativo->__getLastRequest();
		$response = $wsQuantitativo->__getLastResponse();
		ver($obterProgramacaoCompletaQuantitativoResponse, htmlentities(utf8_decode($response)),d);
	} else {
		$operacoes = $wsQuantitativo->__getFunctions();
		$tipos = $wsQuantitativo->__getTypes();
	} 
	exit();
}

// cabecalho padrão do SIMEC
include APPRAIZ . "includes/cabecalho.inc";
monta_titulo('Comunicação de Dados - SIOP', '');

$wsusuario = 'wsmec';
$wssenha = 'Ch0c014t3';

$largura 	= "250px";
$altura 	= "120px";
$id 		= "div_auth";
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<style>	
	.popup_alerta
	{
		width:<?php echo $largura ?>;
		height:<?php echo $altura ?>;
		position:absolute;
		z-index:0;
		top:50%;
		left:50%;
		margin-top:-<?php echo $altura/2 ?>;
		margin-left:-<?php echo $largura/2 ?>;
		border:solid 2px black;
		background-color:#FFFFFF;
		display:none
	}
</style>
<style>
	label { cursor: pointer; }
</style>
<form method="post" name="formulario" id="formulario">
	
<table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
	<tr>
		<td class="subtitulodireita">Funções:</td>
		<td style="padding: 0 20px 20px 20px;">
			<label>
				<input type="radio" name="documento" value="cadastrarProposta" checked/>
				Cadastrar Proposta
			</label><br/>
			<label>
				<input type="radio" name="documento" value="consultarProposta" checked/>
				Consultar Proposta
			</label><br/>
			<label>
				<input type="radio" name="documento" value="excluirProposta" checked/>
				Excluir Proposta
			</label><br/>
			<label>
				<input type="radio" name="documento" value="obterPedidoAlteracao" checked/>
				Obter Pedido Alteracao
			</label><br/>
			<label>
				<input type="radio" name="documento" value="obterProgramacaoCompletaQuantitativo"/>
				Obter Programação Completa
			</label><br/>
			<label>
				<input type="radio" name="documento" value="obterTabelasApoioQuantitativo"/>
				Obter Tabelas Apoio
			</label><br/>
			&nbsp;<br/>
			<!--  <label>
				<input id="todos" type="checkbox" name="" value="acao" onclick="selecionarTodos( this );"/>
				<b>Todos</b>
			</label>-->
		</td>
	</tr>
	<tr>
		<td colspan="2" align="center" style="background-color:#c0c0c0;"><input type="button" class="botao" value="Enviar Solicitação" onclick="solicitarExecucao();"></td>
	</tr>
</table>
<script type="text/javascript">
function selecionarTodos( todos ){
		document.getElementById( 'acao' ).checked = todos.checked;
		document.getElementById( 'programa' ).checked = todos.checked;
		document.getElementById( 'fisico' ).checked = todos.checked;
//		document.getElementById( 'financeiro' ).checked = todos.checked;
		document.getElementById( 'providencia_programa' ).checked = todos.checked;
		document.getElementById( 'providencia_acao' ).checked = todos.checked;
		document.getElementById( 'validacao_acao' ).checked = todos.checked;
	}

	function selecionarOpcao( opcao ){
		if ( !opcao.checked ) {
			document.getElementById( 'todos' ).checked = false;
		} else if(
			document.getElementById( 'obterProgramacaoCompleta' ).checked == true &&
			document.getElementById( 'programa' ).checked == true &&
			document.getElementById( 'fisico' ).checked == true &&
//			document.getElementById( 'financeiro' ).checked == true &&
			document.getElementById( 'providencia_programa' ).checked == true &&
			document.getElementById( 'providencia_acao' ).checked == true &&
			document.getElementById( 'validacao_acao' ).checked == true
		) {
			document.getElementById( 'todos' ).checked = true;
		}
	}

	/*function enviar(){
		divCarregando();
	
		var dados = $('#formulario').serialize();	
		$.ajax({
	   		type: "POST",
	   		url: "monitora.php?modulo=sistema/comunica/importasiop&acao=A",
	   		data: "requisicao=gerararquivo&"+dados,
	   		async: false,
	   		success: function(msg){
	   			document.getElementById('debug').innerHTML = msg;
	   			//alert(msg);
	   			//document.getElementById('<?php echo $id ?>').style.display='none'
	   		}
		});
	
		divCarregado();
		
	}*/
function solicitarExecucao(){
	/*var funcoes = document.getElementById('funcoes').value;
	
	if( funcoes == '' ){
		alert('Selecione uma função');
		return false;
	} else {*/
		document.getElementById('div_auth').style.display='block'
	//}
}
function enviaSolicitacao(){
				
	var form = document.getElementById('formulario');
	
	var usuario = document.getElementById('wsusuario').value;
	var senha = document.getElementById('wssenha').value;
	
	if(!usuario) {
		alert('Favor informar o usuário!');
		return false;
	}
	
	if(!senha) {
		alert('Favor informar a senha!');
		return false;
	}
	

	divCarregando();
	
	var dados = $('#formulario').serialize();

	$.ajax({
   		type: "POST",
   		url: "monitora.php?modulo=sistema/comunica/importasiop&acao=A",
   		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=enviasolicitacao&"+dados,
   		async: false,
   		success: function(msg){
   			document.getElementById('debug').innerHTML = msg;
   			//alert(msg);
   			document.getElementById('<?php echo $id ?>').style.display='none'
   		}
	});

	divCarregado();
	
}

function enviar(){
	document.getElementById('div_auth').style.display='block'
	return false;
	var form = document.getElementById('formulario');
	
	var usuario = document.getElementById('wsusuario').value;
	var senha = document.getElementById('wssenha').value;
	
	if(!usuario) {
		alert('Favor informar o usuário!');
		return false;
	}
	
	if(!senha) {
		alert('Favor informar a senha!');
		return false;
	}
	

	divCarregando();
	
	var dados = $('#formulario').serialize();

	$.ajax({
   		type: "POST",
   		url: "monitora.php?modulo=sistema/comunica/importasiop&acao=A",
   		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=gerararquivo&"+dados,
   		async: false,
   		success: function(msg){
   			document.getElementById('debug').innerHTML = msg;
   			//alert(msg);
   			document.getElementById('<?php echo $id ?>').style.display='none'
   		}
	});

	divCarregado();
	
}
</script>
<div id="debug"></div>

<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php
				$wsusuario = $usuario; 
				echo campo_texto("wsusuario","S","S","Usuário","22","","","","","","","id='wsusuario'", '', 'wsmec') ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="Ch0c014t3" size="23" id="wssenha" name="wssenha">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#D5D5D5" colspan="2">
				<input type="button" name="btn_enviar" onclick="enviaSolicitacao()" value="ok" />
				<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
			</td>
		</tr>
		</table>
	</div>
</div>
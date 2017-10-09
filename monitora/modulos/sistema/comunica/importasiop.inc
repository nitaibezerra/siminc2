<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");
// Pull in the NuSOAP code

/*if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}*/


$wsdl = WEB_SERVICE_SIOP_URL. "WSQuantitativo?wsdl";
$certificado = WEB_SERVICE_SIOP_CERTIFICADO;
$senha = WEB_SERVICE_SIOP_SENHA;
echo "<pre>";
$clienteSoap = new SoapClient($wsdl, array(
		'local_cert'	=> $certificado, 
		'passphrase ' 	=> $senha,
		'exceptions'	=> true,
        'trace'			=> true));

$operacoes = $clienteSoap->__getFunctions();
echo "OPERAÇÕES:<br>";
foreach ($operacoes as $operacao) {
	echo "- $operacao<br>";
}

echo "<br>";

$tipos = $clienteSoap->__getTypes();
echo "TIPOS:<br>";
foreach ($tipos as $tipo) {
	echo "- $tipo<br>";
}
echo "</pre>";
die;

include(APPRAIZ."monitora/classes/ImportacaoSiop.class.inc");
include(APPRAIZ."monitora/classes/array2xml.class.inc");

$obImporta = new ImportacaoSiop();
if($_REQUEST['requisicao'] == 'gerararquivo') {
	$obImporta->getSQLFuncao($_POST);
	//$obImporta->enviaArquivoSIOP($_POST);
	exit;
}

//include APPRAIZ . 'includes/nusoap/lib/nusoap.php';

// Cabecalho padrão do sistema
include APPRAIZ . "includes/cabecalho.inc";

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<?php

/*$usuario = 'lindberg';
$senha = md5('621lindberg');*/

$usuario = WEB_SERVICE_SIOP_USUARIO;
$senha = WEB_SERVICE_SIOP_SENHA;

$largura 	= "250px";
$altura 	= "120px";
$id 		= "div_auth";

?>
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
		<td style="padding: 10px;">
			<p style="width: 50%;">
				O envio de dados para o SIOP deverá ocorrer quantas vezes o Administrador julgar conveniente.
				Ao pressionar o botão "Gerar Arquivo", o sistema irá gerar um conjunto de arquivos no formato XML e disponibilizá-los para o SIOP.
			</p>
		</td>
	</tr>
	<tr>
		<td style="padding: 0 20px 20px 20px;">
			<label>
				<input id="acao" type="checkbox" name="documento[]" value="obterPedidoAlteracao" onchange="selecionarOpcao( this );"/>
				Obter Pedido Alteracao
			</label><br/>
			<label>
				<input id="programa"type="checkbox" name="documento[]" value="programa" onchange="selecionarOpcao( this );"/>
				Programas
			</label><br/>
			<label>
				<input id="fisico" type="checkbox" name="documento[]" value="fisico" onchange="selecionarOpcao( this );"/>
				Dados Físicos
			</label><br/>
			<!--
			<label style="color: #aaaaaa; cursor: default;">
				<input id="financeiro" type="checkbox" name="documento[]" value="financeiro" onchange="selecionarOpcao( this );" disabled="disabled"/>
				Dados Financeiros
			</label><br/>
			-->
			<label>
				<input id="providencia_programa" type="checkbox" name="documento[]" value="providencia_programa" onchange="selecionarOpcao( this );"/>
				Restrições e Providências de Programa
			</label><br/>
			<label >
				<input id="providencia_acao" type="checkbox" name="documento[]" value="providencia_acao" onchange="selecionarOpcao( this );"/>
				Restrições e Providências de Ação
			</label><br/>
			<label>
				<input id="validacao_acao" type="checkbox" name="documento[]" value="validacao_acao" onchange="selecionarOpcao( this );"/>
				Validação de Ação
			</label><br/>
			&nbsp;<br/>
			<label>
				<input id="todos" type="checkbox" name="" value="acao" onclick="selecionarTodos( this );"/>
				<b>Todos</b>
			</label>
			<p><input type="button" class="botao" value="Gerar Arquivos" onclick="enviar();"></p>
		</td>
	</tr>
</table>
<table id="pesquisas" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<thead>
		<tr>
			<td class="textosTabela"></td>
			<td class="camposTabela"></td>
			<td class="textosTabela"></td>
			<td></td>
		</tr>
	</thead>
	<tbody>
	  <tr>
		<td class="SubTituloDireita">Funções:</td>
		<td colspan="3">
		<?php
			$arrFuncoes = array(
								array('codigo' => '01', 'descricao' => 'obterAcoesPorIniciativa'),
								array('codigo' => '02', 'descricao' => 'obterAcoesPorIniciativaResponse'),
								array('codigo' => '03', 'descricao' => 'obterAcoesPorPrograma'),
								array('codigo' => '04', 'descricao' => 'obterAcoesPorProgramaResponse'),
								array('codigo' => '05', 'descricao' => 'obterIndicadoresPorPrograma'),
								array('codigo' => '06', 'descricao' => 'obterIndicadoresPorProgramaResponse'),
								array('codigo' => '07', 'descricao' => 'obterIniciativasPorObjetivo'),
								array('codigo' => '08', 'descricao' => 'obterIniciativasPorObjetivoResponse'),
								array('codigo' => '09', 'descricao' => 'obterLocalizadoresPorAcao'),
								array('codigo' => '10', 'descricao' => 'obterLocalizadoresPorAcaoResponse'),
								array('codigo' => '11', 'descricao' => 'obterMetasPorObjetivo'),
								array('codigo' => '12', 'descricao' => 'obterMetasPorObjetivoResponse'),
								array('codigo' => '13', 'descricao' => 'obterMomentoCarga'),
								array('codigo' => '14', 'descricao' => 'obterMomentoCargaResponse'),
								array('codigo' => '15', 'descricao' => 'obterObjetivosPorPrograma'),
								array('codigo' => '16', 'descricao' => 'obterObjetivosPorProgramaResponse'),
								array('codigo' => '17', 'descricao' => 'obterOrgao'),
								array('codigo' => '18', 'descricao' => 'obterOrgaoPorCodigoSiorg'),
								array('codigo' => '19', 'descricao' => 'obterOrgaoPorCodigoSiorgResponse'),
								array('codigo' => '20', 'descricao' => 'obterOrgaoResponse'),
								array('codigo' => '21', 'descricao' => 'obterProgramacaoCompleta'),
								array('codigo' => '22', 'descricao' => 'obterProgramacaoCompletaResponse'),
								array('codigo' => '23', 'descricao' => 'obterProgramasPorOrgao'),
								array('codigo' => '24', 'descricao' => 'obterProgramasPorOrgaoResponse'),
								array('codigo' => '25', 'descricao' => 'obterRegionalizacoesPorMeta'),
								array('codigo' => '26', 'descricao' => 'obterRegionalizacoesPorMetaResponse'),
								array('codigo' => '27', 'descricao' => 'obterTabelasApoio'),
								array('codigo' => '28', 'descricao' => 'obterTabelasApoioResponse'),
								array('codigo' => '29', 'descricao' => 'obterUnidadesOrcamentariasOrgao'),
								array('codigo' => '30', 'descricao' => 'obterUnidadesOrcamentariasOrgaoResponse'),
								);
			$funcoes = '21';
			$db->monta_combo("funcoes", $arrFuncoes, 'S','-- Selecione uma função --','', '', '',350,'N','funcoes', '', '', 'Funções'); 
		?>
		</td>
	</tr> 
	<tr>
		<td></td>
		<td colspan="3">
			<input type="button" name="solicitar" id="solicitar" value="Solicitar" onclick="solicitarExecucao();" />
		</td>
	</tr> 
	</tbody>
</table>
</form>

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
	var funcoes = document.getElementById('funcoes').value;
	
	if( funcoes == '' ){
		alert('Selecione uma função');
		return false;
	} else {
		document.getElementById('div_auth').style.display='block'
	}
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
				echo campo_texto("wsusuario","S","S","Usuário","22","","","","","","","id='wsusuario'") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?=$senha; ?>" size="23" id="wssenha" name="wssenha">
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
<?

function carregaDadosSolicitacao( $dados ){
	global $db;
	
	//ver( $dados,d );
	
	$arrParams = array();
	$funcao = '';
	
	switch ($dados['funcoes']) {
		case 21:
			$arrParams = obterProgramacaoCompleta( $dados );
			//$funcao = 'obterProgramacaoCompleta';
			$funcao = 'obterPedidoAlteracao';
		break;
		
		default:
			;
		break;
	}
	if( !empty($arrParams) ){
		executaSolicitacao($funcao, $arrParams);
	}
}

function executaSolicitacao($funcao, $param = array() ){
	
	require_once(APPRAIZ . 'www/webservice/painel/nusoap.php');	
	
	#Desenvolvimento
	$urlWS = WEB_SERVICE_SIOP_URL. 'WSAlteracoesOrcamentarias?wsdl';
	
	$endpoint = $urlWS;
	$wsdl = true;
	$proxyhost = false;
	$proxyport = false;
	$proxyusername = false;
	$proxypassword = false;
	$timeout = 0;
	$response_timeout = 30;
	
	// Create the client instance
	$client = new soapcliente($endpoint, $wsdl, $proxyhost, $proxyport, $proxyusername, $proxypassword, $timeout, $response_timeout);
	
	// Check for an error
	$err = $client->getError();
	
	if ($err) {
	    // Display the error
	    echo '<h2>Constructor error</h2><pre>' . $err . '</pre>';
	    // At this point, you know the call that follows will fail
	}
	
	$autenticacao = $client->call($funcao, $param);
	
	// Check for a fault
	if ($client->fault) {
	    echo '<h2>Fault</h2><pre>';
	    print_r($client->fault);
	    echo '</pre>';
	} else {
	    // Check for errors
	    $err = $client->getError();
	    if ($err) {
	        // Display the error
	        echo '<h2>Error</h2><pre>' . $err . '</pre>';
	    } else {
	        // Display the result
	        echo '<h2>Autenticacao</h2><pre>';
	        print_r($autenticacao);
	    echo '</pre>';
	    }
	}
	// Display the request and response
	echo '<h2>Request</h2>';
	echo '<pre>' . simec_htmlspecialchars($client->request, ENT_QUOTES) . '</pre>';
	echo '<h2>Response</h2>';
	echo '<pre>' . simec_htmlspecialchars($client->response, ENT_QUOTES) . '</pre>';
	// Display the debug messages
	echo '<h2>Debug</h2>';
	echo '<pre>' . simec_htmlspecialchars($client->debug_str, ENT_QUOTES) . '</pre>';
	
	echo '----------------------------------------------------------------------';
}

function obterProgramacaoCompleta( $dados ){
	
	$credencialDTO = array("perfil" => 32,
						   "senha" => $dados['wssenha'],
						   "usuario" => $dados['wsusuario']);
	
	/*$arrParams = array(
					'credencial' => $credencialDTO,
					'exercicio' => 2011,
					'codigoMomento' => 9000,
					'retornarOrgaos' => true,
					'retornarProgramas' => false,
					'retornarIndicadores' => false,
					'retornarObjetivos' => false,
					'retornarIniciativas' => false,
					'retornarAcoes' => false,
					'retornarLocalizadores' => false,
					'retornarMetas' => false,
					'retornarRegionalizacoes' => false,
					'dataHoraReferencia' => date("c"),
					);*/
	$arrParams = array(
					'credencial' => $credencialDTO,
					'exercicio' => 2011,
					'identificadorUnicoPedido' => 11733,
					'codigoMomento' => '?'
					);
	
	 return $arrParams;
}
?>
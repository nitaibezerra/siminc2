<?php

if( !$_SESSION['par']['inuid'] ){
	echo "
		<script>
			alert('Erro de sessão');
			window.location.href = 'par.php?modulo=inicio&acao=C';
		</script>";
	die();
}

include_once APPRAIZ.'par/classes/Projeto.class.inc';
$obProjeto = new Projeto( $_REQUEST );

if( $_REQUEST['requisicao'] == 'salvarDados' ){
	
	switch ($_POST['tipo']) {
		case 1: #PAR AQUISICAO
		case 9: #SUBAÇÕES DO BRASIL-PRÓ
			if( $_POST['solicitaContaCorrente'] == 'S' ){
				$sql = "SELECT prpbanco as banco, prpagencia as agencia, prpnumeroprocesso as processo FROM par.processopar WHERE prpid = {$_POST['codigo']} AND prpstatus = 'A'";
				$arrParam = $db->pegaLinha( $sql );
				$retorno = $obProjeto->solicitaContaCorrentePorProcesso($_POST['codigo'], $arrParam);
			} else {
				$retorno = $obProjeto->insereProcessoPAR($_POST['processo']);
			}
		break;
		case 2: #OBRAS PAR
		case 10: #OBRAS DO PAR - BRASIL-PRÓ
			if( $_POST['solicitaContaCorrente'] == 'S' ){
				$sql = "SELECT p.probanco as banco, p.proagencia as agencia, p.pronumeroprocesso as processo FROM par.processoobraspar p WHERE proid = {$_POST['codigo']} and p.prostatus = 'A'";
				$arrParam = $db->pegaLinha( $sql );
				$retorno = $obProjeto->solicitaContaCorrentePorProcesso($_POST['codigo'], $arrParam);
			} else {
				$retorno = $obProjeto->insereProcessoObrasPAR($_POST['processo']);
			}
		break;
		case (3): #OBRAS PAC - Quadras
		case (6): #OBRAS PAC - Proinfância
		case (7): #OBRAS PAC - Proinfância Metodologia Inovadora
		case (8): #OBRAS PAC - Quadras Metodologia Inovadora
			if( $_POST['solicitaContaCorrente'] == 'S' ){
				$sql = "SELECT p.probanco as banco, p.proagencia as agencia, p.pronumeroprocesso as processo FROM par.processoobra p WHERE p.prostatus = 'A'  and  p.proid = {$_POST['codigo']}";
				$arrParam = $db->pegaLinha( $sql );
				$retorno = $obProjeto->solicitaContaCorrentePorProcesso($_POST['codigo'], $arrParam);
			} else {
				$retorno = $obProjeto->insereProcessoObrasPAC($_POST['processo']);
			}
			break;
		case 4:
			if( $_POST['solicitaContaCorrente'] == 'S' ){
				$sql = "SELECT prpbanco as banco, prpagencia as agencia, prpnumeroprocesso as processo FROM par.processopar WHERE prpid = {$_POST['codigo']} and sisid = 57 AND prpstatus = 'A'";
				$arrParam = $db->pegaLinha( $sql );
				$retorno = $obProjeto->solicitaContaCorrentePorProcesso($_POST['codigo'], $arrParam);			
			} else {
				$retorno = $obProjeto->insereProcessoPAR($_POST['processo']);
			}
		break;
		case 5:
			if( $_POST['solicitaContaCorrente'] == 'S' ){
				$sql = "SELECT p.probanco as banco, p.proagencia as agencia, p.pronumeroprocesso as processo FROM par.processoobraspar p WHERE proid = {$_POST['codigo']} and p.prostatus = 'A'";
				$arrParam = $db->pegaLinha( $sql );
				$retorno = $obProjeto->solicitaContaCorrentePorProcesso($_POST['codigo'], $arrParam);
			} else {
				$retorno = $obProjeto->insereProcessoObrasPAR($_POST['processo']);
			}
		break;
		/*case 6:
			if( $_POST['solicitaContaCorrente'] == 'S' ){
				$sql = "SELECT prpbanco as banco, prpagencia as agencia, prpnumeroprocesso as processo FROM par.processopar WHERE prpid = {$_POST['codigo']}";
				$arrParam = $db->pegaLinha( $sql );
				$retorno = $obProjeto->solicitaContaCorrentePorProcesso($_POST['codigo'], $arrParam);
			} else {
				$retorno = $obProjeto->insereProcessoPAR($_POST['processo']);
			}
		break;
		case 7:
			if( $_POST['solicitaContaCorrente'] == 'S' ){
				$sql = "SELECT p.probanco as banco, p.proagencia as agencia, p.pronumeroprocesso as processo FROM par.processoobraspar p WHERE proid = {$_POST['codigo']} and p.prostatus = 'A'";
				$arrParam = $db->pegaLinha( $sql );
				$retorno = $obProjeto->solicitaContaCorrentePorProcesso($_POST['codigo'], $arrParam);
			} else {
				$retorno = $obProjeto->insereProcessoObrasPAR($_POST['processo']);
			}
		break;*/
	}
	
	if( $_POST['bovisulizaxml'] != 'S' && $retorno ){		
		if( $db->commit() ){
			echo $obProjeto->mensagem('Criando Projeto', 'Projeto criado com sucesso.');
			die;
		} else {
			echo $obProjeto->mensagem('Criando Projeto', 'Falha na criação do projeto.');
			die;
		}
	}
	exit();
}

if( $_REQUEST['requisicao'] == 'listaSubacaoProcesso' ){
	header('content-type: text/html; charset=ISO-8859-1');
	echo $obProjeto->listaHistoricoProcessoPorTipo($_POST);
	exit();
}

if( $_REQUEST['requisicao'] == 'carregaHistorico' ){
	header('content-type: text/html; charset=ISO-8859-1');
	echo $obProjeto->listaHistoricoProcesso();
	exit();
}

if( $_REQUEST['requisicao'] == 'carregaDados' ){
	header('content-type: text/html; charset=ISO-8859-1');
	echo $obProjeto->carregaDadosAquisicao();
	exit();
}

if( $_REQUEST['requisicao'] == 'existeprocesso' ){
	
	$estuf	= $_SESSION['par']['estuf'];
	$muncod	= $_SESSION['par']['muncod'];
	
	if( $estuf ){
		$filtro = "estuf = '$estuf'";
	} else if( $muncod ){
		$filtro = "muncod = '$muncod'";
	}
	
	switch ($_POST['tipo']) {
		case 1: #Subações Genérico do PAR
			$sql = "SELECT count(prpid) FROM par.processopar WHERE inuid = {$_SESSION['par']['inuid']} and prpstatus = 'A'";
		break;
		case 2: #Obras do PAR
			$sql = "SELECT count(proid) FROM par.processoobraspar WHERE inuid = {$_SESSION['par']['inuid']} and prostatus = 'A'";
		break;
		case 3: #Obras do PAC
			$sql = "SELECT count(proid) FROM par.processoobra WHERE prostatus = 'A' and $filtro";
		break;
		case 4: #Subações de Emendas no PAR
			$sql = "SELECT count(prpid) FROM par.processopar WHERE inuid = {$_SESSION['par']['inuid']} and prpstatus = 'A'";
		break;
	}	
	$existe = (int)$db->pegaUm( $sql );
	
	if( $existe > 0 ){
		echo 'true';
	} else {
		echo 'false';
	}
	exit();
}

if( $_REQUEST['requisicao'] == 'comboprocesso' ){
	
	$estuf	= $_SESSION['par']['estuf'];
	$muncod	= $_SESSION['par']['muncod'];
	
	if( $estuf ){
		$filtro = "estuf = '$estuf'";
	} else if( $muncod ){
		$filtro = "muncod = '$muncod'";
	}
	
	switch ($_POST['tipo']) {
		case 1: #Subações Genérico do PAR
			$sql = "SELECT prpid as codigo, prpnumeroprocesso as descricao FROM par.processopar WHERE inuid = {$_SESSION['par']['inuid']} and prpstatus = 'A'";
		break;
		case 2: #Obras do PAR
			$sql = "SELECT proid as codigo, pronumeroprocesso as descricao FROM par.processoobraspar WHERE inuid = {$_SESSION['par']['inuid']} and prostatus = 'A'";
		break;
		case 3: #Obras do PAC
			$sql = "SELECT proid as codigo, pronumeroprocesso as descricao FROM par.processoobra WHERE prostatus = 'A' and  $filtro";
		break;
		case 4: #Subações de Emendas no PAR
			$sql = "SELECT count(prpid) FROM par.processopar WHERE inuid = {$_SESSION['par']['inuid']} and prpstatus = 'A'";
		break;
	}
	
	echo $db->monta_combo("processo", $sql, 'S','-- Selecione um Processo --','', '', '',150,'S','processo', '', '', 'Processo');
	exit();
}


//include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
//$mnuid = array( 0 => 12172, 1 => 12185 ); // Esconde as Abas "Reformular Projeto" e "Histórico de Reformulações"
//$db->cria_aba( $abacod_tela, $url, $parametros, $mnuid );
//echo'<br>';

echo montarAbasArray( $obProjeto->criaAbaPar(), "par.php?modulo=principal/projetos&acao=A&abas=montarProjeto" );

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);
monta_titulo( $nmEntidadePar, 'Montar Projeto' );

/*$usuario = 'juliov';  
$senha   = 'vasco011';*/ 
$usuario = '';
$senha   = ''; 

$sql = "SELECT tipid, tipdescricao FROM execucaofinanceira.tipoprocesso WHERE tipstatus = 'A' and sisid = 23 order by tipid";
$arrTipoProcesso = $db->carregar( $sql );
$arrTipoProcesso = $arrTipoProcesso ? $arrTipoProcesso : array();
?>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
    <style type="">
    .ui-dialog-titlebar{
    text-align: center;
    }
    </style>
<form name="formulario" id="formulario" method="post" action="" >
<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
	<?
	foreach ($arrTipoProcesso as $key => $v) {
		if($key == 2 || $key == $numDiv){
			$numDiv = $numDiv + 2;
			echo "</tr><tr>";
		}
		
		echo '<td class="subtituloesquerda" width="50%"><label for="tipos_'.$v['tipid'].'"><input type="radio" name="tipos" id="tipos_'.$v['tipid'].'" value="'.$v['tipid'].'" onclick="carregaDados(\''.$v['tipid'].'\')"><b>'.$v['tipdescricao'].'</b></label></td>';
	}
	?>
	 <tr id="tr_executora">
	 	<td class="subtituloesquerda" colspan="2">Outra Entidade Executora?<br>
	 		<label for="entexecutorachec_sim"><input type="radio" name="entexecutorachec" id="entexecutorachec_sim" value="S" onclick="mostraComboEntidade(this.value);">Sim</label>  
	 		<label for="entexecutorachec_nao"><input type="radio" name="entexecutorachec" id="entexecutorachec_nao" value="N" onclick="mostraComboEntidade(this.value);" checked="checked">Não</label><br>
	 		<div id="div_entidade" style="display: none;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?
			$sql = "SELECT iuecnpj as codigo, iuenome as descricao
					FROM par.instrumentounidade iu
						inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
					WHERE
						iu.inuid = {$_SESSION['par']['inuid']}
					    and iue.iuestatus = 'A'";
			$db->monta_combo( "iuecnpj", $sql, 'S', 'Selecione', '', '', '', '', '', 'iuecnpj' );
	 		?></div>
	 	</td>
	 </tr>
	<tr id="tr_div">
		<td colspan="2"><div id="divCarregaDados" style="overflow-x: auto; overflow-y: auto; width:100%; height: 200px">Carregando...</div></td>
	</tr>
	<tr id="tr_btn">
		<td class="subtituloCentro" colspan="2">
			<input type="button" name="liberar" id="liberar" value="Criar Projeto">
			<?if( $db->testa_superuser() ){ ?>
				<input type="button" name="visulizaxml" id="visulizaxml" value="Visualizar XML">
			<?} ?>
			<input type="hidden" name="bovisulizaxml" id="bovisulizaxml" value="N">
		</td>
	</tr>
</table>
<br>
<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
	<tr>
		<th>Histórico de Processos</th>
	</tr>
	<tr>
		<td><div id="divCarregaHistorico" style="overflow-x: auto; overflow-y: auto; width:100%; height: 200px">Carregando...</div></td>
	</tr>
</table>
</form>
<div id="debug"></div>

<div class="demo">
	<div id="dialog-confirm" title="Montagem do Projeto" style="overflow-x: auto; overflow-y: auto; width:100%; height:250px;"></div>
</div>

<div id="dialog-aut" title="Autenticação" style="display:none;">
	<div id="dialog-content-aut">
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td width="30%" style="text-align: right; font-size: 1.5em; color: #333333; font-weight: bold;">Usuário:</td>
				<td>					
					<input type="text" class="ui-button ui-widget ui-state-default ui-corner-all ui-button-text-only" title="Usuário" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
						onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?=$usuario ?>" size="23" id="wsusuario" name="wsusuario">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td style="text-align: right; font-size: 1.5em; color: #333333; font-weight: bold;">Senha:</td>
				<td>
					<input type="password" class="ui-button ui-widget ui-state-default ui-corner-all ui-button-text-only" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
						onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?=$senha ?>" size="23" id="wssenha" name="wssenha">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
		</table>
	</div>
	<div id="resposta" style="color: red; font-size: 0.9em"></div>
</div>

<div id="dialog-proc" title="Processo" style="display:none;">
	<div id="dialog-content-proc">
		<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td style="text-align: justify; font-size: 1.5em; color: #333333; font-weight: bold;" colspan="3">
					Para este município já existem outros processos de "<span id="divTipo"></span>".
					Deseja adicionar estas subações a um processo existente?</td>
			</tr>
			<tr style="text-align: center;">
				<td width="50%"><input type="radio" name="bovincula" id="n_bovincula" value="N" onclick="vinculaProcessoExistente(this.value)"> Não</td>
				<td width="50%"><input type="radio" name="bovincula" id="s_bovincula" value="S" onclick="vinculaProcessoExistente(this.value)"> Sim</td>
			</tr>
			<tr id="tr_processosim">
				<td style="text-align: right; font-size: 1.5em; color: #333333; font-weight: bold;">Selecione o Processo:</td>
				<td><div id="exibeCombo"></div></td>
			</tr>
			<tr id="tr_processonao">
				<td style="text-align: left; font-size: 1.5em; color: #333333; font-weight: bold;" colspan="2">
					Será solicitado a abertura de um novo processo!</td>
			</tr>
		</table>
	</div>
</div>
				 	
<script type="text/javascript">

function mostraComboEntidade(valor){
	if(valor == 'S'){
		$('#div_entidade').show();
	} else {
		$('#div_entidade').hide();
		$('#iuecnpj').val('');
	}
}

$(document).ready(function(){
	$('#tr_div').hide();
	$('#tr_btn').hide();
	$('#tr_processosim').hide();
	$('#tr_processonao').hide();
	$('#tr_executora').hide();
	//$('#exibeCombo').html('');
	
	carregaHistoricoProcesso();
	
	$('#visulizaxml').click(function(){
		$('#bovisulizaxml').val('S');
		abreAutenticacao();
	});
	
	$('#liberar').click(function(){
		var boSelect = false;
		$("#formulario input").each(function(){
			if( $(this).attr('type') == 'checkbox'){
				if($(this).attr('checked') == true){
					boSelect = true;
				}
			}
		});
		if( boSelect == false ){
			alert('Selecione pelo menos um item!');
			return false;
		} else {
			var checkEnt = $('[name="entexecutorachec"]:checked').val();
			if(checkEnt == 'S'){
				if($('#iuecnpj').val() == ''){
					alert('Informe uma Entidade Executora');
					$('#iuecnpj').focus();
					return false;
				}
			}
			var check = $('[name="tipos"]:checked').val();
			var existeProc = 'false'; //verificaExisteProcesso(check);
			
			if( existeProc == 'true' ){
				/*$('.ui-icon ui-icon-closethick').hide();
				$( '#dialog-aut' ).hide();
				$( '#dialog-proc' ).hide();
				$( '#dialog-confirm' ).hide();
				$( '#dialog-proc' ).dialog({
					resizable: false,
					width: 500,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						'Ok': function() {
							var check = $('[name="bovincula"]:checked').val();
							if( check == 'S' ){
								if( $('#processo').val() == '' ){
									alert('É necessário informar um número de processo!');
									return false;
								}
								$( this ).dialog( 'close' );
								montaProjetoComProcesso();
							} else {
								abreAutenticacao();
								$( this ).dialog( 'close' );
							}
						},
						'Cancel': function() {
							$( this ).dialog( 'close' );
							//window.location.reload();
						}
					}
				});*/
			} else {
				abreAutenticacao();
			}
		}
	});
	
});

function marcarChk(obj){
	var atual = '';
	var proximo = '';	
	var boDiferente = false;
	var getValor = '';
	var preid 	= getValor[0];
	var tipo 	= ''
	
	$('[name="chk[]"]').each(function(){
		if( $(this).attr('checked') == true ){
			getValor 	= $(this).val().split("_");
			proximo		= getValor[2];
			
			if( atual == '' ){
				atual = proximo;
			}
			if(atual != proximo){
				alert('Para criar processo do PAC a classificação das obras devem ser dos mesmo tipo.');
				$(obj).attr('checked', false);
				boDiferente = true;
				return false;
			}
		}
	});
}

function solicitarContaCorrente(codigo, tipo){
	$('.ui-icon ui-icon-closethick').hide();
	$( '#dialog-aut' ).hide();
	$( '#dialog-proc' ).hide();
	$( '#dialog-confirm' ).hide();
	$( '#dialog-aut' ).dialog({
		resizable: false,
		width: 400,
		modal: true,
		show: { effect: 'drop', direction: "up" },
		buttons: {
			'Ok': function() {
				$( this ).dialog( 'close' );
				executaContaCorrente(codigo, tipo);
			},
			'Cancel': function() {
				$( this ).dialog( 'close' );
				//window.location.reload();
			}
		}
	});
}

function executaContaCorrente(codigo, tipo){
	divCarregando('formulario');
	
	var usuario = $('#wsusuario').val();
	var senha = $('#wssenha').val();
	
	if( usuario == '' ){
		alert('Usuário está em branco!');
		//$('#resposta').html('Usuário está em branco!');
		return false;
	}
	if( senha == '' ){
		alert('Senha está em branco!');
		//$('#resposta').html('Usuário está em branco!');
		return false;
	}
	
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/montarProjeto&acao=A",
	   		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=salvarDados&tipo="+tipo+'&solicitaContaCorrente=S&bovisulizaxml=N&tipo=1&codigo='+codigo+'&'+$('#formulario').serialize(),
	   		async: false,
	   		success: function(msg){
	   			$('.ui-icon ui-icon-closethick').hide();
				//$( '#dialog-aut' ).dialog( 'destroy' );
				$( '#dialog-aut' ).hide();
				$( '#dialog-proc' ).hide();
				$( '#dialog-confirm' ).hide();
   				$( "#dialog-confirm" ).html(msg);
				$( "#dialog-confirm" ).dialog({
					resizable: false,
					height:300,
					width:500,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						"OK": function() {
							$( this ).dialog( "close" );
							window.location.reload();								
						}
						
					}
				});
	   		}
		});
	divCarregado();
}

function carregaHistoricoProcesso(){
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/montarProjeto&acao=A",
   		data: "requisicao=carregaHistorico",
   		async: false,
   		success: function(msg){
   			document.getElementById('divCarregaHistorico').innerHTML = msg;
   		}
	});
}

function vinculaProcessoExistente(valor){
	var check = $('[name="tipos"]:checked').val();
	
	if(valor == 'S'){
		$('#exibeCombo').html('');
		carregaComboProcesso(check);
		$('#tr_processosim').show();
		$('#tr_processonao').hide();
	} else {
		$('#exibeCombo').html('');
		$('#tr_processosim').hide();
		$('#tr_processonao').show();
	}
}

function abreAutenticacao(){
	
	$( '#dialog-aut' ).hide();
	$( '#dialog-proc' ).hide();
	$( '#dialog-confirm' ).hide();
	$('.ui-icon-closethick').css('display', 'none');
	$( '#dialog-aut' ).dialog({
		resizable: false,
		width: 400,
		modal: true,
		show: { effect: 'drop', direction: "up" },
		buttons: {
			'Ok': function() {
				$( this ).dialog( 'close' );
				montaProjeto();
			},
			'Cancel': function() {
				$( this ).dialog( 'close' );
				window.location.reload();
			}
		}
	});
}

function montaProjeto(){
	
	var check = $('[name="tipos"]:checked').val();
	
	var usuario = $('#wsusuario').val();
	var senha = $('#wssenha').val();
	
	if( usuario == '' ){
		alert('Usuário está em branco!');
		//$('#resposta').html('Usuário está em branco!');
		return false;
	}
	if( senha == '' ){
		alert('Senha está em branco!');
		//$('#resposta').html('Usuário está em branco!');
		return false;
	}
	divCarregando();
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/montarProjeto&acao=A",
	   		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=salvarDados&tipo="+check+'&bovisulizaxml='+$('#bovisulizaxml').val()+'&'+$('#formulario').serialize(),
	   		async: false,
	   		success: function(msg){
	   			
				$( '#dialog-aut' ).hide();
				$( '#dialog-proc' ).hide();
				$( '#dialog-confirm' ).hide();
   				$( "#dialog-confirm" ).html(msg);		
				$( "#dialog-confirm" ).dialog({
					resizable: false,
					height:300,
					width:500,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					beforeClose: function( event, ui ) {window.location.reload();},
					buttons: {
						"OK": function() {
							$( this ).dialog( "close" );
							window.location.reload();								
						}
						
					}
				});
	   		}
		});
	divCarregado();
}

function montaProjetoComProcesso(){
	
	var check = $('[name="tipos"]:checked').val();
	
	if( $('#processo').val() == '' ){
		alert('É necessário informar um número de processo!');
		//return false;
	} else {
		divCarregando();
		$.ajax({
		   		type: "POST",
		   		url: "par.php?modulo=principal/montarProjeto&acao=A",
		   		data: "processo=" + $('#processo').val() + "&requisicao=salvarDados&bovisulizaxml=N&tipo="+check+'&'+$('#formulario').serialize(),
		   		async: true,
		   		success: function(msg){
		   			$( '#dialog-aut' ).hide();
					$( '#dialog-proc' ).hide();
					$( '#dialog-confirm' ).hide();
	   				$( "#dialog-confirm" ).html(msg);		
					$( "#dialog-confirm" ).dialog({
						resizable: false,
						height:300,
						width:500,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
							"OK": function() {
								$( this ).dialog( "close" );
								window.location.reload();								
							}
							
						}
					});
		   		}
			});
		divCarregado();
	}
}

function carregaDados( tipo ){
	divCarregando('formulario');
	if(tipo == 3){
		$('#tr_executora').hide();
	} else {
		$('#tr_executora').show();
	}
	$('#entexecutorachec_nao').attr('checked', true);
	$('#div_entidade').hide();
	$('#iuecnpj').val('');
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/montarProjeto&acao=A",
	   		data: "requisicao=carregaDados&tipo="+tipo,
	   		async: false,
	   		success: function(msg){
	   			//alert(msg);
	   			if( msg ){
	   				$('#tr_btn').show();
		   			//$('#divCarregaDados').css('height','0');
		   			$('#tr_div').show();
		   			//$('#divCarregaDados').css('height', $(window).height());
		   			$('#divCarregaDados').html(msg);
		   		} else {
		   			$('#divCarregaDados').html('');
		   		}
		   		//$('[class="listagem"]').css('width', '100%');
	   		}
		});
	divCarregado();
}

function carregaComboProcesso( tipo ){
	
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/montarProjeto&acao=A",
	   		data: "requisicao=comboprocesso&tipo="+tipo,
	   		async: false,
	   		success: function(msg){
	   			
	   			if( msg != '' ){
					document.getElementById('exibeCombo').innerHTML = msg;
		   		} else {
					$('#exibeCombo').html('');
		   		}
	   		}
		});
}

function verificaExisteProcesso( tipo ){
	var retorno = '';
	
	if( tipo == 1 ){
		$('#divTipo').html('Subações Genérico do PAR');
	}else if( tipo == 2 ){
		$('#divTipo').html('Obras do PAR');
	}else if( tipo == 3 ){
		$('#divTipo').html('Obras do PAC');
	}else if( tipo == 4 ){
		$('#divTipo').html('Subações de Emendas no PAR');
	}
	
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/montarProjeto&acao=A",
	   		data: "requisicao=existeprocesso&tipo="+tipo,
	   		async: false,
	   		success: function(msg){
	   			retorno = msg;
	   		}
		});
	return retorno;
}

function carregarListaProcesso(idImg, codigo, tipo){
	var img 	 = $('#'+idImg );
	var tr_nome = 'listaSubacaoProcesso_'+ codigo;
	var td_nome  = 'trV_'+ codigo;
	
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ''){
		$('#'+td_nome).html('Carregando...');
		img.attr('src', '../imagens/menos.gif');
		listaSubacaoProcesso(codigo, td_nome, tipo);
	}
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
		$('#'+tr_nome).css('display', '');
		img.attr('src', '../imagens/menos.gif');
	} else {
		$('#'+tr_nome).css('display', 'none');
		img.attr('src', '/imagens/mais.gif');
	}
}

function listaSubacaoProcesso(codigo, td_nome, tipo){
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/montarProjeto&acao=A",
   		data: "requisicao=listaSubacaoProcesso&codigo="+codigo+"&tipo="+tipo,
   		async: false,
   		success: function(msg){
   			$('#'+td_nome).html(msg);
   		}
	});
}
</script>
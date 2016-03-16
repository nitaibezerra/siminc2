<?php
global $db;

if ( $_REQUEST['inserePregao'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	inserePregao( $_REQUEST );
	exit;
}

if ( $_REQUEST['atualizaPregao'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	atualizaPregao( $_REQUEST );
	exit;
}

if ( $_REQUEST['pesquisaItemGrupoPregao'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	pesquisaItemGrupoPregao( $_REQUEST['preid'] );
	exit;
}

if ( $_REQUEST['excluiItemGrupoPregao'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	excluiItemGrupoPregao( $_REQUEST['igpid'] );
	exit;
}

if($_REQUEST['preid']){
	
	$sql = "SELECT 
			  preid,
			  precodigo,
			  preobjeto,
			  predatainicialpregao,
			  predatafinalpregao,
			  predatainicialpreenchimento,
			  predatafinalpreenchimento
			FROM 
			  rehuf.pregao
			WHERE preid = '{$_REQUEST['preid']}'";
			
	$arDados = $db->pegaLinha($sql);
	
	if($arDados){
		$preid						 = $arDados['preid'];
		$precodigo 					 = $arDados['precodigo'];
		$preobjeto 					 = $arDados['preobjeto'];
		$predatainicialpregao 		 = $arDados['predatainicialpregao'];
		$predatafinalpregao 		 = $arDados['predatafinalpregao'];
		$predatainicialpreenchimento = $arDados['predatainicialpreenchimento'];
		$predatafinalpreenchimento 	 = $arDados['predatafinalpreenchimento'];
		
		if($predatainicialpregao){
			$predatainicialpregao = formata_data($predatainicialpregao);
		}		
		if($predatafinalpregao){
			$predatafinalpregao = formata_data($predatafinalpregao);
		}		
		if($predatainicialpreenchimento){
			$predatainicialpreenchimento = formata_data($predatainicialpreenchimento);
		}
		if($predatafinalpreenchimento){
			$predatafinalpreenchimento = formata_data($predatafinalpreenchimento);
		}
	}
}else{
	$preid						 = "";
	$precodigo 					 = "";
	$preobjeto 					 = "";
	$predatainicialpregao 		 = "";
	$predatafinalpregao 		 = "";
	$predatainicialpreenchimento = "";
	$predatafinalpreenchimento 	 = "";
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Cadastro de Pregão', '' );
?>

<style>
@CHARSET "ISO-8859-1";

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 55%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>
<script	src="/includes/calendario.js"></script>
<body>

<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

<input type="hidden" name="preid" id="preid" value="<?=$preid; ?>"> 

<table id="tblCadItensPregao" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Código Pregão:</td>
		<td><?=campo_texto( 'precodigo', 'S', 'S', '', 40, 100, '', '','','','','id="precodigo"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Objeto do Pregão:</td>
		<td><?=campo_texto( 'preobjeto', 'S', 'S', '', 40, 250, '', '','','','','id="preobjeto"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Período de Preenchimento:</td>
		<td><?=campo_data('predatainicialpreenchimento', 'N','S','','','','');?> até
						<?=campo_data('predatafinalpreenchimento', 'S','S','','','',''); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Vigência do Pregão:</td>
		<td><?=campo_data('predatainicialpregao', 'N','S','','','',''); ?> até
						<?=campo_data('predatafinalpregao', 'N','S','','','',''); ?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="validaFormPregao();">
		<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="Voltar();"></th>
	</tr>
</table>
</form>

<div id="addItem">
	<? if($preid){ pesquisaItemGrupoPregao($preid);} ?>
	
</div>

<div id="erro"></div>
<script type="text/javascript">
function excluiItemGrupoPregao(igpid){
	var myAjax = new Ajax.Request('rehuf.php?modulo=principal/cadastroPregao&acao=A',{
				method:		'post',
				parameters: '&excluiItemGrupoPregao=true&igpid='+igpid,
				asynchronous: false,
				onComplete: function(res){
					if(res.responseText == "1"){
						alert('Operação realizada com sucesso!');
						pesquisaItemGrupoPregao();
					}else{
						alert('Operação não realizada!');
					}
				}						
			});
}

function AddItem(){
	window.open('rehuf.php?modulo=principal/popupItemGrupo&acao=A&preid='+$('preid').value,'page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=1000,height=500');
}

function pesquisaItemGrupoPregao(){
	var myAjax = new Ajax.Request('rehuf.php?modulo=principal/cadastroPregao&acao=A',{
					method:		'post',
					parameters: '&pesquisaItemGrupoPregao=true&preid='+$('preid').value,
					asynchronous: false,
					onComplete: function(res){
						$('addItem').innerHTML = res.responseText; 
					}						
				});
}

function validaFormPregao(){
	if($('precodigo').value == ""){
		alert('O campo Código Pregão é de preenchimento obrigatório!');
		$('precodigo').focus();
		return false;
	}else if($('preobjeto').value == ""){
		alert('O campo Objeto do Pregão é de preenchimento obrigatório!');
		$('preobjeto').focus();
		return false;
	}else if( ($('predatainicialpreenchimento').value == "") || ($('predatafinalpreenchimento').value == "") ){
		alert('O campo Período de Preenchimento é de preenchimento obrigatório!');
		if( ($('predatainicialpreenchimento').value == "") ){
			$('predatainicialpreenchimento').focus();
			return false;
		}
		if( ($('predatafinalpreenchimento').value == "") ){
			$('predatafinalpreenchimento').focus();
			return false;
		}
		return false;
	}else if( ($('predatainicialpregao').value != "") && ($('predatafinalpregao').value == "") || ($('predatainicialpregao').value == "") && ($('predatafinalpregao').value != "")){
		alert('O campo Vigência do Pregão é de preenchimento obrigatório!');
		if( ($('predatainicialpregao').value != "") && ($('predatafinalpregao').value == "") ){
			$('predatafinalpregao').focus();
		}else{
			$('predatainicialpregao').focus();
		}
			return false;
	}else if( !validaDataMaior($('predatainicialpreenchimento'), $('predatafinalpreenchimento')) ){
		alert("A preenchimento inicial não pode ser maior que o preenchimento final!");
		$('predatainicialpreenchimento').focus();
		return false;
	}else{
		if( ($('predatainicialpregao').value != "") && ($('predatafinalpregao').value != "") ){
			if( !validaDataMaior($('predatainicialpregao'), $('predatafinalpregao') ) ){
				alert("A data de vigência inicial não pode ser maior que a vigência final!");
				$('predatainicialpregao').focus();
				return false;
			}else if(!validaData($('predatainicialpregao') ) ) {
				alert('Data Vigência esta no formato incorreto');
				$('predatainicialpregao').focus();
				return false;
			}else if(!validaData($('predatafinalpregao') ) ) {
				alert('Data Vigência esta no formato incorreto');
				$('predatafinalpregao').focus();
				return false;
			}
		}
		
		if( ($('predatainicialpreenchimento').value != "") && ($('predatafinalpreenchimento').value != "") ){
			if(!validaData($('predatainicialpreenchimento') ) ) {
				alert('Data do período de preenchimento esta no formato incorreto');
				$('predatainicialpreenchimento').focus();
				return false;
			}else if(!validaData($('predatafinalpreenchimento') ) ) {
				alert('Data do período de preenchimento esta no formato incorreto');
				$('predatafinalpreenchimento').focus();
				return false;
			}
		}
		inserePregao();
	}
	
}
function inserePregao(){
	$('loader-container').show();
	
	if($('preid').value == ""){
		var myAjax = new Ajax.Request('rehuf.php?modulo=principal/cadastroPregao&acao=A',{
					method:		'post',
					parameters: '&inserePregao=true&precodigo='+$('precodigo').value+
												'&preobjeto='+$('preobjeto').value+
												'&predatainicialpreenchimento='+$('predatainicialpreenchimento').value+
												'&predatafinalpreenchimento='+$('predatafinalpreenchimento').value+
												'&predatainicialpregao='+$('predatainicialpregao').value+
												'&predatafinalpregao='+$('predatafinalpregao').value,
					asynchronous: false,
					onComplete: function(res){
						//$('erro').innerHTML = res.responseText;
						if( Number(res.responseText) ){
							alert("Operação realizada com sucesso!");
							$('preid').value = res.responseText;
							pesquisaItemGrupoPregao();
						}else{
							if(res.responseText.length < 200){
								alert(res.responseText);
							}else{
								alert('Operação não realizada, entre em contato com administrador do sistema!');
							}	
						}
					}						
				});
	}else{
		var myAjax = new Ajax.Request('rehuf.php?modulo=principal/cadastroPregao&acao=A',{
					method:		'post',
					parameters: '&atualizaPregao=true&preid='+$('preid').value+
												'&precodigo='+$('precodigo').value+
												'&preobjeto='+$('preobjeto').value+
												'&predatainicialpreenchimento='+$('predatainicialpreenchimento').value+
												'&predatafinalpreenchimento='+$('predatafinalpreenchimento').value+
												'&predatainicialpregao='+$('predatainicialpregao').value+
												'&predatafinalpregao='+$('predatafinalpregao').value,
					asynchronous: false,
					onComplete: function(res){
						//$('erro').innerHTML = res.responseText; 
						if( res.responseText == 1 ){
							alert("Operação realizada com sucesso!");
							//Voltar();
						}else{
							if(res.responseText.length < 200){
								alert(res.responseText);
							}else{
								alert('Operação não realizada, entre em contato com administrador do sistema!');
							}	
						}
					}						
				});
	}
	$('loader-container').hide();
}
function Voltar(){
	window.location.href = 'rehuf.php?modulo=principal/pesquisaPregao&acao=A';
}
</script>
</body>
	
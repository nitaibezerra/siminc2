<?php

if ( $_REQUEST['pesquisar'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	pesquisarPregao( $_REQUEST );
	exit;
}

if ( $_REQUEST['excluiPregao'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	excluiPregao( $_REQUEST['preid'] );
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Pesquisar Pregão', '' );
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

<table id="tblCadItensPregao" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Código Pregão:</td>
		<td><?=campo_texto( 'precodigo', 'N', 'S', '', 40, 100, '', '','','','','id="precodigo"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Objeto do Pregão:</td>
		<td><?=campo_texto( 'preobjeto', 'N', 'S', '', 40, 250, '', '','','','','id="preobjeto"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Período de Preenchimento:</td>
		<td><?=campo_data('predatainicialpreenchimento', 'N','S','','','','');?> até
						<?=campo_data('predatafinalpreenchimento', 'N','S','','','',''); ?></td>
	</tr>
	<!-- <tr>
		<td class="SubTituloDireita">Vigência do Pregão:</td>
		<td><?=campo_data('predatainicialpregao', 'N','S','','','',''); ?> até
						<?=campo_data('predatafinalpregao', 'N','S','','','',''); ?></td>
	</tr> -->
	<tr>
		<td class="SubTituloDireita">Situação do Preenchimento:</td>
		<td>
			<label for="radSituacaoPreenchimento0"><input type="radio" name="radSituacaoPreenchimento" id="radSituacaoPreenchimento0" checked="checked" value="0">Todos</label>
			<label for="radSituacaoPreenchimento3"><input type="radio" name="radSituacaoPreenchimento" id="radSituacaoPreenchimento3" value="3">Preenchimento não iniciado</label>
			<label for="radSituacaoPreenchimento2"><input type="radio" name="radSituacaoPreenchimento" id="radSituacaoPreenchimento2" value="2">Em preenchimento</label>
			<label for="radSituacaoPreenchimento1"><input type="radio" name="radSituacaoPreenchimento" id="radSituacaoPreenchimento1" value="1">Preenchimento concluído</label>
		</td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Pesquisar" name="btnPesquisa" id="btnPesquisa" onclick="pesquisaPregao();">
		<input type="button" value="Novo Pregão" name="btnNovo" id="btnNovo" onclick="cadastroPregao();"></th>
	</tr>
</table>
</form>
<div id="lista">
<? pesquisarPregao(''); ?>
</div>
<div id="erro"></div>
<script type="text/javascript"><!--
function validaPesquisa(){
	if( ($('predatainicialpreenchimento').value != "") && ($('predatafinalpreenchimento').value == "") || ($('predatainicialpreenchimento').value == "") && ($('predatafinalpreenchimento').value != "")){
		
		if($('predatainicialpreenchimento').value == ""){
			alert('O campo período inicial é de preenchimento obrigatório!');
			$('predatainicialpreenchimento').focus();
			return false;
		}else{
			alert('O campo período final é de preenchimento obrigatório!');
			$('predatafinalpreenchimento').focus();
			return false;
		}
	}else{
		if( ($('predatainicialpreenchimento').value != "") && ($('predatafinalpreenchimento').value != "") ){
			
			if( !validaDataMaior($('predatainicialpreenchimento'), $('predatafinalpreenchimento')) ){
				alert("A preenchimento inicial não pode ser maior que o preenchimento final!");
				$('predatainicialpreenchimento').focus();
				return false;
			}

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
	}
	return true;
	
	/*if( ($('predatainicialpregao').value != "") && ($('predatafinalpregao').value == "") || ($('predatainicialpregao').value == "") && ($('predatafinalpregao').value != "")){
		
		if($('predatainicialpregao').value == ""){
			alert('O campo vigência inicial é de preenchimento obrigatório!');
			$('predatainicialpregao').focus();
			return false;
		}else{
			alert('O campo vigência final é de preenchimento obrigatório!');
			$('predatafinalpregao').focus();
			return false;
		}
	}else{
		if( ($('predatainicialpregao').value != "") && ($('predatafinalpregao').value != "") ){
			if( $('predatainicialpregao').value > $('predatafinalpregao').value ){
				alert("A data de vigência inicial não pode ser maior que o vigência final!");
				$('predatainicialpregao').focus();
				return false;
			}
		}else{
			return true;
		}
	}*/
}
function pesquisaPregao(){
//alert(window.document.formulario.radSituacaoPreenchimento.selectedItem);
	if(validaPesquisa()){
		var situacaoPreenchimento;
		for (i=0;i<window.document.formulario.radSituacaoPreenchimento.length;i++) {
			if (window.document.formulario.radSituacaoPreenchimento[i].checked) {
				situacaoPreenchimento = window.document.formulario.radSituacaoPreenchimento[i].value;
			}
		}
	
		$('loader-container').show();
		var myAjax = new Ajax.Request('rehuf.php?modulo=principal/pesquisaPregao&acao=A',{
							method:		'post',
							parameters: '&pesquisar=true&precodigo='+$('precodigo').value+
														'&preobjeto='+$('preobjeto').value+
														'&predatainicialpreenchimento='+$('predatainicialpreenchimento').value+
														'&predatafinalpreenchimento='+$('predatafinalpreenchimento').value+
														'&radSituacaoPreenchimento='+ situacaoPreenchimento,
														//'&predatafinalpregao='+$('predatafinalpregao').value,
							asynchronous: false,
							onComplete: function(res){
								$('lista').innerHTML = res.responseText;
							}						
						});
		$('loader-container').hide();
	}
}

function cadastroPregao(){
	window.location.href = "rehuf.php?modulo=principal/cadastroPregao&acao=A";
}

function excluiPregao(preid){
	if(confirm("Tem certeza que deseja excluir este registro?")){
		$('loader-container').show();
		var myAjax = new Ajax.Request('rehuf.php?modulo=principal/pesquisaPregao&acao=A',{
								method:		'post',
								parameters: '&excluiPregao=true&preid='+preid,
								asynchronous: false,
								onComplete: function(res){
									$('erro').innerHTML = res.responseText;
									if( res.responseText == 1 ){
										alert("Operação realizada com sucesso!");
										pesquisaPregao();
									}else{
										alert('Operação não realizada, entre em contato com administrador do sistema!');	
									}
								}						
							});
		$('loader-container').hide();
	}
}
--></script>
</body>
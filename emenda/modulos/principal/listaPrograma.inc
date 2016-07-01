<?php

if ($_REQUEST['pesquisaAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	pesquisaProgramaAjax($_POST);
	exit;
}
if ($_REQUEST['excluirProgramaAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	excluirProgramaAjax($_POST['proid']);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

//echo montarAbasArray(carregaAbasEmendas('listaPrograma'), "/emenda/emenda.php?modulo=principal/listaPrograma&acao=A");
monta_titulo( 'Listar Programas', "Filtro de Pesquisa" );
unset($_SESSION['emenda']['proid']);
?>
<html>
<head>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
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
</head>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script	src="/includes/calendario.js"></script>
<body>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Nome do Programa:</td>
		<td><?=campo_texto( 'pronome', 'N', 'S', '', 40, 500, '', '','','','','id="pronome"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nível de Ensino:</td>
		<?
		$sql = "SELECT 
				  tpeid as codigo,
				  tpedesc as descricao
				FROM 
				  public.tipoensino
				WHERE
					tpestatus = 'A'";
		?>
		<td><?= $db->monta_combo("tpeid",$sql, 'S','-- Selecione --','', '', '',215,'N','tpeid');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Vigência:</td>
		<td><?=campo_data('provigenciainicial', 'N','S','Vigência Inicial','','','');?> a
						<?=campo_data('provigenciafinal', 'N','S','Vigência Final','','',''); ?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" onclick="pesquisar();">
		<input type="button" value="Novo Programa" name="btnNovo" id="btnNovo" onclick="novoPrograma();"></th>
	</tr>
</table>
</form>
<div id="lista"><? pesquisaProgramaAjax(''); ?></div>

<script type="text/javascript">
	function pesquisar(){
		//alert(formulario.serialize());
		if( ($('provigenciainicial').value != "") && ($('provigenciafinal').value != "") ){
			if( !validaDataMaior($('provigenciainicial'), $('provigenciafinal')) ){
				alert("A vigência inicial não pode ser maior que vigência final!");
					$('provigenciainicial').focus();
				return false;
			}else if(!validaData($('provigenciainicial') ) ) {
				alert('Data Vigência esta no formato incorreto');
				$('provigenciainicial').focus();
				return false;
			}else if(!validaData($('provigenciafinal') ) ) {
				alert('Data Vigência esta no formato incorreto');
				$('provigenciafinal').focus();
				return false;
			}
		}
		
		$('loader-container').show();
		var myajax = new Ajax.Request('emenda.php?modulo=principal/listaPrograma&acao=A', {
			        method:     'post',
			        parameters: '&pesquisaAjax=true&tpeid='+$('tpeid').value+
			        							   '&pronome='+$('pronome').value+
			        							   '&provigenciainicial='+$('provigenciainicial').value+
			        							   '&provigenciafinal='+$('provigenciafinal').value,
			        asynchronous: false,
			        onComplete: function (res){
						$('lista').innerHTML = res.responseText;
			        }
			  });
		$('loader-container').hide();
	}
	function excluiPrograma(proid){
		if(confirm("Tem certeza que deseja excluir este registro?")){
			$('loader-container').show();
		
			var myAjax = new Ajax.Request('emenda.php?modulo=principal/listaPrograma&acao=A', {
						        method:     'post',
						        parameters: '&excluirProgramaAjax=true&proid='+proid,
						        onComplete: function (res){	
									if(Number(res.responseText) ){
										alert('Operação realizada com sucesso!');
										pesquisar(); 
									}else{
										alert('Operação não realizada!');
									}
						        }
						  });
			$('loader-container').hide();
		}
	}
	function novoPrograma(){
		window.location.href = 'emenda.php?modulo=principal/cadastroPrograma&acao=A';
	}
</script>

</body>
</html>
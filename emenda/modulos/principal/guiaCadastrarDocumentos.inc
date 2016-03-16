<?php
if( $_POST['requisicao'] == 'salvar' ){
	extract($_POST);
	
	$gdonome = (($_REQUEST["gdonome"] != "") ? stripcslashes(pg_escape_string( str_replace( "'", '"', $_REQUEST["gdonome"]))) : 'NULL');
	$gdodescricao = (($_REQUEST["gdodescricao"] != "") ? stripcslashes(pg_escape_string( str_replace( "'", '"', $_REQUEST["gdodescricao"]))) : 'NULL');
	
	if( empty($gdoid) ){
		$sql = "INSERT INTO emenda.guiadocumento(gdonome, gdodescricao) 
				VALUES ('$gdonome', '$gdodescricao')";
		$db->executar( $sql );
	} else {
		$sql = "UPDATE emenda.guiadocumento SET 
				  	gdonome = '$gdonome',
				  	gdodescricao = '$gdodescricao' 
				WHERE 
				  	gdoid = $gdoid";
		$db->executar( $sql );
	}
	if($db->commit()){
		$db->sucesso('principal/guiaListaDocumentos');
	} else {
		echo "<script>
				alert('Falha na operação');
			</script>";
	}
}
header('content-type: text/html; charset=ISO-8859-1');

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Documentos Obrigatórios", "Cadastro de Documentos Obrigatórios" );
extract( $_POST );


if( $_REQUEST['gdoid'] ){
	$sql = "SELECT gdoid, gdonome, gdodescricao 
			FROM emenda.guiadocumento WHERE gdoid = ".$_REQUEST['gdoid'];
	$arrDocumento = $db->pegaLinha( $sql );
	$arrDocumento = $arrDocumento ? $arrDocumento : array();
	extract($arrDocumento);
	
	$gdonome 			= $gdonome;
	$gdodescricao 	= $gdodescricao;
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form id="formulariocad" name="formulariocad" action="" method="post" enctype="multipart/form-data" >
<input type="hidden" name="gdoid" id="gdoid" value="<?=$_REQUEST['gdoid'] ?>">
<input type="hidden" name="requisicao" id="requisicao" value="">

<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:19.5%;">Nome:</td>
		<td><?=campo_texto( 'gdonome', 'S', 'S', '', 75, 250, '', '','','','','id="gdonome"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Descrição:</td>
		<td><?=campo_textarea('gdodescricao', 'S', 'S', 'Descrição', 80, 18, 14000, '', '', '', '', 'Descrição');?></td>
	</tr>
	<tr>
		<th align="center" colspan="2">
			<input type="submit" value="Salvar" name="btnSalvar" id="btnSalvar">
			<input type="button" value="Fechar" name="btnFechar" id="btnFechar"/></th>
	</tr>
</table>
<div id="debug"></div>
<script type="text/javascript">
$(document).ready(function(){
	$('#btnSalvar').click(function(){
	
		if( $('#gdonome').val() == '' ){
			alert('O campo "Nome" é obrigatório.');
			$('#gdonome').focus();
			return false;
		}
		if( $('#gdodescricao').val() == '' ){
			alert('O campo "Descrição" é obrigatório.');
			$('#gdodescricao').focus();
			return false;
		}
		
		$('#requisicao').val('salvar');
		$('#formulario').submit();
	});
	
	$('#btnFechar').click(function(){
		window.location.href = 'emenda.php?modulo=principal/guiaListaDocumentos&acao=A';
	});
});
</script>
</form>
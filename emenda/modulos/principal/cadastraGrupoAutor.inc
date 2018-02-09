<?php

if($_REQUEST['gpaid']){
	$sql = "SELECT 
			  gpaid,
			  gpanome,
			  gpacategoria,
			  gpastatus
			FROM 
			  emenda.grupoautor
			WHERE gpaid = ". $_REQUEST['gpaid'];
	
	$arGrupoAutor = $db->pegaLinha($sql);
	
	extract($arGrupoAutor);
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Cadastro de Grupo de Autores', "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios" );
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
<script type="text/javascript" src="js/emenda.js"></script>
<body>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

<input type="hidden" id="gpaid" name="gpaid" value="<?=$gpaid; ?>">

<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Nome:</td>
		<td><?=campo_texto( 'gpanome', 'S', 'S', 'Nome', 60, 200, '', '','','','','id="gpanome"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Categoria:</td>
		<td><?=campo_texto( 'gpacategoria', 'S', 'S', 'Categoria', 20, 10, '', '','','','','id="gpacategoria"'); ?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="salvarGrupoAutor();">
		<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="cancelarGrupoAutor();"></th>
	</tr>
</table>
</form>
<div id="erro"></div>
</body>
</html>
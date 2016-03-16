<?php
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Listar Grupo de Autores', "Filtro de Pesquisa" );
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

<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Nome:</td>
		<td><?=campo_texto( 'gpanome', 'N', 'S', '', 60, 200, '', '','','','','id="gpanome"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Categoria:</td>
		<td><?=campo_texto( 'gpacategoria', 'N', 'S', '', 20, 10, '', '','','','','id="gpacategoria"'); ?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" onclick="pesquisaGrupoAutor();">
		<input type="button" value="Novo Grupo" name="btnTipoAutor" id="btnTipoAutor" onclick="novoGrupoAutor();"></th>
	</tr>
</table>
</form>
	<div id="lista">
		<?php
			echo "<script>pesquisaGrupoAutor();</script>"
		?>
	</div>
</body>
<div id="erro"></div>
</html>
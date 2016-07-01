<?php
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Listar Tipo de Autores', "Filtro de Pesquisa" );
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
		<td class="SubTituloDireita" style="width:39.5%;">Nome do Tipo:</td>
		<td><?=campo_texto( 'tpanome', 'N', 'S', '', 60, 200, '', '','','','','id="tpanome"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Grupo:</td>
		<?
		$sql = "SELECT 
				  gpaid as codigo,
				  gpanome as descricao				  
				FROM 
				  emenda.grupoautor
				WHERE 
				  gpastatus = 'A'
				ORDER BY gpanome";
		?>
		<td><?=$db->monta_combo("gpaid",$sql, 'S','-- Selecione --','', '', '','','N','gpaid');?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" onclick="pesquisaTipoAutor();">
		<input type="button" value="Novo Tipo" name="btnTipoAutor" id="btnTipoAutor" onclick="novoTipoAutor();"></th>
	</tr>
</table>
</form>
	<div id="lista">
		<?php
			echo "<script>pesquisaTipoAutor();</script>"
		?>
	</div>
</body>
<div id="erro"></div>
</html>
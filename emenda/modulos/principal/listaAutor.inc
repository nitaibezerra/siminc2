<?php

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Listar Autores', "Filtro de Pesquisa" );
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
<script	src="/includes/calendario.js"></script>
<body>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Código:</td>
		<td><?=campo_texto( 'autcod', 'N', 'S', '', 20, 15, '[#]', '','','','','id="autcod"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo do Autor:</td>
		<?
		$sql = "SELECT 
				  tpaid as codigo,
				  tpanome as descricao
				FROM 
				  emenda.tipoautor
				WHERE
				  tpastatus = 'A'
				ORDER BY tpanome";
		?>
		<td><?=$db->monta_combo("tpaid",$sql, 'S','-- Selecione --','', '', '','','N','tpaid');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Estado:</td>
		<?
		$sql = "SELECT 
				  estuf as codigo,
				  estuf || ' - ' ||
				  estdescricao as descricao
				FROM 
				  territorios.estado
				ORDER BY estuf";
		?>
		<td><?=$db->monta_combo("estuf",$sql, 'S','-- Selecione --','', '', '','','N','estuf', '', '', 'Estado');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Partido:</td>
		<?
		$sql = "SELECT 
				  parid as codigo,
				  parnome as descricao
				FROM 
				  emenda.partido
				WHERE 
				  parstatus = 'A'
				ORDER BY parcodigo";
		?>
		<td><?=$db->monta_combo("parid",$sql, 'S','-- Selecione --','', '', '','','N','parid');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Nome:</td>
		<td><?=campo_texto( 'autnome', 'N', 'S', '', 60, 255, '', '','','','','id="autnome"'); ?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" onclick="pesquisar();">
		<input type="button" value="Novo Autor" name="btnNovo" id="btnNovo" onclick="novoAutor();"></th>
	</tr>
</table>
</form>
	<div id="lista">
		<?php
			echo "<script>pesquisar();</script>"
		?>
	</div>
</body>
<div id="erro"></div>
</html>
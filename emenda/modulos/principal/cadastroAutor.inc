<?php

if($_REQUEST['autid']){
	$sql = "SELECT 
			  autid,
			  autcod,
			  tpaid,
			  estuf,
			  parid,
			  autnome,
			  autnomeabreviado,
			  autemail,
			  autstatus
			FROM 
			  emenda.autor
			WHERE autid = ". $_REQUEST['autid'];
	
	$arAutor = $db->pegaLinha($sql);
	
	extract($arAutor);
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Cadastro de Autores', "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios" );
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

<input type="hidden" id="autid" name="autid" value="<?=$autid ?>">

<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Código:</td>
		<td><?=campo_texto( 'autcod', 'S', 'S', 'Codigo do Autor', 20, 15, '[#]', '','','','','id="autcod"'); ?></td>
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
		<td><?=$db->monta_combo("tpaid",$sql, 'S','-- Selecione --','', '', '','','S','tpaid', '', '', 'Tipo do Autor');?></td>
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
		<td><?=$db->monta_combo("estuf",$sql, 'S','-- Selecione --','', '', '','','S','estuf', '', '', 'Estado');?></td>
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
		<td><?=$db->monta_combo("parid",$sql, 'S','-- Selecione --','', '', '','','S','parid', '', '', 'Partido');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Nome:</td>
		<td><?=campo_texto( 'autnome', 'S', 'S', 'Nome do Autor', 60, 255, '', '','','','','id="autnome"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Nome Abreviado:</td>
		<td><?=campo_texto( 'autnomeabreviado', 'N', 'S', '', 60, 255, '', '','','','','id="autnomeabreviado"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">e-mail:</td>
		<td><?=campo_texto( 'autemail', 'N', 'S', '', 60, 255, '', '','','','','id="autemail"'); ?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="salvar();">
		<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="cancelar();"></th>
	</tr>
</table>
</form>
<div id="erro"></div>
</body>
</html>
<?php

if($_REQUEST['tpaid']){
	$sql = "SELECT 
			  tpaid,
			  tpanome,
			  gpaid,
			  tpastatus
			FROM 
			  emenda.tipoautor
			WHERE tpaid = ". $_REQUEST['tpaid'];
	
	$arTipoAutor = $db->pegaLinha($sql);
	
	extract($arTipoAutor);
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Cadastro de Tipo de Autores', "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios" );
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

<input type="hidden" id="tpaid" name="tpaid" value="<?=$tpaid; ?>">

<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Nome do Tipo:</td>
		<td><?=campo_texto( 'tpanome', 'S', 'S', 'Nome do Tipo', 60, 200, '', '','','','','id="tpanome"'); ?></td>
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
		<td><?=$db->monta_combo("gpaid",$sql, 'S','-- Selecione --','', '', '','','S','gpaid', '', '', 'Grupo');?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="salvarTipoAutor();">
		<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="cancelarTipoAutor();"></th>
	</tr>
</table>
</form>
<div id="erro"></div>
</body>
</html>
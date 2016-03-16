<?php

unset($_SESSION["emenda"]["resid"]);

if ($_REQUEST['pesquisaAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	pesquisaResponsavelAjax($_POST);
	exit;
}
if ($_REQUEST['excluirResponsavelAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	excluirResponsavelAjax($_POST['resid']);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Listar Responsável', "Filtro de Pesquisa" );

$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) ){
	$retorno = '';
} else {
	$retorno = 'disabled="disabled"';
}

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

<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita">Unidade:</td>
		<?
		$sql = "SELECT 
				  unicod as codigo,
				  unidsc as descricao
				FROM 
				  public.unidade
				WHERE 
				  unitpocod = 'U'
				  AND uniano = '2009'
				  AND unicod like '26%'
				  AND unistatus = 'A'";
		?>
		<td><?= $db->monta_combo("unicod",$sql, 'S','-- Selecione --','', '', '','','N','unicod','','','Unidade');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Responsável:</td>
		<td><?=campo_texto( 'resdsc', 'N', 'S', 'Responsável', 40, 100, '', '','','','','id="resdsc"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Assunto:</td>
		<td><?=campo_texto( 'resassunto', 'N', 'S', 'Tema', 40, 100, '', '','','','','id="restema"'); ?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" onclick="pesquisaResponsavel();">
		<input type="button" value="Novo Responsável" name="btnNovo" id="btnNovo" onclick="novoResponsavel();" <?=$retorno;?> /></th>
	</tr>
</table>
</form>
<div id="lista"><? pesquisaResponsavelAjax(''); ?></div>
</body>
</html>
<?php
include_once( APPRAIZ. "emenda/classes/Partido.class.inc" );

$parid = $_REQUEST['parid'];
$objPartido = new Partido($parid);

if($_POST['acao']){
	$arCampos = array('parid', 'parcodigo', 'parnome', 'parsigla', 'parstatus');
	$objPartido->popularObjeto($arCampos);
	
	if(!$parid){
		$objPartido->parstatus = 'A';		
	}
	
	$objPartido->salvar();
	$objPartido->commit();
	echo "<script>
				alert('Operação realizada com sucesso');
		      	window.location.href='emenda.php?modulo=principal/listaPartido&acao=A';
		     </script>";
		die;
		
}

if($parid){
	$parcodigo = $objPartido->parcodigo;
	$parnome = $objPartido->parnome;
	$parsigla = $objPartido->parsigla;
}


include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Cadastro de Partidos', "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios" );
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
		<td class="SubTituloDireita" style="width:39.5%;">Código:</td>
		<td><?=campo_texto( 'parcodigo', 'S', 'S', 'Código', 20, 15, '[#]', '','','','','id="parcodigo"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Nome:</td>
		<td><?=campo_texto( 'parnome', 'S', 'S', 'Nome', 60, 255, '', '','','','','id="parnome"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Sigla:</td>
		<td><?=campo_texto( 'parsigla', 'S', 'S', 'Sigla', 20, 10, '', '','','','','id="parsigla"'); ?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="salvarPartido();">
		<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="cancelarPartido();"></th>
	</tr>
</table>

<input type="hidden" name="acao" id="acao" value="">
<input type="hidden" name="parid" id="parid" value="<?=$parid; ?>">

</form>
</body>
<div id="erro"></div>
</html>
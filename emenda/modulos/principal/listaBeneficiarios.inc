<?php

if ($_REQUEST['pesquisaAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');	
	pesquisaBeneficiarioAjax($_POST);
	exit;
}

include_once( APPRAIZ. "emenda/classes/Beneficiario.class.inc" );

if($_POST['boExcluir']){
	$objBen = new Beneficiario( $_POST['benid'] );
	$objBen->benstatus = 'I';
	$objBen->salvar();
	$objBen->commit();
	$objBen->sucesso( "principal/listaBeneficiarios", "A" );
}
elseif($_POST['bennome']) {
	
	$sql = " select benid from emenda.beneficiario where bennome = '{$_POST["bennome"]}' and benstatus = 'I' ";	
	$benid = $_POST['benid'] ? $_POST['benid'] : $db->pegaUm( $sql );
	$objBen = new Beneficiario( $benid );
	//ver($_POST,$objBen);
	
	//if( !$benid ){
		$arDados = array( 'benid', 'bennome' );
		$objBen->popularObjeto( $arDados );
	//}
	$objBen->benstatus = 'A';
	
	if( $objBen->salvar() ){
		$objBen->commit();
		
		$objBen->sucesso( "principal/listaBeneficiarios", "A" );
	}
	else{
		$objBen->rollback();	
	}
}


include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( "Beneficiário", "" );

$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );
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
<script type="text/javascript" src="/includes/prototype.js"></script>
<body>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

<input type="hidden" value="" id="bennome" name="bennome">
<input type="hidden" value="" id="boExcluir" name="boExcluir">
<input type="hidden" value="<?=$benid; ?>" id="benid" name="benid">

<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Beneficiário:</td>
		<td><?=campo_texto( 'bennomeConsulta', 'N', 'S', '', 40, 250, '', '','','','','id="bennomeConsulta"'); ?></td>
	</tr>
	<tr>
		<?
		if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) ){
			$retorno = '';
		} else {
			$retorno = 'disabled="disabled"';
		}
		?>
		<th align="center" colspan="2"><input type="button" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" onclick="pesquisar();">
		<input type="button" value="Novo Beneficiário" name="btnNovo" id="btnNovo" onclick="novoBeneficiario();" <?=$retorno;?> /></th>
	</tr>
</table>
</form>
<div id="lista"><? pesquisaBeneficiarioAjax('') ?></div>
<script type="text/javascript">
	function novoBeneficiario(benid, bennome){
		if(!bennome){
			bennome = '';
		}
		if(!benid){
			benid = '';
		}
		var nome  = prompt( "Informe o Beneficiário!", bennome);
		if(nome){
			$('bennome').value = nome;
			$('benid').value = benid;
			document.formulario.submit();
		}
	}
	function excluiPrograma(benid){
		$('benid').value = benid;
		$('boExcluir').value = "true";
		document.formulario.submit();
	}
	function pesquisar(){
		$('loader-container').show();
		var myajax = new Ajax.Request('emenda.php?modulo=principal/listaBeneficiarios&acao=A', {
			        method:     'post',
			        parameters: '&pesquisaAjax=true&bennome='+$('bennomeConsulta').value,
			        asynchronous: false,
			        onComplete: function (res){
						$('lista').innerHTML = res.responseText;
			        }
			  });
		$('loader-container').hide();
	}
</script>
</body>
</html>
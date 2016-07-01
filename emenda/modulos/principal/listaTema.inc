<?php



if ($_REQUEST['pesquisaAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');	
	pesquisaTemaAjax($_POST['temnome']);
	exit;
}

include_once( APPRAIZ. "emenda/classes/Tema.class.inc" );

if($_POST['boExcluir']){
	$objTema = new Tema( $_POST['temid'] );
	$objTema->temstatus = 'I';
	$objTema->salvar();
	$objTema->commit();
	$objTema->sucesso( "principal/listaTema", "A" );
}
elseif($_POST['temnome']){	
	
	$sql = " select temid from emenda.tema where temnome = '{$_POST["temnome"]}' and temstatus = 'I' ";
	
	$temid = $db->pegaUm( $sql );
	$objTema = new Tema($temid );
	
	if(!$temid){
		$temid = $_POST['temid'];
		
		$arDados = array( 'temid', 'temnome' );	
		$objTema->popularObjeto( $arDados );
	}
	$objTema->temstatus = 'A';
	
	if( $objTema->salvar() ){
		$objTema->commit();
		
		$objTema->sucesso( "principal/listaTema", "A" );
	}
	else{
		$objTema->rollback();	
	}
}


include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( "Temas", "" );

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
<script type="text/javascript" src="/includes/prototype.js"></script>
<body>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

<input type="hidden" value="" id="temnome" name="temnome">
<input type="hidden" value="" id="boExcluir" name="boExcluir">
<input type="hidden" value="<?=$temid; ?>" id="temid" name="temid">

<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Tema:</td>
		<td><?=campo_texto( 'temnomeConsulta', 'N', 'S', '', 40, 250, '', '','','','','id="temnomeConsulta"'); ?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" onclick="pesquisar();">
		<input type="button" value="Novo Tema" name="btnNovo" id="btnNovo" onclick="novoTema();" <?=$retorno;?> /></th>
	</tr>
</table>
</form>
<div id="lista"><?php pesquisaTemaAjax(''); ?></div>
<script type="text/javascript">
	function novoTema(temid, temnome){
		if(!temnome){
			temnome = '';
			temnome = $('temnomeConsulta').value;
		}
		
		if(!temid){
			temid = '';
		}
		var nome  = prompt( "Informe o Tema!", temnome);
		if(nome){
			$('temnome').value = nome;
			$('temid').value = temid;
			document.formulario.submit();
		}
	}
	function excluiTema(temid){
		$('temid').value = temid;
		$('boExcluir').value = "true";
		document.formulario.submit();
	}
	function pesquisar(){
		$('loader-container').show();
		var myajax = new Ajax.Request('emenda.php?modulo=principal/listaTema&acao=A', {
			        method:     'post',
			        parameters: '&pesquisaAjax=true&temnome='+$('temnomeConsulta').value,
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
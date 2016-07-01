<?php

if ( $_REQUEST['pesquisaGrupo'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	pesquisaGrupoPregao( $_REQUEST['grunome'] );
	exit;
}

if ( $_REQUEST['excluiGrupo'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	excluiGrupoPregao( $_REQUEST['gruid'] );
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Pesquisa Grupo', '' );
?>

<style>
@CHARSET "ISO-8859-1";

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 55%;
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
<body>

<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="frmPesGrupoPregao" name="frmPesGrupoPregao" action="" method="post" enctype="multipart/form-data" >

<table id="tblPesGrupoPregao" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Nome do Grupo:</td>
		<td><?=campo_texto( 'grunome', 'N', 'S', '', 40, 50, '', '','','','','id="grunome"'); ?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Pesquisar" name="btnPesquisa" id="btnPesquisa" onclick="pesquisaGrupo();">
		<input type="button" value="Novo Grupo" name="btnNovo" id="btnNovo" onclick="cadastroItem();"></th>
	</tr>
</table>
</form>
<div id="lista" style="display: ''">
<?php
	pesquisaGrupoPregao('');
	?>
	
</div>
<script type="text/javascript">
function pesquisaGrupo(){
	$('loader-container').show();
	var myAjax = new Ajax.Request('rehuf.php?modulo=principal/listaGrupoPregao&acao=A',{
							method:		'post',
							parameters: '&pesquisaGrupo=true&grunome='+$('grunome').value,
							asynchronous: false,
							onComplete: function(res){
								$('lista').innerHTML = res.responseText;
							}						
						});
	$('loader-container').hide();
}

function cadastroItem(){
	window.location.href = "rehuf.php?modulo=principal/cadastroGrupoPregao&acao=A";
}

function excluiGrupo(gruid){
	if(confirm("Tem certeza que deseja excluir este registro?")){
		$('loader-container').show();
		var myAjax = new Ajax.Request('rehuf.php?modulo=principal/listaGrupoPregao&acao=A',{
								method:		'post',
								parameters: '&excluiGrupo=true&gruid='+gruid,
								asynchronous: false,
								onComplete: function(res){
									//$('erro').innerHTML = res.responseText;
									if( res.responseText == 1 ){
										alert("Operação realizada com sucesso!");
										pesquisaGrupo();
									}else{
										alert('Operação não realizada, entre em contato com administrador do sistema!');	
									}
								}						
							});
		$('loader-container').hide();
	}
}



</script>
</body>
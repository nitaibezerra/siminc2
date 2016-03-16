<?php

if ( $_REQUEST['pesquisaItem'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	pesquisaItemPregao( $_REQUEST['itecatmat'], $_REQUEST['itedescricao'], $_REQUEST['iteapresentacao'], $_REQUEST['gruid'] );
	exit;
}

if ( $_REQUEST['excluiItem'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	excluiItemPregao( $_REQUEST['iteid'] );
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Pesquisa Itens', '' );
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
		<td class="SubTituloDireita" style="width:39.5%;">CATMAT:</td>
		<td><?=campo_texto( 'itecatmat', 'N', 'S', '', 40, 10, '[#]', '','','','','id="itecatmat"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Descrição:</td>
		<td><?=campo_texto( 'itedescricao', 'N', 'S', '', 40, 1000, '', '','','','','id="itedescricao"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Apresentação:</td>
		<td><?=campo_texto( 'iteapresentacao', 'N', 'S', '', 40, 100, '', '','','','','id="iteapresentacao"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Grupo:</td>
		<?php
			$sql = "SELECT 
					  gruid as codigo,
					  grunome as descricao
					FROM
					  rehuf.grupoitens
					WHERE
					  grustatus = 'A'";
		?>
		<td><?= $db->monta_combo("gruid",$sql, 'S','-- Selecione um grupo --','', '', '',250,'N','gruid');?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Pesquisar" name="btnPesquisa" id="btnPesquisa" onclick="pesquisaItem();">
		<input type="button" value="Novo Item" name="btnNovo" id="btnNovo" onclick="cadastroItens();"></th>
	</tr>
</table>
</form>
<div id="lista" style="display: ''">
<?php
	pesquisaItemPregao('', '', '', '');
	?>
	
</div>
<script type="text/javascript"><!--
function pesquisaItem(){
	var consulta = true;
	var itecatmat = "";
	var itedescricao = "";
	var iteapresentacao = "";
	
	if(trim($('itedescricao').value).length > 0 && trim($('itedescricao').value).length < 5)
	{
		alert('Consulta pela descrição deverá ser realizada com mais de 5 caracteres');
		consulta = false;
	}else if(trim($('iteapresentacao').value).length > 0 && trim($('iteapresentacao').value).length < 5)
	{
		alert('Consulta pela apresentação deverá ser realizada com mais de 5 caracteres');
		consulta = false;
	}
	
	itecatmat = trim($('itecatmat').value);
	itedescricao = trim($('itedescricao').value);
	iteapresentacao = trim($('iteapresentacao').value);

	if(consulta == true){	
		$('loader-container').show();
		var myAjax = new Ajax.Request('rehuf.php?modulo=principal/listaItemPregao&acao=A',{
								method:		'post',
								parameters: '&pesquisaItem=true&itecatmat='+itecatmat+
															'&itedescricao='+itedescricao+
															'&iteapresentacao='+iteapresentacao+
															'&gruid='+$('gruid').value,
								asynchronous: false,
								onComplete: function(res){
									$('lista').innerHTML = res.responseText;
								}						
							});
		$('loader-container').hide();
	}
}

function cadastroItens(){
	window.location.href = "rehuf.php?modulo=principal/cadastroItensPregao&acao=A";
}

function excluiItem(iteid){
	if(confirm("Tem certeza que deseja excluir este registro?")){
		$('loader-container').show();
		var myAjax = new Ajax.Request('rehuf.php?modulo=principal/listaItemPregao&acao=A',{
								method:		'post',
								parameters: '&excluiItem=true&iteid='+iteid,
								asynchronous: false,
								onComplete: function(res){
									//$('erro').innerHTML = res.responseText;
									if( res.responseText == 1 ){
										alert("Operação realizada com sucesso!");
										pesquisaItem();
									}else{
										alert('Operação não realizada, entre em contato com administrador do sistema!');	
									}
								}						
							});
		$('loader-container').hide();
	}
}



--></script>
</body>
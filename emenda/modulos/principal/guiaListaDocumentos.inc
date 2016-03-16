<?php
if( $_POST['requisicao'] == 'excluir' ){
	$sql = "UPDATE emenda.guiadocumento SET gdostatus = 'I' WHERE gdoid = ".$_POST['gdoid'];
	$db->executar( $sql );
	$db->commit();
	$db->sucesso('principal/documentosAdmPta');
}
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Documentos Obrigatórios", "Lista de Documentos Necessário para o Plano de Trabalho" );
extract( $_POST );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

<input type="hidden" name="requisicao" id="requisicao" value="">
<input type="hidden" name="gdoid" id="gdoid" value="">

<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:29.5%;">Nome:</td>
		<td><?=campo_texto( 'gdonome', 'N', 'S', '', 80, 250, '', '','','','','id="gdonome"'); ?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="submit" value="Pesquisar" name="btnPesquisar" id="btnPesquisar">
		<input type="button" value="Listar Todos" name="btnLimpar" id="btnLimpar"/>
		<input type="button" value="Cadastrar Documento" name="btnCadastrar" id="btnCadastrar" onclick="cadastrarDocumento('');"/></th>
	</tr>
</table>
	
<script type="text/javascript">	
	$(document).ready(function(){
		$('#btnPesquisar').click(function(){
			if( $('#gdonome').val() == '' ){
				alert('Informe o nome para realizar a pesquisa.');
				return false;
			}
			$('#formulario').submit();
		});
		
		$('#btnLimpar').click(function(){
			window.location.href = window.location;
		});		
		
	});
	function cadastrarDocumento(gdoid){
		window.location.href = 'emenda.php?modulo=principal/guiaCadastrarDocumentos&acao=A&gdoid='+gdoid;
	}
	
	function excluirDocumento(gdoid){
		$('#requisicao').val('excluir');
		$('#gdoid').val(gdoid);
		$('#formulario').submit();
	}
</script>
</form>
<?php
carregarDocumento($_POST);

function carregarDocumento( $dados ){
	global $db;
	
	$filtro = !empty($dados['gdonome']) ? " and lower(removeacento(gdonome)) ilike lower(removeacento('%".$dados['gdonome']."%'))" : ''; 
	
	$acao = "( '<center><img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"cadastrarDocumento('|| gdoid ||');\" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
	    		   '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluirDocumento('|| gdoid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";
	
	$sql = "SELECT 
			  $acao as acao,
			  gdonome
			FROM 
			  emenda.guiadocumento
			WHERE
				gdostatus = 'A' $filtro";
				
	$cabecalho = array("Opção", "Documento");

	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '', '', '','');
}
?>
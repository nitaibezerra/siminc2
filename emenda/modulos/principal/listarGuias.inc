<?php
if( $_POST['requisicao'] == 'excluir' ){
	$sql = "UPDATE emenda.guia SET guistatus = 'I' WHERE guiid = ".$_POST['guiid'];
	$db->executar( $sql );
	/*$db->executar("DELETE FROM emenda.guia_guiadocumento WHERE guiid = ".$_POST['guiid']);*/
	$db->commit();
	$db->sucesso('principal/listarGuias');
}

if( $_POST['requisicao'] == 'salvar' ){
	extract($_POST);
	
	$guijustificativa = (($_REQUEST["guijustificativa"] != "") ? stripcslashes(pg_escape_string( str_replace( "'", '"', $_REQUEST["guijustificativa"]))) : 'NULL');
	
	if( !$guiid ){
		$sql = "INSERT INTO emenda.guia(guinome, guijustificativa) 
				VALUES ('$guinome', '$guijustificativa') RETURNING guiid";
		$guiid = $db->pegaUm( $sql );
	} else {
		$sql = "UPDATE emenda.guia SET 
				  	guinome = '$guinome',
				  	guijustificativa = '$guijustificativa'
				WHERE 
  					guiid = $guiid";
  		$db->executar( $sql );
	}
	
	if( $db->commit() ){
		$db->sucesso('principal/cadastrarGuiaIniciativa', "&guiid=$guiid");
	} else {
		echo "<script>
				alert('Falha na operação');
			</script>";
	}
}
unset($_SESSION['emenda']['ginid']);
unset($_SESSION['emenda']['guiid']);
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Guias", "Lista de Guias" );
extract( $_POST );

if($_REQUEST['guiid']){
	$sql = "SELECT
				guinome,
  				guijustificativa
			FROM 
  				emenda.guia WHERE guiid = ".$_REQUEST['guiid'];
	$arrGuia = $db->pegaLinha( $sql );
	$arrGuia = $arrGuia ? $arrGuia : array();
	extract($arrGuia);
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

<input type="hidden" name="guiid" id="guiid" value="<?=$_REQUEST['guiid'] ?>">
<input type="hidden" name="ginid" id="ginid" value="">
<input type="hidden" name="requisicao" id="requisicao" value="">

<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:29.5%;">Guia:</td>
		<td><?=campo_texto( 'guinome', 'S', 'S', '', 75, 250, '', '','','','','id="guinome"'); ?>
			<input type="submit" value="Pesquisar" name="btnPesquisar" id="btnPesquisar"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Justificativa:</td>
		<td><?=campo_textarea('guijustificativa', 'S', 'S', 'Justificativa', 80, 10, 15000, '', '', '', '', 'Justificativa');?></td>
	</tr>
	<tr>
		<th align="center" colspan="2">
			<input type="submit" value="Salvar Guia" name="btnSalvar" id="btnSalvar">
			<input type="button" value="Mostrar Todos" name="btnTodos" id="btnTodos">
			</th>
	</tr>
</table>
</form>
<?php 

	$filtro = !empty($_POST['guinome']) ? " and lower(removeacento(guinome)) ilike lower(removeacento('%".$_POST['guinome']."%'))" : '';
	 
	$acao = "( '<center>
					<img src=\"/imagens/editar_nome.gif \" style=\"cursor: pointer\" onclick=\"AlterarPlanoGuia('|| guiid ||');\" border=0 alt=\"Ir\" title=\"Alterar Dados da Guia\">
					<img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"cadastrarGuia('|| guiid ||');\" border=0 alt=\"Ir\" title=\"Alterar\"> ' ||
	    		   '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluirGuia('|| guiid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";
	
	$sql = "SELECT 
			  	$acao as acao,
			  	guinome
			FROM 
			  	emenda.guia
			WHERE
				guistatus = 'A' $filtro";
				
	$cabecalho = array("Opção", "Guia");

	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '', '', '','');
?>
<script type="text/javascript">
	
	$(document).ready(function(){
		
		if( $('#guiid').val() ) $('#tblformIniciativa').css('display', 'block');
	
		$('#btnPesquisar').click(function(){
			if( $('#guinome').val() == '' ){
				alert('Informe o nome para realizar a pesquisa.');
				return false;
			}
			$('#formulario').submit();
		});
		
		$('#btnTodos').click(function(){
			window.location.href = 'emenda.php?modulo=principal/listarGuias&acao=A';
		});
		
		$('#btnSalvar').click(function(){
	
			if( $('#guinome').val() == '' ){
				alert('O campo "Guia" é obrigatório.');
				$('#guinome').focus();
				return false;
			}
			if( $('#guijustificativa').val() == '' ){
				alert('O campo "Justificativa" é obrigatório.');
				$('#guijustificativa').focus();
				return false;
			}
			
			$('#requisicao').val('salvar');
			$('#formulario').submit();
		});
		
	});
	
	function cadastrarGuia(guiid){
		window.location.href = 'emenda.php?modulo=principal/listarGuias&acao=A&guiid='+guiid;
	}
	function AlterarPlanoGuia(guiid){
		window.location.href = 'emenda.php?modulo=principal/cadastrarGuiaIniciativa&acao=A&guiid='+guiid;
	}
	
	function excluirGuia(guiid){
		$('#requisicao').val('excluir');
		$('#guiid').val(guiid);
		$('#formulario').submit();
	}
</script>
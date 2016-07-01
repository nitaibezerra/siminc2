<?php
include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';


if($_POST["submeter"] == 'salvar' ) {
	if( !empty($_POST['objid']) ){
		$status = ( !empty($_POST['objstatus']) ? ", objstatus = '".strtoupper($_POST['objstatus'])."' " : '' );
		
		$sql = "UPDATE par.objeto SET objdsc = '{$_POST['objdsc']}', objtpobj = '{$_POST['objtpobj']}' $status WHERE objid = ".$_POST['objid'];
		$db->executar($sql);
	} else {
		$sql = "INSERT INTO par.objeto( objdsc, objtpobj ) 
				VALUES ('{$_POST['objdsc']}', '{$_POST['objtpobj']}')";
		$db->executar($sql);
	}
	$db->commit(); 
	$db->sucesso('sistema/tabelasapoio/objetoPar');
} elseif($_POST["submeter"] == 'excluir') {
	$sql = "SELECT omcid FROM par.objetominuta where objid = ".$_POST['objid'];
	$omcid = $db->pegaUm( $sql );
	
	if( empty($omcid) ){
		$sql = "UPDATE par.objeto SET objstatus = 'I' WHERE objid = ".$_POST['objid'];
		$db->executar($sql);
		$db->commit();
		echo "<script>
				alert('Objeto de convênio inativado com sucesso!');
				window.location.href = 'par.php?modulo=sistema/tabelasapoio/objetoPar&acao=A';
			  </script>";
		die;
	} else {
		echo "<script>
				alert('Não é possivel realizar a exclusão desse objeto porque ele está vinculado a uma minuta convenio');
				window.location.href = 'par.php?modulo=sistema/tabelasapoio/objetoPar&acao=A';
			  </script>";
		die;
	}
}

if( !empty($_GET['objid']) ){
	$sql = "SELECT objid, objdsc, objstatus, objtpobj FROM par.objeto WHERE objid = ".$_GET['objid'];;
	$arObjeto = $db->pegaLinha( $sql );
	extract($arObjeto);
}

$db->cria_aba( $abacod_tela, $url, '' );
//$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );
monta_titulo('Administração de Objeto', '');
?>
<form id="formulario" method="post" action="">
<input type="hidden" id="objid" name="objid" value="<?=$objid; ?>" />
<input type="hidden" id="submeter" name="submeter" value="" />
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Descrição:</b></td>
		<td><?=campo_textarea('objdsc', 'S', 'S', 'Descrição do objeto', 98, 5, 500, '', '', '', '', 'Descrição do objeto');?></td>
	</tr>
	<?if( $objstatus == 'I' ){ ?>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Status:</b></td>
		<td><input type="radio" value="A" id="objstatus" name="objstatus" <? /*if($objstatus == "A") { echo "checked"; }*/ ?> /> Ativo
			<input type="radio" value="I" id="objstatus" name="objstatus" checked="checked" <? /*if($objstatus == "I") { echo "checked"; }*/ ?> /> Inativo</td>
	</tr>
	<?} ?>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Tipo de Objeto:</b></td>
		<td><input type="radio" value="O" id="objtpobj" name="objtpobj" <? if($objtpobj == "") { echo "checked"; } ?> <? if($objtpobj == "O") { echo "checked"; } ?> /> Original
			<input type="radio" value="A" id="objtpobj" name="objtpobj" <? if($objtpobj == "A") { echo "checked"; } ?> /> Termo Aditivo</td>
	</tr>
	<tr>
		<td bgcolor="#c0c0c0"></td>
		<td align="left" bgcolor="#c0c0c0">
			<input type="button" id="bt_salvar" value="Salvar" onclick="javascript:salvarObjeto();" <?=$retorno ?> />
			&nbsp;
			<input type="button" id="bt_cancel" value="Cancelar" onclick="javascript:cancelar();"/>
		</td>
	</tr>
</table>
</form>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td align="center" colspan="2"><b>Lista de Objeto Convênio</b></td>
	</tr>
</table>
<script>
	function salvarObjeto(){
		document.getElementById('submeter').value = 'salvar';
		document.getElementById('formulario').submit();
	}
	function alterarObjeto(objid){
		window.location.href = 'par.php?modulo=sistema/tabelasapoio/objetoPar&acao=A&objid='+objid;
	}
	function excluirObjeto(objid){
		if(confirm('Deseja realmente inativar este objeto de convênio?')) {
			document.getElementById('objid').value = objid;
			document.getElementById('submeter').value = 'excluir';
			document.getElementById('formulario').submit();
		}
	}
	function cancelar(){
		window.location.href = 'par.php?modulo=sistema/tabelasapoio/objetoPar&acao=A';
	}
</script>
<?
//if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) ){
	$acao = "'<a href=\"#\" onclick=\"alterarObjeto(' || objid || ');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
  			  <a href=\"#\" onclick=\"excluirObjeto(' || objid || ');\" title=\"Status\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'";
/*} else {
	$acao = "'<a href=\"#\" onclick=\"alterarObjeto(' || objid || ');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
  			  <a href=\"#\" title=\"Status\"><img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'";
}*/
 $sql = "SELECT '<center>'||".$acao."||'</center>' as acao, objid, objdsc, 
 			case when objstatus = 'A' then 'Ativo'
 			else 'Inativo' end as ativo,
 			case when objtpobj = 'O' then 'Original'
 			else 'Termo Aditivo' end as tipo
 		FROM par.objeto";
 $cabecalho = array("ações", "Código", "Descrição", "Status", "Tipo");
 $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center');
?>
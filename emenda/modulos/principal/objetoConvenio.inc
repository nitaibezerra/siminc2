<?php
include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';

if($_POST["submeter"] == 'salvar' ) {
	if( !empty($_POST['obcid']) ){
		$status = ( !empty($_POST['obcstatus']) ? ", obcstatus = '".strtoupper($_POST['obcstatus'])."' " : '' );
		
		$sql = "UPDATE emenda.objetoconvenio SET obcdsc = '{$_POST['obcdsc']}', obctpobj = '{$_POST['obctpobj']}' $status WHERE obcid = ".$_POST['obcid'];
		$db->executar($sql);
	} else {
		$sql = "INSERT INTO emenda.objetoconvenio( obcdsc, obctpobj ) 
				VALUES ('{$_POST['obcdsc']}', '{$_POST['obctpobj']}')";
		$db->executar($sql);
	}
	$db->commit();
	$db->sucesso('principal/objetoConvenio');
} elseif($_POST["submeter"] == 'excluir') {
	$sql = "SELECT omcid FROM emenda.objetominutaconvenio where obcid = ".$_POST['obcid'];
	$omcid = $db->pegaUm( $sql );
	
	if( empty($omcid) ){
		$sql = "UPDATE emenda.objetoconvenio SET obcstatus = 'I' WHERE obcid = ".$_POST['obcid'];
		$db->executar($sql);
		$db->commit();
		echo "<script>
				alert('Objeto de convênio inativado com sucesso!');
				window.location.href = 'emenda.php?modulo=principal/objetoConvenio&acao=A';
			  </script>";
		die;
	} else {
		echo "<script>
				alert('Não é possivel realizar a exclusão desse objeto porque ele está vinculado a uma minuta convenio');
				window.location.href = 'emenda.php?modulo=principal/objetoConvenio&acao=A';
			  </script>";
		die;
	}
}

if( !empty($_GET['obcid']) ){
	$sql = "SELECT obcid, obcdsc, obcstatus, obctpobj FROM emenda.objetoconvenio WHERE obcid = ".$_GET['obcid'];;
	$arObjeto = $db->pegaLinha( $sql );
	extract($arObjeto);
}

$db->cria_aba( $abacod_tela, $url, '' );
$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );
monta_titulo('Administração de Objeto Convênio', '');
?>
<form id="formulario" method="post" action="">
<input type="hidden" id="obcid" name="obcid" value="<?=$obcid; ?>" />
<input type="hidden" id="submeter" name="submeter" value="" />
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Descrição:</b></td>
		<td><?=campo_textarea('obcdsc', 'S', 'S', 'Descrição do objeto convênio', 98, 5, 500, '', '', '', '', 'Descrição do objeto convênio');?></td>
	</tr>
	<?if( $obcstatus == 'I' ){ ?>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Status:</b></td>
		<td><input type="radio" value="A" id="obcstatus" name="obcstatus" <? /*if($obcstatus == "A") { echo "checked"; }*/ ?> /> Ativo
			<input type="radio" value="I" id="obcstatus" name="obcstatus" checked="checked" <? /*if($obcstatus == "I") { echo "checked"; }*/ ?> /> Inativo</td>
	</tr>
	<?} ?>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Tipo de Objeto:</b></td>
		<td><input type="radio" value="O" id="obctpobj" name="obctpobj" <? if($obctpobj == "") { echo "checked"; } ?> <? if($obctpobj == "O") { echo "checked"; } ?> /> Original
			<input type="radio" value="A" id="obctpobj" name="obctpobj" <? if($obctpobj == "A") { echo "checked"; } ?> /> Termo Aditivo</td>
	</tr>
	<tr>
		<?
		if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) ){
			$retorno = '';
			/*if(!possuiPermissao($categoria)) {
				$retorno = 'disabled="disabled"';
			}*/
		} else {
			$retorno = 'disabled="disabled"';
		}
		?>
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
	function alterarObjeto(obcid){
		window.location.href = 'emenda.php?modulo=principal/objetoConvenio&acao=A&obcid='+obcid;
	}
	function excluirObjeto(obcid){
		if(confirm('Deseja realmente inativar este objeto de convênio?')) {
			document.getElementById('obcid').value = obcid;
			document.getElementById('submeter').value = 'excluir';
			document.getElementById('formulario').submit();
		}
	}
	function cancelar(){
		window.location.href = 'emenda.php?modulo=principal/objetoConvenio&acao=A';
	}
</script>
<?
if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) ){
	$acao = "'<a href=\"#\" onclick=\"alterarObjeto(' || obcid || ');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
  			  <a href=\"#\" onclick=\"excluirObjeto(' || obcid || ');\" title=\"Status\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'";
} else {
	$acao = "'<a href=\"#\" onclick=\"alterarObjeto(' || obcid || ');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
  			  <a href=\"#\" title=\"Status\"><img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'";
}
 $sql = "SELECT '<center>'||".$acao."||'</center>' as acao, obcid, obcdsc, 
 			case when obcstatus = 'A' then 'Ativo'
 			else 'Inativo' end as ativo,
 			case when obctpobj = 'O' then 'Original'
 			else 'Termo Aditivo' end as tipo
 		FROM emenda.objetoconvenio";
 $cabecalho = array("ações", "Código", "Descrição", "Status", "Tipo");
 $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center');
?>
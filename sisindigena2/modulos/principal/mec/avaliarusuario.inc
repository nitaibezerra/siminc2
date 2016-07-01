<script>
function avaliarCoordenadorIES(uncid) {
	window.location='sisindigena2.php?modulo=principal/mec/mec&acao=A&aba=avaliarusuario&uncid='+uncid;
}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">
	<?
	$sql = "SELECT uncid as codigo, uninome as descricao FROM sisindigena2.universidadecadastro un INNER JOIN sisindigena2.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	$db->monta_combo('uncid', $sql, 'S', 'Selecione', 'avaliarCoordenadorIES', '', '', '', 'N', '','',$_REQUEST['uncid']);
	?>
	</td>
</tr>
</table>
<?

if($_REQUEST['uncid']) :

	$_SESSION['sisindigena2']['mec']['uncid'] = $_REQUEST['uncid'];
	$_SESSION['sisindigena2']['mec']['iusd']  = IUS_AVALIADOR_MEC;
	
	$sql_avaliacao = sqlAvaliacaoMEC(array("uncid"=>$_SESSION['sisindigena2']['mec']['uncid'],"iusd"=>$_SESSION['sisindigena2']['mec']['iusd'],"fpbid"=>$_REQUEST['fpbid']));
	
	
	
	$pflcod_avaliador = PFL_EQUIPEMEC;
	$sis 	  		= "mec";
	$goto_ant 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";
	if($_REQUEST['fpbid']) $goto_sal = "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario&fpbid=".$_REQUEST['fpbid'];
	else $goto_sal = false;
	$goto_pro 		= "sisindigena2.php?modulo=principal/mec/mec&acao=A&aba=gerenciarturmas";
	
	
	include_once APPRAIZ_SISINDIGENA."avaliarequipe.inc";

endif;

?>
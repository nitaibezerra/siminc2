<script>
function avaliarCoordenadorIES(uncid) {
	window.location='sisindigena2.php?modulo=principal/mec/mec&acao=A&aba=gerenciarusuario&uncid='+uncid;
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
	
	$sql_equipe = sqlEquipeMEC(array('uncid' => $_SESSION['sisindigena2']['mec']['uncid']));
	
	$pflcod_gerenciador = PFL_EQUIPEMEC;
	$mensagemalert	= "Dados do MEC IES salvos com sucesso";
	$sis 	  		= "mec";
	$goto_ant 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=configuracoes";
	$goto_sal 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";
	$goto_pro 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario";
	
	
	include_once APPRAIZ_SISINDIGENA."gerenciarequipe.inc";

endif;

?>
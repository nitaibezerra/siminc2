<script>
function avaliarCoordenadorIES(uncid) {
	window.location='sismedio.php?modulo=principal/mec/mec&acao=A&aba=gerenciarusuario&uncid='+uncid;
}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">
	<?
	$sql = "SELECT uncid as codigo, uninome as descricao FROM sismedio.universidadecadastro un INNER JOIN sismedio.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	$db->monta_combo('uncid', $sql, 'S', 'Selecione', 'avaliarCoordenadorIES', '', '', '', 'N', '','',$_REQUEST['uncid']);
	?>
	</td>
</tr>
</table>
<?

if($_REQUEST['uncid']) :

	$_SESSION['sismedio']['mec']['uncid'] = $_REQUEST['uncid'];
	$_SESSION['sismedio']['mec']['iusd']  = IUS_AVALIADOR_MEC;
	
	$sql_equipe = sqlEquipeMEC(array('uncid' => $_SESSION['sismedio']['mec']['uncid']));
	
	$pflcod_gerenciador = PFL_EQUIPEMEC;
	$mensagemalert	= "Dados do MEC IES salvos com sucesso";
	$sis 	  		= "mec";
	$goto_ant 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=configuracoes";
	$goto_sal 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";
	$goto_pro 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario";
	
	
	include_once APPRAIZ_SISMEDIO."gerenciarequipe.inc";

endif;

?>
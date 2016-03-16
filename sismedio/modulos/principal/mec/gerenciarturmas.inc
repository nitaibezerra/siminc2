<script>
function gerenciarturmas(uncid) {
	window.location='sismedio.php?modulo=principal/mec/mec&acao=A&aba=gerenciarturmas&uncid='+uncid;
}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">
	<?
	$sql = "SELECT uncid as codigo, uninome as descricao FROM sismedio.universidadecadastro un INNER JOIN sismedio.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	$db->monta_combo('uncid', $sql, 'S', 'Selecione', 'gerenciarturmas', '', '', '', 'N', '','',$_REQUEST['uncid']);
	?>
	</td>
</tr>
</table>
<?

if($_REQUEST['uncid']) :

	$_SESSION['sismedio']['mec']['uncid'] = $_REQUEST['uncid'];
	$_SESSION['sismedio']['mec']['iusd']  = IUS_AVALIADOR_MEC;
	
	$pflcod_gerenciador = PFL_EQUIPEMEC;
	$sis 	  		= "mec";
	$goto_ant 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarequipe";
	$goto_pro 		= false;
	
	$turformadores = true;
	$turorientadores = true;
	
	include_once APPRAIZ_SISMEDIO."gerenciarturmas.inc";

endif;

?>
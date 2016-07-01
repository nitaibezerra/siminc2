<script>
function gerenciarturmas(uncid) {
	window.location='sispacto2.php?modulo=principal/mec/mec&acao=A&aba=gerenciarturmas&uncid='+uncid;
}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">
	<?
	$sql = "SELECT uncid as codigo, uninome as descricao FROM sispacto2.universidadecadastro un INNER JOIN sispacto2.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	$db->monta_combo('uncid', $sql, 'S', 'Selecione', 'gerenciarturmas', '', '', '', 'N', '','',$_REQUEST['uncid']);
	?>
	</td>
</tr>
</table>
<?

if($_REQUEST['uncid']) :

	$_SESSION['sispacto']['mec']['uncid'] = $_REQUEST['uncid'];
	$_SESSION['sispacto']['mec']['iusd']  = IUS_AVALIADOR_MEC;
	
	$pflcod_gerenciador = PFL_EQUIPEMEC;
	$sis 	  		= "mec";
	$goto_ant 		= "sispacto2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarequipe";
	$goto_pro 		= false;
	
	$turformadores = true;
	$turorientadores = true;
	
	include_once APPRAIZ_SISPACTO."gerenciarturmas.inc";

endif;

?>
<script>
function avaliarCoordenadorIES(uncid) {
	window.location='sispacto3.php?modulo=principal/mec/mec&acao=A&aba=gerenciarusuario&uncid='+uncid;
}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">
	<?
	$sql = "SELECT uncid as codigo, uninome as descricao FROM sispacto3.universidadecadastro un INNER JOIN sispacto3.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	$db->monta_combo('uncid', $sql, 'S', 'Selecione', 'avaliarCoordenadorIES', '', '', '', 'N', '','',$_REQUEST['uncid']);
	?>
	</td>
</tr>
</table>
<?

$exibirqtdgerenciar = 500;

if($_REQUEST['uncid']) :

	$_SESSION['sispacto3']['mec']['uncid'] = $_REQUEST['uncid'];
	$_SESSION['sispacto3']['mec']['iusd']  = IUS_AVALIADOR_MEC;
	
	$sql_equipe = sqlEquipeMEC(array('uncid' => $_SESSION['sispacto3']['mec']['uncid']));
	
	$pflcod_gerenciador = PFL_EQUIPEMEC;
	$mensagemalert	= "Dados do MEC IES salvos com sucesso";
	$sis 	  		= "mec";
	
	include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

endif;

?>
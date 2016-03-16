<?
include APPRAIZ ."includes/workflow.php";
include "_funcoes_universidade.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>';

echo "<br>";

?>

<script>
function avaliarCoordenadorIES(uncid) {
	window.location='sismedio.php?modulo=principal/universidade/turmas&acao=A&uncid='+uncid;
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

	$_SESSION['sismedio']['universidade']['uncid'] = $_REQUEST['uncid'];
	
	
	include_once APPRAIZ_SISMEDIO."composicaoturmas.inc";

endif;

?>
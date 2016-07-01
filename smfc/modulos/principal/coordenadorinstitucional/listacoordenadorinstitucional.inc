<?
include APPRAIZ ."includes/workflow.php";
include "_funcoes_coordenadorinstitucional.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
	
include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/smfc.js"></script>';

echo "<br>";

?>
<script>
function carregarUsuario(requisicao,iusid) {
	window.location=window.location+'&requisicao='+requisicao+'&iusid='+iusid;
}
</script>
<?


$sql = "SELECT '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"carregarUsuario(\'carregarCoordenadorInstitucional\','||i.iusid||')\">' as acao, iuscpf, iusnome FROM smfc.identificacaousuario i 
		INNER JOIN smfc.tipoperfil t ON t.iusid = i.iusid 
		WHERE iusstatus='A' AND t.pflcod='".PFL_COORDENADOR_INSTITUCIONAL."'";


$cabecalho = array("&nbsp;","CPF","Nome");
$db->monta_lista($sql,$cabecalho,100,10,'N','center','N','formulario','','',null,array('ordena'=>false));


?>
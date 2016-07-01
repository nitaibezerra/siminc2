<?
include APPRAIZ ."includes/workflow.php";
include "_funcoes_mec.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

	
include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/smfc.js"></script>';

echo "<br>";

// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);




$subtitulo = "<table class=listagem width=100%>
			 <tr>
				<td class=\"SubTitulodireita\" width=\"10%\"><b>Nome:</b></td>
				<td>".$_SESSION['usunome']."</td>
				<td class=\"SubTitulodireita\" width=\"10%\"><b>Perfil:</b></td>
				<td>Equipe MEC</td>
			 </tr>
			 </table>";

	

monta_titulo( "MEC", $subtitulo);

if(!$_REQUEST['aba']) $_REQUEST['aba'] = "principal";

$abaativa = "/smfc/smfc.php?modulo=principal/mec/meccadastro&acao=A&aba=".$_REQUEST['aba'];

montaAbasSmfc('meccadastro', $abaativa);

include $_REQUEST['aba'].".inc";

?>
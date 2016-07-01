<?
include APPRAIZ ."includes/workflow.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

	
include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sisindigena2.js"></script>';

echo "<br>";


// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);




$subtitulo = "<table class=listagem width=100%>
			 <tr>
				<td class=SubTituloCentro>Acesso da Equipe do MEC</td>
			 </tr>
			 </table>";
	

monta_titulo( "MEC", $subtitulo);

if(!$_REQUEST['aba']) $_REQUEST['aba'] = "configuracoes";

$abaativa = "/sisindigena2/sisindigena2.php?modulo=principal/mec/mec&acao=A&aba=".$_REQUEST['aba'];

montaAbasSisindigena('mec', $abaativa);

include $_REQUEST['aba'].".inc";

?>
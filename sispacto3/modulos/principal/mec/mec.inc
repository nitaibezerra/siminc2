<?
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

include APPRAIZ ."includes/workflow.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}

	
include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto3.js"></script>';

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

$abaativa = "/sispacto3/sispacto3.php?modulo=principal/mec/mec&acao=A&aba=".$_REQUEST['aba'];

montaAbasSispacto('mec', $abaativa);

if(is_file(APPRAIZ_SISPACTO.'mec/'.$_REQUEST['aba'].'.inc')) {
	include $_REQUEST['aba'].".inc";
} else {
	echo "<p align=center><b>Página não encontrada.</b></p>";
}


?>
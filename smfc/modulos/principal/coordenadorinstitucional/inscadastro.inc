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

// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);

	
$subtitulo = "<table class=listagem width=100%>
			 <tr>
				<td class=\"SubTitulodireita\" width=\"10%\"><b>Nome:</b></td>
				<td>".$_SESSION['smfc']['coordenadorinstitucional']['iusnome']."</td>
				<td class=\"SubTitulodireita\" width=\"10%\"><b>Perfil:</b></td>
				<td>".$_SESSION['smfc']['coordenadorinstitucional']['pfldsc']."</td>
				<td class=\"SubTitulodireita\" width=\"10%\"><b>Instituição:</b></td>
				<td>".$_SESSION['smfc']['coordenadorinstitucional']['instituicao']."</td>
			 </tr>
			 </table>";

monta_titulo( "Coordenador Institucional", $subtitulo);

if($_SESSION['smfc']['coordenadorinstitucional']['iusid']) :

	if(!$_REQUEST['aba']) $_REQUEST['aba'] = "principal";
	
	$abaativa = "/smfc/smfc.php?modulo=principal/coordenadorinstitucional/inscadastro&acao=A&aba=".$_REQUEST['aba'];

	montaAbasSmfc('inscadastro', $abaativa);
	
	include $_REQUEST['aba'].".inc";
	
else :
?>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem">
	<tr>
	  <td class="SubTituloCentro">moommoommooms</td>
	</tr>
</table>
<?
endif;
?>
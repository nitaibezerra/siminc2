<?
include APPRAIZ ."includes/workflow.php";

include "_funcoes_coordenadorestadual.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";

echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>';
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>'; 
echo '<script language="javascript" type="text/javascript" src="./js/escolaterra.js"></script>';

echo "<br>";

// Aba Principal
$db->cria_aba($abacod_tela,$url,$parametros);

$subtitulo = "<table class=listagem width=100%>
			 <tr>
				<td class=SubTituloCentro>".$_SESSION['escolaterra']['coordenadorestadual']['descricao']." - ".$_SESSION['escolaterra']['coordenadorestadual']['iusnome']."</td>
			 </tr>
			 </table>";
	

monta_titulo( "Coordenador Estadual", $subtitulo);

if($_SESSION['escolaterra']['coordenadorestadual']['ufpid']) :

	if(!$_REQUEST['aba']) $_REQUEST['aba'] = "principal";
	
	$abaativa = "/escolaterra/escolaterra.php?modulo=principal/coordenadorestadual/coordenadorestadual&acao=A&aba=".$_REQUEST['aba'];

	montaAbasEscolaTerra('coordenadorestadual', $abaativa);
	
	include $_REQUEST['aba'].".inc";
	
else :
?>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="tabela">
	<tr>
	  <td class="SubTituloCentro">Perfil não associado com UFs</td>
	</tr>
</table>
<?
endif;
?>
<?
include APPRAIZ ."includes/workflow.php";
include "_funcoes_escola.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}

	
include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>';

echo "<br>";

// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);

if($_SESSION['sismedio']['gestorescola']['usucpf']!=$_SESSION['usucpf']) {
	
	$sql = "SELECT lemcpfgestor, lemcodigoinep FROM sismedio.listaescolasensinomedio WHERE lemcpfgestor='".$_SESSION['usucpf']."' ORDER BY lemcodigoinep";

	$escolaensinomedio = $db->pegaLinha($sql);
	
	if($escolaensinomedio) {
		carregarGestorEscola(array("cpf"=>$escolaensinomedio['lemcpfgestor'],"codigoinep"=>$escolaensinomedio['lemcodigoinep']));
	}
	
}

if($_SESSION['sismedio']['gestorescola']['descricao']) {
	
	$subtitulo = "<table class=listagem width=100%>
				 <tr>
					<td class=SubTituloCentro>".$_SESSION['sismedio']['gestorescola']['descricao']."</td>
				 </tr>
				 </table>";
		
	
	monta_titulo( "Gestor da Escola", $subtitulo);
	
}
	
if($_SESSION['sismedio']['gestorescola']['usucpf']) : 

	if(!$_REQUEST['aba']) $_REQUEST['aba'] = "principal";
	
	$abaativa = "/sismedio/sismedio.php?modulo=principal/escola/escola&acao=A&aba=".$_REQUEST['aba'];

	montaAbasSisMedio('gestorescola', $abaativa);
	
	if(is_file(APPRAIZ_SISMEDIO.'escola/'.$_REQUEST['aba'].".inc")) {
		include $_REQUEST['aba'].".inc"; 
	} else {
		$al = array("location"=>"sismedio.php?modulo=principal/escola/escola&acao=A&aba=principal");
		alertlocation($al);
	}
	
else :
?>
<table align="center" width="95%" border="0" cellpadding="3" cellspacing="1" class="listagem">
	<tr>
	  <td align="center"><img src="../imagens/AtencaoP.gif" align="absmiddle"> Não foi encontrado associação do Gestor com Escola.</td>
	</tr>
</table>
<?
endif;
?>
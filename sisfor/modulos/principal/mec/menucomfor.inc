<?
include APPRAIZ ."includes/workflow.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

	
include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sisfor.js"></script>';

echo "<br>";

// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);


$subtitulo = "<table class=listagem width=100%>
		 <tr>
			<td class=SubTituloCentro>Acesso do COMFOR</td>
		 </tr>
		 </table>";


monta_titulo( "COMFOR", $subtitulo);



$menu[] = array("id" => 2, "descricao" => "Análise das avaliações", "link" => "sisfor.php?modulo=principal/mec/menucomfor&acao=".$_REQUEST['acao']."&aba=analise");
$menu[] = array("id" => 3, "descricao" => "Relatório Mensal", "link" => "sisfor.php?modulo=principal/mec/menucomfor&acao=".$_REQUEST['acao']."&aba=relatoriomensal");
$menu[] = array("id" => 4, "descricao" => "Análise dos Relatórios Finais", "link" => "sisfor.php?modulo=principal/mec/menucomfor&acao=".$_REQUEST['acao']."&aba=analiserelatoriofinal");


if(!$_REQUEST['aba']) $_REQUEST['aba'] = "analise";
	
switch($_REQUEST['aba']) {
	case 'analisedetalhada':
		$abaAtiva = "sisfor.php?modulo=principal/mec/menucomfor&acao={$_REQUEST['acao']}&aba=analise";
		break;
	default:
		$abaAtiva = "sisfor.php?modulo=principal/mec/menucomfor&acao={$_REQUEST['acao']}&aba={$_REQUEST['aba']}";
}

echo montarAbasArray($menu, $abaAtiva);

include $_REQUEST['aba'].".inc";

?>
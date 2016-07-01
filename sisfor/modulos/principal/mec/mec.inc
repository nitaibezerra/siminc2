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
			<td class=SubTituloCentro>Acesso da Equipe do MEC</td>
		 </tr>
		 </table>";


monta_titulo( "MEC", $subtitulo);


if(!$_REQUEST['aba']) $_REQUEST['aba'] = "configuracoes";

$menu = array(
    0 => array("id" => 1, "descricao" => "Períodos de referência", "link" => "sisfor.php?modulo=principal/mec/mec&acao={$_REQUEST['acao']}&aba=configuracoes"),
    1 => array("id" => 2, "descricao" => "Análise das avaliações", "link" => "sisfor.php?modulo=principal/mec/mec&acao={$_REQUEST['acao']}&aba=analise"),
    2 => array("id" => 3, "descricao" => "Alteração de projeto", "link" => "sisfor.php?modulo=principal/mec/mec&acao={$_REQUEST['acao']}&aba=alterarprojeto"),
    3 => array("id" => 4, "descricao" => "Aprovar nomes", "link" => "sisfor.php?modulo=principal/mec/mec&acao={$_REQUEST['acao']}&aba=aprovarnomes"),
    4 => array("id" => 5, "descricao" => "Relatórios COMFOR", "link" => "sisfor.php?modulo=principal/mec/mec&acao=".$_REQUEST['acao']."&aba=listarelatoriomensal"),
    5 => array("id" => 6, "descricao" => "Análise dos Relatórios Finais", "link" => "sisfor.php?modulo=principal/mec/mec&acao=".$_REQUEST['acao']."&aba=analiserelatoriofinal"),
    6 => array("id" => 7, "descricao" => "Reenviar pagamentos", "link" => "sisfor.php?modulo=principal/mec/mec&acao=".$_REQUEST['acao']."&aba=reenviarpagamentos")
);

switch($_REQUEST['aba']) {
	case 'analisedetalhada':
		$abaAtiva = "sisfor.php?modulo=principal/mec/mec&acao={$_REQUEST['acao']}&aba=analise";
		break;
	default:
		$abaAtiva = "sisfor.php?modulo=principal/mec/mec&acao={$_REQUEST['acao']}&aba={$_REQUEST['aba']}";
}


echo montarAbasArray($menu, $abaAtiva);

include $_REQUEST['aba'].".inc";

?>
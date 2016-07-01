<?

$pesquis = carregarDadosIdentificacaoUsuario(array("pflcod"=>PFL_PESQUISADOR,"iusd" => $_SESSION['sisindigena2']['pesquisador']['iusd']));

$mensagemalert	= "Dados do Pesquisador salvos com sucesso";
$sis 	  		= "pesquisador";
$goto_ant 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= false;

if($pesquis) {
	$pesquis = current($pesquis);
	extract($pesquis);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISINDIGENA."dadosidentificacaousuario.inc";

?>
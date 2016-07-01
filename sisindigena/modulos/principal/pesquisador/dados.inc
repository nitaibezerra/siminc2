<?

$pesquis = carregarDadosIdentificacaoUsuario(array("pflcod"=>PFL_PESQUISADOR,"iusd" => $_SESSION['sisindigena']['pesquisador']['iusd']));

$mensagemalert	= "Dados do Pesquisador salvos com sucesso";
$sis 	  		= "pesquisador";
$goto_ant 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= false;

if($pesquis) {
	$pesquis = current($pesquis);
	extract($pesquis);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISINDIGENA."dadosidentificacaousuario.inc";

?>
<?

$conteu = carregarDadosIdentificacaoUsuario(array("pflcod"=>PFL_CONTEUDISTA,"iusd" => $_SESSION['sisindigena']['conteudista']['iusd']));

$mensagemalert	= "Dados do Coordenador Local salvos com sucesso";
$sis 	  		= "conteudista";
$goto_ant 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= false;

if($conteu) {
	$conteu = current($conteu);
	extract($conteu);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISINDIGENA."dadosidentificacaousuario.inc";

?>
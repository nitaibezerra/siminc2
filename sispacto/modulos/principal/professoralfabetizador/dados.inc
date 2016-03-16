<?
$profAlf = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sispacto']['professoralfabetizador']['uncid'],"pflcod"=>PFL_PROFESSORALFABETIZADOR,"iusd" => $_SESSION['sispacto']['professoralfabetizador']['iusd']));

$mensagemalert	= "Dados do Professor Alfabetizador salvos com sucesso";
$sis 	  		= "professoralfabetizador";
$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliacaocomplementar";

if($profAlf) {
	$profAlf = current($profAlf);
	extract($profAlf);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISPACTO."dadosidentificacaousuario.inc";

?>
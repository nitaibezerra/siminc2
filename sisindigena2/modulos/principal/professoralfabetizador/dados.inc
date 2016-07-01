<?
$proalf = carregarDadosIdentificacaoUsuario(array("pflcod"=>PFL_PROFESSORALFABETIZADOR,"iusd" => $_SESSION['sisindigena2']['professoralfabetizador']['iusd']));

$mensagemalert	= "Dados do Professor Alfabetizador salvos com sucesso";
$sis 	  		= "professoralfabetizador";
$goto_ant 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= false;

if($proalf) {
	$proalf = current($proalf);
	extract($proalf);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISINDIGENA."dadosidentificacaousuario.inc";

?>
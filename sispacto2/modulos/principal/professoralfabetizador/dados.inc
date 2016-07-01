<?
$profAlf = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sispacto2']['professoralfabetizador']['uncid'],"pflcod"=>PFL_PROFESSORALFABETIZADOR,"iusd" => $_SESSION['sispacto2']['professoralfabetizador']['iusd']));

$mensagemalert	= "Dados do Professor Alfabetizador salvos com sucesso";
$sis 	  		= "professoralfabetizador";

if($profAlf) {
	$profAlf = current($profAlf);
	extract($profAlf);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISPACTO."dadosidentificacaousuario.inc";

?>
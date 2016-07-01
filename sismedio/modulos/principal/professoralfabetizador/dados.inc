<?
$profAlf = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sismedio']['professoralfabetizador']['uncid'],"pflcod"=>PFL_PROFESSORALFABETIZADOR,"iusd" => $_SESSION['sismedio']['professoralfabetizador']['iusd']));

$mensagemalert	= "Dados do Professor salvos com sucesso";
$sis 	  		= "professoralfabetizador";

if($profAlf) {
	$profAlf = current($profAlf);
	extract($profAlf);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISMEDIO."dadosidentificacaousuario.inc";

?>
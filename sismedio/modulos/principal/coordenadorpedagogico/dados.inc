<?
$profAlf = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sismedio']['coordenadorpedagogico']['uncid'],"pflcod"=>PFL_COORDENADORPEDAGOGICO,"iusd" => $_SESSION['sismedio']['coordenadorpedagogico']['iusd']));

$mensagemalert	= "Dados do Professor salvos com sucesso";
$sis 	  		= "coordenadorpedagogico";

if($profAlf) {
	$profAlf = current($profAlf);
	extract($profAlf);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISMEDIO."dadosidentificacaousuario.inc";

?>
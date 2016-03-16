<?
$oriEst = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sispacto2']['orientadorestudo']['uncid'],"pflcod"=>PFL_ORIENTADORESTUDO,"iusd" => $_SESSION['sispacto2']['orientadorestudo']['iusd']));

$mensagemalert	= "Dados do Orientador de Estudo salvos com sucesso";
$sis 	  		= "orientadorestudo";

if($oriEst) {
	$oriEst = current($oriEst);
	extract($oriEst);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISPACTO."dadosidentificacaousuario.inc";

?>
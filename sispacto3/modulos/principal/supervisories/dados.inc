<?
$supIES = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sispacto3']['supervisories']['uncid'],"pflcod"=>PFL_SUPERVISORIES,"iusd" => $_SESSION['sispacto3']['supervisories']['iusd']));

$mensagemalert	= "Dados do Supervisor da IES salvos com sucesso";
$sis 	  		= "supervisories";

if($supIES) {
	$supIES = current($supIES);
	extract($supIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISPACTO."dadosidentificacaousuario.inc";

?>
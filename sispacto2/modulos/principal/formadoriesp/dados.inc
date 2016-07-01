<?
$formIES = carregarDadosIdentificacaoUsuario(array("uncid"  => $_SESSION['sispacto2']['formadoriesp']['uncid'],
												   "pflcod" => PFL_FORMADORIESP, 
												   "iusd"   => $_SESSION['sispacto2']['formadoriesp']['iusd']));

$mensagemalert	= "Dados do Formador da IES salvos com sucesso";
$sis 	  		= "formadoriesp";

if($formIES) {
	$formIES = current($formIES);
	extract($formIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISPACTO."dadosidentificacaousuario.inc";

?>
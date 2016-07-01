<?
$formIES = carregarDadosIdentificacaoUsuario(array("uncid"  => $_SESSION['sispacto3']['formadories']['uncid'],
												   "pflcod" => PFL_FORMADORIES, 
												   "iusd"   => $_SESSION['sispacto3']['formadories']['iusd']));

$mensagemalert	= "Dados do Formador da IES salvos com sucesso";
$sis 	  		= "formadories";

if($formIES) {
	$formIES = current($formIES);
	extract($formIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISPACTO."dadosidentificacaousuario.inc";

?>
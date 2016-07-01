<?
$formIES = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sispacto']['formadories']['uncid'],"pflcod"=>PFL_FORMADORIES,"iusd" => $_SESSION['sispacto']['formadories']['iusd']));

$mensagemalert	= "Dados do Formador da IES salvos com sucesso";
$sis 	  		= "formadories";
$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";

if($formIES) {
	$formIES = current($formIES);
	extract($formIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISPACTO."dadosidentificacaousuario.inc";

?>
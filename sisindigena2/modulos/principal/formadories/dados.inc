<?
$formIES = carregarDadosIdentificacaoUsuario(array("pflcod"=>PFL_FORMADORIES,"iusd" => $_SESSION['sisindigena2']['formadories']['iusd']));

$mensagemalert	= "Dados do Formador da IES salvos com sucesso";
$sis 	  		= "formadories";
$goto_ant 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= false;

if($formIES) {
	$formIES = current($formIES);
	extract($formIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISINDIGENA."dadosidentificacaousuario.inc";

?>
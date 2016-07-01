<?
$formIES = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sismedio']['formadorregional']['uncid'],"pflcod"=>PFL_FORMADORREGIONAL,"iusd" => $_SESSION['sismedio']['formadorregional']['iusd']));

$mensagemalert	= "Dados do Formador salvos com sucesso";
$sis 	  		= "formadorregional";
$goto_ant 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";

if($formIES) {
	$formIES = current($formIES);
	extract($formIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISMEDIO."dadosidentificacaousuario.inc";

?>
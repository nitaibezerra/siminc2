<?
$supIES = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sispacto']['supervisories']['uncid'],"pflcod"=>PFL_SUPERVISORIES,"iusd" => $_SESSION['sispacto']['supervisories']['iusd']));

$mensagemalert	= "Dados do Supervisor da IES salvos com sucesso";
$sis 	  		= "supervisories";
$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";

if($supIES) {
	$supIES = current($supIES);
	extract($supIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISPACTO."dadosidentificacaousuario.inc";

?>
<?
$supIES = carregarDadosIdentificacaoUsuario(array("pflcod"=>PFL_SUPERVISORIES,"iusd" => $_SESSION['sisindigena']['supervisories']['iusd']));

$mensagemalert	= "Dados do Supervisor da IES salvos com sucesso";
$sis 	  		= "supervisories";
$goto_ant 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=definirequipe";

if($supIES) {
	$supIES = current($supIES);
	extract($supIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISINDIGENA."dadosidentificacaousuario.inc";

?>
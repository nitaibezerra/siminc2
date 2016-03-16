<?
$supIES = carregarDadosIdentificacaoUsuario(array("uncid"  => $_SESSION['sismedio']['supervisories']['uncid'],
												  "pflcod" => PFL_SUPERVISORIES,
												  "iusd"   => $_SESSION['sismedio']['supervisories']['iusd']));

$mensagemalert	= "Dados do Supervisor da IES salvos com sucesso";
$sis 	  		= "supervisories";
$goto_ant 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";

if($supIES) {
	$supIES = current($supIES);
	extract($supIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISMEDIO."dadosidentificacaousuario.inc";

?>
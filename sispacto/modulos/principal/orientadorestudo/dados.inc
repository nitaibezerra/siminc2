<?
$oriEst = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sispacto']['orientadorestudo']['uncid'],"pflcod"=>PFL_ORIENTADORESTUDO,"iusd" => $_SESSION['sispacto']['orientadorestudo']['iusd']));

$mensagemalert	= "Dados do Orientador de Estudo salvos com sucesso";
$sis 	  		= "orientadorestudo";
$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";

if($oriEst) {
	$oriEst = current($oriEst);
	extract($oriEst);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISPACTO."dadosidentificacaousuario.inc";

?>
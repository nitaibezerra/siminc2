<?
$oriEst = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sismedio']['orientadorestudo']['uncid'],"pflcod"=>PFL_ORIENTADORESTUDO,"iusd" => $_SESSION['sismedio']['orientadorestudo']['iusd']));

$mensagemalert	= "Dados do Orientador de Estudo salvos com sucesso";
$sis 	  		= "orientadorestudo";
$goto_ant 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";

if($oriEst) {
	$oriEst = current($oriEst);
	extract($oriEst);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISMEDIO."dadosidentificacaousuario.inc";

?>
<?

$cooLocal = carregarDadosIdentificacaoUsuario(array("pflcod"=>PFL_COORDENADORLOCAL,"iusd" => $_SESSION['sisindigena']['coordenadorlocal']['iusd']));

$mensagemalert	= "Dados do Coordenador Local salvos com sucesso";
$sis 	  		= "coordenadorlocal";
$goto_ant 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= false;

if($cooLocal) {
	$cooLocal = current($cooLocal);
	extract($cooLocal);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISINDIGENA."dadosidentificacaousuario.inc";

?>
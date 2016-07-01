<?
$coordAdjIES = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sispacto2']['coordenadoradjuntoies']['uncid'],"pflcod"=>PFL_COORDENADORADJUNTOIES,"iusd" => $_SESSION['sispacto2']['coordenadoradjuntoies']['iusd']));

$mensagemalert	= "Dados do Coordenador Adjunto da IES salvos com sucesso";
$sis 	  		= "coordenadoradjuntoies";
$goto_ant 		= "sispacto2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sispacto2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sispacto2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";

if($coordAdjIES) {
	$coordAdjIES = current($coordAdjIES);
	extract($coordAdjIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISPACTO."dadosidentificacaousuario.inc";

?>
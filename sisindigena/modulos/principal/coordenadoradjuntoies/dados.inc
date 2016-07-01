<?
$coordAdjIES = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sisindigena']['coordenadoradjuntoies']['uncid'],"pflcod"=>PFL_COORDENADORADJUNTOIES,"iusd" => $_SESSION['sisindigena']['coordenadoradjuntoies']['iusd']));

$mensagemalert	= "Dados do Coordenador Adjunto da IES salvos com sucesso";
$sis 	  		= "coordenadoradjuntoies";
$goto_ant 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=projetopedagogico";

if($coordAdjIES) {
	$coordAdjIES = current($coordAdjIES);
	extract($coordAdjIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISINDIGENA."dadosidentificacaousuario.inc";

?>
<?
$coordAdjIES = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sispacto3']['coordenadoradjuntoies']['uncid'],"pflcod"=>PFL_COORDENADORADJUNTOIES,"iusd" => $_SESSION['sispacto3']['coordenadoradjuntoies']['iusd']));

$mensagemalert	= "Dados do Coordenador Adjunto da IES salvos com sucesso";
$sis 	  		= "coordenadoradjuntoies";

if($coordAdjIES) {
	$coordAdjIES = current($coordAdjIES);
	extract($coordAdjIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISPACTO."dadosidentificacaousuario.inc";

?>
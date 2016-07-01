<?
$coordAdjIES = carregarDadosIdentificacaoUsuario(array("uncid"  => $_SESSION['sismedio']['coordenadoradjuntoies']['uncid'],
													   "pflcod" => PFL_COORDENADORADJUNTOIES,
													   "iusd"   => $_SESSION['sismedio']['coordenadoradjuntoies']['iusd']));

$mensagemalert	= "Dados do Coordenador Adjunto da IES salvos com sucesso";
$sis 	  		= "coordenadoradjuntoies";
$goto_ant 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";

if($coordAdjIES) {
	$coordAdjIES = current($coordAdjIES);
	extract($coordAdjIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISMEDIO."dadosidentificacaousuario.inc";

?>
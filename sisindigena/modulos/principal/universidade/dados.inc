<?
$coordIES = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sisindigena']['universidade']['uncid'],"pflcod"=>PFL_COORDENADORIES));

$mensagemalert	= "Dados do Coordenador da IES salvos com sucesso";
$sis 	  		= "universidade";
	
$goto_ant 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=orcamento";

if($coordIES) {
	$coordIES = current($coordIES);
	extract($coordIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISINDIGENA."dadosidentificacaousuario.inc";

?>
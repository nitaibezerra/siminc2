<?
$coordIES = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sispacto']['universidade']['uncid'],"pflcod"=>PFL_COORDENADORIES));

$mensagemalert	= "Dados do Coordenador da IES salvos com sucesso";
$sis 	  		= "universidade";
if($_REQUEST['modulo']=='principal/coordenadorlocal/coordenadorlocalexecucao') {
	$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/coordenadorlocalexecucao&acao=A&aba=principal";
	$goto_sal 		= "sispacto.php?modulo=principal/{$sis}/coordenadorlocalexecucao&acao=A&aba=dados";
	$goto_pro 		= "sispacto.php?modulo=principal/{$sis}/coordenadorlocalexecucao&acao=A&aba=gerenciarusuario";
} else {
	$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
	$goto_sal 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
	$goto_pro 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados_projeto";
}

if($coordIES) {
	$coordIES = current($coordIES);
	extract($coordIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISPACTO."dadosidentificacaousuario.inc";

?>
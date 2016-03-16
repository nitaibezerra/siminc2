<?
$coordIES = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sismedio']['universidade']['uncid'],"pflcod"=>PFL_COORDENADORIES));

$mensagemalert	= "Dados do Coordenador da IES salvos com sucesso";
$sis 	  		= "universidade";
if($_REQUEST['modulo']=='principal/coordenadorlocal/universidadeexecucao') {
	$goto_ant 		= "sismedio.php?modulo=principal/{$sis}/universidadeexecucao&acao=A&aba=principal";
	$goto_sal 		= "sismedio.php?modulo=principal/{$sis}/universidadeexecucao&acao=A&aba=dados";
	$goto_pro 		= "sismedio.php?modulo=principal/{$sis}/universidadeexecucao&acao=A&aba=gerenciarusuario";
} else {
	$goto_ant 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
	$goto_sal 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
	$goto_pro 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados_projeto";
}

if($coordIES) {
	$coordIES = current($coordIES);
	extract($coordIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISMEDIO."dadosidentificacaousuario.inc";

?>
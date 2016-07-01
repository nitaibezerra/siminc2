<?
$usuario = carregarDadosIdentificacaoUsuario(array('iusid'=>$_SESSION['smfc']['coordenadorinstitucional']['iusid'],"pflcod"=>PFL_COORDENADOR_INSTITUCIONAL));

$mensagemalert	= "Dados do Cooordenador do Pacto salvos com sucesso";
$sis	  		= "coordenadorlocal";

if($_REQUEST['modulo']=='principal/coordenadorlocal/coordenadorlocalexecucao') {
	$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/coordenadorlocalexecucao&acao=A&aba=principal";
	$goto_sal 		= "sispacto.php?modulo=principal/{$sis}/coordenadorlocalexecucao&acao=A&aba=dados";
	$goto_pro 		= "sispacto.php?modulo=principal/{$sis}/coordenadorlocalexecucao&acao=A&aba=gerenciarusuario";
} else {
	$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
	$goto_sal 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
	$goto_pro 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=orientadorestudo";
}


if($usuario) {
	$usuario = current($usuario);
	extract($usuario);
}
if($iusid) include_once APPRAIZ_SMFC."dadosidentificacaousuario.inc";
else echo "<p align=center>Coordenador Local não esta cadastrado. Por favor, solicite para o responsável (Equipe Municipal - Aprovação) deste município o cadastramento do Coordenador Local</p>";

?>
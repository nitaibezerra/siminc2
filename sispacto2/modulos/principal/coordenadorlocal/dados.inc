<?
$coordlocal = carregarDadosIdentificacaoUsuario(array("picid"=>$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid'],"pflcod"=>PFL_COORDENADORLOCAL,"iusd"=>$_SESSION['sispacto2']['coordenadorlocal']['iusd']));

$mensagemalert	= "Dados do Cooordenador do Pacto salvos com sucesso";
$sis	  		= "coordenadorlocal";

if($_REQUEST['modulo']=='principal/coordenadorlocal/coordenadorlocalexecucao') {
	$goto_ant 		= "sispacto2.php?modulo=principal/{$sis}/coordenadorlocalexecucao&acao=A&aba=principal";
	$goto_sal 		= "sispacto2.php?modulo=principal/{$sis}/coordenadorlocalexecucao&acao=A&aba=dados";
	$goto_pro 		= "sispacto2.php?modulo=principal/{$sis}/coordenadorlocalexecucao&acao=A&aba=gerenciarusuario";
} else {
	$goto_ant 		= "sispacto2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
	$goto_sal 		= "sispacto2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
	$goto_pro 		= "sispacto2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=orientadorestudo";
}


if($coordlocal) {
	$coordlocal = current($coordlocal);
	extract($coordlocal);
}

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORIES,$perfis)) {
	$consulta = true;
}


if($iusd) include_once APPRAIZ_SISPACTO."dadosidentificacaousuario.inc";
else echo "<p align=center>Coordenador Local não esta cadastrado. Por favor, solicite para o responsável (Equipe Municipal - Aprovação) deste município o cadastramento do Coordenador Local</p>";

?>
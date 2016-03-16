<?
$sql_equipe = sqlEquipeCoordenadorLocal(array("picid"=>$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']));

$pflcod_gerenciador = PFL_COORDENADORLOCAL;
$mensagemalert	= "Dados do Coordenador Local salvos com sucesso";
$sis 	  		= "coordenadorlocal";
$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/coordenadorlocalexecucao&acao=A&aba=dados";
$goto_sal 		= "sispacto.php?modulo=principal/{$sis}/coordenadorlocalexecucao&acao=A&aba=gerenciarusuario";
$goto_pro 		= "sispacto.php?modulo=principal/{$sis}/coordenadorlocalexecucao&acao=A&aba=avaliarusuario";


include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

?>
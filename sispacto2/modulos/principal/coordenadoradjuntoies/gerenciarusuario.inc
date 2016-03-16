<?
$sql_equipe = sqlEquipeCoordenadorAdjunto(array("uncid"=>$_SESSION['sispacto2']['coordenadoradjuntoies']['uncid']));

$pflcod_gerenciador = PFL_COORDENADORADJUNTOIES;
$mensagemalert	= "Dados do Coordenador da IES salvos com sucesso";
$sis 	  		= "coordenadoradjuntoies";
$goto_ant 		= "sispacto2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sispacto2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sispacto2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario";


include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

?>
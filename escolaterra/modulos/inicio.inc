<?

$perfis = pegaPerfilGeral();

if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis)) {
	
	die("<script>window.location='escolaterra.php?modulo=centralacompanhamento&acao=A';</script>");
	
} elseif(in_array(PFL_COORDENADORESTADUAL,$perfis)) {
	
	$ufpid = $db->pegaUm("SELECT i.ufpid FROM escolaterra.identificacaousuario i INNER JOIN escolaterra.tipoperfil t ON i.iusid=t.iusid WHERE i.iuscpf='".$_SESSION['usucpf']."' AND t.pflcod=".PFL_COORDENADORESTADUAL."");
	die("<script>window.location='escolaterra.php?modulo=principal/coordenadorestadual/coordenadorestadual&acao=A&ufpid={$ufpid}&requisicao=carregarCoordenadorEstadual&direcionar=true';</script>");
	
} elseif(in_array(PFL_TUTOR,$perfis)) {
	$iusid = $db->pegaUm("SELECT i.iusid FROM escolaterra.identificacaousuario i INNER JOIN escolaterra.tipoperfil t ON i.iusid=t.iusid WHERE i.iuscpf='".$_SESSION['usucpf']."' AND t.pflcod=".PFL_TUTOR."");
	die("<script>window.location='escolaterra.php?modulo=principal/tutor/tutor&acao=A&iusid={$iusid}&requisicao=carregarTutor&direcionar=true';</script>");
}

include  APPRAIZ."includes/cabecalho.inc";

echo "<br>";

echo '<p align="center">[ NENHUM PERFIL DE TRABALHO FOI ENCONTRADO. ]</p>';

?>
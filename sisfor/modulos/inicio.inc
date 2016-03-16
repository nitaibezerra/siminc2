<?php
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";


$perfis = pegaPerfilGeral();


$sql = "SELECT t.pflcod, CASE WHEN t.pflcod=".PFL_COORDENADOR_INST." THEN 1 ELSE s.sifid END as sifid 
		FROM sisfor.identificacaousuario i 
		INNER JOIN sisfor.tipoperfil t ON t.iusd = i.iusd 
		LEFT JOIN sisfor.sisfor s ON s.sifid = t.sifid OR i.iuscpf = s.usucpf 
		LEFT JOIN sisfor.sisfories se ON se.usucpf = i.iuscpf 
		WHERE i.iuscpf='".$_SESSION['usucpf']."'";

$pp = $db->carregar($sql);

if($pp[0]) {
	foreach($pp as $per) {
		$_CONF[$per['pflcod']] = $per['sifid'];
	}
}

if(in_array(PFL_SUPER_USUARIO,$perfis) || in_array(PFL_ADMINISTRADOR,$perfis) || in_array(PFL_EQUIPE_MEC,$perfis) || in_array(PFL_COORDENADOR_MEC,$perfis)  || in_array(PFL_DIRETOR_MEC,$perfis)) {
	die("<script>window.location.href = 'sisfor.php?modulo=principal/coordenador_curso/analisecurso&acao=A';</script>");
}
elseif((in_array(PFL_COORDENADOR_INST,$perfis) && $_CONF[PFL_COORDENADOR_INST]) || (in_array(PFL_COORDENADOR_ADJUNTO_IES,$perfis) && $_CONF[PFL_COORDENADOR_ADJUNTO_IES]) || (in_array(PFL_COORDENADOR_CURSO,$perfis) && $_CONF[PFL_COORDENADOR_CURSO])) {
	die("<script>window.location.href = 'sisfor.php?modulo=principal/listauniversidade&acao=A';</script>");
}
elseif((in_array(PFL_SUPERVISOR_IES,$perfis) && $_CONF[PFL_SUPERVISOR_IES])) {
    die("<script>window.location.href = 'sisfor.php?modulo=principal/supervisor/supervisor&acao=A';</script>");
}
elseif((in_array(PFL_FORMADOR_IES,$perfis) && $_CONF[PFL_FORMADOR_IES])) {
    die("<script>window.location.href = 'sisfor.php?modulo=principal/formador/formador&acao=A';</script>");
}
elseif((in_array(PFL_PROFESSOR_PESQUISADOR,$perfis) && $_CONF[PFL_PROFESSOR_PESQUISADOR])) {
    die("<script>window.location.href = 'sisfor.php?modulo=principal/professor_pesquisador/professor_pesquisador&acao=A';</script>");
}
elseif((in_array(PFL_TUTOR,$perfis) && $_CONF[PFL_TUTOR])) {
    die("<script>window.location.href = 'sisfor.php?modulo=principal/tutor/tutor&acao=A';</script>");
}
else {
	die("<script>window.location.href = 'sisfor.php?modulo=principal/listauniversidade&acao=A';</script>");
}

?>
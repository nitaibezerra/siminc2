<?
$perfis = pegaPerfilGeral();

if(in_array(PFL_SUPERUSUARIO,$perfis) || in_array(PFL_EQUIPEMEC,$perfis) || in_array(PFL_ADMINISTRADOR,$perfis) || in_array(PFL_CONSULTAMEC,$perfis)) {
	die("<script>window.location='sispacto.php?modulo=centralacompanhamento&acao=A';</script>");
} elseif(in_array(PFL_COORDENADORLOCAL,$perfis)) {
	die("<script>window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_COORDENADORIES,$perfis)) {
	die("<script>window.location='sispacto.php?modulo=principal/universidade/universidade&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_COORDENADORADJUNTOIES,$perfis)) {
	die("<script>window.location='sispacto.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=principal';</script>");
}  elseif(in_array(PFL_SUPERVISORIES,$perfis)) {
	die("<script>window.location='sispacto.php?modulo=principal/supervisories/supervisories&acao=A&aba=principal';</script>");
}  elseif(in_array(PFL_FORMADORIES,$perfis)) {
	die("<script>window.location='sispacto.php?modulo=principal/formadories/formadories&acao=A&aba=principal';</script>");
}  elseif(in_array(PFL_ORIENTADORESTUDO,$perfis)) {
	die("<script>window.location='sispacto.php?modulo=principal/orientadorestudo/orientadorestudo&acao=A&aba=principal';</script>");
}  elseif(in_array(PFL_PROFESSORALFABETIZADOR,$perfis)) {
	die("<script>window.location='sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_EQUIPEMUNICIPALAP,$perfis) || in_array(PFL_EQUIPEESTADUALAP,$perfis)) {
	die("<script>window.location='sispacto.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A';</script>");
} elseif(in_array(PFL_CONSULTAMUNICIPAL,$perfis) || in_array(PFL_CONSULTAESTADUAL,$perfis)) {
	die("<script>window.location='sispacto.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A';</script>");
}
?>
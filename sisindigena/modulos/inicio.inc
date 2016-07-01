<?

$perfis = pegaPerfilGeral();

if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis)) {
	die("<script>window.location='sisindigena.php?modulo=centralacompanhamento&acao=A';</script>");
} elseif(in_array(PFL_PESQUISADOR,$perfis)) {
	die("<script>window.location='sisindigena.php?modulo=principal/pesquisador/pesquisador&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_CONTEUDISTA,$perfis)) {
	die("<script>window.location='sisindigena.php?modulo=principal/conteudista/conteudista&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_COORDENADORLOCAL,$perfis)) {
	die("<script>window.location='sisindigena.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_COORDENADORIES,$perfis)) {
	die("<script>window.location='sisindigena.php?modulo=principal/universidade/universidade&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_COORDENADORADJUNTOIES,$perfis)) {
	die("<script>window.location='sisindigena.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=principal';</script>");
}  elseif(in_array(PFL_SUPERVISORIES,$perfis)) {
	die("<script>window.location='sisindigena.php?modulo=principal/supervisories/supervisories&acao=A&aba=principal';</script>");
}  elseif(in_array(PFL_FORMADORIES,$perfis)) {
	die("<script>window.location='sisindigena.php?modulo=principal/formadories/formadories&acao=A&aba=principal';</script>");
}  elseif(in_array(PFL_ORIENTADORESTUDO,$perfis)) {
	die("<script>window.location='sisindigena.php?modulo=principal/orientadorestudo/orientadorestudo&acao=A&aba=principal';</script>");
}  elseif(in_array(PFL_PROFESSORALFABETIZADOR,$perfis)) {
	die("<script>window.location='sisindigena.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_EQUIPEMUNICIPALAP,$perfis) || in_array(PFL_EQUIPEESTADUALAP,$perfis)) {
	die("<script>window.location='sisindigena.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A';</script>");
} elseif(in_array(PFL_CONSULTAMUNICIPAL,$perfis) || in_array(PFL_CONSULTAESTADUAL,$perfis)) {
	die("<script>window.location='sisindigena.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A';</script>");
}


 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial
    */

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

?>
<br>

<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
	<tr bgcolor="#e7e7e7">
	  <td><h1>Bem-vindo</h1></td>
	</tr>
</table>

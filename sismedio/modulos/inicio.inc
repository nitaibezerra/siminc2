<?

$perfis = pegaPerfilGeral();

if(in_array(PFL_SUPERUSUARIO,$perfis)) {
	die("<script>window.location='sismedio.php?modulo=centralacompanhamento&acao=A';</script>");
} elseif(in_array(PFL_COORDENADORIES,$perfis)) {
	die("<script>window.location='sismedio.php?modulo=principal/universidade/universidade&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_COORDENADORADJUNTOIES,$perfis)) {
	die("<script>window.location='sismedio.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_SUPERVISORIES,$perfis)) {
	die("<script>window.location='sismedio.php?modulo=principal/supervisories/supervisories&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_FORMADORIES,$perfis)) {
	die("<script>window.location='sismedio.php?modulo=principal/formadories/formadories&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_FORMADORREGIONAL,$perfis)) {
		die("<script>window.location='sismedio.php?modulo=principal/formadorregional/formadorregional&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_ORIENTADORESTUDO,$perfis)) {
	die("<script>window.location='sismedio.php?modulo=principal/orientadorestudo/orientadorestudo&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_PROFESSORALFABETIZADOR,$perfis)) {
	die("<script>window.location='sismedio.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_COORDENADORPEDAGOGICO,$perfis)) {
	die("<script>window.location='sismedio.php?modulo=principal/coordenadorpedagogico/coordenadorpedagogico&acao=A&aba=principal';</script>");
} elseif(in_array(PFL_DIRIGENTEESTADUAL,$perfis)) {
	die("<script>window.location='sismedio.php?modulo=principal/escola/listaescola&acao=A';</script>");
} elseif(in_array(PFL_CONSULTAMEC,$perfis)) {
	die("<script>window.location='sismedio.php?modulo=centralacompanhamento&acao=A';</script>");
} elseif(in_array(PFL_GESTORESCOLA,$perfis)) {
	die("<script>window.location='sismedio.php?modulo=principal/escola/escola&acao=A&aba=principal';</script>");
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

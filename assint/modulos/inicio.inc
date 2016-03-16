<?
 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial
    */

$perfis = recuperaPerfil();

if(in_array(PERFIL_PEC_G, $perfis)){
	redir('?modulo=principal/PEC_G&acao=A', '');
} else {
	redir('?modulo=principal/listPrograma&acao=A', '');
}
?>
<br>
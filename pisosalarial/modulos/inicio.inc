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

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
$arPerfil = recuperarArrayPerfis($_SESSION['usucpf']);

if(possuiPerfil(array(PERFIL_ADMINISTRADOR, PERFIL_CONSULTA_GERAL))){
    header("Location: pisosalarial.php?modulo=principal/listaMunicipios&acao=A");
}else{
    header("Location: pisosalarial.php?modulo=principal/gestaoderecursos/adicionar&acao=A");
}
?>
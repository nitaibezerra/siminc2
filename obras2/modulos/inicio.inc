<?
//if ( $_SESSION['usucpf'] == '' ){
//	if ( !is_dir( APPRAIZ. "/arquivos/obras2/vistoriaEmp" ) ){
//		mkdir(APPRAIZ. "/arquivos/obras2/vistoriaEmp", 0777);
//	}
//}
if ( $_SESSION['obras2']['acesso'] !== false || $_SESSION['obras2']['acessocpf'] != $_SESSION['usucpf'] ){
	verificaAcessoEmOrgid();
	redirecionaTelaInicial();
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
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
	<tr bgcolor="">
	  <td height="400" valign="top">
	  	<h1>Bem-vindo ao sistema!</h1>
	  	<br>
	  	<h3>Usuário sem permissão de acesso as telas do sistema.</h3>
	  </td>
	</tr>
</table>

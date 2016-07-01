<?php
 /**
  * Sistema Simec 
  * Setor responsvel: MEC 
  * Desenvolvedor: Equipe Consultores Simec
  * Programador: Gustavo Fernandes (gustavo.guarda@mec.gov.br)
  * Mdulo:inicio.inc
  * Finalidade: permitir abrir a página inicial
  * 
  */

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

unset($_SESSION['tarefa']['filtroArvore']);

# Array de perfis que veem dois módulos
$arPerfisVerModulos = array(TAREFA_PERFIL_SUPER_USUARIO, 
					 	    TAREFA_PERFIL_GERENTE,
					 	    TAREFA_PERFIL_TECNICO						 	  
						    );
$_SESSION['tarefa']['boPerfilSuperUsuario'] = false;
if( possuiPerfil(TAREFA_PERFIL_SUPER_USUARIO) ){
	$_SESSION['tarefa']['boPerfilSuperUsuario'] = true;	
} elseif((possuiPerfil(TAREFA_PERFIL_GERENTE) && possuiPerfil(TAREFA_PERFIL_TECNICO)) 
	|| (possuiPerfil(TAREFA_PERFIL_GERENTE) )){
	if($unaid = pegaUnidadeAssociada(TAREFA_PERFIL_GERENTE)){
		$_SESSION['tarefa']['unaid'] = $unaid;
	} else {
		mensagemAcossiacao();
	}
	$_SESSION['tarefa']['setorUsuarioLogado'] = pegaSetorUsuarioLogado(TAREFA_PERFIL_GERENTE);
	$_SESSION['tarefa']['boPerfilGerente'] = 'S';	
} elseif(possuiPerfil(TAREFA_PERFIL_TECNICO)) {
	if($unaid = pegaUnidadeAssociada(TAREFA_PERFIL_TECNICO)){
		$_SESSION['tarefa']['unaid'] = $unaid;
	} else {
		mensagemAcossiacao();
	}
	$_SESSION['tarefa']['setorUsuarioLogado'] = pegaSetorUsuarioLogado(TAREFA_PERFIL_TECNICO);	
	$_SESSION['tarefa']['boPerfilGerente'] = 'N';	
}
?>
<br>
<script>
	window.location.href = 'tarefa.php?modulo=principal/listaTarefas&acao=A';
</script>
<script language='javascript' type='text/javascript'>

	/**
	 * Redireciona para a página de exclusão
	 * @param id - Chave para exclusão
	 * @return void
	 */
	function redirecionaExcluir(id){

		if(confirm('Deseja excluir o registro?'))
			location.href = '?modulo=sistema/tabelasdeapoio/motivoEstadoConservacao/excluir&acao=A&mecid=' + id;

	}

	/**
	 * Redireciona para a página de edição
	 * @param id - Chave para edição
	 * @return void
	 */
	function redirecionaEditar(id){

		location.href = '?modulo=sistema/tabelasdeapoio/motivoEstadoConservacao/editar&acao=A&mecid=' + id;

	}

</script>

<div class="tituloLista"> Motivos de Estados de Conserva&ccedil;&atilde;o Cadastrados </div>
<?php 

// Se for solicitado a filtragem dos dados
if($_POST['requisicao'] == 'filtrar')
	// filtra
	$oMotivoEstadoConservacao->filtrar($_POST['mecid'], $_POST['mecdsc']);
else
	// exibe todos
	$oMotivoEstadoConservacao->filtrar('', '');
	
?>



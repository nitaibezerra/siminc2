<script language='javascript' type='text/javascript'>

	/**
	 * Redireciona para a página de exclusão
	 * @param id - Chave para exclusão
	 * @return void
	 */
	function redirecionaExcluir(id){

		if(confirm('Deseja excluir o registro?'))
			location.href = '?modulo=sistema/tabelasdeapoio/material/excluir&acao=A&matid=' + id;

	}

	/**
	 * Redireciona para a página de edição
	 * @param id - Chave para edição
	 * @return void
	 */
	function redirecionaEditar(id){

		location.href = '?modulo=sistema/tabelasdeapoio/material/editar&acao=A&matid=' + id;

	}

</script>

<div class="tituloLista"> Materiais Cadastrados </div>
<?php 

// Se for solicitado a filtragem dos dados
if($_POST['requisicao'] == 'filtrar')
	// filtra
	$oMaterial->filtrar($_POST['matid'], $_POST['matdsc'], $_POST['matcodsiasg']);
else
	// exibe todos
	$oMaterial->filtrar('', '', '');
	
?>

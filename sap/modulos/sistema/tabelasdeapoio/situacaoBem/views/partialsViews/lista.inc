<script language='javascript' type='text/javascript'>

	/**
	 * Redireciona para a página de edição
	 * @param id - Chave para edição
	 * @return void
	 */
	function redirecionaEditar(id){
		location.href = '?modulo=sistema/tabelasdeapoio/situacaoBem/editar&acao=A&sbmid=' + id;
	}

</script>

<div class="tituloLista"> Situa&ccedil;&atilde;o do Bem Cadastrados </div>
<?php 

// Se for solicitado a filtragem dos dados
if($_POST['requisicao'] == 'filtrar')
	// filtra
	$oSituacaoBem->filtrar($_POST['sbmcod'], $_POST['sbmdsc']);
else
	// exibe todos
	$oSituacaoBem->filtrar('', '');
	
?>

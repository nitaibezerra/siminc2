<script language='javascript' type='text/javascript'>

	/**
	 * Redireciona para a página de exclusão
	 * @param id - Chave para exclusão
	 * @return void
	 */
	function redirecionaExcluir(id){

		if(confirm('Deseja excluir o registro?'))
			location.href = '?modulo=sistema/tabelasdeapoio/tipoDocumento/excluir&acao=A&tdcid=' + id;

	}

	/**
	 * Redireciona para a página de edição
	 * @param id - Chave para edição
	 * @return void
	 */
	function redirecionaEditar(id){
		location.href = '?modulo=sistema/tabelasdeapoio/tipoDocumento/editar&acao=A&tdcid=' + id;
	}

</script>

<div class="tituloLista"> Tipos de Documento Cadastrados </div>
<?php 

// Se for solicitado a filtragem dos dados
if($_POST['requisicao'] == 'filtrar')
	// filtra
	$oTipoDocumento->filtrar($_POST['tdcid'], $_POST['tdcdsc']);
else
	// exibe todos
	$oTipoDocumento->filtrar('', '');
	
?>



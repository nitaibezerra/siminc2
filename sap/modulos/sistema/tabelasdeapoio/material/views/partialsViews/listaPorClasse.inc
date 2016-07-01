<script type="text/javascript">

	/**
	 * Redireciona para a pagina de exclusao
	 * @param id - Chave para exclusao
	 * @return void
	 */
	function redirecionaExcluir(id){

		if(confirm('Deseja excluir o registro?'))
			location.href = '?modulo=sistema/tabelasdeapoio/material/excluir&acao=A&matid=' + id;

	}

	/**
	 * Redireciona para a pagina de edicao
	 * @param id - Chave para edicao
	 * @return void
	 */
	function redirecionaEditar(id){

		location.href = '?modulo=sistema/tabelasdeapoio/material/editar&acao=A&matid=' + id;

	}

</script>

<?php 

	// Se o código do orgão estiver selecionado
	if(!empty($_POST['catcodclasse']) || $catcodclasse):

		// preenchendo a variavel para enviar para o filtro
		$catcodclasse = empty($catcodclasse) ? $_POST['catcodclasse'] : $catcodclasse;
	
?>

		<div class="tituloLista"> Materiais Cadastrados para esta Classe </div>
		<?php $oMaterial->filtrarPorClasse($catcodclasse); ?>
	
<?php endif; ?>

	


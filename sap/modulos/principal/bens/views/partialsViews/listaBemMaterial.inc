<script type="text/javascript">

	/**
	 * Redireciona para a pagina de exclusao
	 * @param id - Chave para exclusao
	 * @return void
	 */
	function redirecionaExcluir(id){

		if(confirm('Deseja excluir o registro?'))
			location.href = "?modulo=principal/bens/excluirBemMaterial&acao=A&bmtid=" + id + "&benid=<?php echo $benid; ?>";

	}

	/**
	 * Redireciona para a pagina de edicao
	 * @param id - Chave para edicao
	 * @return void
	 */
	function redirecionaEditar(id){
		var benid = $('benid').getValue();
		var benstatus = $('benstatus').getValue();
		if(benstatus == 'I'){
			location.href = '?modulo=principal/bens/adicionarBemMaterial2&acao=A&bmtid='+id+'&benid='+benid;
		}
		else if(benstatus == 'A'){
			location.href = '?modulo=principal/bens/adicionarBemMaterialAtivo2&acao=A&bmtid='+id+'&benid='+benid;
		}

	}

	/**
	 * Redireciona para a pagina de tombamento
	 * @param id - Chave do item
	 * @return void
	 */
	function redirecionaTombamento(id){
		var benid = $('benid').getValue();
		var benstatus = $('benstatus').getValue();
		if(benstatus == 'I'){
			location.href = '?modulo=principal/bens/adicionarBemRgp&acao=A&bmtid='+id+'&benid='+benid;
		}
		else if(benstatus == 'A'){
			location.href = '?modulo=principal/bens/adicionarBemRgpAtivo&acao=A&bmtid='+id+'&benid='+benid;
		}
	}

</script>

<?php 

	// exibe todos os itens do bem
	$oBensMaterial->filtrar($benid,$_POST['pesquisadsc']);
	
?>
<script type="text/javascript">

	/**
	 * Redireciona para a pagina de exclusao
	 * @param rgpid - Chave para exclusao
	 * @return void
	 */
	function redirecionaExcluir(rgpid){

		if(confirm('Deseja excluir o registro?'))
			location.href = "?modulo=principal/bens/excluirBemRgp&acao=A&rgpid=" + rgpid + "&benid=<?php echo $benid; ?>";

	}

	/**
	 * Redireciona para a pagina de edicao
	 * @param rgpid - Id do rgp
	 * @return void
	 */
	function redirecionaEditar(rgpid){

		var bmtid = $('bmtid').getValue();
		var benstatus = $('benstatus').getValue();
		if(benstatus == 'I'){
			location.href = "?modulo=principal/bens/adicionarBemRgp2&acao=A&bmtid="+bmtid+"&rgpid="+rgpid+"&benid=<?php echo $benid; ?>";
		}
		else if(benstatus == 'A'){
			location.href = "?modulo=principal/bens/adicionarBemRgpAtivo2&acao=A&bmtid="+bmtid+"&rgpid="+rgpid+"&benid=<?php echo $benid; ?>";
		}

	}

	/**
	 * Redireciona para a pagina de adicao de rgp para o bem material
	 * @param bmtid - Id do bem material
	 * @return void
	 */
	function redirecionaAdicionar(bmtid){

		location.href = "?modulo=principal/bens/adicionarBemRgp2&acao=A&bmtid=" + bmtid + "&benid=<?php echo $benid; ?>";

	}

</script>
<?php 

	// exibe todos os itens do bem
	$oRgp->filtrar($benid,$bmtid,$_POST['pesquisadsc']);
	
?>
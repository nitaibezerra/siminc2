<link type="text/css" rel="stylesheet" href="./css/default.css" />
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>

/**
 * Redireciona para a edição
 * @name detalharBem
 * @param benid - Id do bem
 * @return void
 */
function detalharBemEmAberto(benid){
	window.location = "?modulo=principal/bens/editarBemAberto&acao=A&benid="+benid;
}


/**
 * Redireciona para a exclusão
 * @name removerBem
 * @param benid - Id do bem
 * @return void
 */
function removerBem(benid){
	window.location = "?modulo=principal/bens/excluirBem&acao=A&procab=true&benid="+benid;
}

 /**
	 * Redireciona para a edição
	 * @name detalharBensBaixa
	 * @param bebid - Id da baixa
	 * @return void
	 */
	function detalharBensBaixa(bebid){
		window.location = "?modulo=principal/baixadebens/editar&acao=A&bebid="+bebid;
	}


	/**
	 * Redireciona para a exclusão
	 * @name removerBensBaixa
	 * @param bebid - Id da baixa
	 * @return void
	 */
	function removerBensBaixa(bebid){
		window.location = "?modulo=principal/baixadebens/excluir&acao=A&procab=true&bebid="+bebid;
	}

    /**
     * Redireciona para a exclusão
     * @name removerMovimentacao
     * @param bebid - Id da baixa
     * @return void
     */
    function removerMovimentacao(mvbid){
        window.location = "?modulo=principal/bensmovimentacao/excluir&acao=A&mvbid="+mvbid;
    }


    /**
	 * Redireciona para a página de edição
	 * @param id - Chave para edição
	 * @param tipo - Tipo da movimentação
	 * @return void
	 */
	function detalharMovimentacaoEmAberto(id,tipo){
		if(tipo == 'G'){
			location.href = '?modulo=principal/bensmovimentacao/adicionarMovimentacaoGeral&acao=A&mvbid='+id;
		}
		else if(tipo == 'S'){
			location.href = '?modulo=principal/bensmovimentacao/adicionarMovimentacaoSiape&acao=A&mvbid='+id;
		}
		else if(tipo == 'L'){
			location.href = '?modulo=principal/bensmovimentacao/adicionarMovimentacaoLocalidade&acao=A&mvbid='+id+'&edicao=S'+'&mvbtipo='+tipo;
		}
	}

</script>

<?php include_once APPSAP . 'modulos/principal/processosabertos/views/partialsViews/lista.inc'; ?>
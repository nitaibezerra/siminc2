<script type="text/javascript">

	/**
	 * Chama o ajax para carregar os dados do fornecedor
	 * @name selecionar
	 * @return void
	 */	
	function selecionar(forcpfcnpj){

		if(forcpfcnpj == ''){
			alert('Selecione um Fornecedor.');
		}else{
			executarScriptPai("carregarFornecedor('"+forcpfcnpj+"')");
			self.close();
		}
	}

</script>



<div class="tituloLista"> Fornecedores Cadastrados </div>

<?php $oFornecedor->filtrarFornecedor($_POST['forcpfcnpj'],$_POST['fortipo'],$_POST['forrazaosocial'],$_POST['fornomefantasia']); // filtra ?>
	

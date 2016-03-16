<script type="text/javascript">

	/**
	 * Recupera opcao selecionada
	 * @name selecionar
	 * @return void
	 */	
	function selecionar(cep){

		if(cep == ''){

			alert('Selecione um CEP.');
			
		}else{

			executarScriptPai("carregarEndereco('"+cep+"')");
			self.close();
			
		}

	}

</script>

<div class="tituloLista"> CEP - Resultado da Pesquisa </div>

<?php 

// Se for solicitado a filtragem dos dados
if($_POST['requisicao'] == 'filtrar')
	// filtra
	$oEndereco->filtrarCep($_POST['estado'], $_POST['logradouro'], $_POST['bairro'], $_POST['cidade']);

	
?>

<div class="tituloLista"> 
	<input type='button' class="botao" name='btnSelecionar' id='btnSelecionar' value='Selecionar' onclick='selecionar()' /> 
</div>
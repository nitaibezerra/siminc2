<script type="text/javascript">

	/**
	 * Recupera opcao selecionada
	 * @name selecionar
	 * @return void
	 */	
	function selecionar(empid){

		if(empid == ''){

			alert('Selecione um Empenho.');
			
		}else{

			executarScriptPai("carregarEmpenho('"+empid+"')");
			self.close();
			
		}

	}

</script>



<div class="tituloLista"> Empenhos Cadastrados </div>

<?php $oEmpenho->filtrarEmpenho($_POST['empnumero'],$_POST['empdata']); // filtra ?>


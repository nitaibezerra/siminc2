<script language='javascript' type='text/javascript'>

	/**
	 * Recupera opção selecionada
	 * @name selecionar
	 * @return void
	 */	
	function selecionar(matid){

		if(matid == ''){

			alert('Selecione um Material.');
			
		}else{

			executarScriptPai("carregarMaterial('"+matid+"')");
			self.close();
			
		}

	}

</script>



<div class="tituloLista"> Lista de Materiais </div>

	<?php $oMaterial->filtrarMaterial($_POST['matid'], $_POST['matdsc'], $_POST['matcodsiasg']); // filtra ?>

<div class="tituloLista"> 
	<input type='button' class="botao" name='btnSelecionar' id='btnSelecionar' value='Selecionar' onclick='javascript:selecionar();' /> 
</div>


